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

ClassImp(SubSampleStatCalculator);


SubSampleStatCalculator::SubSampleStatCalculator(const TString & _name,
                                                 Configuration & _configuration)
:
Task(_name,_configuration),
nEventProcessed(0),
sumEventProcessed(0)
{
  appendClassName("SubSampleStatCalculator");
}



void SubSampleStatCalculator::setDefaultConfiguration()
{
  TString none  = "none";
  setParameter("CreateHistograms",       true);
  setParameter("LoadHistograms",         true);
  setParameter("SaveHistograms",         true);
  setParameter("AppendedString",         TString("_Sum"));
  setParameter("ForceHistogramsRewrite", true);
  addParameter("DefaultGroupSize",       10);
  addParameter("nInputFile",             0);
  generateKeyValuePairs("IncludedPattern",none,20);
  generateKeyValuePairs("ExcludedPattern",none,20);
  generateKeyValuePairs("InputFile",      none,100);
}

void SubSampleStatCalculator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  int defaultGroupSize            = getValueInt("DefaultGroupSize");
  TString appendedString          = getValueString("AppendedString");
  TString histogramInputPath      = getValueString("HistogramInputPath");
  TString histogramOutputPath     = getValueString("HistogramOutputPath");
  TString histogramOutputFile     = getValueString("HistogramOutputFile");
  vector<TString> includePatterns = getSelectedValues("IncludedPattern", "none");
  vector<TString> excludePatterns = getSelectedValues("ExcludedPattern", "none");
  bool prependPath = true;
  bool verbose     = true;
  int  maximumDepth = 2;
  vector<TString> allFilesToSum   = listFilesInDir(histoInputPath,includePatterns,excludePatterns,prependPath, verbose, maximumDepth);
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
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << " ==========================================================================================" << endl;
    cout << " nFilesToSum................: " << nFilesToSum << endl;
    cout << " Default group size.........: " << defaultGroupSize << endl;
    cout << " Actual group size..........: " << groupSize << endl;
    cout << " nGroups....................: " << nGroups << endl;
    cout << " appendedString.............: " << appendedString << endl;
    cout << " HistogramInputPath.........: " << histogramInputPath << endl;
    cout << " HistogramOutputPath........: " << histogramOutputPath << endl;
    cout << " HistogramOutputFile........: " << histogramOutputFile << endl;
    cout << " ==========================================================================================" << endl;
    }
  postTaskOk();
  
  for (int iGroup =0; iGroup<nGroups; iGroup++  )
    {
    int first = iGroup*groupSize;
    int last  = (iGroup+1)*groupSize;
    if (last>=nFilesToSum) last = nFilesToSum;
    if (reportInfo(__FUNCTION__)) cout << "Summing files w/ index :" << first << " to : " << last-1 << endl;
    TString outputFileName = histogramOutputFile;
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
    TString parameterNEexecutedTask("nTaskExecuted");
    int nInputFile = last - first+1;
    for (int iFile=first; iFile<last; iFile++)
      {
      TString HistogramInputFile = allFilesToSum[iFile];
      inputFile = openRootFile("", HistogramInputFile, "READ");
      if (!inputFile || !isTaskOk()) return;
      if (reportInfo(__FUNCTION__)) cout << "Loading file: " << HistogramInputFile << endl;
      nEventProcessed = readParameter(inputFile,parameterNEexecutedTask);
      if (!isTaskOk()) return;
      if (iFile==first)
        {
        firstInputFile = inputFile;
        collectionAvg  = new HistogramCollection("Sum",getReportLevel());
        collectionAvg->loadCollection(inputFile);
        //if (reportDebug (__FUNCTION__)) cout << "First Load completed."  << endl;
        sumEventProcessed = nEventProcessed;
        }
      else
        {
        collection = new HistogramCollection(HistogramInputFile,getReportLevel());;
        collection->loadCollection(inputFile);
        collectionAvg->squareDifferenceCollection(*collection, double(sumEventProcessed), double(nEventProcessed), (iFile==(last-1)) ? nInputFile : -iFile);
        sumEventProcessed += nEventProcessed;
        delete collection;
        delete inputFile;
        }
      //if (reportInfo (__FUNCTION__)) cout << "iFile:" << iFile << "    " << parameterNEexecutedTask << " : " << nEventProcessed << endl;
      }
    TFile * outputFile = openRootFile(HistogramOutputPath, outputFileName, "RECREATE");
    if (!isTaskOk()) return;
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << "Saving histgroams to" << endl;
      cout << "HistogramOutputPath...............: " << histogramOutputPath << endl;
      cout << "HistogramOutputFile...............: " << histogramOutputFile << endl;
      cout << "OutputFileName....................: " << outputFileName      << endl;
      cout << "SumEventProcessed.................: " << sumEventProcessed   << endl;
      }
    writeParameter(outputFile,parameterNEexecutedTask, sumEventProcessed);
    collectionAvg->saveHistograms(outputFile);
    outputFile->Close();
    delete collectionAvg;
    delete outputFile;
    delete firstInputFile;
    }  // end of group
  if (reportEnd(__FUNCTION__))
    ;
}

