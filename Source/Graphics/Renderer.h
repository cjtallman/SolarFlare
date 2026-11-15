/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Renderer.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 00:59 - CJT 
**  \brief   This is a simple OpenGL wrapper
*******************************************************************************/

#ifndef SF_OPENGL_RENDERER_H_
#define SF_OPENGL_RENDERER_H_

#include "Utilities/Color.h"



namespace SF
{
  namespace Graphics
  {
    struct OSWindowInfo;

    namespace Renderer
    {
      class OpenGL
      {
      public:
        void CheckMinimumRequirements();

        OpenGL();
        ~OpenGL();

        bool Init( void * data1, void * data2 );

        int Begin();
        int End();

        void PerspectiveProjection();
        void PerspectiveProjection(double fov, double aspect, double nearplane = 0.1, double farplane = 1000.0);
        void OrthographicProjection();
        void Viewport( int x, int y, int w, int h );

        void ResizeWindow( int width, int height );

        int ClearAllBuffers();
        int ClearColorBuffer();
        int ClearDepthBuffer();

        void Lighting( bool enable );
        bool Lighting() const;
        
        void Wireframe( bool enable );
        bool Wireframe() const;

        void ColorWrites( bool enable );
        bool ColorWrites() const;

        void DepthTest( bool enable );
        bool DepthTest() const;

        void DepthWrites( bool enable );
        bool DepthWrites() const;

        void AspectRatio( double aspect );
        double AspectRatio() const;

        void FOV( double fov );
        double FOV() const;

        void ColorClearValue( const Color4f & value );
        Color ColorClearValue() const;

        void DepthClearValue( float value );
        float DepthClearValue() const;

        void WindowWidth( int width );
        int WindowWidth() const;

        void WindowHeight( int height );
        int WindowHeight() const;

        void SetLightAmbient( const Color4f & color );
        void SetLightDiffuse( const Color4f & color );
        void SetLightSpecular( const Color4f & color );
        void SetLightPercent( float percent );

        void VSync( bool enable );
        bool VSync() const;

        int OpenGLErrors() const;

      private:
        
        bool Startup();
        void Shutdown();
        int Present();
        

        struct _RenderingVariables
        {
          int window_width;
          int window_height;
          Color4f light0_ambient;
          Color4f light0_diffuse;
          Color4f light0_specular;
          float light0_percent;
          Color4f clear_color;
          float clear_depth;
          bool color_writes;
          bool depth_test;
          bool depth_writes;
          bool lighting;
          bool wireframe;
          bool vsync;
          double aspect_ratio;
          double fov;
        }variables;

        OSWindowInfo * window;
      };
    }
  }
}

#endif
