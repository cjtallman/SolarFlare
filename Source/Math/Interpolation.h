/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Interpolation.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 03/31/2009 @ 18:47:38 - pmera
**  \brief   
*******************************************************************************/

namespace SF {
namespace Math
{

  // Linear Interpolation
template <typename T>
T Lerp(const T& v0, const T& v1, float t)
{
  return static_cast<T>( v0 + (v1 - v0) * t );
}

  // Interpolation by t^n
template <typename T>
T Lerp(const T& v0, const T& v1, float t, unsigned int power)
{
  float u = 1.0f;

  for(unsigned int i = 0; i < power; ++i)
    u *= t;

  return static_cast<T>( v0 + (v1 - v0) * u );
}

  // t(0) = 1, t(0.5) = 0, t(1) = 1
template <typename T>
T ParabolicInterp(const T& v0, const T& v1, float t)
{
  return static_cast<T>( v0 + (v1 - v0) * (t-0.5)*(t-0.5)*4 );
}

  // t(0) = 0, t(0.5) = 1, t(1) = 0
template <typename T>
T ParabolicInterpNegative(const T& v0, const T& v1, float t)
{
  return static_cast<T>( v0 + (v1 - v0) * (1.0f - (t-0.5)*(t-0.5)*4) );
}

}
}
