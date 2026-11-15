/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    MemoryManager.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/24/2009 @ 18:05:53 - nwilliam
**  \brief   Simple type-specific memory allocator from back in the day.
             Originally written pre-CS280, but I believe I went back and fixed
             it up afterwards.

             MemoryManagerBase is used CRTP-style to provide new/delete
             operators to classes. Observe:

               class Foo : public Base, public MemoryManagerBase<Foo, 1000>

               ...

               Base *foo = new Foo; // Memory allocated using memory manager

            However, MemoryManagerBase will fail to work if it is included
            multiple time in a hierarchy. We solve this using a macro to define
            the ambiguous new/delete methods in the class itself.

              class Base : public MoreBase, public MemoryManager<Base, 1000>
              ...
              class Foo : public Base, public MemoryManager<Foo, 1000>
              {
                MEMORY_MANAGER_SET(FOO, 1000)
                ...
              };

            TODO: Add more debug functionality.

*******************************************************************************/

#ifndef SF_MEMORY_MANAGER_H
#define SF_MEMORY_MANAGER_H

#include <cstring> //size_t
#include "Threading/CriticalSection.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

namespace SF
{
  template <typename T, unsigned int PAGE_SIZE>
  class MemoryManager
  {
    private:
      union Block
      {
        char     data[sizeof(T)];
        Block *  next;
      };

      struct Page
      {
        Page * next;
        Block  data[PAGE_SIZE];
      };

    public:
      MemoryManager();
      ~MemoryManager();

      void  AllocatePage();

      T *   AllocateBlock();
      void  FreeBlock(void *ptr);

      unsigned int  GetBlocksUsed() { return m_blocks_used; }
      unsigned int  GetAllocations() { return m_allocations; }
      unsigned int  GetFrees() { return m_frees; }
      unsigned int  GetPages() { return m_pages; }

    private:
      unsigned int  m_blocks_used;  // Number of allocated blocks
      unsigned int  m_allocations;  // Number of allocations
      unsigned int  m_frees;        // Number of frees
      unsigned int  m_pages;        // Number of pages

      Page *        m_page; // The page list
      Block *       m_free; // The free list

      Threading::CriticalSection m_cs;
  };

  template <typename T, unsigned int PAGE_SIZE = 64>
  class MemoryManagerBase
  {
    public:
      void *operator new(size_t /*bytes*/)
      {
        return m_memoryManager.AllocateBlock();
      }

      void operator delete(void *pointer, size_t /*bytes*/)
      {
        m_memoryManager.FreeBlock(pointer);
      }

      #if defined(_DEBUG) && defined(_MSC_VER)
      void* operator new( size_t /*bytes*/, int /*blockType*/, const char* /*file*/, long /*line*/ )
      {
        return m_memoryManager.AllocateBlock();
      }

      void operator delete(void *pointer, int /*blockType*/, const char* /*file*/, long /*line*/ )
      {
        m_memoryManager.FreeBlock(pointer);
      }
      #endif

      static unsigned int GetBlocksUsed() { return m_memoryManager.GetBlocksUsed(); }
      static unsigned int GetAllocations() { return m_memoryManager.GetAllocations(); }
      static unsigned int GetFrees() { return m_memoryManager.GetFrees(); }
      static unsigned int GetPages() { return m_memoryManager.GetPages(); }

    private:
      static MemoryManager<T, PAGE_SIZE> m_memoryManager;
  };

  template <typename T, unsigned int PAGE_SIZE>
  MemoryManager<T, PAGE_SIZE> MemoryManagerBase<T, PAGE_SIZE>::m_memoryManager;

  /* Ugly multiple-inheritance fix. If both a child and parent class inherit from
   MemoryManagerBase, there will be an ambiguity with all inherited methods. This can
   be addressed by overriding these methods in the child class. */
  #if defined(_DEBUG) && defined(_MSC_VER)
    #define MEMORY_MANAGER_SET(CLASS_TYPE, PAGE_SIZE) \
      void *operator new(size_t bytes,int i,const char* c,long l) { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::operator new(bytes,i,c,l); } \
      void operator delete(void *pointer,int i,const char* c,long l) { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::operator delete(pointer,i,c,l); } \
      void *operator new(size_t bytes) { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::operator new(bytes); } \
      void operator delete(void *pointer, size_t bytes) { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::operator delete(pointer, bytes); } \
      static unsigned int GetBlocksUsed() { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::GetBlocksUsed(); } \
      static unsigned int GetAllocations() { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::GetAllocations(); } \
      static unsigned int GetFrees() { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::GetFrees(); } \
      static unsigned int GetPages() { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::GetPages(); }
  #else
    #define MEMORY_MANAGER_SET(CLASS_TYPE, PAGE_SIZE) \
      void *operator new(size_t bytes) { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::operator new(bytes); } \
      void operator delete(void *pointer, size_t bytes) { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::operator delete(pointer, bytes); } \
      static unsigned int GetBlocksUsed() { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::GetBlocksUsed(); } \
      static unsigned int GetAllocations() { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::GetAllocations(); } \
      static unsigned int GetFrees() { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::GetFrees(); } \
      static unsigned int GetPages() { return MemoryManagerBase<CLASS_TYPE, PAGE_SIZE>::GetPages(); }
  #endif

  template <typename T, unsigned int PAGE_SIZE>
  MemoryManager<T, PAGE_SIZE>::MemoryManager()
  {
    m_blocks_used = 0;
    m_allocations = 0;
    m_frees = 0;
    m_pages = 0;

    m_page = 0;

    AllocatePage();
  }

  template <typename T, unsigned int PAGE_SIZE>
  MemoryManager<T, PAGE_SIZE>::~MemoryManager()
  {
    // Delete all pages allocated by the memory pool
    while(m_page)
    {
      Page *del_page = m_page;
      m_page = m_page->next;
      delete del_page;
    }
  }

  template <typename T, unsigned int PAGE_SIZE>
  void MemoryManager<T, PAGE_SIZE>::AllocatePage()
  {
    #if defined(_DEBUG) && defined(_MSC_VER)
      Page *new_page = new( _CLIENT_BLOCK, __FILE__, __LINE__) Page;
    #else
      Page *new_page = new Page;
    #endif

    new_page->next = 0;

    if(m_page)
    {
      m_page->next = new_page;
    }

    m_page = new_page;

    m_free = &m_page->data[0];

    for(int i = 0; i < PAGE_SIZE; ++i)
    {
      if(i+1 < PAGE_SIZE)
      {
        m_page->data[i].next = &m_page->data[i+1];
      }
      else
      {
        m_page->data[i].next = 0;
      }
    }

    ++m_pages;
  }

  template <typename T, unsigned int PAGE_SIZE>
  T *MemoryManager<T, PAGE_SIZE>::AllocateBlock()
  {
    m_cs.Enter();

    if(!m_free)
    {
      AllocatePage();
    }

    T *result = reinterpret_cast<T *>(m_free->data);

    m_free = m_free->next;

    ++m_allocations;
    ++m_blocks_used;

    //std::memset(result, 0xbb, sizeof(T));

    m_cs.Leave();

    return result;
  }

  template <typename T, unsigned int PAGE_SIZE>
  void MemoryManager<T, PAGE_SIZE>::FreeBlock(void *ptr)
  {
    m_cs.Enter();

    //std::memset(ptr, 0xaa, sizeof(T));

    static_cast<Block *>(ptr)->next = m_free;

    m_free = static_cast<Block *>(ptr);

    ++m_frees;
    --m_blocks_used;

    m_cs.Leave();
  }

} // SF namespace

#endif // SF_MEMORY_MANAGER_H
