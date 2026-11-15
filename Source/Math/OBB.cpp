/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    OBB.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 18:05:27 - pmera
**  \brief   
*******************************************************************************/

#include "OBB.h"

#include "AABB.h"
#include "Sphere.h"

#include "Plane.h"

#include "Triangle.h"
#include "Polygon.h"

namespace SF
{

namespace Math
{

OBB::OBB()
: m_scale(1.0f, 1.0f, 1.0f)
{
}

OBB::OBB(const Vector3& scale)
: m_scale(scale)
{
}

OBB::OBB(float sx, float sy, float sz)
: m_scale(sx, sy, sz)
{
}

bool OBB::Intersection(const Line3& line, float& time) const
{
  Line3 localLine(Vector3() + (line.m_start - GetWorldPosition()), line.m_vector);

  Quaternion worldOrientation = GetWorldOrientation();

  localLine.m_start.RotateInverse( worldOrientation );
  localLine.m_vector.RotateInverse( worldOrientation );

  return AABB::LocalIntersection(m_scale, localLine, time);
}

bool OBB::Contains(const Sphere& sphere) const
{
  OBB internal_obb(
    m_scale.x - sphere.GetRadius(),
    m_scale.y - sphere.GetRadius(),
    m_scale.z - sphere.GetRadius());
  internal_obb.SetPosition( GetWorldPosition() );
  internal_obb.SetOrientation( GetWorldOrientation() );

  return internal_obb.Contains( sphere.GetWorldPosition() );
}

bool OBB::Contains(const Vector3& point) const
{
    // The point relative to the box
  Vector3 localPoint = Vector3() + (point - GetWorldPosition());

    // Apply the inverse rotation of the box on the point to bring the point into local box space
  localPoint.RotateInverse( GetWorldOrientation() );

  return AABB::LocalContainment(m_scale, localPoint);
}

void OBB::GetWorldVertex(int i, Vector3& point) const
{
  if(i > 7)
    throw("OBB::GetWorldVertex - subscript out of range.");

  float X = (i % 2 == 0) ? m_scale.x : -m_scale.x;
  float Y = (i % 4 < 2) ? m_scale.y : -m_scale.y;
  float Z = (i < 4) ? m_scale.z : -m_scale.z;

  point = Vector3(X, Y, Z);

  point.Rotate( GetWorldOrientation() );

  point += GetWorldPosition();
}

}

}
