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
#include "ConfigurationManager.hpp"
using CAP::ConfigurationManager;


ClassImp(CAP::ConfigurationManager);

CAP::ConfigurationManager::ConfigurationManager()
:
MessageLogger(Info),
configured(false),
configuration(),
requestedConfiguration(nullptr)
{
  setClassName("ConfigurationManager");
  setInstanceName("ConfigurationManager");
}

CAP::ConfigurationManager::ConfigurationManager(const Configuration & requestedConfiguration)
:
MessageLogger(Info),
configured(false),
configuration(),
requestedConfiguration(& requestedConfiguration)
{
  setClassName("ConfigurationManager");
  setInstanceName("ConfigurationManager");
}

CAP::ConfigurationManager::~ConfigurationManager()
{
  setConfigured(false);
}

const CAP::Configuration & CAP::ConfigurationManager::getConfiguration() const
{
  return configuration;
}

const CAP::Configuration & CAP::ConfigurationManager::getConfiguration()
{
  return configuration;
}


void CAP::ConfigurationManager::setDefaultConfiguration()
{
  addParameter( "Severity","Info");
}

void CAP::ConfigurationManager::setConfiguration(const  Configuration & _configuration)
{
  //configuration.sanityCheck("EventsAnalyze");
  configuration.addParameters(_configuration);
  //configuration.sanityCheck("EventsAnalyze");
}

void CAP::ConfigurationManager::configure()
{
  setDefaultConfiguration();
  setConfiguration(*requestedConfiguration);
  setSeverity();
}

void CAP::ConfigurationManager::setConfigured(bool _configured)
{
  configured = _configured;
  if (!configured) configuration.clear();
}


bool CAP::ConfigurationManager::isConfigured() const
{
  return configured;
}

bool CAP::ConfigurationManager::getValueBool(const char *  key)   const
{
  return configuration.getValueBool(configurationPath,key);
}

int CAP::ConfigurationManager::getValueInt(const char *  key)    const
{
  return configuration.getValueInt(configurationPath,key);
}

//!
//! Get the value of the parameter named 'name'
//!
long CAP::ConfigurationManager::getValueLong(const char *  key)    const
{
  return configuration.getValueLong(configurationPath,key);
}

//!
//! Get the value of the parameter named 'name'
//!
double CAP::ConfigurationManager::getValueDouble(const char *  key) const
{
  return configuration.getValueDouble(configurationPath,key);
}

CAP::String CAP::ConfigurationManager::getValueString(const char *  key) const
{
  return configuration.getValueString(configurationPath,key);
}

bool CAP::ConfigurationManager::getValueBool(const char * path, const char *  key)   const
{
  return configuration.getValueBool(path,key);
}

int CAP::ConfigurationManager::getValueInt(const char * path, const char *  key)    const
{
  return configuration.getValueInt(path,key);
}

//!
//! Get the value of the parameter named 'name'
//!
long CAP::ConfigurationManager::getValueLong(const char * path, const char *  key)    const
{
  return configuration.getValueLong(path,key);
}

double CAP::ConfigurationManager::getValueDouble(const char * path, const char *  key) const
{
  return configuration.getValueDouble(path,key);
}

CAP::String CAP::ConfigurationManager::getValueString(const char * path, const char *  key) const
{
  return configuration.getValueString(path,key);
}

void CAP::ConfigurationManager::addParameter(const char *  name, bool value)
{
//  cout << "Adding parameter " << name << " with value " << value << " for configurationPath " << configurationPath << endl;
  configuration.addParameter(configurationPath,name,value);
}

void CAP::ConfigurationManager::addParameter(const char *  name, int value)
{
  //cout << "Adding parameter " << name << " with value " << value << " for configurationPath " << configurationPath << endl;
  configuration.addParameter(configurationPath,name,value);
}


void CAP::ConfigurationManager::addParameter(const char *  name, long value)
{
  //cout << "Adding parameter " << name << " with value " << value << " for configurationPath " << configurationPath << endl;
  configuration.addParameter(configurationPath,name,value);
}


void CAP::ConfigurationManager::addParameter(const char *  name, double value)
{
  //cout << "Adding parameter " << name << " with value " << value << " for configurationPath " << configurationPath << endl;
  configuration.addParameter(configurationPath,name,value);
}


void CAP::ConfigurationManager::addParameter(const char *  name, const char *  value)
{
  //cout << "Adding parameter " << name << " with value " << value << " for configurationPath " << configurationPath << endl;
  configuration.addParameter(configurationPath,name,value);
}

void CAP::ConfigurationManager::addParameter(const char * path, const char * name, bool value)
{
  configuration.addParameter(path,name,value);
}

void CAP::ConfigurationManager::addParameter(const char * path, const char * name, int value)
{
  configuration.addParameter(path,name,value);
}

void CAP::ConfigurationManager::addParameter(const char * path, const char * name, long value)
{
  configuration.addParameter(path,name,value);
}

void CAP::ConfigurationManager::addParameter(const char * path, const char * name, double value)
{
  configuration.addParameter(path,name,value);
}

void CAP::ConfigurationManager::addParameter(const char * path, const char * name, const char * value)
{
  configuration.addParameter(path,name,value);
}

void CAP::ConfigurationManager::printConfiguration(ostream & output)
{
  configuration.printConfiguration(output);
}

void CAP::ConfigurationManager::generateKeyValuePairs(const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate)
{
  configuration.generateKeyValuePairs("",keyBaseName,defaultValue,nKeysToGenerate);
}

void CAP::ConfigurationManager::generateKeyValuePairs(const char *  path, const char *  keyBaseName, const char *  defaultValue, int nKeysToGenerate)
{
  configuration.generateKeyValuePairs(path,keyBaseName,defaultValue,nKeysToGenerate);
}

CAP::VectorString  CAP::ConfigurationManager::getSelectedValues(const char *  keyBaseName, const char *  defaultValue) const
{
  return configuration.getSelectedValues("",keyBaseName,defaultValue);
}

CAP::VectorString  CAP::ConfigurationManager::getSelectedValues(const char *  path, const char *  keyBaseName, const char *  defaultValue) const
{
  return configuration.getSelectedValues(path,keyBaseName,defaultValue);
}

int CAP::ConfigurationManager::getNSelectedValues(const char *  keyBaseName, const char *  defaultValue)  const
{
  return configuration.getNSelectedValues("",keyBaseName,defaultValue);
}

int CAP::ConfigurationManager::getNSelectedValues(const char *  path, const char *  keyBaseName, const char *  defaultValue) const
{
  return configuration.getNSelectedValues(path,keyBaseName,defaultValue);
}

void CAP::ConfigurationManager::setSeverity()
{
  MessageLogger::Severity selectedLevel;
  String  logOption = getValueString("Severity");
  if (logOption.Contains("Debug"))        selectedLevel = MessageLogger::Debug;
  else if (logOption.Contains("Info"))    selectedLevel = MessageLogger::Info;
  else if (logOption.Contains("Warning")) selectedLevel = MessageLogger::Warning;
  else selectedLevel = MessageLogger::Info;
  setSeverityLevel(selectedLevel);
}

void CAP::ConfigurationManager::setConfigurationPath(const CAP::String _configurationPath)
{
  configurationPath = _configurationPath;
}

const CAP::String & CAP::ConfigurationManager::getConfigurationPath() const
{
  return configurationPath;
}

void CAP::ConfigurationManager::printItem(const char * keyword, ostream & output, int size, int style)  const
{
  switch (style)
    {
      case 0: output <<  left << setw(size) << setfill('.')<< keyword << " : " << configuration.getParameter(keyword) << setfill(' ') << endl; break;
      case 1: output <<  left << setw(size) << setfill(' ')<< keyword << " : " << configuration.getParameter(keyword) << endl; break;
    }
}

void CAP::ConfigurationManager::printItem(const char * keyword, bool   value, ostream & output, int size, int style)  const
{
  switch (style)
    {
      case 0: output <<  left << setw(size) << setfill('.')<< keyword << " : " << value << setfill(' ') << endl; break;
      case 1: output <<  left << setw(size) << setfill(' ')<< keyword << " : " << value << endl; break;
    }
}

void CAP::ConfigurationManager::printItem(const char * keyword, int    value, ostream & output, int size, int style)  const
{
  switch (style)
    {
      case 0: output <<  left << setw(size) << setfill('.')<< keyword << " : " << value << setfill(' ') << endl; break;
      case 1: output <<  left << setw(size) << setfill(' ')<< keyword << " : " << value << endl; break;
    }
}

void CAP::ConfigurationManager::printItem(const char * keyword, long   value, ostream & output, int size, int style)  const
{
  switch (style)
    {
      case 0: output <<  left << setw(size) << setfill('.')<< keyword << " : " << value << setfill(' ') << endl; break;
      case 1: output <<  left << setw(size) << setfill(' ')<< keyword << " : " << value << endl; break;
    }
}

void CAP::ConfigurationManager::printItem(const char * keyword, double value, ostream & output, int size, int style)  const
{
  switch (style)
    {
      case 0: output <<  left << setw(size) << setfill('.')<< keyword << " : " << value << setfill(' ') << endl; break;
      case 1: output <<  left << setw(size) << setfill(' ')<< keyword << " : " << value << endl; break;
    }
}

void CAP::ConfigurationManager::printItem(const char * keyword, const char * value, ostream & output, int size, int style)  const
{
  switch (style)
    {
      case 0: output <<  left << setw(size) << setfill('.')<< keyword << " : " << value << setfill(' ') << endl; break;
      case 1: output <<  left << setw(size) << setfill(' ')<< keyword << " : " << value << endl; break;
    }
}



