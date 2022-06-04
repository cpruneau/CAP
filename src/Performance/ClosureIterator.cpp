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
#include "ClosureIterator.hpp"
#include "ClosureCalculator.hpp"

ClassImp(ClosureIterator);


ClosureIterator::ClosureIterator(const TString &       _name,
                                 const Configuration & _configuration,
                                 MessageLogger::LogLevel _debugLevel)
:
Task(_name,_configuration,_debugLevel)
{
  appendClassName("ClosureIterator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void ClosureIterator::setDefaultConfiguration()
{
  TString none  = "none";
  configuration.setParameter("createHistograms",        true);
  configuration.setParameter("loadHistograms",          true);
  configuration.setParameter("saveHistograms",          true);
  configuration.setParameter("appendedString",          TString("Closure"));
  configuration.setParameter("forceHistogramsRewrite",  true);
  configuration.setParameter("selectedMethod",          1);
  configuration.generateKeyValuePairs("IncludedPattern",none,20);
  configuration.generateKeyValuePairs("ExcludedPattern",none,20);
  // if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}


TString  substitute(const TString inputString, const TString subString, const TString newSubString)
{
  TString outputString(inputString);
  outputString.ReplaceAll(subString,newSubString);
  return outputString;
}


void ClosureIterator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  TString none  = "none";
  TString appendedString      = configuration.getValueString("appendedString");
  TString histoInputPath      = configuration.getValueString("histoInputPath");
  TString histoOutputPath     = configuration.getValueString("histoOutputPath");
  TString histoModelDataName  = configuration.getValueString("histoModelDataName");
  bool forceHistogramsRewrite = configuration.getValueBool("forceHistogramsRewrite");
  int selectedMethod          = configuration.getValueInt("selectedMethod");


  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++)
    {
    Task & subTask = *subTasks[iTask];
    Configuration & subTaskConfig = subTask.getConfiguration();
    TString taskName              = subTask.getName();
    TString histoAnalyzerName     = subTaskConfig.getValueString("histoAnalyzerName");
    if (reportInfo(__FUNCTION__))  cout << "SubTasks:" << iTask  << " named:" << taskName << " w/ analyzer:" <<  histoAnalyzerName << endl;
    vector<TString> includedPatterns = configuration.getSelectedValues("IncludedPattern",none);
    vector<TString> excludedPatterns = configuration.getSelectedValues("ExcludedPattern",none);
    includedPatterns.push_back(histoModelDataName);
    includedPatterns.push_back(histoAnalyzerName);
    excludedPatterns.push_back("Reco");
    if (reportDebug(__FUNCTION__))
      {
      for (unsigned int k=0;k<includedPatterns.size();k++)
        {
        cout << " k:" << k << "  Include: " << includedPatterns[k] << endl;
        }

      for (unsigned int k=0;k<excludedPatterns.size();k++)
        {
        cout << " k:" << k << "  Exclude: " << excludedPatterns[k] << endl;
        }
      }
    vector<TString> allFilesToProcess = listFilesInDir(histoInputPath,includedPatterns,excludedPatterns);
    int nFilesToProcess = allFilesToProcess.size();
    if (nFilesToProcess<1)
      {
      if (reportError(__FUNCTION__))
        {
        cout << endl;
        cout << "========================================================================"  << endl;
        cout << "========================================================================"  << endl;
        cout << "  Attempting to execute closure analysis with no selected files." << endl;
        cout << "                         Check your code!!!!!!! " <<  endl;
        cout << "========================================================================"  << endl;
        cout << "========================================================================"  << endl;
        }
      return;
      }
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << " ===========================================================" << endl;
      cout << " ===========================================================" << endl;
      cout << "             SubTask Name: " << taskName  << endl;
      cout << "           histoInputPath: " << histoInputPath  << endl;
      cout << "          histoOutputPath: " << histoOutputPath  << endl;
      cout << "       histoModelDataName: " << histoModelDataName  <<   endl;
      cout << "        histoAnalyzerName: " << histoAnalyzerName   << endl;
      cout << "          nFilesToProcess: " << nFilesToProcess << endl;
      cout << "           appendedString: " << appendedString << endl;
      cout << " ===========================================================" << endl;
      cout << " ===========================================================" << endl;
      }
    postTaskOk();

    for (int iFile=0; iFile<nFilesToProcess; iFile++)
      {
      TString histoGeneratorFileName = removeRootExtension(allFilesToProcess[iFile]);
      TString histoDetectorFileName  = substitute(histoGeneratorFileName, "_Gen", "_Reco");
      TString histoClosureFileName   = substitute(histoGeneratorFileName, "_Gen", "_Closure");

      if (reportInfo(__FUNCTION__))
        {
        cout << endl;
        cout << " --------------------------------------------------------------------------" << endl;
        cout << "                iFile: " << iFile << endl;
        cout << "  Generator File Name: " << histoGeneratorFileName << endl;
        cout << "   Detector File Name: " << histoDetectorFileName << endl;
        cout << "    Closure File Name: " << histoClosureFileName << endl;
        }
      Configuration closureConfig;
      closureConfig.setParameter("histoInputPath",         histoInputPath);
      closureConfig.setParameter("histoOutputPath",        histoOutputPath);
      closureConfig.setParameter("histoModelDataName",     histoModelDataName);
      closureConfig.setParameter("histoAnalyzerName",      histoAnalyzerName);
      closureConfig.setParameter("appendedString",         appendedString);
      closureConfig.setParameter("forceHistogramsRewrite", forceHistogramsRewrite);
      closureConfig.setParameter("selectedMethod",         selectedMethod);
      closureConfig.setParameter("histoGeneratorFileName", histoGeneratorFileName);
      closureConfig.setParameter("histoDetectorFileName",  histoDetectorFileName);
      closureConfig.setParameter("histoClosureFileName",   histoClosureFileName);
      ClosureCalculator calculator("ClosureCalculator", closureConfig, getReportLevel());
      calculator.execute();
      } // iFile loop
    } // iTask loop
  if (reportEnd(__FUNCTION__))
    ;
}

