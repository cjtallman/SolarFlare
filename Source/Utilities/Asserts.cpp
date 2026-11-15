/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Asserts.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/03/2008 @ 16:40:17 - nwilliam
**  \brief   Implementation of handier asserts
*******************************************************************************/

#include <iostream>
#include "Asserts.h"

namespace
{
  bool s_callbackSet = false;
  SF::Utilities::Asserts::AssertCallback s_callback;
  
  bool DefaultCallback( const char* file, const int line, const char* condition, const char* msg )
  {
    std::cout << "Assert Failed - [" << file  << "," << line << "] ";
    
    if( condition ) std::cout << "(" << condition << ") ";
    
    if( msg ) std::cout << msg;
    
    std::cout << std::endl;
    
    return true;
  }
}


namespace SF {
namespace Utilities { 
namespace Asserts
{
  const bool g_false = false;
  
  
  AssertCallback GetCallback()
  {
    if( !s_callbackSet )
    {
      s_callback = DefaultCallback;
      s_callbackSet = true;
    }
    
    return s_callback;
  }

  void SetCallback( AssertCallback callback )
  {
    s_callback = callback;
  }
} // Asserts namespace
} // Utilities namespace
} // SF namespace
