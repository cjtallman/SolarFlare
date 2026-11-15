/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Ray.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 14:50:09 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_RAY_H_
#define SF_RAY_H_

#include "TVector3.h"

#include "Line3.h"

namespace SF {
namespace Math
{

struct Ray
{
  Vector3  start;
  Vector3 direction;

  Ray();
  Ray(const Vector3& _start, const Vector3& _direction);
  
  Vector3 operator()(float t) const;

  Line3 ToLine() const;
};

}
}

#endif
