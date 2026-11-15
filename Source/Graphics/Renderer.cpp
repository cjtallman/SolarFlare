/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Renderer.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 00:57 - CJT 
**  \brief   This is a simple OpenGL wrapper
*******************************************************************************/

#include "Renderer.h"
#include "GraphicsErrors.h"
#include "FrameBuffer2.h"

#if defined(_WIN32)
#include <windows.h>
#include "External/glew32/include/glew.h"
#include "External/glew32/include/wglew.h"
#include <GL/gl.h>
#include <GL/glu.h>

#elif defined(__linux)
#include <X11/Xlib.h>
#include "External/glew32/include/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#else
#error "GraphicsEngine not tested on this OS"
#endif

#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    struct OSWindowInfo
    {
#if defined( _WIN32 )
      HWND window_handle;
      HDC device_context;
      HGLRC rendering_context;
#elif defined( __linux )
      Display* display;
      ::Window window;
#else
#error "Write OSWindow for this OS"
#endif
    };

    namespace Renderer
    {

      void OpenGL::CheckMinimumRequirements()
      {
        if( !glewGetExtension( "GL_EXT_framebuffer_object" ) )
          throw("GL_EXT_framebuffer_object not supported on this computer.");
        if( !glewGetExtension( "GL_ARB_texture_non_power_of_two" ) )
          throw("GL_ARB_texture_non_power_of_two not supported on this computer.");
      }

      //////////////////////////////////////////////////////////////////////////
      OpenGL::OpenGL()
      {
        window = new OSWindowInfo();

        variables.window_width = 800;
        variables.window_height = 600;
        variables.aspect_ratio = 4.0 / 3.0;
        variables.fov = 60.0;
        variables.wireframe = false;
        variables.vsync = true;

        DepthClearValue( 1.0f );        
      }

      //////////////////////////////////////////////////////////////////////////
      OpenGL::~OpenGL()
      {
        Shutdown();
        delete window;
      }

      bool OpenGL::Init( void * data1, void * data2 )
      {
#if defined( _WIN32 )
        window->window_handle = *reinterpret_cast<HWND*>( data1 );
        data2;
#elif defined( __linux )
        window->display = *reinterpret_cast<Display**>( data1 );
        window->window = *reinterpret_cast<Window*>( data2 );
#else
#error "Test GraphicsEngine::Init for this OS"
#endif

        if( Startup() == false )
          return false;
        else
          return true;
      }

      //////////////////////////////////////////////////////////////////////////
      int OpenGL::Begin()
      {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        ClearAllBuffers();
        return GFX_OK;
      }

      //////////////////////////////////////////////////////////////////////////
      int OpenGL::End()
      {
        glPopAttrib();
        Present();
        return GFX_OK;

      }

      //////////////////////////////////////////////////////////////////////////
      int OpenGL::Present()
      {
#if defined( _WIN32 )
        SwapBuffers( window->device_context );
#elif defined( __linux )
        glXSwapBuffers( window->display, window->window);
#else
#error "Write Present for this OS"
#endif

#ifdef _DEBUG
        if( OpenGLErrors() )
          return GFX_PRESENT_ERR;
#endif
        return GFX_OK;
      }

      //////////////////////////////////////////////////////////////////////////
      int OpenGL::ClearAllBuffers()
      {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
#ifdef _DEBUG
        if( OpenGLErrors() )
          return GFX_CLEAR_ALL_BUFFERS_ERR;
#endif
        return GFX_OK;
      }

      //////////////////////////////////////////////////////////////////////////
      int OpenGL::ClearColorBuffer()
      {
        glClear( GL_COLOR_BUFFER_BIT );
#ifdef _DEBUG
        if( OpenGLErrors() )
          return GFX_CLEAR_COLOR_BUFFER_ERR;
#endif
        return GFX_OK;
      }

      //////////////////////////////////////////////////////////////////////////
      int OpenGL::ClearDepthBuffer()
      {
        glClear( GL_DEPTH_BUFFER_BIT );
#ifdef _DEBUG
        if( OpenGLErrors() )
          return GFX_CLEAR_DEPTH_BUFFER_ERR;
#endif
        return GFX_OK;
      }

      void OpenGL::WindowWidth( int width )
      {
        variables.window_width = width;
      }

      int OpenGL::WindowWidth() const
      {
        return variables.window_width;
      }

      void OpenGL::WindowHeight( int height )
      {
        variables.window_height = height;
      }

      int OpenGL::WindowHeight() const
      {
        return variables.window_height;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::ColorClearValue( const Color4f & value )
      {
        variables.clear_color = value;
        glClearColor( value.r, value.g, value.b, value.a );
      }

      //////////////////////////////////////////////////////////////////////////
      Color OpenGL::ColorClearValue() const
      {
        return variables.clear_color;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::DepthClearValue( float value )
      {
        variables.clear_depth = value;
        glClearDepth( value );
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::ColorWrites( bool enable )
      {
        if( enable != variables.color_writes )
        {
          if( enable )
          {
            variables.color_writes = true;
            glColorMask(true, true, true, true);
          }
          else
          {
            variables.color_writes = false;
            glColorMask(false, false, false, false);
          }
        }
      }

      //////////////////////////////////////////////////////////////////////////
      bool OpenGL::ColorWrites() const
      {
        return variables.color_writes;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::DepthTest( bool enable )
      {
        if( enable != variables.depth_test )
        {
          if( enable )
          {
            variables.depth_test = true;
            glEnable( GL_DEPTH_TEST );
          }
          else
          {
            variables.depth_test = false;
            glDisable( GL_DEPTH_TEST );
          }
        }
      }

      //////////////////////////////////////////////////////////////////////////
      bool OpenGL::DepthTest() const
      {
        return variables.depth_test;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::DepthWrites( bool enable )
      {
        if( enable != variables.depth_writes )
        {
          if( enable )
          {
            variables.depth_writes = true;
            glDepthMask(GL_TRUE);
          }
          else
          {
            variables.depth_writes = false;
            glDepthMask(GL_FALSE);
          }
        }
      }

      //////////////////////////////////////////////////////////////////////////
      bool OpenGL::DepthWrites() const
      {
        return variables.depth_writes;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::Lighting( bool enable )
      {
        if( enable != variables.lighting )
        {
          if( enable )
          {
            variables.lighting = true;
            glEnable( GL_LIGHTING );
          }
          else
          {
            variables.lighting = false;
            glDisable( GL_LIGHTING );
          }
        }
      }

      //////////////////////////////////////////////////////////////////////////
      bool OpenGL::Lighting() const
      {
        return variables.lighting;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::Wireframe( bool enable )
      {
        if( enable != variables.wireframe )
        {
          if( enable )
          {
            variables.wireframe = true;
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
          }
          else
          {
            variables.wireframe = false;
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
          }
        }
      }

      //////////////////////////////////////////////////////////////////////////
      bool OpenGL::Wireframe() const
      {
        return variables.wireframe;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::AspectRatio( double aspect )
      {
        if( aspect != variables.aspect_ratio )
        {
          variables.aspect_ratio = aspect;
          PerspectiveProjection();
        }
      }

      //////////////////////////////////////////////////////////////////////////
      double OpenGL::AspectRatio() const
      {
        return variables.aspect_ratio;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::FOV( double fov )
      {
        if( fov != variables.fov )
        {
          variables.fov = fov;
          PerspectiveProjection();
        }
      }

      //////////////////////////////////////////////////////////////////////////
      double OpenGL::FOV() const
      {
        return variables.fov;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::PerspectiveProjection()
      {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( variables.fov, variables.aspect_ratio, 2 , 10000000 );
        glMatrixMode( GL_MODELVIEW );
      }

      void OpenGL::PerspectiveProjection(double fov, double aspect, double nearplane, double farplane)
      {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( fov, aspect, nearplane , farplane );
        glMatrixMode( GL_MODELVIEW );
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::OrthographicProjection()
      {
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D(0,1,1,0);
        glMatrixMode( GL_MODELVIEW );
      }

      void OpenGL::Viewport( int x, int y, int w, int h )
      {
        if( w && h )
          glViewport( x, y, w, h );
      }

      void OpenGL::ResizeWindow( int w, int h )
      {
        WindowWidth( w );
        WindowHeight( h );
        Viewport( 0, 0, w, h );
        PerspectiveProjection();
      }

      void OpenGL::SetLightAmbient( const Color4f & color )
      {
        variables.light0_ambient = color;
        glLightfv( GL_LIGHT0, GL_AMBIENT, color.Array() );
      }

      void OpenGL::SetLightDiffuse( const Color4f & color )
      {
        variables.light0_diffuse = color;
        glLightfv( GL_LIGHT0, GL_DIFFUSE, color.Array() );
      }

      void OpenGL::SetLightSpecular( const Color4f & color )
      {
        variables.light0_specular = color;
        glLightfv( GL_LIGHT0, GL_SPECULAR, color.Array() );
      }

      void OpenGL::SetLightPercent( float percent )
      {
        variables.light0_percent = percent;
        glLightfv( GL_LIGHT0, GL_AMBIENT, (percent*variables.light0_ambient).Array() );
        glLightfv( GL_LIGHT0, GL_DIFFUSE, (percent*variables.light0_diffuse).Array() ); 
        glLightfv( GL_LIGHT0, GL_SPECULAR, (percent*variables.light0_specular).Array() ); 
      }


      //////////////////////////////////////////////////////////////////////////
      int OpenGL::OpenGLErrors() const
      {
        return glGetError();
      }

      //////////////////////////////////////////////////////////////////////////
      bool OpenGL::Startup()
      {
#if defined( _WIN32 )
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 0;
        pfd.cAccumBits = 0;

        window->device_context = GetDC( window->window_handle );
        if( !window->device_context )
          return false;

        unsigned int PixelFormat = ChoosePixelFormat( window->device_context, &pfd );
        SetPixelFormat( window->device_context, PixelFormat, &pfd );

        window->rendering_context = wglCreateContext( window->device_context );
        if( !window->rendering_context )
          return false;

        if( !wglMakeCurrent( window->device_context, window->rendering_context ) )
          return false;

#elif defined( __linux )
        // none
#else
#error "Test Startup for this OS"
#endif

        if( glewInit() != GLEW_OK )
          return false;
        else
          return true;
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::Shutdown()
      {
#if defined( _WIN32 )
        if( window->rendering_context )
        {
          wglMakeCurrent( NULL, NULL );
          wglDeleteContext( window->rendering_context );
          window->rendering_context = NULL;
        }

        if( window->device_context )
        {
          ReleaseDC( window->window_handle, window->device_context );
          window->device_context = NULL;
        }
#elif defined(__linux )
        // none
#else
#error "Test Shutdown for this OS"
#endif
      }

      //////////////////////////////////////////////////////////////////////////
      void OpenGL::VSync( bool enable )
      {
        if( enable != variables.vsync )
        {
#if defined( _WIN32 )
          if( wglewIsSupported( "WGL_EXT_swap_control" ) )
          {
            if( enable )
            {
              variables.vsync = true;
              wglSwapIntervalEXT( 1 );
            }
            else
            {
              variables.vsync = false;
              wglSwapIntervalEXT( 0 );
            }
          }
#else
#warning "no setVsync for this OS"
#endif
        }
      }

      bool OpenGL::VSync() const
      {
        return variables.vsync;
      }


    }
  }
}