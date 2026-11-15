/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseNoise.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/09/2009 @ 18:35:07 - nwilliam
**  \brief   Common noise function implementations.
*******************************************************************************/

#include <cmath>        // pow
#include "Noise.h"
#include "AllNoise.h"
#include "Networking/ByteStream.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  NoiseData::NoiseData() : seed( 0 ),
                           width( 512 ),
                           height( 256 ),
                           time( 0.0f ),
                           octaves( 5 ),
                           dt( 0.0f ),
                           animate( false ),
                           scaleMin( -1.0f ),
                           scaleMax( 1.0f ),
                           gain( 2.0f ),
                           offset( 1.0f ),
                           frequency( 1.0f ),
                           lacunarity( 2.0f ),
                           hurstIndex( 1.0f ),
                           persistence( 0.5f ),
                           noiseType( GEN_SIMPLEX ),
                           Noise1( SimplexNoise1D ),
                           Noise2( SimplexNoise2D ),
                           Noise3( SimplexNoise3D ),
                           Noise4( SimplexNoise4D )
  {
  }

  void NoiseData::SetDefaults( GeneratorType type )
  {
    // Set defaults, as above
    *this = NoiseData();
    noiseType = type;

    // For functions that have differe 'default' values
    switch( type )
    {
      case GEN_HMF:
        offset = 0.7f;
        hurstIndex = 0.25f;
      case GEN_RMF:
      case GEN_FBM:
        ComputeWeights();
      break;
    }
  }

  void NoiseData::ComputeWeights()
  {
    if( noiseType == GEN_FBM || noiseType == GEN_HMF || noiseType == GEN_RMF )
    {
      weights = new float[ octaves ];
      
      float freq = 1.0f;
      
      for( int i = 0; i < octaves; ++i )
      {
        weights[i] = std::pow( freq, -hurstIndex );
        freq *= lacunarity;
      }
    }
  }

  Networking::ByteStream& operator>>( Networking::ByteStream& bs, NoiseData& nd )
  {
    bs >> nd.seed >> nd.width >> nd.height >> nd.time >> nd.octaves >> nd.dt 
       >> nd.animate >> nd.gain >> nd.scaleMin >> nd.scaleMax >> nd.offset 
       >> nd.frequency >> nd.lacunarity >> nd.hurstIndex >> nd.persistence;

    int8 type = 0;
    bs >> type;
    nd.noiseType = GeneratorType( type );

    nd.ComputeWeights();

    return bs;
  }

  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const NoiseData& nd )
  {
    bs << nd.seed << nd.width << nd.height << nd.time << nd.octaves << nd.dt 
       << nd.animate << nd.gain << nd.scaleMin << nd.scaleMax << nd.offset 
       << nd.frequency << nd.lacunarity << nd.hurstIndex << nd.persistence;

    int8 type = static_cast<int8>( nd.noiseType );
    bs << type;

    return bs;
  }




  NoiseValue1D GetNoise1D( GeneratorType type )
  {
    switch( type )
    {
      default:
      case GEN_FBM:     return FBMValue1D;      break;
      case GEN_HMF:     return HMFValue1D;      break;
      case GEN_PERLIN:  return PerlinValue1D;   break;
      case GEN_RMF:     return RMFValue1D;      break;
      case GEN_SIMPLEX: return SimplexValue1D;  break;
    }
  }

  NoiseValue2D GetNoise2D( GeneratorType type )
  {
    switch( type )
    {
      default:
      case GEN_FBM:     return FBMValue2D;      break;
      case GEN_HMF:     return HMFValue2D;      break;
      case GEN_PERLIN:  return PerlinValue2D;   break;
      case GEN_RMF:     return RMFValue2D;      break;
      case GEN_SIMPLEX: return SimplexValue2D;  break;
    }
  }

  NoiseValue3D GetNoise3D( GeneratorType type )
  {
    switch( type )
    {
      default:
      case GEN_FBM:     return FBMValue3D;      break;
      case GEN_HMF:     return HMFValue3D;      break;
      case GEN_PERLIN:  return PerlinValue3D;   break;
      case GEN_RMF:     return RMFValue3D;      break;
      case GEN_SIMPLEX: return SimplexValue3D;  break;
    }
  }

  NoiseValue4D GetNoise4D( GeneratorType type )
  {
    switch( type )
    {
      default:
      case GEN_HMF:     return HMFValue4D;      break;
      case GEN_FBM:     return FBMValue4D;      break;
      case GEN_PERLIN:  return PerlinValue4D;   break;
      case GEN_RMF:     return RMFValue4D;      break;
      case GEN_SIMPLEX: return SimplexValue4D;  break;
    }
  }

} // Noise namespace
} // Utilities namespace
} // SF namespace
