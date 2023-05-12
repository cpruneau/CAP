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
using CAP::Configuration;

ClassImp(CAP::Configuration);

CAP::Configuration::Configuration()
:
name(""),
boolMap(),
intMap(),
longMap(),
doubleMap(),
stringMap()
{
  
}

CAP::Configuration::Configuration(const CAP::String & _name)
:
name(_name),
boolMap(),
intMap(),
longMap(),
doubleMap(),
stringMap()
{
  
}

CAP::Configuration::Configuration(const Configuration & _configuration)
:
name(_configuration.name),
boolMap(),
intMap(),
longMap(),
doubleMap(),
stringMap()
{
  addParameters(_configuration);
}


CAP::const Configuration & CAP::Configuration::operator=(const Configuration & _configuration)
{
  if (this!=&_configuration)
    {
    clear();
    name = _configuration.name;
    addParameters(_configuration);
    }
  return *this;
}

void CAP::Configuration::clear()
{
  name = "";
  boolMap.clear();
  intMap.clear();
  longMap.clear();
  doubleMap.clear();
  stringMap.clear();
}

void CAP::Configuration::addParameter(const CAP::String & name, bool value)
{
  std::map<CAP::String,bool>::iterator it;
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

void CAP::Configuration::addParameter(const CAP::String & name, int value)
{
  std::map<CAP::String,int>::iterator it;
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

void CAP::Configuration::addParameter(const CAP::String & name, long value)
{
  std::map<CAP::String,long>::iterator it;
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


void CAP::Configuration::addParameter(const CAP::String & name, double value)
{
  std::map<CAP::String,double>::iterator it;
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

void CAP::Configuration::addParameter(const CAP::String & name, const CAP::String & value)
{
  std::map<CAP::String,CAP::String>::iterator it;
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

void CAP::Configuration::addParameter(const CAP::String & name, bool value)
{
  std::map<CAP::String,bool>::iterator it;
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

void CAP::Configuration::addParameter(const CAP::String & name, int value)
{
  std::map<CAP::String,int>::iterator it;
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

void CAP::Configuration::addParameter(const CAP::String & name, long value)
{
  std::map<CAP::String,long>::iterator it;
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


void CAP::Configuration::addParameter(const CAP::String & name, double value)
{
  std::map<CAP::String,double>::iterator it;
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

void CAP::Configuration::addParameter(const CAP::String & name, const CAP::String & value)
{
  std::map<CAP::String,CAP::String>::iterator it;
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

bool CAP::Configuration::isBool(const CAP::String & name) const
{
  std::map<CAP::String,bool>::const_iterator it;
  it = boolMap.find(name);
  return it != boolMap.cend();
}

bool CAP::Configuration::isInt(const CAP::String & name) const
{
  std::map<CAP::String,int>::const_iterator it;
  it = intMap.find(name);
  return it != intMap.cend();
}

bool CAP::Configuration::isLong(const CAP::String & name) const
{
  std::map<CAP::String,long>::const_iterator it;
  it = longMap.find(name);
  return it != longMap.cend();
}

bool CAP::Configuration::isDouble(const CAP::String & name) const
{
  std::map<CAP::String,double>::const_iterator it;
  it = doubleMap.find(name);
  return it != doubleMap.cend();
}


bool CAP::Configuration::isString(const CAP::String & name) const
{
  std::map<CAP::String,CAP::String>::const_iterator it;
  it = stringMap.find(name);
  return it != stringMap.cend();
}

bool CAP::Configuration::isFound(const CAP::String & name) const
{
  if (isBool(name)) return true;
  if (isInt(name)) return true;
  if (isLong(name)) return true;
  if (isDouble(name)) return true;
  if (isString(name)) return true;
  return false;
}



bool CAP::Configuration::getValueBool(const CAP::String & name) const
{
  bool value = false;
  std::map<CAP::String,bool>::const_iterator it;
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
      std::cout  << " Key:" <<  name << " not a 'bool' but a 'CAP::String'. Use 'getValueString(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> CAP::Configuration::getValueBool(): Did not find key:" << name << std::endl;
    value = false;
    }
  return value;
}

int CAP::Configuration::getValueInt(const CAP::String & name) const
{
  int value = -99999;
  std::map<CAP::String,int>::const_iterator it;
  it = intMap.find(name);
  if (it != intMap.cend())
    {
    //   std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
    if (isBool(name))
      std::cout  << "<W> CAP::Configuration::getValueInt(): Key:" <<  name << " not an 'int' but a 'bool'. Use 'getValueBool(key)' to retrieve it" <<  std::endl;
    else if (isLong(name))
      std::cout  << "<W> CAP::Configuration::getValueInt(): Key:" <<  name << " not an 'int' but a 'long'. Use 'getValueLong(key)' to retrieve it" <<  std::endl;
    else if (isDouble(name))
      std::cout  << "<W> CAP::Configuration::getValueInt(): Key:" <<  name << " not an 'int' but a 'double'. Use 'getValueDouble(key)' to retrieve it" <<  std::endl;
    else if (isString(name))
      std::cout  << "<W> CAP::Configuration::getValueInt(): Key:" <<  name << " not an 'int' but a 'CAP::String'. Use 'getValueString(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> CAP::Configuration::getValueInt(): Did not find key:" << name << std::endl;
    }
  return value;
}

long CAP::Configuration::getValueLong(const CAP::String & name) const
{
  long value = -99999;
  std::map<CAP::String,long>::const_iterator it;
  it = longMap.find(name);
  if (it != longMap.cend())
    {
    //   std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
    if (isBool(name))
      std::cout  << "<W> CAP::Configuration::getValueLong(): Key:" <<  name << " not a 'long' but a 'bool'. Use 'getValueBool(key)' to retrieve it" <<  std::endl;
    else if (isInt(name))
      std::cout  << "<W> CAP::Configuration::getValueLong(): Key:" <<  name << " not a 'long' but an 'int'. Use 'getValueInt(key)' to retrieve it" <<  std::endl;
    else if (isDouble(name))
      std::cout  << "<W> CAP::Configuration::getValueLong(): Key:" <<  name << " not a 'long' but a 'double'. Use 'getValueDouble(key)' to retrieve it" <<  std::endl;
    else if (isString(name))
      std::cout  << "<W> CAP::Configuration::getValueLong(): Key:" <<  name << " not a 'long' but a 'CAP::String'. Use 'getValueString(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> CAP::Configuration::getValueLong(): Did not find key:" << name << std::endl;
    }
  return value;
}


double CAP::Configuration::getValueDouble(const CAP::String & name)  const
{
  double value = 0.0;
  std::map<CAP::String,double>::const_iterator it;
  it = doubleMap.find(name);
  if (it != doubleMap.cend())
    {
    //std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
    if (isBool(name))
      std::cout  << "<W> CAP::Configuration::getValueDouble(): Key:" <<  name << " not a 'double' but a 'bool'. Use 'getValueBool(key)' to retrieve it" <<  std::endl;
    else if (isInt(name))
      std::cout  << "<W> CAP::Configuration::getValueDouble(): Key:" <<  name << " not a 'double' but an 'int'. Use 'getValueInt(key)' to retrieve it" <<  std::endl;
    else if (isLong(name))
      std::cout  << "<W> CAP::Configuration::getValueDouble(): Key:" <<  name << " not a 'double' but a 'long'. Use 'getValueLong(key)' to retrieve it" <<  std::endl;
    else if (isString(name))
      std::cout  << "<W> CAP::Configuration::getValueDouble(): Key:" <<  name << " not a 'double' but a 'CAP::String'. Use 'getValueString(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> CAP::Configuration::getValueDouble(): Did not find key:" << name << std::endl;
    }
  return value;
}

CAP::String  CAP::Configuration::getValueString(const CAP::String & name) const
{
  CAP::String value = "notFound";
  std::map<CAP::String,CAP::String>::const_iterator it;
  it = stringMap.find(name);
  if (it != stringMap.cend())
    {
    //std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
    if (isBool(name))
      std::cout  << "<W> CAP::Configuration::getValueString(): Key:" <<  name << " not a 'CAP::String' but a 'bool'. Use 'getValueBool(key)' to retrieve it" <<  std::endl;
    else if (isInt(name))
      std::cout  << "<W> CAP::Configuration::getValueString(): Key:" <<  name << " not a 'CAP::String' but an 'int'. Use 'getValueInt(key)' to retrieve it" <<  std::endl;
    else if (isLong(name))
      std::cout  << "<W> CAP::Configuration::getValueString(): Key:" <<  name << " not a 'CAP::String' but a 'long'. Use 'getValueLong(key)' to retrieve it" <<  std::endl;
    else if (isDouble(name))
      std::cout  << "<W> CAP::Configuration::getValueString(): Key:" <<  name << " not a 'CAP::String' but a 'double'. Use 'getValueDouble(key)' to retrieve it" <<  std::endl;
    else
      std::cout  << "<W> CAP::Configuration::getValueString(): Did not find key:" << name << std::endl;
    }
  return value;
}

void CAP::Configuration::addParameters(const Configuration &  configuration)
{
  //cout << "CAP::Configuration::addParameters(const Configuration &  configuration)" << endl;
  //cout << " Size bool: "  << configuration.boolMap.size() << endl;
  for (std::map<CAP::String,bool>::const_iterator it=configuration.boolMap.cbegin(); it!=configuration.boolMap.cend(); ++it)
    {
    addParameter(it->first, it->second);
    }
  //cout << " Size int: "  << configuration.intMap.size() << endl;

  for (std::map<CAP::String,int>::const_iterator it=configuration.intMap.cbegin(); it!=configuration.intMap.cend(); ++it)
    {
    addParameter(it->first, it->second);
    }
  //cout << " Size long: "  << configuration.longMap.size() << endl;

  for (std::map<CAP::String,long>::const_iterator it=configuration.longMap.cbegin(); it!=configuration.longMap.cend(); ++it)
    {
    addParameter(it->first, it->second);
    }
  //cout << " Size double: "  << configuration.doubleMap.size() << endl;

  for (std::map<CAP::String,double>::const_iterator it=configuration.doubleMap.cbegin(); it!=configuration.doubleMap.cend(); ++it)
    {
    addParameter(it->first, it->second);
    }
  //cout << " Size CAP::String: "  << configuration.stringMap.size() << endl;

  for (std::map<CAP::String,CAP::String>::const_iterator it=configuration.stringMap.cbegin(); it!=configuration.stringMap.cend(); ++it)
    {
//    cout << "WTF  " << k++ << endl;
//    cout << "  first:" << it->first << endl;
//    cout << " second:" << it->second << endl;
    addParameter(it->first, it->second);
    }
  //cout << "CAP::Configuration::addParameters(const Configuration &  configuration) complted OK" << endl;
}

int CAP::Configuration::getNParameters()
{
  int nParameters = 0;
  nParameters += boolMap.size();
  nParameters += intMap.size();
  nParameters += longMap.size();
  nParameters += doubleMap.size();
  nParameters += stringMap.size();
  return nParameters;
}


void CAP::Configuration::generateKeyValuePairs(const CAP::String & keyBaseName, const CAP::String & defaultValue, int nKeysToGenerate)
{
  CAP::String key;
  for (int k=0; k<nKeysToGenerate; k++)
    {
    key = keyBaseName; key += k;
    addParameter(key,defaultValue);
    }
}

void CAP::Configuration::generateKeyValuePairs(const CAP::String & path, const CAP::String & keyBaseName, const CAP::String & defaultValue, int nKeysToGenerate)
{
  if (path.EndsWith(":"))
    generateKeyValuePairs(path+keyBaseName, defaultValue, nKeysToGenerate);
  else
    generateKeyValuePairs(path+CAP::String(":")+keyBaseName, defaultValue, nKeysToGenerate);
}

vector<CAP::String> CAP::Configuration::getSelectedValues(const CAP::String & keyBaseName, const CAP::String & defaultValue) const
{
  vector<CAP::String> selectedValues;
  for (std::map<CAP::String,CAP::String>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName) && !it->second.Contains(defaultValue) ) selectedValues.push_back(it->second);
    }
  return selectedValues;
}

vector<CAP::String> CAP::Configuration::getSelectedValues(const CAP::String & path, const CAP::String & keyBaseName, const CAP::String & defaultValue) const
{
  if (path.EndsWith(":"))
    return getSelectedValues(path+keyBaseName, defaultValue);
  else
    return getSelectedValues(path+CAP::String(":")+keyBaseName, defaultValue);
}


int CAP::Configuration::getNPossibleValues(const CAP::String & keyBaseName)  const
{
  int nPossible = 0;
  for (std::map<CAP::String,CAP::String>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName)) nPossible++;
    }
  return nPossible;
}

int CAP::Configuration::getNSelectedValues(const CAP::String & keyBaseName, const CAP::String & defaultValue)  const
{
  int nSelected = 0;
  for (std::map<CAP::String,CAP::String>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName) && !it->second.Contains(defaultValue) ) nSelected++;
    }
  return nSelected;
}


void CAP::Configuration::addSelectedValues(const CAP::String & keyBaseName, const CAP::String & defaultValue, const vector<CAP::String> & selectedValues)
{
  CAP::String key;
  int nSelected = getNSelectedValues(keyBaseName,defaultValue);
  for (unsigned int k=0; k<selectedValues.size(); k++)
    {
      CAP::String key = keyBaseName;
      int index = nSelected + k;
      key += index;
    addParameter(key,selectedValues[k]);
    }
}

void CAP::Configuration::printConfiguration(ostream & os)
{
  os
  << endl
  << " ------------------------------------------------------------" << std::endl
  << " name: " << name << std::endl
  << " ------------------------------------------------------------" << std::endl;
  
  for (std::map<CAP::String,bool>::const_iterator it=boolMap.cbegin(); it!=boolMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }
  
  for (std::map<CAP::String,int>::const_iterator it=intMap.cbegin(); it!=intMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }

  for (std::map<CAP::String,long>::const_iterator it=longMap.cbegin(); it!=longMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }

  for (std::map<CAP::String,double>::const_iterator it=doubleMap.cbegin(); it!=doubleMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }
  
  for (std::map<CAP::String,CAP::String>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    std::cout << "     " << it->first << " == " << it->second << endl;
    }
}

void CAP::Configuration::readFromFile(const CAP::String & inputFileName)
{
  cout << "<I> CAP::Configuration::readFromFile() Reading parameter list from file:" <<  inputFileName << endl;
  ifstream inputFile(inputFileName.Data());
  CAP::String key;
  CAP::String type;
  bool    bValue;
  int     iValue;
  long    lValue;
  double  dValue;
  CAP::String sValue;
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
      if (debug) cout << key , type <<  " : " <<  bValue << endl;
      }
    else if (type.Contains("int") || type.Contains("Int"))
      {
      inputFile >> iValue;
      addParameter(key,iValue);
      if (debug) cout << key , type <<  " : " <<  iValue << endl;
      }
    else if (type.Contains("long") || type.Contains("Long"))
      {
      inputFile >> lValue;
      addParameter(key,lValue);
      if (debug) cout << key , type <<  " : " <<  lValue << endl;
      }
    else if (type.Contains("double") || type.Contains("Double"))
      {
      inputFile >> dValue;
      addParameter(key,dValue);
      if (debug) cout << key , type <<   " : " <<  dValue << endl;
      }
    else if (type.Contains("CAP::String") || type.Contains("CAP::String"))
      {
      inputFile >> sValue;
      addParameter(key,sValue);
      if (debug) cout << key , type <<   " : " <<  sValue << endl;
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
  cout << "CAP::Configuration::readFromFile(const Configuration & inputFileName) Completed." << endl;
}


void CAP::Configuration::writeToFile(const CAP::String & outputFileName)
{
  cout << "<I> CAP::Configuration::printToFile() Writing parameter list to file:" <<  outputFileName << endl;
  ofstream outputFile(outputFileName.Data());

  for (std::map<CAP::String,bool>::const_iterator it=boolMap.cbegin(); it!=boolMap.cend(); ++it)
    {
    outputFile <<  it->first << " bool " << it->second << endl;
    }

  for (std::map<CAP::String,int>::const_iterator it=intMap.cbegin(); it!=intMap.cend(); ++it)
    {
    outputFile <<  it->first << " int " << it->second << endl;
    }

  for (std::map<CAP::String,long>::const_iterator it=longMap.cbegin(); it!=longMap.cend(); ++it)
    {
    outputFile <<  it->first << " long " << it->second << endl;
    }

  for (std::map<CAP::String,double>::const_iterator it=doubleMap.cbegin(); it!=doubleMap.cend(); ++it)
    {
    outputFile <<  it->first << " double " << it->second << endl;
    }

  for (std::map<CAP::String,CAP::String>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    outputFile <<  it->first << " CAP::String " << it->second << endl;
    }

  //cout << "Reading in ParticleType resonance decay table.. - 5- " << endl;
  outputFile.close();
  cout << "<I> CAP::Configuration::printToFile() Completed." << endl;
}


//!
//! Get the value of the parameter named 'name'
//!
bool CAP::Configuration::getValueBool(const CAP::String & path, const CAP::String & name)   const
{
  if (hasEndColon(path))
    return getValueBool(path+name);
  else
    return getValueBool(path+CAP::String(":")+name);
}

//!
//! Get the value of the parameter named 'name'
//!
int CAP::Configuration::getValueInt(const CAP::String & path, const CAP::String & name)    const
{
  if (hasEndColon(path))
    return getValueInt(path+name);
  else
    return getValueInt(path+CAP::String(":")+name);
}

//!
//! Get the value of the parameter named 'name'
//!
long CAP::Configuration::getValueLong(const CAP::String & path, const CAP::String & name)    const
{
  if (hasEndColon(path))
    return getValueLong(path+name);
  else
    return getValueLong(path+CAP::String(":")+name);
}

//!
//! Get the value of the parameter named 'name'
//!
double CAP::Configuration::getValueDouble(const CAP::String & path, const CAP::String & name) const
{
  if (hasEndColon(path))
    return getValueDouble(path+name);
  else
    return getValueDouble(path+CAP::String(":")+name);
}

//!
//! Get the value of the parameter named 'name'
//!
CAP::String CAP::Configuration::getValueString(const CAP::String & path, const CAP::String & name) const
{
  if (hasEndColon(path))
    return getValueString(path+name);
  else
    return getValueString(path+CAP::String(":")+name);
  }

void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, bool value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

//!
//! Add an int parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, int value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

//!
//! Add an int parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, long value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

//!
//! Add a double parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, double value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

//!
//! Add a CAP::String parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, const CAP::String & value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, bool value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

//!
//! Set the parameter named 'name'  to the given value
//!
void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, int value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

//!
//! Set the parameter named 'name'  to the given value
//!
void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, long value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

//!
//! Set the parameter named 'name'  to the given value
//!
void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, double value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

//!
//! Set the parameter named 'name'  to the given value
//!
void CAP::Configuration::addParameter(const CAP::String & path, const CAP::String & name, const CAP::String & value)
{
  if (hasEndColon(path))
    addParameter(path+name,value);
  else
    addParameter(path+CAP::String(":")+name,value);
}

bool CAP::Configuration::hasEndColon(const CAP::String & path) const
{
  int last = path.Last(':');
  int size = path.Length() - 1;
  return (last>0 && last == size);
}
