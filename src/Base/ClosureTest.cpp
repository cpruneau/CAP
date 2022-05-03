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
#include "ClosureTest.hpp"

ClassImp(ClosureTest);

TString inputPath;
TString outputPath;
TString generatorLevelFileName;  // Truth
TString detectorLevelFileName;   // Measured
TString outputFileName;          // Comparison
ClosureTestMethod selectedMethod;

ClosureTest::ClosureTest(const TString &   _inputPath,
                         const TString &   _generatorLevel,
                         const TString &   _detectorLevel,
                         const TString &   _outputPath,
                         const TString &   _closureFileName,
                         ClosureTestMethod _selectedMethod,
                         MessageLogger::LogLevel debugLevel)
:
Task(),
inputPath(_inputPath),
outputPath(_outputPath),
generatorLevelFileName(_generatorLevel),
detectorLevelFileName(_detectorLevel),
closureFileName(_closureFileName),
selectedMethod(_selectedMethod)
{
  setReportLevel(debugLevel);
}

void ClosureTest::setDefaultConfiguration()
{
  if (reportStart("ParticleAnalyzer",getName(),"setDefaultConfiguration()"))
    {
    }
  Configuration & configuration = getConfiguration();
  configuration.setName("ClosureTest Configuration");
  configuration.setParameter("useParticles",true);
  configuration.setParameter("createHistograms",true);
  configuration.setParameter("saveHistograms",true);
   
  if (reportDebug("ParticleAnalyzer",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}



void ClosureTest::execute()
{
  if (reportInfo("ClosureTest",getName(),"execute()"))
    {
    cout
    << "   Starting closure test:" << endl
    << "               inputPath:" << inputPath  << endl
    << "              outputPath:" << outputPath << endl
    << "  generatorLevelFileName:" << generatorLevelFileName << endl
    << "   detectorLevelFileName:" << detectorLevelFileName  << endl;
    }


  TFile * generatorFile = openRootFile(inputPath, generatorLevelFileName, "READ");
  TFile * detectorFile  = openRootFile(inputPath, detectorLevelFileName,  "READ");
  TFile * closureFile;

  if (!generatorFile)
    {
    if (reportError("ClosureTest",getName(),"execute()"))
      {
      cout << "Generator File Name: " << generatorLevelFileName << " NOT FOUND" << endl;
      }
    setTaskError();
    return;
    }
  if (!detectorFile)
    {
    if (reportError("ClosureTest",getName(),"execute()"))
      {
      cout << "Detector File Name: " << detectorFile << " NOT FOUND" << endl;
      }
    setTaskError();
    return;
    }

  HistogramCollection * generatorCollection = new HistogramCollection("GeneratorLevel",getReportLevel());
  HistogramCollection * detectorCollection  = new HistogramCollection("DetectorLevel", getReportLevel());
  HistogramCollection * closureCollection   = new HistogramCollection("Closure",       getReportLevel());

  generatorCollection->loadCollection(generatorFile);
  detectorCollection ->loadCollection(detectorFile);

  closureCollection->ratio(*detectorCollection, *generatorCollection);
  TFile * outputFile = openRootFile(outputPath, closureFileName, "RECREATE");
  if (!isTaskOk()) return;
  closureCollection->saveHistograms(outputFile);
  delete generatorCollection;
  delete detectorCollection;
  delete closureCollection;
  if (reportInfo ("ClosureTest",getName(),"execute()")) cout << "Closure Test Completed." << endl;
}

