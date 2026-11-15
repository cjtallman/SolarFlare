/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseGenerator.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/30/2009 @ 19:04:22 - nwilliam
**  \brief   Base class for all NoiseArray generators to inherit from.
*******************************************************************************/

#ifndef SF_BASE_GENERATOR_H
#define SF_BASE_GENERATOR_H

#include "Noise.h"
#include "NoiseArray.h"

namespace SF { 
namespace Utilities {
namespace Noise
{
  class BaseGenerator
  {
    public:
      BaseGenerator();
      virtual ~BaseGenerator();
      
      virtual void Build() = 0;
      unsigned char* BuildTexture( bool buildNoise );

      NoiseData& GetNoiseData();
      void SetNoiseData( const NoiseData& data );

      int Width() const;
      int Height() const;
      NoiseArray& Array();

      void Width( int width );
      void Height( int height );
      void Array( NoiseArray& noiseArray );
      void SetSize( int width, int height );
      void SetNoise( GeneratorType noiseType );

    protected:
      void SetColor( NoiseVal val, unsigned char* pixel ) const;

      NoiseData m_data;
      NoiseArray m_array;
  };

} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_BASE_NOISE_ARRAY_GENERATOR_H
