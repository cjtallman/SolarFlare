/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    InputEngine.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/10/2009 @ 16:34:24 - nwilliam
**  \brief   Implementation of keybourd and mouse input tracking.
*******************************************************************************/

#include "InputEngine.h"
#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"

namespace SF {
namespace Engine
{
  //////////////////////////////////////////////////////////////////////////////
  // Constuctor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  InputEngine::InputEngine() : m_mouseX( 0 ),
                               m_mouseY( 0 ),
                               m_mouseDX( 0 ),
                               m_mouseDY( 0 ),
                               m_wheelDelta( 0 )
  {
    InitInputMap();
  }

  InputEngine::~InputEngine()
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  void InputEngine::Update()
  {
    m_textInput.clear();
    m_mouseDX = m_mouseDY = m_wheelDelta = 0;

    m_triggered.reset();
    m_repeating.reset();

    while( !m_unpress.empty() )
    {
      m_pressed[m_unpress.top()] = false;
      m_unpress.pop();
    }
  }
  
  void InputEngine::TextInput( char c )
  {
    if( Globals::console->IsActive() )
    {
      Globals::console->PushChar( c );
    }

    m_textInput += c;
  }
  
  const std::string& InputEngine::TextInput() const
  {
    return m_textInput;
  }

  bool InputEngine::IsAnyPressed() const
  {
    return m_pressed.any();
  }

  bool InputEngine::IsAnyTriggered() const
  {
    return m_triggered.any();
  }

  bool InputEngine::IsPressed( int input ) const
  {
    return m_pressed[input];
  }

  bool InputEngine::IsTriggered( int input ) const
  {
    return m_triggered[input];
  }

  bool InputEngine::IsRepeating( int input ) const
  {
    return m_repeating[input] || m_triggered[input];
  }

  void InputEngine::SetPressed( int input, bool state )
  {
    if( state )
    {
      if( m_pressed[input] == false )
      {
        m_pressed[input] = true;
        m_triggered[input] = true;
      }
      else
      {
        m_repeating[input] = true;
      }
    }
    else
    {
      m_unpress.push( input );
    }
  }

  void InputEngine::SetTriggered( int input, bool state )
  {
    m_triggered[input] = state;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Mouse
  //////////////////////////////////////////////////////////////////////////////
  int InputEngine::MouseX() const
  {
    return m_mouseX;
  }

  int InputEngine::MouseY() const
  {
    return m_mouseY;
  }

  void InputEngine::MouseX( int x )
  {
    m_mouseDX = x - m_mouseX;
    m_mouseX = x;
  }

  void InputEngine::MouseY( int y )
  {
    m_mouseDY = y - m_mouseY;
    m_mouseY = y;
  }

  int InputEngine::MouseDX() const
  {
    return m_mouseDX;
  }

  int InputEngine::MouseDY() const
  {
    return m_mouseDY;
  }

  void InputEngine::MouseDX( int dx )
  {
    m_mouseDX = dx;
  }

  void InputEngine::MouseDY( int dy )
  {
    m_mouseDY = dy;
  }

  int InputEngine::WheelDelta() const
  {
    return m_wheelDelta;
  }

  void InputEngine::WheelDelta( int delta )
  {
    m_wheelDelta += delta;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( InputEngine )
    LUA_NAME_METHOD( InputEngine, "IsPressed",        LIsPressed )
    LUA_NAME_METHOD( InputEngine, "IsTriggered",      LIsTriggered )
    LUA_NAME_METHOD( InputEngine, "IsRepeating",      LIsRepeating )
    LUA_NAME_METHOD( InputEngine, "IsAnyPressed",     LIsAnyPressed )
    LUA_NAME_METHOD( InputEngine, "IsAnyTriggered",   LIsAnyTriggered )
    LUA_NAME_METHOD( InputEngine, "GetLastTriggered", LGetLastTriggered )
    LUA_NAME_METHOD( InputEngine, "MouseDelta",       LMouseDelta )
    LUA_NAME_METHOD( InputEngine, "MousePosition",    LMousePosition )
  LUA_END_DEFINE_CLASS


  LUA_DEFINE_METHOD( InputEngine, LIsPressed )
  {
    int input = luaL_checkint( ls, 1 );
    lua_pushboolean( ls, IsPressed( input ) );
    return 1;
  }

  LUA_DEFINE_METHOD( InputEngine, LIsTriggered )
  {
    int input = luaL_checkint( ls, 1 );
    lua_pushboolean( ls, IsTriggered( input ) );
    return 1;
  }

  LUA_DEFINE_METHOD( InputEngine, LIsRepeating )
  {
    int input = luaL_checkint( ls, 1 );
    lua_pushboolean( ls, IsRepeating( input ) );
    return 1;
  }

  LUA_DEFINE_METHOD( InputEngine, LIsAnyPressed )
  {
    lua_pushboolean( ls, IsAnyPressed() );
    return 1;
  }

  LUA_DEFINE_METHOD( InputEngine, LIsAnyTriggered )
  {
    lua_pushboolean( ls, IsAnyTriggered() );
    return 1;
  }

  LUA_DEFINE_METHOD( InputEngine, LGetLastTriggered )
  {
    int input = InputMap::INVALID;

    for( size_t i = 0; i < m_triggered.size(); ++i )
    {
      if( m_triggered.test(i) )
      {
        input = (int)i;
        break;
      }
    }

    lua_pushnumber( ls, input );
    return 1;
  }

  LUA_DEFINE_METHOD( InputEngine, LMouseDelta )
  {
    lua_pushinteger( ls, MouseDX() );
    lua_pushinteger( ls, MouseDY() );
    return 2;
  }

  LUA_DEFINE_METHOD( InputEngine, LMousePosition )
  {
    lua_pushinteger( ls, MouseX() );
    lua_pushinteger( ls, MouseY() );
    return 2;
  }

} // Engine namespace
} // SF namespace
