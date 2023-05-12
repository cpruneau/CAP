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
#include <ostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "TObject.h"
#include "TString.h"

#include "Aliases.hpp"
using std::ostream;
using std::endl;
using std::vector;
using namespace std;

namespace CAP
{

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
  Configuration(const String & _name);

  //!
  //! Copy Constructor
  //!
  Configuration(const Configuration & config);
  
  //!
  //! Copy operator
  //!
  const Configuration & operator=(const Configuration & config);

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
  void addParameter(const String & _name, bool value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const String & _name, int value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const String & _name, long value);

  //!
  //! Add a double parameter to the configuration with the given name and value
  //!
  void addParameter(const String & _name, double value);

  //!
  //! Add a string parameter to the configuration with the given name and value
  //!
  void addParameter(const String & _name, const String & value);

  //!
  //! Add a bool parameter to the configuration with the given name and value
  //!
  void addParameter(const String & path, const String & _name, bool value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const String & path, const String & _name, int value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const String & path, const String & _name, long value);

  //!
  //! Add a double parameter to the configuration with the given name and value
  //!
  void addParameter(const String & path, const String & _name, double value);

  //!
  //! Add a string parameter to the configuration with the given name and value
  //!
  void addParameter(const String & path, const String & _name, const String & value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & _name, bool value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & _name, int value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & _name, long value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & _name, double value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & _name, const String & value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & path, const String & _name, bool value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & path, const String & _name, int value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & path, const String & _name, long value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & path, const String & _name, double value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void addParameter(const String & path, const String & _name, const String & value);

  //!
  //! Set the parameters to values supplied in the given configuration
  //!
  void addParameters(const Configuration &  configuration);

  //!
  //! Get the value of the parameter named 'name'
  //!
  bool     getValueBool(const String & _name)   const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  int      getValueInt(const String & _name)    const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  long     getValueLong(const String & _name)    const;

  
  //!
  //! Get the value of the parameter named 'name'
  //!
  double   getValueDouble(const String & _name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  String  getValueString(const String & _name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  bool     getValueBool(const String & path, const String & _name)   const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  int      getValueInt(const String & path, const String & _name)    const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  long     getValueLong(const String & path, const String & _name)    const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  double   getValueDouble(const String & path, const String & _name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  String  getValueString(const String & path, const String & _name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  inline void setName(const String & _name) {  name = _name;  }

  //!
  //! Get the value of the parameter named 'name'
  //!
  inline String getName() const { return name;  }

  //!
  //! Get the value of the parameter named 'name'
  //!
  int getNParameters();

  //!
  //!Generates and stores in the configuration of this task a list of key,value parameters based on the given parameters.value
  //!The keys generated have the form keyBaseName## where keyBaseName is the base name of the keys and ## is an integer k
  //!from 0 to nKeysToGenerate-1 (inclusively)
  //!
  //!@param keyBaseName base name of the keys
  //!@param defaultValue default value given for all key,value pairs
  //!@param nKeysToGenerate number of key,value pairs to generate and add to this task configuration.
  //!
  void generateKeyValuePairs(const String & keyBaseName, const String & defaultValue, int nKeysToGenerate);

  void generateKeyValuePairs(const String & path, const String & keyBaseName, const String & defaultValue, int nKeysToGenerate);

  //!
  //!Parse the configuration associated with task to find the key,value pairs that DO NOT feature the defaultValue
  //!
  //!@param keyBaseName base name of the keys
  //!@param defaultValue default value that is selected against
  //!
  VectorString  getSelectedValues(const String & keyBaseName, const String & defaultValue) const;

  VectorString  getSelectedValues(const String & path, const String & keyBaseName, const String & defaultValue) const;

  //!
  //!Get  the number of key,value pairs with the given key base name  that DO NOT feature the defaultValue
  //!
  //!@param keyBaseName base name of the keys to be tested against.
  //!@param defaultValue default value that is selected against
  //!
  int getNSelectedValues(const String & keyBaseName, const String & defaultValue)  const;

  int getNSelectedValues(const String & path, const String & keyBaseName, const String & defaultValue) const
  {
  return getNSelectedValues(path+TString(":")+keyBaseName, defaultValue);
  }

  int getNPossibleValues(const String & keyBaseName) const;

  int getNPossibleValues(const String & path, const String & keyBaseName) const
  {
  return getNPossibleValues(path+TString(":")+keyBaseName);
  }

  //!
  //!Add key,value pairs with the given key base name  and values provided in the selectedValues array.
  //!
  //!@param keyBaseName base name of the keys to be used
  //!@param defaultValue default value that is selected against
  //!@param selectedValues  number of key,value pairs to be checked
  //!
  void addSelectedValues(const String & keyBaseName, const String & defaultValue, const VectorString  & selectedValues);

  void addSelectedValues(const String & path, const String & keyBaseName, const String & defaultValue, const VectorString  & selectedValues)
  {
  addSelectedValues(path+TString(":")+keyBaseName, defaultValue, selectedValues);
  }

  bool isBool(const String & _name) const;
  bool isInt(const String & _name) const;
  bool isLong(const String & _name) const;
  bool isDouble(const String & _name) const;
  bool isString(const String & _name) const;
  bool isFound(const String & _name) const;
  //!
  //! Get the value of the parameter named 'name'
  //!
  virtual void printConfiguration(ostream & os);

  void readFromFile(const String & inputFileName);

  void writeToFile(const String & outputFileName);

  bool hasEndColon(const String & path) const;
  

  protected:
    
  //!
  //! Name of this configuration
  //!
  String name;
  std::map<TString,bool>    boolMap;
  std::map<TString,int>     intMap;
  std::map<TString,long>    longMap;
  std::map<TString,double>  doubleMap;
  std::map<TString,TString> stringMap;

  ClassDef(Configuration,0)
  
};

} // namespace CAP

#endif /* Configuration_hpp */
