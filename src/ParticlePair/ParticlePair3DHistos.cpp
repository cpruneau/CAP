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
#include "ParticlePair3DHistos.hpp"

ClassImp(ParticlePair3DHistos);

ParticlePair3DHistos::ParticlePair3DHistos(Task * _parent,
                                           const TString & _name,
                                           const Configuration & _configuration)
:
HistogramGroup(_parent,_name,_configuration),
nBins_n1(0),
min_n1(0),
max_n1(0),
nBins_n2(0),
min_n2(0),
max_n2(0),
nBins_pt(0),
min_pt(0),
max_pt(0),
range_pt(0),
scale_pt(0),
nBins_phi(0),
min_phi(0),
max_phi(0),
range_phi(0),
scale_phi(0),
nBins_eta(0),
min_eta(0),
max_eta(0),
range_eta(0),
scale_eta(0),
nBins_y(0),
min_y(0),
max_y(0),
range_y(0),
scale_y(0),
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
fillEta(false),
fillY(false),
fillP2(false),
h_n2(nullptr),
h_n2_ptpt(nullptr),
h_n2_etaEta(nullptr),
h_DptDpt_etaEta(nullptr),
h_n2_phiPhi(nullptr),
h_DptDpt_phiPhi(nullptr),
h_n2_yY(nullptr),
h_DptDpt_yY(nullptr),
h_n2_DetaDphi(nullptr),
h_DptDpt_DetaDphi(nullptr),
h_n2_DyDphi(nullptr),
h_DptDpt_DyDphi(nullptr)
{
  appendClassName("ParticlePair3DHistos");
}

void ParticlePair3DHistos::createHistograms()
{
  if ( reportStart(__FUNCTION__))
    { }
  TString bn = getParentName();
  
  const Configuration & configuration = getConfiguration();
 
  nBins_n2  = configuration.getValueInt(getParentName(),"nBins_n2");
  min_n2    = configuration.getValueDouble(getParentName(),"Min_n2");
  max_n2    = configuration.getValueDouble(getParentName(),"Max_n2");
  
  nBins_n2  = configuration.getValueInt(getParentName(),"nBins_n2");
  min_n2    = configuration.getValueDouble(getParentName(),"Min_n2");
  max_n2    = configuration.getValueDouble(getParentName(),"Max_n2");
 
  nBins_pt = configuration.getValueInt(getParentName(),"nBins_pt");
  min_pt   = configuration.getValueDouble(getParentName(),"Min_pt");
  max_pt   = configuration.getValueDouble(getParentName(),"Max_pt");
  range_pt = max_pt - min_pt;
  scale_pt = double(nBins_pt)/range_pt;

  nBins_phi   = configuration.getValueInt(getParentName(),"nBins_phi");
  min_phi     = configuration.getValueDouble(getParentName(),"Min_phi");
  max_phi     = configuration.getValueDouble(getParentName(),"Max_phi");
  range_phi   = max_phi - min_phi;
  scale_phi   = double(nBins_phi)/range_phi;
  width_Dphi  = range_phi/double(nBins_phi);

  nBins_Dphi       = nBins_phi;
  min_Dphi         = 0.0; //-width_Dphi/2.;
  max_Dphi         = TMath::TwoPi(); // - width_Dphi/2.;
  nBins_Dphi_shft  = int(double(nBins_Dphi)/4.0);
  min_Dphi_shft    = min_Dphi - width_Dphi*double(nBins_Dphi_shft);
  max_Dphi_shft    = max_Dphi - width_Dphi*double(nBins_Dphi_shft);

  nBins_eta = configuration.getValueInt(getParentName(),"nBins_eta");
  min_eta   = configuration.getValueDouble(getParentName(),"Min_eta");
  max_eta   = configuration.getValueDouble(getParentName(),"Max_eta");
  range_eta = max_eta - min_eta;
  scale_eta = double(nBins_eta)/range_eta;

  nBins_Deta= 2*nBins_eta-1;
  min_Deta  = -range_eta;
  max_Deta  = range_eta;


  nBins_y = configuration.getValueInt(getParentName(),"nBins_y");
  min_y   = configuration.getValueDouble(getParentName(),"Min_y");
  max_y   = configuration.getValueDouble(getParentName(),"Max_y");
  range_y = max_y - min_y;
  scale_y = double(nBins_y)/range_y;

  nBins_DeltaP  = configuration.getValueInt(getParentName(),"nBins_DeltaP");
  min_DeltaP    = configuration.getValueDouble(getParentName(),"Min_DeltaP");
  max_DeltaP    = configuration.getValueDouble(getParentName(),"Max_DeltaP");

  nBins_Dy  = 2*nBins_y-1;
  min_Dy    = -range_y;
  max_Dy    = range_y;

  fillEta    = configuration.getValueBool(getParentName(),"FillEta");
  fillY      = configuration.getValueBool(getParentName(),"FillY");
  fillP2     = configuration.getValueBool(getParentName(),"FillP2");
  fill3D     = configuration.getValueBool(getParentName(),"Fill3D");

  h_n2          = createHistogram(makeName(bn,"n2"),         nBins_n2,  min_n2, max_n2, "n_{2}", "Yield");
  h_n2_ptpt     = createHistogram(makeName(bn,"n2_ptpt"),    nBins_pt,  min_pt, max_pt, nBins_pt, min_pt, max_pt,   "p_{T,1}",  "p_{T,2}", "N_{2}");
  h_n2_phiPhi   = createHistogram(makeName(bn,"n2_phiPhi"),  nBins_phi, min_phi, max_phi, nBins_phi, min_phi, max_phi, "#varphi_{1}", "#varphi_{2}", "N_{2}");

  if (fillP2)
    {
    h_DptDpt_phiPhi = createHistogram(makeName(bn,"ptpt_phiPhi"),   nBins_phi, min_phi, max_phi, nBins_phi, min_phi, max_phi, "#varphi_{1}", "#varphi_{2}", "p_{T}xp_{T}");
    }

  if (fillEta)
    {
    h_n2_etaEta   = createHistogram(makeName(bn,"n2_etaEta"),   nBins_eta,  min_eta, max_eta, nBins_eta, min_eta, max_eta, "#eta_{1}", "#eta_{2}", "N_{2}");
    h_n2_DetaDphi = createHistogram(makeName(bn,"n2_DetaDphi"), nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta", "#Delta#phi", "N_{2}");
    if (fillP2)
      {
      h_DptDpt_etaEta = createHistogram(makeName(bn,"ptpt_etaEta"), nBins_eta, min_eta, max_eta, nBins_eta, min_eta, max_eta, "#eta_{1}", "#eta_{2}", "p_{T}xp_{T}");
      h_DptDpt_DetaDphi = createHistogram(makeName(bn,"ptpt_DetaDphi"),nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta", "#Delta#phi", "ptpt");
      }
    }

  if (fillY)
    {
    h_n2_yY     = createHistogram(makeName(bn,"n2_yY"),     nBins_y,  min_y,  max_y,  nBins_y, min_y, max_y, "y_{1}","y_{2}", "N_{2}");
    h_n2_DyDphi = createHistogram(makeName(bn,"n2_DyDphi"), nBins_Dy, min_Dy, max_Dy, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#y", "#Delta#phi", "N_{2}");
    if (fillP2)
      {
      h_DptDpt_yY    = createHistogram(makeName(bn,"ptpt_yY"),  nBins_y,  min_y, max_y, nBins_y, min_y, max_y, "y_{1}","y_{2}", "p_{T}xp_{T}");
      h_DptDpt_DyDphi = createHistogram(makeName(bn,"ptpt_DyDphi"),nBins_Dy, min_Dy, max_Dy, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#y", "#Delta#phi", "ptpt");
      }
    }

  if (fill3D)
    {
    h_n2_DeltaP    = createHistogram(makeName(bn,"n2_DeltaP"),
                                     nBins_DeltaP,  min_DeltaP, max_DeltaP,
                                     nBins_DeltaP,  min_DeltaP, max_DeltaP,
                                     nBins_DeltaP,  min_DeltaP, max_DeltaP,
                                     "p_{s}","p_{o}", "p_{l}","n_{2}");
    }

  if ( reportEnd("ParticlePair3DHistos",getParentName(),"createHistograms()"))
    { }
}

//________________________________________________________________________
void ParticlePair3DHistos::importHistograms(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  TString bn = getParentName();
  const Configuration & configuration = getConfiguration();
  fillEta    = configuration.getValueBool(getParentName(),"FillEta");
  fillY      = configuration.getValueBool(getParentName(),"FillY");
  fillP2     = configuration.getValueBool(getParentName(),"FillP2");
  h_n2          = loadH1(inputFile, makeName(bn,"n2"));
  h_n2_ptpt     = loadH2(inputFile, makeName(bn,"n2_ptpt"));
  h_n2_phiPhi   = loadH2(inputFile, makeName(bn,"n2_phiPhi"));

  if (fillP2)
    {
    h_DptDpt_phiPhi = loadH2(inputFile, makeName(bn,"ptpt_phiPhi"));
    }

  if (fillEta)
    {
    h_n2_etaEta   = loadH2(inputFile, makeName(bn,"n2_etaEta"));
    h_n2_DetaDphi = loadH2(inputFile, makeName(bn,"n2_DetaDphi"));
    if (fillP2)
      {
      h_DptDpt_etaEta   = loadH2(inputFile, makeName(bn,"ptpt_etaEta"));
      h_DptDpt_DetaDphi = loadH2(inputFile, makeName(bn,"ptpt_DetaDphi"));
      }
    }
  if (fillY)
    {
    h_n2_yY     = loadH2(inputFile, makeName(bn,"n2_yY"));
    h_n2_DyDphi = loadH2(inputFile, makeName(bn,"n2_DyDphi"));
    if (fillP2)
      {
      h_DptDpt_yY     = loadH2(inputFile, makeName(bn,"ptpt_yY"));
      h_DptDpt_DyDphi = loadH2(inputFile, makeName(bn,"ptpt_DyDphi"));
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePair3DHistos::fill(vector<ParticleDigit*> & particle1, vector<ParticleDigit*> & particle2, bool same, double weight)
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

      if (fillEta && iEta_1!=0 && iEta_2!=0 )
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

      if (fillY && iY_1!=0 && iY_2!=0 )
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

      if (fill3D)
        {
        //decompose(double *pa,double *pb,double &qlong,double &qout,double &qside,double &qinv)
        // ====================================================================================
        // Method from Scott Pratt to do three dimensional qinv components
        // ====================================================================================
        double pt,s,Mlong,roots;
        double ptot[4],q[4];
        const int g[4]={1,-1,-1,-1};
        int alpha;
        qinv=0.0;
        s=0.0;
        for(alpha=0;alpha<4;alpha++)
          {
          ptot[alpha]=pa[alpha]+pb[alpha];
          s+=g[alpha]*ptot[alpha]*ptot[alpha];
          q[alpha]=pa[alpha]-pb[alpha];
          qinv-=g[alpha]*q[alpha]*q[alpha];
          }
        pt=sqrt(ptot[1]*ptot[1]+ptot[2]*ptot[2]);
        Mlong=sqrt(s+pt*pt);
        roots=sqrt(s);

        qside=(ptot[1]*q[2]-ptot[2]*q[1])/pt;
        qlong=(ptot[0]*q[3]-ptot[3]*q[0])/Mlong;
        qout=(roots/Mlong)*(ptot[1]*q[1]+ptot[2]*q[2])/pt;
        qinv=sqrt(qinv);
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

