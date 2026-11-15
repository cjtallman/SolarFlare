/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Asserts.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/03/2008 @ 16:40:11 - nwilliam
**  \brief   A few more rounded and helpful asserts than std::assert
*******************************************************************************/

// Major source of inspiration, example, and ideas
// http://www.powerof2games.com/node/10

#ifndef SF_ASSERT_H
#define SF_ASSERT_H

#if defined(_DEBUG)
#define SF_ASSERTS_ENABLED 1
#endif

#if !defined(_MSC_VER)
#include <cstdlib>
#endif

namespace SF {
namespace Utilities {
namespace Asserts
{
  extern const bool g_false;
  
  
  #ifdef _MSC_VER
    #define HALT() do { __debugbreak(); } while( SF::Utilities::Asserts::g_false )
  #else
    #define HALT() do { std::abort(); } while( SF::Utilities::Asserts::g_false )
  #endif
  
  #define UNUSED( x ) do { (void)sizeof( x ); } while( SF::Utilities::Asserts::g_false )
  
  
  typedef bool (*AssertCallback)( const char* file, int line, const char* condition, const char* msg );
  
  AssertCallback GetCallback();
  void SetCallback( AssertCallback callback );
  
  
  #ifdef SF_ASSERTS_ENABLED
    #define ASSERT( cond ) \
    do { if( ( !(cond) ) && SF::Utilities::Asserts::GetCallback()( __FILE__, __LINE__, #cond, "No message" ) ) \
            HALT(); } while( SF::Utilities::Asserts::g_false )
    
    #define ASSERT_MSG( cond, msg ) \
      do { if( ( !(cond) ) && SF::Utilities::Asserts::GetCallback()( __FILE__, __LINE__, #cond, msg ) ) \
            HALT(); } while( SF::Utilities::Asserts::g_false )
    
    #define ASSERT_FAIL( msg ) \
      do { if( SF::Utilities::Asserts::GetCallback()( __FILE__, __LINE__, "FAIL", msg ) ) \
            HALT(); } while( SF::Utilities::Asserts::g_false )
  #else
    #define ASSERT( cond ) \
      do { UNUSED( cond ); } while( SF::Utilities::Asserts::g_false )
      
    #define ASSERT_MSG( cond, msg ) \
      do { UNUSED( cond ); UNUSED( msg ); } while( SF::Utilities::Asserts::g_false )
      
    #define ASSERT_FAIL( msg ) \
      do { UNUSED( msg ); } while( SF::Utilities::Asserts::g_false )
  #endif
  
} // Asserts namespace
} // Utilities namespace
} // SF namespace

#endif // SF_ASSERT_H
