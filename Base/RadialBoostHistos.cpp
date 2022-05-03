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
#include "RadialBoostHistos.hpp"
ClassImp(RadialBoostHistos);

RadialBoostHistos::RadialBoostHistos(const TString & _name,
                                     const Configuration& _configuration,
                                     LogLevel  _debugLevel)
:
Histograms(_name,_configuration,_debugLevel)
{
  appendClassName("RadialBoostHistos");
  setInstanceName(name);
}

RadialBoostHistos::~RadialBoostHistos()
{
  //deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void RadialBoostHistos::createHistograms()
{
  Configuration & configuration = getConfiguration();
  int nBins_phi   = configuration.getValueInt("nBins_phi");
  double min_phi  = configuration.getValueDouble("min_phi");
  double max_phi  = configuration.getValueDouble("max_phi");
  int nBins_r     = configuration.getValueInt("nBins_r");
  double min_r    = configuration.getValueDouble("min_r");
  double max_r    = configuration.getValueDouble("max_r");
  int nBins_beta  = configuration.getValueInt("nBins_beta");
  double min_beta = configuration.getValueDouble("min_beta");
  double max_beta = configuration.getValueDouble("max_beta");
  
  TString bn = getHistoBaseName();
  h_phi  = createHistogram(bn+TString("ns_phi"),   nBins_phi,  min_phi,  max_phi,  "#phi",      "N_{s}");
  h_r    = createHistogram(bn+TString("ns_r"),     nBins_r,    min_r,    max_r,    "r",         "N_{s}");
  h_beta = createHistogram(bn+TString("ns_beta"),  nBins_beta, min_beta, max_beta, "#beta_{s}", "N_{s}");
  h_xy   = createHistogram(bn+TString("ns_xy"),    40, -10.0, 10.0,  40, -10.0, 10.0,       "x",    "y", "N_{s}");
}

//________________________________________________________________________
void RadialBoostHistos::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "-Fatal- Attempting to load RadialBoostHistos from an invalid file pointer" << endl;
    return;
    }
  //RadialBoostConfiguration & ac = *(RadialBoostConfiguration*) getConfiguration();
  TString bn = getHistoBaseName();
  h_phi  = loadH1(inputFile,bn+TString("ns_phi")  );
  h_r    = loadH1(inputFile,bn+TString("ns_r")    );
  h_beta = loadH1(inputFile,bn+TString("ns_beta") );
  h_xy   = loadH2(inputFile,bn+TString("ns_xy") );
  return;
}

void RadialBoostHistos::fill(double x, double y, double r, double phi, double beta, double weight)
{
  h_phi  ->Fill(phi,  weight);
  h_r    ->Fill(r,    weight);
  h_beta ->Fill(beta, weight);
  h_xy   ->Fill(x, y, weight);
}



