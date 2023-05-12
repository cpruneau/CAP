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
                               const Configuration & _configuration,
                               vector<EventFilter*> & _eventFilters,
                               vector<ParticleFilter*> & _particleFilters)
:
EventTask(_name,_configuration,_eventFilters,_particleFilters),
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
  addParameter("HistogramsCreate",  true);
  addParameter("HistogramsExport",    true);
  addParameter("EventsUseStream0",   true);
  addParameter("EventsUseStream1",   false);
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

void GlobalAnalyzer::configure()
{
  EventTask::configure();
  setEvent = getValueBool("SetEvent");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("EventsAnalyze");
    printItem("EventsUseStream0");
    printItem("EventsUseStream1");
    printItem("EventsUseStream2");
    printItem("EventsUseStream3");
    printItem("HistogramsCreate");
    printItem("HistogramsExport");
    printItem("EventsUseStream0");
    printItem("EventsUseStream1");
    printItem("SetEvent");
    printItem("FillCorrelationHistos");
    printItem("Fill2D");
    printItem("nBins_n");
    printItem("nBins_n2");
    printItem("Min_n");
    printItem("Max_n");
    printItem("range_n");
    printItem("nBins_e");
    printItem("nBins_e2");
    printItem("Min_e");
    printItem("Max_e");
    printItem("range_e");
    printItem("nBins_q");
    printItem("nBins_q2");
    printItem("Min_q");
    printItem("Max_q");
    printItem("range_n");
    printItem("nBins_b");
    printItem("nBins_b2");
    printItem("Min_b");
    printItem("Max_b");
    printItem("range_b");
    printItem("nBins_ptSum");
    printItem("nBins_ptSum2");
    printItem("Min_ptSum");
    printItem("Max_ptSum");
    printItem("nBins_ptAvg");
    printItem("nBins_ptAvg2");
    printItem("Min_ptAvg");
    printItem("Max_ptAvg");
    cout << endl;
    }
}


void GlobalAnalyzer::initialize()
{
  if (reportStart(__FUNCTION__))
;
  EventTask::initialize();
  int nParticleFilters = particleFilters.size();
  setEvent = true;//getValueBool("SetEvent");
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
  String bn  = getParentName( );
  if (reportInfo(__FUNCTION__))
    {
    printItem("Creating HistogramGroup for",bn);
    printItem("nEventFilters"              ,nEventFilters);
    printItem("nParticleFilters"           ,nParticleFilters);
    }
  histogramManager.addSet("Global");
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    String name = bn;
    name += "_";
    name += efn;
    GlobalHistos * histos = new GlobalHistos(this,name,configuration,particleFilters);
    histos->createHistograms();
    histogramManager.addGroupInSet(0,histos);
    }
  if (reportEnd(__FUNCTION__))
;
}

//Task * _parent,
//const String & _name,
//const Configuration & _configuration,
//vector<ParticleFilter*> _particleFilters

void GlobalAnalyzer::importHistograms(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
;
  String bn  = getParentName( );
  if (reportDebug(__FUNCTION__))
    {
    cout << "Loading HistogramGroup for " << bn  << endl;
    cout << "nEventFilters................ : " << nEventFilters << endl;
    cout << "nParticleFilters............. : " << nParticleFilters << endl;
    }
  histogramManager.addSet("Global");
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    String name = bn;
    name += "_";
    name += efn;
    GlobalHistos * histos = new GlobalHistos(this,name,configuration,particleFilters);
    histos->importHistograms(inputFile);
    histogramManager.addGroupInSet(0,histos);
    }
  if (reportEnd(__FUNCTION__))
;
}

void GlobalAnalyzer::analyzeEvent()
{
//  
//  if (reportStart(__FUNCTION__))
//;
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
          s[iParticleFilter] += type.getNetStrangeness();
          b[iParticleFilter] += type.getBaryonNumber();
          ptSum[iParticleFilter] += momentum.Pt();
          }
        }
      }
    if (iEventFilter==0 && setEvent)
      {
      EventProperties * ep = event.getEventProperties();
      ep->fill(n,ptSum, e,q,s,b);
      }

    GlobalHistos * globalHistos = (GlobalHistos * ) histogramManager.getGroup(0,iEventFilter);
    globalHistos->fill(n,ptSum,e,q,s,b,1.0);
    }
}

void GlobalAnalyzer::createDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
;
  String bn  = getName();
  if (reportInfo(__FUNCTION__))
    {
    cout << "Creating histograms for " << bn << endl;
    cout << "nEventFilters............... : " << nEventFilters << endl;
    cout << "nParticleFilters............ : " << nParticleFilters << endl;
    }
  histogramManager.addSet("GlobalDerived");
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    String name = bn;
    name += "_";
    name += efn;
    GlobalDerivedHistos * histos = new GlobalDerivedHistos(this,name,configuration,particleFilters);
    histos->createHistograms();
    histogramManager.addGroupInSet(1,histos);
    }
  if (reportEnd(__FUNCTION__))
;
}

void GlobalAnalyzer::importDerivedHistograms(TFile & inputFile __attribute__((unused)))
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
  for (int iEventFilter=0; iEventFilter< nEventFilters; iEventFilter++ )
    {
    baseHistos    = (GlobalHistos *) histogramManager.getGroup(0,iEventFilter);
    derivedHistos = (GlobalDerivedHistos *) histogramManager.getGroup(0,iEventFilter);
    derivedHistos->calculateDerivedHistograms(baseHistos);
    }
  if (reportEnd(__FUNCTION__))
;
}
