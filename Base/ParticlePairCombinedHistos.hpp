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
#ifndef CAP__ParticlePairCombinedHistos
#define CAP__ParticlePairCombinedHistos
#include "ParticlePairDerivedHistos.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Two-Particle Correlation Combinations (CI, CD)
//////////////////////////////////////////////////////////////////////////////////////////////////////
class ParticlePairCombinedHistos : public Histograms
{
public:
  
  //!
  //! CTOR
  //!
  ParticlePairCombinedHistos(const TString & name,
                             const Configuration & configuration,
                             LogLevel  debugLevel);
  //!
  //! DTOR
  //!
  virtual ~ParticlePairCombinedHistos();
  
  //!
  //! Create (instatiate) histograms of combined pairs
  //!
  virtual void createHistograms();
  
  //!
  //! Load  histograms of combined pairs
  //!
  virtual void loadHistograms(TFile * inputFile);
  
  //!
  //! compute  histogram of combined pairs
  //!
  virtual void calculate(ParticlePairDerivedHistos * pp,
                         ParticlePairDerivedHistos * pm,
                         ParticlePairDerivedHistos * mp,
                         ParticlePairDerivedHistos * mm,
                         double app, double apm, double amp, double amm);
  
protected:
  
  TH2* h_R2_etaEta;
  TH2* h_R2_phiPhi;
  TH2* h_R2_ptpt;
  TH2* h_R2_DetaDphi_shft;
  TH2* h_R2_yY;
  TH2* h_R2_DyDphi_shft;
  
  TH2* h_DptDpt_etaEta;
  TH2* h_DptDpt_phiPhi;
  TH2* h_DptDpt_DetaDphi_shft;
  TH2* h_DptDpt_yY;
  TH2* h_DptDpt_DyDphi_shft;
  
  TH2* h_P2_etaEta;
  TH2* h_P2_phiPhi;
  TH2* h_P2_DetaDphi_shft;
  TH2* h_P2_yY;
  TH2* h_P2_DyDphi_shft;
  
  TH2* h_G2_etaEta;
  TH2* h_G2_phiPhi;
  TH2* h_G2_DetaDphi_shft;
  TH2* h_G2_yY;
  TH2* h_G2_DyDphi_shft;
  
  TH2* h_BF_etaEta;
  TH2* h_BF_phiPhi;
  TH2* h_BF_DetaDphi_shft;
  TH2* h_BF_yY;
  TH2* h_BF_DyDphi_shft;
  
  
  ClassDef(ParticlePairCombinedHistos,1)
};



#endif /* CAP__ParticlePairCombinedHistos */
