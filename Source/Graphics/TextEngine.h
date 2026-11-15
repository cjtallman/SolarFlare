/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TextEngine.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:28:01 CTallman
**  \brief
*******************************************************************************/

#ifndef _SF_TEXT_ENGINE_H_
#define _SF_TEXT_ENGINE_H_

#include "Texture2D.h"
#include "Utilities/Singleton.h"
#include "BitmapFont.h"
#include <vector>
#include <fstream>

namespace SF
{
  namespace Graphics
  {

    class TextEngine : public Utilities::Patterns::Singleton<TextEngine>
    {
    public:
      TextEngine(void);
      ~TextEngine(void);

      void PrintText( float x, float y, const std::string & text, SF::Color text_color = SF::Color(255,255,255,200), float height = 20, unsigned int font_id = 0 );
      void PrintTextAdv( float x, float y, const std::string & text, SF::Color text_color = SF::Color(255,255,255,200), float height = 20, unsigned int font_id = 0 );

      float GetWidth( const std::string & text, float height = 20, unsigned int font_id = 0 ) const;

      unsigned int GetFontID( const std::string & font_name ) const;

      private:
        void BindTexture(unsigned int font_id);
        void UnbindTexture(unsigned int font_id);

        std::vector< std::pair<std::string,SharedTexture> > fonts;


        std::vector<Text::BitmapFont> bitmaps;
    };

  }
}

#endif
