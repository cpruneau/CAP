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
//nEventAccepted(0),
//sumEventAccepted(0)
{
  appendClassName("SubSampleStatCalculator");
}



void SubSampleStatCalculator::setDefaultConfiguration()
{
  TString none  = "none";
  addParameter("CreateHistograms",       true);
  addParameter("LoadHistograms",         true);
  addParameter("SaveHistograms",         true);
  addParameter("AppendedString",         TString("Sum"));
  setParameter("ForceHistogramsRewrite", true);
  addParameter("DefaultGroupSize",       10);
  addParameter("nInputFile",             0);
  addParameter("HistogramInputPath",     none);
  addParameter("HistogramOutputPath",    none);
  generateKeyValuePairs("IncludedPattern",none,20);
  generateKeyValuePairs("ExcludedPattern",none,20);
  generateKeyValuePairs("InputFile",none,100);
}

void SubSampleStatCalculator::execute()
{
  if (reportInfo(__FUNCTION__)) cout << "Subsample analysis for task of type :" << taskName << endl;
  TString none  = "none";
  configuration.printConfiguration(cout);

  int defaultGroupSize        = getValueInt("DefaultGroupSize");
  TString appendedString      = getValueString("AppendedString");
  TString histogramInputPath  = getValueString("HistogramInputPath");
  TString histogramOutputPath = getValueString("HistogramOutputPath");
  vector<TString> includePatterns = getSelectedValues("IncludedPattern",none);
  vector<TString> excludePatterns = getSelectedValues("ExcludedPattern",none);
  TString histogramOutputFile = taskName;
  if (includePatterns.size()>0)
    {
    if (includePatterns[0].Contains("Derived")) histogramOutputFile += "Derived";
    if (includePatterns[0].Contains("BalFct")) histogramOutputFile += "BalFct";
    }

  includePatterns.push_back(taskName);
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " DefaultGroupSize..........: " << defaultGroupSize << endl;
    cout << " AppendedString............: " << appendedString << endl;
    cout << " HistogramInputPath........: " << histogramInputPath << endl;
    cout << " HistogramOutputPath.......: " << histogramOutputPath << endl;
    cout << " N included patterns.......: " << includePatterns.size() << endl;
    for (unsigned int k=0;k<includePatterns.size();k++) cout << " Included..................:" <<   includePatterns[k] << endl;
    cout << " N excluded patterns.......: " << excludePatterns.size() << endl;
    for (unsigned int k=0;k<excludePatterns.size();k++) cout << " Excluded..................:" <<   excludePatterns[k] << endl;
    }
  bool prependPath = true;
  bool verbose = true;
  int  maximumDepth = 2;
  vector<TString> allFilesToSum = listFilesInDir(histogramInputPath,includePatterns,excludePatterns, prependPath, verbose, maximumDepth);
  int nFilesToSum = allFilesToSum.size();
  int groupSize = (nFilesToSum>defaultGroupSize) ? defaultGroupSize : nFilesToSum;
  int nGroups   = 1 + double(nFilesToSum-1)/double(groupSize);
  if (nFilesToSum<1)
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;
      cout << "========================================================================"  << endl;
      cout << "========================================================================"  << endl;
      cout << "  Attempting to execute subsample analysis with no selected files." << endl;
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
    cout << " nFilesToSum...................: " << nFilesToSum << endl;
    cout << " Default group size............: " << defaultGroupSize << endl;
    cout << " Actual group size.............: " << groupSize << endl;
    cout << " nGroups.......................: " << nGroups << endl;
    cout << " appendedString................: " << appendedString << endl;
    cout << " histogramInputPath............: " << histogramInputPath << endl;
    cout << " histogramOutputPath...........: " << histogramOutputPath << endl;
    cout << " histogramOutputFile...........: " << histogramOutputFile << endl;
    cout << " ===========================================================" << endl;
    }
  postTaskOk();
  
  for (int iGroup =0; iGroup<nGroups; iGroup++  )
    {
    int first = iGroup*groupSize;
    int last  = (iGroup+1)*groupSize;
    if (last>=nFilesToSum) last = nFilesToSum;
    if (reportInfo(__FUNCTION__)) cout << "Summing files w/ index:" << first << " to " << last-1 << endl;
    TString outputFileName = histogramOutputFile;
    outputFileName += appendedString;
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
      TString histogramInputFile = allFilesToSum[iFile];
      inputFile = openRootFile("", histogramInputFile, "READ");
      if (!inputFile || !isTaskOk()) return;
      nEventProcessed = readParameter(inputFile,parameterNEexecutedTask);
      if (!isTaskOk()) return;
      if (iFile==first)
        {
        firstInputFile = inputFile;
        collectionAvg  = new HistogramCollection("Sum",getReportLevel());
        collectionAvg->loadCollection(inputFile);
        sumEventProcessed = nEventProcessed;
        }
      else
        {
        collection = new HistogramCollection(histogramInputFile,getReportLevel());;
        collection->loadCollection(inputFile);
        collectionAvg->squareDifferenceCollection(*collection, double(sumEventProcessed), double(nEventProcessed), (iFile==(last-1)) ? nInputFile : -iFile);
        sumEventProcessed += nEventProcessed;
        delete collection;
        delete inputFile;
        }
      if (reportInfo (__FUNCTION__)) cout << "File:" << iFile << " " << histogramInputFile << " Events: "  << nEventProcessed << " : " << sumEventProcessed << endl;
      }
    TFile * outputFile = openRootFile(histogramOutputPath, outputFileName, "RECREATE");
    if (!isTaskOk()) return;
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << " Saving histgroams: " << endl;
      cout << " histogramOutputPath...........: " << histogramOutputPath << endl;
      cout << " histogramOutputFile...........: " << histogramOutputFile << endl;
      cout << " outputFileName................: " << outputFileName <<  endl;
      cout << " sumEventProcessed.............: " << sumEventProcessed << endl;
      }
    writeParameter(outputFile,parameterNEexecutedTask, sumEventProcessed);
    collectionAvg->saveHistograms(outputFile);
    outputFile->Close();
    delete collectionAvg;
    delete outputFile;
    delete firstInputFile;
    }
  if (reportEnd(__FUNCTION__))
    ;
}

