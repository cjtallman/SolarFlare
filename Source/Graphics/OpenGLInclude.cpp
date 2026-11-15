/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    OpenGLInclude.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#include "OpenGLInclude.h"
#include "SolarFlare/Globals.h"
#include "GraphicsEngine.h"
#include "Utilities/DevConsole.h"
#include <stack>

bool CheckOpenGLError(const char * text, const char * file, const char * function, const long line )
{
  GLenum err = glGetError();
  if( err != GL_NO_ERROR )
  {
    const char * error_string = (const char*)gluErrorString(err);

    if( function )
    {
      Globals::console->Error() << "Function: " << function << std::endl;
    }
    if( file )
    {
      Globals::console->Error() << "In File: " << file << "[" << line << "]" << std::endl;
    }
    if( text )
    {
      Globals::console->Error() << "Text: " << text << std::endl;
    }

    if( error_string )
      Globals::console->Error(error_string);
    else
      Globals::console->Error() << "Unknown opengl error: " << err << std::endl;

    return false;
  }
  return true;
}


static std::stack<Math::Matrix4> modelview_matrix_stack;
static std::stack<Math::Matrix4> projection_matrix_stack;

void PushMatrix()
{
////#ifdef USE_OPENGL_MATRIX_STACK
  glPushMatrix();
////#else
//  Math::Matrix4 mat;
//  int matrix_mode;
//  glGetIntegerv( GL_MATRIX_MODE, &matrix_mode );
//  if( matrix_mode == GL_MODELVIEW )
//  {
//    glGetFloatv( GL_MODELVIEW_MATRIX, mat.Array() );
//    modelview_matrix_stack.push(mat);
//  }
//  else if( matrix_mode == GL_PROJECTION )
//  {
//    glGetFloatv( GL_PROJECTION_MATRIX, mat.Array() );
//    projection_matrix_stack.push(mat);
//  }
////#endif
}

void PopMatrix()
{
////#ifdef USE_OPENGL_MATRIX_STACK
  glPopMatrix();
////#else
//  int matrix_mode;
//  glGetIntegerv( GL_MATRIX_MODE, &matrix_mode );
//  if( matrix_mode == GL_MODELVIEW )
//  {
//    Math::Matrix4 mat = modelview_matrix_stack.top();
//    modelview_matrix_stack.pop();
//    glLoadMatrixf(mat.Array());
//  }
//  else if( matrix_mode == GL_PROJECTION )
//  {
//    Math::Matrix4 mat = projection_matrix_stack.top();
//    projection_matrix_stack.pop();
//    glLoadMatrixf(mat.Array());
//  }
////#endif
}




//************************************
// Method:    Enable2D
// FullName:  Enable2D
// Access:    public
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void Enable2D( bool normalized )
{
  CheckThisForErrors(glMatrixMode( GL_PROJECTION ));
  CheckThisForErrors(PushMatrix());
  CheckThisForErrors(glLoadIdentity());

  if( normalized )
  {
    CheckThisForErrors(gluOrtho2D(0,1,1,0));
  }
  else
  {
    CheckThisForErrors(gluOrtho2D(
      0,
      SF::Globals::graphicsEngine->WindowWidth(),
      SF::Globals::graphicsEngine->WindowHeight(),
      0));
  }
  CheckThisForErrors(glMatrixMode( GL_MODELVIEW ));
  CheckThisForErrors(PushMatrix());
  CheckThisForErrors(glLoadIdentity());
  CheckThisForErrors(glPushAttrib( GL_ALL_ATTRIB_BITS ));
  CheckThisForErrors(glDisable( GL_DEPTH_TEST ));
  CheckThisForErrors(glDisable( GL_LIGHTING ));
  CheckThisForErrors(glEnable( GL_BLEND ));
  CheckThisForErrors(glBlendFunc( GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA ));
}

//************************************
// Method:    Disable2D
// FullName:  Disable2D
// Access:    public
// Returns:   void
// Qualifier:
// Parameter: void
//************************************
void Disable2D( void )
{
  CheckThisForErrors(glPopAttrib());
  CheckThisForErrors(glMatrixMode( GL_PROJECTION ));
  CheckThisForErrors(PopMatrix());
  CheckThisForErrors(glMatrixMode( GL_MODELVIEW ));
  CheckThisForErrors(PopMatrix());
}

void DrawScreenQuad()
{
  CheckThisForErrors(Enable2D(true));
  glBegin(GL_QUADS);
  glColor3f(1,1,1);

  glTexCoord2i(0,1);
  glVertex2i(0,0);

  glTexCoord2i(0,0);
  glVertex2i(0,1);

  glTexCoord2i(1,0);
  glVertex2i(1,1);

  glTexCoord2i(1,1);
  glVertex2i(1,0);

  glEnd();
  CheckThisForErrors(Disable2D());
}

static SF::Math::Matrix4 s_square_perspective_matrix;
SF::Math::Matrix4 & SquarePerspectiveMatrix()
{
  return s_square_perspective_matrix;
}