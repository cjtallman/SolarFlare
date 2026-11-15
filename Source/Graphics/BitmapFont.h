/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BitmapFont.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#ifndef _SF_CHARACTER_H_
#define _SF_CHARACTER_H_

#include <string> 

namespace SF
{
  namespace Text
  {
   

    class BitmapFont
    {
    public:
      BitmapFont( const std::string & filename );

      
      void DrawString( float x, float y, const std::string & text, float size ) const;

      float GetWidth( const std::string & text ) const;

    private:
      
      void Load( const std::string & filename );
      void Build();
      void Destroy();

      struct{
       int image_width;
       int image_height;
       int cell_width;
       int cell_height;
       char start_char;
       char width[256];
      }data;

      unsigned int display_list;
    };
  }
}

#endif
