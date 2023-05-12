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
#include "TextParser.hpp"
//using CAP::Configuration;
using CAP::TextParser;

ClassImp(CAP::Configuration);

CAP::Configuration::Configuration()
{
  parameters.clear();
}

CAP::Configuration::Configuration(const CAP::Configuration & _configuration)
{
  parameters = _configuration.parameters;
}

CAP::Configuration & CAP::Configuration::operator=(const CAP::Configuration & _configuration)
{
  if (this!= &_configuration)
    {
    parameters = _configuration.parameters;
    }
  return *this;
}


CAP::Configuration::~Configuration()
{
  parameters.clear();
}


CAP::String CAP::Configuration::getParameter(const char* aKeyword)  const
{
  vector<Parameter>::const_iterator iter;
  for (iter = parameters.begin(); iter != parameters.end(); iter++)
    {
    if (iter->keyword.Contains(aKeyword))
      {
      return iter->value;
      }
    }
  throw *(new CAP::String(aKeyword));
}

CAP::String  CAP::Configuration::standardize(const char * path, const char* aKeyword) const
{
  CAP::String keyword = path;
  if (keyword.EndsWith(":")) keyword += aKeyword;
  else
    {
    keyword += ":";
    keyword += aKeyword;
    }
  return keyword;
}


bool    CAP::Configuration::getValueBool(  const char * path, const char* aKeyword) const
{
  return getValueBool(standardize(path,aKeyword));
}

int     CAP::Configuration::getValueInt (  const char * path, const char* aKeyword) const
{
  return getValueInt(standardize(path,aKeyword));
}

long    CAP::Configuration::getValueLong(  const char * path, const char* aKeyword) const
{
  return getValueLong(standardize(path,aKeyword));
}

double  CAP::Configuration::getValueDouble(const char * path, const char* aKeyword) const
{
  return getValueDouble(standardize(path,aKeyword));
}

CAP::String  CAP::Configuration::getValueString(const char * path, const char* aKeyword) const
{
  return getValueString(standardize(path,aKeyword));
}

bool  CAP::Configuration::getValueBool(const char* aKeyword) const
{
  try
  {
  CAP::String v = getParameter(aKeyword);
  v.ToUpper();
  //cout << " CAP::Configuration::getValueBool(const char* aKeyword) for keyword: " << aKeyword << " finds:" << v << endl;
  if (v.EqualTo("0") || v.EqualTo("FALSE") || v.EqualTo("YES") ) return false;
  else if (v.EqualTo("1") || v.EqualTo("TRUE")  || v.EqualTo("NO")  ) return true;
  else if (v.IsDec())
    {
    int vv = v.Atoi();
    return vv>0;
    }
  return false;
  }
  catch (CAP::String s)
  {
  cout << "Exception for keyword:" << s << endl;
  return false;
  }
}


int CAP::Configuration::getValueInt(const char* aKeyword) const
{
  try
  {
  CAP::String v = getParameter(aKeyword);
  if (!v.IsDec())
    {
    return -99999;
    }
  return v.Atoi();
  }
  catch (CAP::String s)
  {
  cout << "Exception for keyword:" << s << endl;
  return -99999;
  }
}

long CAP::Configuration::getValueLong(const char* aKeyword) const
{
  try
  {
  CAP::String v = getParameter(aKeyword);
  if (!v.IsDec())
    {
    return -99999;
    }
  return v.Atoll();
  }
  catch (CAP::String s)
  {
  cout << "Exception for keyword:" << s << endl;
  return -99999;
  }
}

double CAP::Configuration::getValueDouble (const char* aKeyword) const
{
  try
  {
  CAP::String v = getParameter(aKeyword);
  if (!v.IsFloat())
    {
    return -1.0E100;
    }
  return v.Atof();
  }
  catch (CAP::String s)
  {
  cout << "Exception for keyword:" << s << endl;
  return -1.0E100;
  }
}


CAP::String CAP::Configuration::getValueString (const char* aKeyword) const
{
  try
  {
  CAP::String v = getParameter(aKeyword);
  //cout << " CAP::Configuration::getValueString(const char* aKeyword) for keyword: " << aKeyword << " finds:" << v << endl;
  return v;
  }
  catch (CAP::String s)
  {
  cout << "Exception for keyword:" << s << endl;
  return CAP::String("");
  }
}


int CAP::Configuration::getNParameters() const
{
  return parameters.size();
}

void CAP::Configuration::addParameters(const Configuration & source)
{
  vector<Parameter>::const_iterator iter;
  //cout << "======== Initial: " << parameters.size() << endl;
  //cout << "======== Adding: " << source.getNParameters() << endl;
  for (iter = source.parameters.begin(); iter != source.parameters.end(); iter++)
    {
    //cout << "======== Adding parameter.keyword = " << iter->keyword << " value   = " << iter->value << endl;
    Parameter  parameter;
    parameter.keyword = iter->keyword;
    parameter.value   = iter->value;
    addParameter(parameter);
    int index = -1 + parameters.size();
    //cout << "======== Added parameter.keyword = " << parameters[index].keyword << " value   = " << parameters[index].value << endl;
    }
  //cout << "======== Final:  " << parameters.size() << endl;
}


void CAP::Configuration::addParameter(Parameter& parameter)
{
  vector<Parameter>::iterator iter;

  bool stop = false;

//  if (parameter.keyword.Contains("EventsAnalyze"))
//    {
//    cout << "Searching for EventsAnalyze " << endl;
//    stop = true;
//    }
  String keySearched = parameter.keyword;
  //cout << "======== CAP::Configuration::addParameter(" << keySearched << ")" << endl;
  for (iter = parameters.begin(); iter != parameters.end(); iter++)
    {
    String keyCompared = iter->keyword;
    if (keyCompared.Contains(keySearched.Data()))
      {
        iter->value = parameter.value;
        return;
      }
    }
  parameters.push_back(parameter);
}

void CAP::Configuration::addParameter(const char * name, bool value)
{
  Parameter p;
  p.keyword = name;
  p.value   = "";
  p.value   += value;
  addParameter(p);
}

//!
//! Add an int parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * name, int value)
{
  Parameter p;
  p.keyword = name;
  p.value   = "";
  p.value   += value;
  addParameter(p);
}


//!
//! Add an int parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * name, long value)
{
  Parameter p;
  p.keyword = name;
  p.value   = "";
  p.value   += value;
  addParameter(p);
}

//!
//! Add a double parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * name, double value)
{
  Parameter p;
  p.keyword = name;
  p.value   = "";
  p.value   += value;
  addParameter(p);
}

//!
//! Add a string parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * keyword, const char * value)
{
  Parameter p;
  p.keyword = keyword;
  p.value   = value;
  addParameter(p);
}

//!
//! Add a string parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * keyword, const String & value)
{
  Parameter p;
  p.keyword = keyword;
  p.value   = value;
  addParameter(p);
}

//!
//! Add a bool parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * path, const char * keyword, bool value)
{
  addParameter(standardize(path,keyword),value);
}

//!
//! Add an int parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * path, const char * keyword, int value)
{
  addParameter(standardize(path,keyword),value);
}

//!
//! Add an int parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * path, const char * keyword, long value)
{
  addParameter(standardize(path,keyword),value);
}

//!
//! Add a double parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * path, const char * keyword, double value)
{
  addParameter(standardize(path,keyword),value);
}

//!
//! Add a const char *  parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * path, const char * keyword, const char * value)
{
  addParameter(standardize(path,keyword),value);
}

//!
//! Add a string parameter to the configuration with the given name and value
//!
void CAP::Configuration::addParameter(const char * path, const char * keyword, const String & value)
{
  addParameter(standardize(path,keyword),value);
}



bool CAP::Configuration::hasEndColon(const char * path) const
{
  CAP::String s = path;
  int last = s.Last(':');
  int size = s.Length() - 1;
  return (last>0 && last == size);
}

int CAP::Configuration::printConfiguration(ostream & output)
{
  vector<Parameter>::iterator iter;
  for (iter = parameters.begin(); iter != parameters.end(); iter++)
    {
    output <<  left << setw(50) << setfill('.')<< iter->keyword << ": " << iter->value<< endl;
    }
  output << setfill(' ')<< endl;
//    output << "\tKeyword: " << iter->keyword << "\tValue: " << iter->value<< endl;
  return 0;
}

bool CAP::Configuration::isBool(const char * keyword) const
{
  bool found = isFound(keyword);
  if (found)
    {
    CAP::String v = getParameter(keyword);
    v.ToUpper();
    if (v.EqualTo("0")    || v.EqualTo("1") )     return true;
    if (v.EqualTo("TRUE") || v.EqualTo("FALSE") ) return true;
    if (v.EqualTo("YES")  || v.EqualTo("NO") )    return true;
    return false;
    }
  else
    return false;
}


bool CAP::Configuration::isInt(const char * keyword) const
{
  try
  {
  CAP::String v = getParameter(keyword);
  if (!v.IsDec())
    {
    return false;
    }
  return true;
  }
  catch (...)
  {
  return false;
  }
}


bool CAP::Configuration::isLong(const char * keyword) const
{
  try
  {
  CAP::String v = getParameter(keyword);
  if (!v.IsDec())
    {
    return false;
    }
  return true;
  }
  catch (...)
  {
  return false;
  }
}

bool CAP::Configuration::isDouble(const char * keyword) const
{
  try
  {
  CAP::String v = getParameter(keyword);
  if (v.IsFloat()) return true;
  return false;
  }
  catch (...)
  {
  return false;
  }
}

bool CAP::Configuration::isString(const char * keyword) const
{
  try
  {
  CAP::String v = getParameter(keyword);
  return true;
  }
  catch (...)
  {
  return false;
  }
}

bool CAP::Configuration::isFound(const char * keyword) const
{
  vector<Parameter>::const_iterator iter;
  for (iter = parameters.begin(); iter != parameters.end(); iter++)
    if (iter->keyword == keyword)
      {
      //cout << "<CAP::Configuration::getParameter>\tReturning value " << Iter->value << " for keyword " << Iter->keyword << endl;
      return true;
      }
  return false;
}



void CAP::Configuration::generateKeyValuePairs(const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate)
{
  CAP::String key;
  for (int k=0; k<nKeysToGenerate; k++)
    {
    key = keyBaseName; key += k;
    addParameter(key,defaultValue);
    }
}

void CAP::Configuration::generateKeyValuePairs(const char *  path, const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate)
{
  CAP::String sPath = path;
  CAP::String sKeyBaseName = keyBaseName;
  if (sPath.EndsWith(":"))
    generateKeyValuePairs(sPath+sKeyBaseName, defaultValue, nKeysToGenerate);
  else
    generateKeyValuePairs(sPath+CAP::String(":")+sKeyBaseName, defaultValue, nKeysToGenerate);
}

vector<CAP::String> CAP::Configuration::getSelectedValues(const char *  keyBaseName, const char *  defaultValue) const
{
  CAP::String sKeyBaseName = keyBaseName;
  vector<CAP::String> selectedValues;
  vector<Parameter>::const_iterator iter;
  for (iter = parameters.begin(); iter != parameters.end(); iter++)
    {
    if (iter->keyword.Contains(keyBaseName) && !iter->value.Contains(defaultValue) ) selectedValues.push_back(iter->value);
    }
  return selectedValues;
}

vector<CAP::String> CAP::Configuration::getSelectedValues(const char *  path, const char *  keyBaseName, const char *  defaultValue) const
{
  CAP::String sPath = path;
  CAP::String sKeyBaseName = keyBaseName;
  if (sPath.EndsWith(":"))
    return getSelectedValues(sPath+sKeyBaseName, defaultValue);
  else
    return getSelectedValues(sPath+CAP::String(":")+sKeyBaseName, defaultValue);
}


int CAP::Configuration::getNPossibleValues(const char *  keyBaseName)  const
{
  int nPossible = 0;
  CAP::String sKeyBaseName = keyBaseName;
  vector<Parameter>::const_iterator iter;
  for (iter = parameters.begin(); iter != parameters.end(); iter++)
    {
    if (iter->keyword.Contains(sKeyBaseName) ) nPossible++;
    }
  return nPossible;
}

int CAP::Configuration::getNPossibleValues(const char *  path, const char *  keyBaseName) const
{
  CAP::String sPath = path;
  CAP::String sKeyBaseName = keyBaseName;
  if (sPath.EndsWith(":"))
    return getNPossibleValues(sPath+sKeyBaseName);
  else
    return getNPossibleValues(sPath+CAP::String(":")+sKeyBaseName);
}


int CAP::Configuration::getNSelectedValues(const char *  keyBaseName, const char *  defaultValue)  const
{
  int nSelected = 0;
  CAP::String sKeyBaseName = keyBaseName;
  vector<Parameter>::const_iterator iter;
  for (iter = parameters.begin(); iter != parameters.end(); iter++)
    {
    if (iter->keyword.Contains(sKeyBaseName) && !iter->value.Contains(defaultValue) ) nSelected++;
    }
  return nSelected;
}


void CAP::Configuration::addSelectedValues(const char *  keyBaseName, const char *  defaultValue, const vector<CAP::String> & selectedValues)
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

void CAP::Configuration::clear()
{
  parameters.clear();
}

void CAP::Configuration::readFromFile(const char * _inputPath,
                                      const char * _inputFileName,
                                      const char * _extension,
                                      MessageLogger::Severity severity)
{
  TextParser parser(severity);
  parser.read(*this,_inputPath,_inputFileName,_extension);
}

void CAP::Configuration::writeToFile(const char * outputFileName __attribute__((unused)))
{
// to be implemented....
}

void CAP::Configuration::sanityCheck(const char * keyword)
{
  int count = 0;
  vector<Parameter>::const_iterator iter;
  for (iter = parameters.begin(); iter != parameters.end(); iter++)
    {
    if (iter->keyword.Contains(keyword))
      {
      cout << "sanityCheck("<< keyword<< ") found " << iter->keyword << " with value:" << iter->value << endl;
      count++;
      }
    }
  cout << "sanityCheck("<< keyword<< ") COUNT=" << count << endl;
}
