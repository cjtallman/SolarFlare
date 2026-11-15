/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Timer.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/12/2008 @ 13:31:16 - nwilliam
**  \brief   Implementation of system independent timer functions
*******************************************************************************/

#include "Timer.h"

namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void Timer::Start()
  {
    m_state = Timer::RUNNING;
    Update();
  }

  void Timer::Stop()
  {
    m_state = Timer::STOPPED;
    m_lastElapsed = 0;
  }

  void Timer::Pause()
  {
    m_state = Timer::PAUSED;
    m_lastElapsed = 0;
  }

  void Timer::Unpause()
  {
    if( m_state == Timer::PAUSED )
    {
      m_state = Timer::RUNNING;
      Update();
      Update();
    }
  }

  void Timer::Reset()
  {
    m_state = Timer::RUNNING;
    Update();
    m_startCount = m_lastCount = m_count;
  }

  float Timer::Elapsed() const
  {
    return m_lastElapsed;
  }

  float Timer::Frequency() const
  {
    return m_frequency;
  }

  float Timer::ElapsedScale() const
  {
    return m_elapsedScale;
  }

  void Timer::DeltaElapsedScale( float delta )
  {
    m_elapsedScale += delta;
  }

  void Timer::ElapsedScale( float scale )
  {
    m_elapsedScale = scale;
  }

  bool Timer::IsPaused() const
  {
    return m_state == Timer::PAUSED;
  }

  bool Timer::IsStopped() const
  {
    return m_state == Timer::STOPPED;
  }

  bool Timer::IsRunning() const
  {
    return m_state == Timer::RUNNING;
  }

} // Utilities namespace
} // SF namespace
