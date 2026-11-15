/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CPhysics.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/14/2008 @ 17:38:11 - nwilliam
**  \brief   
*******************************************************************************/

#include <cfloat>

#include "CPhysics.h"
#include "Engine/Entity.h"
#include "Utilities/Misc.h"
#include "Math/CTransform.h"
#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"
#include "External/ticpp/include/ticpp.h"
#include "External/Lua/include/LunarHelper.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Physics
{

CPhysics::CPhysics()
: Engine::BaseComponent(Engine::ComponentType::PHYSICS )
{
  m_transform = 0;

  m_orientFromVelocity = false;
  m_maxVelocity = FLT_MAX;

  m_linearFriction = 0.0f;
  m_rotationalFriction = 0.0f;

  m_mass = 1e3f;
  m_inverseMass = 1.0f / m_mass;

  m_inertia = 1.0f;
  m_invInertia = 1.0f / m_inertia;
}

CPhysics::CPhysics(const CPhysics& obj)
: Engine::BaseComponent( obj )
{
  name = obj.name;

  m_groupID = obj.m_groupID;
  m_groupName = obj.m_groupName;

  m_transform = 0;

  m_orientFromVelocity = false;
  m_maxVelocity = FLT_MAX;

  m_linearFriction = obj.m_linearFriction;
  m_rotationalFriction = obj.m_rotationalFriction;

  m_mass = obj.m_mass;
  m_inverseMass = obj.m_inverseMass;

  m_inertia = obj.m_inertia;
  m_invInertia = obj.m_invInertia;
}

Math::Vector3 CPhysics::GetVelocity() const
{
  return m_velocity;
}

Math::Vector3 CPhysics::GetForce( const Math::Vector3& position, const Math::Vector3& velocity ) const
{
  return  Globals::physicsEngine->GetGravityForce( m_groupID, position, m_mass ) +
          m_forceAccum +
          -m_linearFriction * velocity;
}

void CPhysics::IntegratePosition(PhysicsEngine::IntegratorType integrator, float dt)
{
  switch(integrator)
  {
  case PhysicsEngine::EULER_EXPLICIT:
    IntegratePosEulerExplicit(dt);
    break;
  case PhysicsEngine::EULER_IMPLICIT:
    IntegratePosEulerImplicit(dt);
    break;
  case PhysicsEngine::RK4:
    IntegratePosRK4(dt);
    break;
  case PhysicsEngine::VERLET:
    IntegratePosVerlet(dt);
    break;
  }
}

void CPhysics::IntegrateOrientation(PhysicsEngine::IntegratorType integrator, float dt)
{
  if(m_orientFromVelocity)
  {
    m_transform->localOrientation = Quaternion::AlignToVector(
      m_velocity,
      Math::Vector3(0,0,1) );
  }
  else
  {
    switch(integrator)
    {
    case PhysicsEngine::EULER_EXPLICIT:
    case PhysicsEngine::EULER_IMPLICIT:
    case PhysicsEngine::RK4:
    case PhysicsEngine::VERLET:
      IntegrateRotEulerExplicit(dt);
      break;
    }
  }
}

void CPhysics::IntegratePosEulerExplicit(float dt)
{
  Vector3& position = m_transform->localPosition;

  m_acceleration = m_inverseMass * GetForce( position, m_velocity );

  position += m_velocity * dt;
  m_velocity += m_acceleration * dt;
}

void CPhysics::IntegratePosEulerImplicit(float dt)
{
  Vector3& position = m_transform->localPosition;

  m_acceleration = m_inverseMass * GetForce( position, m_velocity );

  m_velocity += m_acceleration * dt;
  position += m_velocity * dt;
}

void CPhysics::IntegratePosRK4(float dt)
{
  Vector3& position = m_transform->localPosition;

  float half_dt = dt * 0.5f;

  Math::Vector3 v1, v2, v3, v4;
  Math::Vector3 a1, a2, a3, a4;

  v1 = m_velocity;
  a1 = m_inverseMass * GetForce( position, m_velocity );

  v2 = m_velocity + a1 * half_dt;
  a2 = m_inverseMass * GetForce( position + v1 * half_dt, v1 * half_dt );

  v3 = m_velocity + a2 * half_dt;
  a3 = m_inverseMass * GetForce( position + v2 * half_dt, v2 * half_dt );

  v4 = m_velocity + a3 * dt;
  a4 = m_inverseMass * GetForce( position + v3 * dt, v3 * dt );

  Math::Vector3 velocity = 1.0f/6.0f * (v1 + 2.0f*(v2 + v3) + v4);
  m_acceleration = 1.0f/6.0f * (a1 + 2.0f*(a2 + a3) + a4);

  position += velocity * dt;
  m_velocity += m_acceleration * dt;
}

void CPhysics::IntegratePosVerlet(float dt)
{
  Vector3& position = m_transform->localPosition;

  position += m_velocity * dt + 0.5f * m_acceleration * dt * dt;
  m_velocity += 0.5f * m_acceleration * dt;

  m_acceleration = m_inverseMass * GetForce( position, m_velocity );

  m_velocity += 0.5f * m_acceleration * dt;
}

void CPhysics::IntegrateRotEulerExplicit(float dt)
{
  Quaternion& orientation = m_transform->localOrientation;
 
  m_torque += -m_rotationalFriction * m_angularVelocity;

  Quaternion dOrientation = 0.5f * (Quaternion(m_angularVelocity, 0) * orientation);

  orientation += dOrientation * dt;
  orientation.Normalize();

  m_angularMomentum += m_torque * dt;
  m_angularVelocity = m_angularMomentum * m_invInertia;

  m_torque = Vector3(0,0,0);
}

void CPhysics::ThrottleVelocity()
{
  if( m_velocity * m_velocity > m_maxVelocity * m_maxVelocity )
    m_velocity = m_velocity.Normalized() * m_maxVelocity;
}

void CPhysics::ThrottleAngularVelocity()
{
  /*const float maxAngVelocity = 0.001f;

  if( m_angularVelocity * m_angularVelocity > maxAngVelocity * maxAngVelocity )
  {
    m_angularVelocity = m_angularVelocity.Normalized() * maxAngVelocity;
    m_angularMomentum = m_angularVelocity * m_inertia;
  }*/
}

void CPhysics::Update()
{
  UpdateExtents();

  m_boundingSphere.SetPosition( m_transform->GetWorldPosition() );

  m_forceAccum = Vector3(0,0,0);
}

void CPhysics::UpdateExtents()
{
  Math::Vector3 position = m_transform->GetWorldPosition();

  m_range[0].lowerBound = position.x - m_boundingSphere.GetRadius();
  m_range[0].upperBound = position.x + m_boundingSphere.GetRadius();
  m_range[1].lowerBound = position.z - m_boundingSphere.GetRadius();
  m_range[1].upperBound = position.z + m_boundingSphere.GetRadius();
}

Engine::BaseComponent* CPhysics::Clone() const
{
  CPhysics *newPhysics = new CPhysics( *this );
  
  return newPhysics;
}

bool CPhysics::InitDependencies( Engine::Entity *entity )
{
  if( !entity->Has( Engine::ComponentType::TRANSFORM  ) )
  {
    return false;
  }
  else
  {
    m_transform = static_cast<Math::CTransform *>( entity->GetFirstComponent( Engine::ComponentType::TRANSFORM ) );

    m_boundingSphere.SetPosition( &m_transform->localPosition );

    return true;
  }
}

void CPhysics::PushToLua( lua_State *ls )
{
  Lunar<CPhysics>::push( ls, this );
}

Engine::BaseComponent *CPhysics::Creator(const ticpp::Element *element)
{
  CPhysics *object = new CPhysics;

  try
  {
    element->GetAttribute("group", &object->m_groupName);
  }
  catch( ticpp::Exception & e )
  {
    Globals::console->Error( e.what() );
    delete object;
    return 0;
  }

  return object;
}


////////////////////////////////////////////////////////////////////////////////
// Networking
////////////////////////////////////////////////////////////////////////////////
void CPhysics::InitIn( Networking::ByteStream& bs )
{
  bs >> m_groupName >> m_groupID
     >> m_linearFriction >> m_rotationalFriction >> m_mass >> m_orientFromVelocity
     >> m_inertia
     >> m_angularMomentum >> m_angularVelocity >> m_torque
     >> m_velocity >> m_maxVelocity >> m_acceleration;

  float radius;
  bs >> radius;
  
  m_boundingSphere.SetRadius( radius );
  
  m_inverseMass = 1.0f / m_mass;
  m_invInertia = 1.0f / m_inertia;
}

void CPhysics::DeltaIn( Networking::ByteStream& bs )
{
  Math::Vector3 newPos;
  bs >> newPos >> m_velocity
     >> m_transform->localOrientation >> m_angularVelocity >> m_angularMomentum;

  Math::Vector3 diff( newPos - m_transform->localPosition );

  // Note: 1800 is planet diameter
  if( diff.Magnitude() > 1800.0f )
  {
    m_transform->localPosition = newPos;
  }
  else
  {
    m_transform->localPosition += diff * 0.15f;
  }
}

void CPhysics::InitOut( Networking::ByteStream& bs ) const
{
  bs << m_groupName << m_groupID
     << m_linearFriction << m_rotationalFriction << m_mass << m_orientFromVelocity
     << m_inertia
     << m_angularMomentum << m_angularVelocity << m_torque
     << m_velocity << m_maxVelocity << m_acceleration
     << m_boundingSphere.GetRadius();
}

void CPhysics::DeltaOut( Networking::ByteStream& bs ) const
{
  bs << m_transform->localPosition << m_velocity
     << m_transform->localOrientation << m_angularVelocity << m_angularMomentum;
}




LUA_START_DEFINE_CLASS( CPhysics )
  LUA_NAME_METHOD( CPhysics, "GetVelocity", LGetVelocity )
  LUA_NAME_METHOD( CPhysics, "SetBoundingRadius", LSetBoundingRadius )
  LUA_NAME_METHOD( CPhysics, "SetLinearFriction", LSetLinearFriction )
  LUA_NAME_METHOD( CPhysics, "SetMass", LSetMass )
  LUA_NAME_METHOD( CPhysics, "GetMass", LGetMass )
  LUA_NAME_METHOD( CPhysics, "SetMaxVelocity", LSetMaxVelocity )
  LUA_NAME_METHOD( CPhysics, "SetOrientation", LSetOrientation )
  LUA_NAME_METHOD( CPhysics, "SetOrientationMode", LSetOrientationMode )
  LUA_NAME_METHOD( CPhysics, "SetPosition", LSetPosition )
  LUA_NAME_METHOD( CPhysics, "SetRotationalFriction", LSetRotationalFriction )
  LUA_NAME_METHOD( CPhysics, "SetVelocity", LSetVelocity )

  LUA_NAME_METHOD( CPhysics, "ApplyImpulse", LApplyImpulse )
  LUA_NAME_METHOD( CPhysics, "ApplyForce", LApplyForce )
  LUA_NAME_METHOD( CPhysics, "Torque", LTorque )

  LUA_NAME_METHOD( CPhysics, "GetConstForce", LGetConstForce )
  LUA_NAME_METHOD( CPhysics, "SetConstForce", LSetConstForce )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CPhysics, LGetVelocity )
{
  lua_pushnumber( ls, m_velocity.x );
  lua_pushnumber( ls, m_velocity.y );
  lua_pushnumber( ls, m_velocity.z );

  return 3;
}

LUA_DEFINE_METHOD( CPhysics, LSetBoundingRadius )
{
  float radius = luaL_checkfloat(ls, 1);

  m_boundingSphere.SetRadius(radius);

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LSetMass )
{
  m_mass = luaL_checkfloat(ls, 1);
  m_inverseMass = 1.0f / m_mass;

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LGetMass )
{
  lua_pushnumber(ls, m_mass);

  return 1;
}

LUA_DEFINE_METHOD( CPhysics, LSetMaxVelocity )
{
  m_maxVelocity = luaL_checkfloat(ls,1);

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LSetOrientation )
{
  float yaw = luaL_checkfloat(ls, 1);
  float pitch = luaL_checkfloat(ls, 2);
  float roll = luaL_checkfloat(ls, 3);

  m_transform->localOrientation = Quaternion::EulerAngles(yaw, pitch, roll);

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LSetOrientationMode )
{
  std::string mode = luaL_checkstring(ls, 1);

  if(mode == "FromVelocity")
    m_orientFromVelocity = true;
  else
    m_orientFromVelocity = false;
    
  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LSetPosition )
{
  float x = luaL_checkfloat(ls, 1);
  float y = luaL_checkfloat(ls, 2);
  float z = luaL_checkfloat(ls, 3);

  m_transform->localPosition = Math::Vector3(x, y, z);

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LSetRotationalFriction )
{
  m_rotationalFriction = Utilities::Max(0.0f, luaL_checkfloat(ls, 1));

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LSetLinearFriction )
{
  m_linearFriction = Utilities::Max(0.0f, luaL_checkfloat(ls, 1));
  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LSetVelocity )
{
  float x = luaL_checkfloat(ls, 1);
  float y = luaL_checkfloat(ls, 2);
  float z = luaL_checkfloat(ls, 3);

  m_velocity = Math::Vector3(x, y, z);

  return 0;
}


LUA_DEFINE_METHOD( CPhysics, LApplyImpulse )
{
  Vector3 impulse = Vector3(
    luaL_checkfloat(ls, -3),
    luaL_checkfloat(ls, -2),
    luaL_checkfloat(ls, -1) );

  m_velocity += impulse * m_inverseMass;

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LApplyForce )
{
  Vector3 force = Vector3(
    luaL_checkfloat(ls, -3),
    luaL_checkfloat(ls, -2),
    luaL_checkfloat(ls, -1) );

  m_forceAccum += force;

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LTorque )
{
  float x = luaL_checkfloat(ls, 1);
  float y = luaL_checkfloat(ls, 2);
  float z = luaL_checkfloat(ls, 3);

  m_torque += Vector3( x, y, z );

  return 0;
}

LUA_DEFINE_METHOD( CPhysics, LGetConstForce )
{
  lua_pushnumber(ls, m_constForce.x);
  lua_pushnumber(ls, m_constForce.y);
  lua_pushnumber(ls, m_constForce.z);

  return 3;
}

LUA_DEFINE_METHOD( CPhysics, LSetConstForce )
{
  m_constForce.x = luaL_checkfloat(ls, -3);
  m_constForce.y = luaL_checkfloat(ls, -2);
  m_constForce.z = luaL_checkfloat(ls, -1);

  return 0;
}

}

}
