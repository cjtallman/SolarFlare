/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Perlin.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 01/05/2009 @ 17:36:25 - nwilliam
**  \brief   A version of "improved noise" from Perlin's 2002 SIGGRAPH paper.
*******************************************************************************/

#ifndef SF_PERLIN_H
#define SF_PERLIN_H

// References:
// Of course, all of Ken Perlin's stuff:
// - 'Improved Noise' presentation: http://www.noisemachine.com/talk1/
// - 'Improevd Noise' reference implementation: http://mrl.nyu.edu/~perlin/noise/
// - Information, links, original implementation: http://www.mrl.nyu.edu/~perlin/doc/oscar.html
//
// Simple, step by step guide to noise: http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
//
// Dozen or so academic quality PDFs on terrain modelling: http://oddlabs.com/jo/terrain/
//
// 'Coherent Noise' FAQ from libnoise: http://libnoise.sourceforge.net/coherentnoise/index.html
//
// Math behind Perlin noise, laymen approach: http://www.cs.cmu.edu/~mzucker/code/perlin-noise-math-faq.html
//
// Stefan Gustavson noise page: http://webstaff.itn.liu.se/~stegu/aqsis/DSOs/DSOnoises.html
  
#include "Noise.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  NoiseVal PerlinValue1D( const NoiseData& data, NoiseVal x );
  NoiseVal PerlinValue2D( const NoiseData& data, NoiseVal x, NoiseVal y );
  NoiseVal PerlinValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z );
  NoiseVal PerlinValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );
  
  NoiseVal PerlinNoise1D( NoiseVal x );
  NoiseVal PerlinNoise2D( NoiseVal x, NoiseVal y );
  NoiseVal PerlinNoise3D( NoiseVal x, NoiseVal y, NoiseVal z );
  NoiseVal PerlinNoise4D( NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );
  
  namespace PerlinPrivate
  {
    int FastFloor( NoiseVal x );
    NoiseVal SCurveQuintic( NoiseVal t );
    NoiseVal Lerp( NoiseVal t, NoiseVal a, NoiseVal b );
    
    NoiseVal Gradient( int hash, NoiseVal x );
    NoiseVal Gradient( int hash, NoiseVal x, NoiseVal y );
    NoiseVal Gradient( int hash, NoiseVal x, NoiseVal y, NoiseVal z );
    NoiseVal Gradient( int hash, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w );
    
    extern unsigned char perms[512];
  }
}
}
}

#endif // SF_PERLIN_H
