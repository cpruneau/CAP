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
#ifndef CAP__NuDynDerivedHistos
#define CAP__NuDynDerivedHistos
#include "Histograms.hpp"
#include "NuDynHistos.hpp"


class NuDynDerivedHistos : public Histograms
{
public:

  NuDynDerivedHistos(const TString & name,
                     const Configuration & configuration,
                     LogLevel  debugLevel);
  virtual ~NuDynDerivedHistos() {} 
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  virtual void calculateDerivedHistograms(NuDynHistos* baseHistos);

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  ////////////////////////////////////////////////////////////////////////////
  // n1 number of particles satisfying filter 1
  // n2 number of particles satisfying filter 2
  // h_Fj  = cumulants of order "j"
  // h_Rj  = normalized cumulants of order "j"

  int  nFilters;
  int  multiplicityType;
  bool pairOnly;

  vector<TH1 *> h_F2;
  vector<TH1 *> h_F3;
  vector<TH1 *> h_F4;
  vector<TH1 *> h_R2;
  vector<TH1 *> h_R3;
  vector<TH1 *> h_R4;
  vector<TH1 *> h_nudyn;

  vector<TH1 *> h_F2_vsMult;
  vector<TH1 *> h_F3_vsMult;
  vector<TH1 *> h_F4_vsMult;
  vector<TH1 *> h_R2_vsMult;
  vector<TH1 *> h_R3_vsMult;
  vector<TH1 *> h_R4_vsMult;
  vector<TH1 *> h_nudyn_vsMult;

  ClassDef(NuDynDerivedHistos,0)
};

#endif /* CAP__NuDynDerivedHistos  */



