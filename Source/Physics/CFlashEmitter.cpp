/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CFlashEmitter.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 10/10/2008 @ 01:45:54 - pmera
**  \brief   
*******************************************************************************/

#include <cmath>
#include "Utilities/XML.h"
#include "CFlashEmitter.h"
#include "Engine/Entity.h"
#include "Math/Interpolation.h"
#include "Math/Quaternion.h"
#include "Utilities/Color.h"
#include "SolarFlare/Globals.h"
#include "Script/ScriptHelper.h"
#include "Utilities/DevConsole.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Physics
{

using namespace Math;

CFlashEmitter::ParticleBehavior::ParticleBehavior(float lifetime)
: t(0), dt(1.0f / lifetime)
{
}

bool CFlashEmitter::ParticleBehavior::Update(float elapsed, Particle& particle)
{
  particle.size = Lerp( sizeStart, sizeEnd, t );

  particle.color.a = Lerp( colorStart.a, colorEnd.a, t );
  particle.color.r = Lerp( colorStart.r, colorEnd.r, t );
  particle.color.g = Lerp( colorStart.g, colorEnd.g, t );
  particle.color.b = Lerp( colorStart.b, colorEnd.b, t );

  if(t >= 1.0f)
    return false;
  
  t += dt * elapsed;

  return true;
}

void CFlashEmitter::Update(float dt)
{
  m_time += dt;

  if(m_count > 0)
  {
    if(m_time >= m_frequency)
    {
      --m_count;
      m_time -= m_frequency;

      Math::Vector3 position = m_transform->GetWorldPosition();

      /* planet's normal at the point of explosion, assuming explosion is defined
         relative to planet. */
      Math::Vector3 normal = m_transform->localPosition.Normalized();

      if( m_transform->HasParent() )
      {
        normal.Rotate(m_transform->GetWorldOrientation());
        normal.Normalize();
      }
      else
      {
        normal = Math::Vector3();
      }

      ParticleSystem::Particle particle;

      particle.position = position + normal * 200.0f;
      particle.type = m_particleType;

      ParticleBehavior *behavior = new ParticleBehavior( m_duration );

      behavior->sizeStart = m_sizeStart;
      behavior->sizeEnd = m_sizeEnd;

      behavior->colorStart = m_colorStart;
      behavior->colorEnd = m_colorEnd;

      particle.behavior = behavior;

      Globals::particleEngine->CreateParticle( particle );
    }
  }
}

Engine::BaseComponent *CFlashEmitter::Clone() const
{
  CFlashEmitter *emitter = new CFlashEmitter( *this );

  return emitter;
}

bool CFlashEmitter::InitDependencies( Engine::Entity * entity )
{
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

Engine::BaseComponent *CFlashEmitter::Creator( const ticpp::Element *element )
{
  if( !element )
    return 0;

  CFlashEmitter *emitter = new CFlashEmitter;

  try
  {
    Color defColor(255,255,255,255);

    Utilities::XML::ReadValue( element, "count", &emitter->m_count, (unsigned int)1 );
    Utilities::XML::ReadValue( element, "frequency", &emitter->m_frequency, 1.0f );
    Utilities::XML::ReadValue( element, "duration", &emitter->m_duration, 100.0f );
    Utilities::XML::ReadValue( element, "size_start", &emitter->m_sizeStart, 100.0f );
    Utilities::XML::ReadValue( element, "size_end", &emitter->m_sizeEnd, 100.0f );
    Utilities::XML::ReadColor( element, "color_start", &emitter->m_colorStart, defColor );
    Utilities::XML::ReadColor( element, "color_end", &emitter->m_colorEnd, defColor );
    Utilities::XML::ReadValue<unsigned int>( element, "particle_type", &emitter->m_particleType, (unsigned int)0 );

  }
  catch( ticpp::Exception & e )
  {
    Globals::console->Error( e.what() );
    delete emitter;
    return 0;
  }

  return emitter;
}

void CFlashEmitter::PushToLua( lua_State *ls )
{
  Lunar<CFlashEmitter>::push( ls, this );
}

LUA_START_DEFINE_CLASS( CFlashEmitter )
  LUA_NAME_METHOD( CFlashEmitter, "SetVelocity", LSetVelocity )
  LUA_NAME_METHOD( CFlashEmitter, "SetStartColor", LSetStartColor )
  LUA_NAME_METHOD( CFlashEmitter, "SetEndColor", LSetEndColor )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD_NOLS( CFlashEmitter, LSetVelocity )
{
  return 0;
}

LUA_DEFINE_METHOD( CFlashEmitter, LSetStartColor )
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

LUA_DEFINE_METHOD( CFlashEmitter, LSetEndColor )
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
