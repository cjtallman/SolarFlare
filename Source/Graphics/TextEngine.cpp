/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TextEngine.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/07/2009 @ 19:24:22 - nwilliam
**  \brief   
*******************************************************************************/

#include "TextEngine.h"

#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"
#include "Engine/AssetManager.h"
#include "External/ticpp/include/ticpp.h"
#include "BitmapFont.h"

#include "Utilities/Misc.h"

namespace SF
{
  namespace Graphics
  {



    TextEngine::TextEngine(void)
    {
      std::string asset_dir = Globals::assetManager->GetDirectory();
      std::string file = asset_dir + std::string("XML/fontList.xml");

      try
      {
        ticpp::Document doc;
        doc.LoadFile( file );
        ticpp::Element * root = doc.FirstChildElement();

        if( root )
        {
          ticpp::Iterator< ticpp::Element > child("Font");
          for( child = child.begin(root); child != child.end(); child++ )
          {
            std::string font_name = child->GetAttribute("Name");
            std::string data_file = child->GetAttribute("DataFile");
            std::string mapped_name = child->GetAttribute("MappedTexture");

            if( !mapped_name.empty() )
            {
              Engine::Assets::Texture * tex = (Engine::Assets::Texture*)Globals::assetManager->GetAsset( mapped_name );
              if( tex )
              {
                if( !font_name.empty() )
                  Utilities::StrLwr( font_name );

                fonts.push_back( std::pair<std::string,SharedTexture>(font_name,tex->image));
                bitmaps.push_back( Text::BitmapFont( asset_dir + data_file) );
              }
            }

          }
        }
      }
      catch( ticpp::Exception & e )
      {
        Globals::console->Error(e.what());
      }
    }

    TextEngine::~TextEngine(void)
    {
    }

    unsigned int TextEngine::GetFontID( const std::string & font_name ) const
    {
      std::string font(font_name);
      Utilities::StrLwr(font);

      for( unsigned i = 0; i < (unsigned)fonts.size(); ++i )
        if( font == fonts[i].first )
          return i;

      Globals::console->Warning(std::string("TextEngine::GetFontID failed to find font: ") + font_name );
      return 0;
    }

    void TextEngine::PrintText( float x, float y, const std::string & text, SF::Color text_color, float height, unsigned int font_id )
    {
      BindTexture(font_id);
      Enable2D();
      glColor4ub( text_color.r, text_color.g, text_color.b, text_color.a );
      bitmaps[font_id].DrawString( x, y, text, height );
      Disable2D();
      UnbindTexture(font_id);
    }

    void TextEngine::PrintTextAdv( float x, float y, const std::string & text, SF::Color text_color, float height, unsigned int font_id )
    {
      BindTexture(font_id);
      glColor4ub( text_color.r, text_color.g, text_color.b, text_color.a );
      bitmaps[font_id].DrawString( x, y, text, height );
      UnbindTexture(font_id);
    }

    void TextEngine::BindTexture(unsigned int font_id)
    {
      if( font_id < fonts.size() )
        fonts[font_id].second->Bind();
    }

    void TextEngine::UnbindTexture(unsigned int font_id)
    {
      if( font_id < fonts.size() )
        fonts[font_id].second->Unbind();
    }

    float TextEngine::GetWidth( const std::string & text, float height, unsigned int font_id ) const
    {
      float width = 0;

      if( font_id < (unsigned int)fonts.size() && !text.empty() )
        width = height * bitmaps[font_id].GetWidth( text );

      return width;
    }
  }
}
