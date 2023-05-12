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
#ifndef CAP__ParticleSingleDerivedHistos
#define CAP__ParticleSingleDerivedHistos
#include "ParticleSingleHistos.hpp"
#include "Configuration.hpp"

namespace CAP
{

class ParticleSingleDerivedHistos : public HistogramGroup
{
public:

  ParticleSingleDerivedHistos(Task * _parent,
                        const String & _name,
                        const Configuration & _configuration);
  virtual ~ParticleSingleDerivedHistos() {}
  virtual void createHistograms();
  virtual void importHistograms(TFile & inputFile);
  virtual void calculateDerivedHistograms(ParticleSingleHistos* baseHistos);

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
  ////////////////////////////////////////////////////////////////////////////
  bool fillEta;
  bool fillY;
  bool fillP2;
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

  TH1 * h_n1_phi;
  TH1 * h_n1_eta;
  TH1 * h_n1_y;

  TH1 * h_spt_phi;
  TH1 * h_spt_eta;
  TH1 * h_spt_y;
  TH1 * h_pt_phi;
  TH1 * h_pt_eta;
  TH2 * h_pt_phiEta;
  TH1 * h_pt_y;
  TH2 * h_pt_phiY;

  ClassDef(ParticleSingleDerivedHistos,0)

};

} // namespace CAP

#endif /* CAP__ParticleSingleDerivedHistos  */



