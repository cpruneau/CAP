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
#include "TParameter.h"

ClassImp(SubSampleStatCalculator);


SubSampleStatCalculator::SubSampleStatCalculator(const TString &       _name,
                                                 const Configuration & _configuration,
                                                 MessageLogger::LogLevel debugLevel)
:
Task(_name,_configuration,debugLevel),
nEventProcessed(0),
sumEventProcessed(0),
nEventAccepted(0),
sumEventAccepted(0)
{
  appendClassName("SubSampleStatCalculator");
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void SubSampleStatCalculator::setDefaultConfiguration()
{
  if (reportStart("SubSampleStatCalculator",getName(),"setDefaultConfiguration()"))
    { }
  configuration.setParameter( "createHistograms",       true);
  configuration.setParameter( "loadHistograms",         true);
  configuration.setParameter( "saveHistograms",         true);
  configuration.setParameter( "forceHistogramsRewrite", true);
  configuration.addParameter( "defaultGroupSize",         10);
  configuration.addParameter( "nInputFile",                0);
  TString keyInputFile("InputFile");
  TString keyIncluded("IncludedPattern");
  TString keyExcluded("ExcludedPattern");
  TString key;
  TString value("none");
  for (int k=0; k<100; k++)
    {
    key = keyInputFile; key += k;
    configuration.addParameter(key,value);
    }
  for (int k=0; k<20; k++)
    {
    key = keyIncluded; key += k;
    configuration.addParameter(key, value);
    }
  for (int k=0; k<20; k++)
    {
    key = keyExcluded; key += k;
    configuration.addParameter(key, value);
    }
  if (reportDebug("FileTaskIterator",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
  if (reportEnd("SubSampleStatCalculator",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}


void SubSampleStatCalculator::execute()
{
  if (reportInfo("SubSampleStatCalculator",getName(),"execute()"))
    {
    }
  Configuration & configuration = getConfiguration();
  int     nInputFile          = configuration.getValueInt("nInputFile");
  TString histoInputPath      = configuration.getValueString( "histoInputPath");
  TString histoInputFileName  = "";
  TString histoOutputPath     = configuration.getValueString( "histoOutputPath");
  TString histoOutputFileName = configuration.getValueString( "histoOutputFileName");
  
  TFile * inputFile;
  TFile * firstInputFile;
  sumEventProcessed = 0.0;
  nEventProcessed   = 0.0;
  sumEventAccepted  = 0.0;
  nEventAccepted    = 0.0;
  
  HistogramCollection * collectionAvg = new HistogramCollection("Sum",getReportLevel());
  HistogramCollection * collection;
  TString parameterEventProcessed("EventProcessed");
  TString parameterEventAccepted("EventAccepted");
  TString keyBase("InputFile");
  for (int iFile=0; iFile<nInputFile; iFile++)
    {
    TString key = keyBase; key += iFile;
    histoInputFileName = configuration.getValueString(key);
    inputFile = openRootFile(histoInputPath, histoInputFileName, "READ");
    if (!isTaskOk()) return;
    nEventProcessed = readParameter(inputFile,parameterEventProcessed);
    nEventAccepted  = readParameter(inputFile,parameterEventAccepted);
    if (!isTaskOk()) return;
    if (reportInfo ("SubSampleStatCalculator",getName(),"execute()")) cout << "Loading histograms" << endl;
    if (iFile==0)
      {
      firstInputFile = inputFile;
      collectionAvg->loadCollection(inputFile);
      if (reportDebug ("SubSampleStatCalculator",getName(),"execute()")) cout << "First Load completed."  << endl;
      sumEventProcessed = nEventProcessed;
      sumEventAccepted  = nEventAccepted;
      if (reportInfo ("SubSampleStatCalculator",getName(),"execute()"))
        {
        cout << endl;
        cout << "iFile:" << iFile << endl;
        cout << "    " << parameterEventProcessed << " : " << sumEventProcessed << endl;
        cout << "    " << parameterEventAccepted << " : " << sumEventAccepted << endl;
        }
      }
    else
      {
      collection = new HistogramCollection(histoInputFileName,getReportLevel());;
      collection->loadCollection(inputFile);
      collectionAvg->squareDifferenceCollection(*collection, double(sumEventProcessed), double(nEventProcessed), (iFile==nInputFile) ? nInputFile-1 : -iFile);
      sumEventProcessed += nEventProcessed;
      sumEventAccepted  += nEventAccepted;
      delete collection;
      delete inputFile;
      if (reportInfo ("SubSampleStatCalculator",getName(),"execute()"))
        {
        cout << endl;
        cout << "iFile:" << iFile << endl;
        cout << "input file name: " << histoInputFileName << endl;
        cout << "    " << parameterEventProcessed << " : " << sumEventProcessed << endl;
        cout << "    " << parameterEventAccepted << " : " << sumEventAccepted << endl;
        }
      }
    }
  if (reportInfo ("SubSampleStatCalculator",getName(),"execute()"))
    {
    cout << endl;
    cout << "      histoOutputPath:" << histoOutputPath << endl;
    cout << "  histoOutputFileName:" << histoOutputFileName << endl;
    }
  TFile * outputFile = openRootFile(histoOutputPath, histoOutputFileName, "RECREATE");
  if (!isTaskOk()) return;
  if (reportInfo ("SubSampleStatCalculator",getName(),"execute()"))
    {
    cout << endl;
    cout << "    Saving histgroams:" <<  endl;
    }
  collectionAvg->saveHistograms(outputFile);
  writeParameter(outputFile,parameterEventProcessed, sumEventProcessed);
  writeParameter(outputFile,parameterEventAccepted,  sumEventAccepted);
  
  delete collectionAvg;
  delete outputFile;
  delete firstInputFile;
  if (reportDebug ("SubSampleStatCalculator",getName(),"execute()")) cout << "Subsample analysis completed." << endl;
}

void SubSampleStatCalculator::run()
{
  if (reportInfo("SubSampleStatCalculator",getName(),"run()"))
    {
    }
  Configuration & configuration = getConfiguration();
  int defaultGroupSize    = configuration.getValueInt("defaultGroupSize");
  TString appendedString  = configuration.getValueString("appendedString");
  TString histoInputPath  = configuration.getValueString("histoInputPath");
  TString histoOutputPath = configuration.getValueString("histoOutputPath");
  TString histoBaseName   = configuration.getValueString("histoBaseName");
  vector<TString> includePatterns;
  vector<TString> excludePatterns;
  for (int k=0; k<20; k++)
    {
    TString key = "IncludedPattern"; key += k;
    TString  value = configuration.getValueString(key);
    if (!value.Contains("none")) includePatterns.push_back(value);
    }
  for (int k=0; k<20; k++)
    {
    TString key = "ExcludedPattern"; key += k;
    TString  value = configuration.getValueString(key);
    if (!value.Contains("none")) excludePatterns.push_back(value);
    }
  vector<TString> filesToSum = listFilesInDir(histoInputPath,includePatterns,excludePatterns);
  int nFilesToSum = filesToSum.size();
  int groupSize = (nFilesToSum>defaultGroupSize) ? defaultGroupSize : nFilesToSum;
  int nGroups   = 1 + double(nFilesToSum-1)/double(groupSize);

  if (nFilesToSum<1)
    {
    if (reportError("SubSampleStatCalculator",getName(),"run()"))
      {
      cout << "Attempting to execute subsample analysis with no selected files." << endl;
      cout << "====> Check your code! " <<  endl;
      }
    return;
    }
  if (reportInfo("SubSampleStatCalculator",getName(),"run()"))
    {
    cout << " ===========================================================" << endl;
    cout << " ===========================================================" << endl;
    cout << "        nFilesToSum: " << nFilesToSum << endl;
    cout << " Default group size: " << defaultGroupSize << endl;
    cout << "  Actual group size: " << groupSize << endl;
    cout << "            nGroups: " << nGroups << endl;
    cout << " ===========================================================" << endl;
    cout << " ===========================================================" << endl;
    }
  postTaskOk();
  
  for (int iGroup =0; iGroup<nGroups; iGroup++  )
    {
    int first = iGroup*groupSize;
    int last  = (iGroup+1)*groupSize;
    if (last>=nFilesToSum) last = nFilesToSum-1;
    cout << "Summing files from index:" << first << " to index: " << last << endl;
    TString outputFileName = histoBaseName;
    outputFileName += appendedString;
    outputFileName += first;
    outputFileName += "TO";
    outputFileName += last;
    configuration.setParameter( "nInputFile",          groupSize);
    configuration.setParameter( "histoInputPath",      TString("")      );
    configuration.setParameter( "histoOutputPath",     histoOutputPath     );
    configuration.setParameter( "histoOutputFileName", outputFileName      );
    TString keyInputFile("InputFile");
    TString key;
    TString value("none");
    for (int k=0; k<100; k++)
      {
      key = keyInputFile; key += k;
      configuration.setParameter(key,value);
      }
    for (int k=first; k<=last; k++  )
      {
      TString key("InputFile"); key += k;
      TString name = filesToSum[k];
      cout << "   k: " << k << "    " << name << endl;
      configuration.setParameter(key,name);
      }
    execute();
    }
  if (reportDebug ("SubSampleStatCalculator",getName(),"run()")) cout << "Subsample analysis completed." << endl;
}
