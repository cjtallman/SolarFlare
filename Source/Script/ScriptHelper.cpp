/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ScriptHelper.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/06/2009 @ 12:50:57 - nwilliam
**  \brief   Implementation of script related functions.
*******************************************************************************/

#include "ScriptHelper.h"
#include "ScriptEngine.h"
#include "SolarFlare/Globals.h"

namespace SF {
namespace Script
{
  //////////////////////////////////////////////////////////////////////////////
  // LuaRef wrapper
  //////////////////////////////////////////////////////////////////////////////
  LuaRef::LuaRef() : m_refKey( LUA_NOREF )
  {
  }

  LuaRef::~LuaRef()
  {
    Release();
  }

  bool LuaRef::Valid() const
  {
    return m_refKey != LUA_NOREF;
  }

  LuaRef::operator bool() const
  {
    return Valid();
  }

  bool LuaRef::Set( lua_State* ls )
  {
    Release();

    m_refKey = luaL_ref( ls, LUA_REGISTRYINDEX );

    if( m_refKey == LUA_REFNIL )
    {
      m_refKey = LUA_NOREF;
    }

    return Valid();
  }

  void LuaRef::Get( lua_State* ls ) const
  {
    if( Valid() )
    {
      lua_rawgeti( ls, LUA_REGISTRYINDEX, m_refKey );
    }
    else
    {
      lua_pushnil( ls );
    }
  }

  void LuaRef::Release()
  {
    if( Valid() )
    {
      if( m_ref.Unique() )
      {
        lua_State* ls = Globals::scriptEngine->MasterState();
        luaL_unref( ls, LUA_REGISTRYINDEX, m_refKey );
      }
      else
      {
        m_ref = Utilities::RefCount();
      }
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // LuaFunction wrapper
  //////////////////////////////////////////////////////////////////////////////
  LuaFunction::LuaFunction()
  {
  }

  LuaFunction::~LuaFunction()
  {
  }

  bool LuaFunction::Valid() const
  {
    return m_ref.Valid();
  }

  LuaFunction::operator bool() const
  {
    return Valid();
  }

  bool LuaFunction::Set( lua_State* ls )
  {
    if( lua_isfunction( ls, -1 ) )
    {
      return m_ref.Set( ls );
    }

    return false;
  }

  void LuaFunction::Get( lua_State* ls ) const
  {
    m_ref.Get( ls );
  }

  int LuaFunction::operator()( lua_State* ls, int args )
  {
    Get( ls );

    if( args > 0 )
    {
      int top = lua_gettop( ls );
      lua_insert( ls, top - args );
    }

    return lua_pcall( ls, args, LUA_MULTRET, 0 );
  }


  //////////////////////////////////////////////////////////////////////////////
  // LuaValue
  //////////////////////////////////////////////////////////////////////////////
  LuaValue::LuaValue() : m_type( LUA_TNONE ),
                         m_nval( 0.0 )
  {
  }

  int LuaValue::Type() const
  {
    return m_type;
  }

  bool LuaValue::Vald() const
  {
    return (m_type != LUA_TNONE);
  }

  void LuaValue::ToLua( lua_State* ls ) const
  {
    switch( m_type )
    {
      case LUA_TBOOLEAN:  lua_pushboolean( ls, m_nval != 0.0f );  break;
      case LUA_TNUMBER:   lua_pushnumber( ls, m_nval );           break;
      case LUA_TSTRING:   lua_pushstring( ls, m_sval.c_str() );   break;
      default:            lua_pushnil( ls );
    }
  }

  void LuaValue::FromLua( lua_State* ls, int index )
  {
    m_type = static_cast<int8>( lua_type( ls, index ) );

    switch( m_type )
    {
      case LUA_TBOOLEAN:  m_nval = lua_toboolean( ls, index ) ? 1.0f : 0.0f;  break;
      case LUA_TNUMBER:   m_nval = lua_tonumber( ls, index );                 break;
      case LUA_TSTRING:   m_sval = lua_tostring( ls, index );                 break;
      default:            m_type = LUA_TNONE;
    }
  }

  Networking::ByteStream& operator>>( Networking::ByteStream& bs, LuaValue& val )
  {
    bs >> val.m_type;

    switch( val.m_type )
    {
      case LUA_TBOOLEAN:
        uint8 b;
        bs >> b;
        val.m_nval = b ? 1.0f : 0.0f;
      break;

      case LUA_TNUMBER:
        bs >> val.m_nval;
      break;

      case LUA_TSTRING:
        bs >> val.m_sval;
      break;
    }

    return bs;
  }

  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const LuaValue& val )
  {
    bs << val.m_type;

    switch( val.m_type )
    {
      case LUA_TBOOLEAN:
         bs << static_cast<uint8>( (val.m_nval != 0.0f) ? 0xFF : 0x00 );
      break;

      case LUA_TNUMBER:
        bs << val.m_nval;
      break;

      case LUA_TSTRING:
        bs << val.m_sval;
      break;
    }

    return bs;
  }

} // Script namespace
} // SF namespace
