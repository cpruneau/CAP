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
#include "DerivedHistoIterator.hpp"
#include "SubSampleStatCalculator.hpp"

ClassImp(DerivedHistoIterator);


DerivedHistoIterator::DerivedHistoIterator(const TString & _name,
                                           Configuration & _configuration)
:
Task(_name,_configuration)
{
  appendClassName("DerivedHistoIterator");
}

void DerivedHistoIterator::setDefaultConfiguration()
{
  TString none  = "none";
  setParameter("CreateHistograms",         true);
  setParameter("LoadHistograms",           true);
  setParameter("SaveHistograms",           true);
  setParameter("AppendedString",           TString("_Derived"));
  generateKeyValuePairs("IncludedPattern", none,20);
  generateKeyValuePairs("ExcludedPattern", none,20);
}

void DerivedHistoIterator::execute()
{
  TString none("none");
  TString analyzerName;
  TString appendedString           = getValueString("AppendedString");
  bool    forceHistogramsRewrite   = getValueString("ForceHistogramsRewrite");
  TString histogramInputPath       = getValueString("HistogramInputPath");
  TString histogramOutputPath      = getValueString("HistogramOutputPath");
  vector<TString> includedPatterns = getSelectedValues("IncludedPattern",none);
  vector<TString> excludedPatterns = getSelectedValues("ExcludedPattern",none);
  unsigned int nSubTasks = subTasks.size();

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " SubTasks Count...................: " << nSubTasks              << endl;
    cout << " AppendedString...................: " << appendedString         << endl;
    cout << " ForceHistogramsRewrite...........: " << forceHistogramsRewrite << endl;
    cout << " HistogramInputPath...............: " << histogramInputPath     << endl;
    cout << " HistogramOutputPath..............: " << histogramOutputPath    << endl;
    for (int k=0; k<includedPatterns.size(); k++)
      {
      cout << " Included.................: " << includedPatterns[k]     << endl;
      }
    for (int k=0; k<excludedPatterns.size(); k++)
      {
      cout << " Excluded.................: " << excludedPatterns[k]     << endl;
      }
    }

  for (unsigned int iTask=0; iTask<nSubTasks; iTask++)
    {
    Task & subTask = *subTasks[iTask];
    analyzerName = subTask.getName();
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << " ===========================================================" << endl;
      cout << " SubTask#.......................: " << iTask  << endl;
      cout << " SubTask Name...................: " << analyzerName  << endl;
      cout << " ===========================================================" << endl;
      }
    vector<TString> includePatterns = getSelectedValues("IncludedPattern", "none");
    vector<TString> excludePatterns = getSelectedValues("ExcludedPattern", "none");
    includePatterns.push_back(analyzerName);
    bool isReco = analyzerName.Contains("Reco");
    if (isReco)  includePatterns.push_back(TString("Reco"));
    if (!isReco) excludePatterns.push_back(TString("Reco"));
    excludePatterns.push_back(TString("Derived"));
    bool prependPath = true;
    bool verbose = false;
    int  maximumDepth = 1;
    vector<TString> allFilesToProcess = listFilesInDir(histogramInputPath,includePatterns,excludePatterns, prependPath, verbose, maximumDepth,0);
    
    int nFiles = allFilesToProcess.size();
    if (nFiles<1)
      {
      if (reportError(__FUNCTION__))
        {
        cout << endl;
        cout << "X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
        cout << "X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
        cout << "  Attempting to execute derived histo analysis with no selected files." << endl;
        cout << "                         Check your code!!!!!!! " <<  endl;
        cout << "X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
        cout << "X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#X#"  << endl;
        }
      return;
      }
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << " nFiles................: " << nFiles << endl;
      }
    for (int iFile=0; iFile<nFiles; iFile++)
      {
      TString histogramInputFile  = allFilesToProcess[iFile];
      TString histogramOutputFile = removeRootExtension(histogramInputFile);
      histogramOutputFile += appendedString;
      if (reportInfo(__FUNCTION__))
        {
        cout << endl;
        cout << " nFiles................: " << nFiles << endl;
        cout << " iFile.................: " << iFile  << endl;
        cout << " Input file............: " << histogramInputFile << endl;
        cout << " Output file...........: " << histogramOutputFile << endl;
        }
      postTaskOk();
      TString nullString = "";
      subTask.setParameter("HistogramInputPath",nullString);
      subTask.setParameter("HistogramOutputPath",nullString);
      subTask.setParameter("HistogramInputFile",histogramInputFile);
      subTask.setParameter("HistogramOutputFile",histogramOutputFile);

      subTask.loadHistograms();
      subTask.createDerivedHistograms();
      if (!isTaskOk()) break;
      subTask.calculateDerivedHistograms();
      if (!isTaskOk()) break;
      subTask.saveHistograms();
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

