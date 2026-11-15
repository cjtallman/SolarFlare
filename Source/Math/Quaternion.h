/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Quaternion.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/12/2008 @ 20:06:52 - nwilliam
**  \brief   The quaternion class.
*******************************************************************************/

#ifndef SF_QUATERNION_H
#define SF_QUATERNION_H

#include <cmath>
#include "TVector3.h"
#include "Networking/ByteStream.h"

namespace SF {
namespace Math
{

template <typename T>
class TMatrix4x4;

typedef TMatrix4x4<float> Matrix4;

struct Quaternion
{
  Vector3 v;  // Vector part
  float   w;  // Scalar part

  Quaternion();
  Quaternion(float _x, float _y, float _z, float _w);
  Quaternion(Vector3 _v, float _w);

  float Magnitude() const;
  void Normalize();

  Quaternion Conjugate() const;
  Quaternion Inverse() const;
  
    // Dot product
  float Dot(const Quaternion& rhs) const;

    // Quaternion multiplication
  Quaternion operator*(const Quaternion& rhs) const;
  Quaternion& operator*=(const Quaternion& rhs);

    // Scalar multiplication
  Quaternion operator*(float rhs) const;
  Quaternion& operator*=(float rhs);
  friend Quaternion operator*(float lhs, const Quaternion& rhs);

    // Addition
  Quaternion operator+(const Quaternion& rhs) const;
  Quaternion& operator+=(const Quaternion& rhs);

    // Subtraction
  Quaternion operator-(const Quaternion& rhs) const;
  Quaternion& operator-=(const Quaternion& rhs);

    // Negation
  Quaternion operator-() const;

  Matrix4 GetRotationMatrix() const;

    // Local axes
  Vector3 LocalX() const;
  Vector3 LocalY() const;
  Vector3 LocalZ() const;

    // Retrieve axis-angle rotation
  void AxisAngle(Vector3& axis, float& angle) const;

  float GetYaw() const;
  float GetPitch() const;
  float GetRoll() const;

  void SetIdentity();

    // Set relative rotation
  void Rotate(float yaw, float pitch, float roll);
  void Rotate(const Vector3& axis, float theta);

  void RotateLocal(float yaw, float pitch, float roll);

  void YawLocal(float yaw);
  void PitchLocal(float pitch);
  void RollLocal(float roll);

    // Quaternion construction methods
  static Quaternion Identity();
  static Quaternion AlignToVector(const Vector3& vector, const Vector3& axis);
  static Quaternion AxisAngle(const Vector3& axis);
  static Quaternion AxisAngle(const Vector3& axis, float angle);
  static Quaternion EulerAngles(float yaw, float pitch, float roll);
};

  // Spherical linear interpolation
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

  // Normalized linear interpolation
Quaternion Nlerp(const Quaternion& q1, const Quaternion& q2, float t);

Networking::ByteStream& operator>>( Networking::ByteStream& bs, Quaternion& q );
Networking::ByteStream& operator<<( Networking::ByteStream& bs, const Quaternion& q );

} // Math namespace
} // SF namespace

#endif
