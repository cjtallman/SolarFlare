/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    InputEngineKeyNames.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/10/2009 @ 16:28:24 - nwilliam
**  \brief   All possible keyboard keys and mouse buttons in portable enum
*******************************************************************************/

#ifndef SF_INPUT_ENGINE_INPUT_MAP_H
#define SF_INPUT_ENGINE_INPUT_MAP_H

namespace SF {
namespace Engine
{
  namespace InputMap
  {
    enum
    {
      INVALID       = 0,

      // Begin ASCII mapped keys
      BACKSPACE     = 8,
      TAB           = 9,
      CLEAR         = 12,
      RETURN        = 13,
      ENTER         = 13,
      PAUSE         = 19,
      ESCAPE        = 27,
      SPACE         = 32,
      APOSTROPHE    = 34, // I know ' is 39 and " is 34. Deal with it.
      COMMA         = 44,
      MINUS         = 45,
      PERIOD        = 46,
      SLASH         = 47,
      N_0           = 48,
      N_1           = 49,
      N_2           = 50,
      N_3           = 51,
      N_4           = 52,
      N_5           = 53,
      N_6           = 54,
      N_7           = 55,
      N_8           = 56,
      N_9           = 57,
      SEMICOLON     = 59,
      EQUALS        = 61,
      A             = 65,
      B             = 66,
      C             = 67,
      D             = 68,
      E             = 69,
      F             = 70,
      G             = 71,
      H             = 72,
      I             = 73,
      J             = 74,
      K             = 75,
      L             = 76,
      M             = 77,
      N             = 78,
      O             = 79,
      P             = 80,
      Q             = 81,
      R             = 82,
      S             = 83,
      T             = 84,
      U             = 85,
      V             = 86,
      W             = 87,
      X             = 88,
      Y             = 89,
      Z             = 90,
      LEFT_BRACKET  = 91,
      BACKSLASH     = 92,
      RIGHT_BRACKET = 93,
      BACKQUOTE     = 96,
      TILDE         = 96,
      // End ASCII mapped keys

      // These start of the next numbers is basically randomly chosen because:
      //  - Windows VK_ for these map to ASCII
      //  - SDLK_ examples are just random, leaving large gaps
      //  - X11_ starts right after ASCII (some, anyway)
      // So, here are ours

      // Skip up past lowercase ASCII (127), just to avoid potential problems

      // Numpad
      NUMPAD_0        = 97,
      NUMPAD_1        = 98,
      NUMPAD_2        = 99,
      NUMPAD_3        = 100,
      NUMPAD_4        = 101,
      NUMPAD_5        = 102,
      NUMPAD_6        = 103,
      NUMPAD_7        = 104,
      NUMPAD_8        = 105,
      NUMPAD_9        = 106,
      NUMPAD_DECIMAL  = 107,
      NUMPAD_DIVIDE   = 108,
      NUMPAD_MULTIPLY = 109,
      NUMPAD_SUBTRACT = 110,
      NUMPAD_ADD      = 111,
      NUMPAD_ENTER    = 112,
      NUMPAD_EQUALS   = 113,

      // Arrows
      ARROW_UP        = 114,
      ARROW_DOWN      = 115,
      ARROW_RIGHT     = 116,
      ARROW_LEFT      = 117,

      // Home/End pad area
      INSERT          = 118,
      HOME            = 119,
      END             = 120,
      PAGE_UP         = 121,
      PAGE_DOWN       = 122,

      #undef DELETE
      DELETE          = 123,

      // Function keys
      F1              = 124,
      F2              = 125,
      F3              = 126,
      F4              = 127,
      F5              = 128,
      F6              = 129,
      F7              = 130,
      F8              = 131,
      F9              = 132,
      F10             = 134,
      F11             = 135,
      F12             = 136,
      F13             = 137,
      F14             = 138,
      F15             = 139,

      // Key meaning/state modifiers
      CAPS_LOCK       = 140,
      NUM_LOCK        = 141,
      SCROLL_LOCK     = 142,
      LEFT_SHIFT      = 144,
      RIGHT_SHIFT     = 145,
      LEFT_CONTROL    = 146,
      RIGHT_CONTROL   = 147,
      LEFT_ALT        = 148,
      RIGHT_ALT       = 149,
      LEFT_META       = 150,
      RIGHT_META      = 151,

      LEFT_WINKEY     = 152,  // Left "Windows" key
      RIGHT_WINKEY    = 153,  // Right "Windows" key
      MODE            = 154,  // "Alt Gr" key
      COMPOSE         = 155,  // Multi-key compose key
      HELP            = 156,
      PRINT           = 157,
      PRINT_SCREEN    = 158,
      SYSREQ          = 159,
      BREAK           = 160,
      MENU            = 161,

      MOUSE_LEFT       = 162,
      MOUSE_RIGHT      = 163,
      MOUSE_MIDDLE     = 164,
      MOUSE_WHEEL_UP   = 165,
      MOUSE_WHEEL_DOWN = 166,

      LAST
    };
  }

} // Engine namespace
} // SF namespace

#endif // SF_INPUT_ENGINE_INPUT_MAP_H
