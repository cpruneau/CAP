// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_HadronGasVsTempHistograms
#define WAC_HadronGasVsTempHistograms
#include "Histograms.hpp"
#include "HadronGas.hpp"

class HadronGasVsTempHistograms : public Histograms
{
public:

  HadronGasVsTempHistograms(const TString &       _name,
                            const Configuration & _config,
                            HadronGas *           _hadronGas,
                            LogLevel              _debugLevel);
  virtual ~HadronGasVsTempHistograms() {}
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  void fill(HadronGas & hadronGas);

  // System Wide
  TH1 * numberDensityVsT;
  TH1 * energyDensityVsT;
  TH1 * entropyDensityVsT;
  TH1 * pressureVsT;

  // By Species
  vector<TH1*> nDensityVsT;
  vector<TH1*> eDensityVsT;
  vector<TH1*> sDensityVsT;

  ClassDef(HadronGasVsTempHistograms,0)

};

#endif /* WAC_HadronGasVsTempHistograms  */



