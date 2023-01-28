// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_HadronGasParticlesHistograms
#define WAC_HadronGasParticlesHistograms
#include "HistogramGroup.hpp"
#include "HadronGas.hpp"

namespace CAP
{


// ===================================================
// Particle abundance at a specific temperature or beam
// energy or etc..
class HadronGasHistograms : public HistogramGroup
{
public:

  HadronGasHistograms(Task * _parent,
                      const String & _name,
                      Configuration & _config,
                      HadronGas * _hadronGas);
  virtual ~HadronGasHistograms() {}
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  void fill(HadronGas & hadronGas);
  void fillBf(int trig, int sameSignAssoc, int oppSignAssoc, int bfIndex);
  void fillRho1VsP(HadronGas & hadronGas, double volume);
  void calculateDerivedHistograms();

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
  ////////////////////////////////////////////////////////////////////////////
  HadronGas * hadronGas;
  TProfile * h_rho1AllVsMass;
  TProfile * h_rho1ThVsMass;
  TProfile * h_rho1VsMass;
  TH1 * h_rho1All;
  TH1 * h_rho1Th;
  TH1 * h_rho1;
  TH1 * h_rho1ToRho1Th;

  TH2 * h_rho1thrho1th;
  TH2 * h_rho1rho1;
  TH2 * h_rho2Corr;
  TH2 * h_rho2Uncorr;
  TH2 * h_rho2;
  TH2 * h_C2;
  TH2 * h_R2;
  TH1 * h_BF;

  TH1 ** h_rho1ThVsP;

  ClassDef(HadronGasHistograms,0)

};

} // namespace CAP

#endif /* WAC_HadronGasVsTempHistograms  */



