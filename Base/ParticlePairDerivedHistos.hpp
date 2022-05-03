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
#include "ParticleHistos.hpp"
#include "ParticleDerivedHistos.hpp"
#include "ParticlePairHistos.hpp"
#include "Histograms.hpp"

//!
//! Two-Particle Correlation Function Histograms
//!
class ParticlePairDerivedHistos : public  Histograms
{
public:

  ParticlePairDerivedHistos(const TString & name,
                            const Configuration & configuration,
                            LogLevel  debugLevel);
  virtual ~ParticlePairDerivedHistos();
  virtual void loadHistograms(TFile * inputFile);
  virtual void createHistograms();
  virtual void calculatePairDerivedHistograms(ParticleHistos         & part1BaseHistos,
                                              ParticleHistos         & part2BaseHistos,
                                              ParticleDerivedHistos  & part1DerivedHistos,
                                              ParticleDerivedHistos  & part2DerivedHistos,
                                              ParticlePairHistos     & pairHistos,
                                              double bincorrection);

  bool         fillEta;
  bool         fillY;
  bool         fillP2;
  unsigned int nBins_n1;
  float        min_n1;
  float        max_n1;
  unsigned int nBins_pt;
  float        min_pt;
  float        max_pt;
  unsigned int nBins_phi;
  float        min_phi;
  float        max_phi;
  unsigned int nBins_eta;
  float        min_eta;
  float        max_eta;
  unsigned int nBins_y;
  float        min_y;
  float        max_y;
  unsigned int nBins_Dphi;
  float        min_Dphi;
  float        max_Dphi;
  unsigned int nBins_Deta;
  float        min_Deta;
  float        max_Deta;
  unsigned int nBins_Dy;
  float        min_Dy;
  float        max_Dy;

  int          nBins_Dphi_shft;
  float        min_Dphi_shft;
  float        max_Dphi_shft;

  TH2* h_n1n1_ptpt;
  TH2* h_R2_ptpt;

  TH2* h_n1n1_phiPhi;
  TH2* h_R2_phiPhi;

  TH2* h_pt1pt1_phiPhi;
  TH2* h_DptDpt_phiPhi;
  TH2* h_P2_phiPhi;
  TH2* h_G2_phiPhi;

  TH2* h_n1n1_etaEta;
  TH2* h_R2_etaEta;

  TH2* h_pt1pt1_etaEta;
  TH2* h_DptDpt_etaEta;
  TH2* h_G2_etaEta;
  TH2* h_P2_etaEta;

  TH2* h_n1n1_DetaDphi;
  TH2* h_rho2_DetaDphi;
  TH2* h_B2AB_DetaDphi;
  TH2* h_B2BA_DetaDphi;
  TH2* h_R2_DetaDphi;
  TH2* h_P2_DetaDphi;
  TH2* h_G2_DetaDphi;

  TH2* h_pt1pt1_DetaDphi;
  TH2* h_DptDpt_DetaDphi;


  TH2* h_rho2_DetaDphi_shft;
  TH2* h_B2AB_DetaDphi_shft;
  TH2* h_B2BA_DetaDphi_shft;
  TH2* h_R2_DetaDphi_shft;
  TH2* h_DptDpt_DetaDphi_shft;
  TH2* h_P2_DetaDphi_shft;
  TH2* h_G2_DetaDphi_shft;

  TH2* h_n1n1_yY;
  TH2* h_R2_yY;

  TH2* h_pt1pt1_yY;
  TH2* h_DptDpt_yY;
  TH2* h_G2_yY;
  TH2* h_P2_yY;

  TH2* h_n1n1_DyDphi;
  TH2* h_rho2_DyDphi;
  TH2* h_B2AB_DyDphi;
  TH2* h_B2BA_DyDphi;
  TH2* h_R2_DyDphi;
  TH2* h_P2_DyDphi;
  TH2* h_G2_DyDphi;

  TH2* h_rho2_DyDphi_shft;
  TH2* h_B2AB_DyDphi_shft;
  TH2* h_B2BA_DyDphi_shft;
  TH2* h_R2_DyDphi_shft;
  TH2* h_DptDpt_DyDphi_shft;
  TH2* h_P2_DyDphi_shft;
  TH2* h_G2_DyDphi_shft;

  TH2* h_pt1pt1_DyDphi;
  TH2* h_DptDpt_DyDphi;

  ClassDef(ParticlePairDerivedHistos,1)

};

#endif /* CAP__ParticlePairDerivedHistos */
