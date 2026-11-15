/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*!\file    OBB.h
   \author  Patrick Mera
   \par     Updated:    07/15/2008 @ 17:53:01 - pmera
   \brief   A 3D oriented bounding box.
            TODO: -Intersection with OBB, ABB, Sphere, Polygon, Line/ray/segment
*******************************************************************************/

#ifndef SF_OBB_H_
#define SF_OBB_H_

#include "BoundingVolume.h"

namespace SF
{

namespace Math
{

class OBB : public BoundingVolume
{
public:
  OBB();
  OBB(const Vector3& scale);
  OBB(float sx, float sy, float sz);

  virtual bool Intersection(const Line3& line, float& time) const;

  virtual bool Contains(const Vector3& point) const;
  virtual bool Contains(const Sphere& bs) const;

  void GetWorldVertex(int i, Vector3& point) const;

private:

  Vector3 m_scale;
};

}

}

#endif
