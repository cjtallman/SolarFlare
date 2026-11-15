/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Quad.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/12/2008 @ 17:39:10 - pmera
**  \brief   Geometic quad interface.
*******************************************************************************/

#ifndef SF_QUAD_H_
#define SF_QUAD_H_

#include "BaseGeometry.h"
#include "TVector3.h"
#include "TVector2.h"
#include "TVector3.h"

namespace SF {
namespace Math
{

struct Quad : public BaseGeometry
{
public:
  Quad(float _width, float _height);

  virtual bool Intersection(const Line3& line, float& time) const;

  bool Intersection(const Ray& ray, float& time) const;
  bool Intersection(const LineSegment& segment, float& time) const;

  virtual bool Contains(const Vector3& point) const;

  bool Contains(const Vector3& point, Vector2& offset) const;

  float width;
  float height;
};

}

}

#endif
