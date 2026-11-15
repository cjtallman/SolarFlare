/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    DevConsole.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/26/2009 @ 11:50:32 - nwilliam
**  \brief   Classic ~ based dev/debug console.
*******************************************************************************/

#ifndef SF_DEV_CONSOLE_H
#define SF_DEV_CONSOLE_H

#include <deque>
#include <string>
#include "Color.h"
#include "OutputStream.h"
#include "Script/ScriptHelper.h"

namespace SF {
namespace Utilities
{
  class DevConsole
  {
    static const int max_input = 150;
    static const int display_rows = 18;
    static const int max_input_history = 15;
    static const int max_output_history = 1000;

    class MsgOutput
    {
      public:
        MsgOutput() {}
        MsgOutput( const std::string& text, const Color& col ) : color(col),
                                                                 msg(text) { }

        Color color;
        std::string msg;
    };


    public:
      DevConsole();

      // Default destructor, copy, assignment ok

      void PopLeft();
      void PopRight();
      void PushChar( char c );

      void Toggle();
      bool IsActive();

      void ClearInput();
      void AcceptInput();

      void InputHistoryUp();
      void InputHistoryDown();

      void ScrollOutputUp();
      void ScrollOutputDown();

      void MoveCursor( int amount );

      void DrawAllMessages();

      std::ostream& Error();
      std::ostream& Debug();
      std::ostream& Warning();
      std::ostream& Message();

      void Error( const std::string& text );
      void Debug( const std::string& text );
      void Warning( const std::string& text );
      void Message( const std::string& text );

      void Error( const char * text ){Error(std::string(text));}
      void Debug( const char * text ){Debug(std::string(text));}
      void Warning( const char * text ){Warning(std::string(text));}
      void Message( const char * text ){Message(std::string(text));}

    private:
      typedef std::deque< MsgOutput > History;
      typedef std::deque< std::string > StringDeque;

      void SetInputToHistory();
      void PushMessage( const std::string& msg, const Color& color );

      int input_offset;
      int cursor_offset;
      int output_offset;
      int output_scroll_size;
      bool console_is_active;

      OutputStream m_stream;
      std::string input_line;
      History output_history;
      StringDeque input_history;

      LUA_DECLARE_CLASS( DevConsole )
      LUA_DECLARE_METHOD( LPopLeft )
      LUA_DECLARE_METHOD( LPopRight )
      LUA_DECLARE_METHOD( LPush )
      LUA_DECLARE_METHOD( LClear )
      LUA_DECLARE_METHOD( LToggle )
      LUA_DECLARE_METHOD( LAccept )
      LUA_DECLARE_METHOD( LInputHistoryUp )
      LUA_DECLARE_METHOD( LInputHistoryDown )
      LUA_DECLARE_METHOD( LScrollOutputUp )
      LUA_DECLARE_METHOD( LScrollOutputDown )
      LUA_DECLARE_METHOD( LMoveCursor )
      LUA_DECLARE_METHOD( LError )
      LUA_DECLARE_METHOD( LDebug )
      LUA_DECLARE_METHOD( LWarning )
      LUA_DECLARE_METHOD( LMessage )
  };

} // Utilities namespace
} // SF namespace

#endif // SF_DEV_CONSOLE_H
