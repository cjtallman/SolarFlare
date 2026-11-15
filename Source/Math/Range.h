/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Range.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 01/05/2009 @ 16:34:02 - nwilliam
**  \brief   
*******************************************************************************/

#ifndef SF_RANGE_H
#define SF_RANGE_H

namespace SF {
namespace Math
{
  template <typename T>
  struct Range
  {
    Range();
    Range(T lower, T upper);
    
    bool Intersection(const Range<T>& rhs);
    
    T lowerBound;
    T upperBound;
    
    bool empty;
    bool infinite;
  };
  
} // Math namespace
} // SF namespace




namespace SF {
namespace Math
{
  
  template <typename T>
  Range<T>::Range()
  {
    lowerBound = 0;
    upperBound = 0;
    
    empty = false;
    infinite = false;
  }
  
  template <typename T>
  Range<T>::Range(T lower, T upper)
  {
    lowerBound = lower;
    upperBound = upper;
    
    empty = false;
    infinite = false;
  }
  
  template <typename T>
  bool Range<T>::Intersection(const Range<T>& rhs)
  {
    if(empty || rhs.empty)
      return false;
    
    if(infinite || rhs.infinite)
      return true;
    
    if(rhs.lowerBound > upperBound)
      return false;
    else if(lowerBound > rhs.upperBound)
      return false;
    
    return true;
  }
  
} // Math namespace
} // SF namespace

#endif // SF_RANGE_H
