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
#include "ParticlePairCombinedHistos.hpp"
ClassImp(ParticlePairCombinedHistos)


ParticlePairCombinedHistos::ParticlePairCombinedHistos(const TString       & _name,
                                                       const Configuration & _configuration,
                                                       LogLevel  _debugLevel)
:
Histograms(_name,_configuration,_debugLevel)
{
  appendClassName("ParticlePairCombinedHistos");
  setInstanceName(name);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// DTOR
//////////////////////////////////////////////////////////////////////////////////////////////////////
ParticlePairCombinedHistos::~ParticlePairCombinedHistos()
{
  if (reportDebug(__FUNCTION__)) cout << "ParticlePairCombinedHistos::DTOR() No ops." << endl;
}

void ParticlePairCombinedHistos::createHistograms()
{
  if (reportDebug(__FUNCTION__)) cout << "ParticlePairCombinedHistos::initialize() started" << endl;
  TString name;
  TString bn = getHistoBaseName();
  Configuration & configuration = getConfiguration();
  int    nBins_n2  = configuration.getValueInt("nBins_n2");
  double min_n2    = configuration.getValueDouble("min_n2");
  double max_n2    = configuration.getValueDouble("max_n2");
  int    nBins_pt  = configuration.getValueInt("nBins_pt");
  double min_pt    = configuration.getValueDouble("min_pt");
  double max_pt    = configuration.getValueDouble("max_pt");
  int    nBins_phi = configuration.getValueInt("nBins_phi");
  double min_phi   = configuration.getValueDouble("min_phi");
  double max_phi   = configuration.getValueDouble("max_phi");
  int    nBins_eta = configuration.getValueInt("nBins_eta");
  double min_eta   = configuration.getValueDouble("min_eta");
  double max_eta   = configuration.getValueDouble("max_eta");
  int    nBins_y   = configuration.getValueInt("nBins_y");
  double min_y     = configuration.getValueDouble("min_y");
  double max_y     = configuration.getValueDouble("max_y");
   
  bool  fillEta         = configuration.getValueDouble("fillEta");
  bool  fillY           = configuration.getValueDouble("fillY");
  bool  fillP2          = configuration.getValueDouble("fillP2");


  int   nBins_Dphi        = nBins_phi;
  float width_Dphi        = TMath::TwoPi()/nBins_Dphi;
  int   nBins_Dphi_shft   = int(double(nBins_Dphi)/4.0);
  double min_Dphi         = -width_Dphi/2.;
  double max_Dphi         = TMath::TwoPi() - width_Dphi/2.;

  double min_Dphi_shft    = min_Dphi - width_Dphi*double(nBins_Dphi/4);
  double max_Dphi_shft    = max_Dphi - width_Dphi*double(nBins_Dphi/4);

  int    nBins_Deta       = 2*nBins_eta - 1;
  double min_Deta         = min_eta - max_eta;
  double max_Deta         = max_eta - min_eta;

  int    nBins_Dy         = 2*nBins_y - 1;
  double min_Dy           = min_y - max_y;
  double max_Dy           = max_y - min_y;

  h_R2_ptpt    = createHistogram(makeName(bn,"R2_ptpt"),nBins_pt, min_pt, max_pt, nBins_pt, min_pt, max_pt, "p_{T,1}","p_{T,2}","R_{2}");

  if (fillEta)
    {
    h_R2_etaEta = createHistogram(makeName(bn,"R2_etaEta"),nBins_eta, min_eta,max_eta,nBins_eta,min_eta,max_eta,"#eta_{1}", "#eta_{2}","R_{2}");
    h_R2_phiPhi = createHistogram(makeName(bn,"R2_phiPhi"),nBins_phi, min_phi,max_phi,nBins_phi,min_phi,max_phi,"#varphi_{1}","#varphi_{2}","R_{2}");
    h_R2_DetaDphi_shft = createHistogram(makeName(bn,"R2_DetaDphi_shft"),nBins_Deta,min_Deta,max_Deta,nBins_Dphi,min_Dphi_shft,max_Dphi_shft,"#Delta#eta",  "#Delta#varphi", "R_{2}");

    if (fillP2)
      {
      h_DptDpt_etaEta        = createHistogram(makeName(bn,"DptDpt_etaEta"),        nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "<#Delta p_{T} #Delta p_{T}>");
      h_DptDpt_phiPhi        = createHistogram(makeName(bn,"DptDpt_phiPhi"),        nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "<#Delta p_{T} #Delta p_{T}>");
      h_DptDpt_DetaDphi_shft = createHistogram(makeName(bn,"DptDpt_DetaDphi_shft"), nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>");
      }

    if (fillP2)
      {
      h_P2_etaEta            = createHistogram(makeName(bn,"P2_etaEta"),            nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "P_{2}");
      h_P2_phiPhi            = createHistogram(makeName(bn,"P2_phiPhi"),            nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "P_{2}");
      h_P2_DetaDphi_shft     = createHistogram(makeName(bn,"P2_DetaDphi_shft"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "P_{2}");
      }

    if (fillP2)
      {
      h_G2_etaEta            = createHistogram(makeName(bn,"G2_etaEta"),            nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "G_{2}");
      h_G2_phiPhi            = createHistogram(makeName(bn,"G2_phiPhi"),            nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "G_{2}");
      h_G2_DetaDphi_shft     = createHistogram(makeName(bn,"G2_DetaDphi_shft"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "G_{2}");
      }

//    if (fillP2)
//      {
//      h_BF_etaEta            = createHistogram(makeName(bn,"BF_etaEta"),            nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "BF_{2}");
//      h_BF_phiPhi            = createHistogram(makeName(bn,"BF_phiPhi"),            nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "BF_{2}");
//      h_BF_DetaDphi_shft     = createHistogram(makeName(bn,"BF_DetaDphi_shft"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "BF_{2}");
//      }
    }

  if (fillY)
    {
    h_R2_yY              = createHistogram(makeName(bn,"R2_yY"),                nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}", "R_{2}");
    h_R2_DyDphi_shft     = createHistogram(makeName(bn,"R2_DyDphi_shft"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "R_{2}");

    if (fillP2)
      {
      h_DptDpt_yY          = createHistogram(makeName(bn,"DptDpt_yY"),            nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}",         "<#Delta p_{T} #Delta p_{T}>");
      h_DptDpt_DyDphi_shft = createHistogram(makeName(bn,"DptDpt_DyDphi_shft"),   nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>");
      }
    if (fillP2)
      {
      h_P2_yY              = createHistogram(makeName(bn,"P2_yY"),                nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}",         "P_{2}");
      h_P2_DyDphi_shft     = createHistogram(makeName(bn,"P2_DyDphi_shft"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "P_{2}");
      }
    if (fillP2)
      {
      h_G2_yY              = createHistogram(makeName(bn,"G2_yY"),                nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}",         "G_{2}");
      h_G2_DyDphi_shft     = createHistogram(makeName(bn,"G2_DyDphi_shft"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "G_{2}");
      }
//    if (fillP2)
//      {
//      h_BF_yY              = createHistogram(makeName(bn,"BF_yY"),                nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}",         "BF_{2}");
//      h_BF_DyDphi_shft     = createHistogram(makeName(bn,"BF_DyDphi_shft"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "BF_{2}");
//      }
    }

  if (reportDebug(__FUNCTION__)) cout << "ParticlePairCombinedHistos::initialize() completed" << endl;
}


void ParticlePairCombinedHistos::loadHistograms(TFile * inputFile)
{
  TString fct = "loadHistograms(TFile * inputFile)";
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(fct,inputFile))  return;
}

void ParticlePairCombinedHistos::calculate(ParticlePairDerivedHistos * pp,
                                           ParticlePairDerivedHistos * pm,
                                           ParticlePairDerivedHistos * mp,
                                           ParticlePairDerivedHistos * mm,
                                           double app, double apm, double amp, double amm)
{
  TString fct = "calculate(ParticlePairDerivedHistos * pp, ..)";
  if (reportStart(__FUNCTION__))
    ;

  Configuration configuration = getConfiguration();
  
  if (configuration.getValueBool("fillEta"))
    {
    if (!sameDimensions(fct,pp->h_R2_etaEta, mm->h_R2_etaEta,pm->h_R2_etaEta, h_R2_etaEta)) return;
    h_R2_etaEta->Add(pp->h_R2_etaEta, mm->h_R2_etaEta, app, amm);
    h_R2_etaEta->Add(pm->h_R2_etaEta, apm);
    h_R2_etaEta->Add(mp->h_R2_etaEta, amp);

    if (!sameDimensions(fct,pp->h_R2_phiPhi, mm->h_R2_phiPhi,pm->h_R2_phiPhi, h_R2_phiPhi)) return;
    h_R2_phiPhi->Add(pp->h_R2_phiPhi, mm->h_R2_phiPhi, app, amm);
    h_R2_phiPhi->Add(pm->h_R2_phiPhi, apm);
    h_R2_phiPhi->Add(mp->h_R2_phiPhi, amp);

    h_R2_ptpt->Add(pp->h_R2_ptpt, mm->h_R2_ptpt, app, amm);
    h_R2_ptpt->Add(pm->h_R2_ptpt, apm);
    h_R2_ptpt->Add(mp->h_R2_ptpt, amp);

    if (!sameDimensions(fct,pp->h_R2_DetaDphi_shft, mm->h_R2_DetaDphi_shft,pm->h_R2_DetaDphi_shft, h_R2_DetaDphi_shft)) return;

    h_R2_DetaDphi_shft->Add(pp->h_R2_DetaDphi_shft, mm->h_R2_DetaDphi_shft, app, amm);
    h_R2_DetaDphi_shft->Add(pm->h_R2_DetaDphi_shft, apm);
    h_R2_DetaDphi_shft->Add(mp->h_R2_DetaDphi_shft, amp);

    if (configuration.getValueBool("fillP2") )
      {
      if (!sameDimensions(fct,pp->h_DptDpt_etaEta,  mm->h_DptDpt_etaEta,pm->h_DptDpt_etaEta, h_DptDpt_etaEta)) return;
      h_DptDpt_etaEta->Add(pp->h_DptDpt_etaEta, mm->h_DptDpt_etaEta, app, amm);
      h_DptDpt_etaEta->Add(pm->h_DptDpt_etaEta, apm);
      h_DptDpt_etaEta->Add(mp->h_DptDpt_etaEta, amp);
      h_DptDpt_phiPhi->Add(pp->h_DptDpt_phiPhi, mm->h_DptDpt_phiPhi, app, amm);
      h_DptDpt_phiPhi->Add(pm->h_DptDpt_phiPhi, apm);
      h_DptDpt_phiPhi->Add(mp->h_DptDpt_phiPhi, amp);
      h_DptDpt_DetaDphi_shft->Add(pp->h_DptDpt_DetaDphi_shft, mm->h_DptDpt_DetaDphi_shft, app, amm);
      h_DptDpt_DetaDphi_shft->Add(pm->h_DptDpt_DetaDphi_shft, apm);
      h_DptDpt_DetaDphi_shft->Add(mp->h_DptDpt_DetaDphi_shft, amp);
 
      h_P2_etaEta->Add(pp->h_P2_etaEta, mm->h_P2_etaEta, app, amm);
      h_P2_etaEta->Add(pm->h_P2_etaEta, apm);
      h_P2_etaEta->Add(mp->h_P2_etaEta, amp);
      h_P2_phiPhi->Add(pp->h_P2_phiPhi, mm->h_P2_phiPhi, app, amm);
      h_P2_phiPhi->Add(pm->h_P2_phiPhi, apm);
      h_P2_phiPhi->Add(mp->h_P2_phiPhi, amp);
      h_P2_DetaDphi_shft->Add(pp->h_P2_DetaDphi_shft, mm->h_P2_DetaDphi_shft, app, amm);
      h_P2_DetaDphi_shft->Add(pm->h_P2_DetaDphi_shft, apm);
      h_P2_DetaDphi_shft->Add(mp->h_P2_DetaDphi_shft, amp);
      
      h_G2_etaEta->Add(pp->h_G2_etaEta, mm->h_G2_etaEta, app, amm);
      h_G2_etaEta->Add(pm->h_G2_etaEta, apm);
      h_G2_etaEta->Add(mp->h_G2_etaEta, amp);
      h_G2_phiPhi->Add(pp->h_G2_phiPhi, mm->h_G2_phiPhi, app, amm);
      h_G2_phiPhi->Add(pm->h_G2_phiPhi, apm);
      h_G2_phiPhi->Add(mp->h_G2_phiPhi, amp);
      h_G2_DetaDphi_shft->Add(pp->h_G2_DetaDphi_shft, mm->h_G2_DetaDphi_shft, app, amm);
      h_G2_DetaDphi_shft->Add(pm->h_G2_DetaDphi_shft, apm);
      h_G2_DetaDphi_shft->Add(mp->h_G2_DetaDphi_shft, amp);
      }

//    if (fillP2)
//      {
//      if (!sameDimensions(pp->h_bf12_DetaDphi_shft, mm->h_bf12_DetaDphi_shft,pm->h_bf12_DetaDphi_shft, pm->h_bf21_DetaDphi_shft, h_BF_DetaDphi_shft)) return;
//      h_BF_etaEta->Add(pp->h_bf12_etaEta, mm->h_bf12_etaEta, app, amm);
//      h_BF_etaEta->Add(pm->h_bf12_etaEta, apm);
//      h_BF_etaEta->Add(pm->h_bf21_etaEta, amp);
//      h_BF_phiPhi->Add(pp->h_bf12_phiPhi, mm->h_bf12_phiPhi, app, amm);
//      h_BF_phiPhi->Add(pm->h_bf12_phiPhi, apm);
//      h_BF_phiPhi->Add(pm->h_bf21_phiPhi, amp);
//      h_BF_DetaDphi_shft->Add(pp->h_bf12_DetaDphi_shft, mm->h_bf12_DetaDphi_shft, app, amm);
//      h_BF_DetaDphi_shft->Add(pm->h_bf12_DetaDphi_shft, apm);
//      h_BF_DetaDphi_shft->Add(pm->h_bf21_DetaDphi_shft, amp);
//      }
   }

  if (configuration.getValueBool("fillY") )
    {
    h_R2_yY->Add(pp->h_R2_yY, mm->h_R2_yY, app, amm);
    h_R2_yY->Add(pm->h_R2_yY, apm);
    h_R2_yY->Add(mp->h_R2_yY, amp);
    h_R2_DyDphi_shft->Add(pp->h_R2_DyDphi_shft, mm->h_R2_DyDphi_shft, app, amm);
    h_R2_DyDphi_shft->Add(pm->h_R2_DyDphi_shft, apm);
    h_R2_DyDphi_shft->Add(mp->h_R2_DyDphi_shft, amp);

    if (configuration.getValueBool("fillP2") )
      {
      h_DptDpt_yY->Add(pp->h_DptDpt_yY, mm->h_DptDpt_yY, app, amm);
      h_DptDpt_yY->Add(pm->h_DptDpt_yY, apm);
      h_DptDpt_yY->Add(mp->h_DptDpt_yY, amp);
      h_DptDpt_DyDphi_shft->Add(pp->h_DptDpt_DyDphi_shft, mm->h_DptDpt_DyDphi_shft, app, amm);
      h_DptDpt_DyDphi_shft->Add(pm->h_DptDpt_DyDphi_shft, apm);
      h_DptDpt_DyDphi_shft->Add(mp->h_DptDpt_DyDphi_shft, amp);
    
      h_P2_yY->Add(pp->h_P2_yY, mm->h_P2_yY, app, amm);
      h_P2_yY->Add(pm->h_P2_yY, apm);
      h_P2_yY->Add(mp->h_P2_yY, amp);
      h_P2_DyDphi_shft->Add(pp->h_P2_DyDphi_shft, mm->h_P2_DyDphi_shft, app, amm);
      h_P2_DyDphi_shft->Add(pm->h_P2_DyDphi_shft, apm);
      h_P2_DyDphi_shft->Add(mp->h_P2_DyDphi_shft, amp);
     
      h_G2_yY->Add(pp->h_G2_yY, mm->h_G2_yY, app, amm);
      h_G2_yY->Add(pm->h_G2_yY, apm);
      h_G2_yY->Add(mp->h_G2_yY, amp);
      h_G2_DyDphi_shft->Add(pp->h_G2_DyDphi_shft, mm->h_G2_DyDphi_shft, app, amm);
      h_G2_DyDphi_shft->Add(pm->h_G2_DyDphi_shft, apm);
      h_G2_DyDphi_shft->Add(mp->h_G2_DyDphi_shft, amp);
      }

//    if (fillP2)
//      {
//      h_BF_yY->Add(pp->h_bf12_yY, mm->h_bf12_yY, app, amm);
//      h_BF_yY->Add(pm->h_bf12_yY, apm);
//      h_BF_yY->Add(pm->h_bf21_yY, amp);
//      h_BF_DyDphi_shft->Add(pp->h_bf12_DyDphi_shft, mm->h_bf12_DyDphi_shft, app, amm);
//      h_BF_DyDphi_shft->Add(pm->h_bf12_DyDphi_shft, apm);
//      h_BF_DyDphi_shft->Add(pm->h_bf21_DyDphi_shft, amp);
//      }
    }
  if (reportEnd("ParticlePairCombinedHistos",getName(),"calculate(..)"))
    ;

}
