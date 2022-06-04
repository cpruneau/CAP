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

DerivedHistogramCalculator::DerivedHistogramCalculator(const TString &          _name,
                                                       const Configuration &    _configuration,
                                                       vector<EventFilter*>   & _eventFilters,
                                                       vector<ParticleFilter*>& _particleFilters,
                                                       LogLevel                 _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  appendClassName("DerivedHistogramCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void DerivedHistogramCalculator::saveHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  bool forceHistogramsRewrite     = configuration.getValueBool("forceHistogramsRewrite");
  TString histoOutputPath         = configuration.getValueString("histoOutputPath");
  TString histoOutputFileName     = configuration.getValueString("histoOutputFileName");
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "       nTaskExecuted : " << nTaskExecuted  << endl;
    cout << "  nTaskExecutedTotal : " << nTaskExecutedTotal  << endl;
    cout << "     histoOutputPath : " << histoOutputPath  << endl;
    cout << " histoOutputFileName : " << histoOutputFileName  << endl;
    }
  if (reportInfo(__FUNCTION__)) cout << " Saving: " << histoOutputFileName << endl;
  TFile * outputFile;
  if (forceHistogramsRewrite)
    outputFile = openRootFile(histoOutputPath,histoOutputFileName,"RECREATE");
  else
    outputFile = openRootFile(histoOutputPath,histoOutputFileName,"NEW");
  if (!outputFile) return;

  writeNEventsAccepted(outputFile);
  writeNEexecutedTask(outputFile);
  saveEventCountHistograms(outputFile);
  Task::saveHistograms(outputFile);
  outputFile->Close();
  if (reportEnd(__FUNCTION__))
    ;
}
