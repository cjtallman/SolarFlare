/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BoundingVolume.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 19:02:40 - pmera
**  \brief   
*******************************************************************************/

#include "BoundingVolume.h"

#include "AABB.h"
#include "OBB.h"
#include "Polygon.h"
#include "Triangle.h"

namespace SF
{

namespace Math
{
  bool BoundingVolume::Contains(const BaseGeometry& obj) const
  {
    return obj.ContainedBy( *this );
  }

  bool BoundingVolume::Contains(const AABB& aabb) const
  {
    if( !Contains(aabb.GetWorldPosition()) )
      return false;

    Vector3 point;

    for(int i = 0; i < 8; ++i)
    {
      aabb.GetWorldVertex(i, point);

      if( !Contains(point) )
        return false;
    }

    return true;
  }

  bool BoundingVolume::Contains(const OBB& obb) const
  {
    if( !Contains(obb.GetPosition()) )
      return false;

    Vector3 point;

    for(int i = 0; i < 8; ++i)
    {
      obb.GetWorldVertex(i, point);

      if( !Contains(point) )
        return false;
    }

    return true;
  }

  bool BoundingVolume::Contains(const Polygon& polygon) const
  {
    for(unsigned int i = 0; i < polygon.Points.size(); ++i)
    {
      if( !Contains(polygon.GetWorldVertex(i)) )
        return false;
    }

    return true;
  }

  bool BoundingVolume::Contains(const Triangle& triangle) const
  {
    for(unsigned char i = 0; i < 3; ++i)
    {
      if( !Contains(triangle.GetWorldVertex(i)) )
        return false;
    }

    return true;
  }
}

}
