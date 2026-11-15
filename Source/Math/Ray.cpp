/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Ray.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 14:53:47 - pmera
**  \brief   
*******************************************************************************/

#include "Ray.h"

namespace SF {
namespace Math
{

Ray::Ray()
{
}

Ray::Ray( const Vector3& _start, const Vector3& _direction )
: start(_start), direction(_direction)
{
}

Vector3 Ray::operator()(float t) const
{
  if(t < 0)
    return start;

  return start + direction * t;
}

Line3 Ray::ToLine() const
{
  return Line3(start, direction);
}

}
}
