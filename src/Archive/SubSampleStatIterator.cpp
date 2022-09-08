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
#include "HistogramCollection.hpp"
#include "SubSampleStatIterator.hpp"
#include "SubSampleStatCalculator.hpp"

ClassImp(SubSampleStatIterator);


SubSampleStatIterator::SubSampleStatIterator(const TString & _name,
                                             Configuration & _configuration)
:
Task(_name,_configuration)
{
  appendClassName("SubSampleStatIterator");
}

void SubSampleStatIterator::setDefaultConfiguration()
{
  TString none  = "none";
  setParameter("CreateHistograms",        true);
  setParameter("LoadHistograms",          true);
  setParameter("SaveHistograms",          true);
  setParameter("AppendedString",          TString("Sum"));
  setParameter("ForceHistogramsRewrite",  true);
  addParameter("DefaultGroupSize",        10);
  addParameter("nInputFile",              0);
  generateKeyValuePairs("IncludedPattern",none,20);
  generateKeyValuePairs("ExcludedPattern",none,20);
  generateKeyValuePairs("InputFile",      none,100);
}


void SubSampleStatIterator::execute()
{
  if (reportStart(__FUNCTION__))
    ;

  TString none  = "none";
  TString appendedString            = getValueString("AppendedString");
  bool ForceHistogramsRewrite       = getValueBool(  "ForceHistogramsRewrite");
  int  defaultGroupSize             = getValueInt(   "DefaultGroupSize");
  TString HistogramInputPath        = getValueString("HistogramInputPath");
  TString HistogramOutputPath       = getValueString("HistogramOutputPath");
  vector<TString> includedPatterns  = getSelectedValues("IncludedPattern",none);
  vector<TString> excludedPatterns  = getSelectedValues("ExcludedPattern",none);
  if (reportInfo(__FUNCTION__))
    {
    cout << "include patterns size: " << includedPatterns.size() << endl;
    for (unsigned int k=0; k<includedPatterns.size(); k++)
      {
      cout << " k:" << k << includedPatterns[k] << endl;
      }
    cout << "exclude patterns size: " << excludedPatterns.size() << endl;
    for (unsigned int k=0; k<excludedPatterns.size(); k++)
      {
      cout << " k:" << k << excludedPatterns[k] << endl;
      }
    }


  unsigned int nSubTasks = subTasks.size();
  if (reportDebug())  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++)
    {
    Task & subTask = *subTasks[iTask];
    //Configuration & subTaskConfig = subTask.getConfiguration();
    TString subtaskName        = subTask.getName();
    if (reportInfo(__FUNCTION__))  cout << "SubTasks: " << iTask << " Named:" <<  taskName << endl;
    TString HistogramOutputFile;
    HistogramOutputFile += "XXXXXXX";
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << " SubTask Name................: " << subtaskName  << endl;
      cout << " HistoInputPath..............: " << HistogramInputPath  << endl;
      cout << " HistogramOutputPath.........: " << HistogramOutputPath  << endl;
      cout << " HistogramOutputFile.........: " << HistogramOutputFile  << endl;
      }
//    Configuration subsampleConfig;
//    subsampleConfig.setParameter("HistogramInputPath",     HistogramInputPath);
//    subsampleConfig.setParameter("HistogramOutputPath",    HistogramOutputPath);
//    subsampleConfig.setParameter("AppendedString",         appendedString);
//    subsampleConfig.setParameter("ForceHistogramsRewrite", ForceHistogramsRewrite);
//    subsampleConfig.setParameter("DefaultGroupSize",       defaultGroupSize);
//    SubSampleStatCalculator calculator(subtaskName, subsampleConfig);
//    calculator.configure();
//    calculator.execute();

    cout << " In lieu of code execution" << endl;

    }
  if (reportEnd(__FUNCTION__))
    { }
}

