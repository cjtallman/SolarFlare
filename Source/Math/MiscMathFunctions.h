/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    MiscMathFunctions.h
**  \author  Chris Tallman, Nathan Williams
**  \par     email: ctallman\@digipen.edu, nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 01/05/2009 @ 17:37:29 - nwilliam
**  \brief   Various handy math related functions.
*******************************************************************************/

#ifndef SF_MISC_MATH_FUNCTIONS_H
#define SF_MISC_MATH_FUNCTIONS_H

namespace SF {
namespace Math
{
  ////////////////////////////////////////////////////////////////////////////////
  // For Magic Derivation see: 
  // Chris Lomont
  // http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
  // Credited to Greg Walsh.
  ////////////////////////////////////////////////////////////////////////////////
  
  
  ////////////////////////////////////////////////////////////////////////////////
  // 32  Bit float magic number 
  const int SQRT_MAGIC_F = 0x5f3759df;
  ////////////////////////////////////////////////////////////////////////////////
  
  ////////////////////////////////////////////////////////////////////////////////
  // Inverse Square Root ( Float )
  ////////////////////////////////////////////////////////////////////////////////
  inline float invSqrt(const float x)
  {
    const float xhalf = 0.5f * x;
   
    union // get bits for floating value
    {
      float x;
      int i;
    } u;

    u.x = x;
    u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0

    float temp = u.x * (1.5f - xhalf * u.x * u.x);
    return temp * (1.5f - xhalf * temp * temp);// Newton step, repeating increases accuracy
  }
  
  ////////////////////////////////////////////////////////////////////////////////
  // Square Root ( Float )
  ////////////////////////////////////////////////////////////////////////////////
  inline float fastSqrt_Q3(const float x)
  {
    return x * invSqrt(x);
  }
  
  
  ////////////////////////////////////////////////////////////////////////////////
  // 64  Bit float magic number
  const long long SQRT_MAGIC_D = 0x5fe6ec85e7de30daLL;
  ////////////////////////////////////////////////////////////////////////////////
  
  inline double invSqrt(const double x)
  {
    const double xhalf = 0.5*x;
   
    union // get bits for floating value
    {
      double x;
      long long i;
    } u;
    u.x = x;
    u.i = SQRT_MAGIC_D - (u.i >> 1);  // gives initial guess y0
    
    double temp = u.x * (1.5 - xhalf * u.x * u.x);
    return temp * (1.5 - xhalf * temp * temp);// Newton step, repeating increases accuracy
  }
  
  inline double fastSqrt_Q3(const double x)
  {
    return x * invSqrt(x);
  }
  
} // Math namespace
} // SF namespace

#endif // SF_MISC_MATH_FUNCTIONS_H
