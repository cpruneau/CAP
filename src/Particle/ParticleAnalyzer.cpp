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
#include "ParticleAnalyzer.hpp"
#include "ParticleDerivedHistogramCalculator.hpp"

ClassImp(ParticleAnalyzer);

ParticleAnalyzer::ParticleAnalyzer(const TString &         _name,
                                   const Configuration &   _configuration,
                                   vector<EventFilter*>    _eventFilters,
                                   vector<ParticleFilter*> _particleFilters,
                                   LogLevel                _selectedLevel)
:
Task(_name,_configuration,_eventFilters,_particleFilters,_selectedLevel),
fillEta(true),
fillY(false),
fillP2(false)
{
  appendClassName("ParticleAnalyzer");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
  for (unsigned int k=0; k<particleFilters.size(); k++)
    {
    vector<ParticleDigit*> list;
    filteredParticles.push_back(list);
    }
}

void ParticleAnalyzer::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  configuration.setName("ParticleAnalyzer Configuration");
  configuration.setParameter("useParticles",true);
  configuration.setParameter("createHistograms",true);
  configuration.setParameter("saveHistograms",true);
   
  configuration.addParameter("nBins_n1",  100);
  configuration.addParameter("min_n1",    0.0);
  configuration.addParameter("max_n1",  100.0);

  configuration.addParameter("nBins_eTot",  100);
  configuration.addParameter("min_eTot",    0.0);
  configuration.addParameter("max_eTot",  100.0);

  configuration.addParameter("nBins_pt",  100);
  configuration.addParameter("min_pt",    0.0);
  configuration.addParameter("max_pt",  100.0);

  configuration.addParameter("nBins_eta",  20);
  configuration.addParameter("min_eta",   -1.0);
  configuration.addParameter("max_eta",    1.0);

  configuration.addParameter("nBins_y",  20);
  configuration.addParameter("min_y",   -1.0);
  configuration.addParameter("max_y",    1.0);

  configuration.addParameter("nBins_phi",  36);
  configuration.addParameter("min_phi",    0.0);
  configuration.addParameter("max_phi",    TMath::TwoPi());

  configuration.addParameter("nBins_phiEta",    720);
  configuration.addParameter("nBins_phiEtaPt",  7200);
  configuration.addParameter("nBins_phiY",      720);
  configuration.addParameter("nBins_phiYPt",    7200);
  configuration.addParameter("fillEta",  fillEta);
  configuration.addParameter("fillY",    fillY);
  configuration.addParameter("fillP2",   fillP2);
  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

void ParticleAnalyzer::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); //prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  
  fillEta = configuration.getValueBool("fillEta");
  fillY   = configuration.getValueBool("fillY");
  fillP2  = configuration.getValueBool("fillP2");
  
  if (reportInfo(__FUNCTION__))
    {
    cout << "Creating Histogram(s) for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString partFilterName = particleFilters[iParticleFilter]->getName();
      TString histoName  = prefixName;
      histoName += "_";
      histoName += evtFilterName;
      histoName += "_";
      histoName += partFilterName;
      ParticleHistos * histos = new ParticleHistos(histoName,configuration,getReportLevel());
      histos->createHistograms();
      baseSingleHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void ParticleAnalyzer::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  TString prefixName = getName(); prefixName += "_";
  if (reportInfo(__FUNCTION__))
    {
    cout << endl <<  "Loading Histogram(s) for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = * eventFilters[iEventFilter]->getName();
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString partFilterName = * particleFilters[iParticleFilter]->getName();
      TString histoName  = prefixName;
      histoName += evtFilterName;
      histoName += "_";
      histoName += partFilterName;
      ParticleHistos * histos = new ParticleHistos(histoName,configuration,getReportLevel());
      histos->loadHistograms(inputFile);
      baseSingleHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticleAnalyzer::execute()
{
  incrementTaskExecuted();
  Event & event = *eventStreams[0];
  //Is this event accepted by this task's event filters?
  bool analyzeThisEvent = false;
  resetNParticlesAcceptedEvent();
  vector<unsigned int> eventFilterPassed;
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(event)) continue;
    incrementNEventsAccepted(iEventFilter);
    eventFilterPassed.push_back(iEventFilter);
    analyzeThisEvent = true;
    }
  if (!analyzeThisEvent) return;

  // The event is accepted at least one event filter
  // but it may not have  pairs. If so skip out.
  // Doing the checks in this order guarantees the accepted
  // event count is correct for normalization purposes.
  unsigned int nParticles = event.getNParticles();
  if (nParticles<1) return;

  vector<double> nAccepted(nParticleFilters,0.0);
  vector<double> totalEnergy(nParticleFilters,0.0);

  if (eventFilterPassed.size()>1 || nParticleFilters>1)
    {
    // multiple events filters or particle filters
    // let us first reduce the number of particles to look at
    // and generate sublists for each particle filter
    // and use them to do the histo filling.

    Factory<ParticleDigit> * factory = ParticleDigit::getFactory();
    factory->reset();
    int index;
    int iEventFilter = 0;
//    float pt,e;
//    int iPt, iPhi, iEta, iY;
    ParticleHistos * histos = (ParticleHistos *) baseSingleHistograms[0];
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ ) filteredParticles[iParticleFilter].clear();

    resetNParticlesAcceptedEvent();
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      Particle & particle = * event.getParticleAt(iParticle);
      float pt,e;
      int iPt, iPhi, iEta, iY;
      ParticleDigit * pd;
      bool digitized = false;
      for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        if (particleFilters[iParticleFilter]->accept(particle))
          {
          incrementNParticlesAccepted(iEventFilter,iParticleFilter);
          if (!digitized)
            {
            TLorentzVector & momentum = particle.getMomentum();
            pt     = momentum.Pt();
            e      = momentum.E();
            iPt    = histos->getPtBinFor(pt);
            iPhi   = histos->getPhiBinFor(momentum.Phi());
            iEta   = histos->getEtaBinFor(momentum.Eta());
            iY     = histos->getYBinFor(momentum.Rapidity());
            pd     = factory->getNextObject();
            pd->iY   = iY;
            pd->iEta = iEta;
            pd->iPt  = iPt;
            pd->iPhi = iPhi;
            pd->pt   = pt;
            pd->e    = e;
            digitized = true; // so no need to digitize this particle again...
            }
          if (digitized && ( iPt>0 || iPhi>0 || iEta>=0 || iY>=0))
            {
            filteredParticles[iParticleFilter].push_back(pd);
            }
          } // particle accepted by filter
        } // filter loop
      } //particle loop

    // use the filter particles to fill the histos for the accepted event filters
    for (unsigned int jEventFilter=0; jEventFilter<eventFilterPassed.size(); jEventFilter++ )
      {
      iEventFilter = eventFilterPassed[jEventFilter];
      for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        index = iParticleFilter+iEventFilter*nParticleFilters;
        ParticleHistos * histos = (ParticleHistos *) baseSingleHistograms[index];
        histos->fill(filteredParticles[iParticleFilter],1.0);
        } // iParticleFilter loop
      } // jEventFilter loop
    }
  else
    {
    // 1 event filter and 1 particle filter: just scan the particles...
    // no need for sublists.
    int iParticleFilter = 0;
    int iEventFilter = eventFilterPassed[0];
    int index = iParticleFilter+iEventFilter*nParticleFilters;
    ParticleHistos * histos = (ParticleHistos *) baseSingleHistograms[index];
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      Particle & particle = * event.getParticleAt(iParticle);
      //TLorentzVector & momentum = particle.getMomentum();
      if (particleFilters[iParticleFilter]->accept(particle))
        {
        incrementNParticlesAccepted(0,0);
        //cout << "ParticlePairAnalyzer::execute() accepted:" << endl;
        nAccepted[iParticleFilter]++;
        totalEnergy[iParticleFilter] += particle.getMomentum().E();
        histos->fill(particle,1.0);
        }
      }
    histos->fillMultiplicity(nAccepted[iParticleFilter],totalEnergy[iParticleFilter],1.0);
    }
  
// all done with this event...
}


Task * ParticleAnalyzer::getDerivedCalculator()
{
  if (reportDebug(__FUNCTION__))
    ;
  TString nameD = getName();
  if (reportDebug(__FUNCTION__)) cout << "Name of this task is:" << nameD  << endl;
  Configuration derivedCalcConfiguration;
  // copy the parameters of this task to the new task -- so all the histograms will automatically match
  derivedCalcConfiguration.setParameters(configuration);
  derivedCalcConfiguration.setParameter("createHistograms",       true);
  derivedCalcConfiguration.setParameter("loadHistograms",         true);
  derivedCalcConfiguration.setParameter("saveHistograms",         true);
  Task * calculator = new ParticleDerivedHistogramCalculator(nameD,derivedCalcConfiguration,eventFilters,particleFilters,getReportLevel());
  return calculator;
}

