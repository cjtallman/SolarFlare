/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    LinkedList.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/06/2008 @ 20:07 - CJT 
**  \brief 
*******************************************************************************/

#pragma once

namespace SF
{
  namespace Utilities
  {

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    /// Linked List Node Header
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    class LinkedListNode
    {
      LinkedListNode * next;
      LinkedListNode * prev;
    public:
      static void InitList(LinkedListNode * head, LinkedListNode * tail);
      LinkedListNode();
      ~LinkedListNode();

      LinkedListNode * Next();
      LinkedListNode * Prev();
      bool IsInList();

      void InsertBefore(LinkedListNode * node);
      void InsertAfter(LinkedListNode * node);
      void Remove();
    };


    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    /// Linked List Header
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    class LinkedList
    {
      LinkedListNode * head;
      LinkedListNode * tail;
    public:
      LinkedList();
      ~LinkedList();

      bool Empty();
      LinkedListNode * Front();
      LinkedListNode * Back();

      void PushFront(LinkedListNode * item);
      void PushBack(LinkedListNode * item);
      LinkedListNode * PopFront();
      LinkedListNode * PopBack();
      void InsertBefore(LinkedListNode * new_item, LinkedListNode * old_item);
      void InsertAfter(LinkedListNode * new_item, LinkedListNode * old_item);

      LinkedListNode * Remove(LinkedListNode * node);
      void Clear();
    };


  }
}