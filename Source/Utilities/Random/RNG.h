/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RNG.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/11/2008 @ 16:00:40 - nwilliam
**  \brief   Base class for random number generators. Implemented with CRTP.
*******************************************************************************/

#ifndef SF_RNG_H
#define SF_RNG_H

#include <climits>  // uint max
#include "Networking/ByteStream.h"

namespace SF {
namespace Utilities
{
  template< typename Derived >
  class RNG
  {
    public:
      
      // Range: [0,1)
      double Rand()
      {
        static const double divisor = 1.0 / ( UINT_MAX + 1.0 );
        
        unsigned int x = static_cast< Derived* >(this)->RandVal();
        return x * divisor;
      }
      
      // Range: [0,int max]
      int RandInt()
      {
        return RandInt( INT_MAX - 1 );
      }
      
      // Range: [0,max]
      int RandInt( int max )
      {
        return static_cast<int>( Rand() * ( max + 1 ) );
      }
      
      // Range: [min,max]
      int RandInt( int min, int max )
      {
        return min + RandInt( max - min );
      }
      
      // Range [0,255]
      void RandBytes( unsigned char* buffer, int len )
      {
        if( !buffer ) return;
        
        for( int i = 0; i < len; ++i )
          buffer[i] = static_cast<unsigned char>( RandInt( 255 ) );
      }
      
      void Seed( int seed )
      {
        static_cast< Derived* >(this)->Seed( seed );
      }
  };
  
  
  template< typename Derived >
  Networking::ByteStream& operator>>( Networking::ByteStream& bs, RNG<Derived>& rng )
  {
    bs >> static_cast<Derived&>( rng );
    return bs;
  }
  
  template< typename Derived >
  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const RNG<Derived>& rng )
  {
    bs << static_cast<Derived&>( rng );
    return bs;
  }
  
} // Utilities namespace
} // SF namespace

#endif // SF_RNG_H
