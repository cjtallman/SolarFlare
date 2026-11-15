/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CActionRegion.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 
**  \brief   
*******************************************************************************/

#ifndef SF_CACTION_REGION_H
#define SF_CACTION_REGION_H

#include "Script/ScriptHelper.h"
#include "OpenGLInclude.h"
#include "ShaderLoader.h"
#include "Texture2D.h"
#include "Utilities/Color.h"
#include <deque>
#include <string>

namespace SF{
  namespace Graphics{

    class CActionRegion
    {
    public:
      enum HORIZONTAL { LEFT, CENTER, RIGHT };

      CActionRegion();
      ~CActionRegion();

      void Draw(float elapsed);

      void AddAction( const std::string & text1, Color4f top1, Color4f bottom1, const std::string & text2, Color4f top2, Color4f bottom2, int action );

      void SetHorizontalAlignment( HORIZONTAL align ){alignment = align;}
      void SetRegion( int T, int R, int B, int L );
      void SetFontSize( int size );
      void SetFont( const std::string & fontName );

      void SetFragmentShader( const std::string & mappedName );
      void SetFragmentShader( SharedShader2 shader );
      void SetTexture( int action, SharedTexture texture );
      void SetTexture( int action, const std::string & mappedName );

      void Clear();

      virtual void PushToLua( lua_State * ls );
    private:

      int top, right, bottom, left;
      enum HORIZONTAL alignment;
      int fontSize;
      int fontID;


      struct Action
      {
        std::string text1;
        std::string text2;
        Color4f top1;
        Color4f bottom1;
        Color4f top2;
        Color4f bottom2;

        int action;
        float lifetime;
      };

      typedef std::deque<Action> ActionHistory;
      ActionHistory actionHistory;

      SharedShader2 frag;
      SharedShaderParameter cg_grad_top;
      SharedShaderParameter cg_grad_bottom;

      std::vector<SharedTexture> textures;

      LUA_DECLARE_CLASS( CActionRegion )
        LUA_DECLARE_METHOD( LSetRegion )
        LUA_DECLARE_METHOD( LSetFont )
        LUA_DECLARE_METHOD( LSetFontSize )
        LUA_DECLARE_METHOD( LSetHorizontalAlignment )
        LUA_DECLARE_METHOD( LAddAction )
        LUA_DECLARE_METHOD( LClear )

    };



  }
}

#endif
