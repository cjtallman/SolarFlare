/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    DeformVertex.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

namespace Vertex
{

  class DeformVertex
  {
  public:
    static bool Attribute( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::POSITION:
      case ATTRIBUTES::TEXCOORD0:
      case ATTRIBUTES::TEXCOORD1:
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
        return 3;
      case ATTRIBUTES::TEXCOORD0:
        return 2;
      case ATTRIBUTES::TEXCOORD1:
        return 1;
      default:
        return 0;
      }
    }

    static int ComponentType( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::POSITION:
      case ATTRIBUTES::TEXCOORD0:
      case ATTRIBUTES::TEXCOORD1:
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
      case ATTRIBUTES::TEXCOORD1:
        offset += sizeof(Vector2);
      case ATTRIBUTES::TEXCOORD0:
        offset += sizeof(Vector3);
      case ATTRIBUTES::POSITION:
      default:
        return offset;
      }
    }

    Vector3 position;
    Vector2 texcoord0;
    float scale;
  };

}
