/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    StateManager.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:34:29 CTallman
**  \brief   Maintains the current graphics state.
*******************************************************************************/

#ifndef _SF_STATE_MANAGER_H_
#define _SF_STATE_MANAGER_H_

#include "BaseModel.h"
#include "CgShader.h"

namespace SF
{
  namespace Graphics
  {
    namespace QUALITY
    {
      enum
      {
        LOW,
        MEDIUM,
        HIGH,
        LAST
      };
    }

    namespace Performance
    {      

      enum STATE_ENABLE
      {
        BLEND = GL_BLEND,
        DEPTH_TEST = GL_DEPTH_TEST,
        LIGHTING = GL_LIGHTING,
      };


      class StateManager
      {
      public:
        static void SetActiveModel( Model::BaseModel * _model );
        static void SetActiveVertexShader( CgShader * _shader );
        static void SetActiveFragmentShader( CgShader * _shader );
        static void SetActiveTexture(unsigned int stage = 0);
        static void UnbindAllTextures();

        static void Enable(enum STATE_ENABLE);
        static void Disable(enum STATE_ENABLE);

        static void SetGraphicsQuality( unsigned int _quality );
        static unsigned int GetGraphicsQuality();
        
        static void EnableTexture1D(unsigned int id, unsigned int stage = 0);
        static void DisableTexture1D(unsigned int stage = 0);

        static void EnableTexture2D(unsigned int id, unsigned int stage = 0);
        static void DisableTexture2D(unsigned int stage = 0);

        static void EnableTextureCube(unsigned int id, unsigned int stage = 0);
        static void DisableTextureCube(unsigned int stage = 0);

      private:

        struct TexBind
        {
          TexBind():id(0),target(GL_TEXTURE_2D){}
          unsigned int id;
          unsigned int target;
        };

        static bool TextureIsLoaded(unsigned int id, unsigned int stage = 0);
        static void LoadTexture(unsigned int id, unsigned int stage = 0, unsigned int target = GL_TEXTURE_2D);
        

        static Model::BaseModel * active_model;
        static CgShader * active_vertex_shader;
        static CgShader * active_fragment_shader;

        static TexBind loaded_textures[8];
        static unsigned int active_texture;

        static unsigned int quality;

        
      };
      

    }
  }
}

#endif
