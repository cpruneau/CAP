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
#include "Configuration.hpp"
ClassImp(Configuration);

Configuration::Configuration()
:
name(""),
boolMap(),
intMap(),
longMap(),
doubleMap(),
stringMap()
{
  
}

Configuration::Configuration(const TString & _name)
:
name(_name),
boolMap(),
intMap(),
longMap(),
doubleMap(),
stringMap()
{
  
}

Configuration::Configuration(const Configuration & _configuration)
:
name(_configuration.name),
boolMap(),
intMap(),
longMap(),
doubleMap(),
stringMap()
{
  setParameters(_configuration);
}


Configuration & Configuration::operator=(const Configuration & _configuration)
{
  if (this!=&_configuration)
    {
    clear();
    name = _configuration.name;
    setParameters(_configuration);
    }
  return *this;
}

void Configuration::clear()
{
  name = "";
  boolMap.clear();
  intMap.clear();
  longMap.clear();
  doubleMap.clear();
  stringMap.clear();
}

void Configuration::addParameter(const TString & name, bool value)
{
  std::map<TString,bool>::iterator it;
  it = boolMap.find(name);
  if (it != boolMap.end())
    {
    it->second = value;
    }
  else
    {
    boolMap[name] = value;
    }
}

void Configuration::addParameter(const TString & name, int value)
{
  std::map<TString,int>::iterator it;
  it = intMap.find(name);
  if (it != intMap.end())
    {
    it->second = value;
    }
  else
    {
    intMap[name] = value;
    }
}

void Configuration::addParameter(const TString & name, long value)
{
  std::map<TString,long>::iterator it;
  it = longMap.find(name);
  if (it != longMap.end())
    {
    it->second = value;
    }
  else
    {
    longMap[name] = value;
    }
}


void Configuration::addParameter(const TString & name, double value)
{
  std::map<TString,double>::iterator it;
  it = doubleMap.find(name);
  if (it != doubleMap.end())
    {
    it->second = value;
    }
  else
    {
    doubleMap[name] = value;
    }
}

void Configuration::addParameter(const TString & name, const TString & value)
{
  std::map<TString,TString>::iterator it;
  it = stringMap.find(name);
  if (it != stringMap.end())
    {
    it->second = value;
    }
  else
    {
    stringMap[name] = value;
    }
}

void Configuration::setParameter(const TString & name, bool value)
{
  std::map<TString,bool>::iterator it;
  it = boolMap.find(name);
  if (it != boolMap.end())
    {
    it->second = value;
    }
  else
    {
    boolMap[name] = value;
    }
}

void Configuration::setParameter(const TString & name, int value)
{
  std::map<TString,int>::iterator it;
  it = intMap.find(name);
  if (it != intMap.end())
    {
    it->second = value;
    }
  else
    {
    intMap[name] = value;
    }
}

void Configuration::setParameter(const TString & name, long value)
{
  std::map<TString,long>::iterator it;
  it = longMap.find(name);
  if (it != longMap.end())
    {
    it->second = value;
    }
  else
    {
    longMap[name] = value;
    }
}


void Configuration::setParameter(const TString & name, double value)
{
  std::map<TString,double>::iterator it;
  it = doubleMap.find(name);
  if (it != doubleMap.end())
    {
    it->second = value;
    }
  else
    {
    doubleMap[name] = value;
    }
}

void Configuration::setParameter(const TString & name, const TString & value)
{
  std::map<TString,TString>::iterator it;
  it = stringMap.find(name);
  if (it != stringMap.end())
    {
    it->second = value;
    }
  else
    {
    stringMap[name] = value;
    }
}

bool Configuration::isBool(const TString & name) const
{
  std::map<TString,bool>::const_iterator it;
  it = boolMap.find(name);
  return it != boolMap.cend();
}

bool Configuration::isInt(const TString & name) const
{
  std::map<TString,int>::const_iterator it;
  it = intMap.find(name);
  return it != intMap.cend();
}

bool Configuration::isLong(const TString & name) const
{
  std::map<TString,long>::const_iterator it;
  it = longMap.find(name);
  return it != longMap.cend();
}

bool Configuration::isDouble(const TString & name) const
{
  std::map<TString,double>::const_iterator it;
  it = doubleMap.find(name);
  return it != doubleMap.cend();
}


bool Configuration::isString(const TString & name) const
{
  std::map<TString,TString>::const_iterator it;
  it = stringMap.find(name);
  return it != stringMap.cend();
}

bool Configuration::isFound(const TString & name) const
{
  if (isBool(name)) return true;
  if (isInt(name)) return true;
  if (isLong(name)) return true;
  if (isDouble(name)) return true;
  if (isString(name)) return true;
  return false;
}



bool Configuration::getValueBool(const TString & name) const
{
  bool value = false;
  std::map<TString,bool>::const_iterator it;
  it = boolMap.find(name);
  if (it != boolMap.cend())
    {
    value = it->second;
    }
  else
    {
    if (isInt(name))
      std::cout  << " Key:" <<  name << " not a 'bool' but an 'int'. Use 'getValueInt(key)' to retrieve it" <<  std::endl;
    else if (isLong(name))
      std::cout  << " Key:" <<  name << " not a 'bool' but a 'long'. Use 'getValueLong(key)' to retrieve it" <<  std::endl;
    else if (isDouble(name))
      std::cout  << " Key:" <<  name << " not a 'bool' but a 'double'. Use 'getValueDouble(key)' to retrieve it" <<  std::endl;
    else if (isString(name))
      std::cout  << " Key:" <<  name << " not a 'bool' but a 'string'. Use 'getValueString(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> Configuration::getValueBool(): Did not find key:" << name << std::endl;
    value = false;
    }
  return value;
}

int Configuration::getValueInt(const TString & name) const
{
  int value = -99999;
  std::map<TString,int>::const_iterator it;
  it = intMap.find(name);
  if (it != intMap.cend())
    {
    //   std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
    if (isBool(name))
      std::cout  << "<W> Configuration::getValueInt(): Key:" <<  name << " not an 'int' but a 'bool'. Use 'getValueBool(key)' to retrieve it" <<  std::endl;
    else if (isLong(name))
      std::cout  << "<W> Configuration::getValueInt(): Key:" <<  name << " not an 'int' but a 'long'. Use 'getValueLong(key)' to retrieve it" <<  std::endl;
    else if (isDouble(name))
      std::cout  << "<W> Configuration::getValueInt(): Key:" <<  name << " not an 'int' but a 'double'. Use 'getValueDouble(key)' to retrieve it" <<  std::endl;
    else if (isString(name))
      std::cout  << "<W> Configuration::getValueInt(): Key:" <<  name << " not an 'int' but a 'string'. Use 'getValueString(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> Configuration::getValueInt(): Did not find key:" << name << std::endl;
    }
  return value;
}

long Configuration::getValueLong(const TString & name) const
{
  int value = -99999;
  std::map<TString,long>::const_iterator it;
  it = longMap.find(name);
  if (it != longMap.cend())
    {
    //   std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
    if (isBool(name))
      std::cout  << "<W> Configuration::getValueLong(): Key:" <<  name << " not a 'long' but a 'bool'. Use 'getValueBool(key)' to retrieve it" <<  std::endl;
    else if (isInt(name))
      std::cout  << "<W> Configuration::getValueLong(): Key:" <<  name << " not a 'long' but an 'int'. Use 'getValueInt(key)' to retrieve it" <<  std::endl;
    else if (isDouble(name))
      std::cout  << "<W> Configuration::getValueLong(): Key:" <<  name << " not a 'long' but a 'double'. Use 'getValueDouble(key)' to retrieve it" <<  std::endl;
    else if (isString(name))
      std::cout  << "<W> Configuration::getValueLong(): Key:" <<  name << " not a 'long' but a 'string'. Use 'getValueString(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> Configuration::getValueLong(): Did not find key:" << name << std::endl;
    }
  return value;
}


double Configuration::getValueDouble(const TString & name)  const
{
  double value = 0.0;
  std::map<TString,double>::const_iterator it;
  it = doubleMap.find(name);
  if (it != doubleMap.cend())
    {
    //std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
    if (isBool(name))
      std::cout  << "<W> Configuration::getValueDouble(): Key:" <<  name << " not a 'double' but a 'bool'. Use 'getValueBool(key)' to retrieve it" <<  std::endl;
    else if (isInt(name))
      std::cout  << "<W> Configuration::getValueDouble(): Key:" <<  name << " not a 'double' but an 'int'. Use 'getValueInt(key)' to retrieve it" <<  std::endl;
    else if (isLong(name))
      std::cout  << "<W> Configuration::getValueDouble(): Key:" <<  name << " not a 'double' but a 'long'. Use 'getValueLong(key)' to retrieve it" <<  std::endl;
    else if (isString(name))
      std::cout  << "<W> Configuration::getValueDouble(): Key:" <<  name << " not a 'double' but a 'string'. Use 'getValueString(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> Configuration::getValueDouble(): Did not find key:" << name << std::endl;
    }
  return value;
}

TString  Configuration::getValueString(const TString & name) const
{
  TString value = "notFound";
  std::map<TString,TString>::const_iterator it;
  it = stringMap.find(name);
  if (it != stringMap.cend())
    {
    //std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
    if (isBool(name))
      std::cout  << "<W> Configuration::getValueString(): Key:" <<  name << " not a 'string' but a 'bool'. Use 'getValueBool(key)' to retrieve it" <<  std::endl;
    else if (isInt(name))
      std::cout  << "<W> Configuration::getValueString(): Key:" <<  name << " not a 'string' but an 'int'. Use 'getValueInt(key)' to retrieve it" <<  std::endl;
    else if (isLong(name))
      std::cout  << "<W> Configuration::getValueString(): Key:" <<  name << " not a 'string' but a 'long'. Use 'getValueLong(key)' to retrieve it" <<  std::endl;
    else if (isDouble(name))
      std::cout  << "<W> Configuration::getValueString(): Key:" <<  name << " not a 'string' but a 'double'. Use 'getValueDouble(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> Configuration::getValueString(): Did not find key:" << name << std::endl;
    }
  return value;
}

void Configuration::setParameters(const Configuration &  configuration)
{
  //cout << "Configuration::setParameters(const Configuration &  configuration)" << endl;
  //cout << " Size bool: "  << configuration.boolMap.size() << endl;
  for (std::map<TString,bool>::const_iterator it=configuration.boolMap.cbegin(); it!=configuration.boolMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
  //cout << " Size int: "  << configuration.intMap.size() << endl;

  for (std::map<TString,int>::const_iterator it=configuration.intMap.cbegin(); it!=configuration.intMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
  //cout << " Size long: "  << configuration.longMap.size() << endl;

  for (std::map<TString,long>::const_iterator it=configuration.longMap.cbegin(); it!=configuration.longMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
  //cout << " Size double: "  << configuration.doubleMap.size() << endl;

  for (std::map<TString,double>::const_iterator it=configuration.doubleMap.cbegin(); it!=configuration.doubleMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
  //cout << " Size string: "  << configuration.stringMap.size() << endl;

  int k =0;
  for (std::map<TString,TString>::const_iterator it=configuration.stringMap.cbegin(); it!=configuration.stringMap.cend(); ++it)
    {
//    cout << "WTF  " << k++ << endl;
//    cout << "  first:" << it->first << endl;
//    cout << " second:" << it->second << endl;
    setParameter(it->first, it->second);
    }
  //cout << "Configuration::setParameters(const Configuration &  configuration) complted OK" << endl;
}

int Configuration::getNParameters()
{
  int nParameters = 0;
  nParameters += boolMap.size();
  nParameters += intMap.size();
  nParameters += longMap.size();
  nParameters += doubleMap.size();
  nParameters += stringMap.size();
  return nParameters;
}


void Configuration::generateKeyValuePairs(const TString & keyBaseName, const TString & defaultValue, int nKeysToGenerate)
{
  TString key;
  for (int k=0; k<nKeysToGenerate; k++)
    {
    key = keyBaseName; key += k;
    addParameter(key,defaultValue);
    }
}

void Configuration::generateKeyValuePairs(const TString & path, const TString & keyBaseName, const TString & defaultValue, int nKeysToGenerate)
{
  if (path.EndsWith(":"))
    generateKeyValuePairs(path+keyBaseName, defaultValue, nKeysToGenerate);
  else
    generateKeyValuePairs(path+TString(":")+keyBaseName, defaultValue, nKeysToGenerate);
}

vector<TString> Configuration::getSelectedValues(const TString & keyBaseName, const TString & defaultValue) const
{
  vector<TString> selectedValues;
  for (std::map<TString,TString>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName) && !it->second.Contains(defaultValue) ) selectedValues.push_back(it->second);
    }
  return selectedValues;
}

vector<TString> Configuration::getSelectedValues(const TString & path, const TString & keyBaseName, const TString & defaultValue) const
{
  if (path.EndsWith(":"))
    return getSelectedValues(path+keyBaseName, defaultValue);
  else
    return getSelectedValues(path+TString(":")+keyBaseName, defaultValue);
}


int Configuration::getNPossibleValues(const TString & keyBaseName)  const
{
  int nPossible = 0;
  for (std::map<TString,TString>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName)) nPossible++;
    }
  return nPossible;
}

int Configuration::getNSelectedValues(const TString & keyBaseName, const TString & defaultValue)  const
{
  int nSelected = 0;
  for (std::map<TString,TString>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName) && !it->second.Contains(defaultValue) ) nSelected++;
    }
  return nSelected;
}


void Configuration::addSelectedValues(const TString & keyBaseName, const TString & defaultValue, const vector<TString> & selectedValues)
{
  TString key;
  int nSelected = getNSelectedValues(keyBaseName,defaultValue);
  for (unsigned int k=0; k<selectedValues.size(); k++)
    {
    TString key = keyBaseName+(int(nSelected)+k);
    setParameter(key,selectedValues[k]);
    }
}

void Configuration::printConfiguration(ostream & os)
{
  os
  << endl
  << " ------------------------------------------------------------" << std::endl
  << " name: " << name << std::endl
  << " ------------------------------------------------------------" << std::endl;
  
  for (std::map<TString,bool>::const_iterator it=boolMap.cbegin(); it!=boolMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }
  
  for (std::map<TString,int>::const_iterator it=intMap.cbegin(); it!=intMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }

  for (std::map<TString,long>::const_iterator it=longMap.cbegin(); it!=longMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }

  for (std::map<TString,double>::const_iterator it=doubleMap.cbegin(); it!=doubleMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }
  
  for (std::map<TString,TString>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }
}

void Configuration::readFromFile(const TString & inputFileName)
{
  cout << "<I> Configuration::readFromFile() Reading parameter list from file:" <<  inputFileName << endl;
  ifstream inputFile(inputFileName.Data());
  TString key;
  TString type;
  bool    bValue;
  int     iValue;
  long    lValue;
  double  dValue;
  TString sValue;
  bool debug = true;
  while (1)
    {
    inputFile >> key;
    if (inputFile.eof())
      {
      cout << "Reach EOF" << endl;
      break;
      }
    inputFile >> type;
    if (type.Contains("bool") || type.Contains("Bool"))
      {
      inputFile >> bValue;
      addParameter(key,bValue);
      if (debug) cout << key << " : " <<  type <<  " : " <<  bValue << endl;
      }
    else if (type.Contains("int") || type.Contains("Int"))
      {
      inputFile >> iValue;
      addParameter(key,iValue);
      if (debug) cout << key << " : " <<  type <<  " : " <<  iValue << endl;
      }
    else if (type.Contains("long") || type.Contains("Long"))
      {
      inputFile >> lValue;
      addParameter(key,lValue);
      if (debug) cout << key << " : " <<  type <<  " : " <<  lValue << endl;
      }
    else if (type.Contains("double") || type.Contains("Double"))
      {
      inputFile >> dValue;
      addParameter(key,dValue);
      if (debug) cout << key << " : " <<  type <<   " : " <<  dValue << endl;
      }
    else if (type.Contains("string") || type.Contains("String"))
      {
      inputFile >> sValue;
      addParameter(key,sValue);
      if (debug) cout << key << " : " <<  type <<   " : " <<  sValue << endl;
      }
    else
      {
      inputFile >> sValue;
      cout << "Unknown type: " << type << " for keyword: " << key << endl;
      exit(1);
      }
    }
  //cout << "Reading in ParticleType resonance decay table.. - 5- " << endl;
  inputFile.close();
  cout << "Configuration::readFromFile(Configuration & inputFileName) Completed." << endl;
}


void Configuration::writeToFile(const TString & outputFileName)
{
  cout << "<I> Configuration::printToFile() Writing parameter list to file:" <<  outputFileName << endl;
  ofstream outputFile(outputFileName.Data());

  for (std::map<TString,bool>::const_iterator it=boolMap.cbegin(); it!=boolMap.cend(); ++it)
    {
    outputFile <<  it->first << " bool " << it->second << endl;
    }

  for (std::map<TString,int>::const_iterator it=intMap.cbegin(); it!=intMap.cend(); ++it)
    {
    outputFile <<  it->first << " int " << it->second << endl;
    }

  for (std::map<TString,long>::const_iterator it=longMap.cbegin(); it!=longMap.cend(); ++it)
    {
    outputFile <<  it->first << " long " << it->second << endl;
    }

  for (std::map<TString,double>::const_iterator it=doubleMap.cbegin(); it!=doubleMap.cend(); ++it)
    {
    outputFile <<  it->first << " double " << it->second << endl;
    }

  for (std::map<TString,TString>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    outputFile <<  it->first << " string " << it->second << endl;
    }

  //cout << "Reading in ParticleType resonance decay table.. - 5- " << endl;
  outputFile.close();
  cout << "<I> Configuration::printToFile() Completed." << endl;
}


//!
//! Get the value of the parameter named 'name'
//!
bool Configuration::getValueBool(const TString & path, const TString & name)   const
{
  if (hasEndColon(path))
    return getValueBool(path+name);
  else
    return getValueBool(path+TString(":")+name);
}

//!
//! Get the value of the parameter named 'name'
//!
int Configuration::getValueInt(const TString & path, const TString & name)    const
{
  if (hasEndColon(path))
    return getValueInt(path+name);
  else
    return getValueInt(path+TString(":")+name);
}

//!
//! Get the value of the parameter named 'name'
//!
long Configuration::getValueLong(const TString & path, const TString & name)    const
{
  if (hasEndColon(path))
    return getValueLong(path+name);
  else
    return getValueLong(path+TString(":")+name);
}

//!
//! Get the value of the parameter named 'name'
//!
double Configuration::getValueDouble(const TString & path, const TString & name) const
{
  if (hasEndColon(path))
    return getValueDouble(path+name);
  else
    return getValueDouble(path+TString(":")+name);
}

//!
//! Get the value of the parameter named 'name'
//!
TString Configuration::getValueString(const TString & path, const TString & name) const
{
  if (hasEndColon(path))
    return getValueString(path+name);
  else
    return getValueString(path+TString(":")+name);
  }

void Configuration::addParameter(const TString & path, const TString & name, bool value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+TString(":")+name,value);
}

//!
//! Add an int parameter to the configuration with the given name and value
//!
void Configuration::addParameter(const TString & path, const TString & name, int value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+TString(":")+name,value);
}

//!
//! Add an int parameter to the configuration with the given name and value
//!
void Configuration::addParameter(const TString & path, const TString & name, long value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+TString(":")+name,value);
}

//!
//! Add a double parameter to the configuration with the given name and value
//!
void Configuration::addParameter(const TString & path, const TString & name, double value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+TString(":")+name,value);
}

//!
//! Add a string parameter to the configuration with the given name and value
//!
void Configuration::addParameter(const TString & path, const TString & name, const TString & value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+TString(":")+name,value);
}

void Configuration::setParameter(const TString & path, const TString & name, bool value)
{
  if (hasEndColon(path))
    setParameter(path+name,value);
  else
    setParameter(path+TString(":")+name,value);
}

//!
//! Set the parameter named 'name'  to the given value
//!
void Configuration::setParameter(const TString & path, const TString & name, int value)
{
  if (hasEndColon(path))
    setParameter(path+name,value);
  else
    setParameter(path+TString(":")+name,value);
}

//!
//! Set the parameter named 'name'  to the given value
//!
void Configuration::setParameter(const TString & path, const TString & name, long value)
{
  if (hasEndColon(path))
    setParameter(path+name,value);
  else
    setParameter(path+TString(":")+name,value);
}

//!
//! Set the parameter named 'name'  to the given value
//!
void Configuration::setParameter(const TString & path, const TString & name, double value)
{
  if (hasEndColon(path))
    setParameter(path+name,value);
  else
    setParameter(path+TString(":")+name,value);
}

//!
//! Set the parameter named 'name'  to the given value
//!
void Configuration::setParameter(const TString & path, const TString & name, const TString & value)
{
  if (hasEndColon(path))
    setParameter(path+name,value);
  else
    setParameter(path+TString(":")+name,value);
}

bool Configuration::hasEndColon(const TString & path) const
{
  int last = path.Last(':');
  int size = path.Length() - 1;
  return (last>0 && last == size);
}
