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
using CAP::ParticlePairHistos;

ClassImp(ParticlePairHistos);

ParticlePairHistos::ParticlePairHistos(Task * _parent,
                                       const String & _name,
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
  appendClassName("ParticlePairHistos");
}

void ParticlePairHistos::createHistograms()
{
  if ( reportStart(__FUNCTION__))
    { }

  const String & bn  = getName();
  const String & ptn = getParentName();
  const String & ppn = getParentPathName();
  const Configuration & configuration = getConfiguration();

  nBins_n2  = configuration.getValueInt(ppn,   "nBins_n2");
  min_n2    = configuration.getValueDouble(ppn,"Min_n2");
  max_n2    = configuration.getValueDouble(ppn,"Max_n2");
  
  nBins_n2  = configuration.getValueInt(ppn,   "nBins_n2");
  min_n2    = configuration.getValueDouble(ppn,"Min_n2");
  max_n2    = configuration.getValueDouble(ppn,"Max_n2");

  nBins_pt = configuration.getValueInt(ppn,   "nBins_pt");
  min_pt   = configuration.getValueDouble(ppn,"Min_pt");
  max_pt   = configuration.getValueDouble(ppn,"Max_pt");
  range_pt = max_pt - min_pt;
  scale_pt = double(nBins_pt)/range_pt;

  nBins_phi   = configuration.getValueInt(ppn,   "nBins_phi");
  min_phi     = configuration.getValueDouble(ppn,"Min_phi");
  max_phi     = configuration.getValueDouble(ppn,"Max_phi");
  range_phi   = max_phi - min_phi;
  scale_phi   = double(nBins_phi)/range_phi;
  width_Dphi  = range_phi/double(nBins_phi);

  nBins_Dphi       = nBins_phi;
  min_Dphi         = 0.0; //-width_Dphi/2.;
  max_Dphi         = TMath::TwoPi(); // - width_Dphi/2.;
  nBins_Dphi_shft  = int(double(nBins_Dphi)/4.0);
  min_Dphi_shft    = min_Dphi - width_Dphi*double(nBins_Dphi_shft);
  max_Dphi_shft    = max_Dphi - width_Dphi*double(nBins_Dphi_shft);

  nBins_eta = configuration.getValueInt(ppn,   "nBins_eta");
  min_eta   = configuration.getValueDouble(ppn,"Min_eta");
  max_eta   = configuration.getValueDouble(ppn,"Max_eta");
  range_eta = max_eta - min_eta;
  scale_eta = double(nBins_eta)/range_eta;

  nBins_Deta= 2*nBins_eta-1;
  min_Deta  = -range_eta;
  max_Deta  = range_eta;

  nBins_y = configuration.getValueInt(ppn,   "nBins_y");
  min_y   = configuration.getValueDouble(ppn,"Min_y");
  max_y   = configuration.getValueDouble(ppn,"Max_y");
  range_y = max_y - min_y;
  scale_y = double(nBins_y)/range_y;

  //  nBins_DeltaP  = configuration.getValueInt(ppn,"nBins_DeltaP");
  //  min_DeltaP    = configuration.getValueDouble(ppn,"Min_DeltaP");
  //  max_DeltaP    = configuration.getValueDouble(ppn,"Max_DeltaP");

  nBins_Dy  = 2*nBins_y-1;
  min_Dy    = -range_y;
  max_Dy    = range_y;

  fillEta    = configuration.getValueBool(ppn,"FillEta");
  fillY      = configuration.getValueBool(ppn,"FillY");
  fillP2     = configuration.getValueBool(ppn,"FillP2");
  //fill3D     = configuration.getValueBool(ppn,"Fill3D");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  Pair:Parent Task Name....................: " << ptn << endl;
    cout << "  Pair:Parent Path Name....................: " << ppn << endl;
    cout << "  Pair:Histo Base Name.....................: " << bn << endl;
    cout << "  Pair:FillEta.............................: " << fillEta     << endl;
    cout << "  Pair:FillY...............................: " << fillY       << endl;
    cout << "  Pair:FillP2..............................: " << fillP2      << endl;
    cout << "  Pair:nBins_n2............................: " << nBins_n2    << endl;
    cout << "  Pair:Min_n2..............................: " << min_n2      << endl;
    cout << "  Pair:Max_n2..............................: " << max_n2      << endl;
    cout << "  Pair:nBins_pt............................: " << nBins_pt      << endl;
    cout << "  Pair:Min_pt..............................: " << min_pt        << endl;
    cout << "  Pair:Max_pt..............................: " << max_pt        << endl;
    cout << "  Pair:range_pt............................: " << range_pt      << endl;
    cout << "  Pair:scale_pt............................: " << scale_pt      << endl;
    cout << "  Pair:nBins_phi...........................: " << nBins_phi    << endl;
    cout << "  Pair:Min_phi.............................: " << min_phi      << endl;
    cout << "  Pair:Max_phi.............................: " << max_phi      << endl;
    cout << "  Pair:range_phi...........................: " << range_phi    << endl;
    cout << "  Pair:scale_phi...........................: " << scale_phi    << endl;
    cout << "  Pair:width_Dphi..........................: " << width_Dphi   << endl;
    cout << "  Pair:nBins_Dphi..........................: " << nBins_Dphi   << endl;
    cout << "  Pair:min_Dphi............................: " << min_Dphi     << endl;
    cout << "  Pair:max_Dphi............................: " << max_Dphi     << endl;
    cout << "  Pair:nBins_Dphi_shft.....................: " << nBins_Dphi_shft << endl;
    cout << "  Pair:min_Dphi_shft.......................: " << min_Dphi_shft   << endl;
    cout << "  Pair:max_Dphi_shft.......................: " << max_Dphi_shft   << endl;
    cout << "  Pair:nBins_eta...........................: " << nBins_eta    << endl;
    cout << "  Pair:Min_eta.............................: " << min_eta      << endl;
    cout << "  Pair:Max_eta.............................: " << max_eta      << endl;
    cout << "  Pair:range_eta...........................: " << range_eta    << endl;
    cout << "  Pair:scale_eta...........................: " << scale_eta    << endl;
    cout << "  Pair:nBins_Deta..........................: " << nBins_Deta   << endl;
    cout << "  Pair:min_Deta............................: " << min_Deta     << endl;
    cout << "  Pair:max_Deta............................: " << max_Deta     << endl;
    cout << "  Pair:nBins_y.............................: " << nBins_y      << endl;
    cout << "  Pair:Min_y...............................: " << min_y        << endl;
    cout << "  Pair:Max_y...............................: " << max_y        << endl;
    cout << "  Pair:range_y.............................: " << range_y      << endl;
    cout << "  Pair:scale_y.............................: " << scale_y      << endl;
    cout << "  Pair:nBins_Dy............................: " << nBins_Dy     << endl;
    cout << "  Pair:Min_Dy..............................: " << min_Dy       << endl;
    cout << "  Pair:Max_Dy..............................: " << max_Dy       << endl;
//    cout << "  Pair:nBins_DeltaP........................: " << standaloneMode      << endl;
//    cout << "  Pair:Min_DeltaP..........................: " << standaloneMode      << endl;
//    cout << "  Pair:Max_DeltaP..........................: " << standaloneMode      << endl;
    }

  h_n2          = createHistogram(createName(bn,"n2"),         nBins_n2,  min_n2,  max_n2, "n_{2}", "Yield");
  h_n2_ptpt     = createHistogram(createName(bn,"n2_ptpt"),    nBins_pt,  min_pt,  max_pt, nBins_pt, min_pt, max_pt,   "p_{T,1}",  "p_{T,2}", "N_{2}");
  h_n2_phiPhi   = createHistogram(createName(bn,"n2_phiPhi"),  nBins_phi, min_phi, max_phi, nBins_phi, min_phi, max_phi, "#varphi_{1}", "#varphi_{2}", "N_{2}");

  if (fillP2)
    {
    h_DptDpt_phiPhi = createHistogram(createName(bn,"ptpt_phiPhi"),   nBins_phi, min_phi, max_phi, nBins_phi, min_phi, max_phi, "#varphi_{1}", "#varphi_{2}", "p_{T}xp_{T}");
    }

  if (fillEta)
    {
    h_n2_etaEta   = createHistogram(createName(bn,"n2_etaEta"),   nBins_eta,  min_eta, max_eta, nBins_eta, min_eta, max_eta, "#eta_{1}", "#eta_{2}", "N_{2}");
    h_n2_DetaDphi = createHistogram(createName(bn,"n2_DetaDphi"), nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta", "#Delta#phi", "N_{2}");
    if (fillP2)
      {
      h_DptDpt_etaEta = createHistogram(createName(bn,"ptpt_etaEta"), nBins_eta, min_eta, max_eta, nBins_eta, min_eta, max_eta, "#eta_{1}", "#eta_{2}", "p_{T}xp_{T}");
      h_DptDpt_DetaDphi = createHistogram(createName(bn,"ptpt_DetaDphi"),nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta", "#Delta#phi", "ptpt");
      }
    }

  if (fillY)
    {
    h_n2_yY     = createHistogram(createName(bn,"n2_yY"),     nBins_y,  min_y,  max_y,  nBins_y, min_y, max_y, "y_{1}","y_{2}", "N_{2}");
    h_n2_DyDphi = createHistogram(createName(bn,"n2_DyDphi"), nBins_Dy, min_Dy, max_Dy, nBins_Dphi, min_Dphi, max_Dphi, "#Delta y", "#Delta#phi", "N_{2}");
    if (fillP2)
      {
      h_DptDpt_yY    = createHistogram(createName(bn,"ptpt_yY"),  nBins_y,  min_y, max_y, nBins_y, min_y, max_y, "y_{1}","y_{2}", "p_{T}xp_{T}");
      h_DptDpt_DyDphi = createHistogram(createName(bn,"ptpt_DyDphi"),nBins_Dy, min_Dy, max_Dy, nBins_Dphi, min_Dphi, max_Dphi, "#Delta y", "#Delta#phi", "ptpt");
      }
    }

  //  if (fill3D)
  //    {
  //    h_n2_DeltaP    = createHistogram(createName(bn,"n2_DeltaP"),
  //                                     nBins_DeltaP,  min_DeltaP, max_DeltaP,
  //                                     nBins_DeltaP,  min_DeltaP, max_DeltaP,
  //                                     nBins_DeltaP,  min_DeltaP, max_DeltaP,
  //                                     "p_{s}","p_{o}", "p_{l}","n_{2}");
  //    }

  if ( reportEnd(__FUNCTION__))
    { }
}

//________________________________________________________________________
void ParticlePairHistos::importHistograms(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
   const String & bn  = getName();
  const String & ptn = getParentName();
  const String & ppn = getParentPathName();
  fillEta       = configuration.getValueBool(ppn,"FillEta");
  fillY         = configuration.getValueBool(ppn,"FillY");
  fillP2        = configuration.getValueBool(ppn,"FillP2");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  Pair:Parent Task Name....................: " << ptn << endl;
    cout << "  Pair:Parent Path Name....................: " << ppn << endl;
    cout << "  Pair:Histo Base Name.....................: " << bn << endl;
    cout << "  Pair:FillEta.............................: " << fillEta     << endl;
    cout << "  Pair:FillY...............................: " << fillY       << endl;
    cout << "  Pair:FillP2..............................: " << fillP2      << endl;
    }
  h_n2          = loadH1(inputFile, CAP::createName(bn,"n2"));
  h_n2_ptpt     = loadH2(inputFile, CAP::createName(bn,"n2_ptpt"));
  h_n2_phiPhi   = loadH2(inputFile, CAP::createName(bn,"n2_phiPhi"));

  if (fillP2)
    {
    h_DptDpt_phiPhi = loadH2(inputFile, CAP::createName(bn,"ptpt_phiPhi"));
    }

  if (fillEta)
    {
    h_n2_etaEta   = loadH2(inputFile, CAP::createName(bn,"n2_etaEta"));
    h_n2_DetaDphi = loadH2(inputFile, CAP::createName(bn,"n2_DetaDphi"));
    if (fillP2)
      {
      h_DptDpt_etaEta   = loadH2(inputFile, CAP::createName(bn,"ptpt_etaEta"));
      h_DptDpt_DetaDphi = loadH2(inputFile, CAP::createName(bn,"ptpt_DetaDphi"));
      }
    }
  if (fillY)
    {
    h_n2_yY     = loadH2(inputFile, CAP::createName(bn,"n2_yY"));
    h_n2_DyDphi = loadH2(inputFile, CAP::createName(bn,"n2_DyDphi"));
    if (fillP2)
      {
      h_DptDpt_yY     = loadH2(inputFile, CAP::createName(bn,"ptpt_yY"));
      h_DptDpt_DyDphi = loadH2(inputFile, CAP::createName(bn,"ptpt_DyDphi"));
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePairHistos::fill(vector<ParticleDigit*> & particle1, vector<ParticleDigit*> & particle2, bool same, double weight)
{
  double nPairs    = 0;
  double nPairsEta = 0;
  double nPairsY   = 0;
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

      nPairs++;
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
        nPairsEta++;
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
          nPairsEta++;
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
        nPairsY++;
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
          nPairsY++;
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

      //      if (fill3D)
      //        {
      //        //decompose(double *pa,double *pb,double &qlong,double &qout,double &qside,double &qinv)
      //
      //        // Method from Scott Pratt to do three dimensional qinv components
      //        double pt,s,Mlong,roots;
      //        double ptot[4],q[4];
      //        const int g[4]={1,-1,-1,-1};
      //        int alpha;
      //        qinv=0.0;
      //        s=0.0;
      //        for(alpha=0;alpha<4;alpha++)
      //          {
      //          ptot[alpha]=pa[alpha]+pb[alpha];
      //          s+=g[alpha]*ptot[alpha]*ptot[alpha];
      //          q[alpha]=pa[alpha]-pb[alpha];
      //          qinv-=g[alpha]*q[alpha]*q[alpha];
      //          }
      //        pt=sqrt(ptot[1]*ptot[1]+ptot[2]*ptot[2]);
      //        Mlong=sqrt(s+pt*pt);
      //        roots=sqrt(s);
      //
      //        qside=(ptot[1]*q[2]-ptot[2]*q[1])/pt;
      //        qlong=(ptot[0]*q[3]-ptot[3]*q[0])/Mlong;
      //        qout=(roots/Mlong)*(ptot[1]*q[1]+ptot[2]*q[2])/pt;
      //        qinv=sqrt(qinv);
      //        }
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
    h_n2_etaEta->SetEntries(h_n2_etaEta->GetEntries()+nPairsEta);
    h_n2_DetaDphi->SetEntries(h_n2_DetaDphi->GetEntries()+nPairsEta);
    if (fillP2)
      {
      h_DptDpt_etaEta->SetEntries(h_DptDpt_etaEta->GetEntries()+nPairsEta);
      h_DptDpt_DetaDphi->SetEntries(h_DptDpt_DetaDphi->GetEntries()+nPairsEta);
      }
    }

  if (fillY)
    {
    h_n2_yY->SetEntries(h_n2_yY->GetEntries()+nPairsY);
    h_n2_DyDphi->SetEntries(h_n2_DyDphi->GetEntries()+nPairsY);
    if (fillP2)
      {
      h_DptDpt_yY->SetEntries(h_DptDpt_yY->GetEntries()+nPairsY);
      h_DptDpt_DyDphi->SetEntries(h_DptDpt_DyDphi->GetEntries()+nPairsY);
      }
    }
  h_n2->Fill(double(nPairs),weight);
}

void ParticlePairHistos::fill(Particle & particle1, Particle & particle2, double weight)
{
//  if (reportInfo(__FUNCTION__))
//    ;
  LorentzVector & momentum1 = particle1.getMomentum();
  double pt1   = momentum1.Pt();
  //double e1    = momentum1.E();
  double phi1  = momentum1.Phi();
  double eta1  = momentum1.Eta();
  double y1    = momentum1.Rapidity();
  LorentzVector & momentum2 = particle2.getMomentum();
  double pt2   = momentum2.Pt();
  //double e2    = momentum2.E();
  double phi2  = momentum2.Phi();
  double eta2  = momentum2.Eta();
  double y2    = momentum2.Rapidity();
  if (phi1<0.0) phi1 += TMath::TwoPi();
  if (phi2<0.0) phi2 += TMath::TwoPi();
  double dphi = phi1 - phi2;
  if (dphi<0.0) dphi += TMath::TwoPi();
  else if (dphi>TMath::TwoPi()) dphi -= TMath::TwoPi();

  h_n2_ptpt->Fill(pt1,pt2);
  h_n2_phiPhi->Fill(phi1,phi2);
  //if (fillP2)h_DptDpt_phiPhi ->AddBinContent(iG,weight*dpt1*dpt2);

  if (fillEta)
    {
    double deta = eta1 - eta2;
    h_n2_etaEta   ->Fill(eta1,eta2);
    h_n2_DetaDphi ->Fill(deta,dphi);
    if (fillP2)
      {
      h_DptDpt_etaEta->Fill(eta1,eta2,weight*pt1*pt2);  // needs attention
      h_DptDpt_DetaDphi->Fill(deta,dphi,weight*pt1*pt2); // needs attention
      }
    }
  if (fillY)
    {
    double dy = y1 - y2;
    h_n2_yY     ->Fill(y1,y2);
    h_n2_DyDphi ->Fill(dy,dphi);
    if (fillP2)
      {
      h_DptDpt_yY->Fill(y1,y2,weight*pt1*pt2); // needs attention
      h_DptDpt_DyDphi->Fill(dy,dphi,weight*pt1*pt2); // needs attention
      }
    }
//  if (reportInfo(__FUNCTION__))
//    ;
}

