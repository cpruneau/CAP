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

ClosureCalculator::ClosureCalculator(const TString & _name,
                                     Configuration & _configuration)
:
Task(_name, _configuration)
{
  appendClassName("ClosureCalculator");
}

void ClosureCalculator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  setParameter("CreateHistograms",        true);
  setParameter("SaveHistograms",          true);
  //setParameter("ForceHistogramsRewrite",  true);
  addParameter("SelectedMethod",          0);
  addParameter("HistogramInputPath",          TString("./"));
  addParameter("HistoGeneratorFileName",  TString("HistoGeneratorFileName"));
  addParameter("HistoDetectorFileName",   TString("HistoDetectorFileName"));
  addParameter("HistogramOutputPath",         TString("./"));
  addParameter("HistoClosureFileName",    TString("HistoClosureFileName"));
}

void ClosureCalculator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  bool    ForceHistogramsRewrite = configuration.getValueBool("ForceHistogramsRewrite");
  int     selectedMethod         = configuration.getValueInt("SelectedMethod");
  TString histoInputPath         = getValueString("HistogramInputPath");
  TString histoGeneratorFileName = getValueString("HistoGeneratorFileName");
  TString histoDetectorFileName  = getValueString("HistoDetectorFileName");
  TString HistogramOutputPath    = getValueString("HistogramOutputPath");
  TString histoClosureFileName   = getValueString("HistoClosureFileName");

  if (reportInfo(__FUNCTION__))
    {
    cout
    << endl
    << "   Starting closure test calculation for :" << endl
    << "           histoInputPath: " << histoInputPath  << endl
    << "   histoGeneratorFileName: " << histoGeneratorFileName << endl
    << "    histoDetectorFileName: " << histoDetectorFileName << endl
    << "      HistogramOutputPath: " << HistogramOutputPath  << endl
    << "     histoClosureFileName: " << histoClosureFileName  << endl;
    switch (selectedMethod)
      {
        case 0: cout << "           selectedMethod: Difference" << endl; break;
        case 1: cout << "           selectedMethod: Ratio" << endl; break;
      }
    }

  TFile * generatorFile = openRootFile("", histoGeneratorFileName, "READ");
  TFile * detectorFile  = openRootFile("", histoDetectorFileName,  "READ");
  TFile * closureFile;
  if (ForceHistogramsRewrite)
    closureFile   = openRootFile("", histoClosureFileName,"RECREATE");
  else
    closureFile   = openRootFile("", histoClosureFileName,"NEW");

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

