/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseIterator.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/13/2009 @ 16:06:59 - nwilliam
**  \brief   Base classes to ease implementation of STL friendly custom 
             iterators;
*******************************************************************************/

#ifndef SF_BASE_ITERATOR_H
#define SF_BASE_ITERATOR_H

#include <iterator>

// Forward Declarations
template< typename T> class BaseBidirectionalIterator;
template< typename T> class BaseConstBidirectionalIterator;

template< typename T >
class BaseBidirectionalIterator : public std::iterator< std::bidirectional_iterator_tag, 
                                                        typename T::value_type, 
                                                        typename T::distance_type>
{
  friend class BaseConstBidirectionalIterator<T>;
  
  protected:
    T m_iterator;
    
  public:
    // Required by STL
    typedef typename T::pointer           pointer;
    typedef typename T::reference         reference;
    typedef typename T::const_pointer     const_pointer;
    typedef typename T::const_reference   const_reference;
    
    // Constructors
    explicit BaseBidirectionalIterator() : m_iterator()
    {
    }
    
    BaseBidirectionalIterator( const BaseBidirectionalIterator<T>& rhs ) : m_iterator( rhs.m_iterator )
    {
    }
    
    BaseBidirectionalIterator( const T& rhs ) : m_iterator( rhs )
    {
    }
    
    // Methods
    operator T&() 
    {
      return m_iterator;
    }

    operator const T&() const 
    {
      return m_iterator;
    }

    BaseBidirectionalIterator<T>& operator=( const T& rhs )
    {
      m_iterator = rhs;
      return *this;
    }

    bool operator==( const T& rhs ) const
    {
      return m_iterator == rhs;
    }

    bool operator!=( const T& rhs ) const
    {
      return m_iterator != rhs;
    }

    typename T::reference operator*()
    {
      return m_iterator.Reference();
    }

    typename T::pointer operator->()
    {
      return &m_iterator.Reference();
    }

    BaseBidirectionalIterator<T>& operator++()
    {
      m_iterator.Next();
      return *this;
    }

    BaseBidirectionalIterator<T> operator++(int)
    {
      T tmp( m_iterator );
      m_iterator.Next();
      return tmp;
    }

    BaseBidirectionalIterator<T>& operator--()
    {
      m_iterator.Prev();
      return *this;
    }

    BaseBidirectionalIterator<T> operator--(int)
    {
      T tmp( m_iterator );
      m_iterator.Prev();
      return tmp;
    }
};


template< typename T >
class BaseConstBidirectionalIterator : public std::iterator< std::bidirectional_iterator_tag, 
                                                             typename T::value_type, 
                                                             typename T::distance_type >
{
  protected:
    T m_iterator;
  
  public:
    // Required by STL
    typedef typename T::pointer           pointer;
    typedef typename T::reference         reference;
    typedef typename T::const_pointer     const_pointer;
    typedef typename T::const_reference   const_reference;
    
    // Constructors
    explicit BaseConstBidirectionalIterator() : m_iterator()
    {
    }
    
    BaseConstBidirectionalIterator( const BaseConstBidirectionalIterator<T>& rhs ) : m_iterator( rhs.m_iterator )
    {
    }

    BaseConstBidirectionalIterator( const BaseBidirectionalIterator<T>& rhs ) : m_iterator( rhs.m_iterator )
    {
    }

    BaseConstBidirectionalIterator( const T& rhs ) : m_iterator( rhs )
    {
    }
    
    // Methods
    operator T&() 
    { 
      return m_iterator;
    }

    operator const T&() const 
    {
      return m_iterator;
    }

    BaseConstBidirectionalIterator<T>& operator=( const T& rhs )
    {
      m_iterator = rhs;
      return *this;
    }

    bool operator==( const T& rhs ) const
    {
      return m_iterator == rhs;
    }

    bool operator!=( const T& rhs ) const
    {
      return m_iterator != rhs;
    }

    typename T::const_reference operator*() const
    {
      return m_iterator.Reference();
    }

    typename T::const_pointer operator->() const
    {
      return &m_iterator.Reference();
    }

    BaseConstBidirectionalIterator<T>& operator++()
    {
      m_iterator.Next();
      return *this;
    }

    BaseConstBidirectionalIterator<T> operator++( int )
    {
      T tmp( m_iterator );
      m_iterator.Next();
      return tmp;
    }

    BaseConstBidirectionalIterator<T>& operator--()
    {
      m_iterator.Prev();
      return *this;
    }

    BaseConstBidirectionalIterator<T> operator--( int )
    {
      T tmp( m_iterator );
      m_iterator.Prev();
      return tmp;
    }
    
};

#endif  // SF_BASE_ITERATOR_H
