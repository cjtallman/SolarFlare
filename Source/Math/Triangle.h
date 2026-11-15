/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Triangle.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 12:58:35 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_TRIANGLE_H_
#define SF_TRIANGLE_H_

#include "BaseGeometry.h"

#include "Line3.h"
#include "TVector3.h"

namespace SF {
namespace Math
{

class BoundingVolume;

struct Triangle : public BaseGeometry
{
  Vector3  V[3];

  Triangle(const Vector3& _v1, const Vector3& _v2, const Vector3& _v3);

  Vector3 GetWorldVertex(unsigned char index) const;

  virtual bool ContainedBy(const BoundingVolume& bv) const;

  virtual bool Intersection(const Line3& line, float& time) const;

  virtual bool Intersection(const Plane& plane) const;
  virtual bool Intersection(const Triangle& other) const;

  //virtual bool Intersection(const BaseGeometry& geometry) const;

  bool Contains(const Vector3& point) const;

  

  
};

} // namespace Math

} // namespace SF

#endif
