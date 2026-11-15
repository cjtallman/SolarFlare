/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Timer_Linux.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/12/2009 @ 11:56:48 - nwilliam
**  \brief   POSIX specific timer implementation
*******************************************************************************/

#ifdef __linux

#include "Timer.h"

namespace
{
  timespec SubtractTS( const timespec& time1, const timespec& time2 )
  {
    timespec diff;

    diff.tv_sec = time1.tv_sec - time2.tv_sec;

    if( time1.tv_nsec < time2.tv_nsec )
    {
      diff.tv_nsec = time1.tv_nsec + 1000000000L - time2.tv_nsec ;
      --diff.tv_sec;
    }
    else
    {
      diff.tv_nsec = time1.tv_nsec - time2.tv_nsec ;
    }

    return diff;
  }

  unsigned long long ToMilliseconds( const timespec& ts )
  {
    long long ms = 0;

    ms = ts.tv_sec * 1000;
    ms += (ts.tv_nsec * 1.0e-6);

    return ms;
  }

}


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
    clock_gettime( CLOCK_MONOTONIC, &m_startCount );

    m_lastCount = m_count = m_startCount;

    timespec res;
    clock_getres( CLOCK_MONOTONIC, &res );

    // Nanosecond to milisecond
    m_frequency = res.tv_nsec;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void Timer::Update( void )
  {
    if( m_state == Timer::RUNNING )
    {
      m_lastCount = m_count;

      clock_gettime( CLOCK_MONOTONIC, &m_count );

      timespec diff = SubtractTS( m_count, m_lastCount );

      m_lastElapsed = static_cast<float>( ToMilliseconds( diff ) ) * m_elapsedScale;
    }
  }

  float Timer::ElapsedSinceStart() const
  {
    timespec diff = SubtractTS( m_startCount, m_count );
    return static_cast<float>( ToMilliseconds( diff ) );
  }

} // Utilities namespace
} // SF namespace

#endif // __linux
