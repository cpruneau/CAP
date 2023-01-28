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
#include "EventCountHistos.hpp"
#include "NameManager.hpp"
using CAP::EventCountHistos;
using CAP::String;
using CAP::Configuration;

ClassImp(EventCountHistos);

EventCountHistos::EventCountHistos(Task *          _parent,
                                   const String & _name,
                                   Configuration & _configuration,
                                   int             _nEventFilters,
                                   int             _nParticleFilters)
:
HistogramGroup(_parent,_name,_configuration),
nEventFilters(_nEventFilters),
nParticleFilters(_nParticleFilters),
h_taskExecuted(nullptr),
h_eventAccepted(nullptr),
h_particleAccepted(nullptr)
{
  appendClassName("EventCountHistos");
}

// for now use the same boundaries for eta and y histogram
void EventCountHistos::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  setOwnership(true);
  int n = nEventFilters*nParticleFilters;
  String bn = getParentTaskName();
  h_taskExecuted     = createHistogram(createName(bn,"nTaskExecuted"),1,-0.5, 0.5, "nTaskExecuted", "Count") ;
  h_eventAccepted    = createHistogram(createName(bn,"nEventAccepted"),nEventFilters,-0.5, -0.5+double(nEventFilters), "event filter", "Count");
  h_particleAccepted = createHistogram(createName(bn,"nParticleAccepted"),n,-0.5, -0.5+double(n), "event x particle filter", "Count");
  if (reportEnd(__FUNCTION__))
    ;
 }

//________________________________________________________________________
void EventCountHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  String bn = getParentTaskName();
  h_taskExecuted     = loadH1(inputFile,createName(bn,"nTaskExecuted"));
  h_eventAccepted    = loadH1(inputFile,createName(bn,"nEventAccepted"));
  h_particleAccepted = loadH1(inputFile,createName(bn,"nParticleAccepted"));
  if (reportEnd(__FUNCTION__))
    ;
}

// This fills the event count histogram. Should be called once per histo file save.
void EventCountHistos::fill(long nTaskExecuted,
                            vector<long> & nEventsAccepted,
                            vector<long> & nParticleAccepted)
{
  
  if (reportStart(__FUNCTION__))
    ;
  h_taskExecuted->Fill(0.0, double(nTaskExecuted));
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++)
    {
    h_eventAccepted->Fill(double(iEventFilter),double(nEventsAccepted[iEventFilter]));
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++)
      {
      int index = iEventFilter*nParticleFilters + iParticleFilter;
      h_particleAccepted->Fill(double(index),double(nParticleAccepted[index]));
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

