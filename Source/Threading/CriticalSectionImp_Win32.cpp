/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CriticalSectionImp_Win32.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/03/2009 @ 22:42:05 -- nwilliam
**  \brief
*******************************************************************************/

#if defined( _WIN32 )
#include "CriticalSectionImp_Win32.h"

namespace SF {
namespace Threading
{
  CriticalSectionImp::CriticalSectionImp()
  {
    InitializeCriticalSection( &m_criticalSection );
  }

  CriticalSectionImp::~CriticalSectionImp()
  {
    DeleteCriticalSection( &m_criticalSection );
  }

  void CriticalSectionImp::Enter()
  {
    EnterCriticalSection( &m_criticalSection );
  }

  void CriticalSectionImp::Leave()
  {
    LeaveCriticalSection( &m_criticalSection );
  }

} // Threading namespace
} // SF namespace

#endif // _WIN32
