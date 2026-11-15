/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Window_Win32.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/09/2009 @ 14:12:27 - nwilliam
**  \brief   Implementation of win32 system window class.
*******************************************************************************/

#ifdef _WIN32

#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

// Discard Win95 by defining 0x0501 for WM_MOUSEWHEEL messages
#ifndef WINVER
  #define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS
  #define _WIN32_WINDOWS 0x0501
#endif

#include <windows.h>

#ifndef MAPVK_VK_TO_VSC
  #define MAPVK_VK_TO_VSC     0
  #define MAPVK_VSC_TO_VK     1
  #define MAPVK_VK_TO_CHAR    2
  #define MAPVK_VSC_TO_VK_EX  3
#endif

#include <cctype>  // isprint
#include <cstring> // memset

#include "Window.h"
#include "Engine/InputEngine.h"
#include "SolarFlare/Globals.h"   // messageEngine
#include "Engine/MessageEngine.h"

#include "Utilities/LeakCheck.h"

namespace
{
  using namespace SF;
  using namespace Engine;

  int ScanCodeFromLPARAM( LPARAM lp )
  {
    // LPARAM layout, bits 16-23 are the scancode
    int scan = static_cast<int>( lp >> 16 );
    scan &= 0x000000FF;
    return scan;
  }

  int GetKeyVal( WPARAM wp, LPARAM lp )
  {
    // Note: Will need to trap alt keys and print screen to handle them

    // The keys in the switch get lost in scancode conversion - so don't do it
    switch( wp )
    {
      // MSDN says bits 25-28 is "reserved". 26 distinguishes numpad enter
      case VK_RETURN:    return ( lp & 0x03000000 ) ? InputMap::NUMPAD_ENTER : InputMap::ENTER;

      // If 24th bit is set it is right side, otherwise not
      case VK_CONTROL:   return ( lp & 0x01000000 ) ? InputMap::RIGHT_CONTROL : InputMap::LEFT_CONTROL;

      case VK_PAUSE:     return InputMap::PAUSE;
      case VK_SNAPSHOT:  return InputMap::PRINT_SCREEN;
      case VK_LWIN:      return InputMap::LEFT_WINKEY;
      case VK_RWIN:      return InputMap::RIGHT_WINKEY;
      case VK_APPS:      return InputMap::MENU;
      case VK_NUMPAD0:   return InputMap::NUMPAD_0;
      case VK_NUMPAD1:   return InputMap::NUMPAD_1;
      case VK_NUMPAD2:   return InputMap::NUMPAD_2;
      case VK_NUMPAD3:   return InputMap::NUMPAD_3;
      case VK_NUMPAD4:   return InputMap::NUMPAD_4;
      case VK_NUMPAD5:   return InputMap::NUMPAD_5;
      case VK_NUMPAD6:   return InputMap::NUMPAD_6;
      case VK_NUMPAD7:   return InputMap::NUMPAD_7;
      case VK_NUMPAD8:   return InputMap::NUMPAD_8;
      case VK_NUMPAD9:   return InputMap::NUMPAD_9;
      case VK_MULTIPLY:  return InputMap::NUMPAD_MULTIPLY;
      case VK_ADD:       return InputMap::NUMPAD_ADD;
      case VK_SEPARATOR: return InputMap::NUMPAD_ENTER;
      case VK_SUBTRACT:  return InputMap::NUMPAD_SUBTRACT;
      case VK_DECIMAL:   return InputMap::NUMPAD_DECIMAL;
      case VK_DIVIDE:    return InputMap::NUMPAD_DIVIDE;
    }

    return  InputEngine::MapSysKeyToInputMap( ScanCodeFromLPARAM( lp ) );
  }

  // The evil hackery being done in WM_NCCREATE and WM_DESTROY to tell the
  // owning Window that it is getting destroyed creates these warnings.
  #pragma warning( push )
  #pragma warning( disable : 4244 ) // LONG_PTR to LONG, possible loss of data
  #pragma warning( disable : 4312 ) // Conversion from LONG to Window*

  LRESULT CALLBACK WinProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
  {
    static bool cursorShown = false;

    switch( msg )
    {
      case WM_NCCREATE:
        SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lp)->lpCreateParams );
        return TRUE;
      break;

      case WM_DESTROY:
      {
        Utilities::Window* window = (Utilities::Window*)GetWindowLongPtr( hwnd, GWLP_USERDATA );

        if( window )
          window->Destroy();

        PostQuitMessage( 0 );
      }
      break;

      case WM_CHAR:
      {
        // Note: If we ever do UTF and/or wide chars, this needs to be changed
        char c = static_cast< char >( wp );

        if( std::isprint( c ) )
        {
          Globals::input->TextInput( c );
        }
      }

      break;
      case WM_KEYDOWN:
      {
        Globals::input->SetPressed( GetKeyVal( wp, lp ), true );
      }
      break;

      case WM_KEYUP:
      {
        Globals::input->SetPressed( GetKeyVal( wp, lp ), false );
      }
      break;

      case WM_LBUTTONDOWN:
      case WM_MBUTTONDOWN:
      case WM_RBUTTONDOWN:
      {
        if( wp & MK_LBUTTON )
        {
          Globals::input->SetPressed( InputMap::MOUSE_LEFT, true );
        }

        if( wp & MK_MBUTTON )
        {
          Globals::input->SetPressed( InputMap::MOUSE_MIDDLE, true );
        }

        if( wp & MK_RBUTTON )
        {
          Globals::input->SetPressed( InputMap::MOUSE_RIGHT, true );
        }
      }
      break;

      case WM_LBUTTONUP:
        Globals::input->SetPressed( InputMap::MOUSE_LEFT, false );
      break;

      case WM_MBUTTONUP:
        Globals::input->SetPressed( InputMap::MOUSE_MIDDLE, false );
      break;

      case WM_RBUTTONUP:
        Globals::input->SetPressed( InputMap::MOUSE_RIGHT, false );
      break;

      case WM_MOUSEMOVE:
      {
        short x = LOWORD( lp );
        short y = HIWORD( lp );

        Globals::input->MouseX( x );
        Globals::input->MouseY( y );
      }
      break;

      case WM_MOUSEWHEEL:
      {
        #ifndef WHEEL_DELTA
          const int WHEEL_DELTA = 120;
        #endif

        // Positive spin, away from user, up
        int delta = static_cast<short>( HIWORD( wp ) ) / WHEEL_DELTA;
        int direction = delta > 0 ? InputMap::MOUSE_WHEEL_UP : InputMap::MOUSE_WHEEL_DOWN;

        Globals::input->WheelDelta( delta );
        Globals::input->SetTriggered( direction, true );
      }
      break;

      case WM_MOVE:
      {
        Utilities::Window* window = (Utilities::Window*)GetWindowLongPtr( hwnd, GWLP_USERDATA );

        if( window )
        {
          POINTS in = MAKEPOINTS( lp );

          RECT off = { 0, 0, 500, 500 };
          AdjustWindowRect( &off, GetWindowLong( hwnd, GWL_STYLE ), 0 );

          window->SetPosition( in.x + off.left, in.y + off.top );
        }
      }
      break;

      case WM_SIZE:
      {
        Utilities::Window* window = (Utilities::Window*)GetWindowLongPtr( hwnd, GWLP_USERDATA );

        int width = LOWORD( lp );
        int height = HIWORD( lp );

        if( window )
          window->Resize( width, height );

        Engine::Message* msg = Globals::messageEngine->AddMessage( "WindowResize" );
        msg->stack.Push( width );
        msg->stack.Push( height );
      }
      break;

      case WM_NCHITTEST:
      {
        int val = DefWindowProc( hwnd, msg, wp, lp );
        if( val != HTCLIENT )
        {
          if( !cursorShown )
          {
            cursorShown = true;
            ShowCursor(TRUE);
          }
        }
        else if( cursorShown )
        {
          cursorShown = false;
          ShowCursor(FALSE);
        }

        return val;
      }
      break;

      default:
        return DefWindowProc( hwnd, msg, wp, lp );
      break;
    }

    // Nearly all WM_* messages say "If application handles this message,
    // it should return 0". So we do.
    return 0;
  }
  #pragma warning( pop )

} // Unnamed namespace close




namespace SF {
namespace Utilities
{
  struct WinData_Priv
  {
    WinData_Priv() : atom( 0 ), handle( 0 ), style( 0 ), instance( 0 )
    {
      std::memset( &lastMsg, 0, sizeof( lastMsg ) );
      std::memset( &wndClass, 0, sizeof( wndClass ) );
    }

    ATOM atom;
    MSG lastMsg;
    HWND handle;
    DWORD style;
    HINSTANCE instance;
    WNDCLASSEX wndClass;
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
    return 1;  // HWND
  }

  void* Window::GetOSSpecific( int index )
  {
    if( index == 0 )
      return &m_data->handle;

    return 0;
  }

  void Window::ToggleFullscreen()
  {
    static HMENU lMenu;
    static DWORD lStyle;
    static int lWidth, lHeight, lX, lY;

    m_fullscreen = !m_fullscreen;

    if( m_fullscreen )
    {
      m_FSSwitch = true;

      lX = m_positionX;
      lY = m_positionY;
      lStyle = m_data->style;
      lWidth = m_width;
      lHeight = m_height;

      m_data->style = WS_POPUP | WS_VISIBLE;

      lMenu = GetMenu( m_data->handle );
      SetMenu( m_data->handle, NULL);

      SetWindowLong( m_data->handle, GWL_STYLE, m_data->style );

      DEVMODE dms;
      EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &dms );

      SetPosition( 0, 0 );
      m_positionX = m_positionY = 0;
      SetWindowPos( m_data->handle, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE );
      Resize( dms.dmPelsWidth, dms.dmPelsHeight);

      m_FSSwitch = false;
    }
    else
    {
      m_FSSwitch = true;

      m_data->style = lStyle;
      SetMenu( m_data->handle, lMenu );
      SetWindowLong( m_data->handle, GWL_STYLE, m_data->style );
      m_positionX = lX;
      m_positionY = lY;
      SetWindowPos( m_data->handle, HWND_TOP, lX, lY, 0, 0, SWP_NOSIZE );
      Resize( lWidth, lHeight );

      m_FSSwitch = false;
    }
  }

  void Window::MouseCentered( bool set )
  {
    m_mouseCentered = set;

    if( m_mouseCentered )
    {
      ShowCursor( false );
      Globals::input->MouseX( m_hw ); Globals::input->MouseDX( 0 );
      Globals::input->MouseY( m_hh ); Globals::input->MouseDY( 0 );
    }
    else
    {
      ShowCursor( true );
    }
  }

  void Window::Title( const std::string& title )
  {
    m_title = title;
    SetWindowText( m_data->handle, m_title.c_str() );
  }

  bool Window::Update()
  {
    while( PeekMessage( &m_data->lastMsg, NULL, 0, 0, PM_REMOVE ) )
    {
      TranslateMessage( &m_data->lastMsg );
      DispatchMessage( &m_data->lastMsg );
    }

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

        Globals::input->MouseDX( dx );
        Globals::input->MouseDY( dy );
      }
    }

    // Handle gets set to 0 on destruction of window
    return m_data->handle != 0;
  }

  void Window::Destroy()
  {
    m_data->handle = 0;
    m_data->instance = 0;

    if( !UnregisterClass( MAKEINTATOM( m_data->atom ), m_data->instance ) )
    {
      // "Could Not Unregister Class."
    }
  }

  void Window::SetPosition( int x, int y )
  {
    if( m_FSSwitch )
      return;

    if( m_positionX != x || m_positionY != y )
    {
      m_positionX = x;
      m_positionY = y;

      SetWindowPos( m_data->handle, HWND_TOP, x, y, 0, 0, SWP_NOSIZE );
    }
  }

  void Window::Resize( int width, int height )
  {
    if( !m_data->handle )
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
    }
  }

  void Window::Sleep( float /*milliseconds*/ )
  {
    SwitchToThread();
    //static HANDLE timer = CreateWaitableTimer( NULL, FALSE, NULL );
    //static LARGE_INTEGER waitTime;

    //// Convert it to a number of 100 second intervals
    //// Make it negative (positive indicates absolute time, negative is relative)
    //waitTime.QuadPart = static_cast<LONGLONG>( milliseconds * -10000 );

    //// Give up the rest of the frame.
    //if( !SetWaitableTimer( timer, &waitTime, 0, 0, 0, FALSE ) )
    //{
    //  // Function failed, so just return
    //  return;
    //}

    //WaitForSingleObject( timer, INFINITE );
    ////MsgWaitForMultipleObjects( 1, &timer, FALSE, INFINITE, QS_ALLINPUT );
  }

  void Window::SystemMessageBox( const char* title, const char* message )
  {
    MessageBox( NULL, message, title, MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL | MB_SETFOREGROUND );
  }




  //////////////////////////////////////////////////////////////////////////////
  // Private Below
  //////////////////////////////////////////////////////////////////////////////
  bool Window::Create()
  {
    m_data = new WinData_Priv;

    if( !m_data )
      return false;

    m_data->wndClass.cbSize = sizeof( WNDCLASSEX );
    m_data->wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    m_data->wndClass.lpfnWndProc = WinProc;
    m_data->wndClass.cbClsExtra = 0;
    m_data->wndClass.cbWndExtra = 0;
    m_data->wndClass.hInstance = m_data->instance;
    m_data->wndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    m_data->wndClass.hCursor = LoadCursor( 0, IDC_ARROW );
    m_data->wndClass.hbrBackground = 0;
    m_data->wndClass.lpszMenuName = 0;
    m_data->wndClass.lpszClassName = "Window_Win32";
    m_data->wndClass.hIconSm = 0;

    m_data->atom = RegisterClassEx( &m_data->wndClass );

    if( !m_data->atom )
      return false;

    DEVMODE dms;
    EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &dms );

    if( m_width == 0 || m_height == 0 )
    {
      m_width = static_cast<int>( dms.dmPelsWidth * 0.8 );
      m_height = static_cast<int>( dms.dmPelsHeight * 0.8 );

      m_hw = m_width / 2;
      m_hh = m_height / 2;
    }

    RECT size = { 0, 0, m_width, m_height };

    m_data->style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;

    AdjustWindowRect( &size, m_data->style, 0 );

    int winWidth = size.right - size.left;
    int winHeight = size.bottom - size.top;

    int x = (dms.dmPelsWidth / 2) - (winWidth / 2);
    int y = (dms.dmPelsHeight / 2) - (winHeight / 2);

    m_data->handle = CreateWindow( MAKEINTATOM( m_data->atom ),   // Atom, unique id
                                   m_title.c_str(),               // Window Title
                                   m_data->style,                 // Defined Window Style
                                   x, y,                          // Window Position
                                   size.right - size.left,        // Window Width
                                   size.bottom - size.top,        // Window Height
                                   0,                             // No Parent Window
                                   0,                             // No Menu
                                   m_data->instance,
                                   this );                        // Pass pointer To WM_CREATE, for Destroying

    // For some reason passing the title to CreateWindow() isn't showing it
    // So...this for now
    this->Title( m_title );

    ShowCursor( FALSE );

    return true;
  }

} // Utilities namespace
} // SF namespace

#endif //_WIN32
