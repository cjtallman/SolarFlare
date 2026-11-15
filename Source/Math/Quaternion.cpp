/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Quaternion.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/12/2008 @ 20:06:46 - nwilliam
**  \brief   Implementation of the quaternion class.
*******************************************************************************/

#include <cfloat>
#include <cassert>

#include "Quaternion.h"

#include "TMatrix4x4.h"
#include "Utilities/Misc.h"

namespace SF {
namespace Math
{

Quaternion::Quaternion()
: v(0, 0, 0), w(1)
{
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
: v(_x, _y, _z), w(_w)
{
}

Quaternion::Quaternion(Vector3 _v, float _w)
: v(_v), w(_w)
{
}


float Quaternion::Magnitude() const
{
  return std::sqrt(w*w + v*v);
}

void Quaternion::Normalize()
{
  float magnitude = Magnitude();

  w /= magnitude;
  v /= magnitude;
}

Quaternion Quaternion::Conjugate() const
{
  return Quaternion(-v, w);
}

Quaternion Quaternion::Inverse() const
{
  Quaternion conjugate = this->Conjugate();

  return conjugate * (1 / this->Dot(conjugate));
}

float Quaternion::Dot(const Quaternion& rhs) const
{
  return w * rhs.w + v * rhs.v;
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
  return Quaternion(v.Cross(rhs.v) + rhs.v * w + v * rhs.w,
                    w * rhs.w - v * rhs.v);
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
  v = v.Cross(rhs.v) + rhs.v * w + v * rhs.w;
  w = w * rhs.w - v * rhs.v;

  return *this;
}

Quaternion Quaternion::operator*(float rhs) const
{
  return Quaternion(v * rhs, w * rhs);
}

Quaternion& Quaternion::operator*=(float rhs)
{
  v *= rhs;
  w *= rhs;

  return *this;
}

Quaternion operator*(float lhs, const Quaternion& rhs)
{
  return rhs * lhs;
}

Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
  return Quaternion(v + rhs.v, w + rhs.w);
}

Quaternion& Quaternion::operator+=(const Quaternion& rhs)
{
  v += rhs.v;
  w += rhs.w;

  return *this;
}

Quaternion Quaternion::operator-(const Quaternion& rhs) const
{
  return Quaternion(v - rhs.v, w - rhs.w);
}

Quaternion& Quaternion::operator-=(const Quaternion& rhs)
{
  v -= rhs.v;
  w -= rhs.w;

  return *this;
}

Quaternion Quaternion::operator-() const
{
  return Quaternion(-v, -w);
}

Matrix4 Quaternion::GetRotationMatrix() const
{
  return Matrix4(
    1 - 2 * (v.y * v.y - v.z * v.z),  2 * (v.x * v.y - w * v.z),        2 * (v.x * v.z + w * v.y),        0,
    2 * (v.x * v.y + w * v.z),        1 - 2 * (v.x * v.x - v.z * v.z),  2 * (v.y * v.z - w * v.x),        0,
    2 * (v.x * v.z - w * v.y),        2 * (v.y * v.z + w * v.x),        1 - 2 * (v.x * v.x - v.y * v.y),  0,
    0,                                0,                              0,                                  1
  );
}

Vector3 Quaternion::LocalX() const
{
  return Vector3(
    1 - 2 * v.y * v.y - 2 * v.z * v.z,
    2 * v.x * v.y + 2 * w * v.z,
    2 * v.x * v.z - 2 * w * v.y );
}

Vector3 Quaternion::LocalY() const
{
  return Vector3(
    2 * v.x * v.y - 2 * w * v.z,
    1 - 2 * v.x * v.x - 2 * v.z * v.z,
    2 * v.y * v.z + 2 * w * v.x );
}

Vector3 Quaternion::LocalZ() const
{
  return Vector3(
    2 * v.x * v.z + 2 * w * v.y,
    2 * v.y * v.z - 2 * w * v.x,
    1 - 2 * v.x * v.x - 2 * v.y * v.y );
}

void Quaternion::AxisAngle(Vector3& axis, float& angle) const
{
  float epsilon = 0.001f;
  float magnitude = v.Magnitude();

  if( magnitude > epsilon )
  {
    axis.x = v.x / magnitude;
    axis.y = v.y / magnitude;
    axis.z = v.z / magnitude;
  }
  else
  {
    axis.x = 1;
    axis.y = 0;
    axis.z = 0;
  }

  angle = 2*acos(w);
}

float Quaternion::GetYaw() const
{
  return atan2( 2 * (w*v.x + v.y*v.z), (1 - 2 * (v.x*v.x + v.y*v.y) ) );
}

float Quaternion::GetPitch() const
{
  return asin( 2 * (w*v.y - v.z*v.x) );
}

float Quaternion::GetRoll() const
{
  return atan2( 2 * (w*v.z + v.x*v.y), (1 - 2 * (v.y*v.y + v.z*v.z) ) );
}

void Quaternion::SetIdentity()
{
  v.x = 0;
  v.y = 0;
  v.z = 0;
  w = 1;
}

  // Set relative rotation
void Quaternion::Rotate(float yaw, float pitch, float roll)
{
  Quaternion q = Quaternion::EulerAngles(yaw, pitch, roll);

    // Concatenate these new rotations with the current rotation goal
  *this = q * (*this);
}

  // Set relative rotation
void Quaternion::RotateLocal(float yaw, float pitch, float roll)
{
  Quaternion q = Quaternion::EulerAngles(yaw, pitch, roll);

  *this *= q;

  Normalize();
}

void Quaternion::Rotate(const Vector3& axis, float theta)
{
  float half_theta = theta * .5f;

  Quaternion q1(axis.Normalized() * sin(half_theta), cos(half_theta));

  *this = q1 * (*this);
}


void Quaternion::YawLocal(float yaw)
{
  float halfYaw = yaw*.5f;

  Quaternion QYaw(LocalX() * sin(halfYaw), cos(halfYaw));

  *this = QYaw * (*this);
}

void Quaternion::PitchLocal(float pitch)
{
  float halfPitch = pitch*.5f;

  Quaternion QPitch(LocalY() * sin(halfPitch), cos(halfPitch));

  *this = QPitch * (*this);
}

void Quaternion::RollLocal(float roll)
{
  float halfRoll = roll*.5f;

  Quaternion QRoll(LocalZ() * sin(halfRoll), cos(halfRoll));

  *this = QRoll * (*this);
}

Quaternion Quaternion::Identity()
{
  return Quaternion(0,0,0,1);
}

Quaternion Quaternion::AlignToVector(const Vector3& vector, const Vector3& axis)
{
  // Can't align to zero vector
  if(vector * vector < 1e-5f)
    return Identity();

  Vector3 rotationAxis = axis.Cross(vector);

  // Axis and Vector are colinear
  if(rotationAxis * rotationAxis < 1e-5f)
  {
    // Already aligned
    if( vector * axis > 0 )
    {
      return Identity();
    }
    // Need to align along the negative of the axis
    else
    {
      Vector3 arbitraryAxis = Vector3(1,0,0).Cross( vector );

      if( arbitraryAxis * arbitraryAxis > 1e-5f )
      {
        return Quaternion::AxisAngle( arbitraryAxis, 3.14f );
      }
      else
      {
        arbitraryAxis = Vector3(0,1,0).Cross( vector );
        return Quaternion::AxisAngle( arbitraryAxis, 3.14f );
      }
    }
  }

  float cosAngle = (vector * axis) / (vector.Magnitude() * axis.Magnitude());

  if(1.0f - std::fabs(cosAngle) < 1e-5f)
    return Identity();

  float angle = acos(cosAngle);

  return Quaternion::AxisAngle((const Vector3&)rotationAxis, angle);
}

Quaternion Quaternion::AxisAngle(const Vector3& axis)
{
  float magnitudeSquared = axis * axis;

  if(magnitudeSquared < 1e-5f)
    return Identity();

  float magnitude = Math::fastSqrt_Q3(magnitudeSquared);
  float halfAngle = magnitude * 0.5f;
  float invMagnitude = 1 / magnitude;

  return Quaternion(axis * invMagnitude * std::sin(halfAngle), std::cos(halfAngle));
}

Quaternion Quaternion::AxisAngle(const Vector3& axis, float angle)
{
  float halfAngle = angle * 0.5f;
  float magnitudeSquared = axis*axis;
  
  if( std::abs( 1.0f - magnitudeSquared ) < 1e-5f )
  {
    return Quaternion(axis * std::sin(halfAngle), std::cos(halfAngle));
  }
  else
  {
    float magnitude = Math::fastSqrt_Q3(magnitudeSquared);

    return Quaternion(axis * (std::sin(halfAngle) / magnitude), std::cos(halfAngle));
  }
}

Quaternion Quaternion::EulerAngles(float yaw, float pitch, float roll)
{
  float halfYaw = yaw*.5f;
  float halfPitch = pitch*.5f;
  float halfRoll = roll*.5f;

  float cosYaw = cos(halfYaw);
  float cosPitch = cos(halfPitch);
  float cosRoll = cos(halfRoll);

  float sinYaw = sin(halfYaw);
  float sinPitch = sin(halfPitch);
  float sinRoll = sin(halfRoll);

  Quaternion q;

  q.w =   cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;
  q.v.x = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
  q.v.y = cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll;
  q.v.z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;

  return q;
}

/*
  theta = angle between q1 and q2.
  phi = angle between q1 and interpolated result
*/
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
  Quaternion q2;
  
  float cosTheta = q0.Dot(q1);

  if( cosTheta < 0 )
  {
    cosTheta = -cosTheta;
    q2 = -q1;
  }
  else
    q2 = q1;

  if( std::fabs(cosTheta) < 0.95f)
  {
    float theta = std::acos(cosTheta);

    float invSinTheta = 1.0f / std::sin(theta);

    return q0 * (std::sin((1 - t) * theta) * invSinTheta) +
           q2 * (std::sin(t * theta) * invSinTheta);
  }
  else
    return Nlerp(q0, q2, t);
}

Quaternion Nlerp(const Quaternion& q1, const Quaternion& q2, float t)
{
  Quaternion result = q1 * (1.0f - t) + q2 * t;

  result.Normalize();

  return result;
}


Networking::ByteStream& operator>>( Networking::ByteStream& bs, Quaternion& q )
{
  bs >> q.v >> q.w;
  return bs;
}

Networking::ByteStream& operator<<( Networking::ByteStream& bs, const Quaternion& q )
{
  bs << q.v << q.w;
  return bs;
}


} // Math namespace
} // SF namespace
