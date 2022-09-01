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
#include "ParticleDerivedHistos.hpp"
#include "DerivedHistogramCalculator.hpp"

ClassImp(DerivedHistogramCalculator);

DerivedHistogramCalculator::DerivedHistogramCalculator(const TString & _name,
                                                       Configuration & _configuration,
                                                       vector<EventFilter*> & _eventFilters,
                                                       vector<ParticleFilter*>& _particleFilters)
:
Task(_name, _configuration, _eventFilters, _particleFilters)
{
  appendClassName("DerivedHistogramCalculator");
}

void DerivedHistogramCalculator::saveHistograms()
{
  if (reportEnd(__FUNCTION__))
    ;
  bool forceHistogramsRewrite     = getValueBool("ForceHistogramsRewrite");
  TString histogramOutputPath     = getValueString("HistogramOutputPath");
  TString histogramOutputFile     = getValueString("HistogramOutputFile");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " HistogramOutputPath..........: " << histogramOutputPath  << endl;
    cout << " HistogramOutputFile..........: " << histogramOutputFile  << endl;
    }
  TFile * outputFile;
  if (forceHistogramsRewrite)
    outputFile = openRootFile(histogramOutputPath,histogramOutputFile,"RECREATE");
  else
    outputFile = openRootFile(histogramOutputPath,histogramOutputFile,"NEW");
  if (!outputFile) return;
  writeNEventsAccepted(outputFile);
  writeNEexecutedTask(outputFile);
  saveEventCountHistograms(outputFile);
  Task::saveHistograms(outputFile);
  outputFile->Close();
  if (reportEnd(__FUNCTION__))
    ;
}
