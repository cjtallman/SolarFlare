/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CEllipse.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:33:11 CTallman
**  \brief   Draws an oriented ellipse using a line loop.
*******************************************************************************/

#ifndef _SF_CELLIPSE_H_
#define _SF_CELLIPSE_H_

#include "Engine/BaseComponent.h"
#include "Math/TVector3.h"
#include "Math/Quaternion.h"
#include "External/Lua/include/LunarHelper.h"
#include <vector>


namespace SF { namespace Engine { class Entity; } }
namespace ticpp { class Element; }

namespace SF
{
  namespace Math { class CTransform; }
  namespace Graphics
  {
    namespace Components
    {

      class CEllipse : public Engine::BaseComponent
      {
      public:
        CEllipse();

        virtual BaseComponent * Clone() const;
        virtual bool InitDependencies( Engine::Entity * parent );
        static Engine::BaseComponent * CreateComponent( const ticpp::Element * element );

        void SetCenter( Math::Vector3 origin ) { center = origin; }
        void SetOrientation( Math::Quaternion quat ) { orientation = quat; }
        
        Math::Vector3 GetCenter() const { return center; }
        Math::Quaternion GetOrientation() const { return orientation; }

        void SetSegments( int segs ) { segments = segs; RecalculateEllipsePoints(); }
        void SetMajorAxis( float axis ) { major_axis = axis; RecalculateEllipsePoints(); }
        void SetMinorAxis( float axis ) { minor_axis = axis; RecalculateEllipsePoints(); }

        int GetSegments() const { return segments; }
        float GetMajorAxis() const { return major_axis; }
        float GetMinorAxis() const { return minor_axis; }

        void Draw() const;

        // Networking
        virtual void InitIn( Networking::ByteStream& bs );
        virtual void InitOut( Networking::ByteStream& bs ) const;

        virtual void PushToLua( lua_State *ls );

      private:

        void RecalculateEllipsePoints();

        bool visible;

        int segments;
        float major_axis;
        float minor_axis;

        Math::Vector3 center;
        Math::Quaternion orientation;
        float brightness;

        std::vector<Math::Vector3> ellipse_points;


        LUA_DECLARE_CLASS( CEllipse )
        LUA_DECLARE_METHOD( LGetMajorAxis )
        LUA_DECLARE_METHOD( LGetMinorAxis )
        LUA_DECLARE_METHOD( LSetMajorAxis )
        LUA_DECLARE_METHOD( LSetMinorAxis )

        LUA_DECLARE_METHOD( LSetCenter )
        LUA_DECLARE_METHOD( LSetOrientation )
        LUA_DECLARE_METHOD( LSetBrightness )


        LUA_DECLARE_METHOD( LVisible )

      };
    }
  }
}

#endif
