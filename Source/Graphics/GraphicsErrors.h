/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    GraphicsErrors.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/03/2009 @ 22:03:59 -- nwilliam
**  \brief
*******************************************************************************/

#ifndef SF_GRAPHICS_ERRORS_H
#define SF_GRAPHICS_ERRORS_H


enum {
  GFX_OK = 0,
  GFX_ERR,
  GFX_PRESENT_ERR,
  GFX_CLEAR_COLOR_BUFFER_ERR,
  GFX_CLEAR_DEPTH_BUFFER_ERR,
  GFX_CLEAR_ALL_BUFFERS_ERR
};

#endif // SF_GRAPHICS_ERRORS_H
