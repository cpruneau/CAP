// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef CAP_HadronGasParticlesHistograms
#define CAP_HadronGasParticlesHistograms
#include "Histograms.hpp"

//!
//!Histograming class designed to make plots of the  properties of the  hadron gas (HG)  particle species partial quantities as
//!a function of species index or particle mass.
//!Quantities of interest for each of the (thermal) particle species included in the HG calculation include
//!- number density
//!- entropy density
//!- partial pressure
//!Quantities of interest for each of the  stable particle species included in the HG calculation include
//!- number density
//!
//!The histogram parameters are user supplied via the configuration of the HadronGasGeneratorTask which calls
//!this histogramming class to fill histograms. There should be one call per temperature/muB/muS value. Note that parameters
//!plotThermalSpeciesVsT and plotStableSpeciesVsT are toggle switches allowing to turn on/off the plotting of the
//!histograms for thermal particle and stable particles respsectively.
//!
class HadronGasHistograms : public Histograms
{
public:

  HadronGasHistograms(const TString &       _name,
                      const Configuration & _config,
                      LogLevel              _debugLevel);
  virtual ~HadronGasHistograms() {}
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  void fill(HadronGas & hadronGas);
  void fillRho1VsP(HadronGas & hadronGas, double volume);

protected:

  TH1 * h_numberDensity;
  TH1 * h_energyDensity;
  TH1 * h_entropyDensity;
  TH1 * h_pressure;

  TProfile * h_numberDensityVsMass;
  TProfile * h_energyDensityVsMass;
  TProfile * h_entropyDensityVsMass;
  TProfile * h_pressureVsMass;

  TH1 * h_rho1ThVsIndex;
  TH1 * h_rho1VsIndex;
  TH1 * h_rho1RatioVsIndex;

  TH1 * h_rho1ThVsMass;
  TH1 * h_rho1VsMass;
  TH1 * h_rho1RatioVsMass;

  TH2 * h_rho1thrho1th;
  TH2 * h_rho1rho1;
  TH2 * h_rho2Corr;
  TH2 * h_rho2Uncorr;
  TH2 * h_rho2;
  TH2 * h_C2;
  TH2 * h_R2;
 
  vector<TH1*> h_rho1ThVsP;

  ClassDef(HadronGasHistograms,0)

};

#endif /* CAP_HadronGasVsTempHistograms  */



