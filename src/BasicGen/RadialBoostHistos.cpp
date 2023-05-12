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
using CAP::RadialBoostHistos;

ClassImp(RadialBoostHistos);

RadialBoostHistos::RadialBoostHistos(Task * _partial,
                                     const String & _name,
                                     const Configuration & _configuration)
:
HistogramGroup(_partial,_name,_configuration)
{
  appendClassName("RadialBoostHistos");
}

// for now use the same boundaries for eta and y histogram
void RadialBoostHistos::createHistograms()
{
  int nBins_phi   = configuration.getValueInt(   "nBins_phi");
  double min_phi  = configuration.getValueDouble("Min_phi");
  double max_phi  = configuration.getValueDouble("Max_phi");
  int nBins_r     = configuration.getValueInt(   "nBins_r");
  double min_r    = configuration.getValueDouble("Min_r");
  double max_r    = configuration.getValueDouble("Max_r");
  int nBins_beta  = configuration.getValueInt(   "nBins_beta");
  double min_beta = configuration.getValueDouble("Min_beta");
  double max_beta = configuration.getValueDouble("Max_beta");
  
  String bn = getParentName();
  h_phi  = createHistogram(createName(bn,"ns_phi"),   nBins_phi,  min_phi,  max_phi,  "#phi",      "N_{s}");
  h_r    = createHistogram(createName(bn,"ns_r"),     nBins_r,    min_r,    max_r,    "r",         "N_{s}");
  h_beta = createHistogram(createName(bn,"ns_beta"),  nBins_beta, min_beta, max_beta, "#beta_{s}", "N_{s}");
  h_xy   = createHistogram(createName(bn,"ns_xy"),    40, -10.0, 10.0,  40, -10.0, 10.0,       "x",    "y", "N_{s}");
}

//________________________________________________________________________
void RadialBoostHistos::importHistograms(TFile & inputFile)
{
  String bn = getParentName( );
  h_phi  = loadH1(inputFile,createName(bn,"ns_phi")  );
  h_r    = loadH1(inputFile,createName(bn,"ns_r")    );
  h_beta = loadH1(inputFile,createName(bn,"ns_beta") );
  h_xy   = loadH2(inputFile,createName(bn,"ns_xy") );
  return;
}

void RadialBoostHistos::fill(double x, double y, double r, double phi, double beta, double weight)
{
  h_phi  ->Fill(phi,  weight);
  h_r    ->Fill(r,    weight);
  h_beta ->Fill(beta, weight);
  h_xy   ->Fill(x, y, weight);
}



