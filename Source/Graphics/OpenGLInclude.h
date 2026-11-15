/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    OpenGLInclude.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#ifndef SF_OPENGL_INCLUDE_H_
#define SF_OPENGL_INCLUDE_H_

// Note: glew.h must be included before gl.h
#include "External/glew32/include/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Math/TMatrix4x4.h"
#include "Utilities/Macros.h"


void Enable2D( bool normalized = false );
void Disable2D( void );
void DrawScreenQuad();

void PushMatrix();
void PopMatrix();


//#define CHECK_OPENGL_ERRORS 1

bool CheckOpenGLError( const char * info = 0, const char * file = 0, const char * function = 0, const long line = 0);
#ifdef CHECK_OPENGL_ERRORS
#define CheckThisForErrors(x) x; CheckOpenGLError(TO_STRING(x), __FILE__, __FUNCTION__, __LINE__ );
#else
#define CheckThisForErrors(x) x;
#endif

SF::Math::Matrix4 & SquarePerspectiveMatrix();

#endif
