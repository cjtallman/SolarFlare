/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ParticleVertex.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

namespace Vertex
{

  class ParticleVertex
  {
  public:
    static bool Attribute( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::POSITION:
      case ATTRIBUTES::COLOR:
      case ATTRIBUTES::PSIZE:
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
      case ATTRIBUTES::COLOR:
        return 4;
      case ATTRIBUTES::PSIZE:
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
      case ATTRIBUTES::PSIZE:
        return GL_FLOAT;
      case ATTRIBUTES::COLOR:
        return GL_UNSIGNED_BYTE;
      default:
        return -1;
      }
    }

    static int Offset( ATTRIBUTES::Attributes attr )
    {
      int offset = 0;
      switch ( attr )
      {
      case ATTRIBUTES::PSIZE:
        offset += sizeof(Color);
      case ATTRIBUTES::COLOR:
        offset += sizeof(Vector3);
      case ATTRIBUTES::POSITION:
      default:
        return offset;
      }
    }

    //--------------------------
    Vector3   position;
    Color    color;
    float    size;
  };

}
