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

#include "GlobalAnalyzer.hpp"
#include "GlobalHistos.hpp"
#include "GlobalDerivedHistos.hpp"
using CAP::GlobalAnalyzer;
using CAP::GlobalHistos;
using CAP::GlobalDerivedHistos;

ClassImp(GlobalAnalyzer);

GlobalAnalyzer::GlobalAnalyzer(const String & _name,
                               Configuration & _configuration,
                               vector<EventFilter*> & _eventFilters,
                               vector<ParticleFilter*> & _particleFilters)
:
Task(_name,_configuration,_eventFilters,_particleFilters),
setEvent(false),
n(),
e(),
q(),
s(),
b(),
ptSum()
{
  appendClassName("GlobalAnalyzer");
}

//!
//!
void GlobalAnalyzer::setDefaultConfiguration()
{
  setParameter("UseParticles",      true);
  setParameter("CreateHistograms",  true);
  setParameter("SaveHistograms",    true);
  setParameter("UseEventStream0",   true);
  setParameter("UseEventStream1",   false);
  addParameter("SetEvent",             true);
  addParameter("FillCorrelationHistos",false);
  addParameter("Fill2D",               false);
  addParameter("nBins_n",              100);
  addParameter("nBins_n2",             20);
  addParameter("Min_n",                0.0);
  addParameter("Max_n",                1000.0);
  addParameter("range_n",              1000.0),
  addParameter("nBins_e",              100);
  addParameter("nBins_e2",             20);
  addParameter("Min_e",                0.0);
  addParameter("Max_e",                1000.0);
  addParameter("range_e",              1000.0),
  addParameter("nBins_q",              100);
  addParameter("nBins_q2",             20);
  addParameter("Min_q",                -50.0);
  addParameter("Max_q",                50.0);
  addParameter("range_n",              100.0),
  addParameter("nBins_b",              100);
  addParameter("nBins_b2",             20);
  addParameter("Min_b",                -50.0);
  addParameter("Max_b",                50.0);
  addParameter("range_b",              100.0);
  addParameter("nBins_ptSum",          100);
  addParameter("nBins_ptSum2",         20);
  addParameter("Min_ptSum",            0.0);
  addParameter("Max_ptSum",            100.0);
  addParameter("nBins_ptAvg",          200);
  addParameter("nBins_ptAvg2",         40);
  addParameter("Min_ptAvg",            0.0);
  addParameter("Max_ptAvg",            2.0);
}

void GlobalAnalyzer::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  int nParticleFilters = particleFilters.size();
  setEvent = getValueBool("SetEvent");
  n.assign(nParticleFilters,0.0);
  ptSum.assign(nParticleFilters,0.0);
  e.assign(nParticleFilters,0.0);
  q.assign(nParticleFilters,0.0);
  s.assign(nParticleFilters,0.0);
  b.assign(nParticleFilters,0.0);
  if (reportDebug(__FUNCTION__)) cout << " #event added streams: "  << getNEventStreams() << endl;
}

void GlobalAnalyzer::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  String bn  = getParentTaskName();
  if (reportInfo(__FUNCTION__))
    {
    cout << "  G:Creating HistogramGroup for.......: " << bn  << endl;
    cout << "  G:nEventFilters................ : " << nEventFilters << endl;
    cout << "  G:nParticleFilters............. : " << nParticleFilters << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    GlobalHistos * histos = new GlobalHistos(this,createName(bn,efn),configuration,particleFilters);
    histos->createHistograms();
    histograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void GlobalAnalyzer::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  String bn  = getParentTaskName();
  if (reportDebug(__FUNCTION__))
    {
    cout << "Loading HistogramGroup for " << bn  << endl;
    cout << "nEventFilters................ : " << nEventFilters << endl;
    cout << "nParticleFilters............. : " << nParticleFilters << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    GlobalHistos * histos = new GlobalHistos(this,createName(bn,efn),configuration,particleFilters);
    histos->loadHistograms(inputFile);
    histograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void GlobalAnalyzer::execute()
{
//  
//  if (reportStart(__FUNCTION__))
//    ;
  incrementTaskExecuted();
  Event & event = * getEventStream(0);
  // count eventStreams used to fill histograms and for scaling at the end..
  // resetParticleCounters();
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  unsigned int nParticles       = event.getNParticles();
  resetNParticlesAcceptedEvent();
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(event)) continue;
    incrementNEventsAccepted(iEventFilter);
    if (nParticles<1) continue;
    
    n.assign(nParticleFilters,0.0);
    e.assign(nParticleFilters,0.0);
    q.assign(nParticleFilters,0.0);
    s.assign(nParticleFilters,0.0);
    b.assign(nParticleFilters,0.0);
    ptSum.assign(nParticleFilters,0.0);
    for (unsigned int iParticle=0; iParticle<event.getNParticles(); iParticle++)
      {
      Particle & particle = * event.getParticleAt(iParticle);
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        //cout << iParticleFilter << "  " << particle.getType().getName() << endl;
        if (particleFilters[iParticleFilter]->accept(particle))
          {
          incrementNParticlesAccepted(iEventFilter,iParticleFilter);
          // // incrementParticlesAccepted();
          ParticleType & type = particle.getType();
          LorentzVector & momentum = particle.getMomentum();
          n[iParticleFilter]++;
          e[iParticleFilter] += momentum.E();
          q[iParticleFilter] += type.getCharge();
          s[iParticleFilter] += type.getStrange();
          b[iParticleFilter] += type.getBaryon();
          ptSum[iParticleFilter] += momentum.Pt();
          }
        }
      }
    if (iEventFilter==0 && setEvent)
      {
      EventProperties * ep = event.getEventProperties();
      ep->fill(n,ptSum, e,q,s,b);
      }
    GlobalHistos * globalHistos = (GlobalHistos * ) histograms[iEventFilter];
    globalHistos->fill(n,ptSum,e,q,s,b,1.0);
    }
}

void GlobalAnalyzer::createDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  String bn  = getName();
  if (reportInfo(__FUNCTION__))
    {
    cout << "Creating histograms for " << bn << endl;
    cout << "nEventFilters............... : " << nEventFilters << endl;
    cout << "nParticleFilters............ : " << nParticleFilters << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    GlobalDerivedHistos * histos = new GlobalDerivedHistos(this,createName(bn,efn),configuration,particleFilters);
    histos->createHistograms();
    derivedHistograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void GlobalAnalyzer::loadDerivedHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (reportEnd(__FUNCTION__))
    ;
}

void GlobalAnalyzer::calculateDerivedHistograms()
{
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "Computing derived histograms for....: " << endl;
    cout << "nEventFilters.......................: " << nEventFilters << endl;
    cout << "nParticleFilters....................: " << nParticleFilters << endl;
    }
  GlobalHistos        * baseHistos;
  GlobalDerivedHistos * derivedHistos;

  //!Mode 1: Running rigth after Analysis: base histograms pointers  are copied from analyzer to baseSingleHistograms
  //!Mode 2: Running as standalone: base histograms are loaded from file.
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    baseHistos    = (GlobalHistos *) histograms[iEventFilter];
    derivedHistos = (GlobalDerivedHistos *) derivedHistograms[iEventFilter];
    derivedHistos->calculateDerivedHistograms(baseHistos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}
