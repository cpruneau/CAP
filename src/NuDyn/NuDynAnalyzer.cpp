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
                             const Configuration & _configuration,
                             vector<EventFilter*> & _eventFilters,
                             vector<ParticleFilter*> & _particleFilters)
:
EventTask(_name, _configuration, _eventFilters, _particleFilters),
multiplicityType(1)
{
  appendClassName("NuDynAnalyzer");
}

void NuDynAnalyzer::setDefaultConfiguration()
{
  EventTask::setDefaultConfiguration();
  addParameter("EventsAnalyze",     true);
  addParameter("HistogramsCreate",  true);
  addParameter("HistogramsExport",  true);
  addParameter("EventsUseStream0",  true);
  addParameter("EventsUseStream1",  false);
  addParameter("InputType",         1);
  addParameter("PairOnly",          true);
  addParameter("nBins_mult",        200);
  addParameter("Min_mult",          0.0);
  addParameter("Max_mult",          200.0);
  addParameter("nBins_rapidity",    99);
  addParameter("Min_rapidity",      0.100);
  addParameter("Max_rapidity",      10.00);
}

void NuDynAnalyzer::configure()
{
  EventTask::configure();
  multiplicityType = getValueInt("InputType");
  nBins_rapidity   = getValueInt("nBins_rapidity");
  min_rapidity     = getValueDouble("Min_rapidity");
  max_rapidity     = getValueDouble("Max_rapidity");
  width_rapidity   = (max_rapidity-min_rapidity)/double(nBins_rapidity);

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
    printItem("InputType",     multiplicityType);
    printItem("nBins_rapidity",nBins_rapidity);
    printItem("min_rapidity",  min_rapidity);
    printItem("max_rapidity",  max_rapidity);
    printItem("width_rapidity",width_rapidity);
    }
}

void NuDynAnalyzer::initialize()
{
  EventTask::initialize();
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
    cout << endl;
    printItem("Creating HistogramGroup","");
    printItem("nEventFilters",    int(nEventFilters));
    printItem("nParticleFilters", int(nParticleFilters));
    cout << endl;
    }
  partFilterName = particleFilters[0]->getName();
  partFilterName += particleFilters[1]->getName();
  histogramManager.addSet("NuDyn");

  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    evtFilterName  = eventFilters[iEventFilter]->getName();
    histoName      = prefixName;
    histoName      += evtFilterName;
    histoName      += "_";
    histoName      += partFilterName;
    NuDynHistos * nuDynHistos = new NuDynHistos(this,histoName,configuration);
    nuDynHistos->createHistograms();
    histogramManager.addGroupInSet(0,nuDynHistos);
    }
  if (reportEnd(__FUNCTION__))
    { }
}

void NuDynAnalyzer::importHistograms(TFile & inputFile)
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
    cout << endl;
    printItem("Loading HistogramGroup","");
    printItem("nEventFilters",    int(nEventFilters));
    printItem("nParticleFilters", int(nParticleFilters));
    cout << endl;
    }
  partFilterName = particleFilters[0]->getName();
  partFilterName += particleFilters[1]->getName();
  histogramManager.addSet("NuDyn");

  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    evtFilterName  = eventFilters[iEventFilter]->getName();
    histoName      = prefixName;
    histoName      += evtFilterName;
    histoName      += "_";
    histoName      += partFilterName;
    NuDynHistos * nuDynHistos = new NuDynHistos(this,histoName,configuration);
    nuDynHistos->importHistograms(inputFile);
    histogramManager.addGroupInSet(0,nuDynHistos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void NuDynAnalyzer::analyzeEvent()
{
  incrementTaskExecuted();
  unsigned int nEventFilters    = eventFilters.size();
  //unsigned int nParticleFilters = particleFilters.size();
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
        for (int iY=0; iY<nBins_rapidity; iY++)
          {
          if (rapidity<deltaRapidtyBin[iY]) nAccepted0[iY]++;
          }
        }
      if (particleFilters[1]->accept(particle))
        {
        incrementNParticlesAccepted(iEventFilter,1);
        rapidity = fabs(particle.getMomentum().Rapidity());
        for (int iY=0; iY<nBins_rapidity; iY++)
          {
          if (rapidity<deltaRapidtyBin[iY]) nAccepted1[iY]++;
          }
        }
      }
    EventProperties ep = * event->getEventProperties();
    NuDynHistos * nuDynHistos = (NuDynHistos *)  histogramManager.getGroup(0,iEventFilter);
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
  String prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("Create Derived HistogramGroup","");
    printItem("nEventFilters",    int(nEventFilters));
    printItem("nParticleFilters", int(nParticleFilters));
    cout << endl;
    }
  histogramManager.addSet("NuDynDerived");
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
      histogramManager.addGroupInSet(1,histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void NuDynAnalyzer::importDerivedHistograms(TFile & inputFile __attribute__((unused)))
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
    printItem("Calculate Derived Histograms","");
    printItem("nEventFilters",    int(nEventFilters));
    printItem("nParticleFilters", int(nParticleFilters));
    cout << endl;
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
      baseHistos    = (NuDynHistos *) histogramManager.getGroup(0,index);
      derivedHistos = (NuDynDerivedHistos *) histogramManager.getGroup(1,index);
      derivedHistos->calculateDerivedHistograms(baseHistos);
      }
    }
  if (reportEnd(__FUNCTION__))
    { }
}
