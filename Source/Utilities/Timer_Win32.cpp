/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Timer_Win32.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/12/2009 @ 11:55:47 - nwilliam
**  \brief   Windows specific timer implementation
*******************************************************************************/

#ifdef _WIN32

#include "Timer.h"

namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor
  //////////////////////////////////////////////////////////////////////////////

  Timer::Timer() : m_state( Timer::RUNNING ),
                   m_lastElapsed( 0.0f ),
                   m_frequency( 0.0f ),
                   m_elapsedScale( 1.0f )
  {
    LARGE_INTEGER count;
    LARGE_INTEGER freq;
    
    QueryPerformanceCounter( &count );
    
    QueryPerformanceFrequency( &freq );
    
    m_startCount = m_lastCount = m_count = count.QuadPart;
    
    m_frequency = 1.0f / static_cast<float>( freq.QuadPart );
    m_frequency *= 1000; // Scale to milliseconds
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void Timer::Update( void )
  {
    if( m_state == Timer::RUNNING )
    {
      m_lastCount = m_count;
      
      LARGE_INTEGER count;
      QueryPerformanceCounter( &count );
      m_count = count.QuadPart;
      
      m_lastElapsed = ( m_count - m_lastCount ) * m_frequency * m_elapsedScale;
    }
  }
    
  float Timer::ElapsedSinceStart() const
  {
    return ( m_count - m_startCount ) * m_frequency;
  }

} // Utilities namespace
} // SF namespace

#endif // _WIN32
