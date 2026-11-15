/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    main.cpp
**  \author  Scorched Muffin Entertainment
**  \par     Course: GAM300/350
**  \par     Updated: 03/17/2009 @ 23:33:32 - nwilliam
**  \brief   Entry point for the application.
*******************************************************************************/

#define SF_BUILD_LEAK_CHECK

#include "Game.h"
#include "Globals.h"
#include "Utilities/LeakCheck.h"
#include "Utilities/Window/Window.h"  // SystemMessageBox

#include "Utilities/LeakCheck.h"

#ifdef _WIN32
  #include <windows.h>
  int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
#else
  int main( int argc, const char* argv[] )
#endif
{
  #if defined(_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  #endif

  const char* title = "Exception!";

  try
  {
    SF::Globals::Init();
    SF::Game game;

    game.Run();
    SF::Globals::Destroy();

    return 0;
  }
  catch( const char* text )
  {
    SF::Utilities::Window::SystemMessageBox( title, text );
  }
  catch( const std::string& str )
  {
    SF::Utilities::Window::SystemMessageBox( title, str.c_str() );
  }
  catch( const std::exception& e )
  {
    SF::Utilities::Window::SystemMessageBox( title, e.what() );
  }
  catch( ... )
  {
    SF::Utilities::Window::SystemMessageBox( title, "Unhandled exception of unknown type." );
  }

  return -1;
}
