/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    MessageStack.cpp
**  \author  Patrick Mera, Nathan Williams
**  \par     email: pmera@digipen.edu, nwilliam\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 01/27/2009 @ 16:31:08 - nwilliam
**  \brief   Explicit specializations and other implementations of MessageStack
*******************************************************************************/

#include "MessageStack.h"
#include "Utilities/LeakCheck.h"
namespace TypeEncode
{
  const int8 INVALID = -1;

  const int8 INT = 0;
  const int8 BOOL = 1;
  const int8 FLOAT = 2;
  const int8 DOUBLE = 3;
  const int8 STRING = 4;
}


namespace SF {
namespace Engine
{
  //////////////////////////////////////////////////////////////////////////////
  // Explicit specializations
  //////////////////////////////////////////////////////////////////////////////
  template<> void MessageStack::LuaPush<int>( lua_State *ls, void *object )
  {
    lua_pushinteger( ls, *static_cast<int*>( object ) );
  }

  template<> void MessageStack::LuaPush<bool>( lua_State *ls, void *object )
  {
    lua_pushboolean( ls, *static_cast<bool*>( object ) );
  }

  template<> void MessageStack::LuaPush<float>( lua_State *ls, void *object )
  {
    lua_pushnumber( ls, *static_cast<float*>( object ) );
  }

  template<> void MessageStack::LuaPush<double>( lua_State *ls, void *object )
  {
    lua_pushnumber( ls, *static_cast<double*>( object ) );
  }

  template<> void MessageStack::LuaPush<std::string>( lua_State *ls, void *object )
  {
    const std::string* const str = static_cast<const std::string* const>( object );
    lua_pushlstring( ls, str->c_str(), str->length() );
  }

  template<> void MessageStack::Push<std::string>( const std::string& object )
  {
    StackNode node;

    node.object = new std::string( object );
    node.luaPush = LuaPush<std::string>;

    m_stackByteSize += Networking::ByteStream::SerializedStringSize( object );
    m_stack.push_back( node );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  MessageStack::MessageStack() : m_stackByteSize( 0 )
  {
  }

  MessageStack::~MessageStack()
  {
    while( !Empty() )
      Pop();
  }

  bool MessageStack::Empty() const
  {
    return m_stack.empty();
  }

  size_t MessageStack::Size() const
  {
    return m_stack.size();
  }

  size_t MessageStack::SizeInBytes() const
  {
    return m_stackByteSize;
  }

  void MessageStack::Pop()
  {
    LuaPushFunction pushI = MessageStack::LuaPush<int>;
    LuaPushFunction pushB = MessageStack::LuaPush<bool>;
    LuaPushFunction pushF = MessageStack::LuaPush<float>;
    LuaPushFunction pushD = MessageStack::LuaPush<double>;
    LuaPushFunction pushS = MessageStack::LuaPush<std::string>;

    StackNode node;
    node = m_stack.back();

    if( node.luaPush == pushI )
    {
      m_stackByteSize -= sizeof(int);
      delete static_cast<int*>( node.object );
    }
    else if( node.luaPush == pushB )
    {
      m_stackByteSize -= sizeof(bool);
      delete static_cast<bool*>( node.object );
    }
    else if( node.luaPush == pushF )
    {
      m_stackByteSize -= sizeof(float);
      delete static_cast<float*>( node.object );
    }
    else if( node.luaPush == pushD )
    {
      m_stackByteSize -= sizeof(double);
      delete static_cast<double*>( node.object );
    }
    else if( node.luaPush == pushS )
    {
      std::string* str = static_cast<std::string*>( node.object );
      m_stackByteSize -= Networking::ByteStream::SerializedStringSize( *str );
      delete str;
    }
    else
    {
      *(int*)0 = 0xC0DEDBAD;
    }

    m_stack.pop_back();
  }

  void MessageStack::PushToLua( lua_State* ls ) const
  {
    lua_createtable( ls, static_cast<int>( m_stack.size() ), 0 );

    // Start array indicies at 1, general Lua convention
    for( size_t i = 0; i < m_stack.size(); ++i )
    {
      // Push table position, value, set position in table
      lua_pushinteger( ls, i + 1 );
      m_stack[i].luaPush( ls, m_stack[i].object );
      lua_rawset( ls, -3 );
    }

    // 1 item left on lua Stack (table of msg stack elements, indexed [1,size])
  }


  //////////////////////////////////////////////////////////////////////////////
  // Friend Functions
  //////////////////////////////////////////////////////////////////////////////
  Networking::ByteStream& operator>>( Networking::ByteStream& bs, MessageStack& m )
  {
    uint8 size;
    bs >> size;

    while( !m.Empty() )
      m.Pop();

    for( uint8 i = 0; i < size; ++i )
    {
      int8 type = TypeEncode::INVALID;
      bs >> type;

      switch( type )
      {
        case TypeEncode::INT:
        {
          int32 num;
          bs >> num;
          m.Push( num );
        }
        break;

        case TypeEncode::BOOL:
        {
          int8 val;
          bs >> val;
          m.Push( val != 0 );
        }
        break;

        case TypeEncode::FLOAT:
        {
          float num;
          bs >> num;
          m.Push( num );
        }
        break;

        case TypeEncode::DOUBLE:
        {
          double num;
          bs >> num;
          m.Push( num );
        }
        break;

        case TypeEncode::STRING:
        {
          std::string str;
          bs >> str;
          m.Push( str );
        }
        break;
      }
    }

    return bs;
  }

  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const MessageStack& m )
  {
    uint8 size = static_cast<uint8>( m.m_stack.size() );
    bs << size;

    MessageStack::LuaPushFunction pushI = MessageStack::LuaPush<int>;
    MessageStack::LuaPushFunction pushB = MessageStack::LuaPush<bool>;
    MessageStack::LuaPushFunction pushF = MessageStack::LuaPush<float>;
    MessageStack::LuaPushFunction pushD = MessageStack::LuaPush<double>;
    MessageStack::LuaPushFunction pushS = MessageStack::LuaPush<std::string>;

    for( uint8 i = 0; i < size; ++i )
    {
      if( m.m_stack[i].luaPush == pushI )
      {
        bs << TypeEncode::INT;
        bs << *static_cast<int*>( m.m_stack[i].object );
      }
      else if( m.m_stack[i].luaPush == pushB )
      {
        bs << TypeEncode::BOOL;
        bs << *static_cast<int8*>( m.m_stack[i].object );
      }
      else if( m.m_stack[i].luaPush == pushF )
      {
        bs << TypeEncode::FLOAT;
        bs << *static_cast<float*>( m.m_stack[i].object );
      }
      else if( m.m_stack[i].luaPush == pushD )
      {
        bs << TypeEncode::DOUBLE;
        bs << *static_cast<double*>( m.m_stack[i].object );
      }
      else if( m.m_stack[i].luaPush == pushS )
      {
        std::string& str = *static_cast<std::string*>( m.m_stack[i].object );

        bs << TypeEncode::STRING;
        bs << str;
      }
    }

    return bs;
  }

} // Engine namespace
} // SF namespace
