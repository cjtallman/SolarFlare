/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Line3.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 08/27/2008 @ 18:12:56 - nwilliam
**  \brief   A 3D line with a start point and direction. Pretty much the same
             thing as a ray.
*******************************************************************************/

#ifndef SF_LINE3_H
#define SF_LINE3_H

#include "TVector3.h"

namespace SF {
namespace Math
{

struct Plane;

struct Line3
{
  Vector3 m_start;  // Start point
  Vector3 m_vector; // Direction vector
  
  Line3();
  Line3( const Math::Vector3& start, const Math::Vector3& vector );
  
  Vector3 operator()(float t) const;
};

} // Math namespace
} // SF namespace

#endif
