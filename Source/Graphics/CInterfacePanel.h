/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CInterfacePanel.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:51:50 - nwilliam
**  \brief   Graphical component of the interface panel.
*******************************************************************************/

#ifndef SF_CINTERFACE_PANEL_H
#define SF_CINTERFACE_PANEL_H

#include "Engine/BaseComponent.h"
#include "CBaseInterfaceGraphic.h"
#include "Interface/CBasicElement.h"

#include "Math/CTransform.h"
#include "Utilities/Color.h"
#include "Texture2D.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {


      class CInterfacePanel : public CBaseInterfaceGraphic
      {
      public:
        static Engine::BaseComponent * CreateComponent( const ticpp::Element * element );
        virtual Engine::BaseComponent* Clone() const;
        virtual bool InitDependencies( Engine::Entity* );

        CInterfacePanel();
        ~CInterfacePanel();

        void SetTexture( SharedTexture _texture );
        void SetTexture( const std::string & _texture_name );

        void Draw();

        virtual void PushToLua( lua_State *ls );

      private:

        void BindTexture();
        void UnbindTexture();

        

        SharedTexture texture;


        LUA_DECLARE_CLASS( CInterfacePanel )
          LUA_DECLARE_METHOD( LSetTexture )

          LUA_DECLARE_METHOD( LSetZOrder )
          LUA_DECLARE_METHOD( LGetZOrder )
          
          LUA_DECLARE_METHOD( LSetState )
          LUA_DECLARE_METHOD( LGetState )

          LUA_DECLARE_METHOD( LBase )
      };

    }
  }
}

#endif

