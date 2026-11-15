/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Polygon.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 17:36:43 - pmera
**  \brief   
*******************************************************************************/

#include "Polygon.h"

#include "BoundingVolume.h"

namespace SF {
namespace Math
{

Vector3 Polygon::GetWorldVertex(unsigned int index) const
{
  if(index >= Points.size())
    throw("Triangle::GetVertex - subscript out of range.");

  Vector3 pt = Points[index];

  pt.Rotate( GetWorldOrientation() );

  pt += GetWorldPosition();

  return pt;
}

bool Polygon::ContainedBy(const BoundingVolume& bv)
{
  for(unsigned int i = 0; i < Points.size(); ++i)
  {
    if( !bv.Contains(Points[i]) )
      return false;
  }

  return true;
}

bool Polygon::Intersection(const Line3& line, float& time) const
{
  Line3 local_line = line;

  // Define the line in the triangle's coordinate system
  local_line.m_start = line.m_start - GetWorldPosition();
  local_line.m_start.RotateInverse( GetWorldOrientation() );
  local_line.m_vector.RotateInverse( GetWorldOrientation() );

  Vector3 e1 = Points[1] - Points[0];
  Vector3 e2 = Points[Points.size() - 1] - Points[0];
  
  Vector3 polygon_normal = e1.Cross(e2);

  Plane polygon_plane(Points[0], polygon_normal);

  if(polygon_plane.Intersection(local_line, time))
  {
    return Contains( local_line(time) );
  }

  return false;
}

bool Polygon::Intersection(const Ray& ray, float& time) const
{
  if(Intersection(ray.ToLine(), time) && time >= 0)
    return true;
  else
    return false;
}

bool Polygon::Intersection(const LineSegment& segment, float& time) const
{
  if(Intersection(segment.ToLine(), time) && time >= 0 && time <= 1.0f)
    return true;
  else
    return false;
}

bool Polygon::Intersection(const Polygon&, float&) const
{
  Vector3 e1 = Points[1] - Points[0];
  Vector3 e2 = Points[Points.size() - 1] - Points[0];
  
  Vector3 polygon_normal = e1.Cross(e2);

  // todo: finish intersection using SAT

  return false;
}

bool Polygon::Contains(const Vector3& point) const
{
  Vector3 local_point = (point - GetWorldPosition());

  Vector3 e1 = Points[1] - Points[0];
  Vector3 e2 = Points[Points.size() - 1] - Points[0];
  
  Vector3 polygon_normal = e2.Cross(e1);

  for(unsigned int i = 0; i < Points.size(); ++i)
  {
    Vector3 edge = Points[(i + 1) % Points.size()] - Points[i];
    Plane p(Points[i], edge.Cross(polygon_normal));

    if(!p.Contains(local_point))
      return false;
  }

  return true;
}

}
}
