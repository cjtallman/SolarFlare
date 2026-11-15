/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CubemapTexture.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#include <cmath>    // abs
#include <cstring>  //memset
#include "CubemapTexture.h"
#include "StateManager.h"
#include "External/SOIL/include/SOIL.h"

namespace SF
{
  namespace Graphics
  {
    namespace TextureType
    {


      CubemapTexture::CubemapTexture( int _width )
      {
        CheckThisForErrors(glGenTextures(1, &cubemap));
        CheckThisForErrors(glGenFramebuffersEXT(1, &fbo));

        Bind();
        CheckThisForErrors(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        CheckThisForErrors(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        CheckThisForErrors(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        CheckThisForErrors(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        CheckThisForErrors(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        Unbind();

        width = _width;
        needs_recreated = true;
        bound = false;
        stage = -1;

        pixel_data_size = width*width*4;

        for( int i = 0; i < 6; ++i )
        {
          pixel_data[i] = new unsigned char[pixel_data_size];
          std::memset( pixel_data[i].Get(), 0xFF, pixel_data_size);
        }

        Recreate();

        ValidateFBO();
      }

      CubemapTexture::~CubemapTexture()
      {
        CheckThisForErrors(glDeleteTextures(1, &cubemap));
        CheckThisForErrors(glDeleteFramebuffersEXT(1, &fbo));
      }

      CubemapTexture & CubemapTexture::DeepCopy( const CubemapTexture & A )
      {
        width = A.width;
        needs_recreated = true;
        bound = false;
        stage = -1;

        pixel_data_size = width*width*4;

        for( int i = 0; i < 6; ++i )
        {
          pixel_data[i] = new unsigned char[pixel_data_size];
          std::memcpy(pixel_data[i].Get(), A.pixel_data[i].Get(), pixel_data_size);
        }

        Recreate();
        return *this;
      }

      CubemapTexture & CubemapTexture::ShallowCopy( const CubemapTexture & A )
      {
        width = A.width;
        needs_recreated = true;
        bound = false;
        stage = -1;

        pixel_data_size = width*width*4;

        for( int i = 0; i < 6; ++i )
        {
          pixel_data[i] = A.pixel_data[i];
        }

        Recreate();
        return *this;
      }

      CubemapTexture & CubemapTexture::operator =(const CubemapTexture & A)
      {
        return DeepCopy(A);
      }

      void CubemapTexture::ValidateFBO()
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

      void CubemapTexture::GuardedBind() 
      {
        if( !bound )
        {
          bound = true;
          glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &saved_handle );
          CheckThisForErrors(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo));
        }
      }

      void CubemapTexture::GuardedUnbind() 
      {
        if( bound )
        {
          bound = false;
          CheckThisForErrors(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, (GLuint)saved_handle));
        }
      }

      void CubemapTexture::BeginCapture(int face)
      {
        GuardedBind();
        CheckThisForErrors(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, cubemap, 0));
        //ValidateFBO();

      }

      void CubemapTexture::EndCapture()
      {
        GuardedUnbind();
      }

      void CubemapTexture::UpdateLocalCopy(int face)
      {
        Bind();
        if( face == -1 )
        {
          for( int i = 0; i < 6; ++i )
          {
            CheckThisForErrors(glReadPixels(0,0,width,width,GL_RGBA,GL_UNSIGNED_BYTE, pixel_data[i].Get()));
          }
        }
        else 
        {
          CheckThisForErrors(glReadPixels(0,0,width,width,GL_RGBA,GL_UNSIGNED_BYTE, pixel_data[face].Get()));
        }
        Unbind();
      }

      void CubemapTexture::Bind( int _stage )
      {
        Performance::StateManager::EnableTextureCube(cubemap,_stage);
        stage = _stage;
      }

      void CubemapTexture::Unbind()
      {
        Performance::StateManager::DisableTextureCube(stage);
      }

      int CubemapTexture::PixelDataSize() const
      {
        return pixel_data_size;
      }

      const unsigned char * CubemapTexture::PixelData( int face ) const
      {
        return pixel_data[face].Get();
      }

      void CubemapTexture::Recreate( int _stage )
      {
        needs_recreated = false;

        Bind(_stage);
        for(int i = 0; i < 6; ++i)
        {
          CheckThisForErrors(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA8, width, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data[i].Get()));
        }
        Unbind();

        GuardedBind();
        CheckThisForErrors(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubemap, 0));
        GuardedUnbind();
      }

      SF::Color CubemapTexture::GetTexel( const Math::Vector3 & dir ) const
      {
        //return Color4f(1,1,1,1);
        Math::Vector3 r = dir.Normalized();

        float AX = std::abs(r.x);
        float AY = std::abs(r.y);
        float AZ = std::abs(r.z);
        float sc, tc, ma;
        char face = 0;

        //X major axis
        if( AX > std::max(AY, AZ) )
        {
          if( r.x > 0 )
          {
            face = 0;
            sc = -r.z;
            tc = -r.y;
            ma = AX;
          }
          else
          {
            face = 1;
            sc = r.z;
            tc = -r.y;
            ma = AX;
          }
        }

        //Y major axis
        else if( AY > AZ )
        {
          if( r.y > 0 )
          {
            face = 2;
            sc = r.x;
            tc = r.z;
            ma = AY;
          }
          else
          {
            face = 3;
            sc = r.x;
            tc = -r.z;
            ma = AY;
          }
        }
        //Z major axis
        else
        {
          if( r.z > 0 )
          {
            face = 4;
            sc = r.x;
            tc = -r.y;
            ma = AZ;
          }
          else
          {
            face = 5;
            sc = -r.x;
            tc = -r.y;
            ma = AZ;
          }
        }

        if( ma == 0.0 )
          ma = 1.0f;

        float s = ( sc/ma + 1.0f ) / 2.0f;
        float t = ( tc/ma + 1.0f ) / 2.0f;

        int col = int((width-1) * s);
        int row = int((width-1) * t);

        const unsigned char * texel = pixel_data[face].Get()+4*(row*width + col);
        return Color(texel[0], texel[1], texel[2], texel[3]);
      }

    }//namespace TextureType
  }//namespace Graphics
}//namespace SF
