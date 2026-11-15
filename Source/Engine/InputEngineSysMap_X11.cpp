/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    InputEngineSysMap_X11.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM00/350
**  \par     Updated: 04/10/2009 @ 16:29:15 - nwilliam
**  \brief   X11 specific keyboard maping array/functions.
*******************************************************************************/

#ifdef __linux

#include "InputEngine.h"
#include <cstring> // memset
#include <X11/keysym.h>

namespace
{
  unsigned int s_keyMap[137];
}

namespace SF {
namespace Engine
{
  void InputEngine::InitInputMap()
  {
    // Initialize all to invalid
    std::memset( s_keyMap, SF::Engine::InputMap::INVALID, 137 );

    s_keyMap[XK_BackSpace]  = InputMap::BACKSPACE;
    s_keyMap[XK_Tab]        = InputMap::TAB;

    s_keyMap[XK_Clear]      = InputMap::CLEAR;
    s_keyMap[XK_Return]     = InputMap::RETURN;

    s_keyMap[XK_Menu]       = InputMap::MENU;
    s_keyMap[XK_Pause]      = InputMap::PAUSE;
    s_keyMap[XK_Caps_Lock]  = InputMap::CAPS_LOCK;

    s_keyMap[XK_Escape]     = InputMap::ESCAPE;

    s_keyMap[XK_space]      = InputMap::SPACE;
    s_keyMap[XK_Page_Up]    = InputMap::PAGE_UP;
    s_keyMap[XK_Page_Down]  = InputMap::PAGE_DOWN;
    s_keyMap[XK_End]        = InputMap::END;
    s_keyMap[XK_Home]       = InputMap::HOME;
    s_keyMap[XK_Left]       = InputMap::ARROW_LEFT;
    s_keyMap[XK_Up]         = InputMap::ARROW_UP;
    s_keyMap[XK_Right]      = InputMap::ARROW_RIGHT;
    s_keyMap[XK_Down]       = InputMap::ARROW_DOWN;
    s_keyMap[XK_Print]      = InputMap::PRINT;
    s_keyMap[XK_Print]      = InputMap::PRINT_SCREEN;
    s_keyMap[XK_Insert]     = InputMap::INSERT;

    s_keyMap[XK_Delete]     = InputMap::DELETE;
    s_keyMap[XK_Help]       = InputMap::HELP;

    s_keyMap[XK_KP_0]       = InputMap::N_0;
    s_keyMap[XK_KP_1]       = InputMap::N_1;
    s_keyMap[XK_KP_2]       = InputMap::N_2;
    s_keyMap[XK_KP_3]       = InputMap::N_3;
    s_keyMap[XK_KP_4]       = InputMap::N_4;
    s_keyMap[XK_KP_5]       = InputMap::N_5;
    s_keyMap[XK_KP_6]       = InputMap::N_6;
    s_keyMap[XK_KP_7]       = InputMap::N_7;
    s_keyMap[XK_KP_8]       = InputMap::N_8;
    s_keyMap[XK_KP_9]       = InputMap::N_9;

    s_keyMap[XK_A]          = InputMap::A;
    s_keyMap[XK_B]          = InputMap::B;
    s_keyMap[XK_C]          = InputMap::C;
    s_keyMap[XK_D]          = InputMap::D;
    s_keyMap[XK_E]          = InputMap::E;
    s_keyMap[XK_F]          = InputMap::F;
    s_keyMap[XK_G]          = InputMap::G;
    s_keyMap[XK_H]          = InputMap::H;
    s_keyMap[XK_I]          = InputMap::I;
    s_keyMap[XK_J]          = InputMap::J;
    s_keyMap[XK_K]          = InputMap::K;
    s_keyMap[XK_L]          = InputMap::L;
    s_keyMap[XK_M]          = InputMap::M;
    s_keyMap[XK_N]          = InputMap::N;
    s_keyMap[XK_O]          = InputMap::O;
    s_keyMap[XK_P]          = InputMap::P;
    s_keyMap[XK_Q]          = InputMap::Q;
    s_keyMap[XK_R]          = InputMap::R;
    s_keyMap[XK_S]          = InputMap::S;
    s_keyMap[XK_T]          = InputMap::T;
    s_keyMap[XK_U]          = InputMap::U;
    s_keyMap[XK_V]          = InputMap::V;
    s_keyMap[XK_W]          = InputMap::W;
    s_keyMap[XK_X]          = InputMap::X;
    s_keyMap[XK_Y]          = InputMap::Y;
    s_keyMap[XK_Z]          = InputMap::Z;

    s_keyMap[XK_a]          = InputMap::A;
    s_keyMap[XK_b]          = InputMap::B;
    s_keyMap[XK_c]          = InputMap::C;
    s_keyMap[XK_d]          = InputMap::D;
    s_keyMap[XK_e]          = InputMap::E;
    s_keyMap[XK_f]          = InputMap::F;
    s_keyMap[XK_g]          = InputMap::G;
    s_keyMap[XK_h]          = InputMap::H;
    s_keyMap[XK_i]          = InputMap::I;
    s_keyMap[XK_j]          = InputMap::J;
    s_keyMap[XK_k]          = InputMap::K;
    s_keyMap[XK_l]          = InputMap::L;
    s_keyMap[XK_m]          = InputMap::M;
    s_keyMap[XK_n]          = InputMap::N;
    s_keyMap[XK_o]          = InputMap::O;
    s_keyMap[XK_p]          = InputMap::P;
    s_keyMap[XK_q]          = InputMap::Q;
    s_keyMap[XK_r]          = InputMap::R;
    s_keyMap[XK_s]          = InputMap::S;
    s_keyMap[XK_t]          = InputMap::T;
    s_keyMap[XK_u]          = InputMap::U;
    s_keyMap[XK_v]          = InputMap::V;
    s_keyMap[XK_w]          = InputMap::W;
    s_keyMap[XK_x]          = InputMap::X;
    s_keyMap[XK_y]          = InputMap::Y;
    s_keyMap[XK_z]          = InputMap::Z;

    s_keyMap[XK_Super_L]      = InputMap::LEFT_WINKEY;
    s_keyMap[XK_Super_R]      = InputMap::RIGHT_WINKEY;

    s_keyMap[XK_KP_0]         = InputMap::NUMPAD_0;
    s_keyMap[XK_KP_1]         = InputMap::NUMPAD_1;
    s_keyMap[XK_KP_2]         = InputMap::NUMPAD_2;
    s_keyMap[XK_KP_3]         = InputMap::NUMPAD_3;
    s_keyMap[XK_KP_4]         = InputMap::NUMPAD_4;
    s_keyMap[XK_KP_5]         = InputMap::NUMPAD_5;
    s_keyMap[XK_KP_6]         = InputMap::NUMPAD_6;
    s_keyMap[XK_KP_7]         = InputMap::NUMPAD_7;
    s_keyMap[XK_KP_8]         = InputMap::NUMPAD_8;
    s_keyMap[XK_KP_9]         = InputMap::NUMPAD_9;
    s_keyMap[XK_KP_Multiply]  = InputMap::NUMPAD_MULTIPLY;
    s_keyMap[XK_KP_Add]       = InputMap::NUMPAD_ADD;
    s_keyMap[XK_KP_Enter]     = InputMap::NUMPAD_ENTER;
    s_keyMap[XK_KP_Subtract]  = InputMap::NUMPAD_SUBTRACT;
    s_keyMap[XK_KP_Decimal]   = InputMap::NUMPAD_DECIMAL;
    s_keyMap[XK_KP_Divide]    = InputMap::NUMPAD_DIVIDE;
    s_keyMap[XK_F1]           = InputMap::F1;
    s_keyMap[XK_F2]           = InputMap::F2;
    s_keyMap[XK_F3]           = InputMap::F3;
    s_keyMap[XK_F4]           = InputMap::F4;
    s_keyMap[XK_F5]           = InputMap::F5;
    s_keyMap[XK_F6]           = InputMap::F6;
    s_keyMap[XK_F7]           = InputMap::F7;
    s_keyMap[XK_F8]           = InputMap::F8;
    s_keyMap[XK_F9]           = InputMap::F9;
    s_keyMap[XK_F10]          = InputMap::F10;
    s_keyMap[XK_F11]          = InputMap::F11;
    s_keyMap[XK_F12]          = InputMap::F12;
    s_keyMap[XK_F13]          = InputMap::F13;
    s_keyMap[XK_F14]          = InputMap::F14;
    s_keyMap[XK_F15]          = InputMap::F15;

    s_keyMap[XK_Num_Lock]     = InputMap::NUM_LOCK;
    s_keyMap[XK_Scroll_Lock]  = InputMap::SCROLL_LOCK;

    s_keyMap[XK_Shift_L]      = InputMap::LEFT_SHIFT;
    s_keyMap[XK_Shift_R]      = InputMap::RIGHT_SHIFT;
    s_keyMap[XK_Control_L]    = InputMap::LEFT_CONTROL;
    s_keyMap[XK_Control_R]    = InputMap::RIGHT_CONTROL;

    s_keyMap[XK_Alt_L]        = InputMap::LEFT_ALT;
    s_keyMap[XK_Alt_R]        = InputMap::RIGHT_ALT;

    s_keyMap[XK_semicolon]    = InputMap::SEMICOLON;     // ';:' for US
    s_keyMap[XK_equal]        = InputMap::EQUALS;        // '+' any country
    s_keyMap[XK_comma]        = InputMap::COMMA;         // ',' any country
    s_keyMap[XK_minus]        = InputMap::MINUS;         // '-' any country
    s_keyMap[XK_period]       = InputMap::PERIOD;        // '.' any country
    s_keyMap[XK_slash]        = InputMap::SLASH;         // '/?' for US
    s_keyMap[XK_grave]        = InputMap::BACKQUOTE;     // '`~' for US
    s_keyMap[XK_bracketleft]  = InputMap::LEFT_BRACKET;  //  '[{' for US
    s_keyMap[XK_backslash]    = InputMap::BACKSLASH;     //  '\|' for US
    s_keyMap[XK_bracketright] = InputMap::RIGHT_BRACKET; //  ']}' for US
    s_keyMap[XK_apostrophe]   = InputMap::APOSTROPHE;    //  ''"' for US
  }

  int InputEngine::MapSysKeyToInputMap( int syskey )
  {
    if( syskey > 136 )
      return 0;

    return s_keyMap[syskey];
  }

} // Engine namespace
} // SF namespace

#endif // __linux
