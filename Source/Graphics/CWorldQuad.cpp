/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CWorldQuad.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 17:40:58 - nwilliam
**  \brief   World quad graphics component imlementation.
*******************************************************************************/

#include "OpenGLInclude.h"
#include "CWorldQuad.h"

#include "Engine/Entity.h"
#include "Math/Constants.h"
#include "SolarFlare/Globals.h"
#include "ShaderLoader.h"
#include "Engine/AssetManager.h"
#include "Utilities/DevConsole.h"
#include "StateManager.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {

      Engine::BaseComponent * CWorldQuad::CreateComponent( const ticpp::Element * element )
      {

        CWorldQuad * out = new CWorldQuad();

        try
        {
          ticpp::Element * child = 0;
          child = element->FirstChildElement( "texture", false );
          if( child )
          {
            std::string texture_name = child->GetAttribute("name");
            if( !texture_name.empty() )
              out->SetTexture( texture_name );
          }

          child = element->FirstChildElement( "shader", false );
          if( child )
          {
            std::string shader_name = child->GetAttribute("vertex");
            if( !shader_name.empty() )
            {
              out->SetVertexShader( shader_name.c_str() );
            }

            shader_name = child->GetAttribute("fragment");
            if( !shader_name.empty() )
            {
              out->SetFragmentShader( shader_name.c_str() );
            }
          }

        }
        catch ( ticpp::Exception & e )
        {
          Globals::console->Error( e.what() );
          delete out;
        }

        return out;
      }

      Engine::BaseComponent * CWorldQuad::Clone() const
      {
        CWorldQuad * out = new CWorldQuad();
        out->color = color;
        out->scale = scale;
        out->texture = texture;
        out->vert = vert;
        out->frag = frag;
        return out;
      }

      bool CWorldQuad::InitDependencies( Engine::Entity * parent )
      {
        if( parent && parent->Has( Engine::ComponentType::TRANSFORM ) )
        {
          transform = static_cast<Math::CTransform*>(parent->GetFirstComponent( Engine::ComponentType::TRANSFORM ));
          return true;
        }
        else
          return false;
      }


      CWorldQuad::CWorldQuad()
        : BaseComponent( Engine::ComponentType::WORLD_QUAD )
      {
        vert = new CgShader();
        frag = new CgShader();
        transform = 0;
        scale = Math::Vector2(200.0f,200.0f);
        color = Color(255,255,255,255);
        texture = new TextureType::Texture2D();
      }

      CWorldQuad::~CWorldQuad()
      {
      }

      void CWorldQuad::SetScale(float x_scale, float y_scale)
      {
        scale = Math::Vector2(x_scale,y_scale);
      }

      void CWorldQuad::SetScale(float uniform_scale)
      {
        scale = Math::Vector2( uniform_scale, uniform_scale );
      }

      void CWorldQuad::SetColor( Color _color )
      {
        color = _color;
      }

      void CWorldQuad::SetVertexShader( const char * str )
      {
        vert = Globals::shaderLoader->GetVertexShader(std::string(str));        
      }

      void CWorldQuad::SetFragmentShader( const char * str )
      {
        frag = Globals::shaderLoader->GetFragmentShader(std::string(str));
      }

      void CWorldQuad::SetTexture( const std::string & _texture_name )
      {
        static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
        SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( _texture_name );
        if( asset )
          texture = asset->image;
        else
        {
          Globals::console->Warning("Invalid Texture: \"" + _texture_name + "\" is not loaded.");
        }
      }

      void CWorldQuad::Draw()
      {
        glMatrixMode( GL_MODELVIEW );
        PushMatrix();

        if( transform )
        {
          float rotAngle;
          Math::Vector3 rotAxis;
          Math::Vector3 pos = transform->GetWorldPosition();
          Math::Quaternion rot = transform->GetWorldOrientation();
          rot.AxisAngle(rotAxis, rotAngle);
          glTranslatef( pos.x, pos.y, pos.z );
          glRotatef(
            Math::Constants::RadToDeg * rotAngle,
            rotAxis.x,
            rotAxis.y,
            rotAxis.z);
        }

        glScalef(scale.x, 1.0f, scale.y);

        BindTexture();

        Performance::StateManager::SetActiveFragmentShader(frag.Get());
        Performance::StateManager::SetActiveVertexShader(vert.Get());

        glBegin( GL_QUADS );

        glColor4ub( color.r, color.g, color.b, color.a );
        glTexCoord2f( 0.0f, 1.0f );
        glVertex3f( -0.5f, 0.0f, -0.5f );
        glTexCoord2f( 1.0f, 1.0f );
        glVertex3f( 0.5f,  0.0f, -0.5f );
        glTexCoord2f( 1.0f, 0.0f );
        glVertex3f( 0.5f,  0.0f,  0.5f );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex3f( -0.5f, 0.0f,  0.5f );

        glEnd();

        Performance::StateManager::SetActiveFragmentShader(0);
        Performance::StateManager::SetActiveVertexShader(0);

        UnbindTexture();
        PopMatrix();
      }

      void CWorldQuad::BindTexture() const
      {
        texture->Bind(0);
      }

      void CWorldQuad::UnbindTexture() const
      {
        texture->Unbind();
      }

      void CWorldQuad::PushToLua( lua_State *ls )
      {
        Lunar<CWorldQuad>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CWorldQuad )
        LUA_NAME_METHOD( CWorldQuad, "SetTexture", LSetTexture )
        LUA_NAME_METHOD( CWorldQuad, "SetColor", LSetColor )
        LUA_NAME_METHOD( CWorldQuad, "SetTransparency", LSetTransparency )
        LUA_NAME_METHOD( CWorldQuad, "SetScale", LSetScale )
      LUA_END_DEFINE_CLASS

      LUA_DEFINE_METHOD( CWorldQuad, LSetTexture )
      {
        const char * tex_name = luaL_checkstring( ls, 1 );

        if( tex_name )
          SetTexture(tex_name);

        return 0;
      }

      LUA_DEFINE_METHOD( CWorldQuad, LSetColor )
      {
        int r = 0;
        int g = 0;
        int b = 0;
        int a = 255;

        int top = lua_gettop(ls);
        switch( top )
        {
        case 4:
          a = luaL_checkint(ls, 4);
        case 3:
          b = luaL_checkint(ls, 3);
        case 2:
          g = luaL_checkint(ls, 2);
        case 1:
          r = luaL_checkint(ls, 1);
        default:
          SetColor( Color((unsigned char)r,(unsigned char)g,(unsigned char)b,(unsigned char)a) );
        }

        return 0;
      }

      LUA_DEFINE_METHOD( CWorldQuad, LSetTransparency )
      {
        float percent = luaL_checkfloat(ls, 1);
        color.a = (unsigned char)(percent * 255);
        return 0;
      }

      LUA_DEFINE_METHOD( CWorldQuad, LSetScale )
      {
        float x = 1.0f;
        float y = 1.0f;

        int top = lua_gettop(ls);
        switch( top )
        {
        case 2:
          y = luaL_checkfloat(ls, 2);
          x = luaL_checkfloat(ls, 1);
          break;
        case 1:
          y = x = luaL_checkfloat(ls, 1);
          break;
        }

        SetScale( x, y );

        return 0;
      }


    }
  }
}
