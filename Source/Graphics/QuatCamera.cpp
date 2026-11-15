/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    QuatCamera.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/19/2008 @ 18:25:53 - pmera
**  \brief   Implementation of the quaternion based camera.
*******************************************************************************/

#include <cmath>
#include <cfloat>
#include "QuatCamera.h"
#include "Utilities/Misc.h"
#include "Utilities/Asserts.h"

namespace SF {
namespace Graphics {
  
  //////////////////////////////////////////////////////////////////////////////
  // Constructor
  //////////////////////////////////////////////////////////////////////////////
  QuatCamera::QuatCamera()
  {
    m_radius = 1500.0f;
    m_radiusStart = 1500.0f;
    m_radiusEnd = 1500.0f;
    
    m_rotationTime = 0.0f;
    m_rotationTimeScale = 1.0f;
    
    m_radiusTime = 0.0f;
    m_radiusTimeScale = 1.0f;

    m_targetTime = 0.0f;
    m_targetTimeScale = 1.0f;

    m_targetOffsetTime = 0.0f;
    m_targetOffsetTimeScale = 1.0f;

    m_yaw = 0.0f;
    m_pitch = 0.0f;
    m_roll = 0.0f;

    m_targetMode = true;
    
    m_constrainRoll = true;

    m_geosynchOrbit = false;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Public methods 
  //////////////////////////////////////////////////////////////////////////////
  Matrix4 & QuatCamera::GetViewMatrix()
  {
    return m_viewTransform;
  }

  const Matrix4 & QuatCamera::GetViewMatrix() const
  {
    return m_viewTransform;
  }

  Matrix4 & QuatCamera::GetInverseViewMatrix()
  {
    return m_viewTransform;
  }

  const Matrix4 & QuatCamera::GetInverseViewMatrix() const
  {
    return m_viewTransform;
  }

  void QuatCamera::EnableGeosynchOrbit()
  {
    Quaternion parentInverse = ( Transform.GetWorldOrientation() * Transform.localOrientation.Conjugate() ).Conjugate();

    SetRotation( parentInverse * Transform.localOrientation, 1 );

    m_geosynchOrbit = true;
  }

  void QuatCamera::DisableGeosynchOrbit()
  {
    SetRotation( Transform.GetWorldOrientation() );

    m_geosynchOrbit = false;
  }

  Vector3 QuatCamera::GetPosition() const
  {
    return m_eye;
  }

  Quaternion QuatCamera::GetOrientation() const
  {
    return m_orientation;
  }

  
  void QuatCamera::SetRadius( float r, float time )
  {
    if(r < 0)
      r = -r;
    
    m_radiusStart = m_radius;
    m_radiusEnd = r;

    SetRadiusTime( time );
  }
  
  void QuatCamera::AddRadius( float r, float time )
  {
    if(m_radius + r < 0)
      r = -r;
    
    m_radiusStart = m_radius;
    m_radiusEnd = m_radius + r;

    SetRadiusTime( time );
  }
  
  void QuatCamera::MultiplyRadius( float r, float time )
  {
    if(m_radius * r < 0)
      r = -r;
    
    m_radiusStart = m_radius;
    m_radiusEnd = m_radius * r;

    SetRadiusTime( time );
  }

  void QuatCamera::SetRadiusTime( float time )
  {
    if( time > 1e-5f )
    {
      m_radiusTime = 0.0f;
      m_radiusTimeScale = 1.0f / time;
    }
    else
    {
      m_radiusStart = m_radius = m_radiusEnd;

      m_radiusTime = 1.0f;
      m_radiusTimeScale = 1.0f;
    }
  }
  
  void QuatCamera::SetTarget( const Vector3& target, float time )
  {
    m_targetStart = m_target;
    m_orientationStart = m_orientation;

    Transform.localPosition = target;
    Transform.localOrientation = m_orientation;
    Transform.Orphan();

    SetTargetTime( time );
  }

  void QuatCamera::SetTarget( CTransform *target, float time )
  {
    m_targetStart = m_target;
    m_orientationStart = m_orientation;

    Transform.localPosition = Math::Vector3();

    Transform.Orphan();
    Transform.SetParent(target);

    SetTargetTime( time );
  }

  void QuatCamera::SetTargetTime( float time )
  {
    if( time > 1e-5f )
    {
      m_targetTime = 0.0f;
      m_targetTimeScale = 1.0f / time;
    }
    else
    {
      m_targetStart = m_target = Transform.localPosition;
      m_targetTime = 1.0f;
      m_targetTimeScale = 1.0f;
    }
  }


  void QuatCamera::SetTargetOffset( const Vector3& offset, float time )
  {
    m_targetOffsetStart = m_targetOffset;
    m_targetOffsetEnd = offset;

    SetTargetOffsetTime( time );
  }

  void QuatCamera::SetTargetOffsetTime( float time )
  {
    if( time > 1e-5f )
    {
      m_targetOffsetTime = 0.0f;
      m_targetOffsetTimeScale = 1.0f / time;
    }
    else
    {
      m_targetOffsetStart = m_targetOffset = m_targetOffsetEnd;

      m_targetOffsetTime = 1.0f;
      m_targetOffsetTimeScale = 1.0f;
    }
  }

  void QuatCamera::SetRotationOffset( const Quaternion& offset, float time ) 
  {
    m_rotationOffsetStart = m_rotationOffset;
    m_rotationOffsetEnd = offset;

    SetRotationOffsetTime( time );
  }

  void QuatCamera::SetRotationOffsetTime( float time )
  {
    if( time > 1e-5f )
    {
      m_rotationOffsetTime = 0.0f;
      m_rotationOffsetTimeScale = 1.0f / time;
    }
    else
    {
      m_rotationOffsetStart = m_rotationOffset = m_rotationOffsetEnd;

      m_rotationOffsetTime = 1.0f;
      m_rotationOffsetTimeScale = 1.0f;
    }
  }

  void QuatCamera::SetRotationTime( float time )
  {
    if( time > 1e-5f )
    {
      m_rotationTime = 0.0f;
      m_rotationTimeScale = 1.0f / time;
    }
    else
    {
      m_orientation = m_orientationStart = Transform.localOrientation;
      m_rotationTime = 1.0f;
      m_rotationTimeScale = 1.0f;
    }
  }

  void QuatCamera::SetRotation( const Quaternion& q, float time )
  {
    m_orientationStart = m_orientation;
    
    Transform.localOrientation = q;

    m_yaw = Transform.localOrientation.GetYaw();
    m_pitch = Transform.localOrientation.GetPitch();
    m_roll = Transform.localOrientation.GetRoll();

    SetRotationTime( time );
  }

  void QuatCamera::SetRotation(float yaw, float pitch, float roll, float time)
  {
      // Set our new "initial" oriention from which we will be interpolating
    m_orientationStart = m_orientation;
    
    m_yaw = yaw;
    m_pitch = pitch;
    m_roll = m_constrainRoll ? 0 : roll;

    Transform.localOrientation = Quaternion::EulerAngles(m_yaw, m_pitch, m_roll);

    if( time > 1e-5f )
    {
      m_rotationTime = 0.0f;
      m_rotationTimeScale = 1.0f / time;
    }
    else
    {
      m_orientation = m_orientationStart = Transform.localOrientation;
      m_rotationTime = 1.0f;
      m_rotationTimeScale = 1.0f;
    }
  }

  void QuatCamera::Rotate( float yaw, float pitch, float roll )
  {
    m_yaw += yaw;

    while( m_yaw > 3.14f )
      m_yaw -= 6.28f;
    
    while( m_yaw < -3.14f )
      m_yaw += 6.28f;

    if( m_yaw > 1.57f || m_yaw < -1.57f )
      m_pitch -= pitch;
    else
      m_pitch += pitch;

    if( m_constrainRoll )
    {
      Transform.localOrientation = Quaternion::EulerAngles(m_yaw, m_pitch, m_roll);
    }
    else
    {
      Transform.localOrientation.RotateLocal(yaw, pitch, roll);
    }
  }

  void QuatCamera::Rotate(float yaw, float pitch, float roll, float time)
  {
      // Set our new "initial" oriention from which we will be interpolating
    m_orientationStart = m_orientation;
    
    m_yaw += yaw;

    while( m_yaw > 3.14f )
      m_yaw -= 6.28f;
    
    while( m_yaw < -3.14f )
      m_yaw += 6.28f;

    if( m_yaw > 1.57f || m_yaw < -1.57f )
      m_pitch -= pitch;
    else
      m_pitch += pitch;

    if( m_constrainRoll )
    {
      Transform.localOrientation = Quaternion::EulerAngles(m_yaw, m_pitch, m_roll);
    }
    else
    {
      Transform.localOrientation.RotateLocal(yaw, pitch, roll);
    }

    SetRotationTime( time );
  }
  
  void QuatCamera::InterpolateOrientation(float time_elapsed)
  {
    HacksCheck();

    m_rotationTime += time_elapsed;
    
    float t = Utilities::Clamp(m_rotationTime * m_rotationTimeScale, 0.0f, 1.0f);
    
    t = 1.0f - (t-1)*(t-1);
    
    Quaternion m_orientationEnd;

    if(m_geosynchOrbit)
      m_orientationEnd = Transform.GetWorldOrientation();
    else
      m_orientationEnd = Transform.localOrientation;

    m_orientation = Math::Slerp(m_orientationStart, m_orientationEnd, t);
  }
  
  void QuatCamera::InterpolateRadius(float time_elapsed)
  {
    m_radiusTime += time_elapsed;
    
    float t = Utilities::Clamp(m_radiusTime * m_radiusTimeScale, 0.0f, 1.0f);
    
    t = 1.0f - (t-1)*(t-1);
    
    m_radius = m_radiusStart * (1.0f - t) + m_radiusEnd * t;
  }

  void QuatCamera::InterpolateTarget( float timeElapsed )
  {
    m_targetTime += timeElapsed;
    
    float t = Utilities::Clamp(m_targetTime * m_targetTimeScale, 0.0f, 1.0f);
    
    t = 1.0f - (t-1)*(t-1);
    
    m_target = m_targetStart * (1.0f - t) + Transform.GetWorldPosition() * t;
  }
  
  void QuatCamera::InterpolateTargetOffset( float timeElapsed )
  {
    m_targetOffsetTime += timeElapsed;
    
    float t = Utilities::Clamp(m_targetOffsetTime * m_targetOffsetTimeScale, 0.0f, 1.0f);
    
    t = 1.0f - (t-1)*(t-1);
    
    m_targetOffset = m_targetOffsetStart * (1.0f - t) + m_targetOffsetEnd * t;
  }

  void QuatCamera::InterpolateRotationOffset( float timeElapsed )
  {
    m_rotationOffsetTime += timeElapsed;

    float t = Utilities::Clamp(m_rotationOffsetTime * m_rotationOffsetTimeScale, 0.0f, 1.0f);

    t = 1.0f - (t-1)*(t-1);

    m_rotationOffset = m_rotationOffsetStart * (1.0f - t) + m_rotationOffsetEnd * t;
  }

  void QuatCamera::BuildTransform()
  {
    ASSERT_MSG(
      std::fabs( m_orientation.Magnitude() - 1.0f ) < 0.001f,
      "Attempt to build view transform with non-unit quaternion!" );

    Quaternion orientation = m_orientation;

    orientation *= m_rotationOffset;

    Vector3 right = orientation.LocalX();
    Vector3 up = orientation.LocalY();
    Vector3 view = orientation.LocalZ();

    if( m_targetMode )
    {
      Vector3 offsetTarget = m_target + right * m_targetOffset.x + up * m_targetOffset.y + view * m_targetOffset.z;

        // Set our camera's position based on the target and view vectors, and the radius
      m_eye = offsetTarget - view * m_radius;
    }

      // Time to build a view transform!
    m_viewTransform[0] = -right.x;
    m_viewTransform[4] = -right.y;
    m_viewTransform[8] = -right.z;
    m_viewTransform[12] = right * m_eye;
    
    m_viewTransform[1] = up.x;
    m_viewTransform[5] = up.y;
    m_viewTransform[9] = up.z;
    m_viewTransform[13] = -up * m_eye;
    
    m_viewTransform[2] = -view.x;
    m_viewTransform[6] = -view.y;
    m_viewTransform[10] = -view.z;
    m_viewTransform[14] = view * m_eye;
    
    m_viewTransform[3] = 0;
    m_viewTransform[7] = 0;
    m_viewTransform[11] = 0;
    m_viewTransform[15] = 1;

      // Building an inverse view transform
    m_inverseViewTransform[0] = -right.x;
    m_inverseViewTransform[4] = up.x;
    m_inverseViewTransform[8] = -view.x;
    m_inverseViewTransform[12] = m_eye.x;
    
    m_inverseViewTransform[1] = -right.y;
    m_inverseViewTransform[5] = up.y;
    m_inverseViewTransform[9] = -view.y;
    m_inverseViewTransform[13] = m_eye.y;
    
    m_inverseViewTransform[2] = -right.z;
    m_inverseViewTransform[6] = up.z;
    m_inverseViewTransform[10] = -view.z;
    m_inverseViewTransform[14] = m_eye.z;
    
    m_inverseViewTransform[3] = 0;
    m_inverseViewTransform[7] = 0;
    m_inverseViewTransform[11] = 0;
    m_inverseViewTransform[15] = 1;
  }

  void QuatCamera::Update(float timeElapsed)
  {
    InterpolateRadius( timeElapsed );
    InterpolateOrientation( timeElapsed );
    InterpolateTarget( timeElapsed );
    InterpolateTargetOffset( timeElapsed );
    InterpolateRotationOffset( timeElapsed );

    BuildTransform();
  }
  

  // TODO: Figure out why the camera would ever get a bad orientation. Until then... ya...
  void QuatCamera::HacksCheck()
  {
    if( _isnan(m_orientation.v.x) || _isnan(m_orientation.v.y) ||
        _isnan(m_orientation.v.z) || _isnan(m_orientation.w) ||
        _isnan(m_yaw) || _isnan(m_pitch) || _isnan(m_roll) )
    {
      SetRotation(0,0,0,0);
    }
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Lua methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( QuatCamera )
    LUA_NAME_METHOD( QuatCamera, "EnableRoll", LEnableRoll )
    LUA_NAME_METHOD( QuatCamera, "DisableRoll", LDisableRoll )

    LUA_NAME_METHOD( QuatCamera, "TargetMode", LTargetMode )
    LUA_NAME_METHOD( QuatCamera, "EnableTargetMode", LEnableTargetMode )
    LUA_NAME_METHOD( QuatCamera, "DisableTargetMode", LDisableTargetMode )

    LUA_NAME_METHOD( QuatCamera, "DisableGeosynchOrbit", LDisableGeosynchOrbit )
    LUA_NAME_METHOD( QuatCamera, "EnableGeosynchOrbit", LEnableGeosynchOrbit )
    LUA_NAME_METHOD( QuatCamera, "Rotate", LRotate )
    LUA_NAME_METHOD( QuatCamera, "Move", LMove )

    LUA_NAME_METHOD( QuatCamera, "SetRotation", LSetRotation )
    LUA_NAME_METHOD( QuatCamera, "GetRotationAngles", LGetRotationAngles )

    LUA_NAME_METHOD( QuatCamera, "SetRadius", LSetRadius )
    LUA_NAME_METHOD( QuatCamera, "GetRadius", LGetRadius )
    LUA_NAME_METHOD( QuatCamera, "MultiplyRadius", LMultiplyRadius )

    LUA_NAME_METHOD( QuatCamera, "SetRadiusTime", LSetRadiusTime )
    LUA_NAME_METHOD( QuatCamera, "SetRotationTime", LSetRotationTime )
    LUA_NAME_METHOD( QuatCamera, "SetTarget", LSetTarget)
    LUA_NAME_METHOD( QuatCamera, "SetTargetOffset", LSetTargetOffset )
    LUA_NAME_METHOD( QuatCamera, "SetTargetOffsetTime", LSetTargetOffsetTime )
    LUA_NAME_METHOD( QuatCamera, "GetTargetOffset", LGetTargetOffset )
    LUA_NAME_METHOD( QuatCamera, "SetTargetTime", LSetTargetTime)

    LUA_NAME_METHOD( QuatCamera, "GetUpVector", LGetUpVector )

    LUA_NAME_METHOD( QuatCamera, "GetWorldOrientation", LGetWorldOrientation )
    LUA_NAME_METHOD( QuatCamera, "GetWorldEye", LGetWorldEye )
    LUA_NAME_METHOD( QuatCamera, "GetWorldView", LGetWorldView )
    
    LUA_NAME_METHOD( QuatCamera, "Unfocus", LUnfocus )
    LUA_NAME_METHOD( QuatCamera, "Transform", LTransform )
  LUA_END_DEFINE_CLASS
  
  LUA_DEFINE_METHOD_NOLS( QuatCamera, LEnableRoll )
  {
    m_constrainRoll = false;

    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( QuatCamera, LDisableRoll )
  {
    m_constrainRoll = true;

    SetRotation(m_yaw,m_pitch,m_roll,500);

    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LTargetMode )
  {
    lua_pushboolean(ls, m_targetMode);

    return 1;
  }

  LUA_DEFINE_METHOD_NOLS( QuatCamera, LEnableTargetMode )
  {
    m_targetMode = true;

    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( QuatCamera, LDisableTargetMode )
  {
    m_targetMode = false;

    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( QuatCamera, LDisableGeosynchOrbit )
  {
    DisableGeosynchOrbit();
    
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( QuatCamera, LEnableGeosynchOrbit )
  {
    EnableGeosynchOrbit();
    
    return 0;
  }
  
  LUA_DEFINE_METHOD( QuatCamera, LRotate )
  {
    float yaw = luaL_checkfloat( ls, 1 );
    float pitch = luaL_checkfloat( ls, 2 );
    float roll = luaL_checkfloat( ls, 3 );

    if( lua_gettop(ls) > 3 )
    {
      float time = luaL_checkfloat( ls, 4 );

      Rotate( yaw, pitch, roll, time );
    }
    else
      Rotate( yaw, pitch, roll );
    
    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LSetRotation )
  {
    float yaw = luaL_checkfloat( ls, 1 );
    float pitch = luaL_checkfloat( ls, 2 );
    float roll = luaL_checkfloat( ls, 3 );

    if( lua_gettop(ls) > 3 )
    {
      float time = luaL_checkfloat( ls, 4 );

      SetRotation( yaw, pitch, roll, time );
    }
    else
      SetRotation( yaw, pitch, roll );
    
    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LGetRotationAngles )
  {
    lua_pushnumber(ls, m_yaw);
    lua_pushnumber(ls, m_pitch);
    lua_pushnumber(ls, m_roll);

    return 3;
  }
  
  LUA_DEFINE_METHOD( QuatCamera, LMove )
  {
    float dx = luaL_checkfloat( ls, 1 );
    float dy = luaL_checkfloat( ls, 2 );
    float dz = luaL_checkfloat( ls, 3 );
    //float time = luaL_checkfloat( ls, 4 );

    m_eye += dx * m_orientation.LocalX() + dy * m_orientation.LocalY() + dz * m_orientation.LocalZ();

    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LMultiplyRadius )
  {
    float radius = luaL_checkfloat( ls, 1 );

    if( lua_gettop(ls) > 1 )
    {
      float time = luaL_checkfloat( ls, 2 );
      MultiplyRadius( radius, time );
    }
    else
      MultiplyRadius( radius );
    
    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LSetRadius )
  {
    float radius = luaL_checkfloat( ls, 1 );

    if( lua_gettop(ls) > 1 )
    {
      float time = luaL_checkfloat( ls, 2 );
      SetRadius( radius, time );
    }
    else
      SetRadius( radius );
    
    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LGetRadius )
  {
    lua_pushnumber(ls, m_radius);
    return 1;
  }

  LUA_DEFINE_METHOD( QuatCamera, LSetRadiusTime )
  {
    float time = luaL_checkfloat(ls, 1);

    SetRadiusTime(time);

    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LSetRotationTime )
  {
    float time = luaL_checkfloat(ls, 1);

    SetRotationTime(time);

    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LSetTarget )
  {
    if( lua_gettop(ls) > 2 )
    {
      float x = luaL_checkfloat(ls, 1);
      float y = luaL_checkfloat(ls, 2);
      float z = luaL_checkfloat(ls, 3);

      if( lua_gettop(ls) > 3 )
      {
        float t = luaL_checkfloat(ls, 4);
        SetTarget( Math::Vector3(x,y,z), t );
      }
      else
        SetTarget( Math::Vector3(x, y, z) );
    }
    else
    {
      CTransform *transform = Lunar< CTransform >::check(ls, 1);
    
      if( lua_gettop(ls) > 1 )
      {
        float t = luaL_checkfloat(ls, 2);
        SetTarget( transform, t );
      }
      else
        SetTarget( transform );
    }

    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LGetTargetOffset )
  {
    lua_pushnumber(ls, m_targetOffset.x);
    lua_pushnumber(ls, m_targetOffset.y);
    lua_pushnumber(ls, m_targetOffset.z);

    return 3;
  }

  LUA_DEFINE_METHOD( QuatCamera, LSetTargetOffset )
  {
    float x = luaL_checkfloat(ls, 1);
    float y = luaL_checkfloat(ls, 2);
    float z = luaL_checkfloat(ls, 3);

    if( lua_gettop(ls) > 3 )
    {
      float t = luaL_checkfloat(ls, 4);
      SetTargetOffset( Math::Vector3(x,y,z), t );
    }
    else
      SetTargetOffset( Math::Vector3(x,y,z) );

    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LSetTargetOffsetTime )
  {
    float time = luaL_checkfloat(ls, 1);

    SetTargetOffsetTime(time);

    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LSetTargetTime )
  {
    float time = luaL_checkfloat(ls, 1);

    SetTargetTime(time);

    return 0;
  }

  LUA_DEFINE_METHOD( QuatCamera, LGetUpVector )
  {
    Vector3 up = m_orientation.LocalY().Normalized();
    
    lua_pushnumber( ls, up.x );
    lua_pushnumber( ls, up.y );
    lua_pushnumber( ls, up.z );

    return 3;
  }

  LUA_DEFINE_METHOD( QuatCamera, LGetWorldOrientation )
  {
    lua_pushnumber( ls, m_yaw );
    lua_pushnumber( ls, m_pitch );
    lua_pushnumber( ls, m_roll );

    return 3;
  }

  LUA_DEFINE_METHOD( QuatCamera, LGetWorldEye )
  {
    CTransform eyeTransform( m_eye, Math::Quaternion() );
    
    eyeTransform.SetParent(&Transform);

    Vector3 worldEye = eyeTransform.GetWorldPosition();

    lua_pushnumber( ls, m_eye.x );
    lua_pushnumber( ls, m_eye.y );
    lua_pushnumber( ls, m_eye.z );

    return 3;
  }

  LUA_DEFINE_METHOD( QuatCamera, LGetWorldView )
  {
    Vector3 view = m_orientation.LocalZ().Normalized();
    
    lua_pushnumber( ls, view.x );
    lua_pushnumber( ls, view.y );
    lua_pushnumber( ls, view.z );

    return 3;
  }

  LUA_DEFINE_METHOD_NOLS( QuatCamera, LUnfocus )
  {
    SetTarget( Transform.GetWorldPosition() );

    Transform.Orphan();

    return 0;
  }


  LUA_DEFINE_METHOD( QuatCamera, LTransform )
  {
    Lunar<CTransform>::push( ls, &Transform );

    return 1;
  }
  
} // Graphics namespace
} // SF namespace
