/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SmoothSort.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/05/2009 @ 01:33:07 - nwilliam
**  \brief
*******************************************************************************/

#ifndef SF_SMOOTH_SORT_H
#define SF_SMOOTH_SORT_H

#include <algorithm> // swap

namespace SF {
namespace Utilities
{
  // Forward declaration
  template <typename T, typename Pred>
  void smoothsort (T *, unsigned, Pred) throw ();

  namespace
  {
    class LeonardoNumber
    {
      public:
        LeonardoNumber() throw () : b (1), c (1) { }

        LeonardoNumber(const LeonardoNumber& _l) throw () : b (_l.b), c (_l.c) { }

        /**
        **  Return the "gap" between the actual Leonardo number and the
        **  preceding one.
        **/
        unsigned gap() const throw ()
        {
          return b - c;
        }

        /**  Perform an "up" operation on the actual number.  **/
        LeonardoNumber& operator++ () throw ()
        {
          unsigned s = b; b = b + c + 1; c = s;
          return *this;
        }

        /**  Perform a "down" operation on the actual number.  **/
        LeonardoNumber& operator-- () throw ()
        {
          unsigned s = c; c = b - c - 1; b = s;
          return *this;
        }

        /**  Return "companion" value.  **/
        unsigned operator~ () const throw ()
        {
          return c;
        }

        /**  Return "actual" value.  **/
        operator unsigned () const throw ()
        {
          return b;
        }

      private:
        unsigned b;   /**  Actual number.  **/
        unsigned c;   /**  Companion number.  **/
    };


    /**
    **  Sifts up the root of the stretch in question.
    **
    **    Usage: sift (<array>, <root>, <number>)
    **
    **    Where:     <array> Pointer to the first element of the array in
    **                       question.
    **                <root> Index of the root of the array in question.
    **              <number> Current Leonardo number.
    **/
    template <typename T, typename Pred>
    inline void sift(T * _m, unsigned _r, LeonardoNumber _b, Pred pred)throw ()
    {
      unsigned r2;

      while (_b >= 3)
      {
        if( pred(_m[_r - _b.gap ()], _m[_r - 1]) )
        {
          r2 = _r - _b.gap();
        }
        else
        {
          r2 = _r - 1; --_b;
        }

        if( pred(_m[_r], _m[r2]) )
        {
          break;
        }
        else
        {
          std::swap(_m[_r], _m[r2]); _r = r2; --_b;
        }
      }

      return;
    }

    /**
    **  Trinkles the roots of the stretches of a given array and root.
    **
    **    Usage: trinkle (<array>, <root>, <standard_concat>, <number>)
    **
    **    Where:           <array> Pointer to the first element of the array in
    **                             question.
    **                      <root> Index of the root of the array in question.
    **           <standard_concat> Standard concatenation's codification.
    **                    <number> Current Leonardo number.
    **/
    template <typename T, typename Pred>
    inline void trinkle(T * _m, unsigned _r, unsigned long long _p, LeonardoNumber _b, Pred pred) throw ()
    {
      while(_p)
      {
        for(; !(_p % 2); _p >>= 1)
          ++_b;

        if(!--_p || pred(_m [_r], _m [_r - _b]))
        {
          break;
        }
        else
          if(_b == 1)
          {
            std::swap(_m [_r], _m [_r - _b]); _r -= _b;
          }
          else if(_b >= 3)
          {
            unsigned r2 = _r - _b.gap();
            unsigned r3 = _r - _b;

            if( pred(_m[_r - 1], _m[r2]) )
            {
              r2 = _r - 1;
              _p <<= 1;
              --_b;
            }

            if( pred(_m[r3], _m[r2]) )
            {
              std::swap(_m [_r], _m [r3]);
              _r = r3;
            }
            else
            {
              std::swap(_m [_r], _m [r2]);
              _r = r2;
              --_b;
              break;
            }
          }
      }

      sift<T,Pred>(_m, _r, _b, pred);
    }



    /**
    **  Trinkles the roots of the stretches of a given array and root when the
    **  adjacent stretches are trusty.
    **
    **    Usage: semitrinkle (<array>, <root>, <standard_concat>, <number>)
    **
    **    Where:           <array> Pointer to the first element of the array in
    **                             question.
    **                      <root> Index of the root of the array in question.
    **           <standard_concat> Standard concatenation's codification.
    **                    <number> Current Leonardo number.
    **/
    template <typename T, typename Pred>
    inline void semitrinkle(T * _m, unsigned _r, unsigned long long _p, LeonardoNumber _b, Pred pred) throw ()
    {
      if( pred(_m[_r - ~_b], _m [_r]) )
      {
        std::swap(_m [_r], _m [_r - ~_b]);
        trinkle<T,Pred> (_m, _r - ~_b, _p, _b, pred);
      }
    }
  }


  /**
  **  smoothsort: Sorts the given array in ascending order.
  **
  **  Formal type T should have a comparison operator >= with prototype:
  **    bool T::operator >= (const T &) const throw ();
  **  which should compare its arguments by the given relation
  **     (possibly taking advantage of the type itself).
  **
  **    Usage: smoothsort (<array>, <size>)
  **
  **    Where: <array> pointer to the first element of the array in question.
  **            <size> length of the array to be sorted.
  **/
  template <typename T, typename Pred>
  void smoothsort (T * _m, unsigned _n, Pred pred) throw ()
  {
    if (!(_m && _n)) return;

    unsigned long long p = 1;
    LeonardoNumber b;

    for(unsigned q = 0; ++q < _n; ++p)
    {
      if(p % 8 == 3)
      {
        sift<T,Pred>(_m, q - 1, b, pred);

        ++++b;
        p >>= 2;
      }
      else if(p % 4 == 1)
      {
        if(q + ~b < _n)
          sift<T,Pred>(_m, q - 1, b, pred);
        else
          trinkle<T,Pred> (_m, q - 1, p, b, pred);

        for(p <<= 1; --b > 1; p <<= 1);
      }
    }

    trinkle<T,Pred>(_m, _n - 1, p, b, pred);

      for(--p; _n-- > 1; --p)
      {
        if(b == 1)
        {
          for( ; !(p % 2); p >>= 1)
            ++b;
        }
        else if(b >= 3)
        {
          if(p)
            semitrinkle<T,Pred>(_m, _n - b.gap(), p, b, pred);

          --b;
          p <<= 1;
          ++p;

          semitrinkle<T,Pred>(_m, _n - 1, p, b, pred);

          --b;
          p <<= 1;
          ++p;
        }
      }

    return;
  }

} // Utilities namespace
} // SF namespace

#endif // SF_SMOOTH_SORT_H
