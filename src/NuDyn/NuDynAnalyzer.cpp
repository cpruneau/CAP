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
#include "NuDynAnalyzer.hpp"
#include "NuDynHistos.hpp"
#include "NuDynDerivedHistos.hpp"
using CAP::NuDynAnalyzer;

ClassImp(NuDynAnalyzer);

NuDynAnalyzer::NuDynAnalyzer(const String & _name,
                             Configuration & _configuration,
                             vector<EventFilter*> & _eventFilters,
                             vector<ParticleFilter*> & _particleFilters)
:
Task(_name, _configuration, _eventFilters, _particleFilters),
multiplicityType(1)
{
  appendClassName("NuDynAnalyzer");
}

void NuDynAnalyzer::setDefaultConfiguration()
{
  setParameter("UseParticles",      true);
  setParameter("CreateHistograms",  true);
  setParameter("SaveHistograms",    true);
  setParameter("UseEventStream0",   true);
  setParameter("UseEventStream1",   false);
  addParameter("InputType",         1);
  addParameter("PairOnly",          true);
  addParameter("nBins_mult",        200);
  addParameter("Min_mult",          0.0);
  addParameter("Max_mult",          200.0);
  addParameter("nBins_rapidity",    99);
  addParameter("Min_rapidity",      0.100);
  addParameter("Max_rapidity",      10.00);
}

void NuDynAnalyzer::initialize()
{
  Task::initialize();
  multiplicityType = getValueInt("InputType");

  int    nBins_rapidity = configuration.getValueInt("nBins_rapidity");
  double min_rapidity   = configuration.getValueDouble("Min_rapidity");
  double max_rapidity   = configuration.getValueDouble("Max_rapidity");
  double width_rapidity = (max_rapidity-min_rapidity)/double(nBins_rapidity);

  if (reportInfo(__FUNCTION__))
    {
    cout << " NuDyn:multiplicityType....................: " << multiplicityType  << endl;
    cout << " NuDyn:nBins_rapidity......................: " << nBins_rapidity    << endl;
    cout << " NuDyn:min_rapidity........................: " << min_rapidity      << endl;
    cout << " NuDyn:max_rapidity........................: " << max_rapidity      << endl;
    cout << " NuDyn:width_rapidity......................: " << width_rapidity    << endl;
    }

  for (int iEta=0; iEta<nBins_rapidity; iEta++)
    {
    deltaRapidtyBin.push_back(min_rapidity+double(iEta)*width_rapidity );
    }

}

void NuDynAnalyzer::createHistograms()
{
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  String prefixName = getName(); prefixName += "_";
  String evtFilterName;
  String histoName;
  String partFilterName;
  if (reportInfo(__FUNCTION__))
    {
    cout << " NuDyn:Creating HistogramGroup for......: "  << endl;
    cout << " NuDyn:nEventFilters................: " << nEventFilters << endl;
    cout << " NuDyn:nParticleFilters.............: " << nParticleFilters << endl;
    }
  partFilterName = particleFilters[0]->getName();
  partFilterName += particleFilters[1]->getName();
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    evtFilterName  = eventFilters[iEventFilter]->getName();
    histoName      = prefixName;
    histoName      += evtFilterName;
    histoName      += "_";
    histoName      += partFilterName;
    NuDynHistos * nuDynHistos = new NuDynHistos(this,histoName,getConfiguration());
    nuDynHistos->createHistograms();
    histograms.push_back(nuDynHistos);
    }
  if (reportEnd(__FUNCTION__))
    { }
}

void NuDynAnalyzer::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  String prefixName = getName(); prefixName += "_";
  String evtFilterName;
  String histoName;
  String partFilterName;
  
  if (reportInfo(__FUNCTION__))
    {
    cout << " Creating HistogramGroup for......"  << endl;
    cout << " nEventFilters................: " << nEventFilters << endl;
    cout << " nParticleFilters.............: " << nParticleFilters << endl;
    }
  partFilterName = particleFilters[0]->getName();
  partFilterName += particleFilters[1]->getName();

  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    evtFilterName  = eventFilters[iEventFilter]->getName();
    histoName      = prefixName;
    histoName      += evtFilterName;
    histoName      += "_";
    histoName      += partFilterName;
    NuDynHistos * nuDynHistos = new NuDynHistos(this,histoName,getConfiguration());
    nuDynHistos->loadHistograms(inputFile);
    histograms.push_back(nuDynHistos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void NuDynAnalyzer::execute()
{
  incrementTaskExecuted();
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  Event * event = eventStreams[0];
  resetNParticlesAcceptedEvent();
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(*event)) continue;
    incrementNEventsAccepted(iEventFilter); // count eventStreams used to fill histograms and for scaling at the end..

    int nBins_rapidity = deltaRapidtyBin.size();
    vector<double> nAccepted0(nBins_rapidity,0.0);
    vector<double> nAccepted1(nBins_rapidity,0.0);
    double rapidity;
    for (unsigned long  iParticle=0; iParticle<event->getNParticles(); iParticle++)
      {
      Particle & particle = * event->getParticleAt(iParticle);
      if (particleFilters[0]->accept(particle))
        {
        incrementNParticlesAccepted(iEventFilter,0);
        rapidity = fabs(particle.getMomentum().Rapidity());
        for (int iY; iY<nBins_rapidity; iY++)
          {
          if (rapidity<deltaRapidtyBin[iY]) nAccepted0[iY]++;
          }
        }
      if (particleFilters[1]->accept(particle))
        {
        incrementNParticlesAccepted(iEventFilter,1);
        rapidity = fabs(particle.getMomentum().Rapidity());
        for (int iY; iY<nBins_rapidity; iY++)
          {
          if (rapidity<deltaRapidtyBin[iY]) nAccepted1[iY]++;
          }
        }
      }
    EventProperties ep = * event->getEventProperties();
    NuDynHistos * nuDynHistos = (NuDynHistos *) histograms[iEventFilter];
    switch ( multiplicityType )
      {
        case 0: nuDynHistos->fill(ep.fractionalXSection, nAccepted0, nAccepted1, 1.0); break;
        case 1: nuDynHistos->fill(ep.refMultiplicity,    nAccepted0, nAccepted1, 1.0); break;
        case 2: nuDynHistos->fill(ep.refMultiplicity,    nAccepted0, nAccepted1, 1.0); break;
      }
    }
}

void NuDynAnalyzer::createDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  String prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << " Creating HistogramGroup for......"  << endl;
    cout << " nEventFilters................: " << nEventFilters << endl;
    cout << " nParticleFilters.............: " << nParticleFilters << endl;
    }
  NuDynDerivedHistos * histos;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String evtFilterName = eventFilters[iEventFilter]->getName();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      String partFilterName = particleFilters[iParticleFilter]->getName();
      String histoName  = prefixName;
      histoName += evtFilterName;
      histoName += "_";
      histoName += partFilterName;
      histos = new NuDynDerivedHistos(this,histoName,configuration);
      histos->createHistograms();
      derivedPairHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void NuDynAnalyzer::loadDerivedHistograms(TFile * inputFile __attribute__((unused)))
{

}

void NuDynAnalyzer::calculateDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  //incrementTaskExecuted();
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Computing derived histograms for: " << endl;
    cout << "                   nEventFilters: " << nEventFilters << endl;
    cout << "                nParticleFilters: " << nParticleFilters << endl;
    }
  NuDynHistos        * baseHistos;
  NuDynDerivedHistos * derivedHistos;
  unsigned index;

  //!Mode 1: Running rigth after Analysis: base histograms pointers  are copied from analyzer to baseSingleHistograms
  //!Mode 2: Running as standalone: base histograms are loaded from file.
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      index = iEventFilter*nParticleFilters + iParticleFilter;
      baseHistos    = (NuDynHistos *) baseSingleHistograms[index];
      derivedHistos = (NuDynDerivedHistos *) derivedSingleHistograms[index];
      derivedHistos->calculateDerivedHistograms(baseHistos);
      }
    }
  if (reportEnd(__FUNCTION__))
    { }
}
