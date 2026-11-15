/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CModel.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:30:01 CTallman
**  \brief   
*******************************************************************************/

#ifndef SF_CMODEL_GRAPHIC_H_
#define SF_CMODEL_GRAPHIC_H_

#include "QuatCamera.h"

#include "BaseModel.h"
#include "Texture2D.h"

#include "Math/CTransform.h"
#include "CFrustum.h"
#include "ShaderLoader.h"

#include "External/Lua/include/LunarHelper.h"

namespace SF
{
  namespace Graphics
  {
    
    namespace Components
    {

      Engine::BaseComponent * CreateModel( const ticpp::Element * element );

      class CModel : public Engine::BaseComponent
      {
      public:

        static bool Sorter( const CModel & A, const CModel & B )
        {
          return A.Depth() < B.Depth();
        }

        static bool SorterPtr( const CModel * A, const CModel * B )
        {
          return A->Depth() < B->Depth();
        }

        CModel(void);
        ~CModel(void);

        Engine::BaseComponent * Clone() const;
        bool InitDependencies( Engine::Entity * );

        void Draw( const CFrustum & frustum );

        void SetModel( SharedModel model, int level = -1 );
        void SetModel( const std::string & mappedName, int level = -1 );
        void SetTexture( SharedTexture _texture ) { diffuse_texture = _texture; }
        void SetTexture( const std::string & mappedName );
        void SetNormalmap( SharedTexture _texture ) { normal_texture = _texture; }
        void SetNormalmap( const std::string & mappedName );
        void SetScale( Math::Vector3 _scale ) { scale = _scale; }
        void SetScale( float _scale ) { scale = Math::Vector3(_scale,_scale,_scale); }
        void SetModelColor( Color4f _color );
        void SetGlowAmount( float percent );
        void SetGlowColor( Color4f _color );
        
        void SetVertexShader( const std::string & vert_shader );
        void SetVertexShader( SharedShader2 vert_shader );
        void SetFragmentShader( const std::string & frag_shader );
        void SetFragmentShader( SharedShader2 frag_shader );

        Math::Vector3 BoundingCenter(int i = 0) const;
        float BoundingRadius(int i = 0) const;

        static void BeginRender();
        static void EndRender();

        Math::Vector3 WorldPosition() const;

        bool HasLOD() const { return has_lod; }
        void HasLOD( bool lod ) { has_lod = lod; }
        bool IsOpaque() { return opaque; }
        void IsOpaque(bool isopaque) { opaque = isopaque; }
        float Depth() const { return depth; }
        void Depth(float d){ depth = d; }

        virtual void PushToLua( lua_State * ls );

        // Networking
        virtual void InitIn( Networking::ByteStream& bs );
        virtual void InitOut( Networking::ByteStream& bs ) const;

        unsigned char LevelOfDetail();

      private:

        void BindTexture();
        void UnbindTexture();

        Math::Vector3 scale;

        SharedShader2 frag;
        SharedShader2 vert;
        SharedShaderParameter cg_model_color;
        SharedShaderParameter cg_glow_color;
        SharedShaderParameter cg_glow_amount;

        std::vector<SharedModel> model;
        SharedTexture diffuse_texture;
        SharedTexture normal_texture;
        
        Color4f model_color;
        Color4f glow_color;
        float glow_amount;

        Math::CTransform * transform;

        float depth;
        bool opaque;
        bool has_lod;
        unsigned char LOD;

        LUA_DECLARE_CLASS( CModel )
          LUA_DECLARE_METHOD( LSetModelColor )
          LUA_DECLARE_METHOD( LSetScale )
          LUA_DECLARE_METHOD( LSetGlowAmount )
          LUA_DECLARE_METHOD( LSetGlowColor )

          LUA_DECLARE_METHOD( LSetTexture )
          LUA_DECLARE_METHOD( LSetNormalMap )
          LUA_DECLARE_METHOD( LSetModel )
      }; 
    }
  }
}


#endif
