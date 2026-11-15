/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TTVector3_Explicit.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/12/2009 @ 12:02:52 - nwilliam
**  \brief   Explicit implementations for Vector3, only required because of
             circular dependency between Vector3 and Quaternion.
*******************************************************************************/

#include "TVector3.h"
#include "Quaternion.h"

namespace SF {
namespace Math
{
  template<>
  void TVector3<float>::Rotate( const Quaternion& Q )
  {
    Quaternion Vector( *this, 0 );

    *this = ( Q * Vector * Q.Conjugate() ).v;
  }

  template<>
  void TVector3<float>::RotateInverse( const Quaternion& Q )
  {
    Quaternion Vector( *this, 0 );

    *this = ( Q.Conjugate() * Vector * Q ).v;
  }

} // Math namespace
} // SF namespace
