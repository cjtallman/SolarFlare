/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CubeGenerator.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/30/2009 @ 19:07:14 - nwilliam
**  \brief   Implementation of cube mapped NoiseArray generator.
*******************************************************************************/

#include "CubeGenerator.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  CubeGenerator::CubeGenerator() : m_faceLength( 1.0f )
  {
  }

  CubeGenerator::CubeGenerator( float length, const Math::Vector3& center) : m_faceLength( length ),
                                                                             m_center( center )
  {
  }

  CubeGenerator::~CubeGenerator()
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  float CubeGenerator::FaceLength() const
  {
    return m_faceLength;
  }

  const Math::Vector3& CubeGenerator::Center() const
  {
    return m_center;
  }

  void CubeGenerator::FaceLength( float length )
  {
    m_faceLength = length;
  }

  void CubeGenerator::Center( const Math::Vector3& center )
  {
    m_center = center;
  }

  void CubeGenerator::Build()
  {
    BuildFace( CubeGenerator::LEFT );
  }

  void CubeGenerator::BuildFace( Faces face )
  {
    switch( face )
    {
      case CubeGenerator::TOP:
      case CubeGenerator::BOTTOM:
        BuildTopBottom( face );
      break;
      
      case CubeGenerator::LEFT:
      case CubeGenerator::RIGHT:
        BuildLeftRight( face );
      break;
      
      case CubeGenerator::FRONT:
      case CubeGenerator::BACK:
        BuildFrontBack( face );
      break;
    }
  }

  unsigned char* CubeGenerator::BuildFaceTexture( Faces face )
  {
    BuildFace( face );
    return BaseGenerator::BuildTexture( false );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void CubeGenerator::BuildTopBottom( Faces face )
  {
    float halfWidth = m_faceLength / 2;
    NoiseVal y = m_center.y - halfWidth;

    int width = m_array.Width();
    int depth = m_array.Height();

    NoiseVal xRange = m_faceLength;
    NoiseVal zRange = m_faceLength;
    NoiseVal dx  = xRange / (NoiseVal)width;
    NoiseVal dz  = -zRange / (NoiseVal)depth;
    NoiseVal zVal = m_center.z + halfWidth;
    NoiseVal xMod = -halfWidth;
    
    if( face == CubeGenerator::TOP )
    {
      y = -y;
      dz = -dz;
      zVal = -zVal;
    }
    
    NoiseVal* arrData = m_array.Data();
    NoiseValue3D noise = GetNoise3D( m_data.noiseType );

    for( int z = 0; z < depth; ++z )
    {
      NoiseVal xVal = m_center.x + xMod;
      
      for( int x = 0; x < width; ++x )
      {
        *arrData++ =noise( m_data, xVal, y, zVal );
        xVal += dx;
      }

      zVal += dz;
    }
  }

  void CubeGenerator::BuildFrontBack( Faces face )
  {
    int width = m_array.Width();
    int height = m_array.Height();
    float halfWidth = m_faceLength / 2;
    NoiseVal z = m_center.y + halfWidth;
    NoiseVal xRange = m_faceLength;
    NoiseVal yRange = m_faceLength;
    NoiseVal dx  = xRange / (NoiseVal)width;
    NoiseVal dy  = -yRange / (NoiseVal)height;
    NoiseVal yVal = m_center.y + halfWidth;
    NoiseVal xMod = -halfWidth;

    if( face == CubeGenerator::BACK )
    {
      z = -z;
      dx = -dx;
      xMod = -xMod;
    }

    NoiseVal* arrData = m_array.Data();
    NoiseValue3D noise = GetNoise3D( m_data.noiseType );

    for( int y = 0; y < height; ++y )
    {
      NoiseVal xVal = m_center.x  + xMod;

      for( int x = 0; x < width; ++x )
      {
        *arrData++ = noise( m_data, xVal, yVal, z );
        xVal += dx;
      }

      yVal += dy;
    }
  }

  void CubeGenerator::BuildLeftRight( Faces face )
  {
    float halfWidth = m_faceLength / 2;
    NoiseVal x = m_center.x - halfWidth;

    int depth = m_array.Width();
    int height = m_array.Height();

    NoiseVal zRange = m_faceLength;
    NoiseVal yRange = m_faceLength;
    NoiseVal dz  = zRange / (NoiseVal)depth;
    NoiseVal dy  = yRange / (NoiseVal)height;
    NoiseVal yVal = m_center.y + halfWidth;
    NoiseVal zMod = -halfWidth;

    if( face == CubeGenerator::RIGHT )
    {
      x = -x;
      dz = - dz;
      zMod = -zMod;
    }

    NoiseVal* arrData = m_array.Data();
    NoiseValue3D noise = GetNoise3D( m_data.noiseType );

    for( int y = 0; y < height; ++y )
    {
      NoiseVal zVal = m_center.z + zMod;

      for( int z = 0; z < depth; ++z )
      {
        *arrData++ = noise( m_data, x, yVal, zVal );
        zVal += dz;
      }

      yVal -= dy;
    }
  }

} // Noise namespace
} // Utilities namespace
} // SF namespace
