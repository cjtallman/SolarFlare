/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Misc.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/18/2009 @ 20:29 - CJT 
**  \brief   Various, mostly templatized, handly helper functions.
*******************************************************************************/

#include "Misc.h"
#include <algorithm>
#include <cctype>


namespace SF {
namespace Utilities
{

  void StrUpr( std::string & str )
  {
    std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) std::toupper);
  }

  void StrLwr( std::string & str )
  {
    std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) std::tolower);
  }

}//namespace Utilities
}//namespace SF
