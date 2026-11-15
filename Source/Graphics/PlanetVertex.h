/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    PlanetVertex.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

namespace Vertex
{

  class PlanetVertex
  {
  public:
    static bool Attribute( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::POSITION:
      case ATTRIBUTES::NORMAL:
      case ATTRIBUTES::TANGENT:
      case ATTRIBUTES::BINORMAL:
      case ATTRIBUTES::TEXCOORD0:
        return true;
      default:
        return false;
      }
    }

    static int ComponentCount( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::POSITION:
      case ATTRIBUTES::NORMAL:
      case ATTRIBUTES::TANGENT:
      case ATTRIBUTES::BINORMAL:
        return 3;
      case ATTRIBUTES::TEXCOORD0:
        return 2;
      default:
        return 0;
      }
    }

    static int ComponentType( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::POSITION:
      case ATTRIBUTES::NORMAL:
      case ATTRIBUTES::TANGENT:
      case ATTRIBUTES::BINORMAL:
      case ATTRIBUTES::TEXCOORD0:
        return GL_FLOAT;
      default:
        return -1;
      }
    }

    static int Offset( ATTRIBUTES::Attributes attr )
    {
      int offset = 0;
      switch ( attr )
      {
      case ATTRIBUTES::TEXCOORD0:
        offset += sizeof(Vector3);
      case ATTRIBUTES::BINORMAL:
        offset += sizeof(Vector3);
      case ATTRIBUTES::TANGENT:
        offset += sizeof(Vector3);
      case ATTRIBUTES::NORMAL:
        offset += sizeof(Vector3);
      case ATTRIBUTES::POSITION:
      default:
        return offset;
      }
    }

    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
    Vector3 binormal;
    Vector2 texcoord0;
  };

}
