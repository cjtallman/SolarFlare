/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Quad.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/12/2008 @ 17:39:21 - pmera
**  \brief   Geometric quad implementation.
*******************************************************************************/

#include "Quad.h"

#include "Quaternion.h"
#include "Plane.h"

namespace SF {
namespace Math
{

Quad::Quad(float _width, float _height)
{
  width = _width;
  height = _height;
}

bool Quad::Intersection(const Line3& line, float& time) const
{
  // Normal of a quad on the xy plane (standard orientation)
  Vector3 Z(0, 0, 1.0f);

  Z.Rotate(GetWorldOrientation());

  Plane plane(GetWorldPosition(), Z);

  if( plane.Intersection(line, time) )
  {
    return Contains( line(time) );
  }

  return false;
}

bool Quad::Intersection(const Ray& ray, float& time) const
{
  if(Intersection(ray.ToLine(), time) && time >= 0)
    return true;
  else
    return false;
}

bool Quad::Intersection(const LineSegment& segment, float& time) const
{
  if(Intersection(segment.ToLine(), time) && time >= 0 && time <= 1.0f)
    return true;
  else
    return false;
}

bool Quad::Contains(const Vector3& point) const
{
  Quaternion orientation = GetWorldOrientation();

  /* Rotate the point into the quad's coordinate system;
     now we are dealing with a 2D AABB. */
  Vector3 local_point = point - GetWorldPosition();
  local_point.RotateInverse(orientation);

  if(local_point.x >= width * .5f || local_point.x <= -width * .5f)
    return false;
  if(local_point.y >= height * .5f || local_point.y <= -height * .5f)
    return false;

  return true;
}

bool Quad::Contains(const Vector3& point, Vector2& offset) const
{
  Quaternion orientation = GetWorldOrientation();

  /* Rotate the point into the quad's coordinate system;
     now we are dealing with a 2D AABB. */
  Vector3 local_point = point - GetWorldPosition();
  local_point.RotateInverse(orientation);

  offset = Vector2(local_point.x, local_point.y);

  if(local_point.x > width * .5f || local_point.x < -width * .5f)
    return false;
  if(local_point.y > height * .5f || local_point.y < -height * .5f)
    return false;


  return true;
}

}

}
