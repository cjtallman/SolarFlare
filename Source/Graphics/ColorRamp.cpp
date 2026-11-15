/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ColorRamp.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/25/2009 @ 00:20 - CJT 
**  \brief   A 1D texture.
*******************************************************************************/

#include <cstdlib> // rand

#include "OpenGLInclude.h"
#include "Utilities/Image.h"
#include "Utilities/Macros.h"
#include "ColorRamp.h"

#include "StateManager.h"

#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    namespace TextureType
    {
      ColorRamp::ColorRamp() : max_value(-1), min_value(1)
      {
        CheckThisForErrors(glGenTextures(1, &handle));
        Bind();
        CheckThisForErrors(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        CheckThisForErrors(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        CheckThisForErrors(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        CheckThisForErrors(glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 ));
        Unbind();
        
        image = new Utilities::Image(512, 1, Utilities::Image::RGB);
        stage = -1;

        AddColor(-1.0000, SF::Color (  0,   0, 0, 255));
        AddColor( 1.0000, SF::Color (255, 255, 255, 255));
        Recreate();
      }

      ColorRamp::~ColorRamp()
      {
        CheckThisForErrors(glDeleteTextures(1, &handle ));
      }

      ColorRamp & ColorRamp::operator =(const ColorRamp & A)
      {
        colors = A.colors;
        min_value = A.min_value;
        max_value = A.max_value;
        image = A.image;
        Recreate();
        return *this;
      }

      void ColorRamp::Bind( int _stage )
      {
        Performance::StateManager::EnableTexture1D(handle,_stage);
        stage = _stage;
      }

      void ColorRamp::Unbind()
      {
        Performance::StateManager::DisableTexture1D(stage);
      }

      void ColorRamp::AddColor( const float & _position, const SF::Color & _color )
      {
        min_value = std::min(min_value, _position);
        max_value = std::max(max_value, _position);
        colors[_position] = _color;
      }

      void ColorRamp::MakeColorRamp()
      {
        std::map<float, SF::Color>::iterator iter = colors.begin();
        if( colors.size() > 1 )
        {
          std::pair<float, SF::Color> lower;
          std::pair<float, SF::Color> upper;

          lower = *iter; ++iter;
          upper = *iter;

          int width = image->Width();

          float dt = max_value - min_value;
          dt /= width;

          for( int i = 0; i < width; ++i )
          {
            float current_position = min_value + i*dt;

            if( current_position > upper.first )
            {
              lower = upper;
              ++iter;
              upper = *iter;
            }

            float t = (current_position - lower.first) / (upper.first - lower.first);

            Color C0 = lower.second;
            Color C1 = upper.second;
            Color current_color = Color::Interpolate( Color(C0.r,C0.g, C0.b, C0.a), Color(C1.r, C1.g, C1.b, C1.a), t);

            image->SetTexel(0,i, SF::Color(
              (Color::Type)current_color.r,
              (Color::Type)current_color.g,
              (Color::Type)current_color.b,
              (Color::Type)current_color.a) );
          }
        }
      }

      void ColorRamp::Recreate(int _stage)
      {
        MakeColorRamp();
        Bind(_stage);
        CheckThisForErrors(glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, image->Width(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->PixelData() ));
        Unbind();
      }

      void ColorRamp::ClearColors()
      {
        colors.clear();
        max_value = -1;
        min_value = 1;
      }

      void ColorRamp::RandomizeColors()
      {
          ClearColors();

          int num = 1 + std::rand() % 3;
          int temp = 0;
          AddColor(0, SF::Color( 255, 0, 0, 255 ) );
          AddColor(.05f, SF::Color( 255, 0, 0, 255 ) );
          AddColor(1,  SF::Color( (unsigned char)(rand()%255), (unsigned char)(rand()%255), (unsigned char)(rand()%255), 255 ) );

          for( int i = 0; i < num; ++i )
          {
            float f =  ( (temp + rand()) % (500-i)) / 500.0f;
            temp = (int)(f*500*num);

            int r = (temp + rand() % 256);
            int g = (temp + rand() % 256);
            int b = (temp + rand() % 256);

            SF::Color color( (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 );
            AddColor( f, color );
          }

          Recreate();
      }

      // Networking
      Networking::ByteStream & operator >> ( Networking::ByteStream & bs, ColorRamp & color )
      {
        int num_colors;
        bs >> num_colors;

        color.ClearColors();
        for( int i = 0; i < num_colors; ++i )
        {
          float pos;
          Color col;
          bs >> pos >> col.r >> col.g >> col.b >> col.a;
          color.AddColor( pos, col );
        }

        color.Recreate();
        return bs;
      }

      Networking::ByteStream & operator << ( Networking::ByteStream & bs, const ColorRamp & color )
      {
        int count = (int)color.colors.size();
        bs << count;

        for( std::map<float,Color>::const_iterator i = color.colors.begin(); i != color.colors.end(); ++i )
        {
          bs << i->first;
          bs << i->second.r << i->second.g << i->second.b << i->second.a;
        }

        return bs;
      }


    }
  }
}
