/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CPlanetGraphic.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:30:22 CTallman
**  \brief   Draws the planet component.
*******************************************************************************/

#ifndef SF_CPlanetGraphic_H_
#define SF_CPlanetGraphic_H_

#include "CSunGraphic.h"
#include "FrameBuffer2.h"
#include "CubemapTexture.h"
#include "Math/Quaternion.h"
#include <queue>

namespace SF {
  namespace Graphics {
    namespace Components {

      Engine::BaseComponent * CreatePlanetGraphic( const ticpp::Element * element );      

      class CPlanetGraphic : public CSunGraphic 
      {
      public:

        CPlanetGraphic();
        virtual ~CPlanetGraphic();

        virtual Engine::BaseComponent * Clone() const;
        virtual bool InitDependencies( Engine::Entity * parent );
        virtual void Draw( const CFrustum & frustum ); 
        void Update( float elapsed );

        virtual void SetVertexShader( const std::string & vert_shader );
        virtual void SetVertexShader( SharedShader2 vert_shader );
        virtual void SetFragmentShader( const std::string & frag_shader );
        virtual void SetFragmentShader( SharedShader2 frag_shader );

        static void BeginRender();
        static void EndRender();

        float GetHeight( float phi, float theta ) const;
       
        VertexBuffer3<Vertex::HeightVertex> * GetBackHeightBuffer() { return back_height_buffer; }
        TextureType::Texture2D * GetHeightTexture() { return &height_texture; }
        TextureType::CubemapTexture * GetDeformTexture() { return &deform_texture; }
        SharedTexture crater_texture;

        void DeepCopy( const CPlanetGraphic & planet );

        // Networking
        virtual void InitIn( Networking::ByteStream& bs );
        virtual void InitOut( Networking::ByteStream& bs ) const;

        // Scripting
        virtual void PushToLua( lua_State *ls );

      private:

        void BindTexture();
        void UnbindTexture();
        void RenderModel();
        
        // Height Stuff
        void UpdateHeights();
        void UpdateHeightBuffer();
        void SwapHeightBuffers();
        VertexBuffer3<Vertex::HeightVertex> * height_buffer;
        VertexBuffer3<Vertex::HeightVertex> * back_height_buffer;
        bool updated_heights;

        // Deformation Stuff
        void AddDeform( const Math::Vector3 & position, float scale = 1.0f );
        void RenderDeform();
        void Deform();
        VertexBuffer3<Vertex::DeformVertex> deform_verts;
        std::vector<float> deform_scales;
        TextureType::CubemapTexture deform_texture;
        unsigned char deform_delay;
        unsigned char deform_faces;

        // Shader Stuff
        SharedShader2 deformVert;
        SharedShader2 deformFrag;
        SharedShaderParameter cg_tex_width;
        SharedShaderParameter cg_tex_height;

        LUA_DECLARE_CLASS( CPlanetGraphic )
          LUA_DECLARE_METHOD( LGetMinRadius )
          LUA_DECLARE_METHOD( LSetMinRadius )
          LUA_DECLARE_METHOD( LGetMaxRadius )
          LUA_DECLARE_METHOD( LSetMaxRadius )
          LUA_DECLARE_METHOD( LGetHeight )
          LUA_DECLARE_METHOD( LSetNoise )
          LUA_DECLARE_METHOD( LDeform )
          LUA_DECLARE_METHOD( LDeepCopy )
          LUA_DECLARE_METHOD( LSetModel )

          NO_COPY( CPlanetGraphic );
      };
    }
  }
}
#endif
