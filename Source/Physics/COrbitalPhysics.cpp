/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    COrbitalPhysics.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 10/03/2008 @ 17:41:28 - pmera
**  \brief   
*******************************************************************************/

#include "COrbitalPhysics.h"
#include "Engine/Entity.h"
#include "Math/Constants.h"
#include "Math/CTransform.h"
#include "SolarFlare/Globals.h"
#include "Script/ScriptHelper.h"
#include "Utilities/DevConsole.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Physics
{

COrbitalPhysics::COrbitalPhysics()
{
  m_time = 0.0f;
  m_timeScale = 1.0f;

  m_transform = 0;
  m_rotationPeriod = 0.0f;
  m_mass = 1e8f;
}

COrbitalPhysics::COrbitalPhysics(const COrbitalPhysics& obj)
: CPhysics(obj)
{
  m_rotationPeriod = obj.m_rotationPeriod;
  m_time = obj.m_time;
  m_timeScale = obj.m_timeScale;
}

void COrbitalPhysics::IntegratePosition(PhysicsEngine::IntegratorType, float dt)
{
  Math::Vector3 nextPosition = m_orbit.GetPosition(m_time + dt * m_timeScale);
  nextPosition.Rotate( m_orbitTransform.localOrientation );

  if( dt * m_timeScale > 1e-5f )
    m_velocity = (nextPosition - m_transform->localPosition) / (dt * m_timeScale);
  else
    m_velocity = Math::Vector3(0,0,0);

  m_transform->localPosition = nextPosition;

  m_time += dt * m_timeScale;
}

void COrbitalPhysics::Update()
{
  UpdateExtents();

  m_boundingSphere.SetPosition( m_transform->GetWorldPosition() );
}

void COrbitalPhysics::UpdateFromKepler(float time)
{
  Vector3& position = m_transform->localPosition;

  position = m_orbit.GetPosition(time);
  position.Rotate( m_orbitTransform.localOrientation );

  float timeStep = Globals::physicsEngine->GetTimestep();
  float halfTimeStep = 0.5f * timeStep;

  Math::Vector3 p1 = m_orbit.GetPosition(time - halfTimeStep);
  Math::Vector3 p2 = m_orbit.GetPosition(time + halfTimeStep);

  m_velocity = (p2 - p1) / timeStep;
}

Engine::BaseComponent* COrbitalPhysics::Clone() const
{
  COrbitalPhysics *newComponent = new COrbitalPhysics( *this );

  return newComponent;
}

bool COrbitalPhysics::InitDependencies( Engine::Entity *entity )
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

void COrbitalPhysics::PushToLua( lua_State *ls )
{
  Lunar<COrbitalPhysics>::push( ls, this );
}

Engine::BaseComponent *COrbitalPhysics::Creator(const ticpp::Element *element)
{
  COrbitalPhysics *object = new COrbitalPhysics;

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

// Networking
void COrbitalPhysics::InitIn( Networking::ByteStream& bs )
{
  CPhysics::InitIn( bs );
  bs >> m_orbit.sunDistance >> m_orbit.perihelionDistance >> m_orbit.period
     >> m_orbit.timeShift
     >> m_orbitTransform.localOrientation
     >> m_rotationPeriod >> m_time;
  
  m_orbit.Update();
}

void COrbitalPhysics::DeltaIn( Networking::ByteStream& bs )
{
  bs >> m_time;
}

void COrbitalPhysics::InitOut( Networking::ByteStream& bs ) const
{
  CPhysics::InitOut( bs );
  bs << m_orbit.sunDistance << m_orbit.perihelionDistance << m_orbit.period
     << m_orbit.timeShift
     << m_orbitTransform.localOrientation
     << m_rotationPeriod << m_time;
}

void COrbitalPhysics::DeltaOut( Networking::ByteStream& bs ) const
{
  bs << m_time;
}


LUA_START_DEFINE_CLASS( COrbitalPhysics )
  LUA_NAME_METHOD( COrbitalPhysics, "GetVelocity", LGetVelocity )
  LUA_NAME_METHOD( COrbitalPhysics, "GetPosition", LGetPosition )

  LUA_NAME_METHOD( COrbitalPhysics, "GetPeriod", LGetPeriod )
  LUA_NAME_METHOD( COrbitalPhysics, "GetTime", LGetTime )
  LUA_NAME_METHOD( COrbitalPhysics, "GetMajorAxis", LGetMajorAxis )
  LUA_NAME_METHOD( COrbitalPhysics, "GetMinorAxis", LGetMinorAxis )
  LUA_NAME_METHOD( COrbitalPhysics, "GetPerihelionDistance", LGetPerihelionDistance )

  LUA_NAME_METHOD( COrbitalPhysics, "SetBoundingRadius", LSetBoundingRadius )
  LUA_NAME_METHOD( COrbitalPhysics, "SetMass", LSetMass )
  LUA_NAME_METHOD( COrbitalPhysics, "SetOrientation", LSetOrientation )
  LUA_NAME_METHOD( COrbitalPhysics, "SetPerihelionDistance", LSetPerihelionDistance )
  LUA_NAME_METHOD( COrbitalPhysics, "SetPeriod", LSetPeriod )
  LUA_NAME_METHOD( COrbitalPhysics, "SetSunDistance", LSetSunDistance )
  LUA_NAME_METHOD( COrbitalPhysics, "SetRotationalFriction", LSetRotationalFriction )
  LUA_NAME_METHOD( COrbitalPhysics, "SetRotationPeriod", LSetRotationPeriod )
  LUA_NAME_METHOD( COrbitalPhysics, "SetTime", LSetTime )
  LUA_NAME_METHOD( COrbitalPhysics, "SetTimeShift", LSetTimeShift )

  LUA_NAME_METHOD( COrbitalPhysics, "SetTimeScale", LSetTimeScale )

  LUA_NAME_METHOD( COrbitalPhysics, "Torque", LTorque )
  LUA_NAME_METHOD( COrbitalPhysics, "SetRotationalVelocity", LSetRotationalVelocity )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( COrbitalPhysics, LGetPosition )
{
  lua_pushnumber( ls, m_transform->localPosition.x );
  lua_pushnumber( ls, m_transform->localPosition.y );
  lua_pushnumber( ls, m_transform->localPosition.z );

  return 3;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LGetVelocity )
{
  lua_pushnumber( ls, m_velocity.x );
  lua_pushnumber( ls, m_velocity.y );
  lua_pushnumber( ls, m_velocity.z );

  return 3;
}
  
LUA_DEFINE_METHOD( COrbitalPhysics, LGetPeriod )
{
  lua_pushnumber(ls, m_orbit.period);

  return 1;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LGetTime )
{
  lua_pushnumber(ls, m_time);

  return 1;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LGetMajorAxis )
{
  lua_pushnumber( ls, m_orbit.m_semiMajorAxis );
  return 1;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LGetMinorAxis )
{
  lua_pushnumber( ls, m_orbit.m_semiMinorAxis );
  return 1;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LGetPerihelionDistance )
{
  lua_pushnumber( ls, m_orbit.perihelionDistance );
  return 1;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetBoundingRadius )
{
  float radius = luaL_checkfloat(ls, 1);

  m_boundingSphere.SetRadius(radius);
  
  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetMass )
{
  float mass = luaL_checkfloat(ls, 1);

  m_mass = mass;

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetOrientation )
{
  float yaw = luaL_checkfloat(ls, 1);
  float pitch = luaL_checkfloat(ls, 2);
  float roll = luaL_checkfloat(ls, 3);

  m_orbitTransform.localOrientation = Quaternion::EulerAngles(yaw, pitch, roll);
  
  UpdateFromKepler(m_time);

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetPerihelionDistance )
{
  float d = luaL_checkfloat(ls, 1);

  m_orbit.perihelionDistance = d;
  m_orbit.Update();
  
  UpdateFromKepler(m_time);

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetPeriod )
{
  float period = luaL_checkfloat(ls, 1);

  m_orbit.period = period;
  m_orbit.Update();
  
  UpdateFromKepler(m_time);

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetSunDistance )
{
  float sunDist = luaL_checkfloat(ls, 1);

  m_orbit.sunDistance = sunDist;
  m_orbit.Update();
  
  UpdateFromKepler(m_time);

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetRotationalFriction )
{
  return CPhysics::LSetRotationalFriction(ls);
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetRotationPeriod )
{
  m_rotationPeriod = luaL_checkfloat(ls, 1);
  
  UpdateFromKepler(m_time);

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetTime )
{
  m_time = luaL_checkfloat(ls, 1);

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetTimeShift )
{
  m_orbit.timeShift = luaL_checkfloat(ls, 1);
  
  UpdateFromKepler(m_time);

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetTimeScale )
{
  m_timeScale = luaL_checkfloat(ls, 1);

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LTorque )
{
  float x = luaL_checkfloat(ls, 1);
  float y = luaL_checkfloat(ls, 2);
  float z = luaL_checkfloat(ls, 3);

  m_torque += Vector3( x, y, z );

  return 0;
}

LUA_DEFINE_METHOD( COrbitalPhysics, LSetRotationalVelocity )
{
  float x = luaL_checkfloat(ls, 1);
  float y = luaL_checkfloat(ls, 2);
  float z = luaL_checkfloat(ls, 3);

  m_angularVelocity = Vector3(x, y, z);
  m_angularMomentum = m_angularVelocity * m_inertia;

  return 0;
}

}

}
