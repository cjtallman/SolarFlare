/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    FractionalBrownianMotion.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/20/2008 @ 21:38:33 - nwilliam
**  \brief   FractionalBrownianMotion (fBM) noise generator.
*******************************************************************************/

#ifndef SF_FRACTIONAL_BROWNIAN_MOTION_H
#define SF_FRACTIONAL_BROWNIAN_MOTION_H

#include "Noise.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  NoiseVal FBMValue1D( const NoiseData& data, NoiseVal x );
  NoiseVal FBMValue2D( const NoiseData& data, NoiseVal x, NoiseVal y );
  NoiseVal FBMValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z );
  NoiseVal FBMValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );
  
} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_FRACTIONAL_BROWNIAN_MOTION_H
