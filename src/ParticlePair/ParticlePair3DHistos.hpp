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
#ifndef CAP__ParticlePair3DHistos
#define CAP__ParticlePair3DHistos

#include "HistogramGroup.hpp"
#include "Particle.hpp"

class ParticlePair3DHistos : public HistogramGroup
{
public:

  ParticlePair3DHistos(Task * _parent,
                       const TString & _name,
                       const Configuration & _configuration);
  virtual ~ParticlePair3DHistos() {}
  virtual void createHistograms();
  virtual void fill(vector<Particle*> & particle1, vector<Particle*> & particle2, bool same, double weight);
  virtual void importHistograms(TFile & inputFile);

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
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
  double scale_pt;

  int    nBins_phi;
  double min_phi;
  double max_phi;
  double range_phi;
  double scale_phi;

  int    nBins_eta;
  double min_eta;
  double max_eta;
  double range_eta;
  double scale_eta;

  int    nBins_y;
  double min_y;
  double max_y;
  double range_y;
  double scale_y;

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
  bool fill3D;

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

  TH3 * h_n2_DeltaP;

  ClassDef(ParticlePair3DHistos,0)
};


#endif /* CAP__ParticlePair3DHistos  */
