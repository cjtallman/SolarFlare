/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Line3.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 08/27/2008 @ 18:13:50 - nwilliam
**  \brief   Implementation of 3D line operations.
*******************************************************************************/

#include "Line3.h"

#include "Plane.h"

#include <queue>

namespace SF {
namespace Math
{

Line3::Line3() {}

Line3::Line3(const Math::Vector3& start, const Math::Vector3& vector)
: m_start(start), m_vector(vector) {}

Vector3 Line3::operator()(float t) const
{
  return m_start + m_vector * t;
}

} // Math namespace
} // SF namespace
