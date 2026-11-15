/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    PhysicsEngine.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:44:35 - nwilliam
**  \brief   Physics engine implementation.
*******************************************************************************/

#include <algorithm> // min, max
#include <cfloat>
#include <sstream>

#include "PhysicsEngine.h"
#include "CPhysics.h"
#include "COrbitalPhysics.h"

#include "CConnectionEmitter.h"
#include "CExplosionEmitter.h"
#include "CRocketEmitter.h"
#include "CFlashEmitter.h"

#include "Engine/Entity.h"
#include "Engine/EntityFactory.h" 
#include "Engine/MessageEngine.h"
#include "Math/CTransform.h"
#include "Math/Constants.h"
#include "Math/SweepCollision.h"
#include "Engine/CParticleEmitter.h"
#include "SolarFlare/Globals.h"
#include "Utilities/Asserts.h"
#include "Utilities/DevConsole.h"
#include "Utilities/Misc.h"
#include "Utilities/SmoothSort.h"
#include "Utilities/Timer.h"


namespace SF {
namespace Physics
{

/*****************************************************************************
  PhysicsEngine

*****************************************************************************/

PhysicsEngine::PhysicsEngine()
{
  SetTimestep( 1.0f / 60.0f * 1000.0f );
  SetIntegrator( VERLET );
  m_integrationTime = 0;

  Globals::entityFactory->CreateNotifier( Engine::ComponentType::PHYSICS, Engine::Notifier::Method< PhysicsEngine, &PhysicsEngine::AddPhysicsEntity >(this) );
  Globals::entityFactory->CreateNotifier( Engine::ComponentType::EMITTER, Engine::Notifier::Method< PhysicsEngine, &PhysicsEngine::AddEmitterEntity >(this) );

  Globals::entityFactory->DeleteNotifier( Engine::ComponentType::PHYSICS, Engine::Notifier::Method< PhysicsEngine, &PhysicsEngine::RemovePhysicsEntity >(this) );
  Globals::entityFactory->DeleteNotifier( Engine::ComponentType::EMITTER, Engine::Notifier::Method< PhysicsEngine, &PhysicsEngine::RemoveEmitterEntity >(this) );

  Globals::entityFactory->Creator( "PHYSICS", Engine::CCreator::Function<Physics::CPhysics::Creator>() );
  Globals::entityFactory->Creator( "ORBITAL_PHYSICS", Engine::CCreator::Function<Physics::COrbitalPhysics::Creator>() );
  
  Globals::entityFactory->Creator( "CONNECTION_EMITTER", Engine::CCreator::Function<Physics::CConnectionEmitter::Creator>() );
  Globals::entityFactory->Creator( "EXPLOSIONEMITTER", Engine::CCreator::Function<Physics::CExplosionEmitter::Creator>() );
  Globals::entityFactory->Creator( "ROCKETEMITTER", Engine::CCreator::Function<Physics::CRocketEmitter::Creator>() );
  Globals::entityFactory->Creator( "FLASHEMITTER", Engine::CCreator::Function<Physics::CFlashEmitter::Creator>() );

  m_pause = false;

  /*m_forceField.Build(
    Vector3(0,0,0),
    Vector3(1,1,1) * 1e5f,
    1e3f,
    VectorField::VectorFunction::Method<PhysicsEngine, &PhysicsEngine::GetGravForce>(this) );*/
}

unsigned int PhysicsEngine::GroupID( const std::string& groupName )
{
  GroupMap::iterator iter = m_groupMap.find( groupName );

  if( iter != m_groupMap.end() )
  {
    return iter->second;
  }
  else
  {
    m_groups.push_back( PhysicsGroup() );

    unsigned int id = (unsigned int)m_groups.size() - 1;

    m_groupMap.insert( GroupMap::value_type(groupName, id) );

    for(unsigned int i = 0; i < m_groups.size(); ++i)
      m_groups[id].collidesWith.push_back( false );

    for(unsigned int i = 0; i < m_groups.size() - 1; ++i)
      m_groups[i].collidesWith.push_back( false );

    return id;
  }
}

void PhysicsEngine::SetGravitySource( unsigned int sourceID, unsigned int groupID, float coefficient )
{
  std::vector<GravitySource>& sources = m_groups[groupID].gravitySources;

  bool exists = false;

  for(unsigned int i = 0; i < sources.size(); ++i)
  {
    if(sources[i].first == sourceID)
    {
      exists = true;
      sources[i].second = coefficient;
      break;
    }
  }

  if( !exists )
    sources.push_back( GravitySource( sourceID, coefficient ) );
}

void PhysicsEngine::AddPhysicsEntity( int entityID )
{
  Engine::Entity *entity = Globals::entityFactory->GetEntity(entityID);

  CPhysics *component = static_cast<CPhysics *>(entity->GetFirstComponent( Engine::ComponentType::PHYSICS ));

  component->m_groupID = GroupID( component->m_groupName );

  m_groups[ component->m_groupID ].objects.push_back( entityID );
  m_groups[ component->m_groupID ].components.push_back( component );

  VolumeExtent extent(entityID, &component->m_range[0]);

  VolumeExtentIter extentIter = std::lower_bound( m_extents.begin(), m_extents.end(), extent );
  m_extents.insert( extentIter, extent );

  m_physicsEntities.push_back( entityID );
}

void PhysicsEngine::AddEmitterEntity( int entity_id )
{
  m_emitter_entities.push_back( entity_id );
}

void PhysicsEngine::RemovePhysicsEntity( int entityID )
{
  Engine::EntityIDList::iterator iter = std::find( m_physicsEntities.begin(), m_physicsEntities.end(), entityID );
  
  if( iter == m_physicsEntities.end() )
    return;

  CPhysics *obj = GetComponent( entityID );

  // Remove entity ID
  m_physicsEntities.erase( iter );

  
  // Remove volume extent

  VolumeExtentIterPair extentRange = std::equal_range(  m_extents.begin(), m_extents.end(), VolumeExtent(entityID, obj->m_range) );
  
  for(VolumeExtentIter extentIter = extentRange.first; extentIter != extentRange.second; ++extentIter)
  {
    if(extentIter->entityID == entityID)
    {
      m_extents.erase( extentIter );
      break;
    }
  }


  // Remove group entry

  ObjectList& group = m_groups[obj->m_groupID].objects;

  ObjectList::iterator groupIter = std::find( group.begin(), group.end(), entityID );

  if( groupIter != group.end() )
    group.erase( groupIter );

  std::vector<CPhysics *>& components = m_groups[obj->m_groupID].components;

  std::vector<CPhysics *>::iterator cmpIter = std::find( components.begin(), components.end(), obj );

  if( cmpIter != components.end() )
    components.erase( cmpIter );
}

void PhysicsEngine::RemoveEmitterEntity( int entityID )
{
  Engine::EntityIDList::iterator iter = std::find( m_emitter_entities.begin(), m_emitter_entities.end(), entityID );

  if( iter != m_emitter_entities.end() )
  {
    m_emitter_entities.erase( iter );
  }
}

struct ExtentSortor
{
  bool operator()(const VolumeExtent& lhs, const VolumeExtent& rhs)
  {
    return !(lhs < rhs);
  }
};

void PhysicsEngine::SortExtents()
{
  if( !m_extents.empty() )
    Utilities::smoothsort( &(m_extents[0]), (unsigned int)m_extents.size(), ExtentSortor() );
}

void PhysicsEngine::Update(float elapsed)
{
  unsigned int stepCount = 0;

  if(!m_pause)
  {
    m_integrationTime += elapsed;

    while( m_integrationTime >= m_timeStep && stepCount < 10)
    {
      UpdatePhysics();
      SortExtents();


      m_integrationTime -= m_timeStep;

      ++stepCount;
    }

    UpdateEmitters(m_timeStep);
  }

  while( !m_deletedEntities.empty() )
  {
    RemovePhysicsEntity( m_deletedEntities.back() );
    m_deletedEntities.pop_back();
  }
}

void PhysicsEngine::UpdatePhysics()
{
  // Check for collisions
  CheckAllCollisions();

  // Update physics components
  Engine::EntityIDList::iterator entityIter;

  for(entityIter = m_physicsEntities.begin(); entityIter != m_physicsEntities.end(); ++entityIter)
  {
    CPhysics *obj = GetComponent( *entityIter );

    obj->ThrottleVelocity();
    obj->ThrottleAngularVelocity();

    obj->IntegratePosition( m_integrator, m_timeStep * m_groups[obj->m_groupID].linearTimeScale );
    obj->IntegrateOrientation( m_integrator, m_timeStep * m_groups[obj->m_groupID].rotationalTimeScale );

    obj->Update();
  }
}

void PhysicsEngine::UpdateEmitters( float elapsed )
{
  Engine::EntityIDList::iterator entityIter;

  for(entityIter = m_emitter_entities.begin(); entityIter != m_emitter_entities.end(); ++entityIter)
  {
    Engine::Entity *entity = Globals::entityFactory->GetEntity( *entityIter );

    if(!entity)
      continue;

    Engine::ComponentList& components = entity->GetComponentsByType( Engine::ComponentType::EMITTER );
    Engine::ComponentList::iterator iter;

    for(iter = components.begin(); iter != components.end(); ++iter)
    {
      Engine::ParticleSystem::BaseEmitter * component = static_cast<Engine::ParticleSystem::BaseEmitter*>(*iter);
      component->Update(elapsed);
    }
  }
}

Vector3 PhysicsEngine::ForceField(const Math::Vector3& position)
{
  return m_forceField.GetVector(position);
}

Vector3 PhysicsEngine::GetGravForce(const Vector3& position)
{
  Vector3 totalForce;

  const char *groups[] = {"sun", "planets"};

  for(unsigned int i = 0; i < 2; ++i)
  {
    unsigned int id = GroupID(groups[i]);
  //for(unsigned int i = 0; i < m_groups.size(); ++i)
  //{
    for(unsigned int j = 0; j < m_groups[id].components.size(); ++j)
    {
      CPhysics * const &source = m_groups[id].components[j];

      Math::Vector3 sourceToObject = position - source->m_transform->GetWorldPosition();

      float distanceSquared = sourceToObject * sourceToObject;

      if(distanceSquared < 1e-5f)
        continue;

      Vector3 force = -sourceToObject;
      force.Normalize();

      force *= source->m_mass / distanceSquared;

      totalForce += force;
    }
  }

  return totalForce;
}

// PhysicsEngine:BuildForceField()
Vector3 PhysicsEngine::GetGravityForce(unsigned int groupID, const Math::Vector3& objPosition, float objMass)
{
  //return m_forceField.GetVector(objPosition) * objMass;

  Vector3 totalForce;

  if(groupID >= m_groups.size())
    return totalForce;

  const PhysicsGroup& objGroup = m_groups[groupID];

  for(unsigned int i = 0; i < objGroup.gravitySources.size(); ++i)
  {
    const PhysicsGroup& srcGroup = m_groups[ objGroup.gravitySources[i].first ];
    const float gravityCoefficient = objGroup.gravitySources[i].second;

    for(unsigned int j = 0; j < srcGroup.components.size(); ++j)
    {
      CPhysics * const &source = srcGroup.components[j];

      Math::Vector3 sourceToObject = objPosition - source->m_transform->GetWorldPosition();

      float distanceSquared = sourceToObject * sourceToObject;

      if(distanceSquared < 1e-5f)
        continue;

      Vector3 force = -sourceToObject;
      force.Normalize();

      force *= (gravityCoefficient * source->m_mass) / distanceSquared;

      totalForce += force;
    }
  }

  totalForce *= objMass;

  return totalForce;
}

Vector3 PhysicsEngine::GetDampingForce(const Vector3& objVelocity, float objMass, float drag ) const
{
  return -objVelocity * drag * objMass;
}

CPhysics *PhysicsEngine::GetComponent(Engine::EntityID entityID)
{
  CPhysics *component = 0;

  Engine::Entity *entity = Globals::entityFactory->GetEntity( entityID );

  if(entity)
    component = (CPhysics *)entity->GetFirstComponent( Engine::ComponentType::PHYSICS );

  return component;
}

Engine::Entity *PhysicsEngine::GetEntity(const Math::Ray& ray, float *intersectionTime, const std::string& groupName) const
{
  float maxTime = FLT_MAX;
  Engine::Entity *intersectingEntity = 0;

  Engine::EntityIDList::const_iterator entityIter;

  for(entityIter = m_physicsEntities.begin(); entityIter != m_physicsEntities.end(); ++entityIter)
  {
    Engine::Entity *entity = Globals::entityFactory->GetEntity( *entityIter );

    if(!entity)
      continue;

    CPhysics *component = static_cast<CPhysics *>( entity->GetFirstComponent( Engine::ComponentType::PHYSICS ) );
    Engine::ComponentList::iterator iter;

    if( !groupName.empty() && component->m_groupName != groupName )
      continue;

    float time;

    if( component->m_boundingSphere.Intersection(ray, time) && time < maxTime && time > 0 )
    {
      maxTime = time;
      intersectingEntity = entity;
    }
  }

  if(intersectionTime)
    *intersectionTime = maxTime;

  return intersectingEntity;
}

void PhysicsEngine::CheckAllCollisions()
{
  for(unsigned int i = 0; i < m_extents.size(); ++i)
  {
    for(unsigned int j = i + 1; j < m_extents.size(); ++j)
    {
      if(m_extents[j].range->lowerBound > m_extents[i].range->upperBound)
        break;

      Engine::EntityID entityIDA = m_extents[i].entityID;
      Engine::EntityID entityIDB = m_extents[j].entityID;

      CPhysics *objA = GetComponent( entityIDA );
      CPhysics *objB = GetComponent( entityIDB );

      if( !objA || !objB )
        continue;

      if( objA->m_range[1].Intersection( objB->m_range[1] ) &&
          m_groups[objA->m_groupID].collidesWith[ objB->m_groupID] )
      {
        objA->m_collisions.push_back( entityIDB );
      }
    }
  }

  Engine::EntityIDList::iterator entityIter;

  for(entityIter = m_physicsEntities.begin(); entityIter != m_physicsEntities.end(); ++entityIter)
  {
    Engine::EntityID entityIDA = *entityIter;

    CPhysics *objA = GetComponent( entityIDA );

    if( !objA )
      continue;

    for(unsigned int i = 0; i < objA->m_collisions.size(); ++i)
    {
      Engine::EntityID entityIDB = objA->m_collisions[i];
      CPhysics *objB = GetComponent( entityIDB );

      if(!objB)
        continue;

      /////////////

      if( objA->m_boundingSphere.Intersection( objB->m_boundingSphere ) )
      {
        ASSERT( objA != objB );

        float radiusA = objA->m_boundingSphere.GetRadius();
        float radiusB = objB->m_boundingSphere.GetRadius();

        float r1 = radiusA / (radiusA + radiusB);

        Math::Vector3 collisionPoint = objA->m_transform->GetWorldPosition() + r1 * (objB->m_transform->GetWorldPosition() - objA->m_transform->GetWorldPosition());

        Engine::Message *msgA = Globals::messageEngine->AddMessage("PhysicsCollision", entityIDA);
        Engine::Message *msgB = Globals::messageEngine->AddMessage("PhysicsCollision", entityIDB);

        msgA->stack.Push(entityIDA);
        msgA->stack.Push(entityIDB);
        msgA->stack.Push(collisionPoint.x);
        msgA->stack.Push(collisionPoint.y);
        msgA->stack.Push(collisionPoint.z);

        msgB->stack.Push(entityIDB);
        msgB->stack.Push(entityIDA);
        msgB->stack.Push(collisionPoint.x);
        msgB->stack.Push(collisionPoint.y);
        msgB->stack.Push(collisionPoint.z);
      }

      //////////////
    }

    objA->m_collisions.clear();
  }
}

float PhysicsEngine::GetTimestep()
{
  return m_timeStep;
}

void PhysicsEngine::SetTimestep(float timestep)
{
  if(timestep > 0)
  {
    m_timeStep = timestep;
  }
}

void PhysicsEngine::SetLinearTimeScale(const std::string& groupName, float timeScale)
{
  unsigned int groupID = GroupID(groupName);

  m_groups[groupID].linearTimeScale = timeScale;
}

void PhysicsEngine::SetRotationalTimeScale(const std::string& groupName, float timeScale)
{
  unsigned int groupID = GroupID(groupName);

  m_groups[groupID].rotationalTimeScale = timeScale;
}

void PhysicsEngine::SetIntegrator(IntegratorType type)
{
  m_integrator = type;
}

void PhysicsEngine::Pause()
{
  m_pause = true;
}

void PhysicsEngine::Unpause()
{
  m_pause = false;
}

void PhysicsEngine::TogglePause()
{
  m_pause = !m_pause;
}

bool PhysicsEngine::IsPaused()
{
  return m_pause;
}

LUA_START_DEFINE_CLASS( PhysicsEngine )
  LUA_NAME_METHOD( PhysicsEngine, "BuildForceField", LBuildForceField )

  LUA_NAME_METHOD( PhysicsEngine, "SetGravitySource", LSetGravitySource )

  LUA_NAME_METHOD( PhysicsEngine, "GetEntity", LGetEntity )
  LUA_NAME_METHOD( PhysicsEngine, "RemoveEntity", LRemoveEntity )
  LUA_NAME_METHOD( PhysicsEngine, "SetCollision", LSetCollision )
  LUA_NAME_METHOD( PhysicsEngine, "SetLinearTimeScale", LSetLinearTimeScale )
  LUA_NAME_METHOD( PhysicsEngine, "SetRotationalTimeScale", LSetRotationalTimeScale )
  LUA_NAME_METHOD( PhysicsEngine, "SetIntegrator", LSetIntegrator )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD_NOLS( PhysicsEngine, LBuildForceField )
{
  m_forceField.Build(
    Vector3(0,0,0),
    Vector3(1e5f,1e4f,1e5f),
    1e3f,
    VectorField::VectorFunction::Method<PhysicsEngine, &PhysicsEngine::GetGravForce>(this) );

  return 0;
}

LUA_DEFINE_METHOD( PhysicsEngine, LSetGravitySource )
{
  std::string sourceName = luaL_checkstring( ls, 1 );
  std::string groupName = luaL_checkstring( ls, 2 );
  float coefficient = luaL_checkfloat( ls, 3 );

  SetGravitySource( GroupID(sourceName), GroupID(groupName), coefficient );

  return 0;
}

LUA_DEFINE_METHOD( PhysicsEngine, LGetEntity )
{
  Math::Ray ray;

  ray.start.x = luaL_checkfloat(ls, 1);
  ray.start.y = luaL_checkfloat(ls, 2);
  ray.start.z = luaL_checkfloat(ls, 3);

  ray.direction.x = luaL_checkfloat(ls, 4);
  ray.direction.y = luaL_checkfloat(ls, 5);
  ray.direction.z = luaL_checkfloat(ls, 6);

  std::string groupName;
  
  if(lua_gettop(ls) >= 7)
    groupName = luaL_checkstring(ls, 7);

  float time;

  Engine::Entity *entity = GetEntity(ray, &time, groupName);

  if(entity)
  {
    Math::Vector3 point = ray(time);

    Lunar<Engine::Entity>::push(ls, entity);
    lua_pushnumber(ls, point.x);
    lua_pushnumber(ls, point.y);
    lua_pushnumber(ls, point.z);
  }
  else
  {
    lua_pushnil(ls);
    lua_pushnumber(ls, 0);
    lua_pushnumber(ls, 0);
    lua_pushnumber(ls, 0);
  }

  return 4;
}

LUA_DEFINE_METHOD( PhysicsEngine, LRemoveEntity )
{
  Engine::EntityID entityID = luaL_checkint(ls, 1);

  //m_deletedEntities.push_back( entityID );
  RemovePhysicsEntity( entityID );

  return 0;
}

LUA_DEFINE_METHOD( PhysicsEngine, LSetCollision )
{
  const std::string groupName1 = luaL_checkstring(ls, -3);
  const std::string groupName2 = luaL_checkstring(ls, -2);
  bool collision = lua_toboolean(ls, -1) ? true : false;

  unsigned int group1 = GroupID( groupName1 );
  unsigned int group2 = GroupID( groupName2 );

  m_groups[group1].collidesWith[ group2 ] = collision;
  m_groups[group2].collidesWith[ group1 ] = collision;

  return 0;
}

LUA_DEFINE_METHOD( PhysicsEngine, LSetLinearTimeScale )
{
  SetLinearTimeScale( luaL_checkstring(ls, 1), luaL_checkfloat(ls, 2) );

  return 0;
}

LUA_DEFINE_METHOD( PhysicsEngine, LSetRotationalTimeScale )
{
  SetRotationalTimeScale( luaL_checkstring(ls, 1), luaL_checkfloat(ls, 2) );

  return 0;
}

LUA_DEFINE_METHOD( PhysicsEngine, LSetIntegrator )
{
  std::string integrator_str = luaL_checkstring(ls, 1);

  if( integrator_str == "EULER_EXPLICIT" )
    SetIntegrator(EULER_EXPLICIT);
  else if( integrator_str == "EULER_IMPLICIT" )
    SetIntegrator(EULER_IMPLICIT);
  else if( integrator_str == "RK4" )
    SetIntegrator(RK4);
  else if( integrator_str == "VERLET" )
    SetIntegrator(VERLET);

  return 0;
}

} // Physics namespace
} // SF namespace
