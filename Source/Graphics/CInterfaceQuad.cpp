/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CInterfaceQuad.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 00:56 - CJT 
**  \brief   Draws interface components, mainly used for buttons.
*******************************************************************************/

#include "Engine/Entity.h"
#include "CInterfaceQuad.h"
#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"
#include "Engine/AssetManager.h"
#include "StateManager.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {

      Engine::BaseComponent * CInterfaceQuad::CreateComponent( const ticpp::Element * element )
      {
        if( element )
        {
          CInterfaceQuad * out = new CInterfaceQuad();
          try
          {
            element->GetAttributeOrDefault("transform", &out->m_transformName, std::string());

            ticpp::Element * child;

            child = element->FirstChildElement( "shader", false );
            if( child )
            {
              std::string shader_name = child->GetAttribute("fragment");
              if( !shader_name.empty() )
                out->SetFragmentShader( shader_name );
            }

            child = element->FirstChildElement( "texture", false );
            if( child )
            {
              static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;

              std::string disabled = child->GetAttribute("disabled");
              std::string up = child->GetAttribute("up");
              std::string over = child->GetAttribute("over");
              std::string down = child->GetAttribute("down");

              if( !disabled.empty() )
              {
                SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( disabled );
                if( asset )
                  out->SetTexture( UI_STATE::DISABLED, asset->image );
              }

              if( !up.empty() )
              {
                SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( up );
                if( asset )
                  out->SetTexture( UI_STATE::UP, asset->image );
              }

              if( !over.empty() )
              {
                SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( over );
                if( asset )
                  out->SetTexture( UI_STATE::OVER, asset->image );
              }

              if( !down.empty() )
              {
                SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( down );
                if( asset )
                  out->SetTexture( UI_STATE::DOWN, asset->image );
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
        else
          return 0;
      }


      Engine::BaseComponent * CInterfaceQuad::Clone() const
      {
        CInterfaceQuad * out = new CInterfaceQuad();

        for( int i = 0; i < UI_STATE::LAST; ++i )
          out->SetTexture( i, texture[i] );

        out->SetFragmentShader(frag);
        out->width = width;
        out->height = height;
        out->color = color;
        out->text = text;
        out->text_color = text_color;
        out->text_font = text_font;
        out->text_height = text_height;
        out->text_width = text_width;
        out->ui_state = ui_state;
        out->name = name;
        out->m_transformName = m_transformName;

        return out;
      }



      bool CInterfaceQuad::InitDependencies( Engine::Entity * parent )
      {
        if( parent && parent->Has( Engine::ComponentType::UI_ELEMENT ) )
        {
          interface_element = static_cast<UI::CBasicElement*>(parent->GetFirstComponent( Engine::ComponentType::UI_ELEMENT ) );
        }

        return CBaseInterfaceGraphic::InitDependencies( parent );
      }

      CInterfaceQuad::CInterfaceQuad()
        : texture(UI_STATE::LAST)
      {
        frag = new CgShader;
        cg_grad_top = new ShaderParameter;
        cg_grad_bottom = new ShaderParameter;
        width = 256;
        height = 64;
        text = "";
        text_font = 0;
        text_height = 32;
        text_width = 0;
        color = text_color = SF::Color(255,255,255,255);
      }

      CInterfaceQuad::~CInterfaceQuad(void)
      {
      }

      void CInterfaceQuad::Draw()
      {
        Enable2D();

        Math::Vector3 pos = transform->GetWorldPosition();
        if( BindTexture() )
        {
          PushMatrix();

          glTranslatef(pos.x, pos.y, z_order / 100.0f);
          if( interface_element )
          {
            glScalef(width,height,1);
            //glScalef(interface_element->GetWidth(), interface_element->GetHeight(),1);
            glColor4fv(interface_element->GetColor().Array());
          }
          else
          {
            glScalef(width,height,1);
            glColor4fv(color.Array());
          }

          glBegin( GL_QUADS );
          glTexCoord2f( 0, 0 );
          glVertex3f( -.5f, -.5f, 0 );
          glTexCoord2f( 1, 0 );
          glVertex3f( .5f, -.5f, 0 );
          glTexCoord2f( 1, 1 );
          glVertex3f( .5f, .5f, 0 );
          glTexCoord2f( 0, 1 );
          glVertex3f( -.5f, .5f, 0 );
          glEnd();

          UnbindTexture();
          PopMatrix();
        }

        if( !text.empty() )
        {
          Color4f tmpColor = text_color;

          if(interface_element)
            tmpColor = interface_element->GetColor();

          static TextEngine * text_engine = TextEngine::Instance();

          Performance::StateManager::SetActiveFragmentShader(frag.Get());
          if( cg_grad_top->param )
            cgSetParameter4f( cg_grad_top->param, text_grad_top.r, text_grad_top.g, text_grad_top.b, text_grad_top.a );
          
          if( cg_grad_bottom->param )
            cgSetParameter4f( cg_grad_bottom->param, text_grad_bottom.r, text_grad_bottom.g, text_grad_bottom.b, text_grad_bottom.a );
          
          text_engine->PrintTextAdv( pos.x - text_width/2.0f, pos.y - text_height/2.0f, text, tmpColor, text_height, text_font );
          Performance::StateManager::SetActiveFragmentShader(0);

        }

        Disable2D();


      }

      bool CInterfaceQuad::BindTexture()
      {
        if( texture[GetState()] )
        {
          texture[GetState()]->Bind(0);
          return true;
        }
        else
          return false;
      }

      void CInterfaceQuad::UnbindTexture()
      {
        if( texture[GetState()] )
          texture[GetState()]->Unbind();
      }

      void CInterfaceQuad::SetTexture( int state, SharedTexture _texture )
      {
        if( state < 0 || state >= UI_STATE::LAST )
          return;

        if(_texture)
        {
          _texture->Bind(0);
          _texture->Unbind();
        }

        texture[state] = _texture;
      }

      void CInterfaceQuad::SetTexture( int state, const std::string & _texture_name )
      {
        static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
        SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( _texture_name );
        if( asset )
          SetTexture( state, asset->image);
        else
        {
          Globals::console->Warning("Invalid Texture: \"" + _texture_name + "\" is not loaded.");
        }
      }

      void CInterfaceQuad::SetFragmentShader( SharedShader2 shader )
      {
        frag = shader;
        cg_grad_top     = frag->GetParameter("grad_top");
        cg_grad_bottom  = frag->GetParameter("grad_bottom");
      }

      void CInterfaceQuad::SetFragmentShader( const std::string & filename )
      {
        frag = Globals::shaderLoader->GetFragmentShader(filename);
        cg_grad_top     = frag->GetParameter("grad_top");
        cg_grad_bottom  = frag->GetParameter("grad_bottom");
      }

      std::string& CInterfaceQuad::GetText()
      {
        return text;
      }

      const std::string& CInterfaceQuad::GetText() const
      {
        return text;
      }

      void CInterfaceQuad::SetText( const std::string & _text )
      {
        static TextEngine * text_engine = TextEngine::Instance();
        text = _text;
        UpdateTextWidth();
      }

      void CInterfaceQuad::SetTextColor( const Color4f & _text_color )
      {
        text_grad_top = text_grad_bottom = _text_color;

        if( interface_element )
        {
          text_grad_top.a = text_grad_bottom.a = interface_element->GetColor().a;
        }
        else
        {
          text_grad_top.a = text_grad_bottom.a = color.a;
        }
      }

      void CInterfaceQuad::SetTextColor( const Color4f & A, const Color4f & B )
      {
        text_grad_top = A;
        text_grad_bottom = B;

        if( interface_element )
        {
          text_grad_top.a = interface_element->GetColor().a;
          text_grad_bottom.a = text_grad_top.a;
        }
        else
        {
          text_grad_top.a = color.a;
          text_grad_bottom.a = color.a;
        }
      }

      void CInterfaceQuad::SetTextSize( float _text_size )
      {
        static TextEngine * text_engine = TextEngine::Instance();
        text_height = _text_size;
        UpdateTextWidth();
      }

      void CInterfaceQuad::SetFont( const std::string & font_name )
      {
        text_font = TextEngine::Instance()->GetFontID(font_name);
        UpdateTextWidth();
      }

      void CInterfaceQuad::SetWidth(float w)
      {
        width = w;
      }

      void CInterfaceQuad::SetHeight(float h)
      {
        height = h;
      }

      void CInterfaceQuad::SetColor(const Color4f & c)
      {
        color = c;
      }

      void CInterfaceQuad::SetTransparency(float percent)
      {
        color.a =  percent;
      }

      float CInterfaceQuad::GetWidth() const
      {
        if( interface_element )
          return interface_element->GetWidth();
        else
          return width;
      }

      float CInterfaceQuad::GetHeight() const
      {
        if( interface_element )
          return interface_element->GetHeight();
        else
          return height;
      }

      Color CInterfaceQuad::GetColor() const
      {
        if( interface_element )
        {
          return interface_element->GetColor();
        }
        else
          return color;
      }

      float CInterfaceQuad::GetTransparency() const
      {
        Color c = GetColor();
        return c.a/255.0f;
      }

      void CInterfaceQuad::UpdateTextWidth()
      {
        static TextEngine * text_engine = TextEngine::Instance();
        text_width = text_engine->GetWidth( text, text_height, text_font );
      }

      void CInterfaceQuad::PushToLua( lua_State *ls )
      {
        Lunar<CInterfaceQuad>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CInterfaceQuad )
        LUA_NAME_METHOD( CInterfaceQuad, "SetTexture", LSetTexture )

        LUA_NAME_METHOD( CInterfaceQuad, "SetText", LSetText )
        LUA_NAME_METHOD( CInterfaceQuad, "SetFont", LSetFont )
        LUA_NAME_METHOD( CInterfaceQuad, "SetTextSize", LSetTextSize )
        LUA_NAME_METHOD( CInterfaceQuad, "SetTextColor", LSetTextColor )

        LUA_NAME_METHOD( CInterfaceQuad, "SetWidth", LSetWidth )
        LUA_NAME_METHOD( CInterfaceQuad, "SetHeight", LSetHeight )
        LUA_NAME_METHOD( CInterfaceQuad, "SetColor", LSetColor )
        LUA_NAME_METHOD( CInterfaceQuad, "SetTransparency", LSetTransparency )
        LUA_NAME_METHOD( CInterfaceQuad, "GetWidth", LGetWidth )
        LUA_NAME_METHOD( CInterfaceQuad, "GetHeight", LGetHeight )
        LUA_NAME_METHOD( CInterfaceQuad, "GetWidth", LGetColor )
        LUA_NAME_METHOD( CInterfaceQuad, "GetTransparency", LGetTransparency )

        LUA_NAME_METHOD( CInterfaceQuad, "GetZOrder", LGetZOrder )
        LUA_NAME_METHOD( CInterfaceQuad, "SetZOrder", LSetZOrder )
        LUA_NAME_METHOD( CInterfaceQuad, "GetState", LGetState )
        LUA_NAME_METHOD( CInterfaceQuad, "SetState", LSetState )

        LUA_NAME_METHOD( CInterfaceQuad, "Base", LBase )
      LUA_END_DEFINE_CLASS

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetWidth )
      {
        float w = luaL_checkfloat(ls, 1);
        SetWidth(w);
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetHeight )
      {
        float h = luaL_checkfloat(ls, 1);
        SetHeight(h);
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetColor )
      {
        Color4i col;

        int top = lua_gettop(ls);
        switch( top )
        {
        case 4:
          col.a = luaL_checkint(ls, 4);
        case 3:
          col.b = luaL_checkint(ls, 3);
        case 2:
          col.g = luaL_checkint(ls, 2);
        case 1:
          col.r = luaL_checkint(ls, 1);
        default:
          SetColor( col );
        }

        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetTransparency )
      {
        float t = luaL_checkfloat(ls, 1);
        SetTransparency(t);
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LGetWidth )
      {
        lua_pushnumber( ls, GetWidth() );
        return 1;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LGetHeight )
      {
        lua_pushnumber( ls, GetHeight() );
        return 1;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LGetColor )
      {
        Color c = GetColor();
        lua_pushnumber( ls, c.r/255.0f );
        lua_pushnumber( ls, c.g/255.0f );
        lua_pushnumber( ls, c.b/255.0f );
        lua_pushnumber( ls, c.a/255.0f );
        return 4;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LGetTransparency )
      {
        lua_pushnumber( ls, GetTransparency() );
        return 1;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetText )
      {
        const char * new_text = luaL_checkstring( ls, 1 );
        if( new_text )
          SetText( std::string(new_text) );

        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetFont )
      {
        const char * font_name = luaL_checkstring( ls, 1 );
        if( font_name )
          SetFont( std::string(font_name) );
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetTextSize )
      {
        float size = luaL_checkfloat(ls, 1);
        SetTextSize(size);
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetTextColor )
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
          SetTextColor( A, B );

        else
          SetTextColor( A );

        return 0;
      }


      LUA_DEFINE_METHOD( CInterfaceQuad, LSetTexture )
      {
        const char * options[] = { "DISABLED", "UP", "OVER", "DOWN", 0 };
        int o = luaL_checkoption(ls, 1, 0, options );

        const char * tex_name = luaL_checkstring( ls, 2 );

        if( tex_name )
          SetTexture(o, tex_name);

        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LGetZOrder )
      {
        CBaseInterfaceGraphic::LGetZOrder(ls);
        return 1;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetZOrder )
      {
        CBaseInterfaceGraphic::LSetZOrder(ls);
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LGetState )
      {
        CBaseInterfaceGraphic::LGetState(ls);
        return 1;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LSetState )
      {
        CBaseInterfaceGraphic::LSetState(ls);
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfaceQuad, LBase )
      {
        CBaseInterfaceGraphic::PushToLua(ls);
        return 1;
      }


    }
  }
}
