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
#include "ParticlePairHistos.hpp"

ClassImp(ParticlePairHistos);

ParticlePairHistos::ParticlePairHistos(const TString &       name,
                                       const Configuration & configuration,
                                       LogLevel  debugLevel)
:
Histograms(name,configuration,debugLevel),
nBins_n2(0),
min_n2(0),
max_n2(0),
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
nBins_Dphi(0),
min_Dphi(0),
max_Dphi(0),
width_Dphi(0),
nBins_Deta(0),
min_Deta(0),
max_Deta(0),
nBins_Dy(0),
min_Dy(0),
max_Dy(0),
nBins_Dphi_shft(0),
min_Dphi_shft(0),
max_Dphi_shft(0),
h_n2(0),
h_n2_ptpt(0),
h_n2_etaEta(0),
h_DptDpt_etaEta(0),
h_n2_phiPhi(0),
h_DptDpt_phiPhi(0),
h_n2_yY(0),
h_DptDpt_yY(0),
h_n2_DetaDphi(0),
h_DptDpt_DetaDphi(0),
h_n2_DyDphi(0),
h_DptDpt_DyDphi(0),
fillEta(false),
fillY(false),
fillP2(false)
{
  setClassName("ParticlePairHistos");
  setInstanceName(name);
}

ParticlePairHistos::~ParticlePairHistos()
{
  // no ops
}

void ParticlePairHistos::createHistograms()
{
  if ( reportStart("ParticlePairHistos",getName(),"createHistograms()"))
    { }
  TString bn = getHistoBaseName();
  
  Configuration & configuration = getConfiguration();
 
  nBins_n2  = configuration.getValueInt("nBins_n2");
  min_n2    = configuration.getValueDouble("min_n2");
  max_n2    = configuration.getValueDouble("max_n2");
  
  nBins_n2  = configuration.getValueInt("nBins_n2");
  min_n2    = configuration.getValueDouble("min_n2");
  max_n2    = configuration.getValueDouble("max_n2");
 
  nBins_pt = configuration.getValueInt("nBins_pt");
  min_pt   = configuration.getValueDouble("min_pt");
  max_pt   = configuration.getValueDouble("max_pt");
  range_pt = max_pt - min_pt;
  
  nBins_phi = configuration.getValueInt("nBins_phi");
  min_phi   = configuration.getValueDouble("min_phi");
  max_phi   = configuration.getValueDouble("max_phi");
  range_phi = max_phi - min_phi;
  
  nBins_eta = configuration.getValueInt("nBins_eta");
  min_eta   = configuration.getValueDouble("min_eta");
  max_eta   = configuration.getValueDouble("max_eta");
  range_eta = max_eta - min_eta;
  
  nBins_y = configuration.getValueInt("nBins_y");
  min_y   = configuration.getValueDouble("min_y");
  max_y   = configuration.getValueDouble("max_y");
  range_y = max_y - min_y;
    
  nBins_Dphi= configuration.getValueInt("nBins_Dphi");
  min_Dphi  = configuration.getValueDouble("min_Dphi");
  max_Dphi  = configuration.getValueDouble("max_Dphi");
  
  nBins_Deta= configuration.getValueInt("nBins_Deta");
  min_Deta  = configuration.getValueDouble("min_Deta");
  max_Deta  = configuration.getValueDouble("max_Deta");
  nBins_Dy  = configuration.getValueInt("nBins_Dy");
  min_Dy    = configuration.getValueDouble("min_Dy");
  max_Dy    = configuration.getValueDouble("max_Dy");

  fillEta    = configuration.getValueBool("fillEta");
  fillY      = configuration.getValueBool("fillY");
  fillP2     = configuration.getValueBool("fillP2");

  nBins_Dphi        = nBins_phi;
  width_Dphi        = TMath::TwoPi()/nBins_Dphi;
  nBins_Dphi_shft   = int(double(nBins_Dphi)/4.0);
  min_Dphi         = -width_Dphi/2.;
  max_Dphi         = TMath::TwoPi() - width_Dphi/2.;

  min_Dphi_shft    = min_Dphi - width_Dphi*double(nBins_Dphi/4);
  max_Dphi_shft    = max_Dphi - width_Dphi*double(nBins_Dphi/4);

  nBins_Deta       = 2*nBins_eta - 1;
  min_Deta         = min_eta - max_eta;
  max_Deta         = max_eta - min_eta;
  
  h_n2          = createHistogram(bn+TString("n2"),         nBins_n2,  min_n2, max_n2, "n_{2}", "Yield");
  h_n2_ptpt     = createHistogram(bn+TString("n2_ptpt"),    nBins_pt,  min_pt, max_pt, nBins_pt, min_pt, max_pt,   "p_{T,1}",  "p_{T,2}", "N_{2}");
  h_n2_phiPhi   = createHistogram(bn+TString("n2_phiPhi"),  nBins_phi, min_phi, max_phi, nBins_phi, min_phi, max_phi, "#varphi_{1}", "#varphi_{2}", "N_{2}");

  if (fillP2)
    {
    h_DptDpt_phiPhi = createHistogram(bn+TString("ptpt_phiPhi"),   nBins_phi, min_phi, max_phi, nBins_phi, min_phi, max_phi, "#varphi_{1}", "#varphi_{2}", "p_{T}xp_{T}");
    }

  if (fillEta)
    {
    h_n2_etaEta   = createHistogram(bn+TString("n2_etaEta"),   nBins_eta,  min_eta, max_eta, nBins_eta, min_eta, max_eta, "#eta_{1}", "#eta_{2}", "N_{2}");
    h_n2_DetaDphi = createHistogram(bn+TString("n2_DetaDphi"), nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta", "#Delta#phi", "N_{2}");
    if (fillP2)
      {
      h_DptDpt_etaEta = createHistogram(bn+TString("ptpt_etaEta"), nBins_eta, min_eta, max_eta, nBins_eta, min_eta, max_eta, "#eta_{1}", "#eta_{2}", "p_{T}xp_{T}");
      h_DptDpt_DetaDphi = createHistogram(bn+TString("ptpt_DetaDphi"),nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta", "#Delta#phi", "ptpt");
      }
    }

  if (fillY)
    {
    h_n2_yY     = createHistogram(bn+TString("n2_yY"),     nBins_y,  min_y,  max_y,  nBins_y, min_y, max_y, "y_{1}","y_{2}", "N_{2}");
    h_n2_DyDphi = createHistogram(bn+TString("n2_DyDphi"), nBins_Dy, min_Dy, max_Dy, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#y", "#Delta#phi", "N_{2}");
    if (fillP2)
      {
      h_DptDpt_yY    = createHistogram(bn+TString("ptpt_yY"),  nBins_y,  min_y, max_y, nBins_y, min_y, max_y, "y_{1}","y_{2}", "p_{T}xp_{T}");
      h_DptDpt_DyDphi = createHistogram(bn+TString("ptpt_DyDphi"),nBins_Dy, min_Dy, max_Dy, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#y", "#Delta#phi", "ptpt");
      }
    }
  if ( reportEnd("ParticlePairHistos",getName(),"createHistograms()"))
    { }
}

//________________________________________________________________________
void ParticlePairHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart("ParticlePairHistos",getName(),"loadHistograms(TFile * inputFile)"))
    { }
  if (!inputFile)
    {
    if (reportFatal("ParticlePairHistos",getName(),"loadHistograms(TFile * inputFile)"))
      {
      cout << "Attempting to load ParticleHistos from an invalid (null) file pointer" << endl;
      }
    return;
    }
  TString bn = getHistoBaseName();
  Configuration & configuration = getConfiguration();
  fillEta    = configuration.getValueBool("fillEta");
  fillY      = configuration.getValueBool("fillY");
  fillP2     = configuration.getValueBool("fillP2");
  h_n2          = loadH1(inputFile, bn+TString("n2"));
  h_n2_ptpt     = loadH2(inputFile, bn+TString("n2_ptpt"));
  h_n2_phiPhi   = loadH2(inputFile, bn+TString("n2_phiPhi"));

  if (fillP2)
    {
    h_DptDpt_phiPhi = loadH2(inputFile, bn+TString("ptpt_phiPhi"));
    }

  if (fillEta)
    {
    h_n2_etaEta   = loadH2(inputFile, bn+TString("n2_etaEta"));
    h_n2_DetaDphi = loadH2(inputFile, bn+TString("n2_DetaDphi"));
    if (fillP2)
      {
      h_DptDpt_etaEta   = loadH2(inputFile, bn+TString("ptpt_etaEta"));
      h_DptDpt_DetaDphi = loadH2(inputFile, bn+TString("ptpt_DetaDphi"));
      }
    }
  if (fillY)
    {
    h_n2_yY     = loadH2(inputFile, bn+TString("n2_yY"));
    h_n2_DyDphi = loadH2(inputFile, bn+TString("n2_DyDphi"));
    if (fillP2)
      {
      h_DptDpt_yY     = loadH2(inputFile, bn+TString("ptpt_yY"));
      h_DptDpt_DyDphi = loadH2(inputFile, bn+TString("ptpt_DyDphi"));
      }
    }
  if ( reportEnd("ParticlePairHistos",getName(),"loadHistograms(TFile * inputFile)"))
    { }
}

void ParticlePairHistos::fill(vector<ParticleDigit*> & particle1, vector<ParticleDigit*> & particle2, bool same, double weight)
{
  double nPairs = 0;
  int iG;

  for (unsigned int iPart_1=0; iPart_1<particle1.size(); iPart_1++)
    {
    float pt1 = particle1[iPart_1]->pt;
    float dpt1 = pt1; // - avgPt1
    unsigned int iPt_1  = particle1[iPart_1]->iPt;
    unsigned int iPhi_1 = particle1[iPart_1]->iPhi;
    unsigned int iEta_1 = particle1[iPart_1]->iEta;
    unsigned int iY_1   = particle1[iPart_1]->iY;

    for (unsigned int iPart_2=(same?iPart_1+1: 0); iPart_2<particle2.size(); iPart_2++)
      {
      float pt2 = particle2[iPart_2]->pt;
      float dpt2 = pt2; // - avgPt2
      unsigned int iPt_2  = particle2[iPart_2]->iPt;
      unsigned int iPhi_2 = particle2[iPart_2]->iPhi;
      unsigned int iEta_2 = particle2[iPart_2]->iEta;
      unsigned int iY_2   = particle2[iPart_2]->iY;


      nPairs += 1.0;
      iG = h_n2_ptpt->GetBin(iPt_1,iPt_2);
      h_n2_ptpt->AddBinContent(iG,  weight);

      iG = h_n2_phiPhi->GetBin(iPhi_1,iPhi_2);
      h_n2_phiPhi->AddBinContent(iG,  weight);
      if (fillP2)
        {
        h_DptDpt_phiPhi ->AddBinContent(iG,weight*dpt1*dpt2);
        }
      if (same)
        {
        nPairs++;
        iG = h_n2_ptpt->GetBin(iPt_2,iPt_1);
        h_n2_ptpt->AddBinContent(iG,  weight);
        iG = h_n2_phiPhi->GetBin(iPhi_2,iPhi_1);
        h_n2_phiPhi->AddBinContent(iG,  weight);
        if (fillP2)
          {
          h_DptDpt_phiPhi->AddBinContent(iG,weight*pt1*pt2);
          }
        }

      if (fillEta)
        {
        // delta-eta maps onto a 2n-1 range i.e., 0 to 2n-2
        int iDeltaEta  = iEta_1-iEta_2 + nBins_eta-1;
        int iDeltaPhi  = iPhi_1-iPhi_2;
        if (iDeltaPhi < 0) iDeltaPhi += nBins_phi;

        iG = h_n2_etaEta->GetBin(iEta_1,iEta_2);
        h_n2_etaEta->AddBinContent(iG,weight);
        if (fillP2)
          {
          h_DptDpt_etaEta->AddBinContent(iG,weight*pt1*pt2);
          }

        iG = h_n2_DetaDphi->GetBin(iDeltaEta+1,iDeltaPhi+1);
        h_n2_DetaDphi->AddBinContent(iG,weight);
        if (fillP2)
          {
          h_DptDpt_DetaDphi->AddBinContent(iG,weight*pt1*pt2);
          }
        if (same)
          {
          // delta-eta maps onto a 2n-1 range i.e., 0 to 2n-2
          iDeltaEta  = iEta_2-iEta_1 + nBins_eta-1;
          iDeltaPhi  = iPhi_2-iPhi_1;
          if (iDeltaPhi < 0) iDeltaPhi += nBins_phi;
          iG = h_n2_etaEta->GetBin(iEta_2,iEta_1);
          h_n2_etaEta->AddBinContent(iG,weight);
          if (fillP2)
            {
            h_DptDpt_etaEta->AddBinContent(iG,weight*pt1*pt2);
            }

          iG = h_n2_DetaDphi->GetBin(iDeltaEta+1,iDeltaPhi+1);
          h_n2_DetaDphi->AddBinContent(iG,weight);
          if (fillP2)
            {
            h_DptDpt_DetaDphi->AddBinContent(iG,weight*pt1*pt2);
            }
          }
        }

      if (fillY)
        {
        int iDeltaY    = iY_1-iY_2 + nBins_y-1;
        int iDeltaPhi  = iPhi_1-iPhi_2;
        if (iDeltaPhi < 0) iDeltaPhi += nBins_phi;

        iG = h_n2_yY->GetBin(iY_1,iY_2);
        h_n2_yY->AddBinContent(iG,weight);
        if (fillP2)
          {
          h_DptDpt_yY->AddBinContent(iG,weight*pt1*pt2);
          }

        iG = h_n2_DyDphi->GetBin(iDeltaY+1,iDeltaPhi+1);
        h_n2_DyDphi  ->AddBinContent(iG,weight);
        if (fillP2)
          {
          h_DptDpt_DyDphi->AddBinContent(iG,weight*pt1*pt2);
          }
        if (same)
          {
          int iDeltaY   = iY_2-iY_1 + nBins_y-1;
          int iDeltaPhi  = iPhi_2-iPhi_1;
          if (iDeltaPhi < 0) iDeltaPhi += nBins_phi;

          iG = h_n2_yY->GetBin(iY_1,iY_2);
          h_n2_yY->AddBinContent(iG,weight);
          if (fillP2)
            {
            h_DptDpt_yY->AddBinContent(iG,weight*pt1*pt2);
            }

          iG = h_n2_DyDphi->GetBin(iDeltaY+1,iDeltaPhi+1);
          h_n2_DyDphi  ->AddBinContent(iG,weight);
          if (fillP2)
            {
            h_DptDpt_DyDphi->AddBinContent(iG,weight*pt1*pt2);
            }
          }
        }
      }
    }

  // Update number of entries
  h_n2_ptpt->SetEntries(h_n2_ptpt->GetEntries()+nPairs);
  h_n2_phiPhi->SetEntries(h_n2_phiPhi->GetEntries()+nPairs);
  if (fillP2)
    {
    h_DptDpt_phiPhi->SetEntries(h_DptDpt_phiPhi->GetEntries()+nPairs);
    }
  if (fillEta)
    {
    h_n2_etaEta->SetEntries(h_n2_etaEta->GetEntries()+nPairs);
    h_n2_DetaDphi->SetEntries(h_n2_DetaDphi->GetEntries()+nPairs);
    if (fillP2)
      {
      h_DptDpt_etaEta->SetEntries(h_DptDpt_etaEta->GetEntries()+nPairs);
      h_DptDpt_DetaDphi->SetEntries(h_DptDpt_DetaDphi->GetEntries()+nPairs);
      }
    }

  if (fillY)
    {
    h_n2_yY->SetEntries(h_n2_yY->GetEntries()+nPairs);
    h_n2_DyDphi->SetEntries(h_n2_DyDphi->GetEntries()+nPairs);
    if (fillP2)
      {
      h_DptDpt_yY->SetEntries(h_DptDpt_yY->GetEntries()+nPairs);
      h_DptDpt_DyDphi->SetEntries(h_DptDpt_DyDphi->GetEntries()+nPairs);
      }
    }
  h_n2->Fill(double(nPairs),weight);
}

