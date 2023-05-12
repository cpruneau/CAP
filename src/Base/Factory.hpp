/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/
#ifndef CAP__Factory
#define CAP__Factory

//!
//! Factory
//!
//! Generic base class for factory services.
//! A factory can be used to generate and own a large collection of objects of the same type "T". Objects are allocated on the heap and pointers "T*" to these
//! objects kept in a large array T ** objects. The array is nominally of fixed size but is enlarged whenever more objects are needed. The factory model is useful
//! when objects need to be repeatedly used and discarded (e.g., in data analysis of particles) because no malloc or destroy is required. The memory remains allocated
//! and so there is no time wasted creating and destroying the objects. The reset method is to be called on a factory to indicate that a new "event" is being considered.
//! It must evidently be possible to initialize the objects with "set" methods specific to the class "T". Use the  T * getNextObject() method to obtain an used object.
//!
#include <iostream>
#include "TObject.h"
#include "Exceptions.hpp"

using namespace std;

namespace CAP
{

template < class T >
class Factory : public TObject
{
protected:
  /////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////
  long capacity;
  long index;
  T ** objects;

  public:
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////
  Factory()
  :
  TObject(),
  capacity(0),
  index(0),
  objects(0)
  {
   // no ops
  }
  
  virtual ~Factory()
  {
  delete[] objects;
  }

  void initialize(int initialCapacity)
  {
  if (objects) delete[] objects;
    capacity = initialCapacity;
    objects = new T*[capacity];
    for (int k=0; k<capacity; k++)
      {
      objects[k] = new T();
      }
  }

  void reset()
  {
  index = 0;
  }

  // Returns the capacity of this store/factory
  long getCapacity() const
  {
    return capacity;
  }

  // Returns the number of object currently
  // in use.
  long getCurrentSize() const
  {
    return index;
  }

  long getLastIndex() const
  {
  return index-1;
  }

  T * getNextObject() 
  {
  if (index<capacity)
    {
    ///cout << "Factory<T>::getNextObject() Index: " << index << " Capacity: " << capacity <<  endl;
    return objects[index++];
    }
  else
    {
    ///cout << "Factory<T>::getNextObject() Index: " << index << " Capacity: " << capacity << " ==================================  Capacity will be doubled."  << endl;
    T** keep = objects;
    long newCapacity = 2*capacity;
    objects = new T*[newCapacity];
    if (!objects)
      {
      throw FactoryException(capacity, newCapacity, "Unable to allocate new capacity","Factory::getNextObject()");
      }
    for (long k=0; k<capacity; k++) objects[k] = keep[k];
    for (long k=capacity; k<newCapacity; k++) objects[k] = new T();
    delete[] keep;
    capacity = newCapacity;
    cout << "<W> Factory::getObject() Object capacity increased to : " << capacity << endl;
    return objects[index++];
    }
  }

//  T * getObjectAt(unsigned long index)
//  {
//  if (index>=0 && index<capacity)
//     {
//     return objects[index];
//     }
//   else
//     {
//     cout << "<FATAL> Factory::getObjectAt() Object stored is exceeded" << endl;
//     return 0;
//     }
//  }

  ClassDef(Factory,0)
};

} // namespace CAP

#endif /* Factory_hpp */
