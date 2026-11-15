/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CInterfacePanel.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 17:37:49 - nwilliam
**  \brief   Interface panel implementation.
*******************************************************************************/

#include "Engine/Entity.h"
#include "Math/TVector3.h"
#include "CInterfacePanel.h"
#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {
      Engine::BaseComponent * CInterfacePanel::CreateComponent( const ticpp::Element * element )
      {
        if( element )
        {
          CInterfacePanel * out = new CInterfacePanel();
          try
          {
            element->GetAttributeOrDefault("transform", &out->m_transformName, std::string());

            ticpp::Element * child = element->FirstChildElement( "texture", false );
            if( child )
            {
              static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
              std::string texture_name = child->GetAttribute("name");

              if( !texture_name.empty() )
              {
                SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( texture_name );
                if( asset )
                  out->SetTexture( asset->image );
              }
            }


            return out;
          }

          catch( ticpp::Exception & e )
          {
            Globals::console->Error( e.what() );
            delete out;
            return 0;
          }
        }
        else
          return 0;
      }


      Engine::BaseComponent * CInterfacePanel::Clone() const
      {
        CInterfacePanel * out = new CInterfacePanel();

        out->SetTexture( texture );
        out->name = name;
        out->m_transformName = m_transformName;

        return out;
      }

      bool CInterfacePanel::InitDependencies( Engine::Entity * parent )
      {
        if( parent && !parent->Has( Engine::ComponentType::UI_ELEMENT ) )
          return false;

        interface_element = static_cast<UI::CBasicElement*>(parent->GetFirstComponent( Engine::ComponentType::UI_ELEMENT ) );

        return CBaseInterfaceGraphic::InitDependencies( parent );
      }


      CInterfacePanel::CInterfacePanel()
      {
      }

      CInterfacePanel::~CInterfacePanel()
      {
      }

      void CInterfacePanel::SetTexture( SharedTexture _texture )
      {
        texture = _texture;
        texture->UseMips(false);
      }

      void CInterfacePanel::SetTexture( const std::string & _texture_name )
      {
        static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
        SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( _texture_name );
        if( asset )
          SetTexture( asset->image );
        else
        {
          Globals::console->Warning("Invalid Texture: \"" + _texture_name + "\" is not loaded.");
        }
      }


      void CInterfacePanel::Draw()
      {
 
        Enable2D();

        float divisions = 12.0f;
        const float u0 = 0.0f;
        const float u1 = 1.0f / divisions;
        const float u2 = 1.0f - 1.0f / divisions;
        const float u3 = 1.0f;
        const float v0 = u3;
        const float v1 = u2;
        const float v2 = u1;
        const float v3 = u0;

        const float half_width = interface_element->GetWidth() / 2;
        const float half_height = interface_element->GetHeight() / 2;
        const float corner = 10.0f;

        PushMatrix();

        if( transform )
        {
          Math::Vector3 pos = transform->GetWorldPosition();
          glTranslatef(pos.x, pos.y, z_order / 100.0f);
        }

        BindTexture();

        glBegin(GL_QUADS);
        glColor4fv( interface_element->GetColor().Array() );
        
        //top left
        glTexCoord2f(u0, v0);
        glVertex2f(-half_width,         half_height);
        glTexCoord2f(u1, v0);
        glVertex2f(-half_width+corner,  half_height);
        glTexCoord2f(u1, v1);
        glVertex2f(-half_width+corner,  half_height-corner);
        glTexCoord2f(u0, v1);
        glVertex2f(-half_width,         half_height-corner);
        
        //top
        glTexCoord2f(u1, v0);
        glVertex2f(-half_width+corner,  half_height);
        glTexCoord2f(u2, v0);
        glVertex2f(half_width-corner,   half_height);
        glTexCoord2f(u2, v1); 
        glVertex2f(half_width-corner,   half_height-corner);
        glTexCoord2f(u1, v1);
        glVertex2f(-half_width+corner,  half_height-corner);

        //top right
        glTexCoord2f(u2, v0);
        glVertex2f(half_width-corner,   half_height);
        glTexCoord2f(u3, v0);
        glVertex2f(half_width,          half_height);
        glTexCoord2f(u3, v1);
        glVertex2f(half_width,          half_height-corner);
        glTexCoord2f(u2, v1);
        glVertex2f(half_width-corner,   half_height-corner);

        //left                  
        glTexCoord2f(u0, v1);
        glVertex2f(-half_width,         half_height-corner);
        glTexCoord2f(u1, v1);
        glVertex2f(-half_width+corner,  half_height-corner);
        glTexCoord2f(u1, v2);
        glVertex2f(-half_width+corner,  -half_height+corner);
        glTexCoord2f(u0, v2);
        glVertex2f(-half_width,         -half_height+corner);
        
        //center                
        glTexCoord2f(u1, v1);
        glVertex2f(-half_width+corner,  half_height-corner);
        glTexCoord2f(u2, v1);
        glVertex2f(half_width-corner,   half_height-corner);
        glTexCoord2f(u2, v2); 
        glVertex2f(half_width-corner,   -half_height+corner);
        glTexCoord2f(u1, v2);
        glVertex2f(-half_width+corner,  -half_height+corner);

        //right
        glTexCoord2f(u2, v1);
        glVertex2f(half_width-corner,   half_height-corner);
        glTexCoord2f(u3, v1);
        glVertex2f(half_width,          half_height-corner);
        glTexCoord2f(u3, v2);
        glVertex2f(half_width,          -half_height+corner);
        glTexCoord2f(u2, v2);
        glVertex2f(half_width-corner,   -half_height+corner);

        //bottom left           
        glTexCoord2f(u0, v2);   
        glVertex2f(-half_width,         -half_height+corner);
        glTexCoord2f(u1, v2);   
        glVertex2f(-half_width+corner,  -half_height+corner);
        glTexCoord2f(u1, v3);   
        glVertex2f(-half_width+corner,  -half_height);
        glTexCoord2f(u0, v3); 
        glVertex2f(-half_width,         -half_height);

        //bottom               
        glTexCoord2f(u1, v2);
        glVertex2f(-half_width+corner,  -half_height+corner);
        glTexCoord2f(u2, v2);  
        glVertex2f(half_width-corner,   -half_height+corner);
        glTexCoord2f(u2, v3);  
        glVertex2f(half_width-corner,   -half_height);
        glTexCoord2f(u1, v3);   
        glVertex2f(-half_width+corner,  -half_height);

        //bottom right
        glTexCoord2f(u2, v2);
        glVertex2f(half_width-corner,   -half_height+corner);
        glTexCoord2f(u3, v2);
        glVertex2f(half_width,          -half_height+corner);
        glTexCoord2f(u3, v3);
        glVertex2f(half_width,          -half_height);
        glTexCoord2f(u2, v3); 
        glVertex2f(half_width-corner,   -half_height);

        glEnd();

        UnbindTexture();

        PopMatrix();

        Disable2D();

      }

      void CInterfacePanel::BindTexture()
      {
        texture->Bind();
      }

      void CInterfacePanel::UnbindTexture()
      {
        texture->Unbind();
      }

      void CInterfacePanel::PushToLua( lua_State *ls )
      {
        Lunar<CInterfacePanel>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CInterfacePanel )
        LUA_NAME_METHOD( CInterfacePanel, "SetTexture", LSetTexture )
        LUA_NAME_METHOD( CInterfacePanel, "SetZOrder", LSetZOrder )
        LUA_NAME_METHOD( CInterfacePanel, "GetZOrder", LGetZOrder )
        LUA_NAME_METHOD( CInterfacePanel, "SetState", LSetState )
        LUA_NAME_METHOD( CInterfacePanel, "GetState", LGetState )
        LUA_NAME_METHOD( CInterfacePanel, "Base", LBase )
      LUA_END_DEFINE_CLASS


      LUA_DEFINE_METHOD( CInterfacePanel, LSetTexture )
      {
        const char * new_text = luaL_checkstring( ls, 1 );
        if( new_text )
          SetTexture( std::string(new_text) );
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfacePanel, LGetZOrder )
      {
        lua_pushnumber( ls, GetZOrder() );
        return 1;
      }

      LUA_DEFINE_METHOD( CInterfacePanel, LSetZOrder )
      {
        int z = luaL_checkint(ls, 1);
        SetZOrder(z);
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfacePanel, LGetState )
      {
        lua_pushnumber( ls, GetState() );
        return 1;
      }

      LUA_DEFINE_METHOD( CInterfacePanel, LSetState )
      {
        int z = luaL_checkint(ls, 1);
        SetState(z);
        return 0;
      }

      LUA_DEFINE_METHOD( CInterfacePanel, LBase )
      {
        CBaseInterfaceGraphic::PushToLua(ls);
        return 1;
      }

    }
  }
}