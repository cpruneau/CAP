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
#include "ParticleDerivedHistos.hpp"
#include "ParticleDerivedHistogramCalculator.hpp"

ClassImp(ParticleDerivedHistogramCalculator);

ParticleDerivedHistogramCalculator::ParticleDerivedHistogramCalculator(const TString &          _name,
                                                                       Configuration &          _configuration,
                                                                       vector<EventFilter*>   & _eventFilters,
                                                                       vector<ParticleFilter*>& _particleFilters,
                                                                       LogLevel                 _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  appendClassName("ParticleDerivedHistogramCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

//!
//!  Contructor Version 2 -- Object will get all parameters and histograms for the ParticleAnalyzer object.
//!
ParticleDerivedHistogramCalculator::ParticleDerivedHistogramCalculator(const TString &    _name,
                                                                       ParticleAnalyzer * _analyzer,
                                                                       LogLevel           _selectedLevel)
:
Task(_name,_analyzer->getConfiguration(),_analyzer->getEventFilters(),_analyzer->getParticleFilters(),_selectedLevel)
{
  appendClassName("ParticleDerivedHistogramCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  baseSingleHistograms = _analyzer->getBaseSingleHistograms();
}

void ParticleDerivedHistogramCalculator::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration &  configuration = getConfiguration();
  configuration.setName("ParticleDerivedHistogramCalculator Configuration");
  configuration.setParameter("createHistograms", true);
  configuration.setParameter("loadHistograms", true);
  configuration.setParameter("saveHistograms", true);
   
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

  configuration.addParameter("nBins_phiEta",     720);
  configuration.addParameter("nBins_phiEtaPt",  7200);
  configuration.addParameter("nBins_phiY",       720);
  configuration.addParameter("nBins_phiYPt",    7200);
  configuration.addParameter("fillEta",   true);
  configuration.addParameter("fillY",    false);
  configuration.addParameter("fillP2",   false);

  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

//!
//!  Create derived histogram objects.
//!
void ParticleDerivedHistogramCalculator::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString bn  = getName();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Creating Histogram(s) for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  ParticleDerivedHistos * histos;
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      histos = new ParticleDerivedHistos(makeHistoName(bn,efn,pfn),configuration,getReportLevel());
      histos->createHistograms();
      derivedSingleHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void ParticleDerivedHistogramCalculator::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString bn  = getName();
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "Loading Histogram(s) for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }

  ParticleHistos     * histos;
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    // singles
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle filter (Singles):" << pfn << endl;
      histos = new ParticleHistos(makeHistoName(bn,efn,pfn),configuration,getReportLevel());
      histos->loadHistograms(inputFile);
      baseSingleHistograms.push_back(histos);
      }
    }

  if (reportEnd(__FUNCTION__))
    ;
}

void ParticleDerivedHistogramCalculator::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Computing derived histograms for: " << endl;
    cout << "                   nEventFilters: " << nEventFilters << endl;
    cout << "                nParticleFilters: " << nParticleFilters << endl;
    }
  ParticleHistos        * baseHistos;
  ParticleDerivedHistos * derivedHistos;
  unsigned index;

  //!Mode 1: Running rigth after Analysis: base histograms pointers  are copied from analyzer to baseSingleHistograms
  //!Mode 2: Running as standalone: base histograms are loaded from file.
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      index = iEventFilter*nParticleFilters + iParticleFilter;
      baseHistos    = (ParticleHistos *) baseSingleHistograms[index];
      derivedHistos = (ParticleDerivedHistos *) derivedSingleHistograms[index];
      derivedHistos->calculateDerivedHistograms(baseHistos);
      }
    }
  if (reportEnd(__FUNCTION__))
   { }
}
