/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Simplex.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:46:41 - nwilliam
**  \brief   Implementation of Perlin's lesser known simplex noise algorithm.
*******************************************************************************/

#ifndef SF_SIMPLEX_H
#define SF_SIMPLEX_H

#include "Noise.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  NoiseVal SimplexValue1D( const NoiseData& data, NoiseVal x );
  NoiseVal SimplexValue2D( const NoiseData& data, NoiseVal x, NoiseVal y );
  NoiseVal SimplexValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z );
  NoiseVal SimplexValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );

  NoiseVal SimplexNoise1D( NoiseVal x );
  NoiseVal SimplexNoise2D( NoiseVal x, NoiseVal y );
  NoiseVal SimplexNoise3D( NoiseVal x, NoiseVal y, NoiseVal z );
  NoiseVal SimplexNoise4D( NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );

} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_SIMPLEX_H
