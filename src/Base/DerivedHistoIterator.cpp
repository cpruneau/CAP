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
#include "DerivedHistoIterator.hpp"
using CAP::DerivedHistoIterator;
using CAP::Configuration;
using CAP::String;
using CAP::VectorString;

ClassImp(DerivedHistoIterator);


DerivedHistoIterator::DerivedHistoIterator(const String & _name,
                                           const Configuration & _configuration)
:
Task(_name,_configuration)
{
  appendClassName("DerivedHistoIterator");
}

void DerivedHistoIterator::setDefaultConfiguration()
{
  String none  = "none";
  addParameter("HistogramsCreate",         true);
  addParameter("HistogramsImport",           true);
  addParameter("HistogramsExport",           true);
  addParameter("AppendedString",           TString("_Derived"));
  generateKeyValuePairs("IncludedPattern", none,20);
  generateKeyValuePairs("ExcludedPattern", none,20);
}

void DerivedHistoIterator::execute()
{
  String none("none");
  String analyzerName;
  String appendedString = getValueString("AppendedString");
  histosForceRewrite    = getValueString("HistogramsForceRewrite");
  histosImportPath      = getValueString("HistogramsImportPath");
  histosExportPath      = getValueString("HistogramsExportPath");
  VectorString  includedPatterns = getSelectedValues("IncludedPattern",none);
  VectorString  excludedPatterns = getSelectedValues("ExcludedPattern",none);
  unsigned int nSubTasks = subTasks.size();

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " SubTasks Count....................: " << nSubTasks            << endl;
    cout << " AppendedString....................: " << appendedString       << endl;
    cout << " HistogramsForceRewrite............: " << histosForceRewrite   << endl;
    cout << " HistogramsImportPath..............: " << histosImportPath     << endl;
    cout << " HistogramsExportPath..............: " << histosExportPath     << endl;
    for (unsigned int k=0; k<includedPatterns.size(); k++)
      {
      cout << " Included.................: " << includedPatterns[k]     << endl;
      }
    for (unsigned int k=0; k<excludedPatterns.size(); k++)
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
    VectorString  includePatterns = getSelectedValues("IncludedPattern", "none");
    VectorString  excludePatterns = getSelectedValues("ExcludedPattern", "none");
    includePatterns.push_back(analyzerName);
    bool isReco = analyzerName.Contains("Reco");
    if (isReco)  includePatterns.push_back(TString("Reco"));
    if (!isReco) excludePatterns.push_back(TString("Reco"));
    excludePatterns.push_back(TString("Derived"));
    bool prependPath = true;
    bool verbose = false;
    int  maximumDepth = 1;
    VectorString  allFilesToProcess = listFilesInDir(histosImportPath,includePatterns,excludePatterns, prependPath, verbose, maximumDepth,0);
    
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
      String HistogramsImportFile  = allFilesToProcess[iFile];
      String histosExportFile = removeRootExtension(HistogramsImportFile);
      histosExportFile += appendedString;
      if (reportInfo(__FUNCTION__))
        {
        cout << endl;
        cout << " nFiles................: " << nFiles << endl;
        cout << " iFile.................: " << iFile  << endl;
        cout << " Input file............: " << HistogramsImportFile << endl;
        cout << " Output file...........: " << histosExportFile << endl;
        }
      String nullString = "";
      subTask.addParameter("HistogramsImportPath",nullString);
      subTask.addParameter("HistogramsExportPath",nullString);
      subTask.addParameter("HistogramsImportFile",HistogramsImportFile);
      subTask.addParameter("HistogramsExportFile",histosExportFile);
      subTask.importHistograms();
      subTask.createDerivedHistograms();
      if (!isTaskOk()) break;
      subTask.calculateDerivedHistograms();
      if (!isTaskOk()) break;
      subTask.exportHistograms();
      }
    }
  if (reportEnd(__FUNCTION__))
;
}

