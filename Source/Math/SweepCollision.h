/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SweepCollision.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 10/06/2008 @ 18:32:37 - pmera
**  \brief   Collision tests for geometry moving with a constant velocity.
*******************************************************************************/

#ifndef SF_SWEEPCOLLISION_H_
#define SF_SWEEPCOLLISION_H_

#include "BaseGeometry.h"
#include "BoundingVolume.h"

namespace SF {
namespace Math
{

  // Sphere vs. Sphere
bool Collision(const Sphere& sphereA, const Vector3& velocityA,
               const Sphere& sphereB, const Vector3& velocityB,
               float maxTime, float *time);

  // Sphere vs. Point
bool Collision(const Sphere& sphere, const Vector3& sphereVelocity,
               const Vector3& point, const Vector3& pointVelocity,
               float *time);

}

}

#endif
