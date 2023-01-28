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
using CAP::CalibrationProducer;

ClassImp(CalibrationProducer);

CalibrationProducer::CalibrationProducer(const String & _name,
                                         Configuration & _configuration)
:
Task(_name, _configuration, _reportLevel)
{
  appendClassName("CalibrationProducer");
}

void CalibrationProducer::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  setParameter("CreateHistograms",        true);
  setParameter("SaveHistograms",          true);
  //setParameter("ForceHistogramsRewrite",  true);
  addParameter("efficiencyOpt",           0);
  addParameter("HistogramInputPath",          TString("./"));
  addParameter("HistoRatioFileName",      TString("none"));
  addParameter("HistogramOutputPath",         TString("./"));
  addParameter("HistoEffFileName",        TString("none"));
}

void CalibrationProducer::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  bool    ForceHistogramsRewrite = configuration.getValueBool(  getName(),"ForceHistogramsRewrite");
  int     efficiencyOpt          = configuration.getValueInt(   getName(),"efficiencyOpt");
  String histoInputPath         = getValueString("HistogramInputPath");
  String histoRatioFileName     = getValueString("HistoRatioFileName");
  String HistogramOutputPath        = getValueString("HistogramOutputPath");
  String histoEffFileName       = getValueString("HistoEffFileName");
    
  if (reportInfo(__FUNCTION__))
    {
    cout
    << endl
    << "   Starting Calibration Producer for :" << endl
    << "           histoInputPath: " << histoInputPath  << endl
    << "       histoRatioFileName: " << histoRatioFileName << endl
    << "    histoDetectorFileName: " << histoDetectorFileName << endl
    << "          HistogramOutputPath: " << HistogramOutputPath  << endl
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
  if (ForceHistogramsRewrite)
    outputFile   = openRootFile(HistogramOutputPath,histoEffFileName,"RECREATE");
  else
    outputFile   = openRootFile(HistogramOutputPath,histoEffFileName,"NEW");

  if (!inputFile || !outputFile) return;
  
  HistogramCollection * inputCollection = new HistogramCollection("Input",getSeverityLevel());
  HistogramCollection * outputCollection  = new HistogramCollection("Output", getSeverityLevel());
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
  if (reportInfo (__FUNCTION__)) cout << "Closure Test Completed." << endl;
}

