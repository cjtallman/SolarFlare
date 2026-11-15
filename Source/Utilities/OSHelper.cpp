/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    OSHelper.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/11/2009 @ 21:40:19 - nwilliam
**  \brief   OS specific helper functions
*******************************************************************************/

#include "OSHelper.h"

#if defined( _WIN32 )
  #include <shlobj.h>   // SHGetFolderPath
  #include <windows.h>  // IsClipboardFormatAvailable, OpenClipboard, etc
#elif defined( __linux )
  #include <cerrno>
  #include <sys/stat.h> // mkdir
#endif

namespace SF {
namespace Utilities
{
  std::string GetClipboardText()
  {
    std::string text;

    #ifdef _WIN32

      if( IsClipboardFormatAvailable( CF_TEXT ) && OpenClipboard( 0 ) )
      {
        HANDLE cbHandle = GetClipboardData( CF_TEXT );

        if( cbHandle )
        {
          const char* str = static_cast<const char*>( GlobalLock( cbHandle ) );

          if( str )
          {
            text = str;
            GlobalUnlock( cbHandle );
          }
        }

        CloseClipboard();
      }

    #elif defined( __linux )
      #warning "GetClipboardText not implemented."
    #else
      #error "OS not supported for GetClipboardText"
    #endif

    return text;
  }


  std::string GetUserAppDataPath()
  {
    std::string path;

    #if defined( _WIN32 )

      TCHAR buffer[ MAX_PATH ];

      HRESULT res = SHGetFolderPath( 0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, SHGFP_TYPE_CURRENT, buffer );

      if( res == S_OK )
        path = buffer;

    #elif defined( __linux )

      return std::string( "~/" );

    #else
      #error "OS not supported for GetUserAppDataPath"
    #endif

    return path;
  }

  bool CreateFolder( const std::string& path )
  {
    bool success = false;

    #if defined( _WIN32 )

      success = CreateDirectory( path.c_str(), 0 ) != 0;

      if( !success && ( GetLastError() == ERROR_ALREADY_EXISTS ) )
        success = true;

    #elif defined( __linux )

      int result = mkdir( path.c_str(), S_IRWXU | S_IRWXG );

      if( result || errno == EEXIST )
        success = true;

    #else
      #error "OS not supported for CreateDirectory"
    #endif

    return success;
  }

} // Utilities namespace
} // SF namespace
