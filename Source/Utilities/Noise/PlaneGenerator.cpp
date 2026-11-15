/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    PlaneGenerator.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/30/2009 @ 19:02:25 - nwilliam
**  \brief   Implementation of plane mapping generator
*******************************************************************************/

#include "PlaneGenerator.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors / Destructor
  //////////////////////////////////////////////////////////////////////////////
  PlaneGenerator::PlaneGenerator()
  {
    SetBounds( 0, 4, 0, 4 );
  }

  PlaneGenerator::~PlaneGenerator()
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  void PlaneGenerator::Build()
  {
    int width = m_array.Width();
    int height = m_array.Height();

    NoiseVal xRange = m_xUpper - m_xLower;
    NoiseVal yRange = m_yUpper - m_yLower;
    NoiseVal dx  = xRange / static_cast<NoiseVal>( width );
    NoiseVal dy  = yRange / static_cast<NoiseVal>( height );
    NoiseVal yVal = m_yLower;

    NoiseVal* arrData = m_array.Data();
    NoiseValue2D noise = GetNoise2D( m_data.noiseType );

    for( int y = 0; y < height; ++y )
    {
      NoiseVal xVal = m_xLower;
      
      for( int x = 0; x < width; ++x )
      {
        *arrData++ = noise( m_data, xVal, yVal );
        xVal += dx;
      }
      
      yVal += dy;
    }
  }

  NoiseVal PlaneGenerator::xLower() const
  {
    return m_xLower;
  }

  NoiseVal PlaneGenerator::yLower() const
  {
    return m_yLower;
  }

  NoiseVal PlaneGenerator::xUpper() const
  {
    return m_xUpper;
  }

  NoiseVal PlaneGenerator::yUpper() const
  {
    return m_yUpper;
  }

  void PlaneGenerator::xLower( NoiseVal xLower )
  {
    m_xLower= xLower;
  }

  void PlaneGenerator::yLower( NoiseVal yLower )
  {
    m_yLower= yLower;
  }

  void PlaneGenerator::xUpper( NoiseVal xUpper )
  {
    m_xUpper = xUpper;
  }

  void PlaneGenerator::yUpper( NoiseVal yUpper )
  {
    m_yUpper = yUpper;
  }

  void PlaneGenerator::SetBounds( NoiseVal xLower, NoiseVal xUpper, NoiseVal yLower, NoiseVal yUpper )
  {
    m_xLower= xLower;
    m_yLower= yLower;
    m_xUpper = xUpper;
    m_yUpper = yUpper;
  }

} // Noise namespace
} // Utilities namespace
} // SF namespace


