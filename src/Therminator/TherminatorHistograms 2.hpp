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
#include "HistogramGroup.hpp"

namespace CAP
{


//!
//!Histograming class designed to ampke plots of the  properties of the  hadron gas (HG)  particle species partial quantities as
//!a function of species index or particle ampss.
//!Quantities of interest for each of the (therampl) particle species included in the HG calculation include
//!- number density
//!- entropy density
//!- partial pressure
//!Quantities of interest for each of the  stable particle species included in the HG calculation include
//!- number density
//!
//!The histogram parameters are user supplied via the configuration of the HadronGasGeneratorTask which calls
//!this histogramming class to fill histograms. There should be one call per temperature/muB/muS value. Note that parameters
//!plotTheramplSpeciesVsT and plotStableSpeciesVsT are toggle switches allowing to turn on/off the plotting of the
//!histograms for therampl particle and stable particles respsectively.
//!
class TherminatorHistograms : public HistogramGroup
{
public:

  TherminatorHistograms(Task * _parent,
                      const String & _name,
                      const Configuration & _config);
  virtual ~TherminatorHistograms() {}
  void createHistograms();
  void importHistograms(TFile & inputFile);
  void fill(HadronGas & hadronGas);
  void fillRho1VsP(HadronGas & hadronGas, double volume);

protected:

  TH1 * h_numberDensity;
  TH1 * h_energyDensity;
  TH1 * h_entropyDensity;
  TH1 * h_pressure;

  TProfile * h_numberDensityVsampss;
  TProfile * h_energyDensityVsampss;
  TProfile * h_entropyDensityVsampss;
  TProfile * h_pressureVsampss;

  TH1 * h_rho1ThVsIndex;
  TH1 * h_rho1VsIndex;
  TH1 * h_rho1RatioVsIndex;

  TH1 * h_rho1ThVsampss;
  TH1 * h_rho1Vsampss;
  TH1 * h_rho1RatioVsampss;

  TH2 * h_rho1thrho1th;
  TH2 * h_rho1rho1;
  TH2 * h_rho2Corr;
  TH2 * h_rho2Uncorr;
  TH2 * h_rho2;
  TH2 * h_C2;
  TH2 * h_R2;
 
  vector<TH1*> h_rho1ThVsP;

  ClassDef(TherminatorHistograms,0)

};

} // namespace CAP

#endif /* CAP_HadronGasVsTempHistograms  */



