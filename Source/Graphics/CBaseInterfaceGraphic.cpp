/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CBaseInterfaceGraphic.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:29:33 CTallman
**  \brief   
*******************************************************************************/

#include "CBaseInterfaceGraphic.h"
#include "Utilities/LeakCheck.h"
namespace SF {
  namespace Graphics {
    namespace Components
    {

      bool CBaseInterfaceGraphic::InitDependencies( Engine::Entity * parent )
      {
        if( parent && !parent->Has( Engine::ComponentType::TRANSFORM ) )
          return false;
       
        if( !m_transformName.empty() )
          transform = static_cast<Math::CTransform*>(parent->GetComponentByName( Engine::ComponentType::TRANSFORM, m_transformName ));
        else
          transform = static_cast<Math::CTransform*>(parent->GetFirstComponent( Engine::ComponentType::TRANSFORM ));

        
        return true;
      }

      CBaseInterfaceGraphic::CBaseInterfaceGraphic() 
        : Engine::BaseComponent( Engine::ComponentType::UI_GRAPHIC )
      {
        interface_element = 0;
        transform = 0;
        SetZOrder(0);
        SetState(UI_STATE::UP);
      }

      int CBaseInterfaceGraphic::GetZOrder() const
      {
        return z_order;
      }

      void CBaseInterfaceGraphic::SetZOrder( int z )
      {
        z_order = z;
      }

      void CBaseInterfaceGraphic::SetState( int state )
      {
        if( state < 0 || state >= UI_STATE::LAST )
          return;
        else
          ui_state = state;
      }

      int CBaseInterfaceGraphic::GetState() const
      {
        return ui_state;
      }

      void CBaseInterfaceGraphic::PushToLua( lua_State * ls )
      {
        Lunar<CBaseInterfaceGraphic>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CBaseInterfaceGraphic )
        LUA_NAME_METHOD( CBaseInterfaceGraphic, "GetZOrder", LGetZOrder )
        LUA_NAME_METHOD( CBaseInterfaceGraphic, "SetZOrder", LSetZOrder )
        LUA_NAME_METHOD( CBaseInterfaceGraphic, "GetState", LGetState )
        LUA_NAME_METHOD( CBaseInterfaceGraphic, "SetState", LSetState )
      LUA_END_DEFINE_CLASS

      LUA_DEFINE_METHOD( CBaseInterfaceGraphic, LGetZOrder )
      {
        lua_pushnumber( ls, GetZOrder() );
        return 1;
      }

      LUA_DEFINE_METHOD( CBaseInterfaceGraphic, LSetZOrder )
      {
        int z = luaL_checkint(ls, 1);
        SetZOrder(z);
        return 0;
      }

      LUA_DEFINE_METHOD( CBaseInterfaceGraphic, LGetState )
      {
        const char * options[] = { "DISABLED", "UP", "OVER", "DOWN", 0 };
        lua_pushstring( ls, options[GetState()] );
        return 1;
      }

      LUA_DEFINE_METHOD( CBaseInterfaceGraphic, LSetState )
      {
        const char * options[] = { "DISABLED", "UP", "OVER", "DOWN", 0 };
        int o = luaL_checkoption(ls, 1, 0, options );
        SetState(o);
        return 0;
      }


}
}
}