/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    FrameBuffer2.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:36:26 CTallman
**  \brief   Acts as a render target, drawing to a texture.
*******************************************************************************/

#include "FrameBuffer2.h"
#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"
#include "Utilities/LeakCheck.h"
#include "StateManager.h"
namespace SF
{
  namespace Graphics
  {

    FrameBuffer2::FrameBuffer2()
      : width(0), height(0), type(GL_UNSIGNED_BYTE),
      depth_bits(0), active_texture(0)
    {
      CheckThisForErrors(glGenFramebuffersEXT(1, &main_handle));
      CheckThisForErrors(glGenRenderbuffersEXT( 1, &depth_handle ));
      CheckThisForErrors(glGenTextures(2, texture));
    }

    FrameBuffer2::FrameBuffer2( unsigned int _width, unsigned int _height, unsigned int _type, unsigned int _depth_bits)
      : active_texture(0)
    {
      CheckThisForErrors(glGenFramebuffersEXT(1, &main_handle));
      CheckThisForErrors(glGenRenderbuffersEXT( 1, &depth_handle ));
      CheckThisForErrors(glGenTextures(2, texture));

      Create( _width, _height, _type, _depth_bits );    
    }

    FrameBuffer2::~FrameBuffer2()
    {
      CheckThisForErrors(glDeleteTextures(2, texture));
      CheckThisForErrors(glDeleteRenderbuffersEXT( 1, &depth_handle ));
      CheckThisForErrors(glDeleteFramebuffersEXT(1, &main_handle ));
    }

    void FrameBuffer2::Create( unsigned int _width, unsigned int _height, unsigned int _type, unsigned int _depth_bits)
    {
      width = _width;
      height = _height;
      type = _type;
      depth_bits = _depth_bits;
      active_texture = 0;
    
      CreateTextures();
      if( depth_bits ) 
        CreateDepthTarget();

      Validate();
    }

    void FrameBuffer2::CreateTextures()
    {
      unsigned int format;
      switch( type )
      {
      default:
      case GL_UNSIGNED_BYTE:
        format = GL_RGBA8;
        break;
      case GL_FLOAT:
        format = GL_RGBA32F_ARB;
        break;
      case GL_HALF_FLOAT_ARB:
        format = GL_RGBA16F_ARB;
        break;
      }

      BindTexture();
      CheckThisForErrors(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
      CheckThisForErrors(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
      CheckThisForErrors(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      CheckThisForErrors(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      CheckThisForErrors(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, type, 0));
      SwapTargets();
      BindTexture();
      CheckThisForErrors(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
      CheckThisForErrors(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
      CheckThisForErrors(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      CheckThisForErrors(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      CheckThisForErrors(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, type, 0));
      
      GuardedBind();
      CheckThisForErrors(glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture[0], 0 )); 
      CheckThisForErrors(glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, texture[1], 0 )); 
      UnbindTexture();
      CheckThisForErrors(glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT));
      GuardedUnbind();
    }

    void FrameBuffer2::CreateDepthTarget()
    {
      GuardedBind();
      CheckThisForErrors(glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, depth_handle ));
      
      switch( depth_bits )
      {
      case 16:
        CheckThisForErrors(glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, width, height ));
        break;
      default:
      case 24:
        CheckThisForErrors(glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height ));
        break;
      case 32:
        CheckThisForErrors(glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32, width, height ));
        break;
      }
      
      CheckThisForErrors(glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_handle ));

      //NOT SURE IF THIS IS CORRECT!!!
      //CheckThisForErrors(glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, 0 ));
      GuardedUnbind();
    }

    void FrameBuffer2::BeginCapture()
    {
      CheckThisForErrors(glPushAttrib( GL_VIEWPORT_BIT ));
      CheckThisForErrors(glViewport(0, 0, width, height));
      CheckThisForErrors(GuardedBind());
      CheckThisForErrors(glDrawBuffer( active_texture == 0 ? GL_COLOR_ATTACHMENT1_EXT : GL_COLOR_ATTACHMENT0_EXT ));
    }

    void FrameBuffer2::EndCapture()
    {
      CheckThisForErrors(GuardedUnbind());
      CheckThisForErrors(glPopAttrib());
    }

    void FrameBuffer2::GuardedBind() 
    {
      glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &saved_handle );
      if(main_handle != (GLuint)saved_handle)
      {
        CheckThisForErrors(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, main_handle));
      }
    }

    void FrameBuffer2::GuardedUnbind() 
    {
      if(main_handle != (GLuint)saved_handle)
      {
        CheckThisForErrors(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, (GLuint)saved_handle));
      }
    }

    void FrameBuffer2::BindTexture(unsigned int stage) const
    {
      Performance::StateManager::EnableTexture2D(TextureHandle(), stage);
    }

    void FrameBuffer2::UnbindTexture(unsigned int stage) const
    {
      Performance::StateManager::DisableTexture2D(stage);
    }

    void FrameBuffer2::SwapTargets()
    {
      active_texture ^= 1;
    }

    void FrameBuffer2::ClearTarget()
    {
      CheckThisForErrors(glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ));
    }

    void FrameBuffer2::Validate()
    {
      GuardedBind();
      GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
      switch(status)
      {
      case GL_FRAMEBUFFER_COMPLETE_EXT:
        break;                     
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        Globals::console->Error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        Globals::console->Error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        Globals::console->Error("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        Globals::console->Error("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        Globals::console->Error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        Globals::console->Error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT");
        break;
      default:
      case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        Globals::console->Error("GL_FRAMEBUFFER_UNSUPPORTED_EXT");
        break;
      }
      GuardedUnbind();
    }


  }
}