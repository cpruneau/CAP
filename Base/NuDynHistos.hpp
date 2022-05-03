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
#ifndef CAP__NuDynHistos
#define CAP__NuDynHistos
#include "Histograms.hpp"
#include "Configuration.hpp"

//!
//! Calculate lowest moments up to order 4
//! The calculation is carried out for N distinct types (species) of particles (N distinct filters)
//!
//! Let the species be: a, b, c, etc
//! First order moments:  f_1(a), f_1(b), f_1(c), etc
//! Second order moments: f_2(a,a), f_2(a,b), f_2(a,c), ..., f_2(b,b), f_2(b,c), etc
//! Third order moments: f_3(a,a,a), f_3(a,a,b), f_3(a,a,c), f(a,a,d), f(a,b,b), etc
//! Fourth order moments: f_4(a,a,a,a), f_(a,a,a,b), f_4(a,a,b,b), etc
//!
//!// ================================================================================
// Naming convention
// ================================================================================
// fk_i     : factorial moments of order "k" for particle "i" (i.e., accepted by filter 1)
// Fk_i..j  : factorial cumulants of order "k" for particles "i, ..., j" (i.e., accepted by filter i..j)
// rk_i     : ratio of factorial moments of order "k" for particle "i" (i.e., accepted by filter 1)
//            to product of f1_i
//

// n1 number of particles satisfying filter 1
// n2 number of particles satisfying filter 2
// h_f1_1 = <n1>
// h_f1_2 = <n2>
// h_f2_11 = <n1(n1-1)>
// h_f2_22 = <n2(n2-1)>
// h_f2_12 = <n2(n2-1)>
// h_r2_11 = <n1(n1-1)>/<n1><n1>
// h_r2_22 = <n2(n2-1)>/<n2><n2>
// h_r2_12 = <n1(n2-1)>/<n1><n2>
// h_nudyn_12 = h_r2_11 + h_r2_22 -2*h_r2_12
// etc
class NuDynHistos : public Histograms
{
public:

  NuDynHistos(const TString & name,
              const Configuration & configuration,
              LogLevel  debugLevel);
  virtual ~NuDynHistos();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void fill(double mult, vector<double> & nAccepted,  double weight);

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  ////////////////////////////////////////////////////////////////////////////
  // n1 number of particles satisfying filter 1
  // n2 number of particles satisfying filter 2
  // n3 number of particles satisfying filter 3
  // n4 number of particles satisfying filter 4
  // h_f1_1 = <n1>
  // h_f1_2 = <n2>
  // h_f2_11 = <n1(n1-1)>
  // h_f2_22 = <n2(n2-1)>
  // h_f2_12 = <n2(n2-1)>
  // h_f3_111 = <n1(n1-1)(n1-2)>
  // h_f3_112 = <n1(n1-1)n2>
  // h_f3_113 = <n1(n1-1)n3>
  // h_f3_122 = <n1n2(n2-1)>
  // h_f3_123 = <n1n2n3>
  // etc

  // h_r2_11 = <n1(n1-1)>/<n1><n1>
  // h_r2_22 = <n2(n2-1)>/<n2><n2>
  // h_r2_12 = <n1(n2-1)>/<n1><n2>
  // h_nudyn_12 = h_r2_11 + h_r2_22 -2*h_r2_12
  // etc

  // Min bias all included
  TH1 * h_eventStreams;
  unsigned int nFilters;
  unsigned int multiplicityType;
  bool pairOnly;

  vector<TProfile *> h_f1;
  vector<TProfile *> h_f2;
  vector<TProfile *> h_f3;
  vector<TProfile *> h_f4;
  // vs Mult measured in fiducial
  TH1 * h_eventStreams_vsMult;
  vector<TProfile *> h_f1_vsMult;
  vector<TProfile *> h_f2_vsMult;
  vector<TProfile *> h_f3_vsMult;
  vector<TProfile *> h_f4_vsMult;
 
  ClassDef(NuDynHistos,0)
};

#endif /* CAP__NuDynHistos  */



