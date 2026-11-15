/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Plane.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 08/27/2008 @ 18:20:17 - nwilliam
**  \brief   Implementation of 3D plane
*******************************************************************************/

#include "Plane.h"

namespace SF {
namespace Math
{

Plane::Plane() {}

Plane::Plane(const Math::Vector3& point, const Math::Vector3& normal)
: m_point(point), m_normal(normal)
{
  m_normal.Normalize();
}

  // Test whether a point is "inside" the plane; normal is considered to be outward
bool Plane::Contains(const Vector3& point) const
{
  float d = (point - m_point) * m_normal;

  return d <= 0;
}

bool Plane::ContainedBy(const BoundingVolume&) const
{
  return false;
}

bool Plane::Intersection(const Line3& line, float& time) const
{
  if (m_normal * line.m_vector == 0)
  {
    time = 0.0f;
    return false;
  }

  time = (m_normal * (m_point - line.m_start)) / (m_normal * line.m_vector);

  return true;
}

bool Plane::Intersection(const Ray& ray, float& time) const
{
  if(Intersection(ray.ToLine(), time) && time > 0)
    return true;
  else
    return false;
}

bool Plane::Intersection(const LineSegment& segment, float& time) const
{
  if(Intersection(segment.ToLine(), time) && time > 0 && time < 1.0f)
    return true;
  else
    return false;
}

/* Doesn't really work in all cases right now. */
bool Plane::Intersection(const Plane& other, Line3& line) const
{
  // Only case of non-intersection is parallel planes
  if(m_normal * other.m_normal == 1.0f)
    return false;

  line.m_vector = m_normal.Cross(other.m_normal);

  Vector3& intersection_point = line.m_start;

  Vector3 p1 = m_point;
  Vector3 p2 = other.m_point;

  const Vector3& n1 = m_normal;
  const Vector3& n2 = other.m_normal;

  intersection_point.z = 0;
  intersection_point.y =
    (p2 * n2 - p1 * n1 * (n2.y / n1.x) ) /
    (n2.y - n1.y * (n2.y / n1.x));
  intersection_point.x =
    (p1 * n1 - intersection_point.y * n1.y) / n1.x;

  return false;
}

} // Math namespace
} // SF namespace
