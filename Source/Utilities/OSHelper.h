/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    OSHelper.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/26/2008 @ 11:37:41 - nwilliam
**  \brief   OS specific helper functions
*******************************************************************************/

#ifndef SF_OS_HELPER_H
#define SF_OS_HELPER_H

#include <string>

namespace SF {
namespace Utilities
{
  std::string GetClipboardText();
  std::string GetUserAppDataPath();
  bool CreateFolder( const std::string& path );
  
} // Utilities namespace
} // SF namespace

#endif // SF_OS_HELPER_H
