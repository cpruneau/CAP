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
#ifndef CAP__Filter
#define CAP__Filter
#include "TString.h"
#include <ostream>

class Condition
{
public:

  int filterType;
  int filterSubtype;
  double minimum;
  double maximum;


  Condition(int _filterType,
            int _filterSubtype,
            double _minimum,
            double _maximum)
  :
  filterType(_filterType),
  filterSubtype(_filterSubtype),
  minimum(_minimum),
  maximum(_maximum)
  {}

  Condition(const Condition & other)
  :
  filterType(other.filterType),
  filterSubtype(other.filterSubtype),
  minimum(other.minimum),
  maximum(other.maximum)
  {}

  virtual ~Condition() {}

  const Condition & operator=(const Condition & other)
  {
  if (this!=&other)
    {
    filterType    = other.filterType;
    filterSubtype = other.filterSubtype;
    minimum = other.minimum;
    maximum = other.maximum;
    }
  return *this;
  }

  inline bool accept(double value)
  {
  return (value>=minimum && value<=maximum);
  }

  virtual void printProperties(std::ostream & os)
  {
  os
  << "Type:" << filterType << " Subtype: " << filterSubtype
  << " minimum: " << minimum << " maximum: " << maximum;
  }

};

class ConditionOr : public Condition
{
public:

  double minimum2;
  double maximum2;

  ConditionOr(int _filterType,
              int _filterSubtype,
              double _minimum,
              double _maximum,
              double _minimum2,
              double _maximum2)
  :
  Condition(_filterType,_filterSubtype,_minimum,_maximum),
  minimum2(_minimum2),
  maximum2(_maximum2)
  {}

  ConditionOr(const ConditionOr & other)
  :
  Condition(other),
  minimum2(other.minimum2),
  maximum2(other.maximum2)
  {}

  virtual ~ConditionOr() {}

  const ConditionOr & operator=(const ConditionOr & other)
  {
  if (this!=&other)
    {
    filterType    = other.filterType;
    filterSubtype = other.filterSubtype;
    minimum = other.minimum;
    maximum = other.maximum;
    minimum2 = other.minimum2;
    maximum2 = other.maximum2;
    }
  return *this;
  }

  virtual void printProperties(std::ostream & os)
  {
  os
  << "Type:" << filterType << " Subtype: " << filterSubtype
  << " minimum: " << minimum << " maximum: " << maximum
  << " minimum2: " << minimum2 << " maximum2: " << maximum2;
  }

  virtual bool accept(double value)
  {
  return (value>=minimum && value<=maximum) || (value>=minimum2 && value<=maximum2);
  }
};



//!
//! Abstract and templated filter
//!

template < class T >
class Filter //: public TObject
{
protected:

public:

  //!
  //! Ctor
  //!
  Filter()
  :
  name(""),
  longName(""),
  title(""),
  longTitle(""),
  conditions()
  {
  // no ops
  }
  
  //!
  //! Dtor
  //!
  virtual ~Filter()
  {
  for (unsigned int k=0; k<conditions.size();k++)
    {
    delete conditions[k];
    }
  conditions.clear();
  }

  //!
  //! Test whether the given object is acceptable based on this filters criteria.object
  //! The base class uses the defaultAccept parameter to return acceptability status.
  //!
  virtual bool accept(const T & object __attribute__((unused)))
  {
     return true;
  }

  virtual bool accept(const T & object1 __attribute__((unused)), const T & object2 __attribute__((unused)))
  {
     return true;
  }


  //!
  //! Return the short name of this filter
  //! Call the initialize() method to automatically set names and titles of this filter
  //! or call the corresponding set methods directly to set them manually
  //!
  TString getName() const
  {
  return name;
  }

  //!
  //! Return the short title of this filter
  //! Call the initialize() method to automatically set names and titles of this filter
  //! or call the corresponding set methods directly to set them manually
  //!
  TString getTitle() const
  {
  return title;
  }

  //!
  //! Return the long name of this filter
  //! Call the initialize() method to automatically set names and titles of this filter
  //! or call the corresponding set methods directly to set them manually
  //!
  TString getLongName() const
  {
  return longName;
  }

  //!
  //! Return the long title of this filter
  //! Call the initialize() method to automatically set names and titles of this filter
  //! or call the corresponding set methods directly to set them manually
  //!
  TString getLongTitle() const
  {
  return longTitle;
  }

  //!
  //! Stream  the properties and attributes of this filter onto the given stream.
  //!
  virtual void printProperties(std::ostream & os)
  {
  os << "      Filter: " << name << std::endl;
  os << "   long name: " << longName << std::endl;
  os << "       title: " << title << std::endl;
  os << "  long title: " << longTitle << std::endl;
  for (unsigned int k=0; k<conditions.size();k++)
    {
    os << k << " :: ";
    conditions[k]->printProperties(os);
    os << std::endl;
    }
  }

  //!
  //! Set the short name of this filter
  //!
  void setName(const TString  & newName)
  {
  name = newName;
  }

  //!
  //! Set the short title of this filter
  //!
  void setTitle(const TString  & newTitle)
  {
  title = newTitle;
  }

  //!
  //! Set the long name of this filter
  //!
  void setLongName(const TString  & newLongName)
  {
  longName = newLongName;
  }

  //!
  //! Set the long title of this filter
  //!
  void setLongTitle(const TString  & newLongTitle)
  {
  longTitle = newLongTitle;
  }

  void addCondition(unsigned int type, unsigned int subtype, double minimum, double maximum)
  {
  Condition * condition = new Condition(type,subtype,minimum,maximum);
  conditions.push_back(condition);
  }

  void addCondition(unsigned int type, unsigned int subtype, double minimum, double maximum, double minimum2, double maximum2)
  {
  Condition * condition = new ConditionOr(type,subtype,minimum,maximum,minimum2,maximum2);
  conditions.push_back(condition);
  }


  inline unsigned int getNConditions() const
  {
  return conditions.size();
  }

  const std::vector<Condition*> & getConditions() const
  {
  return conditions;
  }

protected:

  TString name;
  TString longName;
  TString title;
  TString longTitle;
  std::vector<Condition*> conditions;

  ClassDef(Filter,0)
};


#endif /* CAP__Filter  */

