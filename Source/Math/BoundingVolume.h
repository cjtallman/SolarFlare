/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BoundingVolume.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 14:02:36 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_BOUNDINGVOLUME_H_
#define SF_BOUNDINGVOLUME_H_

#include "TVector3.h"
#include "Line3.h"

#include "BaseGeometry.h"

namespace SF {
namespace Math
{

class AABB;
class Sphere;
class OBB;
struct Polygon;
struct Triangle;

class BoundingVolume : public BaseGeometry
{
public:
  virtual bool Contains(const Vector3& point) const = 0;
  virtual bool Contains(const Sphere& bs) const = 0;

  bool Contains(const BaseGeometry& obj) const;

  bool Contains(const AABB& aabb) const;
  bool Contains(const OBB& obb) const;
  bool Contains(const Polygon& polygon) const;
  bool Contains(const Triangle& triangle) const;
};

} // namespace Math

} // namespace SF

#endif
