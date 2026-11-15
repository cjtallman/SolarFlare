/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Window.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 00:38:09 - nwilliam
**  \brief   Portable wrapper around systems specific window implementations.
*******************************************************************************/

#ifndef SF_WINDOW_H
#define SF_WINDOW_H

#include <string>

namespace SF {
namespace Utilities
{
  // Forward declaration
  struct WinData_Priv;


  class Window
  {
    public:
      // Common (Window.cpp)
      Window( const std::string& title, int width = 0 , int height = 0 );

      int Width() const;
      int Height() const;
      void Width( int width );
      void Height( int height );
      const std::string& Title() const;

      bool IsFullscreen() const;
      void FullscreenOn( bool on );
      bool IsMouseCentered() const;

      // OS specific (Window_<OSName>.cpp)
      ~Window();
      int GetOSSpecificCount() const;
      void* GetOSSpecific( int index );

      void ToggleFullscreen();
      void MouseCentered( bool set );
      void Title( const std::string& title );

      bool Update();
      void Destroy();
      void SetPosition( int x, int y );
      void Sleep( float milliseconds );
      void Resize( int width, int height );

      static void SystemMessageBox( const char* title , const char* message );


    private:
      // No copy, assignment
      Window( const Window& );
      Window& operator=( const Window& );

      // Common
      int m_positionX;
      int m_positionY;
      bool m_FSSwitch;

      bool m_fullscreen;
      int m_width, m_hw;
      int m_height, m_hh;
      std::string m_title;
      bool m_mouseCentered;


      // OS specific (Window_<OSName>.cpp)
      bool Create();
      WinData_Priv* m_data;
  };

} // Utilities namespace
} // SF namespace

#endif // SF_WINDOW_H
