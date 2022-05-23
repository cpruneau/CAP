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
ClassImp(GlobalDerivedHistos);

GlobalDerivedHistos::GlobalDerivedHistos(const TString & _name,
                                         const Configuration & _configuration,
                                         vector<ParticleFilter*> _particleFilters,
                                         LogLevel  _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
particleFilters(_particleFilters),
h_ptAvgInc(),
h_ptAvgIncVsN()
{
  appendClassName("GlobalDerivedHistos");
  setInstanceName(name);
}

// for now use the same boundaries for eta and y histogram
void GlobalDerivedHistos::createHistograms()
{
  if ( reportStart(__FUNCTION__))
    { }
  TString bn = getHistoBaseName();
  const Configuration & configuration = getConfiguration();
  int nBins_n  = configuration.getValueInt("nBins_n");
  //int nBins_n2 = configuration.getValueInt("nBins_n2");
  double min_n = configuration.getValueDouble("min_n");
  double max_n = configuration.getValueDouble("max_n");
  unsigned int nParticleFilters = particleFilters.size();
  for (unsigned int k1=0; k1<nParticleFilters; k1++)
    {
    TString pfName1 = particleFilters[k1]->getName();
    h_ptAvgInc.push_back(    createHistogram(makeName(bn,pfName1,"nWB",pfName1,"ptAvgIncl"),  1,       min_n,  max_n,"n","ptAvgIncl")  );
    h_ptAvgIncVsN.push_back( createHistogram(makeName(bn,pfName1,"n",  pfName1,"ptAvgIncl"),  nBins_n, min_n,  max_n,"n","ptAvgIncl") );
    }
  if ( reportEnd(__FUNCTION__))
    { }
}

//________________________________________________________________________
void GlobalDerivedHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  TString bn = getHistoBaseName();
  unsigned int nParticleFilters = particleFilters.size();
  for (unsigned int k1=0; k1<nParticleFilters; k1++)
    {
    TString pfName1 = particleFilters[k1]->getName();
    h_ptAvgInc.push_back(    loadH1(inputFile, makeName(bn,pfName1,"nWB",pfName1,"ptAvgIncl")));
    h_ptAvgIncVsN.push_back( loadH1(inputFile, makeName(bn,pfName1,"n",  pfName1,"ptAvgIncl")));
    }
  if ( reportEnd(__FUNCTION__))
    ;
}

//!
//! Calculates derived histograms. This is currently limited to pt averages vs eta and phi.
//!
void GlobalDerivedHistos::calculateDerivedHistograms(GlobalHistos * baseHistos __attribute__((unused)))
{
  if (reportStart(__FUNCTION__))
    ;

  //p_ptSumVsN

  if (reportEnd(__FUNCTION__))
    ;
}

