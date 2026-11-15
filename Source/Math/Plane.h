/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Plane.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 08/27/2008 @ 18:19:40 - nwilliam
**  \brief   A 3D plane, defined with a point and a normal.
*******************************************************************************/

#ifndef SF_PLANE_H
#define SF_PLANE_H

#include "TVector3.h"

#include "Line3.h"
#include "LineSegment.h"
#include "Ray.h"

namespace SF {
namespace Math
{

class BoundingVolume;

struct Plane
{
  Vector3 m_point;
  Vector3 m_normal;

  Plane();
  Plane( const Math::Vector3& point, const Math::Vector3& normal );
  
  bool Contains(const Vector3& point) const;

  bool Intersection(const Plane& plane, Line3& line) const;

  virtual bool ContainedBy(const BoundingVolume& bv) const;

  virtual bool Intersection(const Line3& line, float& time) const;
  virtual bool Intersection(const Ray& ray, float& time) const;
  virtual bool Intersection(const LineSegment& segment, float& time) const;
};

} // Math namespace
} // SF namespace

#endif
