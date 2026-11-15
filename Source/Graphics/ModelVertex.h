/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    VertexBuffer3.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/11/2009 @ 22:21:59 -nwilliam
**  \brief
*******************************************************************************/

namespace Vertex
{

  class ModelVertex
  {
  public:
    ModelVertex() {}

    static bool Attribute( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::POSITION:
      case ATTRIBUTES::NORMAL:
      case ATTRIBUTES::COLOR:
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
      case ATTRIBUTES::TEXCOORD0:
        return 2;
      case ATTRIBUTES::POSITION:
      case ATTRIBUTES::NORMAL:
      case ATTRIBUTES::TANGENT:
      case ATTRIBUTES::BINORMAL:
        return 3;
      case ATTRIBUTES::COLOR:
        return 4;
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
      case ATTRIBUTES::COLOR:
        return GL_UNSIGNED_BYTE;
      default:
        return -1;
      }
    }

    static int Offset( ATTRIBUTES::Attributes attr )
    {
      int offs = 0;
      switch ( attr )
      {
      case ATTRIBUTES::TEXCOORD0:
        offs += sizeof(Vector3);
      case ATTRIBUTES::BINORMAL:
        offs += sizeof(Vector3);
      case ATTRIBUTES::TANGENT:
        offs += sizeof(Vector3);
      case ATTRIBUTES::NORMAL:
        offs += sizeof(Color);
      case ATTRIBUTES::COLOR:
        offs += sizeof(Vector3);
      case ATTRIBUTES::POSITION:
      default:
        return offs;
      }
    }

    Vector3 position;
    Color   color;
    Vector3 normal;
    Vector3 tangent;
    Vector3 binormal;
    Vector2 texcoord0;
  };

}
