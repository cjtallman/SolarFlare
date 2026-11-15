/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Color.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 00:55 - CJT 
**  \brief   Simple RGBA color class. Yes, it is similar to a vector.
*******************************************************************************/

#ifndef SF_COLOR_H
#define SF_COLOR_H

namespace SF
{
  template< typename T, int MAX = 255>
  class Color3
  {
  public:

    typedef T Type;

    Color3( T red = MAX, T green = MAX, T blue = MAX )
      : r(channel[0]), g(channel[1]), b(channel[2])
    {
      r = red;
      g = green;
      b = blue;
    }

    Color3( const Color3 & copy )
      : r(channel[0]), g(channel[1]), b(channel[2])
    {
      r = copy.r;
      g = copy.g;
      b = copy.b;
    }

    Color3( T val )
      : r(channel[0]), g(channel[1]), b(channel[2])
    {
      r = val;
      g = val;
      b = val;
    }

    Color3<T,MAX> & operator = ( const Color3<T,MAX> & copy )
    {
      r = copy.r;
      g = copy.g;
      b = copy.b;
      return *this;
    }

    template <typename U>
    Color3<T,MAX> operator + ( const U & A ) const
    { return Color3<T,MAX>(r + A, g + A, b + A); }

    template <typename U>
    Color3<T,MAX> operator - ( const U & A ) const
    { return Color3<T,MAX>(r - A, g - A, b - A); }

    template <typename U>
    Color3<T,MAX> operator * ( const U & A ) const
    { return Color3<T,MAX>(r * A, g * A, b * A); }

    template <typename U>
    Color3<T,MAX> operator / ( const U & A ) const
    { return Color3<T,MAX>(r / A, g / A, b / A); }

    template <typename U>
    Color3<T,MAX> & operator += ( const U & A )
    { r += A; g += A; b += A; return *this; }

    template <typename U>
    Color3<T,MAX> & operator -= ( const U & A )
    { r -= A; g -= A; b -= A; return *this; }

    template <typename U>
    Color3<T,MAX> & operator *= ( const U & A )
    { r *= A; g *= A; b *= A; return *this; }

    template <typename U>
    Color3<T,MAX> & operator /= ( const U & A )
    { r /= A; g /= A; b /= A; return *this; }

    Color3<T,MAX> operator + ( const Color3<T,MAX> & A ) const
    { return Color3<T,MAX>(r + A.r, g + A.g, b + A.b); }

    Color3<T,MAX> operator - ( const Color3<T,MAX> & A ) const
    { return Color3<T,MAX>(r - A.r, g - A.g, b - A.b); }

    bool operator == ( const Color3<T,MAX> & A ) const
    { return A.r == r && A.g == g && A.b == b; }

    bool operator != ( const Color3<T,MAX> & A ) const
    { return A.r != r || A.g != g || A.b != b; }

    static Color3<T,MAX> Interpolate( const Color3<T,MAX> & A, const Color3<T,MAX> & B, float t )
    { return A + t ( B - A ); }

    T & r;
    T & g;
    T & b;

    T * Array() { return channel; }

  private:
    T channel[3];
  };

  template <typename T, typename U>
  Color3<T> operator + ( const U & A, const Color3<T> & B )
  { return Color3<T>(A + B.r, A + B.g, A + B.b); }

  template <typename T, typename U>
  Color3<T> operator - ( const U & A, const Color3<T> & B )
  { return Color3<T>(A - B.r, A - B.g, A - B.b); }

  template <typename T, typename U>
  Color3<T> operator * ( const U & A, const Color3<T> & B )
  { return Color3<T>(A * B.r, A * B.g, A * B.b); }

  template <typename T, typename U>
  Color3<T> operator / ( const U & A, const Color3<T> & B )
  { return Color3<T>(A / B.r, A / B.g, A / B.b); }



  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////


  template<typename T, int MAX = 255>
  class Color4
  {
  public:

    typedef T Type;

    Color4( T red = MAX, T green = MAX, T blue = MAX, T alpha = MAX )
      : r(channel[0]), g(channel[1]), b(channel[2]), a(channel[3])
    {
      r = red;
      g = green;
      b = blue;
      a = alpha;
    }

    Color4( const Color4<T,MAX> & copy )
      : r(channel[0]), g(channel[1]), b(channel[2]), a(channel[3])
    {
      r = copy.r;
      g = copy.g;
      b = copy.b;
      a = copy.a;
    }

    Color4( T val )
      : r(channel[0]), g(channel[1]), b(channel[2]), a(channel[3])
    {
      r = val;
      g = val;
      b = val;
      a = val;
    }

    Color4<T,MAX> & operator = ( const Color4<T,MAX> & copy )
    {
      r = copy.r;
      g = copy.g;
      b = copy.b;
      a = copy.a;
      return *this;
    }

    Color4<T,MAX> & operator += ( const Color4<T,MAX> & A )
    {
      r += A.r;
      g += A.g;
      b += A.b;
      a += A.a;
      return *this;
    }

    template <typename U>
    Color4<T,MAX> operator + ( const U & A ) const
    { return Color4<T,MAX>(r + A, g + A, b + A, a + A); }

    template <typename U>
    Color4<T,MAX> operator - ( const U & A ) const
    { return Color4<T,MAX>(r - A, g - A, b - A, a - A); }

    template <typename U>
    Color4<T,MAX> operator * ( const U & A ) const
    { return Color4<T,MAX>(r * A, g * A, b * A, a * A); }

    template <typename U>
    Color4<T,MAX> operator / ( const U & A ) const
    { return Color4<T,MAX>(r / A, g / A, b / A, a / A); }

    template <typename U>
    Color4<T,MAX> & operator += ( const U & A )
    { r += A; g += A; b += A; a += A; return *this; }

    template <typename U>
    Color4<T,MAX> & operator -= ( const U & A )
    { r -= A; g -= A; b -= A; a -= A; return *this; }

    template <typename U>
    Color4<T,MAX> & operator *= ( const U & A )
    { r *= A; g *= A; b *= A; a *= A; return *this; }

    template <typename U>
    Color4<T,MAX> & operator /= ( const U & A )
    { r /= A; g /= A; b /= A; a /= A; return *this; }

    Color4<T,MAX> operator + ( const Color4<T,MAX> & A ) const
    { return Color4<T,MAX>(r + A.r, g + A.g, b + A.b, a + A.a); }

    Color4<T,MAX> operator - ( const Color4<T,MAX> & A ) const
    { return Color4<T,MAX>(r - A.r, g - A.g, b - A.b, a - A.a); }

    bool operator == ( const Color4<T,MAX> & A ) const
    { return A.r == r && A.g == g && A.b == b && A.a == a; }

    bool operator != ( const Color4<T,MAX> & A ) const
    { return A.r != r || A.g != g || A.b != b || A.a != a; }

    static Color4<T,MAX> Interpolate( const Color4<T,MAX> & A, const Color4<T,MAX> & B, float t );
    
    template <typename U, int MAX2>
    operator Color4<U,MAX2>() const
    {
      return Color4<U,MAX2>(
        static_cast<U>(MAX2 * r/(static_cast<float>(MAX))), 
        static_cast<U>(MAX2 * g/(static_cast<float>(MAX))), 
        static_cast<U>(MAX2 * b/(static_cast<float>(MAX))), 
        static_cast<U>(MAX2 * a/(static_cast<float>(MAX))));
    }

    T & r;
    T & g;
    T & b;
    T & a;

    T * Array() { return channel; }
    const T * Array() const { return channel; }

  private:
    T channel[4];
  };

  template <typename T, typename U, int MAX>
  Color4<U> operator + ( const T & A, const Color4<U,MAX> & B )
  { return Color4<U,MAX>(A + B.r, A + B.g, A + B.b, A + B.a); }

  template <typename T, typename U, int MAX>
  Color4<U,MAX> operator - ( const T & A, const Color4<U,MAX> & B )
  { return Color4<U,MAX>(A - B.r, A - B.g, A - B.b, A - B.a); }

  template <typename T, typename U, int MAX>
  Color4<U,MAX> operator * ( const T & A, const Color4<U,MAX> & B )
  { return Color4<U,MAX>(static_cast<U>(A * B.r), static_cast<U>(A * B.g), static_cast<U>(A * B.b), static_cast<U>(A * B.a)); }

  template <typename T, typename U, int MAX>
  Color4<U,MAX> operator / ( const T & A, const Color4<U,MAX> & B )
  { return Color4<U,MAX>(static_cast<U>(A / B.r), static_cast<U>(A / B.g), static_cast<U>(A / B.b), static_cast<U>(A / B.a)); }

  template <typename T, int MAX>
  Color4<T,MAX> Color4<T,MAX>::Interpolate( const Color4<T,MAX> & A, const Color4<T,MAX> & B, float t )
  { return (1 - t) * A + (t) * B; }

  typedef Color4<unsigned char, 255> Color;
  typedef Color4<unsigned char, 255> Color4ub;
  typedef Color4<float, 1> Color4f;
  typedef Color4<int, 255> Color4i;

} // SF namespace

// Template class, include source file

#endif // SF_T_COLOR_4_H
