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
#include "ClosureCalculator.hpp"

ClassImp(ClosureCalculator);

ClosureCalculator::ClosureCalculator(const TString &          _name,
                         const Configuration &    _configuration,
                         MessageLogger::LogLevel  _reportLevel)
:
Task(_name, _configuration, _reportLevel)
{
  appendClassName("ClosureCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void ClosureCalculator::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  configuration.setName("ClosureCalculator Configuration");
  configuration.setParameter("createHistograms",      true);
  configuration.setParameter("saveHistograms",        true);
  configuration.setParameter("forceHistogramsRewrite",true);
  configuration.addParameter("selectedMethod",          0);
  configuration.addParameter("histoInputPath",          TString("./"));
  configuration.addParameter("histoGeneratorFileName",  TString("histoGeneratorFileName"));
  configuration.addParameter("histoDetectorFileName",   TString("histoDetectorFileName"));
  configuration.addParameter("histoOutputPath",         TString("./"));
  configuration.addParameter("histoClosureFileName",    TString("histoClosureFileName"));
  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

void ClosureCalculator::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  bool    forceHistogramsRewrite = configuration.getValueBool("forceHistogramsRewrite");
  int     selectedMethod         = configuration.getValueInt("selectedMethod");
  TString histoInputPath         = configuration.getValueString("histoInputPath");
  TString histoGeneratorFileName = configuration.getValueString("histoGeneratorFileName");
  TString histoDetectorFileName  = configuration.getValueString("histoDetectorFileName");
  TString histoOutputPath        = configuration.getValueString("histoOutputPath");
  TString histoClosureFileName   = configuration.getValueString("histoClosureFileName");
    
  if (reportInfo(__FUNCTION__))
    {
    cout
    << endl
    << "   Starting closure test calculation for :" << endl
    << "           histoInputPath: " << histoInputPath  << endl
    << "   histoGeneratorFileName: " << histoGeneratorFileName << endl
    << "    histoDetectorFileName: " << histoDetectorFileName << endl
    << "          histoOutputPath: " << histoOutputPath  << endl
    << "     histoClosureFileName: " << histoClosureFileName  << endl;
    switch (selectedMethod)
      {
        case 0: cout << "           selectedMethod: Difference" << endl; break;
        case 1: cout << "           selectedMethod: Ratio" << endl; break;
      }
    }

  TFile * generatorFile = openRootFile(histoInputPath, histoGeneratorFileName, "READ");
  TFile * detectorFile  = openRootFile(histoInputPath, histoDetectorFileName,  "READ");
  TFile * closureFile;
  if (forceHistogramsRewrite)
    closureFile   = openRootFile(histoOutputPath, histoClosureFileName,"RECREATE");
  else
    closureFile   = openRootFile(histoOutputPath, histoClosureFileName,"NEW");

  if (!generatorFile || !detectorFile || !closureFile || !isTaskOk()) return;
  
  HistogramCollection * generatorCollection = new HistogramCollection("GeneratorLevel",getReportLevel());
  HistogramCollection * detectorCollection  = new HistogramCollection("DetectorLevel", getReportLevel());
  HistogramCollection * closureCollection   = new HistogramCollection("Closure",       getReportLevel());
  generatorCollection->loadCollection(generatorFile);
  detectorCollection->loadCollection(detectorFile);
  generatorCollection->setOwnership(false);
  detectorCollection->setOwnership(false);
  closureCollection->setOwnership(false);
  switch (selectedMethod)
  {
    case 0: closureCollection->differenceCollection(*detectorCollection,*generatorCollection,true); break;
    case 1: closureCollection->ratioCollection(*detectorCollection,*generatorCollection,true); break;
  }

  //if (!isTaskOk()) return;
  closureCollection->saveHistograms(closureFile);
  generatorFile->Close();
  detectorFile->Close();
  closureFile->Close();
  delete generatorCollection;
  delete detectorCollection;
  delete closureCollection;
  if (reportInfo ()) cout << "Closure Test Completed." << endl;
}

