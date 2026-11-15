/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SweepCollision.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 10/06/2008 @ 18:32:44 - pmera
**  \brief   
*******************************************************************************/

#include "SweepCollision.h"

#include "Sphere.h"

namespace SF {
namespace Math
{

bool Collision(const Sphere& sphereA, const Vector3& velocityA,
               const Sphere& sphereB, const Vector3& velocityB,
               float maxTime, float *time)
{
  const Math::Vector3& v1 = velocityA;
  const Math::Vector3& v2 = velocityB;

  const Math::Vector3& p1 = sphereA.GetWorldPosition();
  const Math::Vector3& p2 = sphereB.GetWorldPosition();

  float r1 = sphereA.GetRadius();
  float r2 = sphereB.GetRadius();

  Vector3 velocityDiff = v1 - v2;
  Vector3 AtoB = p2 - p1;
  Vector3 BtoA = p1 - p2;
  float totalRadius = r1 + r2;

  /*if( AtoB * AtoB <= totalRadius * totalRadius )
  {
    if(time)
      *time = 0.0f;
    return true;
  }*/

  //float f1 = v1 * AtoB;
  //float f2 = v2 * BtoA;

  if( v1 * AtoB < -1e-5f && v2 * BtoA < -1e-5f )
    return false;

  if( velocityDiff * velocityDiff <= 1e-5f )
    return false;

  float a = velocityDiff * velocityDiff;
  float b = 2 * (BtoA * velocityDiff);
  float c = BtoA * BtoA - totalRadius * totalRadius;

  float discriminant = b*b - 4*a*c;

  if(discriminant < 0 || std::fabs(a) < 1e-5f )
    return false;

  float sqrt_discriminant = std::sqrt(discriminant);

  float t1 = (-b + sqrt_discriminant) / (2*a);
  float t2 = (-b - sqrt_discriminant) / (2*a);

  float t = std::min(t1, t2);

  if(t < 0.0f)
    return false;
  if(t > maxTime)
    return false;
  else
  {
    if(time)
      *time = t;
    return true;
  }
}

bool Collision(const Sphere& sphere, const Vector3& sphereVelocity,
               const Vector3& point, const Vector3& pointVelocity,
               float *time)
{
  const Math::Vector3& v1 = sphereVelocity;
  const Math::Vector3& v2 = pointVelocity;

  const Math::Vector3& p1 = sphere.GetWorldPosition();
  const Math::Vector3& p2 = point;

  float r = sphere.GetRadius();

  Vector3 velocityDiff = v1 - v2;
  Vector3 AtoB = p1 - p2;

  if( AtoB * AtoB <= r * r )
  {
    if(time)
      *time = 0.0f;
    return true;
  }

  if(v1 * AtoB <= 0.0f && v2 * AtoB <= 0.0f)
    return false;

  if( velocityDiff * velocityDiff <= 0.001f )
    return false;

  float a = velocityDiff * velocityDiff;
  float b = 2 * (AtoB * velocityDiff);
  float c = AtoB * AtoB - r * r;

  float discriminant = b*b - 4*a*c;

  if(discriminant < 0 || a == 0)
    return false;

  float sqrt_discriminant = std::sqrt(discriminant);

  float t1 = (-b + sqrt_discriminant) / (2*a);
  float t2 = (-b - sqrt_discriminant) / (2*a);

  float t = std::min(t1, t2);

  if(t < 0.0f || t > 1.0f)
    return false;
  else
  {
    if(time)
      *time = t;
    return true;
  }
}

}

}
