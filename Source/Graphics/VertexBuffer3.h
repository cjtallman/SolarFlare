/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    VertexBuffer3.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:50:52 CTallman
**  \brief   Templatized Vertex Buffer Object ( VBO )
*******************************************************************************/

#ifndef SF_VERTEX_BUFFER_3_H_
#define SF_VERTEX_BUFFER_3_H_

#include "OpenGLInclude.h"
#include <vector>
#include "Vertex.h"
#include "Utilities/Asserts.h"

template <typename T>
class VertexBuffer3
{
public:
  typedef T VertexType;

  VertexBuffer3<T>( int _usage = GL_DYNAMIC_DRAW_ARB );
  VertexBuffer3<T>( int _usage, int _size );
  ~VertexBuffer3<T>(void);

  void Resize( int _size );

  void QuickDraw( int mode = GL_TRIANGLES );
  void Draw( int mode = GL_TRIANGLES );
  void Load();
  void Unload();
  void Clear();
  int Count() const;
  unsigned int GetHandle();

  unsigned int AddVertex( T _new_vertex );

  const T & operator[](int i) const;
  T & operator[](int i);


private:
  std::vector<T> vertices;
  int vert_count;
  bool loaded;

  int buffer_usage;
  int max_vertex_attribs;
  bool has_new_vertices;

  unsigned int handle;
};



#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace
{
  bool Error( const std::string & error )
  {
    SF::Globals::console->Error( "VertexBuffer3.cpp --- " + error );
    return false;
  }

  bool Warning( const std::string & error )
  {
    SF::Globals::console->Warning( "VertexBuffer3.cpp --- " + error );
    return true;
  }
}


template <typename T>
VertexBuffer3<T>::VertexBuffer3( int _usage )
: vert_count(0), loaded(false), buffer_usage(_usage), has_new_vertices(false), handle(0)
{
}

template <typename T>
VertexBuffer3<T>::VertexBuffer3( int _usage, int _size )
: vert_count(_size), loaded(false), buffer_usage(_usage), has_new_vertices(false), handle(0)
{
  vertices.resize(vert_count);
}

template <typename T>
VertexBuffer3<T>::~VertexBuffer3(void)
{
  /*if( handle )
  {
    glDeleteBuffersARB(1, &handle);
    CheckOpenGLError();
  }*/
}

template <typename T>
unsigned int VertexBuffer3<T>::GetHandle(void)
{
  return handle;
}

template <typename T>
int VertexBuffer3<T>::Count() const
{
  return vert_count;
}

template <typename T>
const T & VertexBuffer3<T>::operator[]( int i ) const
{
  if( i < 0 || i > vert_count-1 )
    throw("Invalid index for VertexBuffer3::operator[]const");
  else
  {
    return vertices[i];
  }
}

template <typename T>
T & VertexBuffer3<T>::operator[]( int i )
{
  if( i < 0 || i > vert_count-1 )
    throw("Invalid index for VertexBuffer3::operator[]");
  else
  {
    has_new_vertices = true;
    return vertices[i];
  }
}

template <typename T>
void VertexBuffer3<T>::Resize( int _new_size )
{
  vertices.resize( _new_size );
  vert_count = _new_size;
  has_new_vertices = true;
}

template <typename T>
void VertexBuffer3<T>::Load(void)
{
  if( handle == 0 )
  {
    CheckThisForErrors(glGenBuffersARB(1, &handle ));
    CheckThisForErrors(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB, &max_vertex_attribs));
  }

  CheckThisForErrors(glBindBufferARB(GL_ARRAY_BUFFER_ARB, handle));

  if( has_new_vertices )
  {
    CheckThisForErrors(glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(T)*vertices.size(), &vertices.front(), buffer_usage));
    has_new_vertices = false;
  }

#pragma region Enables

  int vert_size = sizeof(T);
  for( int i = 0; i < max_vertex_attribs; ++i )
  {
    if( T::Attribute( ATTRIBUTES::Attributes(i) ) )
    {
      ATTRIBUTES::Attributes attr = ATTRIBUTES::Attributes(i);

      switch( i )
      {
      case ATTRIBUTES::POSITION:
        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset(attr) ) );
        break;

      case ATTRIBUTES::BLENDWEIGHT:
        glEnableClientState( GL_WEIGHT_ARRAY_ARB );
        glWeightPointerARB(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset(attr) ) );
        break;

      case ATTRIBUTES::NORMAL:
        glEnableClientState( GL_NORMAL_ARRAY );
        glNormalPointer(
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset(attr) ) );
        break;

      case ATTRIBUTES::COLOR:
        glEnableClientState( GL_COLOR_ARRAY );
        glColorPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset(attr) ) );
        break;

      case ATTRIBUTES::COLOR1:
        glEnableClientState( GL_SECONDARY_COLOR_ARRAY );
        glSecondaryColorPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset(attr) ) );
        break;

      case ATTRIBUTES::FOGCOORD:
        glEnableClientState( GL_FOG_COORD_ARRAY );
        glFogCoordPointer(
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset(attr) ) );
        break;

      case ATTRIBUTES::PSIZE:
        glEnableVertexAttribArrayARB( attr );
        glVertexAttribPointerARB(
          i,
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          false,
          vert_size,
          BUFFER_OFFSET(T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::BLENDINDICES:
        glEnableClientState( GL_INDEX_ARRAY );
        glIndexPointer(
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::TEXCOORD0:
        glClientActiveTextureARB( GL_TEXTURE0 );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::TEXCOORD1:
        glClientActiveTextureARB( GL_TEXTURE1 );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::TEXCOORD2:
        glClientActiveTextureARB( GL_TEXTURE2 );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::TEXCOORD3:
        glClientActiveTextureARB( GL_TEXTURE3 );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::TEXCOORD4:
        glClientActiveTextureARB( GL_TEXTURE4 );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::TEXCOORD5:
        glClientActiveTextureARB( GL_TEXTURE5 );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::TEXCOORD6:
        glClientActiveTextureARB( GL_TEXTURE6 );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      case ATTRIBUTES::TEXCOORD7:
        glClientActiveTextureARB( GL_TEXTURE7 );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(
          T::ComponentCount( attr ),
          T::ComponentType( attr ),
          vert_size,
          BUFFER_OFFSET( T::Offset( attr ) ) );
        break;

      }
    }
  }

#pragma endregion

  loaded = true;
}

template <typename T>
void VertexBuffer3<T>::Unload()
{
  CheckThisForErrors(glBindBufferARB(GL_ARRAY_BUFFER_ARB, handle));

#pragma region Disables

  for( int i = 0; i < max_vertex_attribs; ++i )
  {
    if( T::Attribute( ATTRIBUTES::Attributes(i) ) )
    {
      switch( i )
      {
      case ATTRIBUTES::POSITION:
        glDisableClientState( GL_VERTEX_ARRAY );
        break;
      case ATTRIBUTES::BLENDWEIGHT:
        glDisableClientState( GL_WEIGHT_ARRAY_ARB );
        break;
      case ATTRIBUTES::NORMAL:
        glDisableClientState( GL_NORMAL_ARRAY );
        break;
      case ATTRIBUTES::COLOR0:
        glDisableClientState( GL_COLOR_ARRAY );
        break;
      case ATTRIBUTES::COLOR1:
        glDisableClientState( GL_SECONDARY_COLOR_ARRAY );
        break;
      case ATTRIBUTES::FOGCOORD:
        glDisableClientState( GL_FOG_COORD_ARRAY );
        break;
      case ATTRIBUTES::PSIZE:
        glDisableVertexAttribArrayARB( ATTRIBUTES::Attributes(i) );
        break;
      case ATTRIBUTES::BLENDINDICES:
        glDisableClientState( GL_INDEX_ARRAY );
        break;
      case ATTRIBUTES::TEXCOORD0:
        glClientActiveTextureARB( GL_TEXTURE0 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        break;
      case ATTRIBUTES::TEXCOORD1:
        glClientActiveTextureARB( GL_TEXTURE1 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        break;
      case ATTRIBUTES::TEXCOORD2:
        glClientActiveTextureARB( GL_TEXTURE2 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        break;
      case ATTRIBUTES::TEXCOORD3:
        glClientActiveTextureARB( GL_TEXTURE3 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        break;
      case ATTRIBUTES::TEXCOORD4:
        glClientActiveTextureARB( GL_TEXTURE4 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        break;
      case ATTRIBUTES::TEXCOORD5:
        glClientActiveTextureARB( GL_TEXTURE5 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        break;
      case ATTRIBUTES::TEXCOORD6:
        glClientActiveTextureARB( GL_TEXTURE6 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        break;
      case ATTRIBUTES::TEXCOORD7:
        glClientActiveTextureARB( GL_TEXTURE7 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        break;
      }
    }
  }

#pragma endregion

  CheckThisForErrors(glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0));
  loaded = false;
}

template <typename T>
void VertexBuffer3<T>::Draw(int _mode)
{
  if( loaded )
  {
    CheckThisForErrors(glBindBufferARB(GL_ARRAY_BUFFER_ARB, handle));
    CheckThisForErrors(glDrawArrays(_mode, 0, vert_count ));
  }
}

template <typename T>
void VertexBuffer3<T>::QuickDraw(int _mode)
{
  CheckThisForErrors(glPushClientAttrib( GL_CLIENT_ALL_ATTRIB_BITS ));
  Load();
  Draw(_mode);
  Unload();
  CheckThisForErrors(glPopClientAttrib());
}

template <typename T>
unsigned int VertexBuffer3<T>::AddVertex( T _new_vertex )
{
  has_new_vertices = true;
  vertices.push_back(_new_vertex);
  return vert_count++;
}

template <typename T>
void VertexBuffer3<T>::Clear()
{
  vertices.clear();
  has_new_vertices = true;
  vert_count = 0;
}


#endif
