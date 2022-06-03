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
#include "GlobalDerivedHistos.hpp"
#include "GlobalDerivedHistogramCalculator.hpp"

ClassImp(GlobalDerivedHistogramCalculator);

GlobalDerivedHistogramCalculator::GlobalDerivedHistogramCalculator(const TString &          _name,
                                                                   Configuration &          _configuration,
                                                                   vector<EventFilter*>   & _eventFilters,
                                                                   vector<ParticleFilter*>& _particleFilters,
                                                                   LogLevel                 _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  appendClassName("GlobalDerivedHistogramCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

//!
//!  Contructor Version 2 -- Object will get all parameters and histograms for the ParticleAnalyzer object.
//!
GlobalDerivedHistogramCalculator::GlobalDerivedHistogramCalculator(const TString &    _name,
                                                                   ParticleAnalyzer * _analyzer,
                                                                   LogLevel           _selectedLevel)
:
Task(_name,_analyzer->getConfiguration(),_analyzer->getEventFilters(),_analyzer->getParticleFilters(),_selectedLevel)
{
  appendClassName("GlobalDerivedHistogramCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  baseSingleHistograms = _analyzer->getBaseSingleHistograms();
}

void GlobalDerivedHistogramCalculator::setDefaultConfiguration()
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration &  configuration = getConfiguration();
  configuration.setName("GlobalDerivedHistogramCalculator Configuration");
  configuration.setParameter("createHistograms", true);
  configuration.setParameter("loadHistograms",   true);
  configuration.setParameter("saveHistograms",   true);
  configuration.addParameter("nBins_n", 100);
  configuration.addParameter("nBins_n2",20);
  configuration.addParameter("min_n",   0.0);
  configuration.addParameter("max_n",   1000.0);
  configuration.addParameter("range_n", 1000.0);
  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

//!
//!  Create derived histogram objects.
//!
void GlobalDerivedHistogramCalculator::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString bn  = getName();
  if (reportDebug(__FUNCTION__))
    {
    cout << "Creating Histograms for : " << bn  << endl;
    cout << "          nEventFilters : " << nEventFilters << endl;
    cout << "       nParticleFilters : " << nParticleFilters << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    GlobalDerivedHistos * histos = new GlobalDerivedHistos(makeHistoName(bn,efn),configuration,particleFilters,getReportLevel());
    histos->createHistograms();
    derivedHistograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void GlobalDerivedHistogramCalculator::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString bn  = getName();

  if (reportDebug(__FUNCTION__))
    {
    cout << "Loading Histograms for " << bn  << endl;
    cout << "        nEventFilters: " << nEventFilters << endl;
    cout << "     nParticleFilters: " << nParticleFilters << endl;
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

void GlobalDerivedHistogramCalculator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "Computing derived histograms for: " << endl;
    cout << "                   nEventFilters: " << nEventFilters << endl;
    cout << "                nParticleFilters: " << nParticleFilters << endl;
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
