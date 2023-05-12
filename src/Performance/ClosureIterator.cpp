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
//#include "HistogramCollection.hpp"
#include "ClosureIterator.hpp"
#include "ClosureCalculator.hpp"
using CAP::ClosureIterator;
using CAP::String;

ClassImp(ClosureIterator);


ClosureIterator::ClosureIterator(const String & _name,
                                 const Configuration & _configuration)
:
Task(_name,_configuration)
{
  appendClassName("ClosureIterator");
}

void ClosureIterator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  String none  = "none";
  addParameter("HistogramsCreate",        true);
  addParameter("HistogramsImport",          true);
  addParameter("HistogramsExport",          true);
  addParameter("AppendedString",          TString("Closure"));
  addParameter("SelectedMethod",          1);
  generateKeyValuePairs("IncludedPattern",none,20);
  generateKeyValuePairs("ExcludedPattern",none,20);
}


String  substitute(const String inputString, const String subString, const String newSubString)
{
  String outputString(inputString);
  outputString.ReplaceAll(subString,newSubString);
  return outputString;
}


void ClosureIterator::execute()
{
  String none  = "none";
  String appendedString        = getValueString("AppendedString");
  String histogramsImportPath  = getValueString("HistogramsImportPath");
  String histogramsExportPath  = getValueString("HistogramsExportPath");
  bool histosForceRewrite      = getValueBool(  "HistogramsForceRewrite");
  int selectedMethod           = getValueInt(   "SelectedMethod");

  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++)
    {
    Task & subTask     = *subTasks[iTask];
    String subTaskName = subTask.getName();
    VectorString  includedPatterns = getSelectedValues("IncludedPattern",none);
    VectorString  excludedPatterns = getSelectedValues("ExcludedPattern",none);
    includedPatterns.push_back("XXXXX");
    includedPatterns.push_back("XXXXY");
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
    VectorString  allFilesToProcess = listFilesInDir(histogramsImportPath,includedPatterns,excludedPatterns);
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
      cout << " SubTask Name................: " << getName()  << endl;
      cout << " HistogramsImportPath........: " << histogramsImportPath  << endl;
      cout << " HistogramsExportPath........: " << histogramsExportPath  << endl;
      cout << " nFilesToProcess.............: " << nFilesToProcess << endl;
      cout << " appendedString..............: " << appendedString << endl;
      cout << " ===========================================================" << endl;
      cout << " ===========================================================" << endl;
      }
    postTaskOk();

    for (int iFile=0; iFile<nFilesToProcess; iFile++)
      {
      String histoGeneratorFileName = removeRootExtension(allFilesToProcess[iFile]);
      String histoDetectorFileName  = substitute(histoGeneratorFileName, "_Gen", "_Reco");
      String histoClosureFileName   = substitute(histoGeneratorFileName, "_Gen", "_Closure");

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
      closureConfig.addParameter("HistogramsImportPath",   histogramsImportPath);
      closureConfig.addParameter("HistogramsExportPath",   histogramsExportPath);
//      closureConfig.addParameter("HistoModelDataName",   histoModelDataName);
//      closureConfig.addParameter("HistoAnalyzerName",    histoAnalyzerName);
      closureConfig.addParameter("AppendedString",         appendedString);
      closureConfig.addParameter("HistogramsForceRewrite", histosForceRewrite);
      closureConfig.addParameter("SelectedMethod",         selectedMethod);
      closureConfig.addParameter("HistoGeneratorFileName", histoGeneratorFileName);
      closureConfig.addParameter("HistoDetectorFileName",  histoDetectorFileName);
      closureConfig.addParameter("HistoClosureFileName",   histoClosureFileName);
      ClosureCalculator calculator("Closure", closureConfig);
      calculator.execute();
      } // iFile loop
    } // iTask loop
  if (reportEnd(__FUNCTION__))
;
}

