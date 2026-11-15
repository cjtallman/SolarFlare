/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CriticalSection.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 10/20/2008 @ 18:48:42 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_CRITICALSECTION_H_
#define SF_CRITICALSECTION_H_

namespace SF {
namespace Threading
{

class CriticalSectionImp;

class ICriticalSection
{
public:
  virtual void Enter() = 0;
  virtual void Leave() = 0;
};

class CriticalSection : public ICriticalSection
{
public:
  CriticalSection();
  ~CriticalSection();

  void Enter();
  void Leave();
private:
  CriticalSectionImp *m_imp;
};

}
}

#endif
