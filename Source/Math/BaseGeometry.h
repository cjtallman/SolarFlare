/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseGeometry.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 14:01:49 - pmera
**  \brief   Base geometry class.
             Common geometry functionality:
               -Contained by boundingvolumes
               -Intersection with rays
               -Intersection with other geometry
*******************************************************************************/

#ifndef SF_BASEGEOMETRY_H_
#define SF_BASEGEOMETRY_H_

#include <vector>

#include "TVector3.h"

namespace SF {
namespace Math
{

class BoundingVolume;
struct Line3;
struct LineSegment;
struct Ray;
struct Quaternion;

class BaseGeometry
{
public:
  BaseGeometry(BaseGeometry *parent = 0);
  ~BaseGeometry();

  void Init();
  void Copy(const BaseGeometry& rhs);

  virtual bool ContainedBy(const BoundingVolume& bv) const;

  virtual bool Intersection(const Line3&, float&) const { return false; }// = 0;
  virtual bool Intersection(const BaseGeometry& geometry) const;

  bool Intersection(const Ray& ray, float& time) const;
  bool Intersection(const LineSegment& segment, float& time) const;

  void AddChild(BaseGeometry *child);
  void RemoveChild(BaseGeometry *child);

  const Vector3&     GetPosition() const;
  const Quaternion& GetOrientation() const;

  Vector3      GetWorldPosition() const;
  Quaternion  GetWorldOrientation() const;

  void SetPosition(Vector3 *pos);
  void SetPosition(const Vector3& pos);

  void SetOrientation(Quaternion *q);
  void SetOrientation(const Quaternion& q);

protected:
  typedef std::vector<BaseGeometry *> GeometryList;


  Vector3 *    m_position;
  bool        m_position_owned;

  Quaternion *m_orientation;
  bool        m_orientation_owned;

  BaseGeometry *m_parent;
  GeometryList  m_children;
};

}
}

#endif
