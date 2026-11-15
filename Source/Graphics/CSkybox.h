/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CSkybox.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:29:14 CTallman
**  \brief   
*******************************************************************************/

#ifndef _SF_CSKYBOX_H_
#define _SF_CSKYBOX_H_

#include "OpenGLInclude.h"
#include "QuatCamera.h"
#include "Texture2D.h"
#include "ShaderLoader.h"

namespace SKYBOX_SIDE
{
  enum SIDE
  {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    FRONT,
    BACK
  };
}
namespace SF
{
  namespace Graphics
  {
    namespace Components
    {
      ////////////////////////////////////////////////////////////////////////
      // Skybox Creator
      ////////////////////////////////////////////////////////////////////////
      Engine::BaseComponent * CreateSkyboxComponent( const ticpp::Element * element );


	  class CSkybox : public Engine::BaseComponent
      {
      public:
        CSkybox();
        ~CSkybox(void);

        void Draw();
		    void EnableShader();
		    void DisableShader();

        virtual Engine::BaseComponent* Clone() const;
        virtual bool InitDependencies( Engine::Entity* );
        void PushToLua(lua_State *ls);

        void SetTexture( SKYBOX_SIDE::SIDE side, SharedTexture _texture );
        void SetTexture( SKYBOX_SIDE::SIDE side, const std::string & _texture_name );

        void SetFragmentShader( SharedShader2 frag );
        void SetFragmentShader( const std::string & filename );

      private:

        void DrawBox();

        SharedShader2 frag;
        SharedShaderParameter cg_brightness;

        SharedTexture left;
        SharedTexture right;
        SharedTexture top;
        SharedTexture bottom;
        SharedTexture front;
        SharedTexture back;

        float brightness;

        LUA_DECLARE_CLASS( CSkybox )

        LUA_DECLARE_METHOD( LSetBrightness )
      };

    }
  }
}

#endif
