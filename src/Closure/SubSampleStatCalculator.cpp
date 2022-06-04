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


SubSampleStatCalculator::SubSampleStatCalculator(const TString &       _name,
                                                 const Configuration & _configuration,
                                                 MessageLogger::LogLevel debugLevel)
:
Task(_name,_configuration,debugLevel),
nEventProcessed(0),
sumEventProcessed(0)
{
  appendClassName("SubSampleStatCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}



void SubSampleStatCalculator::setDefaultConfiguration()
{
  if (reportStart(__FUNCTION__))
    ;
  TString none  = "none";
  configuration.setParameter("createHistograms",       true);
  configuration.setParameter("loadHistograms",         true);
  configuration.setParameter("saveHistograms",         true);
  configuration.setParameter("appendedString",         TString("_Sum"));
  configuration.setParameter("forceHistogramsRewrite", true);
  configuration.addParameter("defaultGroupSize",       10);
  configuration.addParameter("nInputFile",             0);
  configuration.addParameter("histoModelDataName",     "DataSource");
  configuration.addParameter("histoAnalyzerName",      TString("Analyzer"));
  configuration.generateKeyValuePairs("IncludedPattern",none,20);
  configuration.generateKeyValuePairs("ExcludedPattern",none,20);
  configuration.generateKeyValuePairs("InputFile",      none,100);
 // // if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

void SubSampleStatCalculator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  //incrementTaskExecuted();
  int defaultGroupSize       = configuration.getValueInt("defaultGroupSize");
  TString appendedString     = configuration.getValueString("appendedString");
  TString histoInputPath     = configuration.getValueString("histoInputPath");
  TString histoOutputPath    = configuration.getValueString("histoOutputPath");
  TString histoModelDataName = configuration.getValueString("histoModelDataName");
  TString histoAnalyzerName  = configuration.getValueString("histoAnalyzerName");
  TString histoOutputFileName;
  histoOutputFileName = histoModelDataName;
  histoOutputFileName += "_";
  histoOutputFileName += histoAnalyzerName;
  vector<TString> includePatterns = configuration.getSelectedValues("IncludedPattern", "none");
  vector<TString> excludePatterns = configuration.getSelectedValues("ExcludedPattern", "none");

//  if (reportDebug(__FUNCTION__))
//    {
//    for (unsigned int k=0;k<includePatterns.size();k++)
//      {
//      cout << " k:" << k << "  Include: " << includePatterns[k] << endl;
//      }
//
//    for (unsigned int k=0;k<excludePatterns.size();k++)
//      {
//      cout << " k:" << k << "  Exclude: " << excludePatterns[k] << endl;
//      }
//    }

  vector<TString> allFilesToSum = listFilesInDir(histoInputPath,includePatterns,excludePatterns);
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
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << " ==========================================================================================" << endl;
    cout << " ==========================================================================================" << endl;
    cout << "              nFilesToSum: " << nFilesToSum << endl;
    cout << "       Default group size: " << defaultGroupSize << endl;
    cout << "        Actual group size: " << groupSize << endl;
    cout << "                  nGroups: " << nGroups << endl;
    cout << "           appendedString: " << appendedString << endl;
    cout << "           histoInputPath: " << histoInputPath << endl;
    cout << "          histoOutputPath: " << histoOutputPath << endl;
    cout << "       histoModelDataName: " << histoModelDataName << endl;
    cout << "        histoAnalyzerName: " << histoAnalyzerName << endl;
    cout << "      histoOutputFileName: " << histoOutputFileName << endl;
    cout << " ==========================================================================================" << endl;
    cout << " ==========================================================================================" << endl;
    }
  postTaskOk();
  
  for (int iGroup =0; iGroup<nGroups; iGroup++  )
    {
    int first = iGroup*groupSize;
    int last  = (iGroup+1)*groupSize;
    if (last>=nFilesToSum) last = nFilesToSum;
    if (reportInfo(__FUNCTION__)) cout << "Summing files w/ index :" << first << " to : " << last-1 << endl;
    TString outputFileName = histoOutputFileName;
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
      TString histoInputFileName = allFilesToSum[iFile];
      inputFile = openRootFile("", histoInputFileName, "READ");
      if (!inputFile || !isTaskOk()) return;
      if (reportInfo(__FUNCTION__)) cout << "Loading file: " << histoInputFileName << endl;
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
        collection = new HistogramCollection(histoInputFileName,getReportLevel());;
        collection->loadCollection(inputFile);
        collectionAvg->squareDifferenceCollection(*collection, double(sumEventProcessed), double(nEventProcessed), (iFile==(last-1)) ? nInputFile : -iFile);
        sumEventProcessed += nEventProcessed;
        delete collection;
        delete inputFile;
        }
      //if (reportInfo (__FUNCTION__)) cout << "iFile:" << iFile << "    " << parameterNEexecutedTask << " : " << nEventProcessed << endl;
      }
    TFile * outputFile = openRootFile(histoOutputPath, outputFileName, "RECREATE");
    if (!isTaskOk()) return;
    if (reportDebug(__FUNCTION__))
      {
      cout << endl;
      cout << "    Saving histgroams to : " << endl;
      cout << "         histoOutputPath : " << histoOutputPath <<  endl;
      cout << "     histoOutputFileName : " << histoOutputFileName << endl;
      cout << "          outputFileName : " << outputFileName <<  endl;
      cout << "       sumEventProcessed : " << sumEventProcessed << endl;
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

