/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CubemapTexture.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/
#ifndef SF_CUBEMAP_TEXTURE_H_
#define SF_CUBEMAP_TEXTURE_H_

#include "OpenGLInclude.h"
#include "Utilities/Color.h"
#include "Math/TVector3.h"
#include "Utilities/SharedPtr.h"

namespace SF
{
  namespace Graphics
  {
    namespace TextureType
    {

      class CubemapTexture
      {
      public:
        CubemapTexture( int width = 256 );
        ~CubemapTexture();

        int Width() const { return width; }

        void BeginCapture( int face );
        void EndCapture();

        void Bind( int stage = 0 );
        void Unbind();        

        int PixelDataSize() const;
        const unsigned char * PixelData( int face ) const;

        Color GetTexel( const Math::Vector3 & dir ) const;

        void UpdateLocalCopy(int face = -1);

        void Recreate( int stage = 0 );

        CubemapTexture & operator=(const CubemapTexture & A);
        CubemapTexture & DeepCopy(const CubemapTexture & A);
        CubemapTexture & ShallowCopy(const CubemapTexture & A);

      private:

        void ValidateFBO();

        void GuardedBind();
        void GuardedUnbind();

        GLuint cubemap;
        GLuint fbo;
        int width;
        GLint saved_handle;

        bool initialized;
        bool needs_recreated;
        bool bound;
        int stage;

        Utilities::SharedArray<unsigned char> pixel_data[6];
        int pixel_data_size;
      };

    }//namespace TextureType
  }//namespace Graphics
}//namespace SF

#endif
