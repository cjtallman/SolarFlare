/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CPhysics.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/14/2008 @ 17:37:08 - nwilliam
**  \brief   
*******************************************************************************/

#ifndef SF_CPHYSICS_H_
#define SF_CPHYSICS_H_

#include "PhysicsEngine.h"

#include "Engine/BaseComponent.h"
#include "Math/Sphere.h"
#include "External/Lua/include/LunarHelper.h"
#include "Utilities/MemoryManager.h"

using namespace SF::Math;

namespace ticpp
{
  class Element;
}

namespace SF {
namespace Engine { class Entity; }
namespace Math { class CTransform; }
namespace Physics
{

class CPhysics : public Engine::BaseComponent, public MemoryManagerBase<CPhysics, 512>
{
public:
  MEMORY_MANAGER_SET( CPhysics, 512 )

  friend class PhysicsEngine;
  friend class SweepAndPrune;

  CPhysics();
  CPhysics(const CPhysics& obj);

  Math::Vector3 GetVelocity() const;
  Math::Vector3 GetForce( const Math::Vector3& position, const Math::Vector3& velocity ) const;


  virtual void IntegratePosition(PhysicsEngine::IntegratorType integrator, float dt);
  virtual void IntegrateOrientation(PhysicsEngine::IntegratorType integrator, float dt);
  virtual void Update();

  void UpdateExtents();
  void ThrottleVelocity();
  void ThrottleAngularVelocity();

  virtual BaseComponent* Clone() const;
  virtual bool InitDependencies( Engine::Entity *entity );
  virtual void PushToLua( lua_State *ls );

  static Engine::BaseComponent *Creator(const ticpp::Element *element);

  // Networking
  virtual void InitIn( Networking::ByteStream& bs );
  virtual void DeltaIn( Networking::ByteStream& bs );
  virtual void InitOut( Networking::ByteStream& bs ) const;
  virtual void DeltaOut( Networking::ByteStream& bs ) const;


protected:
  void IntegrateRotEulerExplicit(float dt);

  void IntegratePosEulerExplicit(float dt);
  void IntegratePosEulerImplicit(float dt);
  void IntegratePosRK4(float dt);
  void IntegratePosVerlet(float dt);

  std::string       m_groupName;
  unsigned int      m_groupID;

  CTransform *      m_transform;
  Sphere            m_boundingSphere;

  float             m_linearFriction;
  float             m_rotationalFriction;

  float             m_mass;
  float             m_inverseMass;

  float             m_inertia;
  float             m_invInertia;

  Vector3           m_velocity;
  Vector3           m_acceleration;

  Vector3           m_angularMomentum;
  Vector3           m_angularVelocity;
  Vector3           m_torque;

  Vector3           m_forceAccum;
  Vector3           m_constForce;

  bool              m_orientFromVelocity;

  float             m_maxVelocity;

  Math::Range<float>  m_range[2];

  std::vector< Engine::EntityID > m_collisions;

  // Lua functions
  LUA_DECLARE_CLASS( CPhysics )
  LUA_DECLARE_METHOD( LGetVelocity )
  LUA_DECLARE_METHOD( LSetBoundingRadius )
  LUA_DECLARE_METHOD( LSetLinearFriction )
  LUA_DECLARE_METHOD( LSetMass )
  LUA_DECLARE_METHOD( LGetMass )
  LUA_DECLARE_METHOD( LSetMaxVelocity )
  LUA_DECLARE_METHOD( LSetOrientation )
  LUA_DECLARE_METHOD( LSetOrientationMode )
  LUA_DECLARE_METHOD( LSetPosition )
  LUA_DECLARE_METHOD( LSetRotationalFriction )
  LUA_DECLARE_METHOD( LSetVelocity )

  LUA_DECLARE_METHOD( LApplyImpulse )
  LUA_DECLARE_METHOD( LApplyForce )
  LUA_DECLARE_METHOD( LTorque )

  LUA_DECLARE_METHOD( LGetConstForce )
  LUA_DECLARE_METHOD( LSetConstForce )
};

}

}

#endif
