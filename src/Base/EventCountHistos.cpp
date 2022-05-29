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
  appendClassName("EventCountHistos");
  setInstanceName(_name);
}

// for now use the same boundaries for eta and y histogram
void EventCountHistos::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  setOwnership(true);
  int n = nEventFilters*nParticleFilters;
  TString bn = getHistoBaseName();
  h_taskExecutedReset     = createHistogram(makeName(bn,"taskExecuted"),1,-0.5, 0.5, "taskExecuted", "Count") ;
  h_eventAcceptedReset    = createHistogram(makeName(bn,"eventAccepted"),nEventFilters,-0.5, -0.5+double(nEventFilters), "event filter", "Count");
  h_partilceAcceptedReset = createHistogram(makeName(bn,"particleAccepted"),n,-0.5, -0.5+double(n), "event x particle filter", "Count");
  if (reportEnd(__FUNCTION__))
    ;
 }

//________________________________________________________________________
void EventCountHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  TString bn = getHistoBaseName();
  h_taskExecutedReset     = loadH1(inputFile,makeName(bn,"taskExecuted"));
  h_eventAcceptedReset    = loadH1(inputFile,makeName(bn,"eventAccepted"));
  h_partilceAcceptedReset = loadH1(inputFile,makeName(bn,"particleAccepted"));
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

