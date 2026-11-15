/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CFlashEmitter.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/10/2008 @ 20:53:24 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_CFLASHEMITTER_H_
#define SF_CFLASHEMITTER_H_

#include "Graphics/CBaseEmitter.h"
#include "Graphics/ParticleEngine.h"
#include "Math/CTransform.h"

#include "Utilities/MemoryManager.h"

namespace SF {
namespace Physics
{

using namespace ParticleSystem;

class CFlashEmitter : public ParticleSystem::CBaseEmitter, public MemoryManagerBase<CFlashEmitter>
{
public:
  void Update(float timeElapsed);

  static bool Behavior(ParticleSystem::Particle& particle, float timeElapsed);

  BaseComponent * Clone() const;
  bool InitDependencies( Engine::Entity * entity );

  static Engine::BaseComponent * Creator( const ticpp::Element * element );

   void PushToLua( lua_State *ls );

private:
  struct ParticleBehavior : public BaseParticleBehavior, public MemoryManagerBase<ParticleBehavior, 512>
  {
    ParticleBehavior(float lifetime);
    virtual ~ParticleBehavior() { }
    bool Update(float elapsed, Particle& particle);

    float t;
    float dt;

    float sizeStart;
    float sizeEnd;

    Color colorStart;
    Color colorEnd;
  };

  Math::CTransform *m_transform;

  unsigned int m_count;
  float m_frequency;

  float m_duration;

  float m_sizeStart;
  float m_sizeEnd;

  Color m_colorStart;
  Color m_colorEnd;

  unsigned int m_particleType;

  float m_time;

  LUA_DECLARE_CLASS( CFlashEmitter )
  LUA_DECLARE_METHOD( LSetVelocity )
  LUA_DECLARE_METHOD( LSetStartColor )
  LUA_DECLARE_METHOD( LSetEndColor )

};

}

}

#endif