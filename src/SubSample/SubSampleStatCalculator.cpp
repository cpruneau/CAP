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
nEventsProcessed(0),
sumEventsProcessed(0),
nEventsAccepted(nullptr),
sumEventsAccepted(nullptr)
{
  appendClassName("SubSampleStatCalculator");
}



void SubSampleStatCalculator::setDefaultConfiguration()
{
  String none  = "none";
  addParameter("HistogramsCreate",       true);
  addParameter("HistogramsImport",       true);
  addParameter("HistogramsExport",       true);
  addParameter("AppendedString",         TString("Sum"));
  addParameter("HistogramsForceRewrite", true);
  addParameter("DefaultGroupSize",       50);
  addParameter("nInputFile",             0);
  addParameter("HistogramsImportPath",     none);
  addParameter("HistogramsExportPath",    none);
  addParameter("MaximumDepth",           2);
  generateKeyValuePairs("IncludedPattern",none,20);
  generateKeyValuePairs("ExcludedPattern",none,20);
  generateKeyValuePairs("InputFile",none,100);
}

void SubSampleStatCalculator::configure()
{
  nEventFilters = 0;
  defaultGroupSize    = getValueInt(   "DefaultGroupSize");
  appendedString      = getValueString("AppendedString");
  histosImport        = getValueString("HistogramsImport");
  histosImportPath    = getValueString("HistogramsImportPath");
  histosExport        = getValueString("HistogramsExport");
  histosExportPath    = getValueString("HistogramsExportPath");
  histosExportFile    = getName();
  maximumDepth        = getValueInt(   "MaximumDepth");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("HistogramsImport",    histosImport);
    printItem("HistogramsImportPath",histosImportPath);
    printItem("HistogramsExport",    histosExport);
    printItem("HistogramsExportPath",histosExportPath);
    printItem("HistogramsExportPath",histosExportFile);
    printItem("DefaultGroupSize",    defaultGroupSize);
    printItem("AppendedString",      appendedString);
    printItem("MaximumDepth",        maximumDepth);
    cout << endl;
    }
}

void SubSampleStatCalculator::execute()
{
  if (reportInfo(__FUNCTION__)) cout << "Subsample analysis for task of type :" << getName() << endl;
  String none  = "none";
  VectorString  includePatterns = getSelectedValues("IncludedPattern",none);
  VectorString  excludePatterns = getSelectedValues("ExcludedPattern",none);
  if (includePatterns.size()>0)
    {
    if (includePatterns[0].Contains("Derived")) histosExportFile += "Derived";
    if (includePatterns[0].Contains("BalFct")) histosExportFile += "BalFct";
    }
  includePatterns.push_back(getName());
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("DefaultGroupSize",    defaultGroupSize);
    printItem("AppendedString",      appendedString);
    printItem("HistogramsImportPath",histosImportPath);
    printItem("HistogramsExportPath",histosExportPath);
    printItem("MaximumDepth",        maximumDepth);
    printItem("N included patterns", int(includePatterns.size()));
    for (unsigned int k=0;k<includePatterns.size();k++) printItem("Included",includePatterns[k]);
    printItem("N excluded patterns", int(excludePatterns.size()));
    for (unsigned int k=0;k<excludePatterns.size();k++) printItem("Excluded",excludePatterns[k]);
    }
  bool prependPath = true;
  bool verbose = true;
  VectorString  allFilesToSum = listFilesInDir(histosImportPath,includePatterns,excludePatterns, prependPath, verbose, maximumDepth,0);
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
    printItem("nFilesToSum",          nFilesToSum);
    printItem("DefaultGroupSize",     defaultGroupSize);
    printItem("Actual group size",    defaultGroupSize);
    printItem("nGroups",              nGroups);
    printItem("appendedString",       appendedString);
    printItem("HistogramsImportPath", histosImportPath);
    printItem("HistogramsExportPath", histosExportPath);
    printItem("HistogramsExportFile", histosExportFile);
    cout << endl;
    }
  postTaskOk();
  
  for (int iGroup =0; iGroup<nGroups; iGroup++  )
    {
    int first = iGroup*groupSize;
    int last  = (iGroup+1)*groupSize;
    if (last>=nFilesToSum) last = nFilesToSum;
    if (reportInfo(__FUNCTION__)) cout << "Summing files w/ index:" << first << " to " << last-1 << endl;
    String outputFileName = histosExportFile;
    outputFileName += appendedString;
    outputFileName += first;
    outputFileName += "TO";
    outputFileName += (last-1);
    sumEventsProcessed = 0.0;
    nEventsProcessed   = 0.0;
    HistogramCollection * collectionAvg;
    HistogramCollection * collection;
    String parameterName;

    int nInputFile = last - first+1;
    for (int iFile=first; iFile<last; iFile++)
      {
      String HistogramsImportFile = allFilesToSum[iFile];
      TFile & inputFile = openRootFile("", HistogramsImportFile, "READ");
      if (iFile==first)
        {
        collectionAvg  = new HistogramCollection("Sum",getSeverityLevel());
        collectionAvg->loadCollection(inputFile);
        parameterName      = "taskExecuted";
        nEventsProcessed   = readParameter(inputFile,parameterName);
        sumEventsProcessed = nEventsProcessed;
        parameterName      = "nEventFilters";
        nEventFilters      = readParameter(inputFile,parameterName);
        if (nEventFilters>0)
          {
          nEventsAccepted    = new long[nEventFilters];
          sumEventsAccepted  = new long[nEventFilters];
          for (int iFilter=0; iFilter<nEventFilters; iFilter++)
            {
            parameterName = "EventFilter";
            parameterName += iFilter;
            nEventsAccepted[iFilter]   = readParameter(inputFile,parameterName);
            sumEventsAccepted[iFilter] = nEventsAccepted[iFilter];
            }
          }
        else
          {
          if (reportWarning(__FUNCTION__)) cout << "nEventFilters is null" << endl;
          }
        }
      else
        {
        collection = new HistogramCollection(HistogramsImportFile,getSeverityLevel());;
        collection->loadCollection(inputFile);
        collectionAvg->squareDifferenceCollection(*collection, double(sumEventsProcessed), double(nEventsProcessed), (iFile==(last-1)) ? nInputFile : -iFile);
        parameterName      = "taskExecuted";
        nEventsProcessed   = readParameter(inputFile,parameterName);
        sumEventsProcessed += nEventsProcessed;
        if (nEventFilters>0)
          {
          for (int iFilter=0; iFilter<nEventFilters; iFilter++)
            {
            parameterName = "EventFilter";
            parameterName += iFilter;
            nEventsAccepted[iFilter] = readParameter(inputFile,parameterName);
            sumEventsAccepted[iFilter] += nEventsAccepted[iFilter];
            }
          }
        else
          {
          if (reportWarning(__FUNCTION__)) cout << "nEventFilters is null" << endl;
          }
        delete collection;
        }
      if (reportInfo(__FUNCTION__))
        {
        cout << endl;
        printItem("File index",            iFile);
        printItem("HistosImportFile",      histosImportFile);
        printItem("nEventsProcessed",      nEventsProcessed);
        printItem("nEventsProcessed(Sum)", sumEventsProcessed);
        cout << endl;
        }
      }
    TFile & outputFile = openRootFile(histosExportPath, outputFileName, "RECREATE");
    parameterName    = "taskExecuted";
    writeParameter(outputFile,parameterName, sumEventsProcessed);
    parameterName    = "nEventFilters";
    writeParameter(outputFile,parameterName, nEventFilters);
    if (nEventFilters>0)
      {
      for (int iFilter=0; iFilter<nEventFilters; iFilter++)
        {
        parameterName = "EventFilter";
        parameterName += iFilter;
        writeParameter(outputFile,parameterName,sumEventsAccepted[iFilter]);
        }
      }

    collectionAvg->exportHistograms(outputFile);
    outputFile.Close();
    delete collectionAvg;
    }
  if (reportEnd(__FUNCTION__))
;
}

