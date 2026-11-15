/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CriticalSectionImp_Win32.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/03/2009 @ 22:40:38 -- nwilliam
**  \brief
*******************************************************************************/

#if defined( _WIN32 )

#ifndef SF_CRITICAL_SECTION_IMP_WIN32_H
#define SF_CRITICAL_SECTION_IMP_WIN32_H

#include <windows.h>

#include "CriticalSection.h"

namespace SF {
namespace Threading
{
  class CriticalSectionImp : public ICriticalSection
  {
  public:
    CriticalSectionImp();
    ~CriticalSectionImp();

    void Enter();
    void Leave();

  private:
    CRITICAL_SECTION m_criticalSection;
  };

} // Threading namespace
} // SF namespace

#endif // SF_CRITICAL_SECTION_IMP_WIN32_H
#endif // _WIN32
