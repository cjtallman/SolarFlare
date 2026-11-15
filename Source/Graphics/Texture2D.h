/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Texture2D.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/25/2009 @ 00:25 - CJT 
**  \brief   Represents a 2D texture
*******************************************************************************/

#ifndef SF_TEXTURE_2D_H
#define SF_TEXTURE_2D_H

#include "Utilities/Image.h"
#include "Utilities/SharedPtr.h"
#include "Utilities/Noise/BaseGenerator.h"

namespace SF
{
  namespace Graphics
  {
    namespace TextureType
    {

      enum WRAPMODE
      {
        REPEAT,
        CLAMP
      };

      class Texture2D
      {
      public:
        Texture2D();
        ~Texture2D(void);

        void LoadFromFile( const std::string & fullpath );
        void MakeHeightMap( Utilities::Noise::BaseGenerator& generator );

        void SetWrapModeU( enum WRAPMODE wrap ){wrapModeU = wrap;}
        void SetWrapModeV( enum WRAPMODE wrap ){wrapModeV = wrap;}

        int Width() const;
        int Height() const;

        const unsigned char * PixelData() const;

        void SetTexel( int r, int c, SF::Color color );
        void SetTexel( float u, float v, SF::Color color );

        SF::Color GetTexel( float u, float v ) const;
        SF::Color GetTexel( int r, int c ) const;
        
        void Bind(int stage = 0);
        void Unbind();
        unsigned int GetHandle() { return gl_handle; }

        void UseMips(bool use_mips = true);

        void Recreate(int stage = 0);
        Utilities::SharedPtr<Utilities::Image> image;

        Texture2D & operator=(const Texture2D & A);
        Texture2D & DeepCopy(const Texture2D & A);
        Texture2D & ShallowCopy(const Texture2D & A);

      private:

        unsigned int gl_handle;  
        int stage;

        bool mips;
        WRAPMODE wrapModeU;
        WRAPMODE wrapModeV;
      };
      
    }

    typedef Utilities::SharedPtr<Graphics::TextureType::Texture2D> SharedTexture;

  }
}

#endif
