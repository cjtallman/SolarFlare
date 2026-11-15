/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CTransform.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/26/2008 @ 13:37:04 - nwilliam
**  \brief   
*******************************************************************************/

#include <algorithm>
#include "CTransform.h"
#include "SolarFlare/Globals.h"     // entityFactory (serialization)
#include "Engine/EntityFactory.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Math
{

CTransform::CTransform()
: BaseComponent( Engine::ComponentType::TRANSFORM )
{
  m_owner = -1;
  m_parent = 0;
}

CTransform::CTransform(
  const SF::Math::Vector3 &point,
  const SF::Math::Quaternion &orientation)
 : BaseComponent( Engine::ComponentType::TRANSFORM )
{
  m_owner = -1;
  localPosition = point;
  localOrientation = orientation;
  m_parent = 0;
}

CTransform::~CTransform()
{
  if(m_parent)
    m_parent->RemoveChild(this);

  for(ChildList::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
    (*iter)->SetParent(m_parent);

  m_children.clear();
}

bool CTransform::InitDependencies( Engine::Entity* entity )
{
  m_owner = entity->ID();
  return true;
}

void CTransform::PushToLua( lua_State *ls )
{
  Lunar<CTransform>::push( ls, this );
}

Engine::BaseComponent *CTransform::Clone() const
{
  CTransform *clone = new CTransform;
  
  clone->localPosition = localPosition;
  clone->localOrientation = localOrientation;
  clone->name = name;
  
  return clone;
}

void CTransform::SetParent(CTransform *parent)
{
  if(m_parent == parent)
    return;

//  if(m_parent)
//    m_parent->RemoveChild(this);

  m_parent = parent;

  if(parent)
    parent->AddChild(this);
}

void CTransform::SetChild(CTransform *child)
{
  SetParent( child->m_parent );

  AddChild( child );

  if(m_parent)
    m_parent->RemoveChild( child );
}

void CTransform::Orphan()
{
  if(m_parent)
    m_parent->RemoveChild(this);

  m_parent = 0;
}

bool CTransform::HasParent() const
{
  return m_parent != 0;
}

void CTransform::AddChild(CTransform *child)
{
  m_children.push_back(child);
}

void CTransform::RemoveChild(CTransform *child)
{
  ChildList::iterator iter = std::find( m_children.begin(), m_children.end(), child );

  if( iter != m_children.end() )
    m_children.erase(iter);
}

CTransform *CTransform::GetParent()
{
  return m_parent;
}

const CTransform *CTransform::GetParent() const
{
  return m_parent;
}

Vector3  CTransform::GetWorldPosition() const
{
  assert( this != m_parent );

  if(m_parent)
  {
    Vector3 rotatedPosition( localPosition );

    rotatedPosition.Rotate( m_parent->GetWorldOrientation() );

    return rotatedPosition + m_parent->GetWorldPosition();
  }
  else
    return localPosition;
}

Quaternion  CTransform::GetWorldOrientation() const
{
  if(m_parent)
    return m_parent->GetWorldOrientation() * localOrientation;
  else
    return localOrientation;
}

void CTransform::SetLocalPositionFromWorld(const Vector3& worldPosition)
{
  Vector3 parentWorld = m_parent->GetWorldPosition();

  localPosition = (worldPosition - parentWorld);

  localPosition.RotateInverse( m_parent->GetWorldOrientation() );
}

Math::Vector3 CTransform::GetLocalPositionFromWorld(const Vector3& worldPosition)
{
  Vector3 local = (worldPosition - GetWorldPosition());

  local.RotateInverse( localOrientation );

  return local;
}

Engine::BaseComponent *CTransform::Creator(const ticpp::Element *)
{
  return new CTransform;
}


////////////////////////////////////////////////////////////////////////////////
// Networking
////////////////////////////////////////////////////////////////////////////////
void CTransform::InitIn( Networking::ByteStream& bs )
{
  bs >> m_owner;
  
  SetParent( 0 );
  
  uint8 hasParent;
  bs >> hasParent;
  
  if( hasParent )
  {
    std::string tName;
    Engine::EntityID pOwner;
    bs >> tName >> pOwner;
    
    Engine::Entity* e = Globals::entityFactory->GetEntity( pOwner );
    
    if( e )
      SetParent( (CTransform*)e->GetComponentByName( Engine::ComponentType::TRANSFORM, tName ) );
  }
  
  uint32 numChild;
  bs >> numChild;
  
  for( uint32 i = 0; i < numChild; ++i )
  {
    std::string cName;
    Engine::EntityID cOwner;
    
    bs >> cName >> cOwner;
    
    Engine::Entity* e = Globals::entityFactory->GetEntity( cOwner );
    
    if( e )
      SetParent( (CTransform*)e->GetComponentByName( Engine::ComponentType::TRANSFORM, cName ) );
  }
  
  bs >> localPosition >> localOrientation;
}

void CTransform::DeltaIn( Networking::ByteStream& bs )
{
  bs >> localPosition >> localOrientation;
}

void CTransform::InitOut( Networking::ByteStream& bs ) const
{
  bs << m_owner;
  
  if( m_parent )
    bs << (uint8)1 << m_parent->name << m_parent->m_owner;
  else
    bs << (uint8)0;
  
  uint32 numChild = (uint32)m_children.size();
  bs << numChild;
  
  for( uint32 i = 0; i < numChild; ++i )
    bs << m_children[i]->name << m_children[i]->m_owner;
  
  bs << localPosition << localOrientation;
}

void CTransform::DeltaOut( Networking::ByteStream& bs ) const
{
  bs << localPosition << localOrientation;
}



LUA_START_DEFINE_CLASS( CTransform )

  LUA_NAME_METHOD( CTransform, "SetParent", LSetParent )
  LUA_NAME_METHOD( CTransform, "SetChild", LSetChild )

  LUA_NAME_METHOD( CTransform, "Translate", LTranslate )
  LUA_NAME_METHOD( CTransform, "Rotate", LRotate )

  LUA_NAME_METHOD( CTransform, "SetPosition", LSetPosition )
  LUA_NAME_METHOD( CTransform, "SetRotation", LSetRotation )

  LUA_NAME_METHOD( CTransform, "SetLocalPositionFromWorld", LSetLocalPositionFromWorld )
  LUA_NAME_METHOD( CTransform, "SetLocalPositionLength", LSetLocalPositionLength )
  LUA_NAME_METHOD( CTransform, "SetOrientationFromPosition", LSetOrientationFromPosition )

  LUA_NAME_METHOD( CTransform, "GetLocalXAxis", LGetLocalXAxis )
  LUA_NAME_METHOD( CTransform, "GetLocalYAxis", LGetLocalYAxis )
  LUA_NAME_METHOD( CTransform, "GetLocalZAxis", LGetLocalZAxis )

  LUA_NAME_METHOD( CTransform, "GetLocalPositionNormalized", LGetLocalPositionNormalized )
  LUA_NAME_METHOD( CTransform, "GetRotatedLocalPositionNormalized", LGetRotatedLocalPositionNormalized )
  LUA_NAME_METHOD( CTransform, "GetLocalPosition", LGetLocalPosition )
  LUA_NAME_METHOD( CTransform, "GetWorldPosition", LGetWorldPosition )

  LUA_NAME_METHOD( CTransform, "WorldToLocal", LWorldToLocal )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CTransform, LSetParent )
{
  if( lua_isnil(ls, 1) )
  {
    Orphan();
  }
  else
  {
    CTransform *parent = Lunar<CTransform>::check( ls, 1 );
    
    SetParent( parent );
  }

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LSetChild )
{
  CTransform *parent = Lunar<CTransform>::check( ls, 1 );
  
  SetChild( parent );

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LTranslate )
{
  float dx = luaL_checkfloat( ls, 1 );
  float dy = luaL_checkfloat( ls, 2 );
  float dz = luaL_checkfloat( ls, 3 );

  localPosition += Vector3(dx, dy, dz);

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LRotate )
{
  float yaw = luaL_checkfloat( ls, 1 );
  float pitch = luaL_checkfloat( ls, 2 );
  float roll = luaL_checkfloat( ls, 3 );

  localOrientation.Rotate(yaw, pitch, roll);
  localOrientation.Normalize();

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LSetPosition )
{
  float x = luaL_checkfloat( ls, 1 );
  float y = luaL_checkfloat( ls, 2 );
  float z = luaL_checkfloat( ls, 3 );

  localPosition = Vector3(x, y, z);

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LSetRotation )
{
  float yaw = luaL_checkfloat( ls, 1 );
  float pitch = luaL_checkfloat( ls, 2 );
  float roll = luaL_checkfloat( ls, 3 );

  localOrientation = Quaternion::EulerAngles(yaw, pitch, roll);

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LSetLocalPositionFromWorld )
{
  float x = luaL_checkfloat( ls, 1 );
  float y = luaL_checkfloat( ls, 2 );
  float z = luaL_checkfloat( ls, 3 );

  SetLocalPositionFromWorld( Vector3(x,y,z) );

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LSetLocalPositionLength )
{
  float length = luaL_checkfloat( ls, 1 );

  localPosition = (localPosition.Normalized() * length);

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LSetOrientationFromPosition )
{
  Math::Vector3 alignmentAxis;

  alignmentAxis.x = luaL_checkfloat(ls, 1);
  alignmentAxis.y = luaL_checkfloat(ls, 2);
  alignmentAxis.z = luaL_checkfloat(ls, 3);

  int stackSize = lua_gettop(ls);

  Math::Vector3 rotationVector;
  
  if(stackSize < 6)
    rotationVector = localPosition;
  else
  {
    Math::Vector3 referencePoint( luaL_checkfloat(ls, 4), luaL_checkfloat(ls, 5), luaL_checkfloat(ls, 6) );

    rotationVector = localPosition - referencePoint;
  }

  localOrientation = Quaternion::AlignToVector( rotationVector, alignmentAxis );

  return 0;
}

LUA_DEFINE_METHOD( CTransform, LGetLocalXAxis )
{
  Quaternion Q = GetWorldOrientation();

  Vector3 xAxis = Q.LocalX();

  lua_pushnumber(ls, xAxis.x);
  lua_pushnumber(ls, xAxis.y);
  lua_pushnumber(ls, xAxis.z);

  return 3;
}

LUA_DEFINE_METHOD( CTransform, LGetLocalYAxis )
{
  Quaternion Q = GetWorldOrientation();

  Vector3 yAxis = Q.LocalY();

  lua_pushnumber(ls, yAxis.x);
  lua_pushnumber(ls, yAxis.y);
  lua_pushnumber(ls, yAxis.z);

  return 3;
}

LUA_DEFINE_METHOD( CTransform, LGetLocalZAxis )
{
  Quaternion Q = GetWorldOrientation();

  Vector3 zAxis = Q.LocalZ();

  lua_pushnumber(ls, zAxis.x);
  lua_pushnumber(ls, zAxis.y);
  lua_pushnumber(ls, zAxis.z);

  return 3;
}

LUA_DEFINE_METHOD( CTransform, LGetLocalPosition )
{
  lua_pushnumber( ls, localPosition.x );
  lua_pushnumber( ls, localPosition.y );
  lua_pushnumber( ls, localPosition.z );

  return 3;
}

LUA_DEFINE_METHOD( CTransform, LGetWorldPosition )
{
  Vector3 worldPosition = GetWorldPosition();

  lua_pushnumber( ls, worldPosition.x );
  lua_pushnumber( ls, worldPosition.y );
  lua_pushnumber( ls, worldPosition.z );

  return 3;
}

LUA_DEFINE_METHOD( CTransform, LGetLocalPositionNormalized )
{
  Vector3 pos = localPosition.Normalized();

  lua_pushnumber( ls, pos.x );
  lua_pushnumber( ls, pos.y );
  lua_pushnumber( ls, pos.z );

  return 3;
}

LUA_DEFINE_METHOD( CTransform, LGetRotatedLocalPositionNormalized )
{
  Vector3 pos;
  
  if(m_parent)
    pos = (GetWorldPosition() - m_parent->GetWorldPosition()).Normalized();
  else
    pos = GetWorldPosition().Normalized();

  lua_pushnumber( ls, pos.x );
  lua_pushnumber( ls, pos.y );
  lua_pushnumber( ls, pos.z );

  return 3;
}

LUA_DEFINE_METHOD( CTransform, LWorldToLocal )
{
  // Given a vector in world coordinates, returns the equivalent in local coordinates

  float x = luaL_checkfloat(ls, 1);
  float y = luaL_checkfloat(ls, 2);
  float z = luaL_checkfloat(ls, 3);

  Math::Vector3 worldPoint(x, y, z);

  Math::Vector3 localPoint = GetLocalPositionFromWorld(worldPoint);

  localPoint = localPoint.Normalized();

  lua_pushnumber(ls, localPoint.x);
  lua_pushnumber(ls, localPoint.y);
  lua_pushnumber(ls, localPoint.z);

  return 3;
}

}

}
