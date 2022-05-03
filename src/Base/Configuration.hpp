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
#ifndef CAP__Configuration
#define CAP__Configuration
#include <iostream>
#include <map>
#include "TObject.h"
#include "TString.h"
using std::ostream;
using std::endl;
using std::vector;

//!
//!Configuration of a given object or task
//!
class Configuration : public TObject
{
public:
  
  //!
  //! Constructor
  //!
  Configuration();
  
  //!
  //! Constructor
  //!
  Configuration(const TString & _name);

  //!
  //! Copy Constructor
  //!
  Configuration(const Configuration & config);
  
  //!
  //! Copy operator
  //!
  Configuration & operator=(const Configuration & config);

  //!
  //! Dtor
  //!
  virtual ~Configuration() {}

  //!
  //! Clear/erase all parameters held by this configuration
  //!
  void clear();
  
  //!
  //! Add a bool parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & name, bool value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & name, int value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & name, long value);

  //!
  //! Add a double parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & name, double value);

  //!
  //! Add a string parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & name, const TString &  value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & name, bool value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & name, int value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & name, long value);

  
  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & name, double value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & name, const TString &  value);

  //!
  //! Set the parameters to values supplied in the given configuration
  //!
  void setParameters(const Configuration &  config);

  
  //!
  //! Get the value of the parameter named 'name'
  //!
  bool     getValueBool(const TString & name)   const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  int      getValueInt(const TString & name)    const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  int      getValueLong(const TString & name)    const;

  
  //!
  //! Get the value of the parameter named 'name'
  //!
  double   getValueDouble(const TString & name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  TString  getValueString(const TString & name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  inline void setName(const TString & _name) {  name = _name;  }

  //!
  //! Get the value of the parameter named 'name'
  //!
  inline TString getName() const { return name;  }

  //!
  //! Get the value of the parameter named 'name'
  //!
  int getNParameters();
  
  //!
  //! Get the value of the parameter named 'name'
  //!
  virtual void printConfiguration(ostream & os);

  protected:
    
  //!
  //! Name of this configuration
  //!
  TString name;
  std::map<TString,bool>    boolMap;
  std::map<TString,int>     intMap;
  std::map<TString,long>    longMap;
  std::map<TString,double>  doubleMap;
  std::map<TString,TString> stringMap;

  ClassDef(Configuration,0)
  
};

#endif /* Configuration_hpp */
