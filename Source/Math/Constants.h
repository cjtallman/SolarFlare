/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Constants.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/02/2008 @ 23:15:04 - nwilliam
**  \brief   Many handy math related constants.
*******************************************************************************/

#ifndef SF_CONSTANTS_H
#define SF_CONSTANTS_H

namespace SF {
namespace Math {
namespace Constants
{
  typedef float Real;
  
  // Basic constansts
  const Real e         = 2.7182818284590451f;   // e
  const Real pi        = 3.1415926535897931f;   // pi
  const Real eulers    = 0.57721566490153286f;  // Euler-Mascheroni Constant
  const Real catalans  = 0.91596559417721902f;  // Catalan's constant
  const Real nwilliams = 25.0000000000000000f;  // Nathan William's constant
  
  // "Conversion" constants
  const Real DegToRad  = pi / 180.0f;
  const Real RadToDeg  = 180.0f / pi;
  
  // Products
  const Real p_4_pi      = 12.566370614359172f;   // 4*pi
  const Real p_2_pi      = 6.2831853071795862f;   // 2*pi
  
  // Fractions
  const Real f_1_sqrt_2  = 0.70710678118654746f;  // 1/sqrt(2)
  const Real f_pi_2      = 1.5707963267948966f;   // pi/2
  const Real f_pi_4      = 0.78539816339744828f;  // pi/4
  const Real f_1_pi      = 0.31830988618379069f;  // 1/pi
  const Real f_2_pi      = 0.63661977236758138f;  // 2/pi
  const Real f_2_sqrt_pi = 1.1283791670955126f;   // 2/sqrt(pi)
  
  // sqrts
  const Real sqrt_pi     = 1.7724538509055159f;   // sqrt(pi)
  const Real sqrt_2      = 1.4142135623730951f;   // sqrt(2)
  const Real sqrt_3      = 1.7320508075688772f;   // sqrt(3)
  
  // Logarithms
  const Real ln_2        = 0.69314718055994529f;  // ln(2)
  const Real ln_10       = 2.3025850929940459f;   // ln(10)
  const Real ln_2_pi     = 1.8378770664093453f;   // ln(2*pi)
  const Real log2_e      = 1.4426950408889634f;   // log2(e)
  const Real log2_10     = 3.3219280948873626f;   // log2(10)
  const Real log10_2     = 0.30102999566398114f;  // log10(2)
  const Real log10_e     = 0.43429448190325176f;  // log10(e)
  
} // Constants namespace
} // Math namespace
} // SF namespace

#endif //SF_CONSTANTS_H
