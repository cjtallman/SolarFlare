/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CTransform.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/26/2008 @ 13:36:58 - nwilliam
**  \brief   
*******************************************************************************/

#ifndef SF_C_TRANSFORM_H
#define SF_C_TRANSFORM_H

#include <vector>
#include "TVector3.h"
#include "Engine/Entity.h"
#include "Math/Quaternion.h"
#include "Script/ScriptHelper.h"

#include "Utilities/MemoryManager.h"

namespace ticpp { class Element; }

namespace SF {
namespace Math
{
  class CTransform : public Engine::BaseComponent, public MemoryManagerBase< CTransform >
  {
  public:
    typedef std::vector<CTransform *> ChildList;

    CTransform();
    CTransform( const Vector3& point, const Quaternion& orientation );
    ~CTransform();

    virtual bool InitDependencies( Engine::Entity *entity );
    virtual BaseComponent* Clone() const;
    virtual void PushToLua( lua_State *ls );

    void SetParent(CTransform *parent);
    void SetChild(CTransform *child);
    void Orphan();
    bool HasParent() const;

    CTransform *GetParent();
    const CTransform *GetParent() const;

    Vector3      GetWorldPosition() const;
    Quaternion  GetWorldOrientation() const;

    void SetLocalPositionFromWorld(const Vector3& worldPos);
    Math::Vector3 GetLocalPositionFromWorld(const Vector3& worldPos);

    static BaseComponent *Creator(const ticpp::Element *element);

    Vector3      localPosition;
    Quaternion  localOrientation;

    // Networking
    virtual void InitIn( Networking::ByteStream& bs );
    virtual void DeltaIn( Networking::ByteStream& bs );
    virtual void InitOut( Networking::ByteStream& bs ) const;
    virtual void DeltaOut( Networking::ByteStream& bs ) const;


  private:
    void AddChild(CTransform *child);
    void RemoveChild(CTransform *child);

    Engine::EntityID    m_owner;
    CTransform *        m_parent;
    ChildList           m_children;

    LUA_DECLARE_CLASS( CTransform )
    LUA_DECLARE_METHOD( LSetParent )
    LUA_DECLARE_METHOD( LSetChild )

    LUA_DECLARE_METHOD( LTranslate )
    LUA_DECLARE_METHOD( LRotate )

    LUA_DECLARE_METHOD( LSetPosition )
    LUA_DECLARE_METHOD( LSetRotation )

    LUA_DECLARE_METHOD( LSetLocalPositionFromWorld )
    LUA_DECLARE_METHOD( LSetLocalPositionLength )
    LUA_DECLARE_METHOD( LSetOrientationFromPosition )

    LUA_DECLARE_METHOD( LGetLocalXAxis )
    LUA_DECLARE_METHOD( LGetLocalYAxis )
    LUA_DECLARE_METHOD( LGetLocalZAxis )

    LUA_DECLARE_METHOD( LGetLocalPositionNormalized )
    LUA_DECLARE_METHOD( LGetRotatedLocalPositionNormalized )
    LUA_DECLARE_METHOD( LGetLocalPosition )
    LUA_DECLARE_METHOD( LGetWorldPosition )

    LUA_DECLARE_METHOD( LWorldToLocal )
  };

} // Math namespace
} // SF namespace

#endif // SF_C_TRANSFORM_H
