/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    COrbitalPhysics.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 10/03/2008 @ 17:37:24 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_CORBITALPHYSICS_H_
#define SF_CORBITALPHYSICS_H_

#include "CPhysics.h"
#include "KeplerOrbit.h"

#include "Engine/BaseComponent.h"
#include "Math/CTransform.h"
#include "Math/Sphere.h"
#include "External/Lua/include/LunarHelper.h"
#include "Utilities/MemoryManager.h"

namespace ticpp
{
  class Element;
}

namespace SF {
namespace Engine { class Entity; }
namespace Math { class CTransform; }
namespace Physics
{

class COrbitalPhysics : public CPhysics, public MemoryManagerBase<COrbitalPhysics, 64>
{
public:
  MEMORY_MANAGER_SET( COrbitalPhysics, 64 )

  friend class PhysicsEngine;

  COrbitalPhysics();
  COrbitalPhysics(const COrbitalPhysics& obj);

  virtual void IntegratePosition(PhysicsEngine::IntegratorType integrator, float dt);
  virtual void Update();

  void UpdateFromKepler(float elapsed);

  void SetMass( float mass );
  void SetOrbitRotation( float yaw, float pitch, float roll );
  void SetPeriod( float period );
  void SetPerihelionDistance( float perihelion );
  void SetSunDistance( float sun );

  virtual BaseComponent* Clone() const;
  virtual bool InitDependencies( Engine::Entity *entity );
  virtual void PushToLua( lua_State *ls );

  static Engine::BaseComponent *Creator(const ticpp::Element *element);

  // Networking
  virtual void InitIn( Networking::ByteStream& bs );
  virtual void DeltaIn( Networking::ByteStream& bs );
  virtual void InitOut( Networking::ByteStream& bs ) const;
  virtual void DeltaOut( Networking::ByteStream& bs ) const;

private:
  KeplerOrbit       m_orbit;
  Math::CTransform  m_orbitTransform;

  float m_rotationPeriod;
  float m_time;
  float m_timeScale;

  LUA_DECLARE_CLASS( COrbitalPhysics )
  LUA_DECLARE_METHOD( LGetPosition )
  LUA_DECLARE_METHOD( LGetVelocity )
  
  LUA_DECLARE_METHOD( LGetPeriod )
  LUA_DECLARE_METHOD( LGetTime )
  LUA_DECLARE_METHOD( LGetMajorAxis )
  LUA_DECLARE_METHOD( LGetMinorAxis )
  LUA_DECLARE_METHOD( LGetPerihelionDistance )

  LUA_DECLARE_METHOD( LSetBoundingRadius )
  LUA_DECLARE_METHOD( LSetMass )
  LUA_DECLARE_METHOD( LSetOrientation )
  LUA_DECLARE_METHOD( LSetPerihelionDistance )
  LUA_DECLARE_METHOD( LSetPeriod )
  LUA_DECLARE_METHOD( LSetRotationalFriction )
  LUA_DECLARE_METHOD( LSetRotationPeriod )
  LUA_DECLARE_METHOD( LSetSunDistance )
  LUA_DECLARE_METHOD( LSetTime )
  LUA_DECLARE_METHOD( LSetTimeShift )

  LUA_DECLARE_METHOD( LSetTimeScale )

  LUA_DECLARE_METHOD( LTorque )
  LUA_DECLARE_METHOD( LSetRotationalVelocity )
};

}

}

#endif
