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
#include "MessageLogger.hpp"
using std::ostream;
using std::endl;
using std::vector;
using namespace std;

namespace CAP
{

class Configuration
{
  //friend TextParser;

  struct Parameter
  {
  String keyword;
  String value;
  };

public:
  Configuration();
  Configuration(const Configuration & _configuration);
  virtual ~Configuration();
  Configuration & operator=(const Configuration & _configuration);

  String  getParameter(const char* aKeyword)  const;
  int     getNParameters() const;
  bool    getValueBool(const char* aKeyword) const;
  int     getValueInt(const char* aKeyword) const;
  long    getValueLong(const char* aKeyword) const;
  double  getValueDouble(const char* aKeyword) const;
  String  getValueString(const char* aKeyword) const;


  String  standardize(   const char * path, const char* aKeyword) const;
  bool    getValueBool(  const char * path, const char* aKeyword) const;
  int     getValueInt (  const char * path, const char* aKeyword) const;
  long    getValueLong(  const char * path, const char* aKeyword) const;
  double  getValueDouble(const char * path, const char* aKeyword) const;
  String  getValueString(const char * path, const char* aKeyword) const;


  void addParameters(const Configuration & configuration);
  void addParameter(Parameter& aPar);
  void addParameter(const char * name, bool value);
  void addParameter(const char * name, int value);
  void addParameter(const char * name, long value);
  void addParameter(const char * name, double value);
  void addParameter(const char * name, const char * value);
  void addParameter(const char * name, const String & value);
  void addParameter(const char * path, const char * name, bool value);
  void addParameter(const char * path, const char * name, int value);
  void addParameter(const char * path, const char * name, long value);
  void addParameter(const char * path, const char * name, double value);
  void addParameter(const char * path, const char * name, const char * value);
  void addParameter(const char * path, const char * name, const String & value);
  bool hasEndColon(const char * path) const;
  void clear();
  bool isBool(const char * name) const;
  bool isInt(const char * name) const;
  bool isLong(const char * name) const;
  bool isDouble(const char * name) const;
  bool isString(const char * name) const;
  bool isFound(const char * name) const;
  void readFromFile(const char * _inputPath,
                    const char * _inputFileName,
                    const char * _extension=".ini",
                    MessageLogger::Severity severity=MessageLogger::Info);
  void writeToFile(const char * outputFileName);
  int  printConfiguration(ostream & output);

  void sanityCheck(const char * name);


  //!
  //!Generates and stores in the configuration of this task a list of key,value parameters based on the given parameters.value
  //!The keys generated have the form keyBaseName## where keyBaseName is the base name of the keys and ## is an integer k
  //!from 0 to nKeysToGenerate-1 (inclusively)
  //!
  //!@param keyBaseName base name of the keys
  //!@param defaultValue default value given for all key,value pairs
  //!@param nKeysToGenerate number of key,value pairs to generate and add to this task configuration.
  //!
  void generateKeyValuePairs(const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate);

  void generateKeyValuePairs(const char *  path, const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate);

  //!
  //!Parse the configuration associated with task to find the key,value pairs that DO NOT feature the defaultValue
  //!
  //!@param keyBaseName base name of the keys
  //!@param defaultValue default value that is selected against
  //!
  VectorString  getSelectedValues(const char *  keyBaseName, const char *  defaultValue) const;

  VectorString  getSelectedValues(const char *  path, const char *  keyBaseName, const char *  defaultValue) const;

  //!
  //!Get  the number of key,value pairs with the given key base name  that DO NOT feature the defaultValue
  //!
  //!@param keyBaseName base name of the keys to be tested against.
  //!@param defaultValue default value that is selected against
  //!
  int getNSelectedValues(const char *  keyBaseName, const char *  defaultValue)  const;

  int getNSelectedValues(const char *  path, const char *  keyBaseName, const char *  defaultValue) const
  {
  String sPath = path;
  String sKeyBaseName = keyBaseName;
  return getNSelectedValues(sPath+TString(":")+sKeyBaseName, defaultValue);
  }

  int getNPossibleValues(const char *  keyBaseName) const;

  int getNPossibleValues(const char *  path, const char *  keyBaseName) const;

  //!
  //!Add key,value pairs with the given key base name  and values provided in the selectedValues array.
  //!
  //!@param keyBaseName base name of the keys to be used
  //!@param defaultValue default value that is selected against
  //!@param selectedValues  number of key,value pairs to be checked
  //!
  void addSelectedValues(const char *  keyBaseName, const char *  defaultValue, const VectorString  & selectedValues);

  void addSelectedValues(const char * path, const char * keyBaseName, const char * defaultValue, const VectorString  & selectedValues)
  {
  String sPath = path;
  String sKeyBaseName = keyBaseName;
  addSelectedValues(sPath+TString(":")+sKeyBaseName, defaultValue, selectedValues);
  }

protected:
  std::vector<Parameter> parameters;

  ClassDef(Configuration,0)
  
};

} // namespace CAP

#endif /* Configuration_hpp */
