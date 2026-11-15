/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Octree.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:48:17 - nwilliam
**  \brief   Straight forward Octree, space partitioning structure.
*******************************************************************************/

#ifndef SF_OCTREE_H
#define SF_OCTREE_H

#include <vector>

#include "Math/TVector3.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Utilities
{
  template <typename T>
  class Octree
  {
    public:
      Octree(unsigned int level, float worldSize);
      Octree(Octree<T> *parent, unsigned int level, float worldSize, const Math::Vector3 &center);

      void AddItem(const T *item);
      bool RemoveItem(const T *item);
      void Update();

    private:
      void IncrementItemCount();
      void DecrementItemCount();

      void CreateChild(unsigned char child_index);
      void RemoveChild(unsigned char child_index);

      unsigned char GetChildIndex(const Math::Vector3& point);
      Math::Vector3 GetChildCenter(unsigned char child_index);

      void MoveItemUp(const T *item);
      void MoveItemDown(const T *item);

      bool Contains(const T *item);

      void SplitNode();
      void JoinNode();

      typedef typename std::vector<T>::iterator ItemIterator;

      Math::Vector3   m_center;
      float           m_halfSize;

      Octree<T> *     m_parent;
      Octree<T> *     m_child[8];
      unsigned int    m_childCount;

      std::vector<T>  m_items;
      unsigned int    m_itemCountTotal;

      unsigned int    m_level;
  };

} // namespace Utilities
} // namespace SF



namespace SF {
namespace Utilities
{
  /*****************************************************************************
    Constructors
  *****************************************************************************/
  template <typename T>
  Octree<T>::Octree(unsigned int level, float worldSize)
  {
    m_parent = 0;

    m_level = level;

    m_childCount = 0;
    for(int i = 0; i < 8; ++i)
      m_child[i] = 0;

    m_itemCountTotal = 0;

    m_center = Math::Vector3(0,0,0);
    m_halfSize = worldSize * 0.5f;
  }

  template <typename T>
  Octree<T>::Octree(Octree<T> *parent, unsigned int level, float worldSize, const Math::Vector3 &center)
  {
    m_parent = parent;

    m_level = level;

    m_childCount = 0;
    for(int i = 0; i < 8; ++i)
      m_child[i] = 0;

    m_itemCountTotal = 0;

    m_center = center;
    m_halfSize = worldSize * 0.5f;
  }


  /*****************************************************************************
    Public Interface
  *****************************************************************************/
  template <typename T>
  void Octree<T>::AddItem(const T *item)
  {
    m_items.push_back(item);

    IncrementItemCount();
  }

  template <typename T>
  bool Octree<T>::RemoveItem(const T *item)
  {
    if(m_items.size() > 0)
    {
      ItemIterator iter = std::find(m_items.begin(), m_items.end(), item);

      if(iter != m_items.end())
      {
        m_items.erase(iter);
        return true;
      }
    }

    unsigned char child_index = 0;// = GetChildIndex(item->GetGeometry()->GetPosition());

    if(m_child[child_index])
    {
      bool removed = m_child[child_index]->RemoveItem(item);

      return removed;
    }
    else
    {
      return false;
    }
  }

  template <typename T>
  void Octree<T>::Update()
  {
    if(m_items.size() > 0)
    {
      std::list<T> items_temp(m_items);

      m_items.clear();

      for(int i = 0; i < items_temp.size(); ++i)
      {
        if( Contains(items_temp[i]) )
        {
          // The item fits in this node, so we try to move it down
          MoveItemDown(items_temp[i]);
        }
        else
        {
          // The item doesn't fit in this node, so we move it up
          MoveItemUp(items_temp[i]);
        }
      }
    }

    if(m_itemCountTotal == 0)
    {
      JoindNode();
    }
    else
    {
      for(int i = 0; i < 8; ++i)
      {
        if(m_child[i])
          m_child[i]->Update();
      }
    }
  }

  template <typename T>
  void Octree<T>::IncrementItemCount()
  {
    ++m_itemCountTotal;

    if(m_parent)
      m_parent->IncrementItemCount();
  }

  template <typename T>
  void Octree<T>::DecrementItemCount()
  {
    --m_itemCountTotal;

    if(m_parent)
      m_parent->DecrementItemCount();
  }

  /*****************************************************************************
    Child Management
  *****************************************************************************/
  template <typename T>
  void Octree<T>::CreateChild(unsigned char child_index)
  {
    if(!m_child[child_index])
    {
      Math::Vector3 child_center = GetChildCenter(child_index);

      m_child[child_index] = new Octree<T>(this, m_level - 1, m_halfSize, m_center + child_center);
      ++m_childCount;
    }
  }

  template <typename T>
  void Octree<T>::RemoveChild(unsigned char child_index)
  {
    if(m_child[child_index])
    {
      // Remove grandchildren
      for (unsigned char i = 0; i < 8; ++i)
      {
        if (m_child[child_index].m_child[i] != null)
        {
          m_child[child_index].RemoveChild(i);
        }
      }

      // Remove child points
      m_child[child_index].m_items.clear();

      delete m_child[child_index];
      --m_childCount;
    }
  }

  template <typename T>
  unsigned char Octree<T>::GetChildIndex(const Math::Vector3& point)
  {
    Math::Vector3 local_point = point - m_center;

    unsigned int child_index = 0;

    if (local_point.x >= 0)
      child_index |= (1 << 0);
    if (local_point.y >= 0)
      child_index |= (1 << 1);
    if (local_point.z >= 0)
      child_index |= (1 << 2);

    return child_index;
  }

  template <typename T>
  Math::Vector3 Octree<T>::GetChildCenter(unsigned char child_index)
  {
    float quarterSize = m_halfSize * 0.5f;

    Vector3 child_center = new Vector3();

    if ((child_index & (1 << 0)) != 0)
      child_center.x += quarterSize;
    else
      child_center.x -= quarterSize;

    if ((child_index & (1 << 1)) != 0)
      child_center.y += quarterSize;
    else
      child_center.y -= quarterSize;

    if ((child_index & (1 << 2)) != 0)
      child_center.z += quarterSize;
    else
      child_center.z -= quarterSize;

    return child_center;
  }

  /*****************************************************************************
    Item Movement
  *****************************************************************************/
  template <typename T>
  void Octree<T>::MoveItemUp(const T *item)
  {
    if(!m_parent)
    {
      AddItem(item);
    }
    else
    {
      if(m_parent->Contains(item))
      {
        m_parent->MoveItemDown(item);
      }
      else
      {
        m_parent->MoveItemUp(item);
      }
    }
  }

  template <typename T>
  void Octree<T>::MoveItemDown(const T *item)
  {
    if(m_level == 1)
    {
      AddItem(item);
    }
    else
    {
      SplitNode();

      unsigned char child_index = GetChildIndex(item->GetGeometry()->GetPosition());

      if(m_child[child_index]->Contains(item))
      {
        m_child[child_index]->MoveItemDown(item);
      }
      else
      {
        AddItem(item);
      }
    }
  }

  /*****************************************************************************
    Item containment
  *****************************************************************************/
  template <typename T>
  bool Octree<T>::Contains(const T *item)
  {
    // TODO
    return false;
  }

  /*****************************************************************************
    Node splitting/joining
  *****************************************************************************/
  template <typename T>
  void Octree<T>::SplitNode()
  {
    for(unsigned char i = 0; i < 8; ++i)
    {
      if(m_child[i])
        CreateChild(i);
    }
  }

  template <typename T>
  void Octree<T>::JoinNode()
  {
    if(m_childCount > 0)
    {
      for(unsigned char i = 0; i < 8; ++i)
      {
        if(m_child[i])
        {
          // Join children nodes
          if(m_child[i]->m_childCount > 0)
            m_child[i]->JoinNode();

          // Move child's items into this node
          while(m_child[i]->m_items.size() > 0)
            m_items.push_back( m_child[i]->m_items[0] );

          RemoveChild(i);
        }
      }
    }
  }

} // namespace Utilities
} // namespace SF

#endif // SF_OCTREE_H
