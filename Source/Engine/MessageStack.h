/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    MessageStack.h
**  \author  Patrick Mera, Nathan Williams
**  \par     email: pmera\@digipen.edu, nwilliam@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 01/27/2009 @ 16:31:25 - nwilliam
**  \brief   My lovely message stack. We'll see how this goes...
*******************************************************************************/

#ifndef SF_MESSAGE_STACK_H
#define SF_MESSAGE_STACK_H

#include <vector>
#include "Networking/ByteStream.h"
#include "External/Lua/include/lua.hpp"

namespace SF {
namespace Engine
{
  class MessageStack
  {
    public:
      MessageStack();
      ~MessageStack();

      bool Empty() const;
      size_t Size() const;
      size_t SizeInBytes() const;

      void Pop();
      void PushToLua( lua_State *ls ) const;

      template< typename T >
      T& Get( int index );

      template< typename T >
      const T& Get( int index ) const;

      template< typename T >
      void Push( const T& object );


    private:
      friend Networking::ByteStream& operator>>( Networking::ByteStream&, MessageStack& );
      friend Networking::ByteStream& operator<<( Networking::ByteStream&, const MessageStack& );

      typedef void (*LuaPushFunction)(lua_State *, void *object);

      template< typename T >
      static void LuaPush( lua_State *, void * );

      struct StackNode
      {
        void *object;
        LuaPushFunction luaPush;
      };

      size_t m_stackByteSize;
      std::vector< StackNode > m_stack;
  };

  // Forward declaration
  template<>
  void MessageStack::Push<std::string>( const std::string& object );


  Networking::ByteStream& operator>>( Networking::ByteStream& bs, MessageStack& m );
  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const MessageStack& m );


} // Engine namespace
} // SF namespace




// Template function implementation
namespace SF {
namespace Engine
{
  template< typename T >
  T& MessageStack::Get( int index )
  {
    LuaPushFunction addr = LuaPush<T>;

    if( m_stack[index].luaPush != addr )
      *(int*)0 = 0xC0DEDBAD;

    return *static_cast<T*>(m_stack[index].object);
  }

  template< typename T >
  const T& MessageStack::Get( int index ) const
  {
    LuaPushFunction addr = LuaPush<T>;

    if( m_stack[index].luaPush != addr )
      *(int*)0 = 0xC0DEDBAD;

    return *static_cast<T*>(m_stack[index].object);
  }

  template< typename T >
  void MessageStack::Push( const T& object )
  {
    StackNode node;

    node.object = new T( object );
    node.luaPush = LuaPush<T>;

    m_stack.push_back( node );
    m_stackByteSize += sizeof( T );
  }

} // Engine namespace
} // SF namespace

#endif // SF_MESSAGE_STACK_H
