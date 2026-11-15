/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    StateManager.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:34:13 CTallman
**  \brief   Maintains the current graphics state.
*******************************************************************************/

#include "StateManager.h"

namespace SF
{
  namespace Graphics
  {
    namespace Performance
    {
      Model::BaseModel * StateManager::active_model = 0;
      CgShader * StateManager::active_vertex_shader = 0;
      CgShader * StateManager::active_fragment_shader = 0;
      unsigned int StateManager::quality = QUALITY::HIGH;
      StateManager::TexBind StateManager::loaded_textures[8] = {TexBind()};
      unsigned int StateManager::active_texture = 0;

      void StateManager::SetGraphicsQuality( unsigned int _quality )
      {
        quality = _quality;
      }

      unsigned int StateManager::GetGraphicsQuality()
      {
        return quality;
      }

      void StateManager::SetActiveModel( Model::BaseModel * _model )
      {
        if(_model == active_model)
          return;

        if( active_model )
          active_model->Unload();

        active_model = _model;

        if( active_model )
          active_model->Load();
      }

      void StateManager::SetActiveVertexShader( CgShader * _shader )
      {
        if( _shader == active_vertex_shader )
          return;

        if( active_vertex_shader )
          active_vertex_shader->Unbind();

        active_vertex_shader = _shader;

        if( active_vertex_shader )
          active_vertex_shader->Bind();
      }

      void StateManager::SetActiveFragmentShader( CgShader * _shader )
      {
        if( _shader == active_fragment_shader )
          return;

        if( active_fragment_shader )
          active_fragment_shader->Unbind();

        active_fragment_shader = _shader;

        if( active_fragment_shader )
          active_fragment_shader->Bind();
      }

      void StateManager::UnbindAllTextures()
      {
        for( int i = 0; i < 8; ++i )
        {
          if( loaded_textures[i].id )
          {
            SetActiveTexture(i);
            switch(loaded_textures[i].target)
            {
            case GL_TEXTURE_1D:
              DisableTexture1D(i);
              break;
            default:
            case GL_TEXTURE_2D:
              DisableTexture2D(i);
              break;
            case GL_TEXTURE_CUBE_MAP:
              DisableTextureCube(i);
              break;
            }
          }
        }
      }

      
      void StateManager::SetActiveTexture(unsigned int stage)
      {
        if( active_texture != stage )
        {
          active_texture = stage;
          CheckThisForErrors(glActiveTexture(GL_TEXTURE0+stage));
        }
      }

      bool StateManager::TextureIsLoaded(unsigned int id, unsigned int stage)
      {
        return loaded_textures[stage].id == id;
      }

      void StateManager::LoadTexture(unsigned int id, unsigned int stage, unsigned int target)
      {
        loaded_textures[stage].id = id;
        loaded_textures[stage].target = target;
      }

      void StateManager::EnableTexture1D(unsigned int id, unsigned int stage)
      {
        if( TextureIsLoaded(id,stage) )
          return;
        LoadTexture(id,stage,GL_TEXTURE_1D);
        SetActiveTexture(stage);
        CheckThisForErrors(glBindTexture(GL_TEXTURE_1D,id));
        CheckThisForErrors(glEnable(GL_TEXTURE_1D));
      }

      void StateManager::DisableTexture1D(unsigned int stage)
      {
        LoadTexture(0,stage,GL_TEXTURE_1D);
        SetActiveTexture(stage);
        CheckThisForErrors(glDisable(GL_TEXTURE_1D));
        CheckThisForErrors(glBindTexture(GL_TEXTURE_1D,0));
      }


      void StateManager::EnableTexture2D(unsigned int id, unsigned int stage)
      {
        if( TextureIsLoaded(id,stage) )
          return;
        LoadTexture(id,stage,GL_TEXTURE_2D);
        SetActiveTexture(stage);
        CheckThisForErrors(glEnable(GL_TEXTURE_2D));
        CheckThisForErrors(glBindTexture(GL_TEXTURE_2D,id));
      }

      void StateManager::DisableTexture2D(unsigned int stage)
      {
        LoadTexture(0,stage,GL_TEXTURE_2D);
        SetActiveTexture(stage);
        CheckThisForErrors(glBindTexture(GL_TEXTURE_2D,0));
        CheckThisForErrors(glDisable(GL_TEXTURE_2D));
      }

      void StateManager::EnableTextureCube(unsigned int id, unsigned int stage)
      {
        if( TextureIsLoaded(id,stage) )
          return;
        LoadTexture(id,stage,GL_TEXTURE_CUBE_MAP);
        SetActiveTexture(stage);
        CheckThisForErrors(glBindTexture(GL_TEXTURE_CUBE_MAP,id));
        CheckThisForErrors(glEnable(GL_TEXTURE_CUBE_MAP));
      }

      void StateManager::DisableTextureCube(unsigned int stage)
      {
        LoadTexture(0,stage,GL_TEXTURE_CUBE_MAP);
        SetActiveTexture(stage);
        CheckThisForErrors(glDisable(GL_TEXTURE_CUBE_MAP));
        CheckThisForErrors(glBindTexture(GL_TEXTURE_CUBE_MAP,0));
      }


    }
  }
}
