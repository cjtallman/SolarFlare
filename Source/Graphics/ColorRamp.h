/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ColorRamp.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/25/2009 @ 00:20 - CJT 
**  \brief   A 1D texture.
*******************************************************************************/

#ifndef SF_COLOR_RAMP_H
#define SF_COLOR_RAMP_H

#include <map>

#include "Utilities/Color.h"
#include "Utilities/Image.h"
#include "Utilities/Macros.h"
#include "Utilities/SharedPtr.h"
#include "Networking/ByteStream.h"

namespace SF
{
  namespace Graphics
  {
    namespace TextureType
    {
      class ColorRamp
      {
      public:
        ColorRamp(void);
        virtual ~ColorRamp(void);

        void AddColor( const float & position, const SF::Color & color );
        void ClearColors();
        void RandomizeColors();
        void LoadFromFile( const std::string & fullpath, const std::string & ext );
        void Bind(int stage = 0);
        void Unbind();
        unsigned int GetHandle() { return handle; }
        void MakeColorRamp();
        void Recreate(int stage = 0);

        ColorRamp & operator=(const ColorRamp & A);

      protected:

        friend Networking::ByteStream& operator>>( Networking::ByteStream&, ColorRamp& );
        friend Networking::ByteStream& operator<<( Networking::ByteStream&, const ColorRamp& );

        float max_value;
        float min_value;
        std::map< float, SF::Color > colors;

        Utilities::SharedPtr< Utilities::Image > image;

        GLuint handle;
        int stage;
      };

      Networking::ByteStream& operator>>( Networking::ByteStream&, ColorRamp& );
      Networking::ByteStream& operator<<( Networking::ByteStream&, const ColorRamp& );


    }
  }
}

#endif
