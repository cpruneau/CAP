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
#include <TLorentzVector.h>
#include "GlobalAnalyzer.hpp"
#include "GlobalDerivedHistogramCalculator.hpp"

ClassImp(GlobalAnalyzer);

GlobalAnalyzer::GlobalAnalyzer(const TString &           _name,
                               const Configuration &     _configuration,
                               vector<EventFilter*> &    _eventFilters,
                               vector<ParticleFilter*> & _particleFilters,
                               LogLevel                  _requiredLevel)
:
Task(_name,_configuration,_eventFilters,_particleFilters,_requiredLevel),
setEvent(false),
n(),
e(),
q(),
s(),
b(),
ptSum()
{
  appendClassName("GlobalAnalyzer");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

//!
//!
void GlobalAnalyzer::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  configuration.setName("GlobalAnalyzer Configuration");
  configuration.setParameter("useEventStream0",      true);
  configuration.setParameter("useParticles",         true);
  configuration.setParameter("createHistograms",     true);
  configuration.setParameter("saveHistograms",       true);
  configuration.setParameter("histoAnalyzerName",    TString("G"));
  configuration.setParameter("histoBaseName",        TString("G"));
  configuration.addParameter("setEvent",             true);
  configuration.addParameter("fillCorrelationHistos",false);
  configuration.addParameter("fill2D",               false);
  configuration.addParameter("nBins_n",              100);
  configuration.addParameter("nBins_n2",             20);
  configuration.addParameter("min_n",                0.0);
  configuration.addParameter("max_n",                1000.0);
  configuration.addParameter("range_n",              1000.0),
  configuration.addParameter("nBins_e",              100);
  configuration.addParameter("nBins_e2",             20);
  configuration.addParameter("min_e",                0.0);
  configuration.addParameter("max_e",                1000.0);
  configuration.addParameter("range_e",              1000.0),
  configuration.addParameter("nBins_q",              100);
  configuration.addParameter("nBins_q2",             20);
  configuration.addParameter("min_q",                -50.0);
  configuration.addParameter("max_q",                50.0);
  configuration.addParameter("range_n",              100.0),
  configuration.addParameter("nBins_b",              100);
  configuration.addParameter("nBins_b2",             20);
  configuration.addParameter("min_b",                -50.0);
  configuration.addParameter("max_b",                50.0);
  configuration.addParameter("range_b",              100.0);
  configuration.addParameter("nBins_ptSum",          100);
  configuration.addParameter("nBins_ptSum2",         20);
  configuration.addParameter("min_ptSum",            0.0);
  configuration.addParameter("max_ptSum",            100.0);
  configuration.addParameter("nBins_ptAvg",          200);
  configuration.addParameter("nBins_ptAvg2",         40);
  configuration.addParameter("min_ptAvg",            0.0);
  configuration.addParameter("max_ptAvg",            2.0);
  //// if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

void GlobalAnalyzer::initialize()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  int nParticleFilters = particleFilters.size();
  setEvent = configuration.getValueBool("setEvent");
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
  TString bn  = getHistoBaseName();
  if (reportDebug(__FUNCTION__))
    {
    cout << "Creating Histograms for : " << bn  << endl;
    cout << "          nEventFilters : " << nEventFilters << endl;
    cout << "       nParticleFilters : " << nParticleFilters << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    GlobalHistos * histos = new GlobalHistos(makeHistoName(bn,efn),configuration,particleFilters,getReportLevel());
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
  TString bn  = getHistoBaseName();
  if (reportDebug(__FUNCTION__))
    {
    cout << "Creating Histograms for " << bn  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    GlobalHistos * histos = new GlobalHistos(makeHistoName(bn,efn),configuration,particleFilters,getReportLevel());
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
          TLorentzVector & momentum = particle.getMomentum();
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
 
Task * GlobalAnalyzer::getDerivedCalculator()
{
  if (reportDebug(__FUNCTION__))
    ;
  TString nameD = getName();
  Configuration derivedCalcConfiguration;
  // copy the parameters of this task to the new task -- so all the histograms will automatically match
  derivedCalcConfiguration.setParameters(configuration);
  derivedCalcConfiguration.setParameter("createHistograms",       true);
  derivedCalcConfiguration.setParameter("loadHistograms",         true);
  derivedCalcConfiguration.setParameter("saveHistograms",         true);
  derivedCalcConfiguration.setParameter("useEventStream0",        false);
  derivedCalcConfiguration.setParameter("useParticles",           true); // insures the event data are read/written.
  Task * calculator = new GlobalDerivedHistogramCalculator(nameD,derivedCalcConfiguration,eventFilters,particleFilters,getReportLevel());
  return calculator;
}
