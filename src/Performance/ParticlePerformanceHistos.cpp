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
#include "TLorentzVector.h"
#include "ParticlePerformanceHistos.hpp"
ClassImp(ParticlePerformanceHistos);

ParticlePerformanceHistos::ParticlePerformanceHistos(const TString &       _name,
                                                     const Configuration & _configuration,
                                                     LogLevel              _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
fillEta(0),
fillY(0),
nBins_pt(0),
min_pt(0),
max_pt(0),
nBins_phi(0),
min_phi(0),
max_phi(0),
nBins_eta(0),
min_eta(0),
max_eta(0),
nBins_y(0),
min_y(0),
max_y(0),
h_n1_dPt(nullptr),
h_n1_dEta(nullptr),
h_n1_dY(nullptr),
h_n1_dPhi(nullptr),
h_n1_dPtVsPt(nullptr),
h_n1_dEtaVsEta(nullptr),
h_n1_dYVsY(nullptr),
h_n1_dPhiVsPt(nullptr)
{
  appendClassName("ParticlePerformanceHistos");
  setInstanceName(_name);
}


// for now use the same boundaries for eta and y histogram
void ParticlePerformanceHistos::createHistograms()
{
  if ( reportStart("ParticlePerformanceHistos",getName(),"createHistograms()"))
    { }
  TString bn = getHistoBaseName();
  Configuration & configuration = getConfiguration();
  
  fillEta    = configuration.getValueBool("fillEta");
  fillY      = configuration.getValueBool("fillY");
  
  nBins_pt   = configuration.getValueInt("nBins_pt");
  min_pt     = configuration.getValueDouble("min_pt");
  max_pt     = configuration.getValueDouble("max_pt");
  
  nBins_dpt  = configuration.getValueInt("nBins_dpt");
  min_dpt    = configuration.getValueDouble("min_dpt");
  max_dpt    = configuration.getValueDouble("max_dpt");
  
  nBins_phi = configuration.getValueInt("nBins_phi");
  min_phi   = configuration.getValueDouble("min_phi");
  max_phi   = configuration.getValueDouble("max_phi");
  
  nBins_dphi = configuration.getValueInt("nBins_dphi");
  min_dphi   = configuration.getValueDouble("min_dphi");
  max_dphi   = configuration.getValueDouble("max_dphi");
  
  nBins_eta = configuration.getValueInt("nBins_eta");
  min_eta   = configuration.getValueDouble("min_eta");
  max_eta   = configuration.getValueDouble("max_eta");
  
  nBins_deta = configuration.getValueInt("nBins_deta");
  min_deta   = configuration.getValueDouble("min_deta");
  max_deta   = configuration.getValueDouble("max_deta");
  
  nBins_y = configuration.getValueInt("nBins_y");
  min_y   = configuration.getValueDouble("min_y");
  max_y   = configuration.getValueDouble("max_y");
  
  nBins_dy = configuration.getValueInt("nBins_dy");
  min_dy   = configuration.getValueDouble("min_dy");
  max_dy   = configuration.getValueDouble("max_dy");
  
  h_n1_dPt        = createHistogram(makeName(bn,"n1_dPt"),  nBins_dpt,  min_dpt,  max_dpt,  "#Delta p_{T}","N");
  h_n1_dPhi       = createHistogram(makeName(bn,"n1_dPhi"), nBins_dphi, min_dphi, max_dphi, "#Delta #varphi","N");
  h_n1_dPtVsPt    = createHistogram(makeName(bn,"n1_dPtVsPt"), nBins_pt,min_pt,   max_pt,   nBins_dpt,  min_dpt,  max_dpt, "p_{T}", "#Delta p_{T}","N");
  h_n1_dPhiVsPt   = createHistogram(makeName(bn,"n1_dPhiVsPt"),nBins_pt,min_pt,   max_pt,   nBins_dphi, min_dphi, max_dphi,"p_{T}", "#Delta #phi","N");
  
  if (fillEta)
    {
    h_n1_dEta       = createHistogram(makeName(bn,"n1_dEta"),      nBins_deta,  min_deta,  max_deta,  "#Delta#eta","N");
    h_n1_dEtaVsEta  = createHistogram(makeName(bn,"n1_dEtaVsEta"), nBins_eta,   min_eta,   max_eta,   nBins_deta,  min_deta,  max_deta, "#eta", "#Delta#eta","N");
    }
  
  if (fillY)
    {
    h_n1_dY       = createHistogram(makeName(bn,"n1_dY"),      nBins_dy,   min_dy,  max_dy,  "#Delta y","N");
    h_n1_dYVsY    = createHistogram(makeName(bn,"n1_dYVsY"),   nBins_y,    min_y,   max_y,   nBins_dy,  min_dy,  max_dy, "y", "#Delta y","N");
    }
  if ( reportEnd("ParticlePerformanceHistos",getName(),"createHistograms()"))
    { }
}

//________________________________________________________________________
void ParticlePerformanceHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__,inputFile)) return;
  TString bn = getHistoBaseName();

  Configuration & configuration = getConfiguration();
  fillEta    = configuration.getValueBool("fillEta");
  fillY      = configuration.getValueBool("fillY");
  
  h_n1_dPt        = loadH1(inputFile, makeName(bn,"n1_dpt"));
  h_n1_dPhi       = loadH1(inputFile, makeName(bn,"n1_dPhi"));
  h_n1_dPtVsPt    = loadH2(inputFile, makeName(bn,"n1_dpt"));
  h_n1_dPhiVsPt   = loadH2(inputFile, makeName(bn,"n1_dpt"));
  
  if (fillEta)
    {
    h_n1_dEta       = loadH1(inputFile, makeName(bn,"n1_deta"));
    h_n1_dEtaVsEta  = loadH2(inputFile, makeName(bn,"n1_dEtaVsEta"));
    }
  
  if (fillY)
    {
    h_n1_dY       = loadH1(inputFile, makeName(bn,"n1_dY"));
    h_n1_dYVsY    = loadH2(inputFile, makeName(bn,"n1_dYVsY"));
    }
  
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Fiil  histograms of this class with differences between reco and gen particles..
//!
void ParticlePerformanceHistos::fill(Particle & recoParticle, double weight)
{
  Particle & genParticle  = *recoParticle.getTruth();

  //if (reportInfo(__FUNCTION__)) cout << "got genParticle" << endl;

  TLorentzVector & recoMomentum = recoParticle.getMomentum();
  TLorentzVector & genMomentum  = genParticle.getMomentum();

//  if (reportInfo(__FUNCTION__)) cout << "got genMomentum" << endl;

  double recoPt, recoEta, recoY, recoPhi;
  double genPt, genEta, genY, genPhi;
  double dPt, dPhi, dEta, dY;

  recoPt   = recoMomentum.Pt();
  recoEta  = recoMomentum.Eta();
  if (fillY) recoY = recoMomentum.Y();
  recoPhi  = recoMomentum.Phi();

  //if (reportInfo(__FUNCTION__)) cout << "got reco stuff " << endl;


  genPt   = genMomentum.Pt();
  genPhi  = genMomentum.Phi();
  if (fillEta) genEta  = genMomentum.Eta();
  if (fillY)   genY    = genMomentum.Y();

  //if (reportInfo(__FUNCTION__)) cout << "got gen stuff also " << endl;

  dPt  = recoPt  - genPt;
  dPhi = recoPhi - genPhi;
  if (fillEta) dEta = recoEta - genEta;
  if (fillY)   dY   = recoY   - genY;

  //if (reportInfo(__FUNCTION__)) cout << "will fill histos " << endl;


  h_n1_dPt->Fill(dPt,weight);
  h_n1_dPhi->Fill(dPhi,weight);
  h_n1_dPtVsPt->Fill(genPt,dPt,weight);
  h_n1_dPhiVsPt->Fill(genPt,dPhi,weight);

  //if (reportInfo(__FUNCTION__)) cout << "will fill histos 1 " << endl;

  if (fillEta)
    {
    //if (reportInfo(__FUNCTION__)) cout << "will fill histos 2" << endl;

    h_n1_dEta->Fill(dEta,weight);
    h_n1_dEtaVsEta->Fill(genEta,dEta,weight);
    }
  
  if (fillY)
    {
    //if (reportInfo(__FUNCTION__)) cout << "will fill histos 3" << endl;

    h_n1_dY->Fill(dY,weight);
    h_n1_dYVsY->Fill(genY,dY,weight);
    }
}
