/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CTooltip.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 03/13/2009 @ 12:46 - CJT 
**  \brief 
*******************************************************************************/

#include "OpenGLInclude.h"
#include "CToolTip.h"
#include "TextEngine.h"
#include "Math/TVector3.h"
#include "Engine/Entity.h"
#include "Math/CTransform.h"
#include "Engine/AssetManager.h"
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
      std::string CToolTip::empty_string = "";

      Engine::BaseComponent * CToolTip::CreateComponent( const ticpp::Element * element )
      {
        if( element )
        {
          CToolTip * out = new CToolTip();
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
              out->SetColor( Color4i(r,g,b,a) );
            }

            child = element->FirstChildElement( "texture", false );
            if( child )
            {
              static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
              std::string texture_name = child->GetAttribute("name");

              if( !texture_name.empty() )
              {
                out->SetTexture( texture_name );
              }
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
                  out->SetHorizontalAlignment( ALIGNMENT::LEFT );
                else if( horizontal == "center" )
                  out->SetHorizontalAlignment( ALIGNMENT::CENTER );
                else
                  out->SetHorizontalAlignment( ALIGNMENT::RIGHT );
              }

              std::string vertical = child->GetAttribute("vertical");
              if( !vertical.empty() )
              {
                if( vertical == "top" )
                  out->SetVerticalAlignment( ALIGNMENT::TOP );
                else if( vertical == "middle" )
                  out->SetVerticalAlignment( ALIGNMENT::MIDDLE );
                else
                  out->SetVerticalAlignment( ALIGNMENT::BOTTOM );
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

      Engine::BaseComponent * CToolTip::Clone() const
      {
        CToolTip * out = new CToolTip();
        *out = *this;

        out->transform = 0;
        out->SetFragmentShader(frag);
        out->SetTexture(panel_texture);
        

        return out;
      }

      bool CToolTip::InitDependencies( Engine::Entity * parent )
      {
        if( parent && parent->Has( Engine::ComponentType::UI_ELEMENT ) )
          interface_element = static_cast<UI::CBasicElement *>(parent->GetFirstComponent( Engine::ComponentType::UI_ELEMENT ));

        if( parent && parent->Has( Engine::ComponentType::TRANSFORM ) )
          transform = static_cast<Math::CTransform*>(parent->GetFirstComponent( Engine::ComponentType::TRANSFORM ));
        else
          return false;
        return true;
      }

      CToolTip::CToolTip(void)
      {
        transform = 0;
        SetFontSize( 20 );
        horizontal_alignment = ALIGNMENT::CENTER;
        vertical_alignment = ALIGNMENT::MIDDLE;
        anchor = ANCHOR::TOP_LEFT;
        font_id = 0;
        interface_element = 0;
        frag = new CgShader;
        cg_grad_top = new ShaderParameter;
        cg_grad_bottom = new ShaderParameter;    
        padding.top = padding.bottom = padding.left = padding.right = 10;
      }

      void CToolTip::Draw( )
      {
        Enable2D();

        PushMatrix();

        Math::Vector3 offset;
        Math::Vector3 pos;

        if( interface_element )
        {
          pos = interface_element->GetTransform()->GetWorldPosition();
          padding.left = interface_element->paddingLeft;
          padding.right = interface_element->paddingRight;
          padding.top = interface_element->paddingTop;
          padding.bottom = interface_element->paddingBottom;
        }
        else if( transform )
        {
          pos = transform->GetWorldPosition();
        }

        switch( anchor )
        {
        default:
        case ANCHOR::TOP_LEFT:
          offset = Vector3(0, 0, 0);
          break;
        case ANCHOR::TOP:
          offset = Vector3(-text_width/2.0f - 0.5f*(padding.left + padding.right), 0, 0);
          break;
        case ANCHOR::TOP_RIGHT:
          offset = Vector3(-text_width - padding.left, 0, 0);
          break;
        case ANCHOR::RIGHT:
          offset = Vector3(-text_width - padding.left, -text_height/2.0f - .5f * (padding.top + padding.bottom), 0);
          break;
        case ANCHOR::BOTTOM_RIGHT:
          offset = Vector3(-text_width - padding.left, -text_height - padding.top - padding.bottom, 0);
          break;
        case ANCHOR::BOTTOM:
          offset = Vector3(-text_width/2.0f - 0.5f*(padding.left + padding.right), -text_height - padding.top - padding.bottom, 0);
          break;
        case ANCHOR::BOTTOM_LEFT:
          offset = Vector3(0, -text_height - padding.top - padding.bottom, 0);
          break;
        case ANCHOR::LEFT:
          offset = Vector3(0, -text_height/2.0f - .5f * (padding.top + padding.bottom), 0);
          break;
        case ANCHOR::CENTER:
          offset = Vector3(-text_width/2.0f - 0.5f*(padding.left + padding.right), -text_height/2.0f - .5f * (padding.top + padding.bottom), 0);
          break;
        }

        glTranslatef(pos.x + offset.x, pos.y + offset.y, z_order / 100.0f);

        DrawPanel();

        DrawText();

        PopMatrix();

        Disable2D();
      }

      void CToolTip::DrawPanel() const
      {
        float divisions = 12.0f;
        const float u0 = 0.0f;
        const float u1 = 1.0f / divisions;
        const float u2 = 1.0f - 1.0f / divisions;
        const float u3 = 1.0f;
        const float v0 = u0;
        const float v1 = u1;
        const float v2 = u2;
        const float v3 = u3;

        const float corner = 10.0f;

        const float x0 = 0.0f;
        const float x1 = corner;
        const float x2 = text_width + padding.left + padding.right - corner;
        const float x3 = text_width + padding.left + padding.right;

        const float y0 = 0.0f;
        const float y1 = corner;
        const float y2 = text_height + padding.top + padding.bottom - corner;
        const float y3 = text_height + padding.top + padding.bottom;

        panel_texture->Bind(0);

        if( interface_element )
          glColor4fv( interface_element->GetColor().Array() );
        else
          glColor4fv( panel_color.Array() );

        glBegin(GL_QUADS);

        //top left
        glTexCoord2f(u0, v0);
        glVertex2f(x0, y0);
        glTexCoord2f(u1, v0);
        glVertex2f(x1, y0);
        glTexCoord2f(u1, v1);
        glVertex2f(x1, y1);
        glTexCoord2f(u0, v1);
        glVertex2f(x0, y1);

        //top
        glTexCoord2f(u1, v0);
        glVertex2f(x1, y0);
        glTexCoord2f(u2, v0);
        glVertex2f(x2, y0);
        glTexCoord2f(u2, v1); 
        glVertex2f(x2, y1);
        glTexCoord2f(u1, v1);
        glVertex2f(x1, y1);

        //top right
        glTexCoord2f(u2, v0);
        glVertex2f(x2, y0);
        glTexCoord2f(u3, v0);
        glVertex2f(x3, y0);
        glTexCoord2f(u3, v1);
        glVertex2f(x3, y1);
        glTexCoord2f(u2, v1);
        glVertex2f(x2, y1);

        //left                  
        glTexCoord2f(u0, v1);
        glVertex2f(x0, y1);
        glTexCoord2f(u1, v1);
        glVertex2f(x1, y1);
        glTexCoord2f(u1, v2);
        glVertex2f(x1, y2);
        glTexCoord2f(u0, v2);
        glVertex2f(x0, y2);

        //center                
        glTexCoord2f(u1, v1);
        glVertex2f(x1, y1);
        glTexCoord2f(u2, v1);
        glVertex2f(x2, y1);
        glTexCoord2f(u2, v2); 
        glVertex2f(x2, y2);
        glTexCoord2f(u1, v2);
        glVertex2f(x1, y2);

        //right
        glTexCoord2f(u2, v1);
        glVertex2f(x2, y1);
        glTexCoord2f(u3, v1);
        glVertex2f(x3, y1);
        glTexCoord2f(u3, v2);
        glVertex2f(x3, y2);
        glTexCoord2f(u2, v2);
        glVertex2f(x2, y2);

        //bottom left           
        glTexCoord2f(u0, v2);   
        glVertex2f(x0, y2);
        glTexCoord2f(u1, v2);   
        glVertex2f(x1, y2);
        glTexCoord2f(u1, v3);   
        glVertex2f(x1, y3);
        glTexCoord2f(u0, v3); 
        glVertex2f(x0, y3);

        //bottom               
        glTexCoord2f(u1, v2);
        glVertex2f(x1, y2);
        glTexCoord2f(u2, v2);  
        glVertex2f(x2, y2);
        glTexCoord2f(u2, v3);  
        glVertex2f(x2, y3);
        glTexCoord2f(u1, v3);   
        glVertex2f(x1, y3);

        //bottom right
        glTexCoord2f(u2, v2);
        glVertex2f(x2, y2);
        glTexCoord2f(u3, v2);
        glVertex2f(x3, y2);
        glTexCoord2f(u3, v3);
        glVertex2f(x3, y3);
        glTexCoord2f(u2, v3); 
        glVertex2f(x2, y3);

        glEnd();

        panel_texture->Unbind();
      }

      void CToolTip::DrawText() const
      {
        static TextEngine * text_engine = TextEngine::Instance();
        Math::Vector3 offset;

        switch( horizontal_alignment )
        {
        default:
        case ALIGNMENT::LEFT:
          offset.x = 0.0f;
          break;
        case ALIGNMENT::CENTER:
          offset.x = 0.5f;
          break;
        case ALIGNMENT::RIGHT:
          offset.x = 1.0f;
          break;
        }

        Performance::StateManager::SetActiveFragmentShader(frag.Get());

        if( cg_grad_top->param )
          cgSetParameter4f( cg_grad_top->param, text_grad_top.r, text_grad_top.g, text_grad_top.b, text_grad_top.a );

        if( cg_grad_bottom->param )
          cgSetParameter4f( cg_grad_bottom->param, text_grad_bottom.r, text_grad_bottom.g, text_grad_bottom.b, text_grad_bottom.a );
        
        
        Color4f tmpColor;

        if( interface_element )
          tmpColor = interface_element->GetColor();
        else
          tmpColor = panel_color;
        

        int size = (int)text.size();
        for( int i = 0; i < size; ++i )
        {
          float font_width = text_engine->GetWidth( text[i], font_height, font_id );

          text_engine->PrintTextAdv(
            font_width * -offset.x + offset.x * (padding.left + padding.right + text_width),
            padding.top + font_height * i,
            text[i],
            tmpColor,
            font_height,
            font_id);
        }

        Performance::StateManager::SetActiveFragmentShader(0);
      }

      void CToolTip::SetFragmentShader( SharedShader2 shader )
      {
        frag = shader;
        cg_grad_top     = frag->GetParameter("grad_top");
        cg_grad_bottom  = frag->GetParameter("grad_bottom");
      }

      void CToolTip::SetFragmentShader( const std::string & filename )
      {
        frag = Globals::shaderLoader->GetFragmentShader(filename);
        cg_grad_top     = frag->GetParameter("grad_top");
        cg_grad_bottom  = frag->GetParameter("grad_bottom");
      }

      void CToolTip::SetTexture( SharedTexture _texture )
      {
        panel_texture = _texture;
        panel_texture->UseMips(false);
      }

      void CToolTip::SetTexture( const std::string & mappedName )
      {
        static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
        SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( mappedName );
        if( asset )
          SetTexture( asset->image );
        else
        {
          Globals::console->Warning("Invalid Texture: \"" + mappedName + "\" is not loaded.");
        }
      }

      void CToolTip::SetText( const std::vector<std::string> & _text )
      {
        text = _text;
        UpdateTextRegion();
      }

      void CToolTip::SetText( const std::string & _text )
      {
        text.clear();
        text.push_back( _text );
        UpdateTextRegion();
      }

      const std::string & CToolTip::GetText( int index ) const
      {
        if( text.empty() )
          return empty_string;
        else
          return text[index];
      }

      float CToolTip::GetRegionWidth() const
      {
        return text_width;
      }

      float CToolTip::GetRegionHeight() const
      {
        return text_height;
      }

      void CToolTip::SetTextColor( const Color4f & _color )
      {
        text_grad_top = _color;
        text_grad_bottom = _color;
      }

      void CToolTip::SetTextColor( const Color4f & A, const Color4f & B )
      {
        text_grad_top = A;
        text_grad_bottom = B;
      }

      const Color4f & CToolTip::GetTextColor() const
      {
        return text_grad_top;
      }

      void CToolTip::SetColor( const Color4f & _color )
      {
        panel_color = _color;
      }

      const Color4f & CToolTip::GetColor() const
      {
        return panel_color;
      }

      void CToolTip::SetTransparency(float percent)
      {
        panel_color.a = percent;
        text_grad_top.a = percent;
        text_grad_bottom.a = percent;
      }

      float CToolTip::GetTransparency() const
      {
        return panel_color.a;
      }

      void CToolTip::SetFont( const std::string & font_name )
      {
        font_id = TextEngine::Instance()->GetFontID(font_name);
      }

      void CToolTip::SetFontSize( float size )
      {
        font_height = size;
        UpdateTextRegion();
      }

      float CToolTip::GetFontSize() const
      {
        return font_height;
      }

      void CToolTip::SetHorizontalAlignment( ALIGNMENT::HORIZONTAL align )
      {
        horizontal_alignment = align;
      }

      void CToolTip::SetVerticalAlignment( ALIGNMENT::VERTICAL align )
      {
        vertical_alignment = align;
      }

      int CToolTip::GetHorizontalAlignment() const
      {
        return int(horizontal_alignment);
      }

      int CToolTip::GetVerticalAlignment() const
      {
        return int(vertical_alignment);
      }

      void CToolTip::SetPadding(float top, float right, float bottom, float left )
      {
        padding.top = top;
        padding.bottom = bottom;
        padding.left = left;
        padding.right = right;
      }

      void CToolTip::UpdateTextRegion()
      {
        int line_count = (int)text.size();
        float longest_line = 0;

        for( int i = 0; i < line_count; ++i )
          longest_line = std::max<float>( TextEngine::Instance()->GetWidth( text[i], font_height, font_id ), longest_line);

        text_width = longest_line;
        text_height = font_height * line_count;
      }


      void CToolTip::PushToLua( lua_State *ls )
      {
        Lunar<CToolTip>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CToolTip )
        LUA_NAME_METHOD( CToolTip, "GetRegionWidth", LGetRegionWidth )
        LUA_NAME_METHOD( CToolTip, "GetRegionHeight", LGetRegionHeight )
        LUA_NAME_METHOD( CToolTip, "GetFontSize", LGetFontSize )
        LUA_NAME_METHOD( CToolTip, "GetHorizontalAlignment", LGetHorizontalAlignment )
        LUA_NAME_METHOD( CToolTip, "GetVerticalAlignment", LGetVerticalAlignment )

        LUA_NAME_METHOD( CToolTip, "SetColor", LSetColor )
        LUA_NAME_METHOD( CToolTip, "SetText", LSetText )
        LUA_NAME_METHOD( CToolTip, "SetTextColor", LSetTextColor )
        LUA_NAME_METHOD( CToolTip, "SetFont", LSetFont )
        LUA_NAME_METHOD( CToolTip, "SetTransparency", LSetTransparency )
        LUA_NAME_METHOD( CToolTip, "GetTransparency", LGetTransparency )
        LUA_NAME_METHOD( CToolTip, "SetFontSize", LSetFontSize )
        LUA_NAME_METHOD( CToolTip, "SetHorizontalAlignment", LSetHorizontalAlignment )
        LUA_NAME_METHOD( CToolTip, "SetVerticalAlignment", LSetVerticalAlignment )
        LUA_NAME_METHOD( CToolTip, "SetPadding", LSetPadding )
        LUA_NAME_METHOD( CToolTip, "SetAnchor", LSetAnchor )
        LUA_END_DEFINE_CLASS


        LUA_DEFINE_METHOD( CToolTip, LGetRegionWidth )
      {
        lua_pushnumber( ls, GetRegionWidth() );
        return 1;
      }

      LUA_DEFINE_METHOD( CToolTip, LGetRegionHeight )
      {
        lua_pushnumber( ls, GetRegionHeight() );
        return 1;
      }

      LUA_DEFINE_METHOD( CToolTip, LGetFontSize )
      {
        lua_pushnumber( ls, GetFontSize() );
        return 1;
      }

      LUA_DEFINE_METHOD( CToolTip, LGetHorizontalAlignment )
      {
        lua_pushnumber( ls, GetHorizontalAlignment() );
        return 1;
      }

      LUA_DEFINE_METHOD( CToolTip, LGetVerticalAlignment )
      {
        lua_pushnumber( ls, GetVerticalAlignment() );
        return 1;
      }

      LUA_DEFINE_METHOD( CToolTip, LSetFont )
      {
        const char * font_name = luaL_checkstring( ls, 1 );

        if( font_name )
          SetFont( std::string(font_name) );

        return 0;
      }


      LUA_DEFINE_METHOD( CToolTip, LSetText )
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

      LUA_DEFINE_METHOD( CToolTip, LSetColor )
      {
        Color4i A;
        int top = lua_gettop(ls);

        switch( top )
        {
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
        SetColor( A );
        return 0;
      }



      LUA_DEFINE_METHOD( CToolTip, LSetTextColor )
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
          SetTextColor( A, B );
        }
        else
        {
          SetTextColor( A );
        }

        return 0;
      }

      LUA_DEFINE_METHOD( CToolTip, LSetTransparency )
      {
        float percent = luaL_checkfloat(ls, 1);
        SetTransparency(percent);
        return 0;
      }

      LUA_DEFINE_METHOD( CToolTip, LGetTransparency )
      {
        lua_pushnumber( ls, GetTransparency() );
        return 1;
      }

      LUA_DEFINE_METHOD( CToolTip, LSetFontSize )
      {
        float size = luaL_checkfloat(ls, 1);
        SetFontSize(size);
        return 0;
      }

      LUA_DEFINE_METHOD( CToolTip, LSetHorizontalAlignment )
      {
        const char * options[] = { "LEFT", "CENTER", "RIGHT", 0 };
        int opt = luaL_checkoption(ls, 1, 0, options );
        SetHorizontalAlignment( ALIGNMENT::HORIZONTAL(opt));
        return 0;
      }

      LUA_DEFINE_METHOD( CToolTip, LSetVerticalAlignment )
      {
        const char * options[] = { "TOP", "MIDDLE", "BOTTOM", 0 };
        int opt = luaL_checkoption(ls, 1, 0, options );
        SetVerticalAlignment( ALIGNMENT::VERTICAL(opt));
        return 0;
      }

      LUA_DEFINE_METHOD( CToolTip, LSetPadding )
      {
        float top = luaL_checkfloat( ls, 1 );
        float right = luaL_checkfloat( ls, 2 );
        float bottom = luaL_checkfloat( ls, 3 );
        float left = luaL_checkfloat( ls, 4 );

        SetPadding(top,right,bottom,left);
        return 0;
      }

      LUA_DEFINE_METHOD( CToolTip, LSetAnchor )
      {
        const char * options[] = { "TOP_LEFT", "TOP", "TOP_RIGHT", "RIGHT", "BOTTOM_RIGHT", "BOTTOM", "BOTTOM_LEFT", "LEFT", "CENTER", 0 };
        int opt = luaL_checkoption(ls, 1, 0, options );
        anchor = ANCHOR::ANCHORPOS(opt);
        return 0;
      }


    }
  }
}
