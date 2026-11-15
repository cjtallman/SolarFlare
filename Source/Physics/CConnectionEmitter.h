/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CConnectionEmitter.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 03/16/2009 @ 22:00:08 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_CCONNECTIONEMITTER_H_
#define SF_CCONNECTIONEMITTER_H_

#include "Graphics/CBaseEmitter.h"
#include "Graphics/ParticleEngine.h"
#include "Math/CTransform.h"
#include "Utilities/Color.h"
#include "Utilities/Random/Random.h"
#include "Utilities/MemoryManager.h"

namespace SF {
namespace Physics
{

using namespace Math;
using namespace ParticleSystem;

class CConnectionEmitter : public ParticleSystem::CBaseEmitter, public MemoryManagerBase<CConnectionEmitter>
{
public:
  CConnectionEmitter();

  virtual void Update(float Elapsed);

  BaseComponent *Clone() const;
  bool InitDependencies( Engine::Entity * entity );

  static Engine::BaseComponent * Creator( const ticpp::Element * element );

  virtual void PushToLua( lua_State *ls );

private:
  struct ParticleBehavior : public BaseParticleBehavior, public MemoryManagerBase<ParticleBehavior, 1024>
  {
    ParticleBehavior(float lifetime);
    virtual ~ParticleBehavior() { }

    virtual bool Update(float elapsed, Particle& particle);

    float   t;
    float   dt;
    Engine::EntityID entityID1;
    Engine::EntityID entityID2;
  };

  struct Connection
  {
    Connection(unsigned int _id1, unsigned int _id2) 
      : id1(_id1), id2(_id2) {}
    unsigned int  id1;
    unsigned int  id2;
  };

  bool DoCulling(const Vector3& worldPosition);
  bool DoLOD(const Vector3& worldPosition);

  CTransform *  m_transform;
  Color         m_color;

  float         m_emissionTimer;
  float         m_emissionRate;

  Utilities::SNCG m_rand;

  std::vector<Connection> m_connections;

  LUA_DECLARE_CLASS( CConnectionEmitter )
  LUA_DECLARE_METHOD( LAddConnection )
  LUA_DECLARE_METHOD( LRemoveConnection )
  LUA_DECLARE_METHOD( LSetColor )
};

}

}

#endif
