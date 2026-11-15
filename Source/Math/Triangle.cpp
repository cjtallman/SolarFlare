/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Triangle.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 12:58:26 - pmera
**  \brief   
*******************************************************************************/

#include "Triangle.h"

#include "Plane.h"

#include "BoundingVolume.h"

namespace SF {
namespace Math
{
  Triangle::Triangle(const Vector3& _v1, const Vector3& _v2, const Vector3& _v3)
  {
    V[0] = _v1;
    V[1] = _v2;
    V[2] = _v3;
  }

  Vector3 Triangle::GetWorldVertex(unsigned char index) const
  {
    if(index > 2)
      throw("Triangle::GetVertex - subscript out of range.");
    
    Vector3 pt = V[index];

    pt.Rotate( GetWorldOrientation() );

    pt += GetWorldPosition();

    return pt;
  }

  /* Checks if a point (projected onto the triangle's plane) is inside the triangle. */
  bool Triangle::Contains(const Vector3& point) const
  {
    Vector3 local_point = (point - GetWorldPosition());

    Vector3 e1 = V[1] - V[0];
    Vector3 e2 = V[2] - V[1];
    Vector3 e3 = V[0] - V[2];
    
    Vector3 triangle_normal = e1.Cross(V[2] - V[0]);

    // Planes formed by the three sides of triangle (normals are outward)
    Plane p1(V[0], e1.Cross(triangle_normal));
    Plane p2(V[1], e2.Cross(triangle_normal));
    Plane p3(V[2], e3.Cross(triangle_normal));

    if( p1.Contains(local_point) &&
        p2.Contains(local_point) &&
        p3.Contains(local_point))
    {
      return true;
    }
    else
    {
      return false;
    }

  }

  bool Triangle::ContainedBy(const BoundingVolume& bv) const
  {
    for(unsigned char i = 0; i < 3; ++i)
    {
      if( !bv.Contains( GetWorldVertex(i) ) )
        return false;
    }

    return true;
  }

  /* Note: currently would not work if ray/triangle are coplanar */
  bool Triangle::Intersection(const Line3& line, float& time) const
  {
    Line3 local_line = line;

    // Define the line in the triangle's coordinate system
    local_line.m_start = line.m_start - GetWorldPosition();
    local_line.m_start.RotateInverse( GetWorldOrientation() );
    local_line.m_vector.RotateInverse( GetWorldOrientation() );

    Vector3 e1 = V[1] - V[0];
    Vector3 e2 = V[2] - V[0];

    Vector3 triangle_normal = e1.Cross(e2);

    Plane triangle_plane(V[0], triangle_normal);

    if(triangle_plane.Intersection(local_line, time))
    {
      return Contains( local_line(time) );
    }

    return false;
  }

  /* Determine whether two triangles intersect. */
  bool Triangle::Intersection(const Triangle& other) const
  {
    for(int i = 0; i < 3; ++i)
    {
      Line3 line(V[i], V[(i+1) % 3] - V[i]);

      float time;
      if(other.Intersection(line, time) && time >= 0.0f && time <= 1.0f)
      {
        return true;
      }
    }

    for(int i = 0; i < 3; ++i)
    {
      Line3 line(other.V[i], other.V[(i+1) % 3] - other.V[i]);

      float time;
      if(Intersection(line, time) && time >= 0.0f && time <= 1.0f)
      {
        return true;
      }
    }

    return false;
  }

  bool Triangle::Intersection(const Plane& plane) const
  {
    bool contained[3] = 
    {
      plane.Contains(V[0]),
      plane.Contains(V[1]),
      plane.Contains(V[2])
    };

    return !(contained[0] == contained[1] && contained[1] == contained[2]);
  }
}
}
