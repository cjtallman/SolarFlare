/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    AABB.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 17:20:30 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_AABB_H_
#define SF_AABB_H_

#include "BoundingVolume.h"

#include "TVector3.h"

namespace SF
{

namespace Math
{

class AABB : public BoundingVolume
{
public:
  AABB();
  AABB(const Vector3& sv);
  AABB(float sx, float sy, float sz);

  virtual bool  Contains(const Vector3& point) const;
  virtual bool  Contains(const Sphere& bs) const;

  virtual bool  ContainedBy(const BoundingVolume& obj) const;

  virtual bool  Intersection(const Line3& ray, float& time) const;

  void          GetWorldVertex(int i, Vector3& point) const;

  static bool   LocalContainment(const Vector3& scale, const Vector3& localPoint);
  static bool   LocalIntersection(const Vector3& scale, const Line3& localLine, float& time);
private:
  Vector3 m_scale;
};

} // namespace Math

} // namespace SF

#endif
