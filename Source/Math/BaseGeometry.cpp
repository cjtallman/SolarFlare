/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseGeometry.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 14:20:18 - pmera
**  \brief   
*******************************************************************************/

#include <algorithm>

#include "BaseGeometry.h"
#include "BoundingVolume.h"
#include "LineSegment.h"
#include "Ray.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Math
{

BaseGeometry::BaseGeometry(BaseGeometry *parent)
{
  Init();

  m_parent = parent;
}

BaseGeometry::~BaseGeometry()
{
  if(m_parent)
    m_parent->RemoveChild(this);

  while( !m_children.empty() )
  {
    m_children.back()->m_parent = m_parent;
    m_children.pop_back();
  }

  if(m_position_owned)
    delete m_position;

  if(m_orientation_owned)
    delete m_orientation;
}

void BaseGeometry::Init()
{
  m_position = new Vector3;
  m_position_owned = true;

  m_orientation = new Quaternion;
  m_orientation_owned = true;
}

void BaseGeometry::Copy(const BaseGeometry& rhs)
{
  SetPosition( rhs.GetWorldPosition() );
  SetOrientation( rhs.GetWorldOrientation() );
}

bool BaseGeometry::ContainedBy(const BoundingVolume& bv) const
{
  return bv.Contains(*this);
}

bool BaseGeometry::Intersection(const BaseGeometry&) const
{
  // TODO: Implement this
  return false;
  //geometry.Intersection(
}

bool BaseGeometry::Intersection(const Ray& ray, float& time) const
{
  if(Intersection(ray.ToLine(), time) && time >= 0)
    return true;
  else
    return false;
}

bool BaseGeometry::Intersection(const LineSegment& segment, float& time) const
{
  if(Intersection(segment.ToLine(), time) && time >= 0 && time <= 1.0f)
    return true;
  else
    return false;
}

void BaseGeometry::AddChild(BaseGeometry *child)
{
  child->m_parent = this;
  m_children.push_back(child);
}

void BaseGeometry::RemoveChild(BaseGeometry *child)
{
  child->m_parent = 0;

  GeometryList::iterator iter = std::find(m_children.begin(), m_children.end(), child);

  if(iter != m_children.end())
  {
    m_children.erase(iter);
  }
}

const Vector3& BaseGeometry::GetPosition() const
{
  return *m_position;
}

const Quaternion& BaseGeometry::GetOrientation() const
{
  return *m_orientation;
}

Vector3 BaseGeometry::GetWorldPosition() const
{
  if(m_parent)
    return *m_position + m_parent->GetWorldPosition();
  else
    return *m_position;
}

Quaternion BaseGeometry::GetWorldOrientation() const
{
  if(m_parent)
    return *m_orientation * m_parent->GetWorldOrientation();
  else
    return *m_orientation;
}

void BaseGeometry::SetPosition(Vector3 *pos)
{
  if(pos)
  {
    if(m_position_owned)
      delete m_position;

    m_position = pos;
    m_position_owned = false;
  }
}

void BaseGeometry::SetPosition(const Vector3& pos)
{
  if(m_position_owned && m_position)
  {
    *m_position = pos;
  }
  else
  {
    m_position = new Math::Vector3(pos);
    m_position_owned = true;
  }
}

void BaseGeometry::SetOrientation(Quaternion *q)
{
  if(q)
  {
    if(m_orientation_owned)
      delete m_orientation;

    m_orientation = q;
    m_orientation_owned = false;
  }

}

void BaseGeometry::SetOrientation(const Quaternion& q)
{
  if(m_orientation_owned && m_orientation)
  {
    *m_orientation = q;
  }
  else
  {
    m_orientation = new Math::Quaternion(q);
    m_orientation_owned = true;
  }
}

}
}
