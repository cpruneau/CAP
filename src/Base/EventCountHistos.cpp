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
#include "TLorentzVector.h"
#include "EventCountHistos.hpp"
ClassImp(EventCountHistos);

EventCountHistos::EventCountHistos(const TString & _name,
                                   Configuration & _configuration,
                                   int _nEventFilters,
                                   int _nParticleFilters,
                                   LogLevel _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
nEventFilters(_nEventFilters),
nParticleFilters(_nParticleFilters),
h_taskExecutedReset(nullptr),
h_eventAcceptedReset(nullptr),
h_partilceAcceptedReset(nullptr)
{
  // no ops
}

// for now use the same boundaries for eta and y histogram
void EventCountHistos::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  TString bn = getHistoBaseName();
  TString histoName;
  histoName = bn+TString("taskExecuted");
  h_taskExecutedReset = createHistogram(histoName,1,-0.5, 0.5, "taskExecuted", "Count") ;
  histoName = bn+TString("eventAccepted");
  h_eventAcceptedReset = createHistogram(histoName,nEventFilters,-0.5, -0.5+double(nEventFilters), "event filter", "Count");
  histoName = bn+TString("partilceAccepted");
  int n = nEventFilters*nParticleFilters;
  h_partilceAcceptedReset = createHistogram(histoName,n,-0.5, -0.5+double(n), "event x particle filter", "Count");
  if (reportEnd(__FUNCTION__))
    ;
 }

//________________________________________________________________________
void EventCountHistos::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  if (!inputFile)
    {
    if (reportFatal()) cout << "Attempting to load EventCountHistos from an invalid file pointer" << endl;
    return;
    }
  TString bn = getHistoBaseName();
  TString histoName;

  histoName = bn+TString("taskExecuted");
  h_taskExecutedReset = loadH1(inputFile,histoName);
  if (!h_taskExecutedReset && reportError()) cout << "Could not load histogram: " << histoName << endl;
  histoName = bn+TString("eventAccepted");
  h_eventAcceptedReset = loadH1(inputFile,histoName);
  if (!h_eventAcceptedReset && reportError()) cout << "Could not load histogram: " << histoName << endl;
  histoName = bn+TString("partilceAccepted");
  h_partilceAcceptedReset = loadH1(inputFile,histoName);
  if (!h_partilceAcceptedReset && reportError()) cout << "Could not load histogram: " << histoName << endl;
  if (reportEnd(__FUNCTION__))
    ;
}

// This fills the event count histogram. Should be called once per histo file save.
void EventCountHistos::fill(long nTaskExecutedReset,
                            vector<long> & nEventsAcceptedReset,
                            vector<long> & nParticleAcceptedReset)
{
  
  if (reportStart(__FUNCTION__))
    ;
  h_taskExecutedReset->Fill(0.0, double(nTaskExecutedReset));
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++)
    {
    h_eventAcceptedReset->Fill(double(iEventFilter),double(nEventsAcceptedReset[iEventFilter]));
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++)
      {
      int index = iEventFilter*nParticleFilters + iParticleFilter;
      h_partilceAcceptedReset->Fill(double(index),double(nParticleAcceptedReset[index]));
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

