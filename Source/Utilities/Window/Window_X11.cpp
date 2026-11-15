/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Window_Win32.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/11/2009 @ 18:43:27 - nwilliam
**  \brief   Implementation of Linux/X11 system window class.
*******************************************************************************/

#ifdef __linux

#include <time.h> // nanosleep
#include <cstdlib> // system

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "Window.h"
#include "Engine/InputEngine.h"
#include "SolarFlare/Globals.h"   // messageEngine
#include "Engine/MessageEngine.h"

namespace
{


} // Unnamed namespace close




namespace SF {
namespace Utilities
{
  struct WinData_Priv
  {
    WinData_Priv() : window( 0 ), display( 0 ), context( 0 )
    {
    }

    ::Window window;
    Display* display;
    GLXContext context;
  };




  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  Window::~Window()
  {
    Destroy();
    delete m_data;
  }

  int Window::GetOSSpecificCount() const
  {
    return 2;
  }

  void* Window::GetOSSpecific( int index )
  {
    if( index == 0 )
      return &m_data->display;
    else if( index == 1 )
      return &m_data->window;

    return 0;
  }

  void Window::ToggleFullscreen()
  {
    #warning "Implement this"
  }

  void Window::MouseCentered( bool set )
  {
    m_mouseCentered = set;

    #warning "Implement this"
  }

  void Window::Title( const std::string& title )
  {
    m_title = title;
    #warning "Implement this"
  }

  bool Window::Update()
  {
    const int mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask;
    XEvent event;

    while( XCheckWindowEvent( m_data->display, m_data->window, mask, &event ) )
    {
      if( event.xany.window == m_data->window )
      {
        switch( event.type )
        {
          case ConfigureNotify:
          {
            m_width = event.xconfigure.width;
            m_height = event.xconfigure.height;

            SF::Engine::Message* msg = SF::Globals::messageEngine->AddMessage( "WindowResize" );
            msg->stack.Push( m_width );
            msg->stack.Push( m_height );
          }
          break;

          case KeyPress:
          {
            int key = SF::Engine::InputEngine::MapSysKeyToInputMap( XLookupKeysym( &event.xkey, 0 ) );
            Globals::input->SetKeyPressed( key, true );
          }
          break;

          case KeyRelease:
          {
            int key = SF::Engine::InputEngine::MapSysKeyToInputMap( XLookupKeysym( &event.xkey, 0 ) );
            Globals::input->SetKeyPressed( key, false );
          }
          break;
        }
      }
    }

    // TODO: mouse centering on linux
    /*
    if( m_mouseCentered )
    {
      POINT cp;
      GetCursorPos( &cp );
      MapWindowPoints( HWND_DESKTOP, m_data->handle, &cp, 1 );

      if( cp.x != m_hw || cp.y != m_hh )
      {
        int dx = m_hw - cp.x;
        int dy = m_hh - cp.y;

        cp.x = m_hw;
        cp.y = m_hh;
        MapWindowPoints( m_data->handle, HWND_DESKTOP, &cp, 1 );

        SetCursorPos( cp.x, cp.y );

        Globals::input->MouseDeltaX( dx );
        Globals::input->MouseDeltaY( dy );
      }
    }*/

    return m_data->display != 0;
  }

  void Window::Destroy()
  {
    glXDestroyContext( m_data->display, m_data->context);
    XDestroyWindow( m_data->display, m_data->window);
    XCloseDisplay( m_data->display );

    m_data->window = 0;
    m_data->display = 0;
    m_data->context = 0;
  }

  void Window::SetPosition( int x, int y )
  {
    if( m_FSSwitch )
      return;

    #warning "Implement this"
  }

  void Window::Resize( int width, int height )
  {
    #warning "IMplement this"
    /*if( !m_data->handle )
      return;

    RECT crect;
    GetClientRect( m_data->handle, &crect );

    if( crect.right != width || crect.bottom != height )
    {
      m_width = width;
      m_height = height;

      m_hw = width / 2;
      m_hh = height / 2;

      RECT size = { 0, 0, m_width, m_height };
      AdjustWindowRect( &size, m_data->style, 0 );

      SetWindowPos( m_data->handle, HWND_TOP, 0, 0, size.right - size.left, size.bottom - size.top, SWP_NOMOVE );
    }*/
  }

  void Window::Sleep( float milliseconds )
  {
    timespec amount;
    amount.tv_sec = 0;
    amount.tv_nsec = milliseconds * 1000000;

    timespec remaining;

    nanosleep( &amount, &remaining );
  }

  void Window::SystemMessageBox( const char* title, const char* message )
  {
    std::string msg( "xmessage -center \"" );
    msg += title;
    msg += ": ";
    msg += message;
    msg += "\"";

    system( msg.c_str() );
  }




  //////////////////////////////////////////////////////////////////////////////
  // Private Below
  //////////////////////////////////////////////////////////////////////////////
  bool Window::Create()
  {
    m_data = new WinData_Priv;

    m_data->display = XOpenDisplay( NULL );

    if( !m_data->display )
      return false; // Couldn't open default display

    int attrib[] = { GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER, None };

    int screenNum = DefaultScreen( m_data->display );
    ::Window rootWin = RootWindow( m_data->display, screenNum );

    XVisualInfo* visinfo = glXChooseVisual( m_data->display, screenNum, attrib );

    if( !visinfo )
      return false; // Couldn't get double buffered visual

    XSetWindowAttributes attr;

    /* window attributes */
    attr.background_pixel = 0;
    attr.border_pixel = 0;
    attr.colormap = XCreateColormap( m_data->display, rootWin, visinfo->visual, AllocNone );
    attr.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask;

    unsigned long mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

    m_data->window = XCreateWindow( m_data->display, rootWin, 0, 0, m_width, m_height, 0,
                                    visinfo->depth, InputOutput, visinfo->visual, mask, &attr );

    XSizeHints sizehints;
    sizehints.x = 0;
    sizehints.y = 0;
    sizehints.width  = m_width;
    sizehints.height = m_height;
    sizehints.flags = USSize | USPosition;

    XSetNormalHints( m_data->display, m_data->window, &sizehints );

    XSetStandardProperties( m_data->display, m_data->window, m_title.c_str(), m_title.c_str(),
                            None, static_cast<char **>( NULL ), 0, &sizehints );

    m_data->context = glXCreateContext( m_data->display, visinfo, NULL, True );

    if( !m_data->context )
      return false; // context creation failed

    XFree( visinfo );

    XMapWindow( m_data->display, m_data->window);
    glXMakeCurrent( m_data->display, m_data->window, m_data->context );

    return true;
  }

} // Utilities namespace
} // SF namespace

#endif //__linux
