/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CRocketEmitter.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300
**  \par     Updated: 10/12/2008 @ 02:54:37 - pmera
**  \brief   Particle emitter for the rocket/projectile.
*******************************************************************************/

#include "Graphics/GraphicsEngine.h"

#include "Utilities/XML.h"
#include "Engine/Entity.h"
#include "CRocketEmitter.h"
#include "Graphics/ParticleEngine.h"
#include "Math/Interpolation.h"
#include "Physics/PhysicsEngine.h"
#include "SolarFlare/Globals.h"
#include "Utilities/Color.h"
#include "Utilities/DevConsole.h"
#include "Utilities/Random/Random.h"
#include "Utilities/Misc.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Physics
{

using namespace Math;

CRocketEmitter::CRocketEmitter()
{
  m_emitRate = 10.0f;
  m_particleType = 0;
  m_LODFactor = 1;
}

void CRocketEmitter::Update(float elapsed)
{
  Math::Vector3 position = m_transform->GetWorldPosition();
  Math::Quaternion orientation = m_transform->GetWorldOrientation();

  if( !DoCulling( position, 100.0f ) )
  {
    m_lastPosition = position;
    return;
  }

  DoLOD( position );

  if( m_lastPosition == Math::Vector3() )
    m_lastPosition = position;

  Math::Vector3 vel = position - m_lastPosition;
  float velMag = vel.Magnitude();

  unsigned int particleCount = Utilities::Clamp<unsigned int>( velMag / 50 * m_LODFactor, 1, 10 );

  float t = 0.0f;
  float dt = 1 / float(particleCount);

  for(unsigned int i = 0; i < particleCount; ++i, t += dt)
  {
    orientation = Quaternion::EulerAngles( Globals::rRandom->RandInt(1000) / 1000.0f * 6.28f, Globals::rRandom->RandInt(1000) / 1000.0f * 6.28f, Globals::rRandom->RandInt(1000) / 1000.0f * 6.28f );

    Math::Vector3 localX = orientation.LocalX();

    ParticleSystem::Particle particle;

    particle.type = m_particleType;
    
    ParticleBehavior *behavior = new ParticleBehavior( m_particleLifetime - t * elapsed );

    behavior->origin = position - t * vel;
    behavior->axis = localX * m_xMotion;

    behavior->colorStart = m_colorStart;
    behavior->colorEnd =   m_colorEnd;

    behavior->additivityStart = m_additivityStart;
    behavior->additivityEnd =   m_additivityEnd;

    behavior->sizeStart = m_sizeStart;
    behavior->sizeEnd = m_sizeEnd;

    particle.behavior = behavior;

    Globals::particleEngine->CreateParticle(particle);
  }

  m_lastPosition = position;
}

bool CRocketEmitter::DoCulling(const Vector3& worldPosition, float padding)
{
  const Graphics::CFrustum& frustum = Globals::graphicsEngine->GetFrustum();

  return frustum.SphereInside( worldPosition, padding );
}

bool CRocketEmitter::DoLOD(const Vector3& worldPosition)
{
  Vector3 view = Globals::graphicsEngine->CameraView();
  Vector3 cameraToEmitter = worldPosition - Globals::graphicsEngine->CameraPosition();

  const float start = 5000;
  const float cutoff = 50000;

  float dist = cameraToEmitter * view;

  if( dist < start )
  {
    m_LODFactor = 1.0f;
    return true;
  }

  if( dist > cutoff )
  {
    m_LODFactor = 0.0f;
    return true;
  }

  m_LODFactor = 1 - (dist - start) / (cutoff - start);

  return true;
}

CRocketEmitter::ParticleBehavior::ParticleBehavior(float lifetime)
: t(0), dt(1.0f / lifetime)
{
}

bool CRocketEmitter::ParticleBehavior::Update(float elapsed, ParticleSystem::Particle& particle)
{
  particle.position = origin + t * axis;

  particle.color.a = Lerp( colorStart.a, colorEnd.a, t );
  particle.color.r = Lerp( colorStart.r, colorEnd.r, t );
  particle.color.g = Lerp( colorStart.g, colorEnd.g, t );
  particle.color.b = Lerp( colorStart.b, colorEnd.b, t );

  particle.additivity = Lerp( additivityStart, additivityEnd, t );

  particle.size = Lerp( sizeStart, sizeEnd, t );

  if(t >= 1)
    return false;

  t += dt * elapsed;

  return true;
}

Engine::BaseComponent *CRocketEmitter::Clone() const
{
  CRocketEmitter *emitter = new CRocketEmitter( *this );

  return emitter;
}

bool CRocketEmitter::InitDependencies( Engine::Entity * entity )
{
  if( entity->Has( Engine::ComponentType::TRANSFORM ) )
  {
    m_transform = static_cast<Math::CTransform *>( entity->GetFirstComponent( Engine::ComponentType::TRANSFORM ) );

    return true;
  }
  else
  {
    return false;
  }
}

Engine::BaseComponent *CRocketEmitter::Creator( const ticpp::Element * element )
{
  if( !element )
    return 0;

  CRocketEmitter *emitter = new CRocketEmitter;
  
  try
  {
    Color defColor(255,255,255,255);

    Utilities::XML::ReadValue( element, "lifetime", &emitter->m_particleLifetime, 100.0f );
    
    Utilities::XML::ReadValue( element, "emit_rate", &emitter->m_emitRate, 100.0f );

    Utilities::XML::ReadValue( element, "size_start", &emitter->m_sizeStart, 100.0f );
    Utilities::XML::ReadValue( element, "size_end", &emitter->m_sizeEnd, 100.0f );

    Utilities::XML::ReadColor( element, "color_start", &emitter->m_colorStart, defColor );
    Utilities::XML::ReadColor( element, "color_end", &emitter->m_colorEnd, defColor );

    Utilities::XML::ReadValue( element, "additivity_start", &emitter->m_additivityStart, 1.0f );
    Utilities::XML::ReadValue( element, "additivity_end", &emitter->m_additivityEnd, 0.0f );

    Utilities::XML::ReadValue( element, "x_motion", &emitter->m_xMotion, 100.0f );
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

void CRocketEmitter::PushToLua( lua_State *ls )
{
  Lunar<CRocketEmitter>::push( ls, this );
}

LUA_START_DEFINE_CLASS( CRocketEmitter )
LUA_NAME_METHOD( CRocketEmitter, "SetStartColor", LSetStartColor )
LUA_NAME_METHOD( CRocketEmitter, "SetEndColor", LSetEndColor )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CRocketEmitter, LSetStartColor )
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

LUA_DEFINE_METHOD( CRocketEmitter, LSetEndColor )
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
