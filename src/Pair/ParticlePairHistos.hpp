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
#ifndef CAP__ParticlePairHistos
#define CAP__ParticlePairHistos

#include "Histograms.hpp"
#include "Particle.hpp"
#include "ParticleDigit.hpp"

class ParticlePairHistos : public Histograms
{
public:

  ParticlePairHistos(const TString & name,
                     const Configuration & configuration,
                     LogLevel  debugLevel);
  virtual ~ParticlePairHistos();
  virtual void createHistograms();
  virtual void fill(vector<ParticleDigit*> & particle1, vector<ParticleDigit*> & particle2, bool same, double weight);

  virtual void loadHistograms(TFile * inputFile);

  inline int getPtBinFor(float v) const
  {
  int index = 1+double(nBins_pt)*(v-min_pt)/range_pt;
  if (index>int(nBins_pt)) index = -1;
  return index;
  }

  inline int getPhiBinFor(float v) const
  {
  if (v<0.0) v += TMath::TwoPi();
  int index = 1+double(nBins_phi)*(v-min_phi)/range_phi;
  if (index>int(nBins_phi)) index = -1;
  return index;
  }

  inline int getEtaBinFor(float v) const
  {
  int index = 1+ double(nBins_eta)*(v-min_eta)/range_eta;
  if (index>int(nBins_eta)) index = -1;
  return index;
  }

  inline int getYBinFor(float v) const
  {
  int index = 1+ double(nBins_y)*(v-min_y)/range_y;
  if (index>int(nBins_y)) index = -1;
  return index;
  }

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  ////////////////////////////////////////////////////////////////////////////

  int    nBins_n1;
  double min_n1;
  double max_n1;

  int    nBins_n2;
  double min_n2;
  double max_n2;

  int    nBins_pt;
  double min_pt;
  double max_pt;
  double range_pt;

  int    nBins_phi;
  double min_phi;
  double max_phi;
  double range_phi;

  int    nBins_eta;
  double min_eta;
  double max_eta;
  double range_eta;

  int    nBins_y;
  double min_y;
  double max_y;
  double range_y;

  int    nBins_Dphi;
  double min_Dphi;
  double max_Dphi;
  double width_Dphi;

  int    nBins_Deta;
  double min_Deta;
  double max_Deta;
  int    nBins_Dy;
  double min_Dy;
  double max_Dy;

  int    nBins_Dphi_shft;
  double min_Dphi_shft;
  double max_Dphi_shft;

  bool fillEta;
  bool fillY;
  bool fillP2;
 
  TH1 * h_n2;

  TH2 * h_n2_ptpt;

  TH2 * h_n2_etaEta;
  TH2 * h_DptDpt_etaEta;

  TH2 * h_n2_phiPhi;
  TH2 * h_DptDpt_phiPhi;
  
  TH2 * h_n2_yY;
  TH2 * h_DptDpt_yY;

  TH2* h_n2_DetaDphi;
  TH2* h_DptDpt_DetaDphi;

  TH2* h_n2_DyDphi;
  TH2* h_DptDpt_DyDphi;

  ClassDef(ParticlePairHistos,0)
};


#endif /* CAP__ParticlePairHistos  */
