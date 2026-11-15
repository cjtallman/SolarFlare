/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    HashHelpers.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/13/2009 @ 18:41:33 - nwilliam
**  \brief   Helper functions for hash table.
*******************************************************************************/

#ifndef SF_HASH_HELPERS_H
#define SF_HASH_HELPERS_H

#include <string>

namespace SF {
namespace Utilities
{
  unsigned GetClosestPrime( unsigned value );
  unsigned PJWHash( const std::string& str, unsigned tableSize );
  unsigned JOAATHash( const std::string& str, unsigned tableSize );
  
} // Utilities namespac
} // SF namespace

#endif // SF_HASH_HELPERS
