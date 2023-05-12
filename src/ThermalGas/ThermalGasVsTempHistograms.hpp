// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef CAP_ThermalGasVsTempHistograms
#define CAP_ThermalGasVsTempHistograms
#include "HistogramGroup.hpp"
#include "ThermalGas.hpp"

namespace CAP
{


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
//!The histogram parameters are user supplied via the configuration of the ThermalGasModel which calls
//!this histogramming class to fill histograms. There should be one call per temperature value. Note that parameters
//!plotThermalSpeciesVsT and plotStableSpeciesVsT are toggle switches allowing to turn on/off the plotting of the
//!histograms for thermal particle and stable particles respsectively.
//!
class ThermalGasVsTempHistograms : public HistogramGroup
{
public:

  ThermalGasVsTempHistograms(Task * _parent,
                            const String & _name,
                            const Configuration & _config);
  virtual ~ThermalGasVsTempHistograms() {}
  void createHistograms();
  void importHistograms(TFile & inputFile);
  void fill(ThermalGas & ThermalGas);

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

  ClassDef(ThermalGasVsTempHistograms,0)

};

} // namespace CAP

#endif /* CAP_ThermalGasVsTempHistograms  */



