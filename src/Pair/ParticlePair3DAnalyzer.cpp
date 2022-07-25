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
#include "ParticlePair3DAnalyzer.hpp"
#include "ParticlePair3DDerivedHistogramCalculator.hpp"

ClassImp(ParticlePair3DAnalyzer);

ParticlePair3DAnalyzer::ParticlePair3DAnalyzer(const TString &          _name,
                                           const Configuration &    _configuration,
                                           vector<EventFilter*> &   _eventFilters,
                                           vector<ParticleFilter*> &_particleFilters,
                                           LogLevel                 _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel),
fillEta(true),
fillY(false),
fillP2(false)
{
  appendClassName("ParticlePair3DAnalyzer");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
  for (unsigned int k=0; k<particleFilters.size(); k++)
    {
    vector<ParticleDigit*> list;
    filteredParticles.push_back(list);
    }
}

void ParticlePair3DAnalyzer::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  configuration.setName("ParticlePair3DAnalyzer Configuration");
  configuration.setParameter("useParticles",      true);
  configuration.setParameter("createHistograms",  true);
  configuration.setParameter("saveHistograms",    true);
  configuration.setParameter("histoAnalyzerName", TString("Pair3D"));
  configuration.setParameter("histoBaseName",     TString("Pair3D"));
  configuration.setParameter("useAbsDelta",       false);

  configuration.setParameter("nBins_DeltaP",  20);
  configuration.setParameter("min_DeltaP",    -2.0);
  configuration.setParameter("max_DeltaP",     2.0);

  // if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePair3DAnalyzer::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  LogLevel debugLevel = getReportLevel();
  TString bn  = getName();
  useAbsDelta = configuration.getValueBool("useAbsDelta");

  Histograms * histos;
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "       Creating Histograms: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle filter (Singles):" << pfn << endl;
      histos = new Particle3dHistos(makeHistoName(bn,efn,pfn),configuration,debugLevel);
      histos->createHistograms();
      baseSingleHistograms.push_back(histos);
      }

    // pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
        histos = new ParticlePair3DHistos(makeHistoName(bn,efn,pfn1,pfn2),configuration,debugLevel);
        histos->createHistograms();
        basePairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePair3DAnalyzer::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(inputFile)) return;
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  LogLevel debugLevel    = getReportLevel();
  TString bn  = getName();
  Histograms * histos;
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "       Creating Histograms: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle filter (Singles):" << pfn << endl;
      histos = new Particle3DHistos(makeHistoName(bn,efn,pfn),configuration,debugLevel);
      histos->loadHistograms(inputFile);
      baseSingleHistograms.push_back(histos);
      }

    // pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
        histos = new ParticlePair3DHistos(makeHistoName(bn,efn,pfn1,pfn2),configuration,debugLevel);
        histos->loadHistograms(inputFile);
        basePairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void ParticlePair3DAnalyzer::execute()
{
//  
//  if (reportStart(__FUNCTION__))
//    ;
  incrementTaskExecuted();
  Event & event = *eventStreams[0];

  //Is this event accepted by this task's event filters?
  bool analyzeThisEvent = false;
  unsigned int nEventFilters = eventFilters.size();
  vector<unsigned int> eventFilterPassed;
  eventFilterPassed.clear();
  resetNParticlesAcceptedEvent();
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(event)) continue;
    incrementNEventsAccepted(iEventFilter);
    eventFilterPassed.push_back(iEventFilter);
    analyzeThisEvent = true;
    }
  //if (reportInfo("ParticlePair3DAnalyzer",getName(),"createHistograms()")) cout << " -- 2 --" << endl;

  if (!analyzeThisEvent) return;

  //if (reportInfo("ParticlePair3DAnalyzer",getName(),"createHistograms()")) cout << " -- 2b --" << endl;

  // The event is accepted at least one event filter
  // but it may not have particle pairs. If so skip out.
  // Doing the checks in this order guarantees the accepted
  // event count is correct for normalization purposes.
  unsigned int nParticles = event.getNParticles();
  if (nParticles<2) return;

  //if (reportInfo("ParticlePair3DAnalyzer",getName(),"createHistograms()")) cout << " -- 3 --" << endl;
  vector< vector<Particle*> > acceptedParticles;
  for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
    {
    vector<Particle*> accepted;
    acceptedParticles.push_back(accepted);
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      if (particleFilters[iParticleFilter]->accept(particle))
        acceptedParticles[iParticleFilter].push_back(particle);
      }
    if (reportDebug(__FUNCTION__))
      {
      cout << "Particle Filter:" << iParticleFilter << " accepted:" << acceptedParticles[iParticleFilter].size() << endl;
      }
    }

  // now do the analysis

  for (unsigned int jEventFilter=0; jEventFilter<eventFilterPassed.size(); jEventFilter++ )
    {
    //if (reportInfo("ParticlePair3DAnalyzer",getName(),"createHistograms()")) cout << " -- 9 --" << endl;
    unsigned int  iEventFilter = eventFilterPassed[jEventFilter];
    unsigned int  baseSingle   = iEventFilter*nParticleFilters;
    unsigned int  basePair     = iEventFilter*nParticleFilters*nParticleFilters;
    unsigned int  index;
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      incrementNParticlesAccepted(iEventFilter,iParticleFilter1);
      //if (reportInfo("ParticlePair3DAnalyzer",getName(),"createHistograms()")) cout << " -- 10 --" << endl;
      index = baseSingle + iParticleFilter1;
      Particle3DHistos * histos = (ParticleHistos3D *) baseSingleHistograms[index];
      histos->fill(acceptedParticles[iParticleFilter1],1.0);
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        //if (reportInfo("ParticlePair3DAnalyzer",getName(),"createHistograms()")) cout << " -- 11 --" << endl;
        index = basePair + iParticleFilter1*nParticleFilters + iParticleFilter2;
        ParticlePair3DHistos * histos = (ParticlePair3DHistos *) basePairHistograms[index];
        histos->fill(acceptedParticles[iParticleFilter1],acceptedParticles[iParticleFilter2],iParticleFilter1==iParticleFilter2,1.0);
        //if (reportInfo("ParticlePair3DAnalyzer",getName(),"createHistograms()")) cout << " -- 13 --" << endl;
        }
      }
    }
//  if (reportEnd(__FUNCTION__))
//    ;
}


void ParticlePair3DAnalyzer::scaleHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  double scalingFactor;
  if (reportInfo(__FUNCTION__)) printEventStatistics();
  int index = 0;
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    long nAccepted = getNEventsAcceptedReset();
    if (nAccepted>1)
      {

      scalingFactor = 1.0/double(nAccepted);
      for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        index = iEventFilter*nParticleFilters + iParticleFilter;
        baseSingleHistograms[index]->scale(scalingFactor);
        }
      for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
        {
        for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
          {
          index = iEventFilter*nParticleFilters*nParticleFilters + iParticleFilter1*nParticleFilters + iParticleFilter2;
          basePairHistograms[index]->scale(scalingFactor);
          }
        }
      }
    else
      {
      if (reportWarning(__FUNCTION__))
        {
        cout << endl;
        cout << "                            iEventFilter: " <<  iEventFilter<< endl;
        cout << "      nEventsAcceptedTotal[iEventFilter]: " <<  nAccepted<< endl;
        cout << "                    no scaling performed: " <<  endl;
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

Task * ParticlePair3DAnalyzer::getDerivedCalculator()
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
  Task * calculator = new ParticlePair3DDerivedHistogramCalculator(nameD,derivedCalcConfiguration,eventFilters,particleFilters,getReportLevel());
  return calculator;
}
