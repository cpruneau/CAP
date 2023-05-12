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
#ifndef CAP__ParticleHistos
#define CAP__ParticleHistos
#include "HistogramGroup.hpp"
#include "Particle.hpp"
#include "ParticleDigit.hpp"
#include "Configuration.hpp"
#include "TLorentzVector.h"

class ParticleHistos : public HistogramGroup
{
public:

  ParticleHistos(Task * _parent,
                 const TString & _name,
                 Configuration & _configuration);
  virtual ~ParticleHistos();
  virtual void createHistograms();
  virtual void importHistograms(TFile & inputFile);
  virtual void loadCalibration(TFile & inputFile);
  virtual void fill(vector<ParticleDigit*> & particles, double weight);
  virtual void fill(Particle & particle, double weight);
  virtual void fillMultiplicity(double nAccepted, double totalEnergy, double weight);
  
  inline int getPtBinFor(float v) const
  {
  int index = 1+double(nBins_pt)*(v-min_pt)/scale_pt;
  if (index>int(nBins_pt)) index = -1;
  //cout << "getPtBinFor() v:" << v << " min:" << min_pt <<  " max:" << max_pt <<  " range:" << scale_pt << " index:" << index << endl;
  return index;
  }

  inline int getPhiBinFor(float v) const
  {
  if (v<0.0) v += TMath::TwoPi();
  int index = 1+ double(nBins_phi)*(v-min_phi)/scale_phi;
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
  int index = 1+double(nBins_y)*(v-min_y)/range_y;
  if (index>int(nBins_y)) index = -1;
  return index;
  }

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
  ////////////////////////////////////////////////////////////////////////////
  bool fillEta;
  bool fillY;
  bool fillP2;
  bool useEffCorrection;
  int  efficiencyOpt;

  unsigned int nBins_n1;
  float        min_n1;
  float        max_n1;
  unsigned int nBins_pt;
  float        min_pt;
  float        max_pt;
  float        scale_pt;
  unsigned int nBins_phi;
  float        min_phi;
  float        max_phi;
  float        scale_phi;
  unsigned int nBins_eta;
  float        min_eta;
  float        max_eta;
  float        range_eta;
  unsigned int nBins_y;
  float        min_y;
  float        max_y;
  float        range_y;
  unsigned int nBins_phiEta;
  unsigned int nBins_phiEtaPt;
  unsigned int nBins_phiY;
  unsigned int nBins_phiYPt;
  
  //! Primary histograms

  TH1 * h_n1;
  TH1 * h_n1_eTotal;
  TH1 * h_n1_pt;
  TH1 * h_n1_ptXS;  // 1/pt dN/dptdy

  TH2 * h_n1_phiEta;
  TH2 * h_spt_phiEta;

  TH2 * h_n1_phiY;
  TH2 * h_spt_phiY;

  TH1 * h_pdgId;

  TH1 * h_eff_pt;
  TH2 * h_eff_ptEta;
  TH2 * h_eff_ptY;
  TH3 * h_eff_ptPhiEta;
  TH3 * h_eff_ptPhiY;

    ClassDef(ParticleHistos,0)

};

#endif /* CAP__ParticleHistos  */



