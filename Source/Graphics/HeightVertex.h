/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    HeightVertex.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/
namespace Vertex
{

  class HeightVertex
  {
  public:
    static bool Attribute( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::ATTR10:
        return true;
      default:
        return false;
      }
    }

    static int ComponentCount( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::ATTR10:
        return 1;
      default:
        return 0;
      }
    }

    static int ComponentType( ATTRIBUTES::Attributes attr )
    {
      switch( attr )
      {
      case ATTRIBUTES::ATTR10:
        return GL_FLOAT;
      default:
        return -1;
      }
    }

    static int Offset( ATTRIBUTES::Attributes attr )
    {
      switch ( attr )
      {
      case ATTRIBUTES::ATTR10:
        return 0;
      default:
        return -1;
      }
    }

    float displacement;
  };

}
