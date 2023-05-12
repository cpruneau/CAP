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
#include "SubSampleStatCalculator.hpp"
using CAP::SubSampleStatCalculator;

ClassImp(SubSampleStatCalculator);


SubSampleStatCalculator::SubSampleStatCalculator(const String & _name,
                                                 const Configuration & _configuration)
:
Task(_name,_configuration),
nEventProcessed(0),
sumEventProcessed(0)
{
  appendClassName("SubSampleStatCalculator");
}



void SubSampleStatCalculator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  String none  = "none";
  addParameter("HistogramsCreate",       true);
  addParameter("HistogramsImport",       true);
  addParameter("HistogramsExport",       true);
  addParameter("AppendedString",         TString("Sum"));
  addParameter("HistogramsForceRewrite", true);
  addParameter("DefaultGroupSize",       10);
  addParameter("nInputFile",             0);
  generateKeyValuePairs("IncludedPattern",none,20);
  generateKeyValuePairs("ExcludedPattern",none,20);
  generateKeyValuePairs("InputFile",      none,100);
}

void SubSampleStatCalculator::configure()


void SubSampleStatCalculator::execute()
{
  if (reportStart(__FUNCTION__))
;
  int defaultGroupSize          = getValueInt("DefaultGroupSize");
  String appendedString         = getValueString("AppendedString");
  String HistogramsImportPath   = getValueString("HistogramsImportPath");
  String histosExportPath       = getValueString("HistogramsExportPath");
  String histosExportFile       = getValueString("HistogramsExportFile");
  VectorString  includePatterns = getSelectedValues("IncludedPattern", "none");
  VectorString  excludePatterns = getSelectedValues("ExcludedPattern", "none");
  bool prependPath  = true;
  bool verbose      = true;
  int  maximumDepth = 2;
  VectorString  allFilesToSum   = listFilesInDir(histoInputPath,includePatterns,excludePatterns,prependPath, verbose, maximumDepth);
  int nFilesToSum = allFilesToSum.size();
  int groupSize   = (nFilesToSum>defaultGroupSize) ? defaultGroupSize : nFilesToSum;
  int nGroups     = 1 + double(nFilesToSum-1)/double(groupSize);
  if (nFilesToSum<1)
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;
      cout << "========================================================================"  << endl;
      cout << "  Attempting to execute subsample analysis with no selected files." << endl;
      cout << "                         Check your code!!!!!!! " <<  endl;
      cout << "========================================================================"  << endl;
      }
    return;
    }
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " ==========================================================================================" << endl;
    cout << " nFilesToSum................: " << nFilesToSum << endl;
    cout << " Default group size.........: " << defaultGroupSize << endl;
    cout << " Actual group size..........: " << groupSize << endl;
    cout << " nGroups....................: " << nGroups << endl;
    cout << " appendedString.............: " << appendedString << endl;
    cout << " HistogramsImportPath.........: " << HistogramsImportPath << endl;
    cout << " HistogramsExportPath........: " << histosExportPath << endl;
    cout << " HistogramsExportFile........: " << histosExportFile << endl;
    cout << " ==========================================================================================" << endl;
    }
  postTaskOk();
  
  for (int iGroup =0; iGroup<nGroups; iGroup++  )
    {
    int first = iGroup*groupSize;
    int last  = (iGroup+1)*groupSize;
    if (last>=nFilesToSum) last = nFilesToSum;
    if (reportInfo(__FUNCTION__)) cout << "Summing files w/ index :" << first << " to : " << last-1 << endl;
    String outputFileName = histosExportFile;
    outputFileName += appendedString;
    outputFileName += "_";
    outputFileName += first;
    outputFileName += "TO";
    outputFileName += (last-1);

    TFile * inputFile;
    TFile * firstInputFile;
    sumEventProcessed = 0.0;
    nEventProcessed   = 0.0;

    HistogramCollection * collectionAvg;
    HistogramCollection * collection;
    String parameterNEexecutedTask("taskExecuted");
    int nInputFile = last - first+1;
    for (int iFile=first; iFile<last; iFile++)
      {
      String HistogramsImportFile = allFilesToSum[iFile];
      inputFile = openRootFile("", HistogramsImportFile, "READ");
      if (!inputFile || !isTaskOk()) return;
      if (reportInfo(__FUNCTION__)) cout << "Loading file: " << HistogramsImportFile << endl;
      nEventProcessed = readParameter(inputFile,parameterNEexecutedTask);
      if (!isTaskOk()) return;
      if (iFile==first)
        {
        firstInputFile = inputFile;
        collectionAvg  = new HistogramCollection("Sum",getSeverityLevel());
        collectionAvg->loadCollection(inputFile);
        //if (reportDebug (__FUNCTION__)) cout << "First Load completed."  << endl;
        sumEventProcessed = nEventProcessed;
        }
      else
        {
        collection = new HistogramCollection(HistogramsImportFile,getSeverityLevel());;
        collection->loadCollection(inputFile);
        collectionAvg->squareDifferenceCollection(*collection, double(sumEventProcessed), double(nEventProcessed), (iFile==(last-1)) ? nInputFile : -iFile);
        sumEventProcessed += nEventProcessed;
        delete collection;
        delete inputFile;
        }
      //if (reportInfo (__FUNCTION__)) cout << "iFile:" << iFile << "    " << parameterNEexecutedTask ,nEventProcessed << endl;
      }
    TFile * outputFile = openRootFile(HistogramsExportPath, outputFileName, "RECREATE");
    if (!isTaskOk()) return;
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << "Saving histgroams to" << endl;
      printItem("HistogramsExportPath",histosExportPath);
      printItem("HistogramsExportFile",histosExportFile);
      printItem("OutputFileName",      outputFileName);
      printItem("SumEventProcessed",   sumEventProcessed);
      }
    writeParameter(outputFile,parameterNEexecutedTask, sumEventProcessed);
    collectionAvg->exportHistograms(outputFile);
    outputFile->Close();
    delete collectionAvg;
    delete outputFile;
    delete firstInputFile;
    }  // end of group
  if (reportEnd(__FUNCTION__))
;
}

