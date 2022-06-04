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
#include "NuDynDerivedHistogramCalculator.hpp"

ClassImp(NuDynAnalyzer);

NuDynAnalyzer::NuDynAnalyzer(const TString &         _name,
                             const Configuration &   _configuration,
                             vector<EventFilter*>    _eventFilters,
                             vector<ParticleFilter*> _particleFilters,
                             LogLevel                _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel),
multiplicityType(1)
{
  appendClassName("NuDynAnalyzer");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void NuDynAnalyzer::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  configuration.setName("NuDynAnalyzer Configuration");
  configuration.setParameter("useParticles",      true);
  configuration.setParameter("histoAnalyzerName", TString("NuDyn"));
  configuration.setParameter("histoBaseName",     TString("NuDyn"));
  configuration.addParameter("inputType",         1);
  configuration.addParameter("pairOnly",          true);
  configuration.addParameter("nBins_mult",        200);
  configuration.addParameter("min_mult",          0.0);
  configuration.addParameter("max_mult",          200.0);
  if (reportDebug("NuDynAnalyzer",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}

void NuDynAnalyzer::initialize()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  multiplicityType = configuration.getValueInt("inputType");
}

void NuDynAnalyzer::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  TString prefixName = getName(); prefixName += "_";
  TString evtFilterName;
  TString histoName;
  TString partFilterName;
  if (reportInfo(__FUNCTION__))
    {
    cout << "Creating Histograms for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  partFilterName = particleFilters[0]->getName();
  for (unsigned int iParticleFilter=1; iParticleFilter<nParticleFilters; iParticleFilter++ )
    {
    partFilterName += particleFilters[iParticleFilter]->getName();
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    evtFilterName  = eventFilters[iEventFilter]->getName();
    histoName      = prefixName;
    histoName      += evtFilterName;
    histoName      += "_";
    histoName      += partFilterName;
    NuDynHistos * nuDynHistos = new NuDynHistos(histoName,getConfiguration(),getReportLevel());
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
  TString prefixName = getName(); prefixName += "_";
  TString evtFilterName;
  TString histoName;
  TString partFilterName;
  
  if (reportDebug(__FUNCTION__))
    {
    cout << "Creating Histograms for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  partFilterName = particleFilters[0]->getName();
  for (unsigned int iParticleFilter=1; iParticleFilter<nParticleFilters; iParticleFilter++ )
    {
    partFilterName += particleFilters[iParticleFilter]->getName();
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    evtFilterName  = eventFilters[iEventFilter]->getName();
    histoName      = prefixName;
    histoName      += evtFilterName;
    histoName      += "_";
    histoName      += partFilterName;
    NuDynHistos * nuDynHistos = new NuDynHistos(histoName,getConfiguration(),getReportLevel());
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
    
    vector<double> nAccepted(nParticleFilters,0.0);
    for (unsigned long  iParticle=0; iParticle<event->getNParticles(); iParticle++)
      {
      Particle & particle = * event->getParticleAt(iParticle);
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++)
        {
        if (particleFilters[iParticleFilter]->accept(particle))
          {
          incrementNParticlesAccepted(iEventFilter,iParticleFilter);
          nAccepted[iParticleFilter]++;
          }
        }
      }
    EventProperties ep = * event->getEventProperties();
    NuDynHistos * nuDynHistos = (NuDynHistos *) histograms[iEventFilter];
    switch ( multiplicityType )
      {
        case 0: nuDynHistos->fill(ep.fractionalXSection,   nAccepted,1.0); break;
        case 1: nuDynHistos->fill(ep.referenceMultiplicity, nAccepted,1.0); break;
        case 2: nuDynHistos->fill(ep.referenceMultiplicity, nAccepted,1.0); break;
      }
    }
}


Task * NuDynAnalyzer::getDerivedCalculator()
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
  Task * calculator = new NuDynDerivedHistogramCalculator(nameD,derivedCalcConfiguration,eventFilters,particleFilters,getReportLevel());
  return calculator;
}
