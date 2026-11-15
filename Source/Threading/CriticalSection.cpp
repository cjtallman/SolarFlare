/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CriticalSection.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/03/2009 @ 23:12:12 -- nwilliam
**  \brief
*******************************************************************************/

#include "CriticalSection.h"

#if defined( _WIN32 )
  #include "CriticalSectionImp_Win32.h"
#elif defined( __linux )
  #include "CriticalSectionImp_POSIX.h"
#else
  #error "No critical section implementation."
#endif

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Threading
{
  CriticalSection::CriticalSection()
  {
    m_imp = new CriticalSectionImp;
  }

  CriticalSection::~CriticalSection()
  {
    delete m_imp;
  }

  void CriticalSection::Enter()
  {
    m_imp->Enter();
  }

  void CriticalSection::Leave()
  {
    m_imp->Leave();
  }

} // Threading namespace
} // SF namespace
