/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    VertexBuffer3.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/11/2009 @ 22:22:11 CTallman
**  \brief
*******************************************************************************/

#include <cstring> // memcpy
#include <fstream>
#include "BitmapFont.h"
#include "OpenGLInclude.h"


namespace SF
{
  namespace Text
  {

    BitmapFont::BitmapFont( const std::string & filename )
    {
      Load( filename );
    }

    void BitmapFont::Load( const std::string & filename )
    {
      std::fstream file;
      file.open( filename.c_str(), std::fstream::in | std::fstream::binary );
      if( file.is_open() )
      {
        char dat[512];
        file.read(dat, 512);
        std::memcpy( &data, dat, sizeof(data) );
        file.close();
      }
      else
      {
        data.image_width = 512;
        data.image_height = 512;
        data.cell_width = 32;
        data.cell_height = 32;
        data.start_char = 0;
        std::memset( data.width, 32, 256 );
      }

      Build();
    }

    void BitmapFont::Build()
    {
      display_list = glGenLists(256);
      int pitch = data.image_width / data.cell_width;
      float row_factor = static_cast<float>(data.cell_width) / static_cast<float>(data.image_width);
      float col_factor = static_cast<float>(data.cell_height) / static_cast<float>(data.image_height);
      float one_pixel_wide = 1.0f / static_cast<float>(data.image_width);
      float one_pixel_high = 1.0f / static_cast<float>(data.image_height);

      for( int i = 0; i < 256; ++i )
      {
        int row = ( i - data.start_char ) / pitch;
        int col = ( i - data.start_char ) - ( row * pitch );
        float U0 = col * col_factor;
        float V0 = row * row_factor;
        float U1 = U0 + col_factor - one_pixel_wide;
        float V1 = V0 + row_factor - one_pixel_high;

        float width = static_cast<float>(data.width[i]) / static_cast<float>(data.cell_width);

        glNewList( display_list + i, GL_COMPILE );
        glBegin(GL_QUADS);
        glTexCoord2f( U0, V0 );
        glVertex3i( 0, 0, 0 );
        glTexCoord2f( U1, V0 );
        glVertex3i( 1, 0, 0 );
        glTexCoord2f( U1, V1 );
        glVertex3i( 1, 1, 0 );
        glTexCoord2f( U0, V1 );
        glVertex3i( 0, 1, 0 );
        glEnd();
        glTranslatef( width, 0, 0 );
        glEndList();
      }
    }

    void BitmapFont::Destroy()
    {
      glDeleteLists(display_list, 256);
    }

    void BitmapFont::DrawString( float x, float y, const std::string & text, float size ) const
    {
      float scale = size;

      PushMatrix();
      glTranslatef( x, y, 0.0f );
      glScalef( scale, scale, 1.0f);
      glListBase( display_list );
      glCallLists( static_cast<int>(text.length()), GL_UNSIGNED_BYTE, text.c_str() );
      PopMatrix();
    }


    float BitmapFont::GetWidth( const std::string & text ) const
    {
      int width = 0;
      for( std::string::const_iterator itr = text.begin(); itr != text.end(); ++itr )
        width += data.width[*itr];

      return static_cast<float>(width) / static_cast<float>(data.cell_width);
    }
  }
}
