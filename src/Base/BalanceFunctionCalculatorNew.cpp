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
#include "BalanceFunctionCalculator.hpp"

ClassImp(BalanceFunctionCalculator);

BalanceFunctionCalculator::BalanceFunctionCalculator(const TString &       _name,
                                                     const Configuration & _configuration,
                                                     MessageLogger::LogLevel debugLevel)
:
Task(_name,_configuration,debugLevel)
{
  appendClassName("BalanceFunctionCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void BalanceFunctionCalculator::setDefaultConfiguration()
{
  TString none  = "none";
  configuration.setParameter("createHistograms",       true);
  configuration.setParameter("loadHistograms",         true);
  configuration.setParameter("saveHistograms",         true);
  configuration.setParameter("appendedString",         TString("_Derived"));
  configuration.setParameter("forceHistogramsRewrite", true);
  configuration.generateKeyValuePairs("IncludedPattern",none,20);
  configuration.generateKeyValuePairs("ExcludedPattern",none,20);
//  if (reportDebug(__FUNCTION__))
//    {
//    configuration.printConfiguration(cout);
//    }
}


void BalanceFunctionCalculator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  TString none("none");
  TString appendedString           = configuration.getValueString("appendedString");
  bool    forceHistogramsRewrite   = configuration.getValueBool("forceHistogramsRewrite");
  TString histoInputPath           = configuration.getValueString("histoInputPath");
  TString histoOutputPath          = configuration.getValueString("histoOutputPath");
  TString histoOutputDataName      = configuration.getValueString("histoOutputDataName");
  vector<TString> includedPatterns = configuration.getSelectedValues("IncludedPattern",none);
  vector<TString> excludedPatterns = configuration.getSelectedValues("ExcludedPattern",none);
  TString histoOutputAnalyzerName;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug())  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int iTask=0; iTask<nSubTasks; iTask++)
    {
    Task & subTask = *subTasks[iTask];
    Configuration & subTaskConfig = subTask.getConfiguration();
    histoOutputAnalyzerName = subTaskConfig.getValueString("histoOutputAnalyzerName");
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << " ===========================================================" << endl;
      cout << " ===========================================================" << endl;
      cout << "               SubTask #: " << iTask  << endl;
      cout << "            SubTask Name: " << taskName  << endl;
      cout << "          histoInputPath: " << histoInputPath  << endl;
      cout << "         histoOutputPath: " << histoOutputPath  << endl;
      cout << "     histoOutputDataName: " << histoOutputDataName  <<   endl;
      cout << " histoOutputAnalyzerName: " << histoOutputAnalyzerName   << endl;
      cout << " ===========================================================" << endl;
      cout << " ===========================================================" << endl;
      }
    vector<TString> includePatterns = configuration.getSelectedValues("IncludedPattern", "none");
    vector<TString> excludePatterns = configuration.getSelectedValues("ExcludedPattern", "none");
    includePatterns.push_back(histoOutputDataName);
    includePatterns.push_back(histoOutputAnalyzerName);
    vector<TString> allFilesToProcess = listFilesInDir(histoInputPath,includePatterns,excludePatterns);
    int nFiles = allFilesToProcess.size();
    if (nFiles<1)
      {
      if (reportError(__FUNCTION__))
        {
        cout << endl;
        cout << "X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
        cout << "X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
        cout << "         Attempting to execute Balance Function calculation with no selected files." << endl;
        cout << "                               Check your code!!!!!!! " <<  endl;
        cout << "X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
        cout << "X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
        }
      return;
      }
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << " ===========================================================" << endl;
      cout << "                   nFiles: " << nFiles << endl;
      cout << "           appendedString: " << appendedString << endl;
      cout << "           histoInputPath: " << histoInputPath << endl;
      cout << "          histoOutputPath: " << histoOutputPath << endl;
      cout << "      histoOutputDataName: " << histoOutputDataName << endl;
      cout << "  histoOutputAnalyzerName: " << histoOutputAnalyzerName << endl;
      cout << " ===========================================================" << endl;
      }
    for (int iFile=0; iFile<nFiles; iFile++)
      {
      Task * task = subTask.getDerivedCalculator();
      if (!task)
        {
        if (reportError(__FUNCTION__))
          {
          cout << endl;
          cout << " X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
          cout << "     iTask: " << iTask << " Task Named: " << subTask.getName() << endl;
          cout << "     HAS NO DERIVED CALCULATOR"  << endl;
          cout << " X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
          break;
          }
        }
      task->setReportLevel(getReportLevel());
      TString histoInputFileName  = allFilesToProcess[iFile];
      TString histoOutputFileName = removeRootExtension(histoInputFileName);
      histoOutputFileName += appendedString;
//      if (reportDebug(__FUNCTION__))
//        {
//        cout << endl;
//        cout << " ===========================================================" << endl;
//        cout
//        << "         iFile: " << iFile << endl
//        << "    Input file: " << histoInputFileName << endl
//        << "   Output file: " << histoOutputFileName << endl;
//        }
      if (reportInfo(__FUNCTION__)) cout << "iFile: " << iFile << " File: " << histoInputFileName << endl;
      Configuration config;
      config.setParameter("histoInputPath",         TString(""));
      config.setParameter("histoInputFileName",     histoInputFileName);
      config.setParameter("forceHistogramsRewrite", forceHistogramsRewrite);
      config.setParameter("histoOutputPath",        TString(""));
      config.setParameter("histoOutputFileName",    histoOutputFileName);
      config.setParameter("histoOutputAnalyzerName",histoOutputAnalyzerName);
      config.setParameter("createHistograms",       true);
      config.setParameter("loadHistograms",         true);
      config.setParameter("saveHistograms",         true);
      config.setParameter("scaleHistograms",        false);
      config.setParameter("doSubsampleAnalysis",    false);
      config.setParameter("doPartialSaves",         false);
      config.setParameter("useParticles",           true);
      config.addParameter("useEventStream0",        false);
      config.addParameter("useEventStream1",        false);

      task->setConfiguration(config);
      task->setReportLevel(getReportLevel());
      postTaskOk();
      task->initialize();
      if (!isTaskOk()) break;
      task->execute();
      if (!isTaskOk()) break;
      task->finalize();
      task->clear();
      delete task;
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

