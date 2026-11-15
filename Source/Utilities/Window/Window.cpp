/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Window.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 00:38:04 - nwilliam
**  \brief   All common, system independent window functionality
*******************************************************************************/

#include "Window.h"

#if !defined( _WIN32 ) && !defined( __linux )
  #error "Window wrapper does not support this OS yes."
#endif

namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  Window::Window( const std::string& title, int width, int height ) : m_width( width ),
                                                                      m_height( height ),
                                                                      m_hw( width / 2 ),
                                                                      m_hh( height / 2 ),
                                                                      m_fullscreen( false ),
                                                                      m_title( title ),
                                                                      m_positionX( 0 ),
                                                                      m_positionY( 0 ),
                                                                      m_mouseCentered( false )
  {
    if( !Create() )
      throw( "Window creation failed." );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  int Window::Width() const
  {
    return m_width;
  }

  int Window::Height() const
  {
    return m_height;
  }

  void Window::Width( int width )
  {
    Resize( width, m_height );
  }

  void Window::Height( int height )
  {
    Resize( m_width, height );
  }

  const std::string& Window::Title() const
  {
    return m_title;
  }

  bool Window::IsFullscreen() const
  {
    return m_fullscreen;
  }

  bool Window::IsMouseCentered() const
  {
    return m_mouseCentered;
  }

  void Window::FullscreenOn( bool on )
  {
    if( on != m_fullscreen )
    {
      ToggleFullscreen();
    }
  }

} // Utilities namespace
} // SF namespace
