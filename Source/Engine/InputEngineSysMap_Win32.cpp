/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    InputEngineSysMap_Win32.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM00/350
**  \par     Updated: 04/10/2009 @ 16:28:56 - nwilliam
**  \brief   System specific keyboard maping array/functions.
*******************************************************************************/

#ifdef _WIN32

#include <windows.h>
#include "InputEngine.h"

namespace
{
  unsigned char s_keyMap[255];
}

namespace SF {
namespace Engine
{
  void InputEngine::InitInputMap()
  {
    // Initialize all to invalid
    std::memset( s_keyMap, SF::Engine::InputMap::INVALID, 255 );

    //s_keyMap[VK_LBUTTON]
    //s_keyMap[VK_RBUTTON]
    //s_keyMap[VK_CANCEL]
    //s_keyMap[VK_MBUTTON]

    //#if(_WIN32_WINNT >= 0x0500)
    //s_keyMap[VK_XBUTTON1]
    //s_keyMap[VK_XBUTTON2]
    //#endif

    s_keyMap[VK_BACK]     = InputMap::BACKSPACE;
    s_keyMap[VK_TAB]      = InputMap::TAB;

    s_keyMap[VK_CLEAR]    = InputMap::CLEAR;
    s_keyMap[VK_RETURN]   = InputMap::RETURN;

    //s_keyMap[VK_SHIFT]
    //s_keyMap[VK_CONTROL] = InputMap::left
    s_keyMap[VK_MENU]     = InputMap::MENU;
    s_keyMap[VK_PAUSE]    = InputMap::PAUSE;
    s_keyMap[VK_CAPITAL]  = InputMap::CAPS_LOCK;

    s_keyMap[VK_ESCAPE]   = InputMap::ESCAPE;

    s_keyMap[VK_SPACE]    = InputMap::SPACE;
    s_keyMap[VK_PRIOR]    = InputMap::PAGE_UP;
    s_keyMap[VK_NEXT]     = InputMap::PAGE_DOWN;
    s_keyMap[VK_END]      = InputMap::END;
    s_keyMap[VK_HOME]     = InputMap::HOME;
    s_keyMap[VK_LEFT]     = InputMap::ARROW_LEFT;
    s_keyMap[VK_UP]       = InputMap::ARROW_UP;
    s_keyMap[VK_RIGHT]    = InputMap::ARROW_RIGHT;
    s_keyMap[VK_DOWN]     = InputMap::ARROW_DOWN;
    s_keyMap[VK_PRINT]    = InputMap::PRINT;
    s_keyMap[VK_SNAPSHOT] = InputMap::PRINT_SCREEN;
    s_keyMap[VK_INSERT]   = InputMap::INSERT;

    // Sigh.. defined in winnt.h
    #undef DELETE
    s_keyMap[VK_DELETE]   = InputMap::DELETE;
    s_keyMap[VK_HELP]     = InputMap::HELP;

    s_keyMap['0']         = InputMap::N_0;
    s_keyMap['1']         = InputMap::N_1;
    s_keyMap['2']         = InputMap::N_2;
    s_keyMap['3']         = InputMap::N_3;
    s_keyMap['4']         = InputMap::N_4;
    s_keyMap['5']         = InputMap::N_5;
    s_keyMap['6']         = InputMap::N_6;
    s_keyMap['7']         = InputMap::N_7;
    s_keyMap['8']         = InputMap::N_8;
    s_keyMap['9']         = InputMap::N_9;

    s_keyMap['A']         = InputMap::A;
    s_keyMap['B']         = InputMap::B;
    s_keyMap['C']         = InputMap::C;
    s_keyMap['D']         = InputMap::D;
    s_keyMap['E']         = InputMap::E;
    s_keyMap['F']         = InputMap::F;
    s_keyMap['G']         = InputMap::G;
    s_keyMap['H']         = InputMap::H;
    s_keyMap['I']         = InputMap::I;
    s_keyMap['J']         = InputMap::J;
    s_keyMap['K']         = InputMap::K;
    s_keyMap['L']         = InputMap::L;
    s_keyMap['M']         = InputMap::M;
    s_keyMap['N']         = InputMap::N;
    s_keyMap['O']         = InputMap::O;
    s_keyMap['P']         = InputMap::P;
    s_keyMap['Q']         = InputMap::Q;
    s_keyMap['R']         = InputMap::R;
    s_keyMap['S']         = InputMap::S;
    s_keyMap['T']         = InputMap::T;
    s_keyMap['U']         = InputMap::U;
    s_keyMap['V']         = InputMap::V;
    s_keyMap['W']         = InputMap::W;
    s_keyMap['X']         = InputMap::X;
    s_keyMap['Y']         = InputMap::Y;
    s_keyMap['Z']         = InputMap::Z;

    s_keyMap[VK_LWIN]     = InputMap::LEFT_WINKEY;
    s_keyMap[VK_RWIN]     = InputMap::RIGHT_WINKEY;
    s_keyMap[VK_APPS]     = InputMap::MENU;

    s_keyMap[VK_NUMPAD0]   = InputMap::NUMPAD_0;
    s_keyMap[VK_NUMPAD1]   = InputMap::NUMPAD_1;
    s_keyMap[VK_NUMPAD2]   = InputMap::NUMPAD_2;
    s_keyMap[VK_NUMPAD3]   = InputMap::NUMPAD_3;
    s_keyMap[VK_NUMPAD4]   = InputMap::NUMPAD_4;
    s_keyMap[VK_NUMPAD5]   = InputMap::NUMPAD_5;
    s_keyMap[VK_NUMPAD6]   = InputMap::NUMPAD_6;
    s_keyMap[VK_NUMPAD7]   = InputMap::NUMPAD_7;
    s_keyMap[VK_NUMPAD8]   = InputMap::NUMPAD_8;
    s_keyMap[VK_NUMPAD9]   = InputMap::NUMPAD_9;
    s_keyMap[VK_MULTIPLY]  = InputMap::NUMPAD_MULTIPLY;
    s_keyMap[VK_ADD]       = InputMap::NUMPAD_ADD;
    s_keyMap[VK_SEPARATOR] = InputMap::NUMPAD_ENTER;
    s_keyMap[VK_SUBTRACT]  = InputMap::NUMPAD_SUBTRACT;
    s_keyMap[VK_DECIMAL]   = InputMap::NUMPAD_DECIMAL;
    s_keyMap[VK_DIVIDE]    = InputMap::NUMPAD_DIVIDE;
    s_keyMap[VK_F1]        = InputMap::F1;
    s_keyMap[VK_F2]        = InputMap::F2;
    s_keyMap[VK_F3]        = InputMap::F3;
    s_keyMap[VK_F4]        = InputMap::F4;
    s_keyMap[VK_F5]        = InputMap::F5;
    s_keyMap[VK_F6]        = InputMap::F6;
    s_keyMap[VK_F7]        = InputMap::F7;
    s_keyMap[VK_F8]        = InputMap::F8;
    s_keyMap[VK_F9]        = InputMap::F9;
    s_keyMap[VK_F10]       = InputMap::F10;
    s_keyMap[VK_F11]       = InputMap::F11;
    s_keyMap[VK_F12]       = InputMap::F12;
    s_keyMap[VK_F13]       = InputMap::F13;
    s_keyMap[VK_F14]       = InputMap::F14;
    s_keyMap[VK_F15]       = InputMap::F15;

    s_keyMap[VK_NUMLOCK]   = InputMap::NUM_LOCK;
    s_keyMap[VK_SCROLL]    = InputMap::SCROLL_LOCK;

    s_keyMap[VK_LSHIFT]    = InputMap::LEFT_SHIFT;
    s_keyMap[VK_RSHIFT]    = InputMap::RIGHT_SHIFT;
    s_keyMap[VK_LCONTROL]  = InputMap::LEFT_CONTROL;
    s_keyMap[VK_RCONTROL]  = InputMap::RIGHT_CONTROL;

    s_keyMap[VK_LMENU]     = InputMap::LEFT_ALT;
    s_keyMap[VK_RMENU]     = InputMap::RIGHT_ALT;

    #ifndef VK_OEM_PLUS
      #define VK_OEM_PLUS 0xBB
      #define VK_OEM_COMMA 0xBC
      #define VK_OEM_MINUS 0xBD
      #define VK_OEM_PERIOD 0xBE
      #define VK_ICO_CLEAR 0xE6
    #endif

    s_keyMap[VK_OEM_1]      = InputMap::SEMICOLON;     // ';:' for US
    s_keyMap[VK_OEM_PLUS]   = InputMap::EQUALS;        // '+' any country
    s_keyMap[VK_OEM_COMMA]  = InputMap::COMMA;         // ',' any country
    s_keyMap[VK_OEM_MINUS]  = InputMap::MINUS;         // '-' any country
    s_keyMap[VK_OEM_PERIOD] = InputMap::PERIOD;        // '.' any country
    s_keyMap[VK_OEM_2]      = InputMap::SLASH;         // '/?' for US
    s_keyMap[VK_OEM_3]      = InputMap::BACKQUOTE;     // '`~' for US
    s_keyMap[VK_OEM_4]      = InputMap::LEFT_BRACKET;  //  '[{' for US
    s_keyMap[VK_OEM_5]      = InputMap::BACKSLASH;     //  '\|' for US
    s_keyMap[VK_OEM_6]      = InputMap::RIGHT_BRACKET; //  ']}' for US
    s_keyMap[VK_OEM_7]      = InputMap::APOSTROPHE;    //  ''"' for US

    s_keyMap[VK_ICO_CLEAR]  = InputMap::CLEAR;
  }

  int InputEngine::MapSysKeyToInputMap( int syskey )
  {
    #ifndef MAPVK_VSC_TO_VK_EX
      const int MAP_VK_VSC_TO_VK_EX = 3;
    #endif

    int vk = MapVirtualKey( syskey, MAP_VK_VSC_TO_VK_EX );

    return s_keyMap[vk];
  }

} // Engine namespace
} // SF namespace

#endif // _WIN32
