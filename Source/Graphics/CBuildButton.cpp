/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CBuildButton.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#include "Engine/Entity.h"
#include "CBuildButton.h"
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

      Engine::BaseComponent * CBuildButton::CreateComponent( const ticpp::Element * element )
      {
        if( element )
        {
          CBuildButton * out = new CBuildButton();
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


      Engine::BaseComponent * CBuildButton::Clone() const
      {
        CBuildButton * out = new CBuildButton();

        for( int i = 0; i < UI_STATE::LAST; ++i )
          out->SetTexture( i, texture[i] );

        out->SetFragmentShader(frag);
        out->width = width;
        out->height = height;
        out->color = color;
        out->ui_state = ui_state;
        out->name = name;
        out->resource_percent = resource_percent;
        out->m_transformName = m_transformName;

        return out;
      }



      bool CBuildButton::InitDependencies( Engine::Entity * parent )
      {
        if( parent && parent->Has( Engine::ComponentType::UI_ELEMENT ) )
        {
          interface_element = static_cast<UI::CBasicElement*>(parent->GetFirstComponent( Engine::ComponentType::UI_ELEMENT ) );
        }

        return CBaseInterfaceGraphic::InitDependencies( parent );
      }

      CBuildButton::CBuildButton()
        : texture(UI_STATE::LAST)
      {
        frag = new CgShader;
        cg_resource_percent = new ShaderParameter;
        width = 64;
        height = 64;
        resource_percent = 1.0f;
        color = SF::Color(255,255,255,255);
      }

      CBuildButton::~CBuildButton(void)
      {
      }

      void CBuildButton::Draw()
      {
        Enable2D();

        Performance::StateManager::SetActiveFragmentShader( frag.Get() );

        if( cg_resource_percent->param )
            cgSetParameter1f( cg_resource_percent->param, resource_percent );
        
        if( BindTexture() )
        {
          PushMatrix();

          Math::Vector3 pos = transform->GetWorldPosition();
          glTranslatef(pos.x, pos.y, z_order / 100.0f);
          if( interface_element )
          {
            glScalef(width,height,1);
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

        Performance::StateManager::SetActiveFragmentShader( 0 );

        Disable2D();
      }

      bool CBuildButton::BindTexture()
      {
        if( texture[GetState()] )
        {
          texture[GetState()]->Bind(0);
          return true;
        }
        else
          return false;
      }

      void CBuildButton::UnbindTexture()
      {
        if( texture[GetState()] )
          texture[GetState()]->Unbind();
      }

      void CBuildButton::SetTexture( int state, SharedTexture _texture )
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

      void CBuildButton::SetTexture( int state, const std::string & _texture_name )
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

      void CBuildButton::SetFragmentShader( SharedShader2 shader )
      {
        frag = shader;
        cg_resource_percent     = frag->GetParameter("resource_percent");
      }

      void CBuildButton::SetFragmentShader( const std::string & filename )
      {
        frag = Globals::shaderLoader->GetFragmentShader(filename);
        cg_resource_percent     = frag->GetParameter("resource_percent");
      }

      void CBuildButton::SetWidth(float w)
      {
        width = w;
      }

      void CBuildButton::SetHeight(float h)
      {
        height = h;
      }

      void CBuildButton::SetColor(const Color4f & c)
      {
        color = c;
      }

      void CBuildButton::SetTransparency(float percent)
      {
        color.a = (unsigned char)(percent * 255);
      }

      float CBuildButton::GetWidth() const
      {
        if( interface_element )
          return interface_element->GetWidth();
        else
          return width;
      }

      float CBuildButton::GetHeight() const
      {
        if( interface_element )
          return interface_element->GetHeight();
        else
          return height;
      }

      Color CBuildButton::GetColor() const
      {
        if( interface_element )
        {
          return interface_element->GetColor();
        }
        else
          return color;
      }

      float CBuildButton::GetTransparency() const
      {
        Color c = GetColor();
        return c.a/255.0f;
      }

      void CBuildButton::PushToLua( lua_State *ls )
      {
        Lunar<CBuildButton>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CBuildButton )
        LUA_NAME_METHOD( CBuildButton, "SetTexture", LSetTexture )

        LUA_NAME_METHOD( CBuildButton, "SetResourcePercent", LSetResourcePercent )

        LUA_NAME_METHOD( CBuildButton, "SetWidth", LSetWidth )
        LUA_NAME_METHOD( CBuildButton, "SetHeight", LSetHeight )
        LUA_NAME_METHOD( CBuildButton, "SetColor", LSetColor )
        LUA_NAME_METHOD( CBuildButton, "SetTransparency", LSetTransparency )
        LUA_NAME_METHOD( CBuildButton, "GetWidth", LGetWidth )
        LUA_NAME_METHOD( CBuildButton, "GetHeight", LGetHeight )
        LUA_NAME_METHOD( CBuildButton, "GetWidth", LGetColor )
        LUA_NAME_METHOD( CBuildButton, "GetTransparency", LGetTransparency )

        LUA_NAME_METHOD( CBuildButton, "GetZOrder", LGetZOrder )
        LUA_NAME_METHOD( CBuildButton, "SetZOrder", LSetZOrder )
        LUA_NAME_METHOD( CBuildButton, "GetState", LGetState )
        LUA_NAME_METHOD( CBuildButton, "SetState", LSetState )

        LUA_NAME_METHOD( CBuildButton, "Base", LBase )
      LUA_END_DEFINE_CLASS

      LUA_DEFINE_METHOD( CBuildButton, LSetResourcePercent )
      {
        float p = luaL_checkfloat(ls, 1);
        resource_percent = Utilities::Clamp<float>(p,0.0f,1.0f);
        return 0;
      }


      LUA_DEFINE_METHOD( CBuildButton, LSetWidth )
      {
        float w = luaL_checkfloat(ls, 1);
        SetWidth(w);
        return 0;
      }

      LUA_DEFINE_METHOD( CBuildButton, LSetHeight )
      {
        float h = luaL_checkfloat(ls, 1);
        SetHeight(h);
        return 0;
      }

      LUA_DEFINE_METHOD( CBuildButton, LSetColor )
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

      LUA_DEFINE_METHOD( CBuildButton, LSetTransparency )
      {
        float t = luaL_checkfloat(ls, 1);
        SetTransparency(t);
        return 0;
      }

      LUA_DEFINE_METHOD( CBuildButton, LGetWidth )
      {
        lua_pushnumber( ls, GetWidth() );
        return 1;
      }

      LUA_DEFINE_METHOD( CBuildButton, LGetHeight )
      {
        lua_pushnumber( ls, GetHeight() );
        return 1;
      }

      LUA_DEFINE_METHOD( CBuildButton, LGetColor )
      {
        Color c = GetColor();
        lua_pushnumber( ls, c.r/255.0f );
        lua_pushnumber( ls, c.g/255.0f );
        lua_pushnumber( ls, c.b/255.0f );
        lua_pushnumber( ls, c.a/255.0f );
        return 4;
      }

      LUA_DEFINE_METHOD( CBuildButton, LGetTransparency )
      {
        lua_pushnumber( ls, GetTransparency() );
        return 1;
      }

      LUA_DEFINE_METHOD( CBuildButton, LSetTexture )
      {
        const char * options[] = { "DISABLED", "UP", "OVER", "DOWN", 0 };
        int o = luaL_checkoption(ls, 1, 0, options );

        const char * tex_name = luaL_checkstring( ls, 2 );

        if( tex_name )
          SetTexture(o, tex_name);

        return 0;
      }

      LUA_DEFINE_METHOD( CBuildButton, LGetZOrder )
      {
        return CBaseInterfaceGraphic::LGetZOrder(ls);
      }

      LUA_DEFINE_METHOD( CBuildButton, LSetZOrder )
      {
        return CBaseInterfaceGraphic::LSetZOrder(ls);
      }

      LUA_DEFINE_METHOD( CBuildButton, LGetState )
      {
        return CBaseInterfaceGraphic::LGetState(ls);
      }

      LUA_DEFINE_METHOD( CBuildButton, LSetState )
      {
        return CBaseInterfaceGraphic::LSetState(ls);
      }

      LUA_DEFINE_METHOD( CBuildButton, LBase )
      {
        CBaseInterfaceGraphic::PushToLua(ls);
        return 1;
      }


    }
  }
}
