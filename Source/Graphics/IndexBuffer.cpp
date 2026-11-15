/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    IndexBuffer.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:49:39 CTallman
**  \brief   Opengl wrapper for index buffers.
*******************************************************************************/

#include "IndexBuffer.h"
#include "Utilities/LeakCheck.h"

IndexBuffer::IndexBuffer( int _usage )
: buffer_usage(_usage), handle(0), new_indices(false)
{
}

IndexBuffer::~IndexBuffer(void)
{
  /*if( handle )
  {  
    glDeleteBuffersARB(1, &handle);
    CheckOpenGLError();
  }*/
}

void IndexBuffer::Draw( int _mode )
{
  if( indices.empty() )
    return;

  if( handle == 0 )
  {
    CheckThisForErrors(glGenBuffersARB(1, &handle ));
  }


  CheckThisForErrors(glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, handle));

  if( new_indices )
  {
    CheckThisForErrors(glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*indices.size(), &indices.front(), buffer_usage));
    new_indices = false;
  }

  CheckThisForErrors(glDrawElements(_mode, (int)indices.size(), GL_UNSIGNED_INT, 0 ));
  CheckThisForErrors(glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0));

  return;
}

void IndexBuffer::AddIndex( unsigned int idx )
{
  indices.push_back(idx);
  new_indices = true;
}

void IndexBuffer::Clear()
{
  indices.clear();
}

