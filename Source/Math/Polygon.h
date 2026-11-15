/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Polygon.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 17:23:07 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_POLYGON_H_
#define SF_POLYGON_H_

#include <vector>

#include "BaseGeometry.h"

#include "TVector3.h"
#include "Plane.h"

namespace SF {
namespace Math
{

struct Polygon : public BaseGeometry
{
  std::vector<Vector3> Points;

  Vector3 GetWorldVertex(unsigned int index) const;

  virtual bool ContainedBy(const BoundingVolume& bv);

  virtual bool Intersection(const Line3& line, float& time) const;
  virtual bool Intersection(const Ray& ray, float& time) const;
  virtual bool Intersection(const LineSegment& segment, float& time) const;

  virtual bool Intersection(const Polygon& polygon, float& time) const;

  bool Contains(const Vector3& point) const;
};

}
}

#endif
