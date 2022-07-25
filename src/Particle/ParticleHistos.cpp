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
#include "ParticleHistos.hpp"
ClassImp(ParticleHistos);

ParticleHistos::ParticleHistos(const TString &       _name,
                               const Configuration & _configuration,
                               LogLevel              _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
fillEta(0),
fillY(0),
fillP2(0),
useEffCorrection(0),
efficiencyOpt(0),
nBins_n1(0),
min_n1(0),
max_n1(0),
nBins_pt(0),
min_pt(0),
max_pt(0),
scale_pt(0),
nBins_phi(0),
min_phi(0),
max_phi(0),
scale_phi(0),
nBins_eta(0),
min_eta(0),
max_eta(0),
range_eta(0),
nBins_y(0),
min_y(0),
max_y(0),
range_y(0),
h_n1(nullptr),
h_n1_eTotal(nullptr),
h_n1_pt(nullptr),
h_n1_ptXS(nullptr),
h_n1_phiEta(nullptr),
h_spt_phiEta(nullptr),
h_n1_phiY(nullptr),
h_spt_phiY(nullptr),
h_pdgId(nullptr)
{
  appendClassName("ParticleHistos");
  setInstanceName(_name);
}

ParticleHistos::~ParticleHistos()
{
  //deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void ParticleHistos::createHistograms()
{
  if ( reportStart("ParticleHistos",getName(),"createHistograms()"))
    { }
  TString bn = getHistoBaseName();
  Configuration & configuration = getConfiguration();
  
  nBins_n1 = configuration.getValueInt("nBins_n1");
  min_n1   = configuration.getValueDouble("min_n1");
  max_n1   = configuration.getValueDouble("max_n1");
  
  nBins_pt = configuration.getValueInt("nBins_pt");
  min_pt   = configuration.getValueDouble("min_pt");
  max_pt   = configuration.getValueDouble("max_pt");
  scale_pt = max_pt - min_pt;
  
  nBins_phi = configuration.getValueInt("nBins_phi");
  min_phi   = configuration.getValueDouble("min_phi");
  max_phi   = configuration.getValueDouble("max_phi");
  scale_phi = max_phi - min_phi;
  
  nBins_eta = configuration.getValueInt("nBins_eta");
  min_eta   = configuration.getValueDouble("min_eta");
  max_eta   = configuration.getValueDouble("max_eta");
  range_eta = max_eta - min_eta;
  
  nBins_y = configuration.getValueInt("nBins_y");
  min_y   = configuration.getValueDouble("min_y");
  max_y   = configuration.getValueDouble("max_y");
  range_y = max_y - min_y;
  
  fillEta = configuration.getValueBool("fillEta");
  fillY   = configuration.getValueBool("fillY");
  fillP2  = configuration.getValueBool("fillP2");
  
  h_n1         = createHistogram(makeName(bn,"n1"),           nBins_n1,  min_n1,  max_n1,  "n_1","N");
  h_n1_eTotal  = createHistogram(makeName(bn,"n1_eTotal"),    nBins_n1,  min_n1,  10.0*max_n1,  "n1_eTotal","N");
  h_n1_pt      = createHistogram(makeName(bn,"n1_pt"),        nBins_pt,  min_pt,  max_pt,  "p_{T}","N");
  h_n1_ptXS    = createHistogram(makeName(bn,"n1_ptXS"),      nBins_pt,  min_pt,  max_pt,  "p_{T}","1/p_{T} dN/p_{T}");

  if (fillEta)
    {
    h_n1_phiEta  = createHistogram(makeName(bn,"n1_phiEta"),  nBins_eta, min_eta, max_eta, nBins_phi, min_phi, max_phi, "#eta", "#varphi","N");
    if (fillP2)
      {
      h_spt_phiEta  = createHistogram(makeName(bn,"spt_phiEta"),  nBins_eta, min_eta, max_eta, nBins_phi, min_phi, max_phi, "#eta", "#varphi","N");
      }
    }
  if (fillY)
    {
    h_n1_phiY  = createHistogram(makeName(bn,"n1_phiY"),     nBins_y,   min_y,   max_y, nBins_phi, min_phi, max_phi, "y", "#varphi","N");
    if (fillP2)
      {
      h_spt_phiY  = createHistogram(makeName(bn,"spt_phiY"),  nBins_y,   min_y,   max_y, nBins_phi, min_phi, max_phi, "y", "#varphi","N");
      }
    }

  h_pdgId  = createHistogram(makeName(bn,"n1_indexId"),   400,  -0.5, 399.5, "Index", "N");

  if ( reportEnd("ParticleHistos",getName(),"createHistograms()"))
    { }
}




//________________________________________________________________________
void ParticleHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__,inputFile)) return;

  Configuration & configuration = getConfiguration();
  fillEta    = configuration.getValueBool("fillEta");
  fillY      = configuration.getValueBool("fillY");
  fillP2     = configuration.getValueBool("fillP2");
  TString bn = getHistoBaseName();
  h_n1         = loadH1(inputFile,  makeName(bn,"n1"));
  h_n1_eTotal  = loadH1(inputFile,  makeName(bn,"n1_eTotal"));
  h_n1_pt      = loadH1(inputFile,  makeName(bn,"n1_pt"));
  h_n1_ptXS    = loadH1(inputFile,  makeName(bn,"n1_ptXS"));

  if (fillEta)
    {
    h_n1_phiEta  = loadH2(inputFile,  makeName(bn,"n1_phiEta"));
    if (fillP2)
      {
      h_spt_phiEta  = loadH2(inputFile,  makeName(bn,"spt_phiEta"));
      }
    }
  if (fillY)
    {
    h_n1_phiY  = loadH2(inputFile,  makeName(bn,"n1_phiY"));
    if (fillP2)
      {
      h_spt_phiY  = loadH2(inputFile,  makeName(bn,"spt_phiY"));
      }
    }

  h_pdgId  = loadH2(inputFile,  makeName(bn,"n1_indexId"));

  if (reportEnd(__FUNCTION__))
    ;
}

void ParticleHistos::loadCalibration(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__,inputFile)) return;

  Configuration & configuration = getConfiguration();
  useEffCorrection = true;
  efficiencyOpt    = configuration.getValueInt("efficientOpt");
  TString bn = getHistoBaseName();

  switch (efficiencyOpt)
    {
      case 0: // pT dependence only
      h_eff_pt = loadH1(inputFile,  makeName(bn,"eff_pt"));
      break;

      case 1: // pT vs eta dependence
      h_eff_ptEta = loadH2(inputFile,  makeName(bn,"eff_ptEta"));
      break;

      case 2: // pT vs y dependence
      h_eff_ptY   = loadH2(inputFile,  makeName(bn,"eff_ptY"));
      break;

      case 3: // pT vs vs phi vs eta dependence
      h_eff_ptPhiEta = loadH3(inputFile,  makeName(bn,"eff_ptPhiEta"));

      case 4: // pT vs vs phi vs y dependence
      h_eff_ptPhiY = loadH3(inputFile,  makeName(bn,"eff_ptPhiY"));
      break;

    }
  if (reportEnd(__FUNCTION__))
    ;
}


//!
//! Fiil  single particle histograms of this class with the particles contained in the given list.
//!
void ParticleHistos::fill(vector<ParticleDigit*> & particles, double weight)
{
  double nSingles      = 0;
  double nSinglesEta   = 0;
  double nSinglesY     = 0;
  double totalEnergy   = 0;

  for (unsigned int iPart=0; iPart<particles.size(); iPart++)
    {
    float        e    = particles[iPart]->e;
    float        pt   = particles[iPart]->pt;
    float        eta  = particles[iPart]->eta;
    float        phi  = particles[iPart]->phi;
    float        y    = particles[iPart]->y;
    unsigned int iPt  = particles[iPart]->iPt;
    unsigned int iPhi = particles[iPart]->iPhi;
    unsigned int iEta = particles[iPart]->iEta;
    unsigned int iY   = particles[iPart]->iY;

    if (useEffCorrection)
      {
      int k = 0;
      double eff = 0.0;
      switch (efficiencyOpt)
        {
          case 0:
          k = h_eff_pt->FindBin(pt);
          eff = h_eff_pt->GetBinContent(k);
          break;

          case 1:
          k = h_eff_ptEta->FindBin(eta,pt);
          eff = h_eff_ptEta->GetBinContent(k);
          break;

          case 2:
          k = h_eff_ptY->FindBin(y,pt);
          eff = h_eff_ptY->GetBinContent(k);
          break;

          case 3:
          k = h_eff_ptPhiEta->FindBin(eta,phi,pt);
          eff = h_eff_ptPhiEta->GetBinContent(k);
          break;

          case 4:
          k = h_eff_ptPhiY->FindBin(y,phi,pt);
          eff = h_eff_ptPhiY->GetBinContent(k);
          break;
        }
      if (eff>0) weight /= eff;
      }

    nSingles++;
    totalEnergy += e;

    int iG = h_n1_pt->GetBin(iPt);
    h_n1_pt  ->AddBinContent(iG,weight);
    h_n1_ptXS->AddBinContent(iG,weight/pt);

    if (fillEta)
      {
      iG = h_n1_phiEta->GetBin(iEta,iPhi);
      if (iG<=0)
        {
        cout << "iG:" << iG << endl;
        }
      nSinglesEta++;
      h_n1_phiEta->AddBinContent(iG,weight);
      if (fillP2) h_spt_phiEta->AddBinContent(iG,weight*pt);
      }

    if (fillY)
      {
      iG = h_n1_phiY->GetBin(iY,iPhi);
      if (iG<=0)
        {
        cout << "iG:" << iG << endl;
        }
      nSinglesY++;
      h_n1_phiY->AddBinContent(iG,weight);
      if (fillP2) h_spt_phiY->AddBinContent(iG,weight*pt);
      }
    }
  h_n1_pt->SetEntries(h_n1_pt->GetEntries()+nSingles);
  h_n1_ptXS->SetEntries(h_n1_ptXS->GetEntries()+nSingles);
  if (fillEta)
    {
    h_n1_phiEta->SetEntries(h_n1_phiEta->GetEntries()+nSinglesEta);
    if (fillP2) h_spt_phiEta->SetEntries(h_spt_phiEta->GetEntries()+nSinglesEta);
    }
  if (fillY)
    {
    h_n1_phiY->SetEntries(h_n1_phiY->GetEntries()+nSinglesY);
    if (fillP2) h_spt_phiY->SetEntries(h_spt_phiY->GetEntries()+nSinglesY);
    }
  h_n1->Fill(nSingles, weight);
  h_n1_eTotal->Fill(totalEnergy, weight);
}

//!
//! Fiil  single particle histograms of this class with the particles contained in the given list.
//!
void ParticleHistos::fill(Particle & particle, double weight)
{
  TLorentzVector & momentum = particle.getMomentum();
  float pt   = momentum.Pt();
  float eta  = momentum.Eta();
  float phi  = momentum.Phi();
  float rapidity = momentum.Rapidity();
  if (phi<0) phi += TMath::TwoPi();

  if (useEffCorrection)
    {
    int k = 0;
    double eff = 0.0;
    switch (efficiencyOpt)
      {
        case 0:
        k = h_eff_pt->FindBin(pt);
        eff = h_eff_pt->GetBinContent(k);
        break;

        case 1:
        k = h_eff_ptEta->FindBin(eta,pt);
        eff = h_eff_ptEta->GetBinContent(k);
        break;

        case 2:
        k = h_eff_ptY->FindBin(rapidity,pt);
        eff = h_eff_ptY->GetBinContent(k);
        break;

        case 3:
        k = h_eff_ptPhiEta->FindBin(eta,phi,pt);
        eff = h_eff_ptPhiEta->GetBinContent(k);
        break;

        case 4:
        k = h_eff_ptPhiY->FindBin(rapidity,phi,pt);
        eff = h_eff_ptPhiY->GetBinContent(k);
        break;
      }
    if (eff>0) weight /= eff;
    }

  h_n1_pt  ->Fill(pt,weight);
  h_n1_ptXS->Fill(pt,weight/pt);
  if (fillEta)
    {
    h_n1_phiEta->Fill(eta,phi,weight);
    if (fillP2) h_spt_phiEta->Fill(eta,phi,weight*pt);
    }
  if (fillY)
    {
    h_n1_phiY->Fill(rapidity,phi,weight);
    if (fillP2) h_spt_phiY->Fill(rapidity,phi,weight*pt);
    }

  double pdgIndex = ParticleTypeCollection::getMasterParticleCollection()->findIndexForType(particle.getTypePtr());
  h_pdgId->Fill(pdgIndex);
}

//!
//! Fiil the global histogram for the multiplicity and the total energy of an event.
//! Call this function only once per event.
//!
void ParticleHistos::fillMultiplicity(double nAccepted, double totalEnergy, double weight)
{
  h_n1->Fill(nAccepted, weight);
  h_n1_eTotal->Fill(totalEnergy, weight);
}
