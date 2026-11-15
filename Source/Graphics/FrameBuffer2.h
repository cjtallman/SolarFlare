/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    FrameBuffer2.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:36:51 CTallman
**  \brief   Acts as a render target, drawing to a texture.
*******************************************************************************/

#ifndef _SF_FRAME_BUFFER_2_H_
#define _SF_FRAME_BUFFER_2_H_

#include "OpenGLInclude.h"

namespace SF
{
  namespace Graphics
  {


    class FrameBuffer2
    {
    public:

      FrameBuffer2();
      FrameBuffer2(unsigned int _width, unsigned int _height, unsigned int _type = GL_UNSIGNED_BYTE,
        unsigned int _depth_bits = 0);

      ~FrameBuffer2();

      void Create(unsigned int _width, unsigned int _height, unsigned int _type = GL_UNSIGNED_BYTE,
        unsigned int _depth_bits = 0);

      void BindTexture(unsigned int stage = 0) const;
      void UnbindTexture(unsigned int stage = 0) const;

      void SwapTargets();
      void ClearTarget();

      void BeginCapture();
      void EndCapture();

      void Validate();

      unsigned int TextureHandle() const { return texture[active_texture]; }
      unsigned int Width() const { return width; }
      unsigned int Height() const { return height; }

    private:

      void GuardedBind();
      void GuardedUnbind();

      void CreateTextures();
      void CreateDepthTarget();


      GLint active_texture;
      GLuint main_handle;
      GLint saved_handle;
      GLuint depth_handle;
      GLuint texture[2];

      GLuint width;
      GLuint height;
      GLuint type;
      GLuint depth_bits;

    };





  }
}

#endif

