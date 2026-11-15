/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SphereGenerator.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 01/29/2009 @ 16:23:00 - nwilliam
**  \brief   Implementation of spherically mapped noise generator.
*******************************************************************************/

#include <cmath> // sin, cos
#include <vector>
#include "SphereGenerator.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  SphereGenerator::SphereGenerator()
  {
  }

  SphereGenerator::SphereGenerator( int width, int height )
  {
    m_array.Resize( width, height );
  }

  SphereGenerator::~SphereGenerator()
  {
  }

  void SphereGenerator::Build()
  {
    int width = m_array.Width();
    int height = m_array.Height();
    
    std::vector<float> sinPhi, cosPhi;
    NoiseVal dp = Math::Constants::pi / height;
    
    for( int i = 0; i < height; ++i )
    {
      NoiseVal phi = Math::Constants::f_pi_2 - ( dp * i );
      sinPhi.push_back( std::sin( phi ) );
      cosPhi.push_back( std::cos( phi ) );
    }

    std::vector<float> sinTheta, cosTheta;
    NoiseVal dt = Math::Constants::p_2_pi / width;

    for( int i = 0; i < width; ++i )
    {
      NoiseVal theta = dt * i;
      sinTheta.push_back( std::sin( theta ) );
      cosTheta.push_back( std::cos( theta ) );
    }

    NoiseVal* arrData = m_array.Data();
    NoiseValue3D noise = GetNoise3D( m_data.noiseType );

    for( int i = 0; i < height; ++i )
    {
      for( int j = 0; j < width; ++j )
      {
        NoiseVal x = sinTheta[j] * cosPhi[i];
        NoiseVal y = sinPhi[i];
        NoiseVal z = cosTheta[j] * cosPhi[i];
        
        *arrData++ = noise( m_data, x, y, z );
      }
    }
  }

  void SphereGenerator::BuildAnim()
  {
    int width = m_array.Width();
    int height = m_array.Height();

    std::vector<float> sinPhi, cosPhi;
    NoiseVal dp = Math::Constants::pi / height;

    for( int i = 0; i < height; ++i )
    {
      NoiseVal phi = Math::Constants::f_pi_2 - ( dp * i );
      sinPhi.push_back( std::sin( phi ) );
      cosPhi.push_back( std::cos( phi ) );
    }

    std::vector<float> sinTheta, cosTheta;
    NoiseVal dt = Math::Constants::p_2_pi / width;

    for( int i = 0; i < width; ++i )
    {
      NoiseVal theta = dt * i;
      sinTheta.push_back( std::sin( theta ) );
      cosTheta.push_back( std::cos( theta ) );
    }

    NoiseVal time = m_data.time;
    NoiseVal* arrData = m_array.Data();
    NoiseValue4D noise = GetNoise4D( m_data.noiseType );

    for( int i = 0; i < height; ++i )
    {
      for( int j = 0; j < width; ++j )
      {
        NoiseVal x = sinTheta[j] * cosPhi[i];
        NoiseVal y = sinPhi[i];
        NoiseVal z = cosTheta[j] * cosPhi[i];

        *arrData++ = noise( m_data, x, y, z, time );
      }
    }
  }

} // Noise namespace
} // Utilities namespace
} // SF namespace
