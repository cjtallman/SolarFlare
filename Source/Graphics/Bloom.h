/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Bloom.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:36:04 CTallman
**  \brief
*******************************************************************************/

#ifndef _SF_BLOOM_H_
#define _SF_BLOOM_H_

#include "FrameBuffer2.h"
#include "ShaderLoader.h"

namespace SF
{
  namespace Graphics
  {
    class Bloom
    {
    public:
      Bloom( int width = 1024, int height = 1024 );
      ~Bloom(void);

      void Process( const FrameBuffer2 * main_scene );
      void Resize( int width, int height );

    private:

      void Render( const FrameBuffer2 * main_scene, int index );
      void Resample(const FrameBuffer2 * input, FrameBuffer2 * output );
      void BloomCurve(const FrameBuffer2 * input, FrameBuffer2 * output);

      void Blur( FrameBuffer2 * input, int index );

      void Combine( const FrameBuffer2 * main, FrameBuffer2 * output );

      static const int fbo_count = 5;
      FrameBuffer2 fbo[fbo_count];

      void SetFragmentShader( SharedShader2 shader, int index );
      void SetFragmentShader( const std::string & shader, int index );
      
      SharedShader2 frag[5];

      enum{
        BLUR3X3,
        BLUR5X5,
        BLUR11X11,
        BLOOMCURVE,
        BLOOMCOMBINE
      };

      SharedShaderParameter params[3];
    };

  }
}


#endif

