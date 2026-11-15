/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    VectorField.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 12/24/2008 @ 15:35:56 - pmera
**  \brief   
*******************************************************************************/

#include "VectorField.h"

namespace SF {
namespace Physics
{

void VectorField::Build(const Vector3 &center, const Vector3 &extents, float spacing, const VectorFunction& vectorFunction)
{
  m_extents = extents;

  float width = 2 * extents.x;
  float height = 2 * extents.y;
  float length = 2 * extents.z;

  unsigned int widthSegments = (unsigned int)(width / spacing);
  unsigned int heightSegments = (unsigned int)(height / spacing);
  unsigned int lengthSegments = (unsigned int)(length / spacing);

  float x = -extents.x;

  m_center = center;

  m_vectors.resize( widthSegments );

  for(unsigned int i = 0; i < m_vectors.size(); ++i)
  {
    m_vectors[i].resize( heightSegments );
    
    float y = -extents.y;

    for(unsigned int j = 0; j < m_vectors[i].size(); ++j)
    {
      m_vectors[i][j].resize( lengthSegments );

      float z = -extents.z;

      for(unsigned int k = 0; k < m_vectors[i][j].size(); ++k)
      {
        m_vectors[i][j][k] = vectorFunction( Vector3(x,y,z) );
        
        z += spacing;
      }

      y += spacing;
    }

    x += spacing;
  }
}

Vector3 VectorField::GetVector(const Vector3& position)
{
  // TODO: avg, ie. xIndex = floor(4.25), force.x = 0.75 * vectorfield[x][y][z].x + 0.25 * vectorfield[x+1][y][z]
  // force.x = vectorfield[x][y][z].x + 0.25 * (vectorfield[x+1][y][z] - vectorfield[x][y][z].x)

  float x = std::max(0.0f, position.x + m_extents.x) / (m_extents.x*2) * m_vectors.size();
  float xFloor = std::floor( x );
  float xDiff = x - xFloor;
  size_t xIndex = std::min( (size_t)xFloor, m_vectors.size() - 1 );
  size_t xIndexNext = std::min( (size_t)xFloor, m_vectors.size() - 1 );
  
  float y = std::max(0.0f, position.y + m_extents.y) / (m_extents.y*2) * m_vectors[xIndex].size();
  float yFloor = std::floor( y );
  float yDiff = y - yFloor;
  size_t yIndex = std::min( (size_t)yFloor, m_vectors[xIndex].size() - 1 );
  size_t yIndexNext = std::min( (size_t)yFloor, m_vectors[xIndexNext].size() - 1 );

  float z = std::max(0.0f, position.z + m_extents.z) / (m_extents.z*2) * m_vectors[xIndex][yIndex].size();
  float zFloor = std::floor( z );
  float zDiff = z - zFloor;
  size_t zIndex = std::min( (size_t)zFloor, m_vectors[xIndex][yIndex].size() - 1 );
  size_t zIndexNext = std::min( (size_t)zFloor, m_vectors[xIndexNext][yIndexNext].size() - 1 );

  Vector3 force;

  force.x = (1 - xDiff) * m_vectors[xIndex][yIndex][zIndex].x + xDiff * m_vectors[xIndexNext][yIndex][zIndex].x;
  force.y = (1 - yDiff) * m_vectors[xIndex][yIndex][zIndex].y + yDiff * m_vectors[xIndex][yIndexNext][zIndex].y;
  force.z = (1 - zDiff) * m_vectors[xIndex][yIndex][zIndex].z + zDiff * m_vectors[xIndex][yIndex][zIndexNext].z;

  return force;
}

}
}
