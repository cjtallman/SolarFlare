/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    HybridMultiFractal.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/20/2008 @ 21:37:00 - nwilliam
**  \brief   Hybrid multi-fractal based noise generator.
*******************************************************************************/

#ifndef SF_HYBRID_MULTI_FRACTAL_H
#define SF_HYBRID_MULTI_FRACTAL_H

#include "Noise.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  NoiseVal HMFValue1D( const NoiseData& data, NoiseVal x );
  NoiseVal HMFValue2D( const NoiseData& data, NoiseVal x, NoiseVal y );
  NoiseVal HMFValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z );
  NoiseVal HMFValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );
  
} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_HYBRID_MULTI_FRACTAL_H
