/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Sphere.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 18:05:09 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_SPHERE_H_
#define SF_SPHERE_H_

#include "BoundingVolume.h"

namespace SF
{

namespace Math
{

class Sphere : public BoundingVolume
{
public:
  Sphere();
  Sphere(float radius);

  virtual bool Contains(const Vector3& point) const;
  virtual bool Contains(const Sphere& bs) const;

  virtual bool ContainedBy(const BoundingVolume& bv) const;

  virtual bool Intersection(const Sphere& rhs) const;

  virtual bool Intersection(const Line3& ray, float& time);
  virtual bool Intersection(const Ray& ray, float& time);
  virtual bool Intersection(const LineSegment& ray, float& time);

  float GetRadius() const;
  void  SetRadius(float radius);

private:
  float m_radius;
};

}

}

#endif
