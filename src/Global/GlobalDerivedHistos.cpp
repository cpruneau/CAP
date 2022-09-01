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

GlobalDerivedHistos::GlobalDerivedHistos(Task * _parent,
                                         const TString & _name,
                                         Configuration & _configuration,
                                         vector<ParticleFilter*> _particleFilters)
:
Histograms(_parent,_name,_configuration),
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
  Configuration & configuration = getConfiguration();
  const TString & bn  = getName();
  const TString & ptn = getParentTaskName();
  const TString & ppn = getParentPathName();
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
  const TString & bn  = getName();
  const TString & ptn = getParentTaskName();
  const TString & ppn = getParentPathName();
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

