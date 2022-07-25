/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   07/17/2022
 *
 * *********************************************************************/
#include "CalibrationProducer.hpp"

ClassImp(CalibrationProducer);

CalibrationProducer::CalibrationProducer(const TString &          _name,
                                         const Configuration &    _configuration,
                                         MessageLogger::LogLevel  _reportLevel)
:
Task(_name, _configuration, _reportLevel)
{
  appendClassName("CalibrationProducer");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void CalibrationProducer::setDefaultConfiguration()
{
  if (reportStart(__FUNCTION__))
    ;
  configuration.setName("CalibrationProducer Configuration");
  configuration.setParameter("createHistograms",        true);
  configuration.setParameter("saveHistograms",          true);
  configuration.setParameter("forceHistogramsRewrite",  true);
  configuration.addParameter("efficiencyOpt",           0);
  configuration.addParameter("histoInputPath",          TString("./"));
  configuration.addParameter("histoRatioFileName",      TString("none"));
  configuration.addParameter("histoOutputPath",         TString("./"));
  configuration.addParameter("histoEffFileName",        TString("none"));
}

void CalibrationProducer::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  bool    forceHistogramsRewrite = configuration.getValueBool("forceHistogramsRewrite");
  int     efficiencyOpt          = configuration.getValueInt("efficiencyOpt");
  TString histoInputPath         = configuration.getValueString("histoInputPath");
  TString histoRatioFileName     = configuration.getValueString("histoRatioFileName");
  TString histoOutputPath        = configuration.getValueString("histoOutputPath");
  TString histoEffFileName       = configuration.getValueString("histoEffFileName");
    
  if (reportInfo(__FUNCTION__))
    {
    cout
    << endl
    << "   Starting Calibration Producer for :" << endl
    << "           histoInputPath: " << histoInputPath  << endl
    << "       histoRatioFileName: " << histoRatioFileName << endl
    << "    histoDetectorFileName: " << histoDetectorFileName << endl
    << "          histoOutputPath: " << histoOutputPath  << endl
    << "         histoEffFileName: " << histoEffFileName  << endl;
    switch (efficiencyOpt)
      {
        case 0: cout << "           efficiencyOpt: eff vs. pt" << endl; break;
        case 1: cout << "           efficiencyOpt: eff vs. pt vs eta" << endl; break;
        case 2: cout << "           efficiencyOpt: eff vs. pt vs y" << endl; break;
        case 3: cout << "           efficiencyOpt: eff vs. pt vs phi vs eta" << endl; break;
        case 4: cout << "           efficiencyOpt: eff vs. pt vs phi vs y" << endl; break;
      }
    }

  TFile * inputFile = openRootFile(histoInputPath, histoRatioFileName, "READ");
  TFile * outputFile;
  if (forceHistogramsRewrite)
    outputFile   = openRootFile(histoOutputPath,histoEffFileName,"RECREATE");
  else
    outputFile   = openRootFile(histoOutputPath,histoEffFileName,"NEW");

  if (!inputFile || !outputFile) return;
  
  HistogramCollection * inputCollection = new HistogramCollection("Input",getReportLevel());
  HistogramCollection * outputCollection  = new HistogramCollection("Output", getReportLevel());
  inputCollection->loadCollection(inputFile);
  inputCollection->setOwnership(false);
  outputCollection->setOwnership(false);
  switch (selectedMethod)
  {
    case 0:
    break;
    case 1:
    break;
  }

  //if (!isTaskOk()) return;
  outputCollection->saveHistograms(closureFile);
  generatorFile->Close();
  detectorFile->Close();
  closureFile->Close();
  delete generatorCollection;
  delete detectorCollection;
  delete closureCollection;
  if (reportInfo ()) cout << "Closure Test Completed." << endl;
}

