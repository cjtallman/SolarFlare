/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Misc.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/06/2009 @ 16:42:01 - nwilliam
**  \brief   Various, mostly templatized, handly helper functions.
*******************************************************************************/

#ifndef SF_MISC_H
#define SF_MISC_H

#include <string>

namespace SF {
namespace Utilities
{
  //Convert string to uppercase
  void StrUpr( std::string & str );

  //Convert string to lowercase
  void StrLwr( std::string & str );

  template< typename T >
  T Clamp( T value, T lowerBound, T upperBound )
  {
    // lol
    // return ( value < lowerBound ) ? lowerBound : ( ( value > upperBound ) ? upperBound : value );
    
    if( value < lowerBound )
    {
      return lowerBound;
    }
    else if( value > upperBound )
    {
      return upperBound;
    }
    else
    {
      return value;
    }
  }
  
  
  template< typename T >
  T ClampWrap( T value, T lowerBound , T upperBound )
  {
    if( value < lowerBound )
    {
      while( value < lowerBound )
      {
        value += (upperBound - lowerBound);
      }
      
      return upperBound + lowerBound - value - 1;
    }
    else if( value > upperBound )
    {
      while( value > upperBound )
      {
        value -= (upperBound - lowerBound);
      }
      
      return lowerBound + upperBound - value + 1;
    }
    else
    {
      return value;
    }
  }
  
  
  template< typename T >
  T Min( T a, T b )
  {
    return ( a < b ) ? a : b;
  }
  
  
  template< typename T >
  T Min( T a, T b, T c )
  {
    // lol x2
    //  return ( ( ( a < b ? a : b ) ) < c ? ( ( a < b ? a : b ) ) : c );
    
    if( a < b )
    {
      return ( a < c ) ? a : c;
    }
    else if( a < c )
    {
      return b;
    }
    else
    {
      return ( b < c ) ? b : c;
    }
  }
  
  
  template< typename T >
  T Max( T a, T b )
  {
    return ( a > b ) ? a : b;
  }
  
  
  template< typename T >
  T Max( T a, T b, T c )
  {
    // lols x3
    // return ( ( ( a > b ? a : b ) ) > c ? ( ( a > b ? a : b ) ) : c );
    
    if( a > b )
    {
      return ( a > c ) ? a : c;
    }
    else if( a > c )
    {
      return b;
    }
    else
    {
      return ( b > c ) ? b : c;
    }
  }
  
  template< int >
  int Abs( int x )
  {
    return ( x ^ (x>>31) ) - (x >> 31);
  }

  template< typename T>
  T Abs( T x )
  {
    return x < 0 ? -x : x;
  }

  template< int >
  void Swap( int & a, int & b )
  {
    a ^= b;
    b ^= a;
    a ^= b;
  }

  template< typename T >
  void Swap( T& a, T& b )
  {
    T temp = a;
    a = b;
    b = temp;
  }
  
} // Utilities namespace
} // SF namespace

#endif // SF_MISC_H
