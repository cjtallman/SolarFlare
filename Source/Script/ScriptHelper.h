/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ScriptHelper.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/06/2009 @ 12:37:18 - nwilliam
**  \brief   Helper include for script related actions (binding, functions, etc)
*******************************************************************************/

#ifndef SF_SCRIPT_HELPER_H
#define SF_SCRIPT_HELPER_H

#include "Networking/ByteStream.h"
#include "Utilities/RefCount.h"
#include "External/Lua/include/LunarHelper.h"

namespace SF {
namespace Script
{
  class LuaRef
  {
    public:
      LuaRef();
      ~LuaRef();
      
      bool Valid() const;
      operator bool() const;

      bool Set( lua_State* ls );
      void Get( lua_State* ls ) const;

    private:
      void Release();

      int m_refKey;
      Utilities::RefCount m_ref;
  };


  class LuaFunction
  {
    public:
      LuaFunction();
      ~LuaFunction();

      bool Valid() const;
      operator bool() const;
      
      bool Set( lua_State* ls );
      void Get( lua_State* ls ) const;
      int operator()( lua_State* ls, int args = 0 );

    private:
      LuaRef m_ref;
  };


  class LuaValue
  {
    public:
      LuaValue();

      int Type() const;
      bool Vald() const;
      operator bool() const;
      void ToLua( lua_State* ls ) const;
      void FromLua( lua_State* ls, int index );

    private:
      friend Networking::ByteStream& operator>>( Networking::ByteStream&, LuaValue& );
      friend Networking::ByteStream& operator<<( Networking::ByteStream&, const LuaValue& );

      int8 m_type;
      double m_nval;
      std::string m_sval;
  };

  Networking::ByteStream& operator>>( Networking::ByteStream&, LuaValue& );
  Networking::ByteStream& operator<<( Networking::ByteStream&, const LuaValue& );


} // Script namespace
} // SF namespace

#endif // SF_SCRIPT_HELPER_H
