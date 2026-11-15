/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    LeakCheck.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 16:37:15 - nwilliam
**  \brief   Simple MS CRT debug heap leak checking stuff.
*******************************************************************************/

#ifndef SF_LEAK_CHECK_H
#define SF_LEAK_CHECK_H

#if defined(_DEBUG) && defined(_MSC_VER)
  #pragma once
  
  #include <crtdbg.h>

  #define new             new( _CLIENT_BLOCK, __FILE__, __LINE__)
  //#define malloc( size )  _malloc_dbg( size , _CLIENT_BLOCK , __FILE__ , __LINE__ );

#endif

#endif // SF_LEAK_CHECK_H
