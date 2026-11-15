/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Sphere.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 17:20:51 - pmera
**  \brief   
*******************************************************************************/

#include "Utilities/Misc.h"

#include "Line3.h"
#include "Ray.h"
#include "LineSegment.h"

#include "AABB.h"
#include "OBB.h"
#include "Sphere.h"

namespace SF
{

namespace Math
{

Sphere::Sphere()
{
  m_radius = 1.0f;
}

Sphere::Sphere(float radius)
{
  m_radius = radius;
}

bool Sphere::Contains(const Vector3& point) const
{
  Vector3 v = point - *m_position;

  if(v.MagnitudeSquared() > m_radius * m_radius)
    return false;
  else
    return true;
}

bool Sphere::Contains(const Sphere& bs) const
{
  Vector3 v = bs.GetPosition() - *m_position;

  if(v.MagnitudeSquared() < m_radius * m_radius - 2 * m_radius * bs.GetRadius() + bs.GetRadius() * bs.GetRadius())
    return true;
  else
    return false;
}

bool Sphere::ContainedBy(const BoundingVolume& bv) const
{
  return bv.Contains(*this);
}

bool Sphere::Intersection(const Sphere& rhs) const
{
  Math::Vector3 v = GetWorldPosition() - rhs.GetWorldPosition();
  float R = m_radius + rhs.m_radius;

  return v * v < R * R;
}

bool Sphere::Intersection(const Line3& ray, float& time)
{
  // Ray starts in sphere
  if(Contains(ray.m_start))
  {
    time = 0.0f;
    return true;
  }

  Vector3 OriginToSphere = *m_position - ray.m_start;

  float a = ray.m_vector * ray.m_vector;
  float b = -2 * (OriginToSphere * ray.m_vector);
  float c = OriginToSphere * OriginToSphere - m_radius * m_radius;

  float discriminant = b*b - 4*a*c;

  // No intersection
  if (discriminant < 0)
  {
    time = 0.0f;
    return false;
  }

  // Single intersection
  if ( std::fabs(discriminant) < 0.0001f)
  {
    time = (-b) / (2 * a);
    return true;
  }

  float sqrt_discriminant = std::sqrt(discriminant);

  float t1 = (-b + sqrt_discriminant) / (2*a);
  float t2 = (-b - sqrt_discriminant) / (2*a);

  float t = Utilities::Min(t1, t2);

  if (t >= 0)
  {
    time = t;
    return true;
  }
  else
  {
    time = 0.0f;
    return false;
  }
}

bool Sphere::Intersection(const Ray& ray, float& time)
{
  if(Intersection(ray.ToLine(), time) && time >= 0)
    return true;
  else
    return false;
}

bool Sphere::Intersection(const LineSegment& segment, float& time)
{
  if(Intersection(segment.ToLine(), time) && time >= 0 && time <= 1.0f)
    return true;
  else
    return false;
}

float Sphere::GetRadius() const
{
  return m_radius;
}

void Sphere::SetRadius(float radius)
{
  m_radius = radius;
}

}

}
