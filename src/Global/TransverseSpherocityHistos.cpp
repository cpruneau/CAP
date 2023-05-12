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
#include "TransverseSpherocityHistos.hpp"
using CAP::TransverseSpherocityHistos;
using CAP::Particle;

ClassImp(TransverseSpherocityHistos);

TransverseSpherocityHistos::TransverseSpherocityHistos(Task * _parent,
                                                       const String & _name,
                                                       const Configuration & _configuration,
                                                       vector<ParticleFilter*> & _particleFilters)
:
HistogramGroup(_parent,_name,_configuration),
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
}

void TransverseSpherocityHistos::createHistograms()
{
  if ( reportStart(__FUNCTION__))
    { }
  const String & bn  = getName();
  const String & ptn = getParentName();
  const String & ppn = getParentPathName();
  const Configuration & configuration = getConfiguration();
  fillS0     = configuration.getValueBool(ppn,"FillS0");
  fillS1     = configuration.getValueBool(ppn,"FillS1");
  fillS1VsS0 = configuration.getValueBool(ppn,"FillS1VsS0");
  fillCorrelationHistos   = configuration.getValueBool(ppn,"FillCorrelationHistos");
  int nBins_spherocity    = configuration.getValueInt(ppn,"nBins_spherocity");
  double min_spherocity   = configuration.getValueDouble(ppn,"Min_spherocity");
  double max_spherocity   = configuration.getValueDouble(ppn,"Max_spherocity");
  nParticleFilters = particleFilters.size();

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  S:Parent Task Name....................: " << ptn << endl;
    cout << "  S:Parent Path Name....................: " << ppn << endl;
    cout << "  S:Histo Base Name.....................: " << bn << endl;
    cout << "  S:nParticleFilters....................: " << nParticleFilters     << endl;
    cout << "  S:fillS0..............................: " << fillS0     << endl;
    cout << "  S:fillS1..............................: " << fillS1     << endl;
    cout << "  S:fillS1VsS0..........................: " << fillS1VsS0 << endl;
    cout << "  S:nBins_spherocity....................: " << nBins_spherocity << endl;
    cout << "  S:min_spherocity......................: " << min_spherocity   << endl;
    cout << "  S:max_spherocity......................: " << max_spherocity   << endl;
    }
  for (int iPartFilter1=0; iPartFilter1<nParticleFilters; iPartFilter1++ )
    {
    String pfn1 = particleFilters[iPartFilter1]->getName();
    if (fillS0) h_s0.push_back( createHistogram(createName(bn,pfn1,"S0"), nBins_spherocity,min_spherocity,max_spherocity,"S_{0}","N"));
    if (fillS1) h_s1.push_back( createHistogram(createName(bn,pfn1,"S1"), nBins_spherocity,min_spherocity,max_spherocity,"S_{1}","N"));
    if (fillS1VsS0)
      h_s1VsS0.push_back( createHistogram(createName(bn,pfn1,"S1VsS0"),
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
          String pfn2 = particleFilters[iPartFilter2]->getName();
          h_s0VsS0.push_back( createHistogram(createName(bn,pfn1,"S0VsS0"),
                                              nBins_spherocity,min_spherocity,max_spherocity,
                                              nBins_spherocity,min_spherocity,max_spherocity,
                                              "S_{0}","S_{0}","N"));
          }
      }
    }
    if ( reportEnd(__FUNCTION__))
    { }
}

void TransverseSpherocityHistos::importHistograms(TFile & inputFile)
{
  if ( reportStart(__FUNCTION__))
    { }
  const String & bn  = getName();
  const String & ptn = getParentName();
  const String & ppn = getParentPathName();
  const Configuration & configuration = getConfiguration();
  fillS0     = configuration.getValueBool(ppn,"FillS0");
  fillS1     = configuration.getValueBool(ppn,"FillS1");
  fillS1VsS0 = configuration.getValueBool(ppn,"FillS1VsS0");
  fillCorrelationHistos = configuration.getValueBool(ppn,"FillCorrelationHistos");
  nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  S:Parent Task Name....................: " << ptn << endl;
    cout << "  S:Parent Path Name....................: " << ppn << endl;
    cout << "  S:Histo Base Name.....................: " << bn << endl;
    cout << "  S:nParticleFilters....................: " << nParticleFilters     << endl;
    cout << "  S:fillS0..............................: " << fillS0     << endl;
    cout << "  S:fillS1..............................: " << fillS1     << endl;
    cout << "  S:fillS1VsS0..........................: " << fillS1VsS0 << endl;
    }
  for (int iPartFilter1=0; iPartFilter1<nParticleFilters; iPartFilter1++ )
    {
    String pfn1 = particleFilters[iPartFilter1]->getName();
    if (fillS0) h_s0.push_back( loadH1(inputFile, createName(bn,pfn1,"S0")));
    if (fillS1) h_s1.push_back( loadH1(inputFile, createName(bn,pfn1,"S1")));
    if (fillS1VsS0) h_s1VsS0.push_back( loadH2(inputFile, createName(bn,pfn1,"S1VsS0")));
    if (fillCorrelationHistos)
      {
      for (int iPartFilter2=iPartFilter1+1; iPartFilter2<nParticleFilters; iPartFilter2++ )
          {
          String pfn2 = particleFilters[iPartFilter2]->getName();
          h_s0VsS0.push_back( loadH2(inputFile, createName(bn,pfn1,pfn2,"S0VsS0")));
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

