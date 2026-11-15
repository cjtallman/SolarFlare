/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    HashTable.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/03/2009 @ 21:55:49 - nwilliam
**  \brief   Open addressing Hash Table container.
*******************************************************************************/

#ifndef SF_HASH_TABLE_H
#define SF_HASH_TABLE_H

#include <string>
#include <vector>
#include "BaseIterator.h"

#if defined ( _MSC_VER )
  #define TYPENAMEMSC typename
  #define TYPENAMEGNU
#elif defined( __GNUC__ )
  #define TYPENAMEMSC
  #define TYPENAMEGNU typename
#else
  #define TYPENAMEMSC
  #define TYPENAMEGNU
#endif

namespace SF {
namespace Utilities
{
  template< typename T >
  class HashTable
  {
    // Forward Declarations
    struct Slot;

    public:
      typedef T                   value_type;
      typedef value_type*         pointer;
      typedef const value_type*   const_pointer;
      typedef value_type&         reference;
      typedef const value_type&   const_reference;
      typedef unsigned            size_type;
      typedef ptrdiff_t           difference_type;

      class HashTableIterator
      {
        private:
          friend class HashTable<T>;

          Slot* m_position;
          const HashTable<T>* m_container;

        public:
          // Required by STL
          typedef T           value_type;
          typedef T*          pointer;
          typedef const T*    const_pointer;
          typedef T&          reference;
          typedef const T&    const_reference;
          typedef ptrdiff_t   distance_type;

          // Constructors
          explicit HashTableIterator();
          HashTableIterator( const HashTableIterator* rhs );
          HashTableIterator( const HashTable<T>* container, Slot* pos = 0 );

          void Next();
          void Prev();
          reference Reference();
          const_reference Reference() const;
          bool operator==( const HashTableIterator& rhs ) const;
          bool operator!=( const HashTableIterator& rhs ) const;
          HashTableIterator& operator=( const HashTableIterator& rhs );
      };

      typedef TYPENAMEMSC BaseBidirectionalIterator< HashTableIterator >      iterator;
      typedef TYPENAMEMSC BaseConstBidirectionalIterator< HashTableIterator > const_iterator;

      HashTable( int startSize = 5, double MaxLoad = 0.5, double GrowthFactor = 2.0 );
      ~HashTable();

      void clear();
      void erase( iterator it );
      size_type erase( const std::string& key );
      iterator insert( const std::string& key, const T& data );

      bool empty()  const;
      size_t size() const;
      size_t capacity() const;
      void reserve( size_t n );

      iterator end();
      iterator begin();
      const_iterator end() const;
      const_iterator begin() const;

      iterator find( const std::string& key );
      const_iterator find( const std::string& key ) const;

    private:
      friend class HashTableIterator;

      struct Slot
      {
        enum State { OCCUPIED, UNOCCUPIED, DELETED };

        Slot();
        Slot( const_reference _data, State _state, const std::string& _key );

        State state;
        value_type data;
        std::string key;
      };

      void GrowTable();
      void EraseSlot( Slot* slot );
      size_type GetStride( const std::string& key ) const;
      int IndexOf( const std::string& key, Slot*& slot ) const;

      Slot* m_table;
      double m_maxLoad;
      double m_growthFactor;
      size_type m_capacity;
      mutable size_type m_size;
      mutable size_type m_probes;
      mutable size_type m_expansions;
  };

} // Utilities namespace
} // SF namespace




#include <cstring>
#include "HashHelpers.h"

namespace SF {
namespace Utilities {
  //////////////////////////////////////////////////////////////////////////////
  // Constructor (Slot)
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  HashTable<T>::Slot::Slot() : data(),
                               state( Slot::UNOCCUPIED ),
                               key()
  {
  }

  template< typename T >
  HashTable<T>::Slot::Slot( const_reference _data, State _state, const std::string& _key ) : data( _data ),
                                                                                             state( _state ),
                                                                                             key( _key )
  {
  }

  //////////////////////////////////////////////////////////////////////////////
  // Iterator
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  HashTable<T>::HashTableIterator::HashTableIterator() : m_position( 0 ),
                                                                    m_container( 0 )
  {
  }

  template< typename T >
  HashTable<T>::HashTableIterator::HashTableIterator( const HashTable<T>* container, Slot* pos ) : m_position( pos ),
                                                                                                   m_container( container )
  {
  }

  template< typename T >
  HashTable<T>::HashTableIterator::HashTableIterator( const HashTableIterator* rhs ) : m_position( rhs.m_position ),
                                                                                       m_container( rhs.m_container )
  {
  }

  template< typename T >
  typename HashTable<T>::HashTableIterator& HashTable<T>::HashTableIterator::operator=( TYPENAMEMSC const HashTable<T>::HashTableIterator& rhs )
  {
    m_position = rhs.m_position;
    m_container = rhs.m_container;
    return *this;
  }

  template< typename T >
  bool HashTable<T>::HashTableIterator::operator==( TYPENAMEMSC const HashTable<T>::HashTableIterator& rhs ) const
  {
    return (m_container == rhs.m_container) && (m_position == rhs.m_position);
  }

  template< typename T >
  bool HashTable<T>::HashTableIterator::operator!=( TYPENAMEMSC const HashTable<T>::HashTableIterator& rhs ) const
  {
    return (m_position != rhs.m_position) || (m_container != rhs.m_container );
  }

  template< typename T >
  typename HashTable<T>::HashTableIterator::reference HashTable<T>::HashTableIterator::Reference()
  {
    return m_position->data;
  }

  template< typename T >
  typename HashTable<T>::HashTableIterator::const_reference HashTable<T>::HashTableIterator::Reference() const
  {
    return m_position->data;
  }

  template< typename T >
  void HashTable<T>::HashTableIterator::Next()
  {
    Slot* end = m_container->m_table + m_container->m_capacity;

    do
    {
      ++m_position;
    }
    while( m_position->state != HashTable<T>::Slot::OCCUPIED && m_position != end );
  }

  template< typename T >
  void HashTable<T>::HashTableIterator::Prev()
  {
    do
    {
      --m_position;
    }
    while( m_position->state != HashTable<T>::Slot::OCCUPIED );
  }

  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  HashTable<T>::HashTable( int startSize, double maxLoad, double growthFactor ) : m_table( 0 ),
                                                                                  m_capacity( startSize ),
                                                                                  m_maxLoad( maxLoad ),
                                                                                  m_growthFactor( growthFactor )
  {
    m_table = new Slot[ m_capacity ];
  }

  template< typename T >
  HashTable<T>::~HashTable()
  {
    clear();
    delete[] m_table;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  void HashTable<T>::clear()
  {
    for( typename HashTable<T>::size_type i = 0; i < m_capacity; ++i )
      m_table[i].state = Slot::UNOCCUPIED;

    m_size = 0;
  }

  template< typename T >
  typename HashTable<T>::size_type HashTable<T>::erase( const std::string& key )
  {
    Slot* slot = 0;
    int index = IndexOf( key,  slot );

    if( index == -1 )
      return 0; // Key not in table

    EraseSlot( slot );
    return 1; // If this is ever many-to-one, this will change
  }

  template< typename T >
  void HashTable<T>::erase( typename HashTable<T>::iterator it )
  {
    HashTableIterator test( it );
    EraseSlot( test.m_position );
  }

  template< typename T >
  typename HashTable<T>::iterator HashTable<T>::insert( const std::string& key, const T& data )
  {
    Slot* slot = 0;

    double load = ( m_size + 1.0 ) / m_capacity;

    if( load > m_maxLoad )
      GrowTable();

    if( IndexOf( key,  slot ) != -1 )
      throw( "Key already in table." );

    m_size++;
    slot->data = data;
    slot->key = key;
    slot->state = Slot::OCCUPIED;

    return typename HashTable<T>::HashTableIterator( this, slot );
  }

  template< typename T >
  bool HashTable<T>::empty() const
  {
    return m_size == 0;
  }

  template< typename T >
  size_t HashTable<T>::size() const
  {
    return m_size;
  }

  template< typename T >
  size_t HashTable<T>::capacity() const
  {
    return m_capacity;
  }

  template< typename T >
  void HashTable<T>::reserve( size_t n )
  {
    // TODO: Implement HashTable::Reserve
  }

  template< typename T >
  typename HashTable<T>::iterator HashTable<T>::end()
  {
    return typename HashTable<T>::HashTableIterator( this, m_table + m_capacity );
  }

  template< typename T >
  typename HashTable<T>::iterator HashTable<T>::begin()
  {
    if( m_size != 0 )
    {
      Slot* s = m_table;
      Slot* end = m_table + m_capacity;

      while( s->state != HashTable<T>::Slot::OCCUPIED && s != end )
        ++s;

      return typename HashTable<T>::HashTableIterator( this, s );
    }

    return end();
  }

  template< typename T >
  typename HashTable<T>::const_iterator HashTable<T>::end() const
  {
    return HashTable<T>::const_iterator( this, m_table + m_capacity );
  }

  template< typename T >
  typename HashTable<T>::const_iterator HashTable<T>::begin() const
  {
    if( m_size != 0 )
    {
      Slot* s = m_table;
      Slot* end = m_table + m_capacity;

      while( s->state != HashTable<T>::Slot::OCCUPIED && s != end )
        ++s;

      return HashTable<T>::const_iterator( this, s );
    }

    return End();
  }

  template< typename T >
  typename HashTable<T>::iterator HashTable<T>::find( const std::string& key )
  {
    Slot* slot = 0;

    if( IndexOf( key, slot ) == -1 )
      return end();

    return typename HashTable<T>::HashTableIterator( this, slot );
  }

  template< typename T >
  typename HashTable<T>::const_iterator HashTable<T>::find( const std::string& key ) const
  {
    Slot* slot = 0;

    if( IndexOf( key, slot ) == -1 )
      return End();

    return HashTable<T>::HashTableIterator( this, slot );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private Methods
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  void HashTable<T>::GrowTable()
  {
    unsigned size = GetClosestPrime( static_cast<unsigned>( m_capacity * m_growthFactor ) );

    Slot* oldTable = m_table;
    typename HashTable<T>::size_type oldSize = m_capacity;

    // TOOD: We could get a bad alloc, and to be fully exception safe we would
    //       then have to free what we did get newed.
    m_table = new Slot[ size ];

    m_size = 0;
    m_capacity = size;

    // Reinsert all the items from the old table into the new
    for( typename HashTable<T>::size_type i = 0; i < oldSize; ++i )
    {
      if( oldTable[i].state == Slot::OCCUPIED )
      {
        insert( oldTable[i].key, oldTable[i].data );
      }
    }

    // Get rid of the old table
    delete[] oldTable;

    ++m_expansions;
  }

  template< typename T >
  void HashTable<T>::EraseSlot( typename HashTable<T>::Slot* slot )
  {
    if( slot == 0 || slot < m_table || slot >= (m_table + m_capacity) )
      return;

    --m_size;

    // Commented out portion was for PACK-ing method
    //if( m_policy == MARK )
    //{
      slot->state = Slot::DELETED;
    //}
    //else
    //{
    //  slot->state = Slot::UNOCCUPIED;
    //
    //  int start = index;
    //
    //  while( ( m_table[index].state == Slot::OCCUPIED ) && ( ++index != start ) )
    //  {
    //    if( index >= m_capacity )
    //      index = 0;
    //
    //    m_size--;
    //    m_table[index].state = Slot::UNOCCUPIED;
    //
    //    Insert( m_table[index].key,  m_table[index].data );
    //  }
    //}
  }

  template< typename T >
  typename HashTable<T>::size_type HashTable<T>::GetStride( const std::string& key ) const
  {
    // Linear probing by default
    typename HashTable<T>::size_type stride = PJWHash( key, m_capacity - 1 ) + 1;
    return stride;
  }

  template< typename T >
  int HashTable<T>::IndexOf( const std::string& key, Slot*& slot ) const
  {
    ++m_probes;

    int index = -1;
    int firstDel = -1;
    typename HashTable<T>::size_type count = 0;
    unsigned stride = GetStride( key );
    unsigned pos = JOAATHash( key, m_capacity );

    while( m_table[pos].state != Slot::UNOCCUPIED )
    {
      if( firstDel == -1 && m_table[pos].state == Slot::DELETED )
        firstDel = pos;

      if( m_table[pos].key == key )
      {
        index = pos;
        break;
      }

      if( ++count >= m_capacity )
        break;

      ++m_probes;
      pos += stride;

      if( pos >= m_capacity )
        pos = pos - m_capacity;
    }

    slot = &m_table[pos];

    // If PACK method is ever used again, this needs uncommented
    if( ( index == -1 ) /*&& ( m_policy == MARK )*/ && ( firstDel != -1 ) )
      slot = &m_table[firstDel];

    return index;
  }

} // Utilities namespace
} // SF namespace

#endif
