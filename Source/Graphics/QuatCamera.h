/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    QuatCamera.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/19/2008 @ 18:25:45 - pmera
**  \brief   Quaternion based camera class.
*******************************************************************************/

#ifndef SF_QUAT_CAMERA_H
#define SF_QUAT_CAMERA_H

#include "Math/TVector3.h"
#include "Math/TMatrix4x4.h"
#include "Math/Quaternion.h"
#include "Math/CTransform.h"
#include "External/Lua/include/LunarHelper.h"

using namespace SF;
using namespace Math;

namespace SF {
namespace Graphics
{
  class QuatCamera
  {
    public:
      QuatCamera();
      
      Matrix4 & GetViewMatrix();
      const Matrix4 & GetViewMatrix() const;
      
      Matrix4 & GetInverseViewMatrix();
      const Matrix4 & GetInverseViewMatrix() const;

      void EnableGeosynchOrbit();
      void DisableGeosynchOrbit();

      Vector3     GetPosition() const;
      Quaternion  GetOrientation() const;

      void SetTargetOffset( const Vector3& offset, float time = 0.0f );
      void SetTargetOffsetTime( float time );

      void SetRotationOffset( const Quaternion& offset, float time = 0.0f );
      void SetRotationOffsetTime( float time );

      void AddRadius( float r, float time = 0.0f );
      void SetRadius( float r, float time = 0.0f );
      void MultiplyRadius( float r, float time = 0.0f );
      void SetRadiusTime( float time );

      void SetTarget( const Vector3& target, float time = 0.0f );
      void SetTarget( CTransform *target, float time = 0.0f );
      void SetTargetTime( float time );
      void SetRotationTime( float time );

        // Set absolute orientation
      void SetRotation( const Quaternion& q, float time = 0.0f );
      void SetRotation( float yaw, float pitch, float roll, float time = 0.0f );
      
        // Set relative rotation
      void Rotate( float yaw, float pitch, float roll );
      void Rotate( float yaw, float pitch, float roll, float time );
      
      void InterpolateRadius( float time_elapsed );
      void InterpolateOrientation( float time_elapsed );
      void InterpolateTarget( float timeElapsed );
      void InterpolateTargetOffset( float timeElapsed );
      void InterpolateRotationOffset( float timeElapsed );

      void Update( float time_elapsed );
      
      // Target & Orientation
      CTransform Transform;

    private:
      void BuildTransform();

      void HacksCheck();

      bool        m_targetMode;

      Vector3     m_eye;

        // Target
      Vector3     m_target;
      Vector3     m_targetStart;

        // Target Offset
      Vector3     m_targetOffset;
      Vector3     m_targetOffsetStart;
      Vector3     m_targetOffsetEnd;
      
        // Rotation Offset
      Quaternion  m_rotationOffset;
      Quaternion  m_rotationOffsetStart;
      Quaternion  m_rotationOffsetEnd;

        // Radius
      float       m_radius;
      float       m_radiusStart;
      float       m_radiusEnd;

        // Orientation
      Quaternion  m_orientation;
      Quaternion  m_orientationStart;
      float       m_yaw;
      float       m_pitch;
      float       m_roll;
      bool        m_constrainRoll;
      
      // The view transform
      Matrix4     m_viewTransform;
      Matrix4     m_inverseViewTransform;
      
      // Interpolation timers
      float       m_rotationTime;
      float       m_rotationTimeScale;
      
      float       m_radiusTime;
      float       m_radiusTimeScale;

      float       m_targetTime;
      float       m_targetTimeScale;
      
      float       m_targetOffsetTime;
      float       m_targetOffsetTimeScale;

      float       m_rotationOffsetTime;
      float       m_rotationOffsetTimeScale;

      bool        m_geosynchOrbit;
      
    LUA_DECLARE_CLASS( QuatCamera )

    LUA_DECLARE_METHOD( LEnableRoll )
    LUA_DECLARE_METHOD( LDisableRoll )

    LUA_DECLARE_METHOD( LTargetMode )
    LUA_DECLARE_METHOD( LEnableTargetMode )
    LUA_DECLARE_METHOD( LDisableTargetMode )

    LUA_DECLARE_METHOD( LEnableGeosynchOrbit )
    LUA_DECLARE_METHOD( LDisableGeosynchOrbit )

    LUA_DECLARE_METHOD( LRotate )
    LUA_DECLARE_METHOD( LSetRotation )
    LUA_DECLARE_METHOD( LGetRotationAngles )

    LUA_DECLARE_METHOD( LSetRadius )
    LUA_DECLARE_METHOD( LGetRadius )
    LUA_DECLARE_METHOD( LMultiplyRadius )

    LUA_DECLARE_METHOD( LMove )

    LUA_DECLARE_METHOD( LSetRadiusTime )
    LUA_DECLARE_METHOD( LSetRotationTime )

    LUA_DECLARE_METHOD( LSetTarget )
    LUA_DECLARE_METHOD( LSetTargetTime )

    LUA_DECLARE_METHOD( LGetTargetOffset )
    LUA_DECLARE_METHOD( LSetTargetOffset )
    LUA_DECLARE_METHOD( LSetTargetOffsetTime )

    LUA_DECLARE_METHOD( LGetUpVector )

    LUA_DECLARE_METHOD( LGetWorldOrientation )
    LUA_DECLARE_METHOD( LGetWorldEye )
    LUA_DECLARE_METHOD( LGetWorldView )

    LUA_DECLARE_METHOD( LUnfocus )

    LUA_DECLARE_METHOD( LTransform )
  };
} // Graphics namespace
} // SF namespaace

#endif // SF_QUAT_CAMERA_H
