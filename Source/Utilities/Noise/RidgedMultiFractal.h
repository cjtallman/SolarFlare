/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RidgedMultiFractal.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/20/2008 @ 21:26:43 - nwilliam
**  \brief   Ridged mutli-fractal based noise generator.
*******************************************************************************/

#ifndef SF_RIDGED_MULTI_FRACTAL_H
#define SF_RIDGED_MULTI_FRACTAL_H

#include "Noise.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  NoiseVal RMFValue1D( const NoiseData& data, NoiseVal x );
  NoiseVal RMFValue2D( const NoiseData& data, NoiseVal x, NoiseVal y );
  NoiseVal RMFValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z );
  NoiseVal RMFValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );
  
} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_RIDGED_MULTI_FRACTAL_H
