/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CSkybox.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:29:05 CTallman
**  \brief   
*******************************************************************************/

#include "CSkybox.h"
#include "Utilities/Noise/CubeGenerator.h"
#include "External/ticpp/include/ticpp.h"

#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"
#include "Utilities/DevConsole.h"
#include "StateManager.h"
#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {

      Engine::BaseComponent * CreateSkyboxComponent( const ticpp::Element * element )
      {
        if( element )
        {
          

          CSkybox * out = new CSkybox();
          try
          {

            ticpp::Element * child = element->FirstChildElement( "texture", false );
            if( child )
            {
              std::string texture = child->GetAttribute("name");
              if( !texture.empty() )
              {
                out->SetTexture(SKYBOX_SIDE::LEFT, texture);
                out->SetTexture(SKYBOX_SIDE::RIGHT, texture);
                out->SetTexture(SKYBOX_SIDE::TOP, texture);
                out->SetTexture(SKYBOX_SIDE::BOTTOM, texture);
                out->SetTexture(SKYBOX_SIDE::FRONT, texture);
                out->SetTexture(SKYBOX_SIDE::BACK, texture);
              }
              else
              {
                std::string left = child->GetAttribute("left");
                if( !left.empty() )
                  out->SetTexture( SKYBOX_SIDE::LEFT, left );

                std::string right = child->GetAttribute("right");
                if( !right.empty() )
                  out->SetTexture( SKYBOX_SIDE::RIGHT, right );

                std::string top = child->GetAttribute("top");
                if( !top.empty() )
                  out->SetTexture( SKYBOX_SIDE::TOP, top );

                std::string bottom = child->GetAttribute("bottom");
                if( !bottom.empty() )
                  out->SetTexture( SKYBOX_SIDE::BOTTOM, bottom );

                std::string front = child->GetAttribute("front");
                if( !front.empty() )
                  out->SetTexture( SKYBOX_SIDE::FRONT, front );

                std::string back = child->GetAttribute("back");
                if( !back.empty() )
                  out->SetTexture( SKYBOX_SIDE::BACK, back );
              }

            }

            child = element->FirstChildElement( "shader", false );
            if( child )
            {
              std::string shader_name = child->GetAttribute("fragment");
              if( !shader_name.empty() )
              {
                out->SetFragmentShader( shader_name );
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

      CSkybox::CSkybox()
        : BaseComponent(Engine::ComponentType::SKYBOX)
      {
        left = new TextureType::Texture2D();
        right = new TextureType::Texture2D();
        top = new TextureType::Texture2D();
        bottom = new TextureType::Texture2D();
        front = new TextureType::Texture2D();
        back = new TextureType::Texture2D();

        brightness = 1.2f;

        frag = new CgShader();
        cg_brightness = new ShaderParameter();
      }

      CSkybox::~CSkybox(void)
      {
      }

      Engine::BaseComponent* CSkybox::Clone() const
      {
        CSkybox * out = new CSkybox();
        
        out->SetTexture( SKYBOX_SIDE::LEFT, left );
        out->SetTexture( SKYBOX_SIDE::RIGHT, right );
        out->SetTexture( SKYBOX_SIDE::TOP, top );
        out->SetTexture( SKYBOX_SIDE::BOTTOM, bottom );
        out->SetTexture( SKYBOX_SIDE::FRONT, front );
        out->SetTexture( SKYBOX_SIDE::BACK, back );

        out->SetFragmentShader(frag);

        return out;
      }

      bool CSkybox::InitDependencies( Engine::Entity* )
      {
        return true;
      }

      void CSkybox::PushToLua(lua_State *ls)
      {
        Lunar<CSkybox>::push(ls, this);
      }

      void CSkybox::SetFragmentShader( SharedShader2 shader )
      {
        frag = shader;
        cg_brightness = frag->GetParameter("brightness");
      }

      void CSkybox::SetFragmentShader( const std::string & shader )
      {
        frag = Globals::shaderLoader->GetFragmentShader(shader);
        cg_brightness = frag->GetParameter("brightness");
      }


      void CSkybox::SetTexture( SKYBOX_SIDE::SIDE side, SF::Graphics::SharedTexture _texture)
      {
        switch( side )
        {
        case SKYBOX_SIDE::LEFT:
          left = _texture;
          break;
        case SKYBOX_SIDE::RIGHT:
          right = _texture;
          break;
        case SKYBOX_SIDE::TOP:
          top = _texture;
          break;
        case SKYBOX_SIDE::BOTTOM:
          bottom = _texture;
          break;
        case SKYBOX_SIDE::FRONT:
          front = _texture;
          break;
        case SKYBOX_SIDE::BACK:
          back = _texture;
          break;
        }
      }

      void CSkybox::SetTexture(  SKYBOX_SIDE::SIDE side, const std::string & _texture_name )
      {

        static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
        SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( _texture_name );
        if( asset )
          SetTexture(side, asset->image);
        else
        {
          Globals::console->Warning("Invalid Texture: \"" + _texture_name + "\" is not loaded.");
        }

      }

      void CSkybox::EnableShader()
      {
        Performance::StateManager::SetActiveFragmentShader(frag.Get());
      }

      void CSkybox::DisableShader()
      {
        Performance::StateManager::SetActiveFragmentShader(0);        
      }

      void CSkybox::Draw()
      {
        if( cg_brightness->param )
          cgSetParameter1f( cg_brightness->param, brightness );

        glMatrixMode( GL_MODELVIEW );
        PushMatrix();

        float view[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, view );

        view[12] = 0.0f;
        view[13] = 0.0f;
        view[14] = 0.0f;
        view[15] = 1.0f;
        glLoadMatrixf( view );

        EnableShader();
        DrawBox();
        DisableShader();

        PopMatrix();
      }

      void CSkybox::DrawBox()
      { 
        glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA );
        glDisable( GL_DEPTH_TEST );

        float width = 100.0f;
        float height = width + 0.005f;

        left->Bind();
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); 
        glVertex3f(-width, -height, -height);
        glTexCoord2f(0,  1); 
        glVertex3f(-width, height, -height);
        glTexCoord2f(1,  1); 
        glVertex3f(-width, height, height);
        glTexCoord2f(1, 0); 
        glVertex3f(-width, -height, height);
        glEnd();

        right->Bind();
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); 
        glVertex3f(width, -height, height);
        glTexCoord2f(0,  1); 
        glVertex3f(width, height, height);
        glTexCoord2f(1,  1); 
        glVertex3f(width, height, -height);
        glTexCoord2f(1, 0); 
        glVertex3f(width, -height, -height);
        glEnd();

        top->Bind();
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-height, width, height);
        glTexCoord2f(0,  1);
        glVertex3f(-height, width, -height);
        glTexCoord2f(1,  1);
        glVertex3f(height, width, -height);
        glTexCoord2f(1, 0);
        glVertex3f(height, width, height);
        glEnd();

        bottom->Bind();
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-height, -width, -height);
        glTexCoord2f(0,  1);
        glVertex3f(-height, -width, height);
        glTexCoord2f(1,  1);
        glVertex3f(height, -width, height);
        glTexCoord2f(1, 0);
        glVertex3f(height, -width, -height);
        glEnd();

        front->Bind();
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-height, -height, width);
        glTexCoord2f(0,  1);
        glVertex3f(-height, height, width);
        glTexCoord2f(1,  1);
        glVertex3f(height, height, width);
        glTexCoord2f(1, 0);
        glVertex3f(height, -height, width);
        glEnd();

        back->Bind();
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(height, -height, -width);
        glTexCoord2f(0,  1);
        glVertex3f(height, height, -width);
        glTexCoord2f(1,  1); 
        glVertex3f(-height, height, -width);
        glTexCoord2f(1, 0);
        glVertex3f(-height, -height, -width);
        glEnd();
        back->Unbind();

        glPopAttrib();
      }

      LUA_START_DEFINE_CLASS( CSkybox )

      LUA_NAME_METHOD( CSkybox, "SetBrightness", LSetBrightness )

      LUA_END_DEFINE_CLASS

      LUA_DEFINE_METHOD( CSkybox, LSetBrightness )
      {
        brightness = luaL_checkfloat(ls, 1);
        return 0;
      }
    }
  }
}
