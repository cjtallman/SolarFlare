/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    InputEngine.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/10/2009 @ 16:22:37 - nwilliam
**  \brief   Keyboard and mouse input tracking.
*******************************************************************************/

#ifndef SF_INPUT_ENGINE_H
#define SF_INPUT_ENGINE_H

#include <stack>
#include <bitset>
#include <string>
#include "Script/ScriptHelper.h"
#include "InputEngineInputMap.h"

namespace SF {
namespace Engine
{
  class InputEngine
  {
    public:
      InputEngine();
      ~InputEngine();

      void Update();

      void TextInput( char c );
      const std::string& TextInput() const;

      // All Input
      bool IsAnyPressed() const;
      bool IsAnyTriggered() const;
      bool IsPressed( int input ) const;
      bool IsTriggered( int input ) const;
      bool IsRepeating( int innput ) const;
      void SetPressed( int input, bool state );
      void SetTriggered( int input, bool state );
 
      // Mouse Specific
      int MouseX() const;
      int MouseY() const;
      void MouseX( int x );
      void MouseY( int y );

      int MouseDX() const;
      int MouseDY() const;
      void MouseDX( int dx );
      void MouseDY( int dy );

      int WheelDelta() const;
      void WheelDelta( int delta );

      // Note: Implemented in InputEngineSysMap_<foo>.cpp
      static void InitInputMap();
      static int MapSysKeyToInputMap( int syskey );

    private:
      typedef std::stack< int > Stack;

      int m_mouseX;
      int m_mouseY;
      int m_mouseDX;
      int m_mouseDY;
      int m_wheelDelta;

      int m_lastPressed;
      int m_lastTriggered;

      Stack m_unpress;
      std::string m_textInput;
      std::bitset< InputMap::LAST > m_pressed;
      std::bitset< InputMap::LAST > m_triggered;
      std::bitset< InputMap::LAST > m_repeating;


    LUA_DECLARE_CLASS( InputEngine )
    LUA_DECLARE_METHOD( LIsPressed )
    LUA_DECLARE_METHOD( LIsTriggered )
    LUA_DECLARE_METHOD( LIsRepeating )
    LUA_DECLARE_METHOD( LIsAnyPressed )
    LUA_DECLARE_METHOD( LIsAnyTriggered )
    LUA_DECLARE_METHOD( LGetLastTriggered )

    LUA_DECLARE_METHOD( LMouseDelta )
    LUA_DECLARE_METHOD( LMousePosition )
  };

} // Engine namespace
} // SF namespace

#endif // SF_INPUT_ENGINE_H
