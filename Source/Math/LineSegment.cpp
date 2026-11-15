/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    LineSegment.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 15:04:20 - pmera
**  \brief   
*******************************************************************************/

#include "LineSegment.h"

namespace SF {
namespace Math
{

LineSegment::LineSegment()
{
}

LineSegment::LineSegment(const Vector3& _start, const Vector3& _end)
: start(_start), end(_end)
{
}

Vector3 LineSegment::operator()(float t) const
{
  if(t <= 0)
    return start;
  if(t >= 1.0f)
    return end;

  return start + t * (end - start);
}

Line3 LineSegment::ToLine() const
{
  return Line3(start, end - start);
}

}
}
