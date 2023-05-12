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
using CAP::GlobalDerivedHistos;

ClassImp(GlobalDerivedHistos);

GlobalDerivedHistos::GlobalDerivedHistos(Task * _parent,
                                         const String & _name,
                                         const Configuration & _configuration,
                                         vector<ParticleFilter*> _particleFilters)
:
HistogramGroup(_parent,_name,_configuration),
particleFilters(_particleFilters),
h_ptAvgInc(),
h_ptAvgIncVsN()
{
  appendClassName("GlobalDerivedHistos");
}

// for now use the same boundaries for eta and y histogram
void GlobalDerivedHistos::createHistograms()
{
  if ( reportStart(__FUNCTION__))
    { }
  const Configuration & configuration = getConfiguration();
  const String & bn  = getName();
  const String & ptn = getParentName( );
  const String & ppn = getParentPathName();
  int nBins_n  = configuration.getValueInt(ppn,"nBins_n");
  double min_n = configuration.getValueDouble(ppn,"Min_n");
  double max_n = configuration.getValueDouble(ppn,"Max_n");
  unsigned int nParticleFilters = particleFilters.size();

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  G:Parent Task Name....................: " << ptn << endl;
    cout << "  G:Parent Path Name....................: " << ppn << endl;
    cout << "  G:Histo Base Name.....................: " << bn << endl;
    cout << "  G:Derived:nBins_n.....................: " << nBins_n << endl;
    cout << "  G:Derived:Min_n.......................: " << min_n << endl;
    cout << "  G:Derived:Max_n.......................: " << max_n << endl;
    }

  for (unsigned int k1=0; k1<nParticleFilters; k1++)
    {
    String pfName1 = particleFilters[k1]->getName();
    h_ptAvgInc.push_back(    createHistogram(createName(bn,pfName1,"nWB",pfName1,"ptAvgIncl"),  1,       min_n,  max_n,"n","ptAvgIncl")  );
    h_ptAvgIncVsN.push_back( createHistogram(createName(bn,pfName1,"n",  pfName1,"ptAvgIncl"),  nBins_n, min_n,  max_n,"n","ptAvgIncl") );
    }
  if ( reportEnd(__FUNCTION__))
    { }
}

//________________________________________________________________________
void GlobalDerivedHistos::importHistograms(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  const String & bn  = getName();
  const String & ptn = getParentName();
  const String & ppn = getParentPathName();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  G:Parent Task Name....................: " << ptn << endl;
    cout << "  G:Parent Path Name....................: " << ppn << endl;
    cout << "  G:Histo Base Name.....................: " << bn << endl;
    }

  for (unsigned int k1=0; k1<nParticleFilters; k1++)
    {
    String pfName1 = particleFilters[k1]->getName();
    h_ptAvgInc.push_back(    loadH1(inputFile, createName(bn,pfName1,"nWB",pfName1,"ptAvgIncl")));
    h_ptAvgIncVsN.push_back( loadH1(inputFile, createName(bn,pfName1,"n",  pfName1,"ptAvgIncl")));
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

