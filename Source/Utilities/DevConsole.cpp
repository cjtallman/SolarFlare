/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    DevConsole.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/26/2009 @ 11:50:38 - nwilliam
**  \brief   Implementation of developer console.
*******************************************************************************/

#include "DevConsole.h"
#include "Utilities/Misc.h"
#include "SolarFlare/Globals.h"       // scriptEngine
#include "Script/ScriptEngine.h"
#include "Graphics/TextEngine.h"
#include "Graphics/GraphicsEngine.h"

namespace SF {
namespace Utilities
{
  namespace
  {
    const Color DebugColor( 0, 255, 255, 200 );
    const Color WarningColor( 255, 255, 0, 200 );
    const Color ErrorColor( 255, 0, 0, 200 );
    const Color NormalColor( 255, 255, 255, 200 );

    StreamBuffer::SyncCallback s_errorDel;
    StreamBuffer::SyncCallback s_debugDel;
    StreamBuffer::SyncCallback s_warningDel;
    StreamBuffer::SyncCallback s_messageDel;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DevConsole
  //////////////////////////////////////////////////////////////////////////////
  DevConsole::DevConsole() : input_offset( 0 ),
                             cursor_offset( 0 ),
                             output_offset( 0 ),
                             output_scroll_size( DevConsole::display_rows - 3 ),
                             console_is_active( false )
  {
    // Nothing to see here..
    // .. YOU try creating a pointer to an overloaded member function cleaner!
    s_errorDel = StreamBuffer::SyncCallback::Method< DevConsole, static_cast<void (DevConsole::*)(const char*)>(&DevConsole::Error) >( this );
    s_debugDel = StreamBuffer::SyncCallback::Method< DevConsole, static_cast<void (DevConsole::*)(const char*)>(&DevConsole::Debug) >( this );
    s_warningDel = StreamBuffer::SyncCallback::Method< DevConsole, static_cast<void (DevConsole::*)(const char*)>(&DevConsole::Warning) >( this );
    s_messageDel = StreamBuffer::SyncCallback::Method< DevConsole, static_cast<void (DevConsole::*)(const char*)>(&DevConsole::Message) >( this );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void DevConsole::PopLeft()
  {
    int offset = int( input_line.size() ) - cursor_offset - 1;

    if( !input_line.empty() && offset >= 0 )
      input_line.erase( offset, 1 );
  }

  void DevConsole::PopRight()
  {
    int offset = int( input_line.size() ) - cursor_offset;
    MoveCursor( 1 );

    if( !input_line.empty() && offset >= 0 )
      input_line.erase( offset, 1 );
  }

  void DevConsole::PushChar(char c)
  {
    if( input_line.size() < max_input )
      input_line.insert( input_line.end() - cursor_offset, c );
  }

  void DevConsole::Toggle()
  {
    console_is_active = !console_is_active;
  }

  bool DevConsole::IsActive()
  {
    return console_is_active;
  }

  void DevConsole::ClearInput()
  {
    input_line.clear();
  }

  void DevConsole::AcceptInput()
  {
    if( !input_line.empty() )
    {
      Message(input_line);
      
      input_history.push_front( input_line );
      
      if( input_history.size() > max_input_history )
        input_history.pop_back();
      
      ClearInput();
      
      input_offset = 0;
      cursor_offset = 0;
    }
  }

  void DevConsole::InputHistoryUp()
  {
    input_offset = Utilities::Min( input_offset + 1, int( input_history.size() ) );
    SetInputToHistory();
  }

  void DevConsole::InputHistoryDown()
  {
    input_offset = Utilities::Max( input_offset - 1, 0 );
    SetInputToHistory();
  }

  void DevConsole::ScrollOutputUp()
  {
    output_offset = Utilities::Min( output_offset + output_scroll_size, (int)output_history.size() );
  }

  void DevConsole::ScrollOutputDown()
  {
    output_offset = Utilities::Max( output_offset - output_scroll_size, 0 );
  }

  void DevConsole::MoveCursor( int amount )
  {
    cursor_offset = Utilities::Clamp( cursor_offset - amount, 0, (int)input_line.size() );
  }

  void DevConsole::DrawAllMessages()
  {
    static const std::string cursor_string("|");
    static const Color text_color( 255, 255, 255, 200 );

    if( console_is_active == false )
      return;

    Graphics::TextEngine* text_engine = Graphics::TextEngine::Instance();
    int width = Globals::graphicsEngine->WindowWidth();

    float y = 0.0f;
    float h = 14.0f;
    float height = display_rows * h - h;

    Enable2D();

    //Draw the semi-transparent windowpane
    glBegin( GL_QUADS );
    glColor4ub( 50, 50, 50, 220 );
    glVertex2i( 0, 0 );
    glVertex2i( 0, int(height) );
    glVertex2i( width, int(height) );
    glVertex2i( width, 0 );
    glEnd();

    //Draw the line between active and inactive text
    glBegin( GL_LINES );
    glColor4ub( 150, 150, 150, 128 );
    glVertex2i( 0, int(height - h) );
    glVertex2i( width, int(height - h) );
    glEnd();

    //Draw the active input line
    if( !input_line.empty() )
      text_engine->PrintTextAdv( 0, height - h, input_line, text_color, h );

    //Draw the cursor/caret
    std::string substring = input_line.substr( 0, input_line.size() - cursor_offset );
    float str_width = text_engine->GetWidth( substring, h ) - 3;
    text_engine->PrintTextAdv( str_width, height - h, cursor_string, text_color, h );

    //Draw all messages
    int last = Utilities::Min( output_offset + display_rows - 1, int( output_history.size() ) );

    History::iterator it = output_history.begin() + output_offset;
    History::iterator end = output_history.begin() + last;

    while( it != end )
    {
      text_engine->PrintTextAdv( 0, height - y - h - h, it->msg, it->color, h );
      y += h;
      ++it;
    }

    Disable2D();
  }

  std::ostream& DevConsole::Error()
  {
    m_stream.SetSyncCallback( s_errorDel );
    return m_stream;
  }

  std::ostream& DevConsole::Debug()
  {
    m_stream.SetSyncCallback( s_debugDel );
    return m_stream;
  }

  std::ostream& DevConsole::Warning()
  {
    m_stream.SetSyncCallback( s_warningDel );
    return m_stream;
  }

  std::ostream& DevConsole::Message()
  {
    m_stream.SetSyncCallback( s_messageDel );
    return m_stream;
  }

  void DevConsole::Error( const std::string& text )
  {
    PushMessage( text, ErrorColor );
  }

  void DevConsole::Debug( const std::string& text )
  {
    PushMessage( text, DebugColor );
  }

  void DevConsole::Warning( const std::string& text )
  {
    PushMessage( text, WarningColor );
  }

  void DevConsole::Message( const std::string& text )
  {
    PushMessage( text, NormalColor );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void DevConsole::SetInputToHistory()
  {
    cursor_offset = 0;
    
    if( input_offset )
      input_line = input_history[input_offset - 1];
    else
      input_line = std::string();
  }

  void DevConsole::PushMessage( const std::string& msg, const Color& color )
  {
    std::string copy( msg );
    
    while( copy.size() > DevConsole::max_input )
    {
      size_t pos = DevConsole::max_input;

      while( pos && copy[pos] != ' ' )
      {
        --pos;
      }

      pos = Utilities::Max( pos, 4u );

      std::string temp = copy.substr( 0, pos );
      output_history.push_front( MsgOutput( temp, color ) );
      copy = std::string("  ") + copy.substr( pos );
    }
    
    output_history.push_front( MsgOutput( copy, color ) );
    
    if( output_history.size() > max_output_history )
      output_history.pop_back();
  }




  //////////////////////////////////////////////////////////////////////////////
  // Lua Methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( DevConsole )
    LUA_NAME_METHOD( DevConsole, "PopLeft",           LPopLeft )
    LUA_NAME_METHOD( DevConsole, "PopRight",          LPopRight )
    LUA_NAME_METHOD( DevConsole, "Push",              LPush )
    LUA_NAME_METHOD( DevConsole, "Clear",             LClear )
    LUA_NAME_METHOD( DevConsole, "Toggle",            LToggle )
    LUA_NAME_METHOD( DevConsole, "Accept",            LAccept )
    LUA_NAME_METHOD( DevConsole, "InputHistoryUp",    LInputHistoryUp )
    LUA_NAME_METHOD( DevConsole, "InputHistoryDown",  LInputHistoryDown )
    LUA_NAME_METHOD( DevConsole, "ScrollOutputUp",    LScrollOutputUp )
    LUA_NAME_METHOD( DevConsole, "ScrollOutputDown",  LScrollOutputDown )
    LUA_NAME_METHOD( DevConsole, "MoveCursor",        LMoveCursor )
    LUA_NAME_METHOD( DevConsole, "Error",   LError )
    LUA_NAME_METHOD( DevConsole, "Debug",   LDebug )
    LUA_NAME_METHOD( DevConsole, "Warning", LWarning )
    LUA_NAME_METHOD( DevConsole, "Message", LMessage )
  LUA_END_DEFINE_CLASS


  LUA_DEFINE_METHOD_NOLS( DevConsole, LPopLeft )
  {
    PopLeft();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( DevConsole, LPopRight )
  {
    PopRight();
    return 0;
  }

  LUA_DEFINE_METHOD( DevConsole, LPush )
  {
    std::string str = luaL_checkstring( ls, -1 );
    
    for( std::string::iterator it = str.begin(); it != str.end(); ++it )
      PushChar( *it );
    
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( DevConsole, LClear )
  {
    ClearInput();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( DevConsole, LToggle )
  {
    Toggle();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( DevConsole, LAccept )
  {
    std::string str = input_line;
    AcceptInput();
    
    Globals::scriptEngine->DoString( 0, str.c_str() );
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( DevConsole, LInputHistoryUp )
  {
    InputHistoryUp();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( DevConsole, LInputHistoryDown )
  {
    InputHistoryDown();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( DevConsole, LScrollOutputUp )
  {
    ScrollOutputUp();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( DevConsole, LScrollOutputDown )
  {
    ScrollOutputDown();
    return 0;
  }

  LUA_DEFINE_METHOD( DevConsole, LMoveCursor )
  {
    int amount = luaL_checkint( ls, 1 );
    MoveCursor( amount );
    return 0;
  }

  LUA_DEFINE_METHOD( DevConsole, LError )
  {
    std::string str = luaL_checkstring( ls, -1 );
    Error( str );
    return 0;
  }

  LUA_DEFINE_METHOD( DevConsole, LDebug )
  {
    std::string str = luaL_checkstring( ls, -1 );
    Debug( str );
    return 0;
  }

  LUA_DEFINE_METHOD( DevConsole, LWarning )
  {
    std::string str = luaL_checkstring( ls, -1 );
    Warning( str );
    return 0;
  }

  LUA_DEFINE_METHOD( DevConsole, LMessage )
  {
    std::string str = luaL_checkstring( ls, -1 );
    Message( str );
    return 0;
  }

} // Utilities namespace
} // SF namespace
