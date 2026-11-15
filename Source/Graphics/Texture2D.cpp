/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Texture2D.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/30/2009 @ 18:29:17 - nwilliam
**  \brief   Represents a 2D texture
*******************************************************************************/

#include "OpenGLInclude.h"
#include "Utilities/Misc.h"
#include "Utilities/Image.h"
#include "Texture2D.h"
#include "StateManager.h"

#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    namespace TextureType
    {
      Texture2D::Texture2D()
      {
        image = new Utilities::Image;
        stage = -1;

        CheckThisForErrors(glGenTextures(1, &gl_handle));
        Bind();
        Unbind();

        wrapModeU = REPEAT;
        wrapModeV = CLAMP;
        mips = true;
      }

      Texture2D::~Texture2D(void)
      {
        (glDeleteTextures(1, &gl_handle ));
      }

      Texture2D & Texture2D::ShallowCopy( const Texture2D & A )
      {
        mips = A.mips;
        wrapModeU = A.wrapModeU;
        wrapModeV = A.wrapModeV;
        stage = A.stage;
        image = A.image;
        Recreate();
        return *this;
      }

      Texture2D & Texture2D::DeepCopy( const Texture2D & A )
      {
        mips = A.mips;
        wrapModeU = A.wrapModeU;
        wrapModeV = A.wrapModeV;
        stage = A.stage;
        unsigned char * new_data = new unsigned char[A.image->PixelDataSize()];
        std::memcpy(new_data, A.image->PixelData(), A.image->PixelDataSize());
        image->SetData(new_data, A.image->Width(), A.image->Height(), A.image->Format());
        Recreate();
        return *this;
      }

      Texture2D & Texture2D::operator =(const Texture2D & A)
      {
        return ShallowCopy(A);
      }

      int Texture2D::Width() const
      {
        return image->Width();
      }

      int Texture2D::Height() const
      {
        return image->Height();
      }

      const unsigned char * Texture2D::PixelData() const
      {
        return image->PixelData();
      }

      void Texture2D::SetTexel( float u, float v, SF::Color color )
      {
        image->SetTexel( u, v, color );
      }

      void Texture2D::SetTexel( int r, int c, SF::Color color )
      {
        image->SetTexel(r,c,color);
      }

      SF::Color Texture2D::GetTexel( float u, float v ) const
      {
        int r = (int)(v * (image->Height()));
        int c = (int)(u * (image->Width()));

        switch( wrapModeV )
        {
        case REPEAT:
          r = r % image->Height();
          break;

        case CLAMP:
          if( r < 0 ) r = 0;
          else if( r > image->Height()-1 ) r = image->Height()-1;

          break;
        }
        
        switch( wrapModeU )
        {
        case REPEAT:
          c = c % image->Width();
          break;

        case CLAMP:
          if( c < 0 ) c = 0;
          else if( c > image->Width()-1 ) c = image->Width()-1;

          break;
        }
        
        return image->GetTexel( r, c );
      }

      SF::Color Texture2D::GetTexel( int r, int c ) const
      {
        return image->GetTexel(r,c);
      }

      void Texture2D::LoadFromFile( const std::string & fullpath )
      {
        image = new Utilities::Image( fullpath );
      }

      void Texture2D::MakeHeightMap( Utilities::Noise::BaseGenerator& generator )
      {
        image->SetData( generator.BuildTexture( false ), 
                        generator.Width(), 
                        generator.Height(), 
                        Utilities::Image::RGB );
      }

      void Texture2D::Bind(int _stage)
      {
        Performance::StateManager::EnableTexture2D(gl_handle,_stage);
        stage = _stage;
      }

      void Texture2D::Unbind()
      {
        Performance::StateManager::DisableTexture2D(stage);
      }

      void Texture2D::Recreate(int _stage)
      {
        Bind(_stage);

        CheckThisForErrors(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        CheckThisForErrors(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        CheckThisForErrors(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        if( mips == true )
        {
          CheckThisForErrors(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        }
        else
        {
          CheckThisForErrors(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        }

        switch( image->Format() )
        {
        default:
        case Utilities::Image::GRAY:
          {
            CheckThisForErrors(gluBuild2DMipmaps( GL_TEXTURE_2D, 1, image->Width(), image->Height(), GL_RED, GL_UNSIGNED_BYTE, image->PixelData() ));
          }
          break;
        case Utilities::Image::RGB:
          {
            CheckThisForErrors(gluBuild2DMipmaps( GL_TEXTURE_2D, 3, image->Width(), image->Height(), GL_RGB, GL_UNSIGNED_BYTE, image->PixelData() ));
          }
          break;
        case Utilities::Image::RGBA:
          {
            CheckThisForErrors(gluBuild2DMipmaps( GL_TEXTURE_2D, 4, image->Width(), image->Height(), GL_RGBA, GL_UNSIGNED_BYTE, image->PixelData() ));
          }
          break;
        }
        Unbind();
      }

      void Texture2D::UseMips( bool use_mips )
      {
        if( mips == use_mips )
          return;
        mips = use_mips;

        Bind();
        if( mips == true )
        {
          CheckThisForErrors(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        }
        else
        {
          CheckThisForErrors(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        }
        Unbind();
      }
    }
  }
}
