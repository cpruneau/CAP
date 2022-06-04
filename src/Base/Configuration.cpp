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

Configuration::Configuration(const Configuration & configuration)
:
name(configuration.name),
boolMap(configuration.boolMap),
intMap(configuration.intMap),
longMap(configuration.longMap),
doubleMap(configuration.doubleMap),
stringMap(configuration.stringMap)
{
  
}


Configuration & Configuration::operator=(const Configuration & configuration)
{
  if (this!=&configuration)
    {
    name      = configuration.name;
    boolMap   = configuration.boolMap;
    intMap    = configuration.intMap;
    longMap   = configuration.longMap;
    doubleMap = configuration.doubleMap;
    stringMap = configuration.stringMap;
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

void Configuration::addParameter(const TString & name, const TString &  value)
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

void Configuration::setParameter(const TString & name, const TString &  value)
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


bool Configuration::getValueBool(const TString & name) const
{
  bool value = false;
  std::map<TString,bool>::const_iterator it;
  it = boolMap.find(name);
  if (it != boolMap.cend())
    {
    //std::cout  << " Key:" <<  name << " value:" <<  it->second << std::endl;
    value = it->second;
    }
  else
    {
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
    std::cout  << "<W> Configuration::getValueInt(): Did not find key:" << name << std::endl;
    }
  return value;
}

int Configuration::getValueLong(const TString & name) const
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
    std::cout  << "<W> Configuration::getValueString(): Did not find key:" << name << std::endl;
    }
  return value;
}

void Configuration::setParameters(const Configuration &  configuration)
{
  for (std::map<TString,bool>::const_iterator it=configuration.boolMap.cbegin(); it!=configuration.boolMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
  
  for (std::map<TString,int>::const_iterator it=configuration.intMap.cbegin(); it!=configuration.intMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
  
  for (std::map<TString,long>::const_iterator it=configuration.longMap.cbegin(); it!=configuration.longMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
  
  for (std::map<TString,double>::const_iterator it=configuration.doubleMap.cbegin(); it!=configuration.doubleMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
  
  for (std::map<TString,TString>::const_iterator it=configuration.stringMap.cbegin(); it!=configuration.stringMap.cend(); ++it)
    {
    setParameter(it->first, it->second);
    }
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


void Configuration::generateKeyValuePairs(const TString keyBaseName, const TString defaultValue, int nKeysToGenerate)
{
  TString key;
  for (int k=0; k<nKeysToGenerate; k++)
    {
    key = keyBaseName; key += k;
    addParameter(key,defaultValue);
    }
}

vector<TString> Configuration::getSelectedValues(const TString keyBaseName, const TString defaultValue) const 
{
  vector<TString> selectedValues;
  for (std::map<TString,TString>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName) && !it->second.Contains(defaultValue) ) selectedValues.push_back(it->second);
    }
  return selectedValues;
}

int Configuration::getNPossibleValues(const TString keyBaseName)  const
{
  int nPossible = 0;
  for (std::map<TString,TString>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName)) nPossible++;
    }
  return nPossible;
}

int Configuration::getNSelectedValues(const TString keyBaseName, const TString defaultValue)  const
{
  int nSelected = 0;
  for (std::map<TString,TString>::const_iterator it=stringMap.cbegin(); it!=stringMap.cend(); ++it)
    {
    if (it->first.Contains(keyBaseName) && !it->second.Contains(defaultValue) ) nSelected++;
    }
  return nSelected;
}


void Configuration::addSelectedValues(const TString keyBaseName, const TString defaultValue, const vector<TString> & selectedValues)
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

