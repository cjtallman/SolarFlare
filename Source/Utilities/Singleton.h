/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Singleton.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 20:54:14 - nwilliam
**  \brief 
*******************************************************************************/

#ifndef SF_SINGLETON_H
#define SF_SINGLETON_H

#include "Macros.h"

namespace SF {
namespace Utilities {
namespace Patterns
{
  template <class T>
  class Singleton
  {
    public:
      Singleton()
      {
        if (s_instance)
        {
          throw( "Singleton Error" );
        }
        else
        {
          s_instance = (T*)(this);
        }
      }

      ~Singleton(void)
      {
        if( s_instance != this )
          SAFE_DELETE(s_instance); 
      }

      static inline T * Instance() 
      { 
        if( !s_instance )
          new T();

        return s_instance; 
      }

    private:
      static T * s_instance;
  };

  template <class T> T * Singleton<T>::s_instance = 0;

} // Patterns namespace
} // Utilities namespace
} // SF namespace

#endif
