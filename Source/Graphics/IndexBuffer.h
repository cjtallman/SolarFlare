/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    IndexBuffer.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:50:07 CTallman
**  \brief   Opengl wrapper for index buffers.
*******************************************************************************/

#ifndef _SF_INDEX_BUFFER_H_
#define _SF_INDEX_BUFFER_H_

#include "OpenGLInclude.h"

#include <vector>

class IndexBuffer
{
public:
  IndexBuffer( int _usage = GL_STATIC_DRAW_ARB );
  ~IndexBuffer(void);

  void Draw(int _mode);
  void AddIndex( unsigned int idx );
  void Clear();

private:
  GLuint handle;
  std::vector<unsigned int> indices; 
  int buffer_usage; 
  bool new_indices;
};


#endif
