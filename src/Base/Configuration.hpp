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
using std::ostream;
using std::endl;
using std::vector;
using namespace std;

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
  void addParameter(const TString & _name, bool value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & _name, int value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & _name, long value);

  //!
  //! Add a double parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & _name, double value);

  //!
  //! Add a string parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & _name, const TString & value);

  //!
  //! Add a bool parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & path, const TString & _name, bool value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & path, const TString & _name, int value);

  //!
  //! Add an int parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & path, const TString & _name, long value);

  //!
  //! Add a double parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & path, const TString & _name, double value);

  //!
  //! Add a string parameter to the configuration with the given name and value
  //!
  void addParameter(const TString & path, const TString & _name, const TString & value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & _name, bool value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & _name, int value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & _name, long value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & _name, double value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & _name, const TString & value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & path, const TString & _name, bool value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & path, const TString & _name, int value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & path, const TString & _name, long value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & path, const TString & _name, double value);

  //!
  //! Set the parameter named 'name'  to the given value
  //!
  void setParameter(const TString & path, const TString & _name, const TString & value);

  //!
  //! Set the parameters to values supplied in the given configuration
  //!
  void setParameters(const Configuration &  configuration);

  //!
  //! Get the value of the parameter named 'name'
  //!
  bool     getValueBool(const TString & _name)   const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  int      getValueInt(const TString & _name)    const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  long     getValueLong(const TString & _name)    const;

  
  //!
  //! Get the value of the parameter named 'name'
  //!
  double   getValueDouble(const TString & _name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  TString  getValueString(const TString & _name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  bool     getValueBool(const TString & path, const TString & _name)   const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  int      getValueInt(const TString & path, const TString & _name)    const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  long     getValueLong(const TString & path, const TString & _name)    const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  double   getValueDouble(const TString & path, const TString & _name) const;

  //!
  //! Get the value of the parameter named 'name'
  //!
  TString  getValueString(const TString & path, const TString & _name) const;

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
  //!Generates and stores in the configuration of this task a list of key,value parameters based on the given parameters.value
  //!The keys generated have the form keyBaseName## where keyBaseName is the base name of the keys and ## is an integer k
  //!from 0 to nKeysToGenerate-1 (inclusively)
  //!
  //!@param keyBaseName base name of the keys
  //!@param defaultValue default value given for all key,value pairs
  //!@param nKeysToGenerate number of key,value pairs to generate and add to this task configuration.
  //!
  void generateKeyValuePairs(const TString & keyBaseName, const TString & defaultValue, int nKeysToGenerate);

  void generateKeyValuePairs(const TString & path, const TString & keyBaseName, const TString & defaultValue, int nKeysToGenerate);

  //!
  //!Parse the configuration associated with task to find the key,value pairs that DO NOT feature the defaultValue
  //!
  //!@param keyBaseName base name of the keys
  //!@param defaultValue default value that is selected against
  //!
  vector<TString> getSelectedValues(const TString & keyBaseName, const TString & defaultValue) const;

  vector<TString> getSelectedValues(const TString & path, const TString & keyBaseName, const TString & defaultValue) const;

  //!
  //!Get  the number of key,value pairs with the given key base name  that DO NOT feature the defaultValue
  //!
  //!@param keyBaseName base name of the keys to be tested against.
  //!@param defaultValue default value that is selected against
  //!
  int getNSelectedValues(const TString & keyBaseName, const TString & defaultValue)  const;

  int getNSelectedValues(const TString & path, const TString & keyBaseName, const TString & defaultValue) const
  {
  return getNSelectedValues(path+TString(":")+keyBaseName, defaultValue);
  }

  int getNPossibleValues(const TString & keyBaseName) const;

  int getNPossibleValues(const TString & path, const TString & keyBaseName) const
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
  void addSelectedValues(const TString & keyBaseName, const TString & defaultValue, const vector<TString> & selectedValues);

  void addSelectedValues(const TString & path, const TString & keyBaseName, const TString & defaultValue, const vector<TString> & selectedValues)
  {
  addSelectedValues(path+TString(":")+keyBaseName, defaultValue, selectedValues);
  }

  bool isBool(const TString & _name) const;
  bool isInt(const TString & _name) const;
  bool isLong(const TString & _name) const;
  bool isDouble(const TString & _name) const;
  bool isString(const TString & _name) const;
  bool isFound(const TString & _name) const;
  //!
  //! Get the value of the parameter named 'name'
  //!
  virtual void printConfiguration(ostream & os);

  void readFromFile(const TString & inputFileName);

  void writeToFile(const TString & outputFileName);

  bool hasEndColon(const TString & path) const;
  

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
