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
#ifndef CAP__ParticlePairDerivedHistos
#define CAP__ParticlePairDerivedHistos
#include "ParticleSingleHistos.hpp"
#include "ParticleSingleDerivedHistos.hpp"
#include "ParticlePairHistos.hpp"
#include "HistogramGroup.hpp"

namespace CAP
{


//!
//! Two-Particle Correlation Function HistogramGroup
//!
class ParticlePairDerivedHistos : public  HistogramGroup
{
public:

  ParticlePairDerivedHistos(Task * _parent,
                            const String & _name,
                            Configuration & _configuration);
  virtual ~ParticlePairDerivedHistos() {}
  virtual void loadHistograms(TFile * inputFile);
  virtual void createHistograms();
  virtual void calculatePairDerivedHistograms(ParticleSingleHistos         & part1BaseHistos,
                                              ParticleSingleHistos         & part2BaseHistos,
                                              ParticleSingleDerivedHistos  & part1DerivedHistos,
                                              ParticleSingleDerivedHistos  & part2DerivedHistos,
                                              ParticlePairHistos     & pairHistos,
                                              double bincorrection);

  void calculateOmegaFactor(TH2 * h_DyDphi, vector<double> & omegaFactor);
  void calculateAverageYbar(TH2 * h_DyDphi, TH2 * h_DyDphi_Avg, vector<double> & omegaFactor);

  bool         fillEta;
  bool         fillY;
  bool         fillP2;
  unsigned int nBins_n2;
  double       min_n2;
  double       max_n2;
  unsigned int nBins_pt;
  double       min_pt;
  double       max_pt;
  unsigned int nBins_phi;
  double       min_phi;
  double       max_phi;
  unsigned int nBins_eta;
  double       min_eta;
  double       max_eta;
  unsigned int nBins_y;
  double       min_y;
  double       max_y;
  unsigned int nBins_Dphi;
  double       min_Dphi;
  double       max_Dphi;
  unsigned int nBins_Deta;
  double       min_Deta;
  double       max_Deta;
  unsigned int nBins_Dy;
  double       min_Dy;
  double       max_Dy;

  int          nBins_Dphi_shft;
  double       min_Dphi_shft;
  double       max_Dphi_shft;

  TH2* h_n1n1_ptpt;
  TH2* h_A2_ptpt;
  TH2* h_B2_ptpt;
  TH2* h_C2_ptpt;
  TH2* h_R2_ptpt;

  TH2* h_n1n1_phiPhi;
  TH2* h_A2_phiPhi;
  TH2* h_B2_phiPhi;
  TH2* h_C2_phiPhi;
  TH2* h_R2_phiPhi;

  TH2* h_pt1pt1_phiPhi;
  TH2* h_DptDpt_phiPhi;
  TH2* h_P2_phiPhi;
  TH2* h_G2_phiPhi;

  TH2* h_n1n1_etaEta;
  TH2* h_A2_etaEta;
  TH2* h_B2_etaEta;
  TH2* h_C2_etaEta;
  TH2* h_R2_etaEta;

  TH2* h_pt1pt1_etaEta;
  TH2* h_DptDpt_etaEta;
  TH2* h_G2_etaEta;
  TH2* h_P2_etaEta;

  TH2* h_n1n1_DetaDphi;
  TH2* h_rho2_DetaDphi;
  TH2* h_A2_DetaDphi;
  TH2* h_B2_DetaDphi;
  TH2* h_C2_DetaDphi;
  TH2* h_R2_DetaDphi;
  TH2* h_P2_DetaDphi;
  TH2* h_G2_DetaDphi;

  TH2* h_pt1pt1_DetaDphi;
  TH2* h_DptDpt_DetaDphi;


  TH2* h_rho2_DetaDphi_shft;
  TH2* h_A2_DetaDphi_shft;
  TH2* h_B2_DetaDphi_shft;
  TH2* h_C2_DetaDphi_shft;
  TH2* h_R2_DetaDphi_shft;
  TH2* h_DptDpt_DetaDphi_shft;
  TH2* h_P2_DetaDphi_shft;
  TH2* h_G2_DetaDphi_shft;

  TH2* h_n1n1_yY;
  TH2* h_A2_yY;
  TH2* h_B2_yY;
  TH2* h_C2_yY;
  TH2* h_R2_yY;

  TH2* h_pt1pt1_yY;
  TH2* h_DptDpt_yY;
  TH2* h_G2_yY;
  TH2* h_P2_yY;

  TH2* h_n1n1_DyDphi;
  TH2* h_rho2_DyDphi;
  TH2* h_A2_DyDphi;
  TH2* h_B2_DyDphi;
  TH2* h_C2_DyDphi;
  TH2* h_R2_DyDphi;
  TH2* h_P2_DyDphi;
  TH2* h_G2_DyDphi;

  TH2* h_rho2_DyDphi_shft;
  TH2* h_A2_DyDphi_shft;
  TH2* h_B2_DyDphi_shft;
  TH2* h_C2_DyDphi_shft;
  TH2* h_R2_DyDphi_shft;
  TH2* h_DptDpt_DyDphi_shft;
  TH2* h_P2_DyDphi_shft;
  TH2* h_G2_DyDphi_shft;

  TH2* h_A2A_DyDphi_shft;
  TH2* h_B2A_DyDphi_shft;
  TH2* h_C2A_DyDphi_shft;
  TH2* h_R2A_DyDphi_shft;

  TH2* h_pt1pt1_DyDphi;
  TH2* h_DptDpt_DyDphi;

  ClassDef(ParticlePairDerivedHistos,1)

};

} // namespace CAP

#endif /* CAP__ParticlePairDerivedHistos */
