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
#include "HistogramGroup.hpp"
#include "HadronGas.hpp"

namespace CAP
{


class HadronGasVsTempHistograms : public HistogramGroup
{
public:

  HadronGasVsTempHistograms(Task * _parent,
                            const String & _name,
                            Configuration & _config,
                            HadronGas * _hadronGas);
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


} // namespace CAP

#endif /* WAC_HadronGasVsTempHistograms  */



