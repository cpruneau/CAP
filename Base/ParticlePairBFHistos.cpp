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
#include "ParticlePairBFHistos.hpp"

ClassImp(ParticlePairBFHistos);

ParticlePairBFHistos::ParticlePairBFHistos(const TString & name,
                                           ParticlePairBFHistosConfiguration * configuration,
                                           LogLevel  debugLevel)
:
Histograms(name,configuration,debugLevel),
fillEta(  configuration.fillEta),
fillY(    configuration.fillY),
fillP2(   configuration.fillP2),
nBins_n2( configuration.nBins_n2),
min_n2(   configuration.min_n2),
max_n2(   configuration.max_n2),
nBins_pt( configuration.nBins_pt),
min_pt(   configuration.min_pt),
max_pt(   configuration.max_pt),
range_pt( configuration.max_pt-configuration.min_pt),
nBins_phi(configuration.nBins_phi),
min_phi(  configuration.min_phi),
max_phi(  configuration.max_phi),
range_phi(configuration.max_phi-configuration.min_phi),
nBins_eta(configuration.nBins_eta),
min_eta(  configuration.min_eta),
max_eta(  configuration.max_eta),
range_eta(configuration.max_eta-configuration.min_eta),
nBins_y(  configuration.nBins_y),
min_y(    configuration.min_y),
max_y(    configuration.max_y),
range_y(  configuration.max_y-configuration.min_y),
nBins_Dphi(configuration.nBins_Dphi),
min_Dphi(  configuration.min_Dphi),
max_Dphi(  configuration.max_Dphi),
nBins_Deta(configuration.nBins_Deta),
min_Deta(  configuration.min_Deta),
max_Deta(  configuration.max_Deta),
range_Deta(configuration.max_Deta-configuration.min_Deta),
nBins_Dy(  configuration.nBins_Dy),
min_Dy(    configuration.min_Dy),
max_Dy(    configuration.max_Dy),
range_Dy(  configuration.max_Dy-configuration.min_Dy),
rho1_1(nullptr),
rho1_1B(nullptr),
rho1_2(nullptr),
rho1_2B(nullptr),
rho1_1_phiY(nullptr),
rho1_1B_phiY(nullptr),
rho1_2_phiY(nullptr),
rho1_2B_phiY(nullptr),
r2_12(nullptr),
r2_1B2(nullptr),
r2_12B(nullptr),
r2_1B2B(nullptr),
r2CI(nullptr),
r2CD(nullptr),
b12B(nullptr),
b1B2(nullptr),
h_x(nullptr),
h_y(nullptr),
r2Diff(nullptr),
b2Diff(nullptr)
{
  setClassName("ParticlePairBFHistos");
  setInstanceName(name);
}

ParticlePairBFHistos(const TString & name,
                     const Configuration & configuration,
                     LogLevel  debugLevel);
virtual ~ParticlePairBFHistos();
virtual void createHistograms();
virtual void loadHistograms(TFile * inputFile);
virtual void calculateBalanceFunctions(TFile * inputFile);

ParticlePairBFHistos::~ParticlePairBFHistos()
{
  // no ops
}

void ParticlePairBFHistos::createHistograms()
{
  if ( reportStart("ParticlePairBFHistos",getName(),"createHistograms()"))
    { }
  TString bn = getHistoBaseName();
  Configuration & configuration = getConfiguration();
  nBins_n2 = configuration.getParameterInt("nBins_n2");
  min_n2 = configuration.getParameterDouble("min_n2");
  max_n2 = configuration.getParameterDouble("max_n2");
  h_n2 = createHistogram(bn+TString("n2"), nBins_n2,  min_n2, max_n2, "n_{2}", "Yield");
   
  if ( reportEnd("ParticlePairBFHistos",getName(),"createHistograms()"))
    { }
}

//________________________________________________________________________
void ParticlePairBFHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart("ParticlePairBFHistos",getName(),"loadHistograms(TFile * inputFile)"))
    { }
  if (!inputFile)
    {
    if (reportFatal("ParticlePairBFHistos",getName(),"loadHistograms(TFile * inputFile)"))
      {
      cout << "Attempting to load ParticleHistos from an invalid (null) file pointer" << endl;
      }
    return;
    }
  TString bn = getHistoBaseName();

  
  if ( reportEnd("ParticlePairBFHistos",getName(),"loadHistograms(TFile * inputFile)"))
    { }
}

void ParticlePairBFHistos::calculateBalanceFunctions();
{
}

