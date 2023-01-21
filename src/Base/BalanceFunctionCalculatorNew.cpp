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

BalanceFunctionCalculator::BalanceFunctionCalculator(const TString & _name
                                                     Configuration & _configuration)
:
Task(_name,_configuration)
{
  appendClassName("BalanceFunctionCalculator");
}

void BalanceFunctionCalculator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  TString none  = "none";
  setParameter("CreateHistograms",       true);
  setParameter("LoadHistograms",         true);
  setParameter("SaveHistograms",         true);
  setParameter("AppendedString",         TString("_Derived"));
  //setParameter("ForceHistogramsRewrite", true);
  generateKeyValuePairs("IncludedPattern",none,20);
  generateKeyValuePairs("ExcludedPattern",none,20);
}

void BalanceFunctionCalculator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  TString none("none");
  TString appendedString           = getValueString("AppendedString");
  bool    ForceHistogramsRewrite   = getValueBool("ForceHistogramsRewrite");
  TString histoInputPath           = getValueString("HistogramInputPath");
  TString HistogramOutputPath      = getValueString("HistogramOutputPath");
  TString histoOutputDataName      = getValueString("HistoOutputDataName");
  vector<TString> includedPatterns = getSelectedValues("IncludedPattern",none);
  vector<TString> excludedPatterns = getSelectedValues("ExcludedPattern",none);
  TString histoOutputAnalyzerName;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug())  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int iTask=0; iTask<nSubTasks; iTask++)
    {
    Task & subTask = *subTasks[iTask];
    Configuration & subTaskConfig = subTask.getConfiguration();
    histoOutputAnalyzerName = subTaskConfig.getValueString(getName(),"HistoOutputAnalyzerName");
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << " ===========================================================" << endl;
      cout << " ===========================================================" << endl;
      cout << "               SubTask #: " << iTask  << endl;
      cout << "            SubTask Name: " << taskName  << endl;
      cout << "          histoInputPath: " << histoInputPath  << endl;
      cout << "       stogramOutputPath: " << HistogramOutputPath  << endl;
      cout << "     histoOutputDataName: " << histoOutputDataName  <<   endl;
      cout << " histoOutputAnalyzerName: " << histoOutputAnalyzerName   << endl;
      cout << " ===========================================================" << endl;
      cout << " ===========================================================" << endl;
      }
    vector<TString> includePatterns = getSelectedValues("IncludedPattern", "none");
    vector<TString> excludePatterns = getSelectedValues("ExcludedPattern", "none");
    includePatterns.push_back(histoOutputDataName);
    includePatterns.push_back(histoOutputAnalyzerName);
    bool prependPath = true;
    bool verbose     = true;
    int  maximumDepth = 2;
    vector<TString> allFilesToProcess   = listFilesInDir(histoInputPath,includePatterns,excludePatterns,prependPath, verbose, maximumDepth);
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
      cout << "      HistogramOutputPath: " << HistogramOutputPath << endl;
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
      task->setSeverityLevel(getSeverityLevel());
      TString HistogramInputFile  = allFilesToProcess[iFile];
      TString HistogramOutputFile = removeRootExtension(HistogramInputFile);
      HistogramOutputFile += appendedString;
//      if (reportDebug(__FUNCTION__))
//        {
//        cout << endl;
//        cout << " ===========================================================" << endl;
//        cout
//        << "         iFile: " << iFile << endl
//        << "    Input file: " << HistogramInputFile << endl
//        << "   Output file: " << HistogramOutputFile << endl;
//        }
      if (reportInfo(__FUNCTION__)) cout << "iFile: " << iFile << " File: " << HistogramInputFile << endl;
      task->setParameter("HistogramInputPath",     TString(""));
      task->setParameter("HistogramInputFile",     HistogramInputFile);
      task->setParameter("ForceHistogramsRewrite", ForceHistogramsRewrite);
      task->setParameter("HistogramOutputPath",    TString(""));
      task->setParameter("HistogramOutputFile",    HistogramOutputFile);
      task->setParameter("CreateHistograms",       true);
      task->setParameter("LoadHistograms",         true);
      task->setParameter("SaveHistograms",         true);
      task->setParameter("ScaleHistograms",        false);
      task->setParameter("DoSubsampleAnalysis",    false);
      task->setParameter("DoPartialSaves",         false);
      task->setParameter("UseParticles",           true);
      task->setParameter("UseEventStream0",        false);
      task->setParameter("UseEventStream1",        false);
      task->setSeverityLevel(getSeverityLevel());
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

