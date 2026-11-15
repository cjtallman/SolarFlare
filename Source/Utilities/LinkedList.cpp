/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    LinkedList.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/06/2008 @ 20:17 - CJT 
**  \brief 
*******************************************************************************/

#include "LinkedList.h"

namespace SF
{
  namespace Utilities
  {

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    /// Linked List Node Implementation 
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //************************************
    // Method:    InitList
    // FullName:  SF::Utilities::LinkedListNode::InitList
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: LinkedListNode * head
    // Parameter: LinkedListNode * tail
    //************************************
    void LinkedListNode::InitList( LinkedListNode * head, LinkedListNode * tail )
    {
      head->next = tail;
      tail->prev = head;
    }

    //************************************
    // Method:    LinkedListNode
    // FullName:  SF::Utilities::LinkedListNode::LinkedListNode
    // Access:    public 
    // Returns:   
    // Qualifier:
    //************************************
    LinkedListNode::LinkedListNode()
    {
      next = 0;
      prev = 0;
    }

    //************************************
    // Method:    ~LinkedListNode
    // FullName:  SF::Utilities::LinkedListNode::~LinkedListNode
    // Access:    public 
    // Returns:   
    // Qualifier:
    //************************************
    LinkedListNode::~LinkedListNode()
    {
      Remove();
    }

    //************************************
    // Method:    Next
    // FullName:  SF::Utilities::LinkedListNode::Next
    // Access:    public 
    // Returns:   LinkedListNode *
    // Qualifier:
    //************************************
    LinkedListNode * LinkedListNode::Next()
    {
      return next;
    }

    //************************************
    // Method:    Prev
    // FullName:  SF::Utilities::LinkedListNode::Prev
    // Access:    public 
    // Returns:   LinkedListNode *
    // Qualifier:
    //************************************
    LinkedListNode * LinkedListNode::Prev()
    {
      return prev;
    }

    //************************************
    // Method:    IsInList
    // FullName:  SF::Utilities::LinkedListNode::IsInList
    // Access:    public 
    // Returns:   bool
    // Qualifier:
    //************************************
    bool LinkedListNode::IsInList()
    {
      return (prev != 0 && next != 0);
    }

    //************************************
    // Method:    InsertBefore
    // FullName:  SF::Utilities::LinkedListNode::InsertBefore
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: LinkedListNode * node
    //************************************
    void LinkedListNode::InsertBefore( LinkedListNode * node )
    {
      if (node != 0 && IsInList() == false)
      {
        next = node;
        prev = node->prev;
        node->prev->next = this;
        node->prev = this;
      }
    }

    //************************************
    // Method:    InsertAfter
    // FullName:  SF::Utilities::LinkedListNode::InsertAfter
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: LinkedListNode * node
    //************************************
    void LinkedListNode::InsertAfter( LinkedListNode * node )
    {
      if (node != 0 && IsInList() == false)
      {
        prev = node;
        next = node->next;
        node->next->prev = this;
        node->next = this;
      }
    }

    //************************************
    // Method:    Remove
    // FullName:  SF::Utilities::LinkedListNode::Remove
    // Access:    public 
    // Returns:   void
    // Qualifier:
    //************************************
    void LinkedListNode::Remove()
    {
      if (prev != 0)
        prev->next = next;
      if (next != 0)
        next->prev = prev;

      next = prev = 0;
    }


    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    /// Linked List Implementation 
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //************************************
    // Method:    LinkedList
    // FullName:  SF::Utilities::LinkedList::LinkedList
    // Access:    public 
    // Returns:   
    // Qualifier:
    //************************************
    LinkedList::LinkedList()
    {
      head = new LinkedListNode();
      tail = new LinkedListNode();

      LinkedListNode::InitList(head, tail);
    }

    //************************************
    // Method:    ~LinkedList
    // FullName:  SF::Utilities::LinkedList::~LinkedList
    // Access:    public 
    // Returns:   
    // Qualifier:
    //************************************
    LinkedList::~LinkedList()
    {
      Clear();
    }

    //************************************
    // Method:    Empty
    // FullName:  SF::Utilities::LinkedList::Empty
    // Access:    public 
    // Returns:   bool
    // Qualifier:
    //************************************
    bool LinkedList::Empty()
    {
      return head->Next() == tail;
    }

    //************************************
    // Method:    Front
    // FullName:  SF::Utilities::LinkedList::Front
    // Access:    public 
    // Returns:   LinkedListNode *
    // Qualifier:
    //************************************
    LinkedListNode * LinkedList::Front()
    {
      if( Empty() )
        return head;
      else
        return head->Next();
    }

    //************************************
    // Method:    Back
    // FullName:  SF::Utilities::LinkedList::Back
    // Access:    public 
    // Returns:   LinkedListNode *
    // Qualifier:
    //************************************
    LinkedListNode * LinkedList::Back()
    {
      if( Empty() )
        return tail;
      else
        return tail->Prev();
    }

    //************************************
    // Method:    PushFront
    // FullName:  SF::Utilities::LinkedList::PushFront
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: LinkedListNode * item
    //************************************
    void LinkedList::PushFront( LinkedListNode * item )
    {
      item->InsertAfter(head);
    }

    //************************************
    // Method:    PushBack
    // FullName:  SF::Utilities::LinkedList::PushBack
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: LinkedListNode * item
    //************************************
    void LinkedList::PushBack( LinkedListNode * item )
    {
      item->InsertBefore(tail);
    }

    //************************************
    // Method:    PopFront
    // FullName:  SF::Utilities::LinkedList::PopFront
    // Access:    public 
    // Returns:   LinkedListNode *
    // Qualifier:
    //************************************
    LinkedListNode * LinkedList::PopFront()
    {
      if(Empty())
        return 0;
      else
        return Remove(Front());
    }

    //************************************
    // Method:    PopBack
    // FullName:  SF::Utilities::LinkedList::PopBack
    // Access:    public 
    // Returns:   LinkedListNode *
    // Qualifier:
    //************************************
    LinkedListNode * LinkedList::PopBack()
    {
      if(Empty())
        return 0;
      else
        return Remove(Back());
    }

    //************************************
    // Method:    InsertBefore
    // FullName:  SF::Utilities::LinkedList::InsertBefore
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: LinkedListNode * new_item
    // Parameter: LinkedListNode * old_item
    //************************************
    void LinkedList::InsertBefore( LinkedListNode * new_item, LinkedListNode * old_item )
    {
      new_item->InsertBefore(old_item);
    }

    //************************************
    // Method:    InsertAfter
    // FullName:  SF::Utilities::LinkedList::InsertAfter
    // Access:    public 
    // Returns:   void
    // Qualifier:
    // Parameter: LinkedListNode * new_item
    // Parameter: LinkedListNode * old_item
    //************************************
    void LinkedList::InsertAfter( LinkedListNode * new_item, LinkedListNode * old_item )
    {
      new_item->InsertAfter(old_item);
    }

    //************************************
    // Method:    Remove
    // FullName:  SF::Utilities::LinkedList::Remove
    // Access:    public 
    // Returns:   LinkedListNode *
    // Qualifier:
    // Parameter: LinkedListNode * node
    //************************************
    LinkedListNode * LinkedList::Remove( LinkedListNode * node )
    {
      if( node->IsInList() )
        node->Remove();

      return node;
    }

    //************************************
    // Method:    Clear
    // FullName:  SF::Utilities::LinkedList::Clear
    // Access:    public 
    // Returns:   void
    // Qualifier:
    //************************************
    void LinkedList::Clear()
    {
      while (Front()->IsInList())   // Not the best implementation. I'll clean it up later.
        Front()->Remove();
    }
  }

}