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
#ifndef CAP__ParticlePerformanceHistos
#define CAP__ParticlePerformanceHistos
#include "HistogramGroup.hpp"
#include "Particle.hpp"
#include "ParticleDigit.hpp"
#include "Configuration.hpp"

namespace CAP
{


class ParticlePerformanceHistos : public HistogramGroup
{
public:
  
  ParticlePerformanceHistos(Task * _parent,
                            const String & _name,
                            const Configuration & _configuration);
  virtual ~ParticlePerformanceHistos() {}
  virtual void createHistograms();
  virtual void importHistograms(TFile & inputFile);
  virtual void fill(Particle & particle, double weight);
  
  
protected:

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
  ////////////////////////////////////////////////////////////////////////////
  bool fillEta;
  bool fillY;
 
  
  unsigned int nBins_pt;
  float        min_pt;
  float        max_pt;
  
  unsigned int nBins_dpt;
  float        min_dpt;
  float        max_dpt;
  
  unsigned int nBins_phi;
  float        min_phi;
  float        max_phi;
  
  unsigned int nBins_dphi;
  float        min_dphi;
  float        max_dphi;
  
  unsigned int nBins_eta;
  float        min_eta;
  float        max_eta;
  
  unsigned int nBins_deta;
  float        min_deta;
  float        max_deta;
  
  unsigned int nBins_y;
  float        min_y;
  float        max_y;
  
  unsigned int nBins_dy;
  float        min_dy;
  float        max_dy;
   
  //! Primary histograms

  TH1 * h_n1_dPt;
  TH1 * h_n1_dEta;
  TH1 * h_n1_dY;
  TH1 * h_n1_dPhi;

  TH2 * h_n1_dPtVsPt;
  TH2 * h_n1_dEtaVsEta;
  TH2 * h_n1_dYVsY;
  TH2 * h_n1_dPhiVsPt;

  ClassDef(ParticlePerformanceHistos,0)

};

} // namespace CAP

#endif /* CAP__ParticlePerformanceHistos  */



