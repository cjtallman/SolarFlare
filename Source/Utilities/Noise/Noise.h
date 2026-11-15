/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Noise.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 20:05:37 - nwilliam
**  \brief   Helper functions, typedefs, structs that apply to all noise.
*******************************************************************************/

#ifndef SF_NOISE_H
#define SF_NOISE_H

#include "Utilities/SharedArray.h"

// Forward declaration
namespace SF { namespace Networking { class ByteStream; } }

namespace SF {
namespace Utilities {
namespace Noise
{
  typedef float NoiseVal;


  typedef NoiseVal (*NoiseFunc1D)( NoiseVal x );
  typedef NoiseVal (*NoiseFunc2D)( NoiseVal x, NoiseVal y );
  typedef NoiseVal (*NoiseFunc3D)( NoiseVal x, NoiseVal y, NoiseVal z );
  typedef NoiseVal (*NoiseFunc4D)( NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );


  enum GeneratorType { GEN_FBM,
                       GEN_HMF,
                       GEN_PERLIN,
                       GEN_RMF,
                       GEN_SIMPLEX
                      };

  struct NoiseData
  {
    NoiseData();
    void SetDefaults( GeneratorType type );

    int seed;
    int width;
    int height;
    int octaves;
    NoiseVal dt;
    bool animate;
    NoiseVal time;
    NoiseVal gain;
    float scaleMin;
    float scaleMax;
    NoiseVal offset;
    NoiseVal frequency;
    NoiseVal lacunarity;
    NoiseVal hurstIndex;
    NoiseVal persistence;
    GeneratorType noiseType;
    
    void ComputeWeights();
    NoiseFunc1D Noise1;
    NoiseFunc2D Noise2;
    NoiseFunc3D Noise3;
    NoiseFunc4D Noise4;
    Utilities::SharedArray<float> weights;
  };

  Networking::ByteStream& operator>>( Networking::ByteStream& bs, NoiseData& nd );
  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const NoiseData& nd );



  typedef NoiseVal (*NoiseValue1D)( const NoiseData& data, NoiseVal x );
  typedef NoiseVal (*NoiseValue2D)( const NoiseData& data, NoiseVal x, NoiseVal y );
  typedef NoiseVal (*NoiseValue3D)( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z );
  typedef NoiseVal (*NoiseValue4D)( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );
  
  
  enum ParameterFlag { PF_SEED         = 1,
                       PF_OCTAVES      = 2,
                       PF_GAIN         = 4,
                       PF_OFFSET       = 8,
                       PF_FREQUENCY    = 16,
                       PF_LACUNARITY   = 32,
                       PF_HURST_INDEX  = 64,
                       PF_PERSISTENCE  = 128
                      };


  NoiseValue1D GetNoise1D( GeneratorType type );
  NoiseValue2D GetNoise2D( GeneratorType type );
  NoiseValue3D GetNoise3D( GeneratorType type );
  NoiseValue4D GetNoise4D( GeneratorType type );

} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_BASE_NOISE_H
