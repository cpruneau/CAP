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
#ifndef CAP__Collection
#define CAP__Collection

// ====================================================
//
// Collection of pointers to objects of type T
//
// Generic base class for Collection services.
// Set ownership with setOwn()
//
// ====================================================
#include <iostream>
#include "TString.h"
#include "MessageLogger.hpp"

using namespace std;

template < class T >
class Collection : public MessageLogger
{
protected:
  std::vector<T*> objects;
  bool            ownership;
  TString         name;

public:

  Collection()
  :
  MessageLogger(Debug),
  objects(),
  ownership(false),
  name("Unnamed")
  {  }

  Collection(const TString& _name, bool _ownership, LogLevel logLevel)
  :
  MessageLogger(logLevel),
  objects(),
  ownership(_ownership),
  name(_name)
  {  }

  Collection(const Collection & source)
  :
  MessageLogger(source),
  objects(source.objects),
  ownership(source.ownership),
  name()
  {   }


  virtual ~Collection()
  {
    clear();
  }

  Collection & operator=(const Collection source)
  {
  if (this==&source) return *this;
  clear();
  MessageLogger::operator=(source);
  ownership = source.ownership;
  objects   = source.objects;
  name      = source.name;
  return *this;
  }

  bool hasOwnerShip() const
  {
  return ownership;
  }

  void setOwnership(bool owns = true)
  {
    ownership = owns;
  }

  void setName(const TString& _name)
  {
  name = _name;
  }

  const TString& getName() const
  {
  return name;
  }

  void clear()
  {
  
  if (reportInfo(__FUNCTION__)) cout << " Has ownership? " << ownership << endl;
  if (ownership)
    {
    for (unsigned long k=0;k<objects.size();k++)
      {
      delete objects[k];
      }
    }
  objects.clear();
  name = "";
  if (reportInfo(__FUNCTION__)) cout << " Completed!!!!!! " << endl;
  }


  unsigned long size() const
  {
    return objects.size();
  }

  unsigned long getNObject() const
  {
    return objects.size();
  }

  T *  getObjectAt(int index)
  {
  return objects[index];
  }

  T *  operator[](int index)
  {
  return objects[index];
  }

  unsigned long getCollectionSize() const
  {
  return objects.size();
  }

  T * push_back(T * object)
  {
  objects.push_back(object);
  return object;
  }

  T * append(T * object)
  {
  objects.push_back(object);
  return object;
  }

  T * back()
  {
  return objects.back();
  }

  bool sameSizeAs(const Collection<T> & other) const
  {
  return objects.size() == other.objects.size();
  }

  // Shallow copy (pointers only)
  void copy(long n, T ** source)
  {
  for (int iObject=0; iObject<n; iObject++)
    {
    objects.push_back(source[iObject]);
    }
  }

  void deepCopy(long n, T ** source)
  {
  for (int iObject=0; iObject<n; iObject++)
    {
    objects.push_back( (T*) source[iObject]->Clone());
    }
  }

  ClassDef(Collection,1)
};

#endif /* CAP__Collection */
