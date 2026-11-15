/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Timer.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/10/2008 @ 15:38:20 - nwilliam
**  \brief   Interface for portable high resolution timer class.
*******************************************************************************/

#ifndef SF_TIMER_H
#define SF_TIMER_H

#if defined(_WIN32)
  #include <windows.h>
  namespace SF { namespace Utilities { typedef LONGLONG TimeCount; } }
#elif defined(__linux)
  #include <time.h>
  #include <sys/time.h>

  namespace SF { namespace Utilities { typedef timespec TimeCount; } }
#else
  #error "Timer does not support this OS!"
#endif

namespace SF {
namespace Utilities
{
  class Timer
  {
    public:
      Timer();

      // Default copy, assingment, destructor ok

      void Update();

      void Stop();
      void Start();
      void Reset();
      void Pause();
      void Unpause();

      bool IsPaused() const;
      bool IsStopped() const;
      bool IsRunning() const;

      float Frequency() const;

      float Elapsed() const;
      float ElapsedSinceStart() const;

      float ElapsedScale() const;
      void ElapsedScale( float scale );
      void DeltaElapsedScale( float delta );

    private:
      enum TimerState { PAUSED , STOPPED , RUNNING };

      TimerState m_state;
      float m_frequency;
      float m_lastElapsed;
      float m_elapsedScale;

      TimeCount m_count;
      TimeCount m_lastCount;
      TimeCount m_startCount;
  };

} // Utilities namespace
} // SF namespace

#endif
