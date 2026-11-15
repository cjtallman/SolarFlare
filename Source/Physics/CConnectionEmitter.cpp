/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CConnectionEmitter.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 03/16/2009 @ 21:59:48 - pmera
**  \brief   
*******************************************************************************/

#include "Graphics/GraphicsEngine.h"

#include "CConnectionEmitter.h"
#include "Engine/EntityFactory.h"
#include "Math/Interpolation.h"
#include "SolarFlare/Globals.h"
#include "Graphics/ParticleEngine.h"
#include "Utilities/Misc.h"


namespace SF {
namespace Physics
{

using namespace Utilities;

CConnectionEmitter::ParticleBehavior::ParticleBehavior(float lifetime)
: t(0), dt(1.0f / lifetime)
{
}

bool CConnectionEmitter::ParticleBehavior::Update(float elapsed, ParticleSystem::Particle& particle)
{
  elapsed = 16.667f;

  Engine::Entity *entity1 = Globals::entityFactory->GetEntity( entityID1 );
  Engine::Entity *entity2 = Globals::entityFactory->GetEntity( entityID2 );

  if( !entity1 || !entity2 )
    return false;

  CTransform *transform1 = (CTransform *)entity1->GetFirstComponent( Engine::ComponentType::TRANSFORM );
  CTransform *transform2 = (CTransform *)entity2->GetFirstComponent( Engine::ComponentType::TRANSFORM );

  // TODO: move transform evaluation to emitter?
  Vector3 p1 = transform1->GetWorldPosition();
  Vector3 p2 = transform2->GetWorldPosition();

  particle.position = Math::Lerp(p1, p2, t);

  Quaternion q = transform1->GetWorldOrientation();

  Vector3 n = q.LocalY();

  particle.position += n * 50;

  if(t >= 1)
    return false;

  t += dt * elapsed;
  return true;
}

CConnectionEmitter::CConnectionEmitter()
{
  m_emissionTimer = 0;
  m_emissionRate = 10;

  m_rand.Seed(0);
}

/*CConnectionEmitter::CConnectionEmitter(const CConnectionEmitter& rhs)
{
}*/

void CConnectionEmitter::Update(float elapsed)
{
  Vector3 worldPosition = m_transform->GetWorldPosition();
  Quaternion worldOrientation = m_transform->GetWorldOrientation();

  Vector3 normal = worldOrientation.LocalY();

  worldPosition += normal * 90;

  if( !DoCulling(worldPosition) )
    return;

  if( !DoLOD(worldPosition) )
    return;

  if( m_emissionTimer > 0.0f )
  {
    m_emissionTimer -= elapsed * (1 / m_emissionRate);
    return;
  }

  m_emissionTimer = 1.0f;

  unsigned int particleCount = 1 + unsigned int(Min( elapsed / 60, 1.0f ) * 3);

  for(unsigned int i = 0; i < m_connections.size(); ++i)
  {
    const Connection& connection = m_connections[i];

    for(unsigned int i = 0; i < particleCount; ++i)
    {
      //float t = i / float(particleCount);

      ParticleSystem::Particle particle;

      particle.color = m_color;
      particle.color.r = (unsigned char)Clamp<unsigned char>( particle.color.r * 1.5, 0, 255 );
      particle.color.g = (unsigned char)Clamp<unsigned char>( particle.color.g * 1.5, 0, 255 );
      particle.color.b = (unsigned char)Clamp<unsigned char>( particle.color.b * 1.5, 0, 255 );

      particle.size = 24.0f + (float)m_rand.Rand() * 24.0f;
      particle.additivity = 0.75f;
      particle.type = 1;

      ParticleBehavior *behavior = new ParticleBehavior(700);

      behavior->entityID1 = connection.id1;
      behavior->entityID2 = connection.id2;

      particle.behavior = behavior;

      Globals::particleEngine->CreateParticle( particle );
    }
  }
}

bool CConnectionEmitter::DoCulling(const Vector3& worldPosition)
{
  const Graphics::CFrustum& frustum = Globals::graphicsEngine->GetFrustum();

  return frustum.PointInside( worldPosition );
}

bool CConnectionEmitter::DoLOD(const Vector3& worldPosition)
{
  Vector3 view = Globals::graphicsEngine->CameraView();
  Vector3 cameraToEmitter = worldPosition - Globals::graphicsEngine->CameraPosition();

  const float cutoff = 8000;

  float dist = cameraToEmitter * view;

  if( dist > cutoff )
  {
    m_emissionTimer = 0.0f;
    return false;
  }

  m_emissionRate = 10 + dist / cutoff * 200;

  return true;
}

Engine::BaseComponent *CConnectionEmitter::Clone() const
{
  return new CConnectionEmitter(*this);
}

bool CConnectionEmitter::InitDependencies( Engine::Entity * entity )
{
  if( !entity->Has( Engine::ComponentType::TRANSFORM ) )
    return false;

  m_transform = static_cast<CTransform *>( entity->GetFirstComponent( Engine::ComponentType::TRANSFORM ) );

  return true;
}

Engine::BaseComponent *CConnectionEmitter::Creator( const ticpp::Element * )
{
  return new CConnectionEmitter();
}

void CConnectionEmitter::PushToLua( lua_State *ls )
{
  Lunar<CConnectionEmitter>::push( ls, this );
}

LUA_START_DEFINE_CLASS( CConnectionEmitter )
  LUA_NAME_METHOD( CConnectionEmitter, "AddConnection", LAddConnection )
  LUA_NAME_METHOD( CConnectionEmitter, "RemoveConnection", LRemoveConnection )
  LUA_NAME_METHOD( CConnectionEmitter, "SetColor", LSetColor )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CConnectionEmitter, LAddConnection )
{
  unsigned int id1 = luaL_checkint(ls, 1);
  unsigned int id2 = luaL_checkint(ls, 2);

  m_connections.push_back( Connection(id1, id2) );

  return 0;
}

LUA_DEFINE_METHOD( CConnectionEmitter, LRemoveConnection )
{
  unsigned int id = luaL_checkint(ls, 1);

  for(unsigned int i = 0; i < m_connections.size(); ++i)
  {
    if( m_connections[i].id1 == id )
      m_connections.erase( m_connections.begin() + i );
  }

  return 0;
}

LUA_DEFINE_METHOD( CConnectionEmitter, LSetColor )
{
  unsigned char r = (unsigned char)luaL_checkint(ls, 1);
  unsigned char g = (unsigned char)luaL_checkint(ls, 2);
  unsigned char b = (unsigned char)luaL_checkint(ls, 3);

  m_color = Color(r, g, b, 255);

  return 0;
}

}

}
