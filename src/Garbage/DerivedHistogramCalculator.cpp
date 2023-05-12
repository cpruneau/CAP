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
#include "ParticleSingleDerivedHistos.hpp"
#include "DerivedHistogramCalculator.hpp"
using CAP::DerivedHistogramCalculator;

ClassImp(DerivedHistogramCalculator);

DerivedHistogramCalculator::DerivedHistogramCalculator(const String & _name,
                                                       const Configuration & _configuration,
                                                       vector<EventFilter*> & _eventFilters,
                                                       vector<ParticleFilter*>& _particleFilters)
:
EventTask(_name, _configuration, _eventFilters, _particleFilters)
{
  appendClassName("DerivedHistogramCalculator");
}

void DerivedHistogramCalculator::exportHistograms()
{
  if (reportEnd(__FUNCTION__))
;
  bool histosForceRewrite     = getValueBool("HistogramsForceRewrite");
  String histosExportPath     = getValueString("HistogramsExportPath");
  String histosExportFile     = getValueString("HistogramsExportFile");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " HistogramsExportPath..........: " << histosExportPath  << endl;
    cout << " HistogramsExportFile..........: " << histosExportFile  << endl;
    }
  TFile * outputFile;
  if (histosForceRewrite)
    outputFile = openRootFile(histosExportPath,histosExportFile,"RECREATE");
  else
    outputFile = openRootFile(histosExportPath,histosExportFile,"NEW");
  if (!outputFile) return;
  writeNEventsAccepted(outputFile);
  writeNEexecutedTask(outputFile);
  Task::exportHistograms(outputFile);
  outputFile->Close();
  if (reportEnd(__FUNCTION__))
;
}
