/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CRocketEmitter.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300
**  \par     Updated: 10/12/2008 @ 02:32:55 - pmera
**  \brief   Particle emitter for the rocket/projectile.
*******************************************************************************/

#ifndef SF_CROCKETEMITTER_H
#define SF_CROCKETEMITTER_H

#include "Graphics/CBaseEmitter.h"
#include "Graphics/ParticleEngine.h"
#include "Math/CTransform.h"

#include "Utilities/MemoryManager.h"

namespace SF {
namespace Physics
{

using namespace ParticleSystem;

class CRocketEmitter : public ParticleSystem::CBaseEmitter, public MemoryManagerBase<CRocketEmitter>
{
  public:
    CRocketEmitter();

    void Update(float timeElapsed);

    static bool Behavior(ParticleSystem::Particle& particle, float timeElapsed);

    BaseComponent * Clone() const;
    bool InitDependencies( Engine::Entity * entity );

    static Engine::BaseComponent * Creator( const ticpp::Element * element );

    void PushToLua( lua_State *ls );

  private:
    struct ParticleBehavior : public BaseParticleBehavior, public MemoryManagerBase<ParticleBehavior, 1024>
    {
      ParticleBehavior(float lifetime);
      virtual ~ParticleBehavior() { }
      bool Update(float elapsed, Particle& particle);

      float t;
      float dt;

      Vector3 origin;
      Vector3 axis;

      float additivityStart;
      float additivityEnd;

      Color colorStart;
      Color colorEnd;

      float sizeStart;
      float sizeEnd;
    };

    bool DoCulling(const Vector3& worldPosition, float padding);
    bool DoLOD(const Vector3& worldPosition);

    Math::CTransform *m_transform;

    Math::Vector3 m_lastPosition;
    Math::Vector3 m_displacementStart;

    float m_displacement;

    float m_emitRate;
    float m_LODFactor;

    float m_sizeStart;
    float m_sizeEnd;

    float m_particleLifetime;
    Color m_colorStart;
    Color m_colorEnd;

    float m_additivityStart;
    float m_additivityEnd;

    float m_xMotion;

    unsigned int m_particleType;

    LUA_DECLARE_CLASS( CRocketEmitter )
      LUA_DECLARE_METHOD( LSetStartColor )
      LUA_DECLARE_METHOD( LSetEndColor )
};

}
}

#endif
