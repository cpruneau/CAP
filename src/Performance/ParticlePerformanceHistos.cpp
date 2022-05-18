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
  
  h_n1_dPt        = createHistogram(bn+TString("n1_dPt"),  nBins_dpt,  min_dpt,  max_dpt,  "#Delta p_{T}","N");
  h_n1_dPhi       = createHistogram(bn+TString("n1_dPhi"), nBins_dphi, min_dphi, max_dphi, "#Delta #varphi","N");
  h_n1_dPtVsPt    = createHistogram(bn+TString("n1_dPtVsPt"), nBins_pt,min_pt,   max_pt,   nBins_dpt,  min_dpt,  max_dpt, "p_{T}", "#Delta p_{T}","N");
  h_n1_dPhiVsPt   = createHistogram(bn+TString("n1_dPhiVsPt"),nBins_pt,min_pt,   max_pt,   nBins_dphi, min_dphi, max_dphi,"p_{T}", "#Delta #phi","N");
  
  if (fillEta)
    {
    h_n1_dEta       = createHistogram(bn+TString("n1_dEta"),      nBins_deta,  min_deta,  max_deta,  "#Delta#eta","N");
    h_n1_dEtaVsEta  = createHistogram(bn+TString("n1_dEtaVsEta"), nBins_eta,   min_eta,   max_eta,   nBins_deta,  min_deta,  max_deta, "#eta", "#Delta#eta","N");
    }
  
  if (fillY)
    {
    h_n1_dY       = createHistogram(bn+TString("n1_dY"),      nBins_dy,   min_dy,  max_dy,  "#Delta y","N");
    h_n1_dYVsY    = createHistogram(bn+TString("n1_dYVsY"),   nBins_y,    min_y,   max_y,   nBins_dy,  min_dy,  max_dy, "y", "#Delta y","N");
    }
  if ( reportEnd("ParticlePerformanceHistos",getName(),"createHistograms()"))
    { }
}

//________________________________________________________________________
void ParticlePerformanceHistos::loadHistograms(TFile * inputFile)
{
  if ( reportStart("ParticlePerformanceHistos",getName(),"loadHistograms(TFile * inputFile)"))
    { }
  TString fct = "loadHistograms(TFile * inputFile)";
  if (!ptrFileExist(fct,inputFile)) return;
  TString bn = getHistoBaseName();

  Configuration & configuration = getConfiguration();
  fillEta    = configuration.getValueBool("fillEta");
  fillY      = configuration.getValueBool("fillY");
  
  h_n1_dPt        = loadH1(inputFile, bn+TString("n1_dpt"));
  h_n1_dPhi       = loadH1(inputFile, bn+TString("n1_dPhi"));
  h_n1_dPtVsPt    = loadH2(inputFile, bn+TString("n1_dpt"));
  h_n1_dPhiVsPt   = loadH2(inputFile, bn+TString("n1_dpt"));
  
  if (fillEta)
    {
    h_n1_dEta       = loadH1(inputFile, bn+TString("n1_deta"));
    h_n1_dEtaVsEta  = loadH2(inputFile, bn+TString("n1_dEtaVsEta"));
    }
  
  if (fillY)
    {
    h_n1_dY       = loadH1(inputFile, bn+TString("n1_dY"));
    h_n1_dYVsY    = loadH2(inputFile, bn+TString("n1_dYVsY"));
    }
  
  if ( reportEnd("ParticlePerformanceHistos",getName(),"loadHistograms(TFile * inputFile)"))
    { }
}

//!
//! Fiil  histograms of this class with differences between reco and gen particles...
//!
void ParticlePerformanceHistos::fill(Particle & recoParticle, double weight)
{
  Particle & genParticle  = *recoParticle.getTruth();
  TLorentzVector & recoMomentum = recoParticle.getMomentum();
  TLorentzVector & genMomentum  = genParticle.getMomentum();
  
  
  double    recoPt   = recoMomentum.Pt();
  double    recoEta  = recoMomentum.Eta();
  double    recoY    = recoMomentum.Y();
  double    recoPhi  = recoMomentum.Pt();
  
  double    genPt   = genMomentum.Pt();
  double    genEta  = genMomentum.Eta();
  double    genY    = genMomentum.Y();
  double    genPhi  = genMomentum.Pt();
  
  double dPt  = recoPt  - genPt;
  double dPhi = recoPhi - genPhi;
  double dEta = recoEta - genEta;
  double dY   = recoY   - genY;
  
  h_n1_dPt->Fill(dPt,weight);
  h_n1_dPhi->Fill(dPhi,weight);
  h_n1_dPtVsPt->Fill(genPt,dPt,weight);
  h_n1_dPhiVsPt->Fill(genPt,dPhi,weight);
  
  if (fillEta)
    {
    h_n1_dEta->Fill(dEta,weight);
    h_n1_dEtaVsEta->Fill(genEta,dEta,weight);
    }
  
  if (fillY)
    {
    h_n1_dY->Fill(dY,weight);
    h_n1_dYVsY->Fill(genY,dY,weight);
    }
}
