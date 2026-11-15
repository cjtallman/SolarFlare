/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CExplosionEmitter.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 10/10/2008 @ 01:42:07 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_CEXPLOSIONEMITTER_H_
#define SF_CEXPLOSIONEMITTER_H_

#include "Utilities/Color.h"
#include "Math/CTransform.h"
#include "Graphics/CBaseEmitter.h"
#include "Graphics/ParticleEngine.h"

#include "Utilities/MemoryManager.h"

namespace SF {
namespace Physics
{

using namespace ParticleSystem;

class CExplosionEmitter : public ParticleSystem::CBaseEmitter, public MemoryManagerBase<CExplosionEmitter>
{
public:
  void Update(float timeElapsed);

  bool DoCulling(const Vector3& worldPosition, float padding);

  void PushToLua( lua_State *ls );
  BaseComponent * Clone() const;
  bool InitDependencies( Engine::Entity * entity );

  static Engine::BaseComponent * Creator( const ticpp::Element * element );

private:
  struct ParticleBehavior : public BaseParticleBehavior, public MemoryManagerBase<ParticleBehavior, 1024>
  {
    ParticleBehavior(float lifetime);
    virtual ~ParticleBehavior() { }

    virtual bool Update(float elapsed, ParticleSystem::Particle& particle);

    Engine::EntityID  emitterID;

    float t;
    float dt;

    Vector2     xAxis;
    float       xExtent;
    float       zExtent;
    Quaternion  orientation;

    float additivityStart;
    float additivityEnd;

    Color colorStart;
    Color colorEnd;

    float sizeStart;
    float sizeEnd;
  };

  Engine::EntityID  m_entityID;

  Math::CTransform *m_transform;
  Math::Vector3     m_velocity;

  float m_duration;

  float m_additivityStart;
  float m_additivityEnd;

  float m_sizeStart;
  float m_sizeEnd;

  Color m_colorStart;
  Color m_colorEnd;

  float m_xMotion;
  float m_zMotion;

  float m_particleCount;

  float m_time;

  unsigned int m_particleType;

  bool m_done;

  LUA_DECLARE_CLASS( CExplosionEmitter )
  LUA_DECLARE_METHOD( LSetVelocity )
  LUA_DECLARE_METHOD( LSetStartColor )
  LUA_DECLARE_METHOD( LSetEndColor )

};

}

}

#endif
