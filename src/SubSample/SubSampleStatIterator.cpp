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


SubSampleStatIterator::SubSampleStatIterator(const TString &       _name,
                                                 const Configuration & _configuration,
                                                 MessageLogger::LogLevel debugLevel)
:
Task(_name,_configuration,debugLevel)
{
  appendClassName("SubSampleStatIterator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void SubSampleStatIterator::setDefaultConfiguration()
{
  TString none  = "none";
  configuration.setParameter("createHistograms",       true);
  configuration.setParameter("loadHistograms",         true);
  configuration.setParameter("saveHistograms",         true);
  configuration.setParameter("appendedString",         TString("SubSampleSum_"));
  configuration.setParameter("forceHistogramsRewrite", true);
  configuration.addParameter("defaultGroupSize",         10);
  configuration.addParameter("nInputFile",                0);
  configuration.generateKeyValuePairs("IncludedPattern",none,20);
  configuration.generateKeyValuePairs("ExcludedPattern",none,20);
  configuration.generateKeyValuePairs("InputFile",none,100);
  // if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}


void SubSampleStatIterator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  TString none  = "none";
  TString appendedString      = configuration.getValueString("appendedString");
  bool forceHistogramsRewrite = configuration.getValueBool("forceHistogramsRewrite");
  int  defaultGroupSize       = configuration.getValueInt("defaultGroupSize");
  TString histoInputPath      = configuration.getValueString("histoInputPath");
  TString histoOutputPath     = configuration.getValueString("histoOutputPath");
  TString histoModelDataName  = configuration.getValueString("histoModelDataName");
  vector<TString> includedPatterns = configuration.getSelectedValues("IncludedPattern",none);
  vector<TString> excludedPatterns = configuration.getSelectedValues("ExcludedPattern",none);
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
    Configuration & subTaskConfig = subTask.getConfiguration();
    TString taskName        = subTask.getName();
    if (reportInfo(__FUNCTION__))  cout << "SubTasks: " << iTask << " Named:" <<  taskName << endl;

    TString histoAnalyzerName = subTaskConfig.getValueString("histoAnalyzerName");
    TString histoOutputFileName;
    histoOutputFileName = histoModelDataName;
    histoOutputFileName += "_";
    histoOutputFileName += histoAnalyzerName;
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << "            SubTask Name: " << taskName  << endl;
      cout << "          histoInputPath: " << histoInputPath  << endl;
      cout << "         histoOutputPath: " << histoOutputPath  << endl;
      cout << "      histoModelDataName: " << histoModelDataName  <<   endl;
      cout << "       histoAnalyzerName: " << histoAnalyzerName   << endl;
      cout << "     histoOutputFileName: " << histoOutputFileName  << endl;
      }
    Configuration subsampleConfig;
    subsampleConfig.setParameter("histoInputPath",         histoInputPath);
    subsampleConfig.setParameter("histoOutputPath",        histoOutputPath);
    subsampleConfig.setParameter("histoBaseName",          histoOutputFileName);
    subsampleConfig.setParameter("appendedString",         appendedString);
    subsampleConfig.setParameter("forceHistogramsRewrite", forceHistogramsRewrite);
    subsampleConfig.setParameter("histoModelDataName",     histoModelDataName);
    subsampleConfig.setParameter("histoAnalyzerName",      histoAnalyzerName);
    subsampleConfig.setParameter("defaultGroupSize",       defaultGroupSize);
    subsampleConfig.setParameter("IncludedPattern",        taskName);
    subsampleConfig.addSelectedValues("IncludedPattern", "none", includedPatterns);
    subsampleConfig.addSelectedValues("ExcludedPattern", "none", excludedPatterns);
    TString statCalculatorName = "SubSampleStatCalculator_";
    statCalculatorName += taskName;
    SubSampleStatCalculator calculator(statCalculatorName, subsampleConfig, getReportLevel());
    calculator.execute();
    }
  if (reportEnd(__FUNCTION__))
    { }
}

