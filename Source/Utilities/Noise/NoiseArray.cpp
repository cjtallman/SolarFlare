/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    NoiseArray.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/29/2009 @ 14:51:41 - nwilliam
**  \brief   Implementation of the NoiseArray class.
*******************************************************************************/

#include <cstring> //memset
#include "NoiseArray.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors
  //////////////////////////////////////////////////////////////////////////////
  NoiseArray::NoiseArray() : m_width(),
                             m_height(),
                             m_data( 0 )
  {
  }
  
  NoiseArray::NoiseArray( int width, int height ) : m_width( width ),
                                                    m_height( height ),
                                                    m_data( 0 )
  {
    Resize( width, height );
  }
  
  NoiseArray::~NoiseArray()
  {
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  NoiseVal* NoiseArray::Data()
  {
    return m_data.Get();
  }
  
  const NoiseVal* NoiseArray::Data() const
  {
    return m_data.Get();
  }
  
  int NoiseArray::Width() const
  {
    return m_width;
  }
  
  int NoiseArray::Height() const
  {
    return m_height;
  }
  
  void NoiseArray::Width( int width )
  {
    Resize( width, m_height );
  }
  
  void NoiseArray::Height( int height )
  {
    Resize( m_width, height );
  }
  
  NoiseVal* NoiseArray::operator[]( int index )
  {
    return &(m_data[index]);
  }
  
  NoiseVal NoiseArray::operator[]( int index ) const
  {
    return m_data[index];
  }
  
  NoiseVal NoiseArray::GetValue( int x, int y )
  {
    return m_data[ x + ( m_width * y ) ];
  }
  
  void NoiseArray::SetValue( int x, int y, NoiseVal val )
  {
    m_data[ x + ( m_width * y ) ] = val;
  }
  
  void NoiseArray::Clear()
  {
    Resize( 0, 0 );
  }
  
  void NoiseArray::Resize( int width, int height )
  {
    m_width = width;
    m_height = height;
    
    m_data = new NoiseVal[ width * height ];
    
    std::memset( m_data.Get(), 0, sizeof(NoiseVal) * width * height );
  }
  
  void NoiseArray::Scale( NoiseVal min, NoiseVal max)
  {
    const int count = m_width * m_height;
    
    float curMin = 5000.0f;
    float curMax = -5000.0f;
    
    for( int i = 0; i < count; ++i )
    {
      if( m_data[i] < curMin )
        curMin = m_data[i];
      else if( m_data[i] > curMax )
        curMax = m_data[i];
    }
    
    float rangeDiff = (max - min) / (curMax - curMin);
    
    if( curMin < 0.0f )
      curMin = -curMin;

    if( min < 0.0f )
      min = -min;

    for( int i = 0; i < count; ++i )
    {
      m_data[i] = (m_data[i] + curMin) * rangeDiff - min;
    }
  }
  
} // Noise namespace
} // Utilities namespace
} // SF namespace
