/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    NoiseArray.cs
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/08/2009 @ 14:54:52 - nwilliam
**  \brief   Simple NoiseVal array wrapper to keep track of width and height.
*******************************************************************************/

#ifndef SF_NOISE_ARRAY_H
#define SF_NOISE_ARRAY_H

#include "Noise.h"
#include "Utilities/SharedArray.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  class NoiseArray
  {
    public:
      NoiseArray();
      NoiseArray( int width, int height );
      ~NoiseArray();
      
      NoiseVal* Data();
      const NoiseVal* Data() const;
      
      int Width() const;
      int Height() const;
      
      void Width( int width );
      void Height( int height );
      
      NoiseVal* operator[]( int index );
      NoiseVal operator[]( int index ) const;
      
      NoiseVal GetValue( int x, int y );
      void SetValue( int x, int y, NoiseVal val );
      
      void Clear();
      void Resize( int width, int height );
      void Scale( NoiseVal min, NoiseVal max );
      
    private:
      int m_width;
      int m_height;
      SharedArray<NoiseVal> m_data;
  };
  
} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_NOISE_ARRAY_H
