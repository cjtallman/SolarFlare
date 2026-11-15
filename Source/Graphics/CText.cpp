/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CText.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 00:56 - CJT 
**  \brief   A Text rendering component.
*******************************************************************************/

#include "OpenGLInclude.h"
#include "CText.h"
#include "Math/TVector3.h"
#include "Engine/Entity.h"
#include "Math/CTransform.h"
#include "External/ticpp/include/ticpp.h"
#include "StateManager.h"

#include <sstream>

#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {
      std::string CText::empty_string = "";

      Engine::BaseComponent * CText::CreateComponent( const ticpp::Element * element )
      {
        if( element )
        {
          CText * out = new CText();
          try
          {
            ticpp::Element * child = 0;

            ////////////////////////////////////////////////////////////////////
            // Color Portion
            ////////////////////////////////////////////////////////////////////
            child = element->FirstChildElement( "color", false );
            if( child )
            {
              int r;
              int g;
              int b;
              int a;
              child->GetAttributeOrDefault("r", &r, 255);
              child->GetAttributeOrDefault("g", &g, 255);
              child->GetAttributeOrDefault("b", &b, 255);
              child->GetAttributeOrDefault("a", &a, 255);
              out->SetColor( SF::Color(
                (SF::Color::Type)r,
                (SF::Color::Type)g,
                (SF::Color::Type)b,
                (SF::Color::Type)a));
            }

            ////////////////////////////////////////////////////////////////////
            // Font Size
            ////////////////////////////////////////////////////////////////////
            child = element->FirstChildElement( "size", false );
            if( child )
            {
              float fontsize;
              child->GetAttributeOrDefault("value", &fontsize, 20.0f );
              out->SetFontSize( fontsize );
            }

            ////////////////////////////////////////////////////////////////////
            // Font Portion
            ////////////////////////////////////////////////////////////////////
            child = element->FirstChildElement( "font", false );
            if( child )
            {
              std::string fontname = child->GetAttribute("name");
              out->SetFont( fontname );
            }

            child = element->FirstChildElement( "shader", false );
            if( child )
            {
              std::string shader_name = child->GetAttribute("fragment");
              if( !shader_name.empty() )
                out->SetFragmentShader( shader_name );
            }

            ////////////////////////////////////////////////////////////////////
            // Alignment Portion
            ////////////////////////////////////////////////////////////////////
            child = element->FirstChildElement( "alignment", false );
            if( child )
            {
              std::string horizontal = child->GetAttribute("horizontal");
              if( !horizontal.empty() )
              {
                if( horizontal == "left" )
                  out->SetHorizontalAlignment( LEFT );
                else if( horizontal == "center" )
                  out->SetHorizontalAlignment( CENTER );
                else
                  out->SetHorizontalAlignment( RIGHT );
              }

              std::string vertical = child->GetAttribute("vertical");
              if( !vertical.empty() )
              {
                if( vertical == "top" )
                  out->SetVerticalAlignment( TOP );
                else if( vertical == "middle" )
                  out->SetVerticalAlignment( MIDDLE );
                else
                  out->SetVerticalAlignment( BOTTOM );
              }
            }

            return out;
          }

          catch( ticpp::Exception & )
          {
            delete out;
            return 0;
          }
        }
        return 0;
      }

      Engine::BaseComponent * CText::Clone() const
      {
        CText * new_text = new CText();
        *new_text = *this;
        new_text->transform = 0;

        new_text->SetFragmentShader(frag);

        return new_text;
      }

      bool CText::InitDependencies( Engine::Entity * parent )
      {
        if( parent && parent->Has( Engine::ComponentType::UI_ELEMENT ) )
          interface_element = static_cast<UI::CBasicElement *>(parent->GetFirstComponent( Engine::ComponentType::UI_ELEMENT ));

        if( parent && parent->Has( Engine::ComponentType::TRANSFORM ) )
          transform = static_cast<Math::CTransform*>(parent->GetFirstComponent( Engine::ComponentType::TRANSFORM ));
        else
          return false;
        return true;
      }

      CText::CText(void)
        : Engine::BaseComponent( Engine::ComponentType::TEXT )
      {
        transform = 0;
        SetFontSize( 20 );
        horizontal_alignment = LEFT;
        vertical_alignment = TOP;
        font_id = 0;
        interface_element = 0;
        frag = new CgShader;
        cg_grad_top = new ShaderParameter;
        cg_grad_bottom = new ShaderParameter;
      }

      void CText::Draw( ) const
      {
        static TextEngine * text_engine = TextEngine::Instance();
        Math::Vector3 offset;

        Enable2D();

        PushMatrix();

        if( transform )
        {
          Math::Vector3 pos = transform->GetWorldPosition();
          glTranslatef(pos.x, pos.y, 0);
        }

        switch( horizontal_alignment )
        {
        default:
        case LEFT:
          offset.x = 0.0f;
          break;
        case CENTER:
          offset.x = 0.5f;
          break;
        case RIGHT:
          offset.x = 1.0f;
          break;
        }

        switch ( vertical_alignment )
        {
        default:
        case TOP:
          offset.y = 0.0f * text_height;
          break;
        case MIDDLE:
          offset.y = 0.5f * text_height;
          break;
        case BOTTOM:
          offset.y = 1.0f * text_height;
          break;
        }

        

        Color4f tmpColor(1.f,1.f,1.f,1.f);

        if(interface_element)
          tmpColor = interface_element->GetColor();

        int size = (int)text.size();
        Performance::StateManager::SetActiveFragmentShader(frag.Get());
          if( cg_grad_top->param )
            cgSetParameter4f( cg_grad_top->param, text_grad_top.r, text_grad_top.g, text_grad_top.b, text_grad_top.a );
          
          if( cg_grad_bottom->param )
            cgSetParameter4f( cg_grad_bottom->param, text_grad_bottom.r, text_grad_bottom.g, text_grad_bottom.b, text_grad_bottom.a );
          

        for( int i = 0; i < size; ++i )
        {
          float font_width = text_engine->GetWidth( text[i], font_height, font_id );
          
          text_engine->PrintTextAdv(
            -offset.x * font_width,
            -offset.y + font_height * i,
            text[i],
            tmpColor,
            font_height,
            font_id);
        }
        Performance::StateManager::SetActiveFragmentShader(0);

        PopMatrix();

        Disable2D();

      }

      void CText::SetFragmentShader( SharedShader2 shader )
      {
        frag = shader;
        cg_grad_top     = frag->GetParameter("grad_top");
        cg_grad_bottom  = frag->GetParameter("grad_bottom");
      }

      void CText::SetFragmentShader( const std::string & filename )
      {
        frag = Globals::shaderLoader->GetFragmentShader(filename);
        cg_grad_top     = frag->GetParameter("grad_top");
        cg_grad_bottom  = frag->GetParameter("grad_bottom");
      }

      void CText::SetText( const std::vector<std::string> & _text )
      {
        text = _text;
        UpdateTextRegion();
      }

      void CText::SetText( const std::string & _text )
      {
        text.clear();
        text.push_back( _text );
        UpdateTextRegion();
      }

      const std::string & CText::GetText( int index ) const
      {
        if( text.empty() )
          return empty_string;
        else
          return text[index];
      }

      float CText::GetRegionWidth() const
      {
        return text_width;
      }

      float CText::GetRegionHeight() const
      {
        return text_height;
      }

      void CText::SetColor( const SF::Color4f & _color )
      {
        text_grad_top = _color;
        text_grad_bottom = _color;
      }

      void CText::SetColor( const Color4f & A, const Color4f & B )
      {
        text_grad_top = A;
        text_grad_bottom = B;
      }

      const SF::Color4f & CText::GetColor() const
      {
        return text_grad_top;
      }

      void CText::SetTransparency(float percent)
      {
        text_grad_top.a = percent;
        text_grad_bottom.a = percent;
      }

      float CText::GetTransparency() const
      {
        return text_grad_top.a;
      }

      void CText::SetFont( const std::string & font_name )
      {
        font_id = TextEngine::Instance()->GetFontID(font_name);
      }

      void CText::SetFontSize( float size )
      {
        font_height = size;
        UpdateTextRegion();
      }

      float CText::GetFontSize() const
      {
        return font_height;
      }

      void CText::SetHorizontalAlignment( HORIZONTAL align )
      {
        horizontal_alignment = align;
      }

      void CText::SetVerticalAlignment( VERTICAL align )
      {
        vertical_alignment = align;
      }

      int CText::GetHorizontalAlignment() const
      {
        return int(horizontal_alignment);
      }

      int CText::GetVerticalAlignment() const
      {
        return int(vertical_alignment);
      }

      void CText::UpdateTextRegion()
      {
        int line_count = (int)text.size();
        float longest_line = 0;

        for( int i = 0; i < line_count; ++i )
          longest_line = std::max<float>( TextEngine::Instance()->GetWidth( text[i], font_height, font_id ), longest_line);

        text_width = longest_line;
        text_height = font_height * line_count;
      }


      void CText::PushToLua( lua_State *ls )
      {
        Lunar<CText>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CText )
        LUA_NAME_METHOD( CText, "GetRegionWidth", LGetRegionWidth )
        LUA_NAME_METHOD( CText, "GetRegionHeight", LGetRegionHeight )
        LUA_NAME_METHOD( CText, "GetFontSize", LGetFontSize )
        LUA_NAME_METHOD( CText, "GetHorizontalAlignment", LGetHorizontalAlignment )
        LUA_NAME_METHOD( CText, "GetVerticalAlignment", LGetVerticalAlignment )

        LUA_NAME_METHOD( CText, "SetText", LSetText )
        LUA_NAME_METHOD( CText, "SetTextColor", LSetTextColor )
        LUA_NAME_METHOD( CText, "SetFont", LSetFont )
        LUA_NAME_METHOD( CText, "SetTransparency", LSetTransparency )
        LUA_NAME_METHOD( CText, "GetTransparency", LGetTransparency )
        LUA_NAME_METHOD( CText, "SetFontSize", LSetFontSize )
        LUA_NAME_METHOD( CText, "SetHorizontalAlignment", LSetHorizontalAlignment )
        LUA_NAME_METHOD( CText, "SetVerticalAlignment", LSetVerticalAlignment )
      LUA_END_DEFINE_CLASS


      LUA_DEFINE_METHOD( CText, LGetRegionWidth )
      {
        lua_pushnumber( ls, GetRegionWidth() );
        return 1;
      }

      LUA_DEFINE_METHOD( CText, LGetRegionHeight )
      {
        lua_pushnumber( ls, GetRegionHeight() );
        return 1;
      }

      LUA_DEFINE_METHOD( CText, LGetFontSize )
      {
        lua_pushnumber( ls, GetFontSize() );
        return 1;
      }

      LUA_DEFINE_METHOD( CText, LGetHorizontalAlignment )
      {
        lua_pushnumber( ls, GetHorizontalAlignment() );
        return 1;
      }

      LUA_DEFINE_METHOD( CText, LGetVerticalAlignment )
      {
        lua_pushnumber( ls, GetVerticalAlignment() );
        return 1;
      }

      LUA_DEFINE_METHOD( CText, LSetFont )
      {
        const char * font_name = luaL_checkstring( ls, 1 );
        
        if( font_name )
          SetFont( std::string(font_name) );

        return 0;
      }


      LUA_DEFINE_METHOD( CText, LSetText )
      {
        const char * new_text = luaL_checkstring( ls, 1 );
        std::vector<std::string> paragraph;

        if( new_text )
        {
          std::stringstream ss(new_text);
          do
          {
            std::stringbuf buf;
            ss.get( buf, '\n' );
            ss.get();

            if( !buf.str().empty() )
              paragraph.push_back(buf.str());
          } while ( ss.gcount() );

          SetText( paragraph );
        }
        return 0;
      }

      LUA_DEFINE_METHOD( CText, LSetTextColor )
      {
        Color4i A;
        Color4i B;

        int top = lua_gettop(ls);
        

        switch( top )
        {
        case 8:
          B.a = luaL_checkint(ls, 8);
        case 7:
          B.b = luaL_checkint(ls, 7);
        case 6:
          B.g = luaL_checkint(ls, 6);
        case 5:
          B.r = luaL_checkint(ls, 5);
        case 4:
          A.a = luaL_checkint(ls, 4);
        case 3:
          A.b = luaL_checkint(ls, 3);
        case 2:
          A.g = luaL_checkint(ls, 2);
        case 1:
          A.r = luaL_checkint(ls, 1);
        default:
          break;
        }

        if( top > 4 )
        {
          SetColor( A, B );
        }
        else
        {
          SetColor( A );
        }

        return 0;
      }

      LUA_DEFINE_METHOD( CText, LSetTransparency )
      {
        float percent = luaL_checkfloat(ls, 1);
        SetTransparency(percent);
        return 0;
      }

       LUA_DEFINE_METHOD( CText, LGetTransparency )
      {
        lua_pushnumber( ls, GetTransparency() );
        return 1;
      }

      LUA_DEFINE_METHOD( CText, LSetFontSize )
      {
        float size = luaL_checkfloat(ls, 1);
        SetFontSize(size);
        return 0;
      }

      LUA_DEFINE_METHOD( CText, LSetHorizontalAlignment )
      {
        const char * options[] = { "LEFT", "CENTER", "RIGHT", 0 };
        int opt = luaL_checkoption(ls, 1, 0, options );
        SetHorizontalAlignment( HORIZONTAL(opt));
        return 0;
      }

      LUA_DEFINE_METHOD( CText, LSetVerticalAlignment )
      {
        const char * options[] = { "TOP", "MIDDLE", "BOTTOM", 0 };
        int opt = luaL_checkoption(ls, 1, 0, options );
        SetVerticalAlignment( VERTICAL(opt));
        return 0;
      }


    }
  }
}
