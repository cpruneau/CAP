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
#include "NuDynDerivedHistogramCalculator.hpp"

ClassImp(NuDynDerivedHistogramCalculator);

NuDynDerivedHistogramCalculator::NuDynDerivedHistogramCalculator(const TString &                _name,
                                                                       Configuration &          _configuration,
                                                                       vector<EventFilter*>   & _eventFilters,
                                                                       vector<ParticleFilter*>& _particleFilters,
                                                                       LogLevel                 _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  appendClassName("NuDynDerivedHistogramCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void NuDynDerivedHistogramCalculator::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration &  configuration = getConfiguration();
  configuration.setName("NuDynDerivedHistogramCalculator Configuration");
  configuration.setParameter("createHistograms", true);
  configuration.setParameter("loadHistograms", true);
  configuration.setParameter("saveHistograms", true);
  configuration.addParameter("inputType",1);
  configuration.addParameter("pairOnly",true);
  configuration.addParameter("nBins_mult",200);
  configuration.addParameter("min_mult", 0.0);
  configuration.addParameter("max_mult",200.0);
  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

//!
//!  Create derived histogram objects.
//!
void NuDynDerivedHistogramCalculator::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "Creating Histogram(s) for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  NuDynDerivedHistos * histos;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString partFilterName = particleFilters[iParticleFilter]->getName();
      TString histoName  = prefixName;
      histoName += evtFilterName;
      histoName += "_";
      histoName += partFilterName;
      histos = new NuDynDerivedHistos(histoName,configuration,getReportLevel());
      histos->createHistograms();
      derivedPairHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void NuDynDerivedHistogramCalculator::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << "Loading Histogram(s) for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }

  NuDynHistos     * histos;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    // singles
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug(__FUNCTION__))cout << "Particle filter (Singles):" << pfn << endl;
      histos = new NuDynHistos(makeHistoName(prefixName,efn,pfn,""),configuration,getReportLevel());
      histos->loadHistograms(inputFile);
      baseSingleHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void NuDynDerivedHistogramCalculator::execute()
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
