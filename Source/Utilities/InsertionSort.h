/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    InsertionSort.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: PHY 350
**  \par     Updated: 03/21/2009 @ 16:47:21 - pmera
**  \brief   
*******************************************************************************/

#include <functional>

template <typename ITER, typename SORTER>
void InsertionSort(const ITER& begin, const ITER& end, const SORTER& sorter)
{
  for(ITER iter = begin; iter != end; ++iter)
  {
    ITER::value_type val = *iter;
    ITER j = iter;

    while( j > begin && sorter( val, *(j - 1) ) )
    {
      *j = *(j - 1);
      --j;
    }

    *j = val;
  }
}

template <typename ITER>
void InsertionSort(const ITER& begin, const ITER& end)
{
  InsertionSort(begin, end, std::less<ITER::value_type>());
}
