/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CExplosionEmitter.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 10/10/2008 @ 01:45:54 - pmera
**  \brief   
*******************************************************************************/

#include "Graphics/GraphicsEngine.h"

#include <cmath>
#include "Utilities/XML.h"
#include "Engine/Entity.h"
#include "Engine/EntityFactory.h"
#include "Utilities/Color.h"
#include "Math/Interpolation.h"
#include "Math/Quaternion.h"
#include "CExplosionEmitter.h"
#include "SolarFlare/Globals.h"
#include "Script/ScriptHelper.h"
#include "Utilities/DevConsole.h"
#include "Utilities/Random/Random.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Physics
{

using namespace Math;

CExplosionEmitter::ParticleBehavior::ParticleBehavior(float lifetime)
: t(0), dt(1.0f / lifetime)
{
}

bool CExplosionEmitter::ParticleBehavior::Update(float elapsed, ParticleSystem::Particle& particle)
{  
  /* Damped harmonic mushroom cloud: 
  
  float s = 0.5 + (2.5/(t+5.0) * sin(-1.57*1 + t * 1.5 * 3.14));
  float u = 1 + (0.2/(t+0.2) * sin(-1.57 + t * 2.5 * 3.14));
  s = std::max(s, 0.0f);*/

  Engine::Entity *emitterEntity = Globals::entityFactory->GetEntity( emitterID );

  if( !emitterEntity )
    return false;

  // TODO: have the emitter update its origin and basis in its update, then have particles access that
  CTransform *transform = (CTransform *)emitterEntity->GetFirstComponent( Engine::ComponentType::TRANSFORM );

  Quaternion q = transform->GetWorldOrientation();

  q *= orientation;

  // Note: z-axis is pointing out of the planet in this particular case
  Vector3 origin = transform->GetWorldPosition();
  Vector3 x = q.LocalX();
  Vector3 y = q.LocalY();
  Vector3 z = q.LocalZ();

  // Rotated axis of outward motion in the quaternions frame
  x = xAxis.x * x + xAxis.y * z;

  particle.position = origin + t * (x * xExtent + y * zExtent);

  particle.size = Lerp( sizeStart, sizeEnd, t );

  particle.additivity = Lerp( additivityStart, additivityEnd, t );

  particle.color.a = Lerp( colorStart.a, colorEnd.a, t, 2 );
  particle.color.r = Lerp( colorStart.r, colorEnd.r, t );
  particle.color.g = Lerp( colorStart.g, colorEnd.g, t );
  particle.color.b = Lerp( colorStart.b, colorEnd.b, t );

  if(t >= 1)
    return false;

  t += dt * elapsed;

  return true;
}

void CExplosionEmitter::Update(float dt)
{
  m_time += dt;

  if(!m_done)
  {
    m_time = 0;

    Math::Vector3 position = m_transform->GetWorldPosition();

    if( m_transform->HasParent() )
      m_transform->localOrientation = Quaternion::AlignToVector(m_transform->localPosition, Math::Vector3(0,1,0));
    else
      m_transform->localOrientation = Quaternion::EulerAngles( Globals::rRandom->RandInt(1000) / 1000.0f * 6.28f, Globals::rRandom->RandInt(1000) / 1000.0f * 6.28f, Globals::rRandom->RandInt(1000) / 1000.0f * 6.28f );;

    // We don't cull until we've set the transform's orientation
    if( !DoCulling(position, 100.0f) )
    {
      m_done = true;
      return;
    }

    // TODO: Particle-Count LOD

    for(int i = 0; i < m_particleCount; ++i)
    {
      // Axis of outward motion, each particle moves up (z) and out (xy)
      Vector2 axis = Vector2(1,0).Rotated( i * (6.28f / m_particleCount) );

      ParticleSystem::Particle particle;

      particle.position = position;
      particle.type = m_particleType;

      ParticleBehavior *behavior = new ParticleBehavior(m_duration);

      behavior->emitterID = m_entityID;

      behavior->sizeStart = m_sizeStart;
      behavior->sizeEnd = m_sizeEnd;

      behavior->additivityStart = m_additivityStart;
      behavior->additivityEnd = m_additivityEnd;

      behavior->colorStart = m_colorStart;
      behavior->colorEnd = m_colorEnd;

      behavior->xExtent = (Globals::rRandom->RandInt(100) / 100.0f) * m_xMotion;
      behavior->zExtent = (Globals::rRandom->RandInt(100) / 100.0f) * m_zMotion;

      behavior->xAxis = axis;

      particle.behavior = behavior;

      Globals::particleEngine->CreateParticle( particle );
    }

    m_done = true;
  }
}

bool CExplosionEmitter::DoCulling(const Vector3& worldPosition, float padding)
{
  const Graphics::CFrustum& frustum = Globals::graphicsEngine->GetFrustum();

  return frustum.SphereInside( worldPosition, padding );
}

void CExplosionEmitter::PushToLua( lua_State *ls )
{
  Lunar<CExplosionEmitter>::push( ls, this );
}

Engine::BaseComponent *CExplosionEmitter::Clone() const
{
  CExplosionEmitter *emitter = new CExplosionEmitter( *this );

  emitter->m_done = false;

  return emitter;
}

bool CExplosionEmitter::InitDependencies( Engine::Entity * entity )
{
  m_entityID = entity->ID();

  if( entity->Has( Engine::ComponentType::TRANSFORM ) )
  {
    m_transform = static_cast<Math::CTransform *>(entity->GetFirstComponent( Engine::ComponentType::TRANSFORM ));

    return true;
  }
  else
  {
    return false;
  }
}


Engine::BaseComponent *CExplosionEmitter::Creator( const ticpp::Element *element )
{
  if( !element )
    return 0;

  CExplosionEmitter *emitter = new CExplosionEmitter;

  try
  {
    Color defColor(255,255,255,255);
    
    Utilities::XML::ReadValue( element, "duration", &emitter->m_duration, 100.0f );
    Utilities::XML::ReadValue( element, "additivity_start", &emitter->m_additivityStart, 1.0f );
    Utilities::XML::ReadValue( element, "additivity_end", &emitter->m_additivityEnd, 1.0f );
    Utilities::XML::ReadValue( element, "size_start", &emitter->m_sizeStart, 100.0f );
    Utilities::XML::ReadValue( element, "size_end", &emitter->m_sizeEnd, 100.0f );
    Utilities::XML::ReadColor( element, "color_start", &emitter->m_colorStart, defColor );
    Utilities::XML::ReadColor( element, "color_end", &emitter->m_colorEnd, defColor );
    Utilities::XML::ReadValue( element, "x_motion", &emitter->m_xMotion, 400.0f );
    Utilities::XML::ReadValue( element, "z_motion", &emitter->m_zMotion, 400.0f );
    Utilities::XML::ReadValue( element, "particle_count", &emitter->m_particleCount, 100.0f );
    Utilities::XML::ReadValue( element, "particle_type", &emitter->m_particleType, (unsigned int)0 );
  }
  catch( ticpp::Exception & e )
  {
    Globals::console->Error( e.what() );
    delete emitter;
    return 0;
  }

  return emitter;
}

LUA_START_DEFINE_CLASS( CExplosionEmitter )
  LUA_NAME_METHOD( CExplosionEmitter, "SetVelocity", LSetVelocity )
  LUA_NAME_METHOD( CExplosionEmitter, "SetStartColor", LSetStartColor )
  LUA_NAME_METHOD( CExplosionEmitter, "SetEndColor", LSetEndColor )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CExplosionEmitter, LSetVelocity )
{
  m_velocity.x = luaL_checkfloat(ls, 1);
  m_velocity.y = luaL_checkfloat(ls, 2);
  m_velocity.z = luaL_checkfloat(ls, 3);

  return 0;
}

LUA_DEFINE_METHOD( CExplosionEmitter, LSetStartColor )
{
  int r = 255;
  int g = 255;
  int b = 255;
  int a = 255;

  int top = lua_gettop(ls);
  switch( top )
  {
  case 4:
    a = luaL_checkint(ls, 4);
  case 3:
    b = luaL_checkint(ls, 3);
  case 2:
    g = luaL_checkint(ls, 2);
  case 1:
    r = luaL_checkint(ls, 1);
  default:
    m_colorStart = Color( (unsigned char)r, 
      (unsigned char)g,
      (unsigned char)b,
      (unsigned char)a);
  }

  return 0;
}

LUA_DEFINE_METHOD( CExplosionEmitter, LSetEndColor )
{
  int r = 255;
  int g = 255;
  int b = 255;
  int a = 255;

  int top = lua_gettop(ls);
  switch( top )
  {
  case 4:
    a = luaL_checkint(ls, 4);
  case 3:
    b = luaL_checkint(ls, 3);
  case 2:
    g = luaL_checkint(ls, 2);
  case 1:
    r = luaL_checkint(ls, 1);
  default:
    m_colorEnd = Color( (unsigned char)r, 
      (unsigned char)g,
      (unsigned char)b,
      (unsigned char)a);
  }

  return 0;
}



}

}
