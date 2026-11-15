/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CActionRegion.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 
**  \brief   
*******************************************************************************/

#include "CActionRegion.h"
#include "TextEngine.h"
#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"
#include "StateManager.h"

namespace SF{
  namespace Graphics{


    CActionRegion::CActionRegion()
    {
    }

    CActionRegion::~CActionRegion()
    {

    }

    void CActionRegion::SetFragmentShader( SharedShader2 shader )
    {
      frag = shader;
      cg_grad_top     = frag->GetParameter("grad_top");
      cg_grad_bottom  = frag->GetParameter("grad_bottom");
    }

    void CActionRegion::SetFragmentShader( const std::string & filename )
    {
      frag = Globals::shaderLoader->GetFragmentShader(filename);
      cg_grad_top     = frag->GetParameter("grad_top");
      cg_grad_bottom  = frag->GetParameter("grad_bottom");
    }

    void CActionRegion::SetTexture( int action, SharedTexture _texture )
    {
      if( action+1 > (int)textures.size() )
        textures.resize(action+1);
      
      textures[action] = _texture;
      textures[action]->UseMips(false);
    }

    void CActionRegion::SetTexture( int action, const std::string & mappedName )
    {
      static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
      SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( mappedName );
      if( asset )
        SetTexture( action, asset->image );
      else
      {
        Globals::console->Warning("Invalid Texture: \"" + mappedName + "\" is not loaded.");
      }
    }



    void CActionRegion::AddAction( const std::string & text1, Color4f top1, Color4f bottom1, const std::string & text2, Color4f top2, Color4f bottom2, int action )
    {
      Action newAction;
      newAction.text1 = text1;
      newAction.text2 = text2;
      newAction.action = action;
      if( action == 0 ) // Hit
        newAction.lifetime = 5000.0f;
      else              // Death
        newAction.lifetime = 10000.0f;
      newAction.top1 = top1;
      newAction.top2 = top2;
      newAction.bottom1 = bottom1;
      newAction.bottom2 = bottom2;

      if( actionHistory.size() > 20 )
        actionHistory.pop_front();

      actionHistory.push_back(newAction);
    }

    void CActionRegion::Draw(float elapsed)
    {
      static TextEngine * textEngine = TextEngine::Instance();
      ActionHistory::iterator itr = actionHistory.begin();
      float i = 0;

      Enable2D();

      while( itr != actionHistory.end() )
      {
        itr->lifetime -= elapsed;
        if( itr->lifetime <= 0.0f )
          itr = actionHistory.erase(itr);
        else
          ++itr;
      }

      

      itr = actionHistory.begin();

      float offset = 0;
      if( !actionHistory.empty() )
      {
        offset = actionHistory.front().lifetime;
        if( offset <= 1000.0f )
          offset /= 1000.0f;
        else
          offset = 1.0f;

        offset -= 1.0f;
      }

      while( itr != actionHistory.end() )
      {
        const std::string & text1 = itr->text1;
        const std::string & text2 = itr->text2;

        float lifetime = itr->lifetime;

        float alpha = 1.0f;
        if( lifetime <= 1000.0f )
          alpha = lifetime/1000.0f;
        
        float width1 = textEngine->GetWidth(text1,(float)fontSize,fontID) + 20 + fontSize;
        float width2 = textEngine->GetWidth(text2,(float)fontSize,fontID) + 10;

        Performance::StateManager::SetActiveFragmentShader(frag.Get());
        
        if( cg_grad_top->param )
          cgSetParameter4f( cg_grad_top->param, itr->top1.r, itr->top1.g, itr->top1.b, alpha*.8f );
        if( cg_grad_bottom->param )
          cgSetParameter4f( cg_grad_bottom->param, itr->bottom1.r, itr->bottom1.g, itr->bottom1.b, alpha*.8f );
        textEngine->PrintTextAdv( (float)right - width1 - width2, (float)top + i + offset*fontSize, text1, Color4f(1,1,1,alpha), (float)fontSize, fontID );

        if( cg_grad_top->param )
          cgSetParameter4f( cg_grad_top->param, itr->top2.r, itr->top2.g, itr->top2.b, alpha*.8f );
        if( cg_grad_bottom->param )
          cgSetParameter4f( cg_grad_bottom->param, itr->bottom2.r, itr->bottom2.g, itr->bottom2.b, alpha*.8f );
        textEngine->PrintTextAdv( (float)right - width2, (float)top + i + offset*fontSize, text2, Color4f(1,1,1,alpha), (float)fontSize, fontID );
        
        Performance::StateManager::SetActiveFragmentShader(0);

        if( itr->action < (int)textures.size() )
        {
          textures[itr->action]->Bind(0);
          glBegin(GL_QUADS);
          glTexCoord2f( 0, 0 );
          glVertex2f( right - width2 - fontSize - 10, (float)top + i + offset*fontSize );

          glTexCoord2f( 1, 0 );
          glVertex2f( right - width2 - 10, (float)top + i + offset*fontSize );

          glTexCoord2f( 1, 1 );
          glVertex2f( right - width2 - 10, (float)top + i + (1+offset)*fontSize );

          glTexCoord2f( 0, 1 );
          glVertex2f( right - width2 - fontSize - 10, (float)top + i + (1+offset)*fontSize );

          glEnd();
          textures[itr->action]->Unbind();
        }

        ++itr;
        i += fontSize;
      }


      Disable2D();
    }

    void CActionRegion::Clear()
    {
      actionHistory.clear();
    }

    void CActionRegion::SetRegion( int T, int R, int B, int L )
    {
      top = T;
      right = R;
      bottom = B;
      left = L;
    }

    void CActionRegion::SetFont( const std::string & fontName )
    {
      fontID = TextEngine::Instance()->GetFontID( fontName );
    }

    void CActionRegion::SetFontSize( int size )
    {
      fontSize = size;
    }


    void CActionRegion::PushToLua( lua_State *ls )
    {
      Lunar<CActionRegion>::push( ls, this );
    }

    LUA_START_DEFINE_CLASS( CActionRegion )
      LUA_NAME_METHOD( CActionRegion, "SetRegion", LSetRegion )
      LUA_NAME_METHOD( CActionRegion, "SetFont", LSetFont )
      LUA_NAME_METHOD( CActionRegion, "SetFontSize", LSetFontSize )
      LUA_NAME_METHOD( CActionRegion, "SetHorizontalAlignment", LSetHorizontalAlignment )
      LUA_NAME_METHOD( CActionRegion, "Clear", LClear )
      LUA_END_DEFINE_CLASS

    LUA_DEFINE_METHOD( CActionRegion, LSetRegion )
    {
      int T = luaL_checkint( ls, 1 );
      int R = luaL_checkint( ls, 2 );
      int B = luaL_checkint( ls, 3 );
      int L = luaL_checkint( ls, 4 );
      SetRegion( T, R, B, L );
      return 0;
    }

    LUA_DEFINE_METHOD( CActionRegion, LSetFont )
    {
      const char * name = luaL_checkstring( ls, 1 );
      if( name )
        SetFont(name);
      return 0;
    }

    LUA_DEFINE_METHOD( CActionRegion, LSetFontSize )
    {
      int size = luaL_checkint( ls, 1 );
      SetFontSize(size);
      return 0;
    }

    LUA_DEFINE_METHOD( CActionRegion, LSetHorizontalAlignment )
    {
      const char * options[] = { "LEFT", "CENTER", "RIGHT", 0 };
      int opt = luaL_checkoption(ls, 1, 0, options );
      SetHorizontalAlignment(HORIZONTAL(opt));
      return 0;
    }

    LUA_DEFINE_METHOD_NOLS( CActionRegion, LClear )
    {
      Clear();
      return 0;
    }



  }
}