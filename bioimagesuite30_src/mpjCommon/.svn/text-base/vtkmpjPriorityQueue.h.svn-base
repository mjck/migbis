

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


// .NAME vtkmpjPriorityQueue - an list of ids arranged in priority order
// .SECTION Description
// vtkmpjPriorityQueue is a general object for creating and manipulating lists
// of object ids (e.g., point or cell ids). Object ids are sorted according
// to a user-specified priority, where entries at the top of the queue have
// the smallest values.
//
// This implementation provides a feature beyond the usual ability to insert
// and retrieve (or pop) values from the queue. It is also possible to
// pop any item in the queue given its id number. This allows you to delete
// entries in the queue which can useful for reinserting an item into the
// queue. 
//
// .SECTION Caveats
// This implementation is a variation of the priority queue described in
// "Data Structures & Algorithms" by Aho, Hopcroft, Ullman. It creates 
// a balanced, partially ordered binary tree implemented as an ordered
// array. This avoids the overhead associated with parent/child pointers,
// and frequent memory allocation and deallocation.

#ifndef __vtkmpjPriorityQueue_h
#define __vtkmpjPriorityQueue_h

#include "vtkObject.h"
#include "vtkIdTypeArray.h"

//BTX
typedef struct _vtkmpjPriorityItem
  {
    double priority;
    vtkIdType id;
} vtkmpjPriorityItem;
//ETX

class vtkmpjPriorityQueue : public vtkObject
{
public:
  // Description:
  // Instantiate priority queue with default size and extension size of 1000.
  static vtkmpjPriorityQueue *New();

  vtkTypeMacro(vtkmpjPriorityQueue,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Allocate initial space for priority queue.
  void Allocate(const vtkIdType sz, const vtkIdType ext=1000);

  // Description:
  // Insert id with priority specified. The id is generally an
  // index like a point id or cell id.
  void Insert(double priority, vtkIdType id);

  // Description:
  // Removes item at specified location from tree; then reorders and
  // balances tree. The location == 0 is the root of the tree. If queue
  // is exhausted, then a value < 0 is returned. (Note: the location
  // is not the same as deleting an id; id is mapped to location.)

  vtkIdType Pop(double &priority, vtkIdType location=0);

  // Description:
  // Same as above but simplified for easier wrapping into interpreted
  // languages.
  vtkIdType Pop(vtkIdType location=0);

  // Description:
  // Peek into the queue without actually removing anything. Returns the
  // id and the priority.
  vtkIdType Peek(double &priority, vtkIdType location=0);
  
  // Description:
  // Peek into the queue without actually removing anything. Returns the
  // id.
  vtkIdType Peek(vtkIdType location=0);

  // Description:
  // Delete entry in queue with specified id. Returns priority value
  // associated with that id; or VTK_DOUBLE_MAX if not in queue.
  double DeleteId(vtkIdType id);

  // Description:
  // Get the priority of an entry in the queue with specified id. Returns
  // priority value of that id or VTK_DOUBLE_MAX if not in queue.
  double GetPriority(vtkIdType id);

  // Description:
  // Return the number of items in this queue.
  vtkIdType GetNumberOfItems() {return this->MaxId+1;};

  // Description:
  // Empty the queue but without releasing memory. This avoids the
  // overhead of memory allocation/deletion.
  void Reset();

protected:
  vtkmpjPriorityQueue();
  ~vtkmpjPriorityQueue();
  
  vtkmpjPriorityItem *Resize(const vtkIdType sz);

  vtkIdTypeArray *ItemLocation;
  vtkmpjPriorityItem *Array;
  vtkIdType Size;
  vtkIdType MaxId;
  vtkIdType Extend;
private:
  vtkmpjPriorityQueue(const vtkmpjPriorityQueue&);  // Not implemented.
  void operator=(const vtkmpjPriorityQueue&);  // Not implemented.
};

inline double vtkmpjPriorityQueue::DeleteId(vtkIdType id)
{
  double priority=VTK_DOUBLE_MAX;
  int loc;

  if ( id <= this->ItemLocation->GetMaxId() &&  
  (loc=this->ItemLocation->GetValue(id)) != -1 )
    {
    this->Pop(priority,loc);
    }
  return priority;
}

inline double vtkmpjPriorityQueue::GetPriority(vtkIdType id)
{
  int loc;

  if ( id <= this->ItemLocation->GetMaxId() &&  
  (loc=this->ItemLocation->GetValue(id)) != -1 )
    {
    return this->Array[loc].priority;
    }
  return VTK_DOUBLE_MAX;
}

inline vtkIdType vtkmpjPriorityQueue::Peek(double &priority, vtkIdType location)
{
  if ( this->MaxId < 0 )
    {
    return -1;
    }
  else
    {
    priority = this->Array[location].priority;
    return this->Array[location].id;
    }
}

inline vtkIdType vtkmpjPriorityQueue::Peek(vtkIdType location)
{
  if ( this->MaxId < 0 )
    {
    return -1;
    }
  else
    {
    return this->Array[location].id;
    }
}

#endif
