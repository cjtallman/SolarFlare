/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    PhysicsEngine.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:44:08 - nwilliam
**  \brief   Engine controlling movement, collision, all physics.
*******************************************************************************/

#ifndef SF_PHYSICSENGINE_H
#define SF_PHYSICSENGINE_H

#include <map>
#include <queue>
#include <vector>
#include <cmath>

#include "VectorField.h"

#include "Engine/BaseEngine.h"
#include "Engine/EntityFactory.h" 
#include "Math/Range.h"
#include "Math/Ray.h"
#include "SolarFlare/Globals.h"

namespace SF {
namespace Physics
{


class CPhysics;
class COrbitalPhysics;


struct VolumeExtent
{
  VolumeExtent(Engine::EntityID id, Math::Range<float> *rng) : entityID(id), range(rng) {}

  Math::Range<float> *range;
  Engine::EntityID    entityID;

  bool operator < (const VolumeExtent& rhs) const { return range->lowerBound < rhs.range->lowerBound; }
  bool operator <= (const VolumeExtent& rhs) const { return range->lowerBound <= rhs.range->lowerBound; }
};

struct PhysicsGroup
{
  PhysicsGroup() : linearTimeScale(1.0f), rotationalTimeScale(1.0f) {}

  std::vector< std::pair<unsigned int, float> > gravitySources;
  std::vector<Engine::EntityID>                 objects;
  std::vector<CPhysics *>                       components;
  
  std::vector<bool>                             collidesWith;
  
  float                                         linearTimeScale;
  float                                         rotationalTimeScale;
};

class PhysicsEngine : public Engine::BaseEngine
{
public:
  enum IntegratorType { EULER_EXPLICIT, EULER_IMPLICIT, RK4, VERLET };

  typedef std::map<std::string, unsigned int> GroupMap;
  typedef std::vector<PhysicsGroup>           GroupList;
  typedef std::vector<Engine::EntityID>       ObjectList;
  typedef std::pair<unsigned int, float>      GravitySource;

  typedef std::vector<VolumeExtent>::iterator VolumeExtentIter;
  typedef std::pair<VolumeExtentIter, VolumeExtentIter> VolumeExtentIterPair;

  PhysicsEngine();
  
  unsigned int GroupID( const std::string& groupName );

  void SetGravitySource( unsigned int sourceID, unsigned int groupID, float coefficient );

  void AddPhysicsEntity( int entity_id );
  void AddEmitterEntity( int entity_id );

  void RemovePhysicsEntity( int entityID );
  void RemoveEmitterEntity( int entityID );

  Math::Vector3 ForceField(const Math::Vector3& position);
  Math::Vector3 GetGravForce(const Math::Vector3& position);
  Math::Vector3 GetGravityForce(unsigned int groupID, const Math::Vector3& objPosition, float objMass);
  Math::Vector3 GetDampingForce(const Math::Vector3& objVelocity, float objMass, float drag) const;

  CPhysics *GetComponent(Engine::EntityID entityID);
  Engine::Entity *GetEntity(const Math::Ray& ray, float *intersectionTime, const std::string& groupName) const;

  void CheckAllCollisions();

  float GetTimestep();
  void SetTimestep(float timestep);
  void SetLinearTimeScale(const std::string& groupName, float timeScale);
  void SetRotationalTimeScale(const std::string& groupName, float timeScale);
  void SetIntegrator(IntegratorType type);

  void SortExtents();

  void Update(float elapsed);
  void UpdatePhysics();
  void UpdateEmitters( float elapsed );

  void Pause();
  void Unpause();
  void TogglePause();
  bool IsPaused();

private:
  GroupMap  m_groupMap;
  GroupList m_groups;

  Engine::EntityIDList m_physicsEntities;
  Engine::EntityIDList m_emitter_entities;

  Engine::EntityIDList m_deletedEntities;

  IntegratorType  m_integrator;

  float m_timeStep;
  float m_integrationTime; // time available for us to integrate

  bool  m_pause;

  std::vector<VolumeExtent> m_extents;

  VectorField m_forceField;

  LUA_DECLARE_CLASS( PhysicsEngine )

  LUA_DECLARE_METHOD( LBuildForceField )

  LUA_DECLARE_METHOD( LSetGravitySource )
  LUA_DECLARE_METHOD( LGetEntity )
  LUA_DECLARE_METHOD( LRemoveEntity )
  LUA_DECLARE_METHOD( LSetCollision )
  LUA_DECLARE_METHOD( LSetLinearTimeScale )
  LUA_DECLARE_METHOD( LSetRotationalTimeScale )
  LUA_DECLARE_METHOD( LSetIntegrator )
};


} // namespace Physics
} // SF namespace

#endif
