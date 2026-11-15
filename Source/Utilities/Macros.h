/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Macros.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 20:51:48 - nwilliam
**  \brief 
*******************************************************************************/


// prevent copying
#define NO_COPY(a)  private:\
                      a(const a& src);\
                      a& operator=(const a& src);\


#define SAFE_DELETE_ARRAY(p)  delete[] p;\
                              p = 0;


#define SAFE_DELETE(p)        delete p;\
                              p = 0;


#define TO_STRING(a)          (#a)
