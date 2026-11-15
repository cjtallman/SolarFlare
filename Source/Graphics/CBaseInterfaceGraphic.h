/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CBaseInterfaceGraphic.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:26:33 CTallman
**  \brief   
*******************************************************************************/

#ifndef _SF_CBASE_INTERFACE_GRAPHIC_H_
#define _SF_CBASE_INTERFACE_GRAPHIC_H_

#include "OpenGLInclude.h"
#include "Utilities/Macros.h"
#include "Engine/BaseComponent.h"
#include "External/Lua/include/LunarHelper.h"
#include "Interface/CBasicElement.h"

// Forward declaration
namespace ticpp { class Node; class Element; }
namespace SF { namespace Math{ class CTransform; } namespace Engine { class Entity; } };

namespace SF{ 
  namespace Graphics{
    namespace Components{

        namespace UI_STATE 
        {
          enum
          {
            DISABLED,       //0 = Grayed Out
            UP,             //1 = Normal
            OVER,           //2 = Hover Over
            DOWN,           //3 = Clicked/Selected
            LAST            //UNUSED
          };
        }

      class CBaseInterfaceGraphic : public Engine::BaseComponent
      {
        

      public:
        ////////////////////////////////////////////////////////////////////////
        // Public BaseComponent Methods
        ////////////////////////////////////////////////////////////////////////
        virtual Engine::BaseComponent * Clone() const { return 0; }
        virtual bool InitDependencies( Engine::Entity* );

        ////////////////////////////////////////////////////////////////////////
        // Public GraphicsComponent Methods
        ////////////////////////////////////////////////////////////////////////
        CBaseInterfaceGraphic();
        virtual ~CBaseInterfaceGraphic(){}

        virtual void Update( float ){}
        virtual void Draw(){};
        
        
        int GetZOrder() const;
        void SetZOrder( int z );

        void SetState( int state );
        int GetState() const;

        virtual void PushToLua( lua_State * ls );

      protected:

        UI::CBasicElement * interface_element;
        Math::CTransform * transform;
        std::string m_transformName;

        int z_order;
        int ui_state;

        LUA_DECLARE_CLASS( CBaseInterfaceGraphic )
          LUA_DECLARE_METHOD( LGetZOrder )
          LUA_DECLARE_METHOD( LSetZOrder )
          LUA_DECLARE_METHOD( LGetState )
          LUA_DECLARE_METHOD( LSetState )
      };

      class InterfaceGraphicSorter
      {
      public:
        bool operator()(const CBaseInterfaceGraphic * A, const CBaseInterfaceGraphic * B)
        {
          return A->GetZOrder() < B->GetZOrder();
        }
      };


    }
  }
}


#endif

