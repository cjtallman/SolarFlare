/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    AABB.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 17:20:35 - pmera
**  \brief   
*******************************************************************************/

#include <cmath>

#include "AABB.h"

#include "OBB.h"
#include "Plane.h"
#include "Polygon.h"
#include "Range.h"
#include "Sphere.h"
#include "Triangle.h"
#include "TVector3.h"
#include "TVector3.h"

#include "Utilities/Misc.h"

namespace SF
{

namespace Math
{

AABB::AABB()
: m_scale(1.0f, 1.0f, 1.0f)
{
}

AABB::AABB(const Vector3& sv)
: m_scale(sv)
{
}

AABB::AABB(float sx, float sy, float sz)
: m_scale(sx, sy, sz)
{
}

bool AABB::Contains(const Sphere& sphere) const
{
  AABB internal_aabb(
    m_scale.x - sphere.GetRadius(),
    m_scale.y - sphere.GetRadius(),
    m_scale.z - sphere.GetRadius());
  internal_aabb.SetPosition(*m_position);

  return internal_aabb.Contains(sphere.GetPosition());
}

bool AABB::Contains(const Vector3& point) const
{
  Vector3 localPoint = Vector3() + (point - GetWorldPosition());

  return LocalContainment(m_scale, localPoint);
}

bool AABB::ContainedBy(const BoundingVolume& obj) const
{
  return obj.Contains(*this);
}

bool AABB::Intersection(const Line3& line, float& time) const
{
  Line3 localLine(Vector3() + (line.m_start - GetWorldPosition()), line.m_vector);

  return LocalIntersection(m_scale, localLine, time);
}

void AABB::GetWorldVertex(int i, Vector3& point) const
{
  float X = (i % 2 == 0) ? m_scale.x : -m_scale.x;
  float Y = (i % 4 < 2) ? m_scale.y : -m_scale.y;
  float Z = (i < 4) ? m_scale.z : -m_scale.z;

  point = Vector3(X, Y, Z) + GetWorldPosition();
}

bool AABB::LocalContainment(const Vector3& scale, const Vector3& localPoint)
{
  float epsilon = 0.001f;

  if (localPoint.x + scale.x < -epsilon || localPoint.x - scale.x > epsilon)
    return false;
  if (localPoint.y + scale.y < -epsilon || localPoint.y - scale.y > epsilon)
    return false;
  if (localPoint.z + scale.z < -epsilon || localPoint.z - scale.z > epsilon)
    return false;

  return true;
}

bool AABB::LocalIntersection(const Vector3& scale, const Line3& localLine, float& time)
{
  if( LocalContainment(scale, localLine.m_start) )
  {
    time = 0.0f;
    return true;
  }

  Plane boxPlane[6] = 
  {
    Plane( Vector3(scale.x, 0, 0),  Vector3(1.0f, 0, 0) ),
    Plane( Vector3(-scale.x, 0, 0), Vector3(-1.0f, 0, 0) ),
    Plane( Vector3(0, scale.y, 0),  Vector3(0, 1.0f, 0) ),
    Plane( Vector3(0, -scale.y, 0), Vector3(0, -1.0f, 0) ),
    Plane( Vector3(0, 0, scale.z),  Vector3(0, 0, 1.0f) ),
    Plane( Vector3(0, 0, -scale.z), Vector3(0, 0, -1.0f) )
  };

  float planeTime[6];
  Range<float> slabRange[3];

  for (int i = 0; i < 3; i += 2)
  {
    bool intersection = true;

    intersection &= boxPlane[i].Intersection(localLine, planeTime[i]);
    intersection &= boxPlane[i + 1].Intersection(localLine, planeTime[i + 1]);

    if(!intersection)
    {
      if( boxPlane[i].Contains(localLine.m_start) &&
          boxPlane[i + 1].Contains(localLine.m_start) )
      {
        slabRange[i].infinite = true;
      }
      else
      {
        slabRange[i].empty = true;
      }
    }
    else
    {
      slabRange[i].lowerBound = Utilities::Min( planeTime[i], planeTime[i+1] );
      slabRange[i].lowerBound = Utilities::Max( planeTime[i], planeTime[i+1] );

      if(i == 0)
        time = planeTime[i];

      if( std::abs(planeTime[i]) < time )
        time = planeTime[i];
      if( std::abs(planeTime[i+1]) < time )
        time = planeTime[i+1];
    }
  }

  if( slabRange[0].Intersection( slabRange[1] ) &&
      slabRange[1].Intersection( slabRange[2] ) &&
      slabRange[0].Intersection( slabRange[2] ) )
  {
    return true;
  }
  else
    return false;
}

} // namespace Math

} // namespace SF
