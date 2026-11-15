/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CModel.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:29:56 CTallman
**  \brief   
*******************************************************************************/

#include "CModel.h"
#include "StateManager.h"
#include "Engine/Entity.h"
#include "GraphicsEngine.h"
#include "Math/Constants.h"
#include "SolarFlare/Globals.h"
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
      Engine::BaseComponent * CreateModel( const ticpp::Element * element )
      {
        if( element )
        {
          CModel * out = new CModel();
          try
          {
            ticpp::Element * child = 0;

            child = element->FirstChildElement( "model", false );
            if( child )
            {

              float scale = 1.0f;
              child->GetAttributeOrDefault( "scale", &scale, 1.0f );
              out->SetScale( Math::Vector3(scale,scale,scale) );

              int transparent = 0;
              child->GetAttributeOrDefault( "transparent", &transparent, 0 );
              out->IsOpaque(!transparent);

              std::string model_name = child->GetAttribute("name");
              if( !model_name.empty() )
              {
                SF::Engine::Assets::ModelAsset * base_model = (SF::Engine::Assets::ModelAsset*)Globals::assetManager->GetAsset( model_name );
                if( base_model )
                  out->SetModel( SharedModel(base_model->model) );
              }
              else
              {
                model_name = child->GetAttribute("low");
                if( !model_name.empty() )
                {
                  SF::Engine::Assets::ModelAsset * base_model = (SF::Engine::Assets::ModelAsset*)Globals::assetManager->GetAsset( model_name );
                  if( base_model )
                    out->SetModel( SharedModel(base_model->model), QUALITY::LOW );
                }

                model_name = child->GetAttribute("medium");
                if( !model_name.empty() )
                {
                  SF::Engine::Assets::ModelAsset * base_model = (SF::Engine::Assets::ModelAsset*)Globals::assetManager->GetAsset( model_name );
                  if( base_model )
                    out->SetModel( SharedModel(base_model->model), QUALITY::MEDIUM );
                }

                model_name = child->GetAttribute("high");
                if( !model_name.empty() )
                {
                  SF::Engine::Assets::ModelAsset * base_model = (SF::Engine::Assets::ModelAsset*)Globals::assetManager->GetAsset( model_name );
                  if( base_model )
                    out->SetModel( SharedModel(base_model->model), QUALITY::HIGH );
                }
              }
            }

            child = element->FirstChildElement( "texture", false );
            if( child )
            {
              std::string texture_name = child->GetAttribute("name");
              if( !texture_name.empty() )
              {
                SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
                SF::Engine::Assets::Texture * texture = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( texture_name );
                if( texture )
                  out->SetTexture( texture->image );
              }
            }

            child = element->FirstChildElement( "normalmap", false );
            if( child )
            {
              std::string texture_name = child->GetAttribute("name");
              if( !texture_name.empty() )
              {
                SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
                SF::Engine::Assets::Texture * texture = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( texture_name );
                if( texture )
                  out->SetNormalmap( texture->image );
              }
            }

            child = element->FirstChildElement( "shader", false );
            if( child )
            {
              std::string shader_name = child->GetAttribute("vertex");
              if( !shader_name.empty() )
              {
                out->SetVertexShader( shader_name );
              }

              shader_name = child->GetAttribute("fragment");
              if( !shader_name.empty() )
              {
                out->SetFragmentShader( shader_name );
              }
            }


          }
          catch( ticpp::Exception & e )
          {
            Globals::console->Error(e.what());
            delete out;
            return 0;
          }

          return out;
        }

        return 0;
      }

      Engine::BaseComponent * CModel::Clone() const
      {
        CModel * out = new CModel();
        
        
        
        for( unsigned int i = 0; i < model.size(); ++i )
          out->SetModel( model[i], i );
        
        out->LOD = LOD;
        out->HasLOD(HasLOD());
        out->SetTexture( diffuse_texture );
        out->SetNormalmap( normal_texture );
        out->SetScale( scale );
        out->SetGlowAmount( glow_amount );
        out->SetFragmentShader( frag );
        out->SetVertexShader( vert );
        out->IsOpaque(opaque);

        return out;
      }  



      bool CModel::InitDependencies( Engine::Entity * parent )
      {
        bool succeed = true;
        if( parent && parent->Has( Engine::ComponentType::TRANSFORM ) )
          transform = static_cast<Math::CTransform*>(parent->GetFirstComponent( Engine::ComponentType::TRANSFORM ));

        else
          succeed &= false;

        return succeed;
      }


      CModel::CModel(void)
        : BaseComponent(Engine::ComponentType::MODEL)
      {
        transform = 0;
        
        diffuse_texture = new TextureType::Texture2D();
        normal_texture = 0;

        SetScale( Math::Vector3(1,1,1) );
        SetGlowAmount( 0 );
        SetGlowColor( Color(255,255,255,255) );
        SetModelColor( Color(255,255,255,255) );
        IsOpaque(true);

        SharedModel default_model(new Model::BaseModel());
        for( size_t i = 0; i < QUALITY::LAST; ++i )
          model.push_back(default_model);
        
        frag = new CgShader();
        vert = new CgShader();
        cg_glow_amount = new ShaderParameter();
        cg_glow_color = new ShaderParameter();
        cg_model_color = new ShaderParameter();
      }

      CModel::~CModel(void)
      {
      }

      void CModel::BindTexture()
      {
        if( diffuse_texture ) 
          diffuse_texture->Bind(0);

        if( normal_texture ) 
          normal_texture->Bind(1);
      }

      void CModel::UnbindTexture()
      {
        if( diffuse_texture ) 
          diffuse_texture->Unbind();

        if( normal_texture ) 
          normal_texture->Unbind();
      }

      void CModel::SetVertexShader( SharedShader2 shader )
      {
        vert = shader;
      }

      void CModel::SetFragmentShader( SharedShader2 shader )
      {
        frag = shader;
        cg_model_color = frag->GetParameter("model_color");
        cg_glow_color = frag->GetParameter("glow_color");
        cg_glow_amount = frag->GetParameter("glow_amount");
      }

      void CModel::SetVertexShader( const std::string & shader )
      {
        vert = Globals::shaderLoader->GetVertexShader(shader);
      }

      void CModel::SetFragmentShader( const std::string & shader )
      {
        frag = Globals::shaderLoader->GetFragmentShader(shader);
        cg_model_color = frag->GetParameter("model_color");
        cg_glow_color = frag->GetParameter("glow_color");
        cg_glow_amount = frag->GetParameter("glow_amount");
      }

      void CModel::SetModel(SharedModel _model, int _level)
      {
        if( _level == -1 )
        {
          if( model.size() > 1 )
            HasLOD(true);
          else
            HasLOD(false);

          for( size_t i = 0; i < model.size(); ++i )
          {
            model[i] = _model;
          }
        }
        else
        {
          model[_level] = _model;
        }
      }

      void CModel::SetModel( const std::string & mappedName, int _level )
      {
        Engine::Assets::ModelAsset * asset = (Engine::Assets::ModelAsset*)Globals::assetManager->GetAsset( mappedName );
        if( asset )
          SetModel( asset->model, _level );
      }

      void CModel::SetTexture( const std::string & mappedName )
      {
        Engine::Assets::Texture * asset = (Engine::Assets::Texture*)Globals::assetManager->GetAsset( mappedName );
        if( asset )
          SetTexture( asset->image );
      }

      void CModel::SetNormalmap( const std::string & mappedName )
      {
        Engine::Assets::Texture * asset = (Engine::Assets::Texture*)Globals::assetManager->GetAsset( mappedName );
        if( asset )
          SetNormalmap( asset->image );
      }

      Math::Vector3 CModel::WorldPosition() const
      {
        if( !transform )
          return Math::Vector3(0,0,0);
        else
          return transform->GetWorldPosition();
      }


      void CModel::BeginRender()
      {
        glPushAttrib( GL_ALL_ATTRIB_BITS );

        glEnable( GL_DEPTH_TEST );
        glEnable( GL_CULL_FACE );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      }

      void CModel::EndRender()
      {
        Performance::StateManager::SetActiveVertexShader(0);
        Performance::StateManager::SetActiveFragmentShader(0);
        Performance::StateManager::SetActiveModel(0);
        Performance::StateManager::UnbindAllTextures();

        glPopAttrib();
      }


      void CModel::Draw( const CFrustum & frustum )
      {
        LevelOfDetail();
        if( !transform || !frustum.SphereInside(transform->GetWorldPosition()+BoundingCenter(LOD), scale.x * BoundingRadius(LOD) )  )
          return;

        glMatrixMode( GL_MODELVIEW );
        PushMatrix();

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
        glScalef(scale.x, scale.y, scale.z);

        Performance::StateManager::SetActiveVertexShader(vert.Get());
        Performance::StateManager::SetActiveFragmentShader(frag.Get());
        Performance::StateManager::SetActiveModel( model[LOD].Get() );

        if( cg_model_color->param )
          cgSetParameter4f( cg_model_color->param, model_color.r, model_color.g, model_color.b, model_color.a );

        if( cg_glow_color )
          cgSetParameter4f( cg_glow_color->param, glow_color.r, glow_color.g, glow_color.b, glow_color.a );

        if( cg_glow_amount )
          cgSetParameter1f( cg_glow_amount->param, glow_amount );

        BindTexture();
        model[LOD]->Draw();
        //UnbindTexture();

        PopMatrix();

      }

      unsigned char Round( float x )
      {
        return static_cast<unsigned char>(x + .5f);
      }

      unsigned char CModel::LevelOfDetail()
      {
        if( !HasLOD() )
          return 0;

        //MAGIC NUMBER ALERT!
        float max_depth = 20000;
        float min_depth = 5000;
        float d = Utilities::Clamp<float>(depth, min_depth, max_depth);

        float t = (d-min_depth)/(max_depth-min_depth);
        LOD = (unsigned char)((QUALITY::HIGH - QUALITY::LOW) * (1 - t*t));
        return LOD;
      }

      Math::Vector3 CModel::BoundingCenter(int i) const
      {
        return model[i]->BoundingCenter();
      }

      float CModel::BoundingRadius(int i) const
      {
        return model[i]->BoundingRadius();
      }

      void CModel::PushToLua( lua_State *ls )
      {
        Lunar<CModel>::push( ls, this );
      }
      // Networking
      void CModel::InitIn( Networking::ByteStream& bs )
      {
        bs >> scale >> glow_amount >> 
          glow_color.r >> glow_color.g >> glow_color.b >> glow_color.a >> 
          model_color.r >> model_color.g >> model_color.b >> model_color.a;
      }

      void CModel::InitOut( Networking::ByteStream& bs ) const
      {
        bs << scale << glow_amount << 
          glow_color.r << glow_color.g << glow_color.b << glow_color.a << 
          model_color.r << model_color.g << model_color.b << model_color.a;
      }

      void CModel::SetModelColor( Color4f _color )
      {
        model_color = _color;
      }

      void CModel::SetGlowColor( Color4f _color )
      {
        glow_color = _color;
      }


      void CModel::SetGlowAmount(float percent)
      {
        glow_amount = percent;
      }

      LUA_START_DEFINE_CLASS( CModel )
        LUA_NAME_METHOD( CModel, "SetModelColor", LSetModelColor )
        LUA_NAME_METHOD( CModel, "SetScale", LSetScale )
        LUA_NAME_METHOD( CModel, "SetGlowAmount", LSetGlowAmount )
        LUA_NAME_METHOD( CModel, "SetGlowColor", LSetGlowColor )
        
        LUA_NAME_METHOD( CModel, "SetTexture", LSetTexture )
        LUA_NAME_METHOD( CModel, "SetNormalMap", LSetNormalMap )
        LUA_NAME_METHOD( CModel, "SetModel", LSetModel )

        LUA_END_DEFINE_CLASS


        LUA_DEFINE_METHOD( CModel, LSetModelColor )
      {
        Color col;

        int top = lua_gettop(ls);
        switch( top )
        {
        case 4:
          col.a = (unsigned char)luaL_checkint(ls, 4);
        case 3:
          col.b = (unsigned char)luaL_checkint(ls, 3);
        case 2:
          col.g = (unsigned char)luaL_checkint(ls, 2);
        case 1:
          col.r = (unsigned char)luaL_checkint(ls, 1);
        default:
          SetModelColor( col );
        }
        return 0;
      }

      LUA_DEFINE_METHOD( CModel, LSetScale )
      {
        float s = luaL_checkfloat(ls, 1);
        SetScale( s );
        return 0;
      }

      LUA_DEFINE_METHOD( CModel, LSetGlowAmount )
      {
        float percent = luaL_checkfloat(ls, 1);
        SetGlowAmount( percent );
        return 0;
      }

      LUA_DEFINE_METHOD( CModel, LSetGlowColor )
      {
        Color col;

        int top = lua_gettop(ls);
        switch( top )
        {
        case 4:
          col.a = (unsigned char)luaL_checkint(ls, 4);
        case 3:
          col.b = (unsigned char)luaL_checkint(ls, 3);
        case 2:
          col.g = (unsigned char)luaL_checkint(ls, 2);
        case 1:
          col.r = (unsigned char)luaL_checkint(ls, 1);
        default:
          SetGlowColor( col );
        }
        return 0;
      }


      LUA_DEFINE_METHOD( CModel, LSetTexture )
      {
        const char *  mappedName = luaL_checkstring(ls, 1);
        SetTexture(mappedName);
        return 0;
      }

      LUA_DEFINE_METHOD( CModel, LSetNormalMap )
      {
        const char *  mappedName = luaL_checkstring(ls, 1);
        SetNormalmap(mappedName);
        return 0;
      }

      LUA_DEFINE_METHOD( CModel, LSetModel )
      {
        const char *  mappedName = luaL_checkstring(ls, 1);
        SetModel(mappedName);
        return 0;
      }


    }
  }
}
