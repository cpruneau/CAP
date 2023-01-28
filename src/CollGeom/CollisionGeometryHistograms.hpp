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
#ifndef CAP__CollisionGeometryHistograms
#define CAP__CollisionGeometryHistograms
#include "HistogramGroup.hpp"
#include "Event.hpp"

namespace CAP
{

class CollisionGeometryHistograms : public HistogramGroup
{
public:

  CollisionGeometryHistograms(Task * _parent,
                              const String & _name,
                              Configuration & _configuration);
  virtual ~CollisionGeometryHistograms() { }

  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void fill(Event & event, double weight);
  virtual void noFill(Event & event, double weight);
  virtual void calculateRms(TProfile * h1, TProfile * h1Sq, TH1* h1Rms, TH1* h1Omega, TH1* h1R2);

protected:

  TH1      * h_nProcessedVsB;
  TH1      * h_nAcceptedVsB;
  TH2      * h_nPartVsB;
  TH2      * h_nBinaryVsB;
  TProfile * h_nPartVsB_Prof;
  TProfile * h_nPartSqVsB_Prof;
  TProfile * h_nBinaryVsB_Prof;
  TProfile * h_nBinarySqVsB_Prof;

  TH1      * h_nPart;
  TProfile * h_nBinaryVsNPart_Prof;
  TProfile * h_nBinarySqVsNPart_Prof;
  TProfile * h_bVsNPart_Prof;
  TProfile * h_bSqVsNPart_Prof;

  TH1      * h_nBinary;
  TProfile * h_nPartVsNBinary_Prof;
  TProfile * h_nPartSqVsNBinary_Prof;
  TProfile * h_bVsNBinary_Prof;
  TProfile * h_bSqVsNBinary_Prof;

  TH1      * h_crossSection;
  TProfile * h_nPartVsXsect_Prof;
  TProfile * h_nPartSqVsXsect_Prof;
  TProfile * h_nBinaryVsXsect_Prof;
  TProfile * h_nBinarySqVsXsect_Prof;
  TProfile * h_bVsXsect_Prof;
  TProfile * h_bSqVsXsect_Prof;

  TH2      * h_xyDistInteractions;
  TH2      * h_xyDistNucleons;

  // Derived HistogramGroup
  TH1      * h_nPartRmsVsB;
  TH1      * h_nPartOmegaVsB;
  TH1      * h_nPartR2VsB;
  TH1      * h_nBinaryRmsVsB;
  TH1      * h_nBinaryOmegaVsB;
  TH1      * h_nBinaryR2VsB;
  TH1      * h_nBinaryRmsVsNPart;
  TH1      * h_nBinaryOmegaVsNPart;
  TH1      * h_nBinaryR2VsNPart;
  TH1      * h_bRmsVsNPart;
  TH1      * h_bOmegaVsNPart;
  TH1      * h_bR2VsNPart;

  TH1      * h_nPartRmsVsNBinary;
  TH1      * h_nPartOmegaVsNBinary;
  TH1      * h_nPartR2VsNBinary;
  TH1      * h_bRmsVsNBinary;
  TH1      * h_bOmegaVsNBinary;
  TH1      * h_bR2VsNBinary;

  TH1      * h_nPartRmsVsXsect;
  TH1      * h_nPartOmegaVsXsect;
  TH1      * h_nPartR2VsXsect;

  TH1      * h_nBinaryRmsVsXsect;
  TH1      * h_nBinaryOmegaVsXsect;
  TH1      * h_nBinaryR2VsXsect;
  TH1      * h_bRmsVsXsect;
  TH1      * h_bOmegaVsXsect;
  TH1      * h_bR2VsXsect;


//  TH3      * h_xyNNIntVsB;
//  TProfile * h_varXVsB_Prof;
//  TProfile * h_varYVsB_Prof;
//  TProfile * h_covXYVsB_Prof;
//  TProfile * h_epsilonXVsB_Prof;
//  TProfile * h_epsilonYVsB_Prof;
//  TProfile * h_epsilonXYVsB_Prof;
//  TH3      * h_epsilonXYVsB;
//  TProfile * h_psi2VsB_Prof;
//  TH2      * h_psi2VsB;

  ClassDef(CollisionGeometryHistograms,0)

};

}
#endif /* CAP__CollisionGeometryHistograms  */



