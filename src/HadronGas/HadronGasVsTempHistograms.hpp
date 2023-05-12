// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef CAP_HadronGasVsTempHistograms
#define CAP_HadronGasVsTempHistograms
#include "Histograms.hpp"
#include "HadronGas.hpp"

//!
//!Histograming class designed to make plots of the global hadron gas (HG) and particle species partial quantities as
//!a function of the temperature and chemical potential of the gas. Global hadron gas quantities of interest i.e., being plotted are
//!- number density
//!-energy density
//!- entropy density
//!- pressure
//!Quantities of interest for each of the (thermal) particle species included in the HG calculation include
//!- number density
//!- entropy density
//!- partial pressure
//!Quantities of interest for each of the  stable particle species included in the HG calculation include
//!- number density
//!
//!The histogram parameters are user supplied via the configuration of the HadronGasGeneratorTask which calls
//!this histogramming class to fill histograms. There should be one call per temperature value. Note that parameters
//!plotThermalSpeciesVsT and plotStableSpeciesVsT are toggle switches allowing to turn on/off the plotting of the
//!histograms for thermal particle and stable particles respsectively.
//!
class HadronGasVsTempHistograms : public Histograms
{
public:

  HadronGasVsTempHistograms(Task * _parent,
                            const TString & _name,
                            Configuration & _config);
  virtual ~HadronGasVsTempHistograms() {}
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  void fill(HadronGas & hadronGas);

  // System Wide

  //!
  //! Number density of the hadron gas vs. temperature
  //!
  TH1 * numberDensityVsT;

  //!
  //! Energy density of the hadron gas vs. temperature
  //!
  TH1 * energyDensityVsT;

  //!
  //! Entropy density of the hadron gas vs. temperature
  //!
  TH1 * entropyDensityVsT;

  //!
  //! Pressure of the hadron gas vs. temperature
  //!
  TH1 * pressureVsT;

  // By (Thermal) Species

  //!
  //! Number densities of each of  the species composing the hadron gas vs. temperature
  //!
  vector<TH1*> partialNumberDensityVsT;

  //!
  //! Energy densities of each of  the species composing the hadron gas vs. temperature
  //!
  vector<TH1*> partialEnergyDensityVsT;

  //!
  //! Entropy densities of   the species composing the hadron gas vs. temperature
  //!
  vector<TH1*> partialEntropyDensityVsT;

  //!
  //! Partial pressure of   the species composing the hadron gas vs. temperature
  //!
  vector<TH1*> partialPressureVsT;

  // By (Stable) Species

  //!
  //! Number densities of each of  the species after decay of all the particles  composing the hadron gas vs. temperature
  //!
  vector<TH1*> nStableDensityVsT;

  ClassDef(HadronGasVsTempHistograms,0)

};

#endif /* CAP_HadronGasVsTempHistograms  */



