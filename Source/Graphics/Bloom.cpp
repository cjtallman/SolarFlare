/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Bloom.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:35:39 CTallman
**  \brief   
*******************************************************************************/

#include "Bloom.h"
#include "StateManager.h"
#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"

namespace SF
{
  namespace Graphics
  {
    Bloom::Bloom( int _width, int _height )
    {
      SetFragmentShader( "blur3x3_frag", BLUR3X3 );
      SetFragmentShader( "blur5x5_frag", BLUR5X5 );
      SetFragmentShader( "blur11x11_frag", BLUR11X11 );
      SetFragmentShader( "bloomCurve", BLOOMCURVE );
      SetFragmentShader( "bloomCombine", BLOOMCOMBINE );

      for( int i = 0; i < fbo_count; ++i )
        fbo[i].Create( _width >> i, _height >> i, GL_UNSIGNED_BYTE );
    }

    Bloom::~Bloom()
    {
    }

    void Bloom::SetFragmentShader( SharedShader2 shader, int index )
    {
      frag[index] = shader;
      if( index < 3 )
        params[index] = frag[index]->GetParameter("PixelSize");
    }

    void Bloom::SetFragmentShader( const std::string & shader, int index )
    {
      frag[index] = Globals::shaderLoader->GetFragmentShader(shader);
      if( index < 3 )
        params[index] = frag[index]->GetParameter("PixelSize");
    }

    void Bloom::Resize(int _width, int _height)
    {
      for( int i = 0; i < fbo_count; ++i )
        fbo[i].Create( _width >> i, _height >> i, GL_UNSIGNED_BYTE );
    }

    void Bloom::Process( const FrameBuffer2 * main_scene )
    {
      int index = Performance::StateManager::GetGraphicsQuality();
      Render( main_scene, index );
    }

    void Bloom::Render( const FrameBuffer2 * main_scene, int index )
    {
      int passes = fbo_count;
      BloomCurve(main_scene, &fbo[0]);

      for( int i = 0; i < passes-1; ++i )
        Resample(&fbo[i], &fbo[i+1]);

      Blur(&fbo[passes-1], index);

      for( int i = 0; i < passes-1; ++i )
      {
        Resample(&fbo[passes-1-i], &fbo[passes-1-i-1]);
        Blur(&fbo[passes-1-i-1], 0);
      }

      Combine(main_scene, &fbo[0]);

      fbo[0].BindTexture();
      DrawScreenQuad();
      fbo[0].UnbindTexture();
    }

    void Bloom::BloomCurve(const FrameBuffer2 * input, FrameBuffer2 * output)
    {
      Performance::StateManager::SetActiveFragmentShader(frag[BLOOMCURVE].Get());

      input->BindTexture(0);
      output->BeginCapture();
      output->ClearTarget();
      DrawScreenQuad();
      output->EndCapture();
      input->UnbindTexture(0);
      output->SwapTargets();

      Performance::StateManager::SetActiveFragmentShader(0);
    }

    void Bloom::Resample(const FrameBuffer2 * input, FrameBuffer2 * output)
    {
      input->BindTexture(0);
      output->BeginCapture();
      DrawScreenQuad();
      output->EndCapture();
      input->UnbindTexture(0);
      output->SwapTargets();
    }

    void Bloom::Blur( FrameBuffer2 * output, int index )
    {
      Performance::StateManager::SetActiveFragmentShader(frag[index].Get());

      if( params[index]->param )
        cgSetParameter2f(
        params[index]->param, 
        1.0f / output->Width(),
        0.0f );

      output->BindTexture(0);
      output->BeginCapture();
      DrawScreenQuad();
      output->EndCapture();
      output->UnbindTexture(0);
      output->SwapTargets();

      if( params[index]->param )
        cgSetParameter2f(
        params[index]->param, 
        0.0f,
        1.0f / output->Height() );

      output->BindTexture(0);
      output->BeginCapture();
      DrawScreenQuad();
      output->EndCapture();
      output->UnbindTexture(0);
      output->SwapTargets();

      Performance::StateManager::SetActiveFragmentShader(0);
    }

    void Bloom::Combine( const FrameBuffer2 * main, FrameBuffer2 * output )
    {
      Performance::StateManager::SetActiveFragmentShader(frag[BLOOMCOMBINE].Get());

      main->BindTexture(0);
      output->BindTexture(1);
      output->BeginCapture();
      DrawScreenQuad();
      output->EndCapture();
      output->UnbindTexture(1);
      main->UnbindTexture(0);
      output->SwapTargets();

      Performance::StateManager::SetActiveFragmentShader(0);
    }


  }
}
