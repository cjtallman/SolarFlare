/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CubeGenerator.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/30/2009 @ 19:06:55 - nwilliam
**  \brief   NoiseArray generator that maps output to a cube.
*******************************************************************************/

#ifndef SF_CUBE_GENERATOR_H
#define SF_CUBE_GENERATOR_H

#include "Math/TVector3.h"
#include "BaseGenerator.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  class CubeGenerator : public BaseGenerator
  {
    public:
      enum Faces { LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK };

      CubeGenerator();
      CubeGenerator( float length, const Math::Vector3& center );
      virtual ~CubeGenerator();

      float FaceLength() const;
      void FaceLength( float length );

      const Math::Vector3& Center() const;
      void Center( const Math::Vector3& center );

      virtual void Build();
      void BuildFace( Faces face );
      unsigned char* BuildFaceTexture( Faces face );

    private:
      void BuildTopBottom( Faces face );
      void BuildFrontBack( Faces face );
      void BuildLeftRight( Faces face );

      float m_faceLength;
      Math::Vector3 m_center;
  };

} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_CUBE_GENERATOR_H
