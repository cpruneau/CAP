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
#include "Particle.hpp"
#include "TLorentzVector.h"
#include "TransverseSpherocityHistos.hpp"

ClassImp(TransverseSpherocityHistos);

TransverseSpherocityHistos::TransverseSpherocityHistos(const TString &           _name,
                                                       const Configuration &     _configuration,
                                                       vector<ParticleFilter*> & _particleFilters,
                                                       LogLevel                  _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
fillS0(true),
fillS1(false),
fillS1VsS0(false),
fillCorrelationHistos(false),
nParticleFilters(0),
particleFilters(_particleFilters),
h_s0(),
h_s1(),
h_s1VsS0(),
h_s0VsS0()
{
  appendClassName("TransverseSpherocityHistos");
  setInstanceName(_name);
}

void TransverseSpherocityHistos::createHistograms()
{
  if ( reportStart("TransverseSpherocityHistos",getName(),"createHistograms()"))
    { }
  TString bn = getHistoBaseName();
  Configuration & configuration = getConfiguration();
  fillS0     = configuration.getValueBool("fillS0");
  fillS1     = configuration.getValueBool("fillS1");
  fillS1VsS0 = configuration.getValueBool("fillS1VsS0");
  fillCorrelationHistos   = configuration.getValueBool("fillCorrelationHistos");
  int nBins_spherocity    = configuration.getValueInt("nBins_spherocity");
  double min_spherocity   = configuration.getValueDouble("min_spherocity");
  double max_spherocity   = configuration.getValueDouble("max_spherocity");
  nParticleFilters = particleFilters.size();
  for (int iPartFilter1=0; iPartFilter1<nParticleFilters; iPartFilter1++ )
    {
    TString pfn1 = particleFilters[iPartFilter1]->getName();
    if (fillS0) h_s0.push_back( createHistogram(makeName(bn,pfn1,"S0"), nBins_spherocity,min_spherocity,max_spherocity,"S_{0}","N"));
    if (fillS1) h_s1.push_back( createHistogram(makeName(bn,pfn1,"S1"), nBins_spherocity,min_spherocity,max_spherocity,"S_{1}","N"));
    if (fillS1VsS0)
      h_s1VsS0.push_back( createHistogram(makeName(bn,pfn1,"S1VsS0"),
                                          nBins_spherocity,min_spherocity,max_spherocity,nBins_spherocity,min_spherocity,max_spherocity,
                                          "S_{0}","S_{1}","N"));
    if (fillCorrelationHistos)
      cout << " fillCorrelationHistos IS ON w/ nParticleFilters:" << nParticleFilters << endl;
    else
      cout << " fillCorrelationHistos IS OFF" << endl;

    if (fillCorrelationHistos)
      {
      for (int iPartFilter2=iPartFilter1+1; iPartFilter2<nParticleFilters; iPartFilter2++ )
          {
          TString pfn2 = particleFilters[iPartFilter2]->getName();
          h_s0VsS0.push_back( createHistogram(makeName(bn,pfn1,"S0VsS0"),
                                              nBins_spherocity,min_spherocity,max_spherocity,
                                              nBins_spherocity,min_spherocity,max_spherocity,
                                              "S_{0}","S_{0}","N"));
          }
      }
    }
    if ( reportEnd("TransverseSpherocityHistos",getName(),"createHistograms()"))
    { }
}

void TransverseSpherocityHistos::loadHistograms(TFile * inputFile)
{
  if ( reportStart(__FUNCTION__))
    { }
  TString bn = getHistoBaseName();
  Configuration & configuration = getConfiguration();
  fillS0     = configuration.getValueBool("fillS0");
  fillS1     = configuration.getValueBool("fillS1");
  fillS1VsS0 = configuration.getValueBool("fillS1VsS0");
  fillCorrelationHistos = configuration.getValueBool("fillCorrelationHistos");
  nParticleFilters = particleFilters.size();
  for (int iPartFilter1=0; iPartFilter1<nParticleFilters; iPartFilter1++ )
    {
    TString pfn1 = particleFilters[iPartFilter1]->getName();
    if (fillS0) h_s0.push_back( loadH1(inputFile, makeName(bn,pfn1,"S0")));
    if (fillS1) h_s1.push_back( loadH1(inputFile, makeName(bn,pfn1,"S1")));
    if (fillS1VsS0) h_s1VsS0.push_back( loadH2(inputFile, makeName(bn,pfn1,"S1VsS0")));
    if (fillCorrelationHistos)
      {
      for (int iPartFilter2=iPartFilter1+1; iPartFilter2<nParticleFilters; iPartFilter2++ )
          {
          TString pfn2 = particleFilters[iPartFilter2]->getName();
          h_s0VsS0.push_back( loadH2(inputFile, makeName(bn,pfn1,pfn2,"S0VsS0")));
          }
      }
    }
  
  if ( reportEnd(__FUNCTION__))
    { }
}

//!
//! Fiil  single particle histograms of this class with the particles contained in the given list.
//!
void TransverseSpherocityHistos::fill(vector<double> & s0, vector<double> & s1, double weight)
{
//  cout << "TransverseSpherocityHistos::fill(vector<double> & s0, vector<double> & s1, double weight)" << endl;
//  cout << " h_s0.size(): " << h_s0.size()<< endl;
//  cout << " h_s1.size(): " << h_s1.size()<< endl;
//  cout << " h_s1VsS0.size(): " << h_s1VsS0.size()<< endl;
//  cout << " h_s0VsS0.size(): " << h_s0VsS0.size()<< endl;
//  cout << " s0.size(): " << s0.size()<< endl;
//  cout << " s1.size(): " << s1.size()<< endl;
  
  int k = 0;
  for (int iPartFilter1=0; iPartFilter1<nParticleFilters; iPartFilter1++ )
    {
    if (fillS0) h_s0[iPartFilter1]->Fill(s0[iPartFilter1],weight);
    if (fillS1) h_s1[iPartFilter1]->Fill(s1[iPartFilter1],weight);
    if (fillS1VsS0) h_s1VsS0[iPartFilter1]->Fill(s0[iPartFilter1],s1[iPartFilter1],weight);
    if (fillCorrelationHistos)
      {
      for (int iPartFilter2=iPartFilter1+1; iPartFilter2<nParticleFilters; iPartFilter2++ )
          {
          h_s0VsS0[k]->Fill(s0[iPartFilter1],s0[iPartFilter2],weight);
          k++;
          }
      }
    }
}

