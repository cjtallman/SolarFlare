/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RefCount.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/05/2009 @ 00:52:52 - nwilliam
**  \brief   Simple wrapper for a non-thread safe ref counter.
*******************************************************************************/

#ifndef SF_REF_COUNT_H
#define SF_REF_COUNT_H

#include "MemoryManager.h"

namespace SF {
namespace Utilities
{
  class RefCount
  {
    public:
      RefCount();
      RefCount( const RefCount& rhs );
      ~RefCount();

      int Count() const;
      bool Unique() const;

      void Swap( RefCount& rhs );
      RefCount& operator=( const RefCount& rhs );

      typedef MemoryManager< int, 128 > IntMemPool;

      static IntMemPool s_memPool;

    private:
      void Release();

      int* m_count;
  };

} // Utilities namespace
} // SF namespace

#endif // SF_REF_COUNT_H
