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
#include "ParticlePairDerivedHistos.hpp"
ClassImp(ParticlePairDerivedHistos);

ParticlePairDerivedHistos::ParticlePairDerivedHistos(Task * _parent,
                                                     const TString & _name,
                                                     Configuration & _configuration)
:
Histograms(_parent,_name,_configuration),
fillEta(0),
fillY(0),
fillP2(0),
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
nBins_Deta(0),
min_Deta(0),
max_Deta(0),
nBins_Dy(0),
min_Dy(0),
max_Dy(0),
h_n1n1_ptpt(nullptr),
h_A2_ptpt(nullptr),
h_B2_ptpt(nullptr),
h_C2_ptpt(nullptr),
h_R2_ptpt(nullptr),

h_n1n1_phiPhi(nullptr),
h_A2_phiPhi(nullptr),
h_B2_phiPhi(nullptr),
h_C2_phiPhi(nullptr),
h_R2_phiPhi(nullptr),

h_pt1pt1_phiPhi(nullptr),
h_DptDpt_phiPhi(nullptr),
h_P2_phiPhi(nullptr),
h_G2_phiPhi(nullptr),

h_n1n1_etaEta(nullptr),
h_A2_etaEta(nullptr),
h_B2_etaEta(nullptr),
h_C2_etaEta(nullptr),
h_R2_etaEta(nullptr),

h_pt1pt1_etaEta(nullptr),
h_DptDpt_etaEta(nullptr),
h_G2_etaEta(nullptr),
h_P2_etaEta(nullptr),

h_n1n1_DetaDphi(nullptr),
h_rho2_DetaDphi(nullptr),
h_A2_DetaDphi(nullptr),
h_B2_DetaDphi(nullptr),
h_C2_DetaDphi(nullptr),
h_R2_DetaDphi(nullptr),
h_P2_DetaDphi(nullptr),
h_G2_DetaDphi(nullptr),

h_pt1pt1_DetaDphi(nullptr),
h_DptDpt_DetaDphi(nullptr),

h_rho2_DetaDphi_shft(nullptr),
h_A2_DetaDphi_shft(nullptr),
h_B2_DetaDphi_shft(nullptr),
h_C2_DetaDphi_shft(nullptr),
h_R2_DetaDphi_shft(nullptr),
h_DptDpt_DetaDphi_shft(nullptr),
h_P2_DetaDphi_shft(nullptr),
h_G2_DetaDphi_shft(nullptr),

h_n1n1_yY(nullptr),
h_A2_yY(nullptr),
h_B2_yY(nullptr),
h_C2_yY(nullptr),
h_R2_yY(nullptr),

h_pt1pt1_yY(nullptr),
h_DptDpt_yY(nullptr),
h_G2_yY(nullptr),
h_P2_yY(nullptr),

h_n1n1_DyDphi(nullptr),
h_rho2_DyDphi(nullptr),
h_A2_DyDphi(nullptr),
h_B2_DyDphi(nullptr),
h_C2_DyDphi(nullptr),
h_R2_DyDphi(nullptr),
h_P2_DyDphi(nullptr),
h_G2_DyDphi(nullptr),

h_rho2_DyDphi_shft(nullptr),
h_A2_DyDphi_shft(nullptr),
h_B2_DyDphi_shft(nullptr),
h_C2_DyDphi_shft(nullptr),
h_R2_DyDphi_shft(nullptr),
h_DptDpt_DyDphi_shft(nullptr),
h_P2_DyDphi_shft(nullptr),
h_G2_DyDphi_shft(nullptr),

h_pt1pt1_DyDphi(nullptr),
h_DptDpt_DyDphi(nullptr)
{
  appendClassName("ParticlePairDerivedHistos");
}

void ParticlePairDerivedHistos::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  const TString & bn  = getName();
  const TString & ptn = getParentTaskName();
  const TString & ppn = getParentPathName();
  Configuration & configuration = getConfiguration();
  nBins_n2   = configuration.getValueInt(ppn,"nBins_n2");
  min_n2     = configuration.getValueDouble(ppn,"Min_n2");
  max_n2     = configuration.getValueDouble(ppn,"Max_n2");
  nBins_n2   = configuration.getValueInt(ppn,"nBins_n2");
  min_n2     = configuration.getValueDouble(ppn,"Min_n2");
  max_n2     = configuration.getValueDouble(ppn,"Max_n2");
  nBins_pt   = configuration.getValueInt(ppn,"nBins_pt");
  min_pt     = configuration.getValueDouble(ppn,"Min_pt");
  max_pt     = configuration.getValueDouble(ppn,"Max_pt");
  nBins_phi  = configuration.getValueInt(ppn,"nBins_phi");
  min_phi    = configuration.getValueDouble(ppn,"Min_phi");
  max_phi    = configuration.getValueDouble(ppn,"Max_phi");
  fillEta    = configuration.getValueBool(ppn,"FillEta");
  fillY      = configuration.getValueBool(ppn,"FillY");
  fillP2     = configuration.getValueBool(ppn,"FillP2");

  double scale_phi   = max_phi - min_phi;
  double width_Dphi  = scale_phi/nBins_phi;

  nBins_Dphi       = nBins_phi;
  min_Dphi         = 0.0; //-width_Dphi/2.;
  max_Dphi         = TMath::TwoPi(); // - width_Dphi/2.;
  nBins_Dphi_shft  = int(double(nBins_Dphi)/4.0);
  min_Dphi_shft    = min_Dphi - width_Dphi*double(nBins_Dphi_shft);
  max_Dphi_shft    = max_Dphi - width_Dphi*double(nBins_Dphi_shft);

  nBins_eta = configuration.getValueInt(ppn,"nBins_eta");
  min_eta   = configuration.getValueDouble(ppn,"Min_eta");
  max_eta   = configuration.getValueDouble(ppn,"Max_eta");
  double range_eta = max_eta - min_eta;

  nBins_Deta= 2*nBins_eta-1;
  min_Deta  = -range_eta;
  max_Deta  = range_eta;

  nBins_y = configuration.getValueInt(ppn,"nBins_y");
  min_y   = configuration.getValueDouble(ppn,"Min_y");
  max_y   = configuration.getValueDouble(ppn,"Max_y");
  double range_y = max_y - min_y;

  nBins_Dy  = 2*nBins_y-1;
  min_Dy    = -range_y;
  max_Dy    = range_y;

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  Pair:Parent Task Name....................: " << ptn << endl;
    cout << "  Pair:Parent Path Name....................: " << ppn << endl;
    cout << "  Pair:Histo Base Name.....................: " << bn << endl;
    cout << "  Pair:nBins_n2............................: " << nBins_n2 << endl;
    cout << "  Pair:Min_n2..............................: " << min_n2 << endl;
    cout << "  Pair:Max_n2..............................: " << max_n2 << endl;
    cout << "  Pair:nBins_pt............................: " << nBins_pt << endl;
    cout << "  Pair:Min_pt..............................: " << min_pt << endl;
    cout << "  Pair:Max_pt..............................: " << max_pt << endl;
    cout << "  Pair:nBins_phi...........................: " << nBins_phi << endl;
    cout << "  Pair:Min_phi.............................: " << min_phi << endl;
    cout << "  Pair:Max_phi.............................: " << max_phi << endl;
    cout << "  Pair:scale_phi...........................: " << scale_phi << endl;

    cout << "  Pair:nBins_Dphi..........................: " << nBins_Dphi << endl;
    cout << "  Pair:Min_Dphi............................: " << min_Dphi << endl;
    cout << "  Pair:Max_Dphi............................: " << max_Dphi << endl;
    cout << "  Pair:width_Dphi..........................: " << width_Dphi << endl;
    cout << "  Pair:nBins_Dphi_shft.....................: " << nBins_Dphi_shft << endl;
    cout << "  Pair:min_Dphi_shft.......................: " << min_Dphi_shft << endl;
    cout << "  Pair:max_Dphi_shft.......................: " << max_Dphi_shft << endl;

    cout << "  Pair:nBins_eta...........................: " << nBins_eta << endl;
    cout << "  Pair:Min_eta.............................: " << min_eta << endl;
    cout << "  Pair:Max_eta.............................: " << max_eta << endl;
    cout << "  Pair:range_eta...........................: " << range_eta << endl;
    cout << "  Pair:nBins_Deta..........................: " << nBins_Deta << endl;
    cout << "  Pair:min_Deta............................: " << min_Deta << endl;
    cout << "  Pair:max_Deta............................: " << max_Deta << endl;

    cout << "  Pair:nBins_y.............................: " << nBins_y << endl;
    cout << "  Pair:Min_y...............................: " << min_y << endl;
    cout << "  Pair:Max_y...............................: " << max_y << endl;
    cout << "  Pair:range_y.............................: " << range_y << endl;
    cout << "  Pair:nBins_Dy............................: " << nBins_Dy << endl;
    cout << "  Pair:min_Dy..............................: " << min_Dy << endl;
    cout << "  Pair:max_Dy..............................: " << max_Dy << endl;

    cout << "  Pair:FillEta.............................: " << fillEta << endl;
    cout << "  Pair:FillY...............................: " << fillY << endl;
    cout << "  Pair:FillP2..............................: " << fillP2 << endl;
    }


  h_n1n1_ptpt            = createHistogram(makeName(bn,"n1n1_ptpt"),   nBins_pt,  min_pt,  max_pt,  nBins_pt,  min_pt,   max_pt,   "p_{T,1}", "p_{T,2}", "<n_{1}><n_{2}>");
  h_A2_ptpt              = createHistogram(makeName(bn,"A2_ptpt"),     nBins_pt,  min_pt,  max_pt,  nBins_pt,  min_pt,   max_pt,   "p_{T,1}", "p_{T,2}", "A_{2}");
  h_B2_ptpt              = createHistogram(makeName(bn,"B2_ptpt"),     nBins_pt,  min_pt,  max_pt,  nBins_pt,  min_pt,   max_pt,   "p_{T,1}", "p_{T,2}", "B_{2}");
  h_C2_ptpt              = createHistogram(makeName(bn,"C2_ptpt"),     nBins_pt,  min_pt,  max_pt,  nBins_pt,  min_pt,   max_pt,   "p_{T,1}", "p_{T,2}", "C_{2}");
  h_R2_ptpt              = createHistogram(makeName(bn,"R2_ptpt"),     nBins_pt,  min_pt,  max_pt,  nBins_pt,  min_pt,   max_pt,   "p_{T,1}", "p_{T,2}", "R_{2}");

  h_n1n1_phiPhi          = createHistogram(makeName(bn,"n1n1_phiPhi"), nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}", "<n_{1}><n_{2}>");
  h_A2_phiPhi            = createHistogram(makeName(bn,"A2_phiPhi"),   nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}", "A_{2}");
  h_B2_phiPhi            = createHistogram(makeName(bn,"B2_phiPhi"),   nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}", "B_{2}");
  h_C2_phiPhi            = createHistogram(makeName(bn,"C2_phiPhi"),   nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}", "C_{2}");
  h_R2_phiPhi            = createHistogram(makeName(bn,"R2_phiPhi"),   nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}", "R_{2}");

  if (fillP2)
    {
    h_pt1pt1_phiPhi      = createHistogram(makeName(bn,"pt1pt1_phiPhi"), nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}","#varphi_{2}",   "pt1pt1");
    h_DptDpt_phiPhi      = createHistogram(makeName(bn,"DptDpt_phiPhi"), nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}",   "<#Delta p_{T} #Delta p_{T}>");
    h_P2_phiPhi          = createHistogram(makeName(bn,"P2_phiPhi"),     nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}",   "P_{2}");
    h_G2_phiPhi          = createHistogram(makeName(bn,"G2_phiPhi"),     nBins_phi, min_phi, max_phi, nBins_phi, min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}",   "G_{2}");
    }
  
  if (fillEta)
    {
    h_n1n1_etaEta        = createHistogram(makeName(bn,"n1n1_etaEta"),    nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,  "#eta_{1}", "#eta_{2}", "<n_{1}><n_{2}>");
    h_A2_etaEta          = createHistogram(makeName(bn,"A2_etaEta"),      nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,  "#eta_{1}", "#eta_{2}", "A_{2}");
    h_B2_etaEta          = createHistogram(makeName(bn,"B2_etaEta"),      nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,  "#eta_{1}", "#eta_{2}", "B_{2}");
    h_C2_etaEta          = createHistogram(makeName(bn,"C2_etaEta"),      nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,  "#eta_{1}", "#eta_{2}", "C_{2}");
    h_R2_etaEta          = createHistogram(makeName(bn,"R2_etaEta"),      nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,  "#eta_{1}", "#eta_{2}", "R_{2}");

    h_n1n1_DetaDphi      = createHistogram(makeName(bn,"n1n1_DetaDphi"),  nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta","#Delta#varphi", "<n_{1}><n_{1}>");
    h_rho2_DetaDphi      = createHistogram(makeName(bn,"rho2_DetaDphi"),  nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta","#Delta#varphi", "#rho_{2}");
    h_A2_DetaDphi        = createHistogram(makeName(bn,"A2_DetaDphi"),    nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta","#Delta#varphi", "A_{2}");
    h_B2_DetaDphi        = createHistogram(makeName(bn,"B2_DetaDphi"),    nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta","#Delta#varphi", "B_{2}");
    h_C2_DetaDphi        = createHistogram(makeName(bn,"C2_DetaDphi"),    nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta","#Delta#varphi", "C_{2}");
    h_R2_DetaDphi        = createHistogram(makeName(bn,"R2_DetaDphi"),    nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta","#Delta#varphi", "R_{2}");
    
    h_rho2_DetaDphi_shft = createHistogram(makeName(bn,"rho2_DetaDphi_shft"),  nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta","#Delta#varphi", "#rho_{2}");
    h_A2_DetaDphi_shft   = createHistogram(makeName(bn,"A2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta","#Delta#varphi", "A_{2}");
    h_B2_DetaDphi_shft   = createHistogram(makeName(bn,"B2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta","#Delta#varphi", "B_{2}");
    h_C2_DetaDphi_shft   = createHistogram(makeName(bn,"C2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta","#Delta#varphi", "C_{2}");
    h_R2_DetaDphi_shft   = createHistogram(makeName(bn,"R2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta","#Delta#varphi", "R_{2}");

    if (fillP2)
      {
      h_pt1pt1_etaEta        = createHistogram(makeName(bn,"pt1pt1_etaEta"),  nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,            "#eta_{1}", "#eta_{2}",      "pt1pt1");
      h_DptDpt_etaEta        = createHistogram(makeName(bn,"DptDpt_etaEta"),  nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,            "#eta_{1}", "#eta_{2}",      "<#Delta p_{T} #Delta p_{T}>");
      h_P2_etaEta            = createHistogram(makeName(bn,"P2_etaEta"),      nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,            "#eta_{1}", "#eta_{2}",      "P_{2}");
      h_G2_etaEta            = createHistogram(makeName(bn,"G2_etaEta"),      nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,            "#eta_{1}", "#eta_{2}",         "G_{2}");
      
      h_pt1pt1_DetaDphi      = createHistogram(makeName(bn,"pt1pt1_DetaDphi"),nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta", "#Delta#varphi", "pt1pt1");
      h_DptDpt_DetaDphi      = createHistogram(makeName(bn,"DptDpt_DetaDphi"),nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>");
      h_P2_DetaDphi          = createHistogram(makeName(bn,"P2_DetaDphi"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta",  "#Delta#varphi", "P_{2}");
      h_G2_DetaDphi          = createHistogram(makeName(bn,"G2_DetaDphi"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta","#Delta#varphi",   "G_{2}");
      h_DptDpt_DetaDphi_shft = createHistogram(makeName(bn,"DptDpt_DetaDphi_shft"),nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>");
      h_P2_DetaDphi_shft     = createHistogram(makeName(bn,"P2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "P_{2}");
      h_G2_DetaDphi_shft     = createHistogram(makeName(bn,"G2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta", "#Delta#varphi",  "G_{2}");
      }
    }
  
  if (fillY)
    {
    h_n1n1_yY           = createHistogram(makeName(bn,"n1n1_yY"),      nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                 "y_{1}",     "y_{2}", "<n_{1}><n_{1}>");
    h_A2_yY             = createHistogram(makeName(bn,"A2_yY"),        nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                 "y_{1}",    "y_{2}", "A_{2}");
    h_B2_yY             = createHistogram(makeName(bn,"B2_yY"),        nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                 "y_{1}",    "y_{2}", "B_{2}");
    h_C2_yY             = createHistogram(makeName(bn,"C2_yY"),        nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                 "y_{1}",    "y_{2}", "C_{2}");
    h_R2_yY             = createHistogram(makeName(bn,"R2_yY"),        nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                 "y_{1}",    "y_{2}", "R_{2}");

    h_n1n1_DyDphi       = createHistogram(makeName(bn,"n1n1_DyDphi"),  nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,     "#Delta y",  "#Delta#varphi", "<n_{1}><n_{1}>");
    h_rho2_DyDphi       = createHistogram(makeName(bn,"rho2_DyDphi"),  nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,     "#Delta y",  "#Delta#varphi", "#rho_{2}>");
    h_A2_DyDphi         = createHistogram(makeName(bn,"A2_DyDphi"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,     "#Delta y",  "#Delta#varphi", "A_{2}");
    h_B2_DyDphi         = createHistogram(makeName(bn,"B2_DyDphi"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,     "#Delta y",  "#Delta#varphi", "B_{2}");
    h_C2_DyDphi         = createHistogram(makeName(bn,"C2_DyDphi"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,     "#Delta y",  "#Delta#varphi", "C_{2}");
    h_R2_DyDphi         = createHistogram(makeName(bn,"R2_DyDphi"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,     "#Delta y",  "#Delta#varphi", "R_{2}");

    h_rho2_DyDphi_shft  = createHistogram(makeName(bn,"rho2_DyDphi_shft"),  nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "#rho_{2}>");
    h_A2_DyDphi_shft    = createHistogram(makeName(bn,"A2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "A_{2}");
    h_B2_DyDphi_shft    = createHistogram(makeName(bn,"B2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "B_{2}");
    h_C2_DyDphi_shft    = createHistogram(makeName(bn,"C2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "C_{2}");
    h_R2_DyDphi_shft    = createHistogram(makeName(bn,"R2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "R_{2}");

    if (fillP2)
      {
      h_pt1pt1_yY         = createHistogram(makeName(bn,"pt1pt1_yY"),    nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                  "y_{1}",    "y_{2}", "pt1pt1");
      h_DptDpt_yY         = createHistogram(makeName(bn,"DptDpt_yY"),    nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                  "y_{1}",    "y_{2}", "<#Delta p_{T} #Delta p_{T}>");
      h_P2_yY             = createHistogram(makeName(bn,"P2_yY"),        nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",    "y_{2}",         "P_{2}");
      h_G2_yY             = createHistogram(makeName(bn,"G2_yY"),        nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                  "y_{1}",    "y_{2}", "G_{2}");
      
      h_pt1pt1_DyDphi     = createHistogram(makeName(bn,"pt1pt1_DyDphi"),nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "pt1pt1");
      h_DptDpt_DyDphi     = createHistogram(makeName(bn,"DptDpt_DyDphi"),nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>");
      h_P2_DyDphi         = createHistogram(makeName(bn,"P2_DyDphi"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "P_{2}");
      h_G2_DyDphi         = createHistogram(makeName(bn,"G2_DyDphi"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "G_{2}");
      
      h_DptDpt_DyDphi_shft= createHistogram(makeName(bn,"DptDpt_DyDphi_shft"),nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>");
      h_P2_DyDphi_shft    = createHistogram(makeName(bn,"P2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "P_{2}");
      h_G2_DyDphi_shft    = createHistogram(makeName(bn,"G2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "G_{2}");
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePairDerivedHistos::loadHistograms(TFile * inputFile)
{
  if (reportDebug(__FUNCTION__))
    ;
  if (ptrFileExist(__FUNCTION__,inputFile)) return;
  const TString & bn  = getName();
  const TString & ptn = getParentTaskName();
  const TString & ppn = getParentPathName();
  Configuration & configuration = getConfiguration();

  fillEta    = configuration.getValueBool(ppn,"FillEta");
  fillY      = configuration.getValueBool(ppn,"FillY");
  fillP2     = configuration.getValueBool(ppn,"FillP2");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  Pair:Parent Task Name....................: " << ptn << endl;
    cout << "  Pair:Parent Path Name....................: " << ppn << endl;
    cout << "  Pair:Histo Base Name.....................: " << bn << endl;
    cout << "  Pair:FillEta.............................: " << fillEta << endl;
    cout << "  Pair:FillY...............................: " << fillY << endl;
    cout << "  Pair:FillP2..............................: " << fillP2 << endl;
    }

  h_n1n1_phiPhi          = loadH2(inputFile, makeName(bn,"n1n1_phiPhi"));
  h_A2_phiPhi            = loadH2(inputFile, makeName(bn,"A2_phiPhi"));
  h_B2_phiPhi            = loadH2(inputFile, makeName(bn,"B2_phiPhi"));
  h_C2_phiPhi            = loadH2(inputFile, makeName(bn,"C2_phiPhi"));
  h_R2_phiPhi            = loadH2(inputFile, makeName(bn,"R2_phiPhi"));

  h_n1n1_ptpt            = loadH2(inputFile, makeName(bn,"n1n1_ptpt"));
  h_A2_ptpt              = loadH2(inputFile, makeName(bn,"A2_ptpt"));
  h_B2_ptpt              = loadH2(inputFile, makeName(bn,"B2_ptpt"));
  h_C2_ptpt              = loadH2(inputFile, makeName(bn,"C2_ptpt"));
  h_R2_ptpt              = loadH2(inputFile, makeName(bn,"R2_ptpt"));

  if (fillP2)
    {
    h_pt1pt1_phiPhi      = loadH2(inputFile, makeName(bn,"pt1pt1_phiPhi"));
    h_DptDpt_phiPhi      = loadH2(inputFile, makeName(bn,"DptDpt_phiPhi"));
    h_P2_phiPhi          = loadH2(inputFile, makeName(bn,"P2_phiPhi"));
    h_G2_phiPhi          = loadH2(inputFile, makeName(bn,"G2_phiPhi"));
    }

  if (fillEta)
    {
    h_n1n1_etaEta        = loadH2(inputFile, makeName(bn,"n1n1_etaEta"));
    h_A2_etaEta          = loadH2(inputFile, makeName(bn,"A2_etaEta"));
    h_B2_etaEta          = loadH2(inputFile, makeName(bn,"B2_etaEta"));
    h_C2_etaEta          = loadH2(inputFile, makeName(bn,"C2_etaEta"));
    h_R2_etaEta          = loadH2(inputFile, makeName(bn,"R2_etaEta"));

    h_n1n1_DetaDphi      = loadH2(inputFile, makeName(bn,"n1n1_DetaDphi"));
    h_rho2_DetaDphi      = loadH2(inputFile, makeName(bn,"rho2_DetaDphi"));
    h_A2_DetaDphi        = loadH2(inputFile, makeName(bn,"A2_DetaDphi"));
    h_B2_DetaDphi        = loadH2(inputFile, makeName(bn,"B2_DetaDphi"));
    h_C2_DetaDphi        = loadH2(inputFile, makeName(bn,"C2_DetaDphi"));
    h_R2_DetaDphi        = loadH2(inputFile, makeName(bn,"R2_DetaDphi"));

    h_rho2_DetaDphi_shft = loadH2(inputFile, makeName(bn,"rho2_DetaDphi_shft"));
    h_A2_DetaDphi_shft   = loadH2(inputFile, makeName(bn,"A2_DetaDphi_shft"));
    h_B2_DetaDphi_shft   = loadH2(inputFile, makeName(bn,"B2_DetaDphi_shft"));
    h_C2_DetaDphi_shft   = loadH2(inputFile, makeName(bn,"C2_DetaDphi_shft"));
    h_R2_DetaDphi_shft   = loadH2(inputFile, makeName(bn,"R2_DetaDphi_shft"));

    if (fillP2)
      {
      h_pt1pt1_etaEta        = loadH2(inputFile, makeName(bn,"pt1pt1_etaEta"));
      h_DptDpt_etaEta        = loadH2(inputFile, makeName(bn,"DptDpt_etaEta"));
      h_P2_etaEta            = loadH2(inputFile, makeName(bn,"P2_etaEta"));
      h_G2_etaEta            = loadH2(inputFile, makeName(bn,"G2_etaEta"));

      h_pt1pt1_DetaDphi      = loadH2(inputFile, makeName(bn,"pt1pt1_DetaDphi"));
      h_DptDpt_DetaDphi      = loadH2(inputFile, makeName(bn,"DptDpt_DetaDphi"));
      h_P2_DetaDphi          = loadH2(inputFile, makeName(bn,"P2_DetaDphi"));
      h_G2_DetaDphi          = loadH2(inputFile, makeName(bn,"G2_DetaDphi"));
      h_DptDpt_DetaDphi_shft = loadH2(inputFile, makeName(bn,"DptDpt_DetaDphi_shft"));
      h_P2_DetaDphi_shft     = loadH2(inputFile, makeName(bn,"P2_DetaDphi_shft"));
      h_G2_DetaDphi_shft     = loadH2(inputFile, makeName(bn,"G2_DetaDphi_shft"));
      }
    }

  if (fillY)
    {
    h_n1n1_yY           = loadH2(inputFile, makeName(bn,"n1n1_yY"));
    h_A2_yY             = loadH2(inputFile, makeName(bn,"A2_yY"));
    h_B2_yY             = loadH2(inputFile, makeName(bn,"B2_yY"));
    h_C2_yY             = loadH2(inputFile, makeName(bn,"C2_yY"));
    h_R2_yY             = loadH2(inputFile, makeName(bn,"R2_yY"));

    h_n1n1_DyDphi       = loadH2(inputFile, makeName(bn,"n1n1_DyDphi"));
    h_rho2_DyDphi       = loadH2(inputFile, makeName(bn,"rho2_DyDphi"));
    h_A2_DyDphi         = loadH2(inputFile, makeName(bn,"A2_DyDphi"));
    h_B2_DyDphi         = loadH2(inputFile, makeName(bn,"B2_DyDphi"));
    h_C2_DyDphi         = loadH2(inputFile, makeName(bn,"C2_DyDphi"));
    h_R2_DyDphi         = loadH2(inputFile, makeName(bn,"R2_DyDphi"));

    h_rho2_DyDphi_shft  = loadH2(inputFile, makeName(bn,"rho2_DyDphi_shft"));
    h_A2_DyDphi_shft    = loadH2(inputFile, makeName(bn,"A2_DyDphi_shft"));
    h_B2_DyDphi_shft    = loadH2(inputFile, makeName(bn,"B2_DyDphi_shft"));
    h_C2_DyDphi_shft    = loadH2(inputFile, makeName(bn,"C2_DyDphi_shft"));
    h_R2_DyDphi_shft    = loadH2(inputFile, makeName(bn,"R2_DyDphi_shft"));

    if (fillP2)
      {
      h_pt1pt1_yY         = loadH2(inputFile, makeName(bn,"pt1pt1_yY"));
      h_DptDpt_yY         = loadH2(inputFile, makeName(bn,"DptDpt_yY"));
      h_P2_yY             = loadH2(inputFile, makeName(bn,"P2_yY"));
      h_G2_yY             = loadH2(inputFile, makeName(bn,"G2_yY"));

      h_pt1pt1_DyDphi     = loadH2(inputFile, makeName(bn,"pt1pt1_DyDphi"));
      h_DptDpt_DyDphi     = loadH2(inputFile, makeName(bn,"DptDpt_DyDphi"));
      h_P2_DyDphi         = loadH2(inputFile, makeName(bn,"P2_DyDphi"));
      h_G2_DyDphi         = loadH2(inputFile, makeName(bn,"G2_DyDphi"));

      h_DptDpt_DyDphi_shft= loadH2(inputFile, makeName(bn,"DptDpt_DyDphi_shft"));
      h_P2_DyDphi_shft    = loadH2(inputFile, makeName(bn,"P2_DyDphi_shft"));
      h_G2_DyDphi_shft    = loadH2(inputFile, makeName(bn,"G2_DyDphi_shft"));
      }
    }
}

void quickCopy(TH2 * source, TH2 * target)
{
  int sNx = source->GetNbinsX();
  int sNy = source->GetNbinsY();
  int tNx = target->GetNbinsX();
  int tNy = target->GetNbinsY();


  if (sNx!=tNx || sNy!=tNy)
    {
    cout << "You are out of luck body" << endl;
    return;
    }
  for (int iX=1; iX<sNx; iX++)
    {
    for (int iY=1; iY<sNy; iY++)
      {
      double v  = source->GetBinContent(iX,iY);
      double ev = source->GetBinError(iX,iY);
      target->SetBinContent(iX,iY,v);
      target->SetBinError(iX,iY,v);
      }
    }
}


// Histograms from ParticlePairDerivedHistos must be normalized "per event" before calling this function
void ParticlePairDerivedHistos::calculatePairDerivedHistograms(ParticleHistos         & part1BaseHistos,
                                                               ParticleHistos         & part2BaseHistos,
                                                               ParticleDerivedHistos  & part1DerivedHistos,
                                                               ParticleDerivedHistos  & part2DerivedHistos,
                                                               ParticlePairHistos     & pairHistos,
                                                               double bincorrection)
{
  if (reportStart(__FUNCTION__))
    ;
  MessageLogger::LogLevel store = getReportLevel();
  setReportLevel(MessageLogger::Debug);
  
  double yield1;
  double yield2;
  double avgPt1Phi;
  double avgPt2Phi;
  double scalingFactorPhi;
  double avgPt1Eta;
  double avgPt2Eta;
  double avgPt1Y;
  double avgPt2Y;
  double scalingFactorEta;
  double scalingFactorY;
  int ijNormalization = 0;


  //
  // pt vs pt
  //
  if (reportDebug(__FUNCTION__))
    {
    if (!part1BaseHistos.h_n1_pt ) { cout << "  no part1BaseHistos" << endl; exit(0); }
    if (!part2BaseHistos.h_n1_pt ) { cout << "  no part2BaseHistos" << endl; exit(0); }
    if (!pairHistos.h_n2_ptpt )    { cout << "  no pairHistos.h_n2_ptpt" << endl; exit(0); }
    if (!h_n1n1_ptpt ) { cout << "  no h_n1n1_ptpt" << endl; exit(0); }
    if (!h_C2_ptpt ) { cout << "  no h_C2_ptpt" << endl; exit(0); }
    if (!h_B2_ptpt ) { cout << "  no h_B2_ptpt" << endl; exit(0); }
    if (!h_A2_ptpt ) { cout << "  no h_A2_ptpt" << endl; exit(0); }
    if (!h_R2_ptpt ) { cout << "  h_R2_ptpt" << endl; exit(0); }
    }

    cout << "Calculate XXX_ptpt" << endl;

  yield1 = part1BaseHistos.h_n1_pt->Integral();
  yield2 = part2BaseHistos.h_n1_pt->Integral();
  calculateN1N1_H1H1H2( part1BaseHistos.h_n1_pt,   part2BaseHistos.h_n1_pt,  h_n1n1_ptpt,1.0, 1.0);
  h_C2_ptpt->Add(pairHistos.h_n2_ptpt,h_n1n1_ptpt,1.0,-1.0);
  h_B2_ptpt->Add(pairHistos.h_n2_ptpt);
  h_B2_ptpt->Scale(1.0/yield2);
  h_A2_ptpt->Add(h_C2_ptpt);
  h_A2_ptpt->Scale(1.0/yield2);
  calculateR2_H2H2H2(pairHistos.h_n2_ptpt,h_n1n1_ptpt,h_R2_ptpt,0, 1.0, 1.0);

  //
  // phi vs phi
  //
  if (reportDebug(__FUNCTION__))  cout << "Calculate XXX_phiPhi" << endl;
  yield1 = part1DerivedHistos.h_n1_phi->Integral();
  yield2 = part2DerivedHistos.h_n1_phi->Integral();
  calculateN1N1_H1H1H2( part1DerivedHistos.h_n1_phi,  part2DerivedHistos.h_n1_phi, h_n1n1_phiPhi,1.0, 1.0);
  h_C2_phiPhi->Add(pairHistos.h_n2_phiPhi,h_n1n1_phiPhi,1.0,-1.0);
  h_B2_phiPhi->Add(pairHistos.h_n2_phiPhi);
  h_B2_phiPhi->Scale(1.0/yield2);
  h_A2_phiPhi->Add(h_C2_phiPhi);
  h_A2_phiPhi->Scale(1.0/yield2);
  calculateR2_H2H2H2(pairHistos.h_n2_phiPhi,h_n1n1_phiPhi,h_R2_phiPhi,0, 1.0, 1.0);

  if (fillP2)
    {
    if (reportDebug(__FUNCTION__))  cout << " Calculate P2 vs phiPhi" << endl;
    avgPt1Phi = avgValue(part1DerivedHistos.h_pt_phi);
    avgPt2Phi = avgValue(part2DerivedHistos.h_pt_phi);
    scalingFactorPhi = 1.0/avgPt1Phi/avgPt2Phi;
    h_P2_phiPhi->Add(h_DptDpt_phiPhi,h_DptDpt_phiPhi,1.0,0.0);
    h_P2_phiPhi->Scale(scalingFactorPhi);
    }
  
  
  if (fillEta)
    {

    //
    // eta vs eta
    //
    if (reportDebug(__FUNCTION__))  cout <<  "Calculate XXX_etaEta" << endl;
    yield1 = part1DerivedHistos.h_n1_eta->Integral();
    yield2 = part2DerivedHistos.h_n1_eta->Integral();
    calculateN1N1_H1H1H2(part1DerivedHistos.h_n1_eta,part2DerivedHistos.h_n1_eta,h_n1n1_etaEta,1.0, 1.0);
    h_C2_etaEta->Add(pairHistos.h_n2_etaEta,h_n1n1_etaEta,1.0,-1.0);
    h_B2_etaEta->Add(pairHistos.h_n2_etaEta);
    h_B2_etaEta->Scale(1.0/yield2);
    h_A2_etaEta->Add(h_C2_etaEta);
    h_A2_etaEta->Scale(1.0/yield2);
    calculateR2_H2H2H2(pairHistos.h_n2_etaEta,h_n1n1_etaEta,h_R2_etaEta,0, 1.0, 1.0);

    //
    // Deta vs Dphi
    //
    if (reportDebug(__FUNCTION__))  cout << "Calculate XXX_DetaDphi" << endl;
    reduce_n1EtaPhiN1EtaPhiOntoN1N1DetaDphi(part1BaseHistos.h_n1_phiEta,part2BaseHistos.h_n1_phiEta,h_n1n1_DetaDphi,nBins_Deta,nBins_Dphi);
    h_rho2_DetaDphi->Add(pairHistos.h_n2_DetaDphi);
    h_C2_DetaDphi->Add(h_rho2_DetaDphi,h_n1n1_DetaDphi, 1.0, -1.0);
    h_B2_DetaDphi->Add(h_rho2_DetaDphi);
    h_B2_DetaDphi->Scale(1.0/yield2);
    h_A2_DetaDphi->Add(h_C2_DetaDphi);
    h_A2_DetaDphi->Scale(1.0/yield2);
    calculateR2_H2H2H2(h_rho2_DetaDphi,h_n1n1_DetaDphi,h_R2_DetaDphi,0,1.0,1.0);

    if (reportDebug(__FUNCTION__))  cout <<  " shiftY XXX_DetaDphi" << endl;
    shiftY(*h_rho2_DetaDphi,*h_rho2_DetaDphi_shft,nBins_Dphi_shft);
    shiftY(*h_A2_DetaDphi,*h_A2_DetaDphi_shft,nBins_Dphi_shft);
    shiftY(*h_B2_DetaDphi,*h_B2_DetaDphi_shft,nBins_Dphi_shft);
    shiftY(*h_C2_DetaDphi,*h_C2_DetaDphi_shft,nBins_Dphi_shft);
    shiftY(*h_R2_DetaDphi,*h_R2_DetaDphi_shft,nBins_Dphi_shft);
    
    if (fillP2)
      {
      avgPt1Eta = avgValue(part1DerivedHistos.h_pt_eta);
      avgPt2Eta = avgValue(part2DerivedHistos.h_pt_eta);
      scalingFactorEta = 1.0/avgPt1Eta/avgPt2Eta;
      h_P2_etaEta->Add(h_DptDpt_etaEta,h_DptDpt_etaEta,1.0,0.0);
      h_P2_etaEta->Scale(scalingFactorEta);
      
      if (reportDebug(__FUNCTION__))  cout <<  " shiftY(*h_R2_DetaDphi,*h_R2_DetaDphi_shft,nBins_Dphi_shft)" << endl;
      
      shiftY(*h_DptDpt_DetaDphi, *h_DptDpt_DetaDphi_shft,nBins_Dphi_shft);
      
      /// calculate average pt base on h_pt_eta histograms
      h_P2_DetaDphi->Add(h_DptDpt_DetaDphi,h_DptDpt_DetaDphi,1.0,0.0);
      h_P2_DetaDphi->Scale(scalingFactorEta);
      //symmetrizeDeltaEtaDeltaPhi(h_P2_DetaDphi,ijNormalization);
      shiftY(*h_P2_DetaDphi,*h_P2_DetaDphi_shft, nBins_Dphi_shft);
      
      calculateG2_H2H2H2H2( h_DptDpt_etaEta,   h_n1n1_etaEta,   h_pt1pt1_etaEta,   h_G2_etaEta,   ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( h_DptDpt_phiPhi,   h_n1n1_phiPhi,   h_pt1pt1_phiPhi,   h_G2_phiPhi,   ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( h_DptDpt_DetaDphi, h_n1n1_DetaDphi, h_pt1pt1_DetaDphi, h_G2_DetaDphi, ijNormalization, bincorrection, 1.0);
      shiftY(*h_G2_DetaDphi,*h_G2_DetaDphi_shft,nBins_Dphi_shft);
      }
    }
  
  if (fillY)
    {
    //
    // y vs y
    //
    if (reportDebug(__FUNCTION__))  cout <<  "Calculate XXX_yY" << endl;
    yield1 = part1DerivedHistos.h_n1_y->Integral();
    yield2 = part2DerivedHistos.h_n1_y->Integral();
    calculateN1N1_H1H1H2(part1DerivedHistos.h_n1_y,part2DerivedHistos.h_n1_y,h_n1n1_yY,1.0, 1.0);
    h_C2_yY->Add(pairHistos.h_n2_yY,h_n1n1_yY,1.0,-1.0);
    h_B2_yY->Add(pairHistos.h_n2_yY);
    h_B2_yY->Scale(1.0/yield2);
    h_A2_yY->Add(h_C2_yY);
    h_A2_yY->Scale(1.0/yield2);
    calculateR2_H2H2H2(pairHistos.h_n2_yY,h_n1n1_yY,h_R2_yY,0, 1.0, 1.0);

    //
    // Dy vs Dphi
    //
    if (reportDebug(__FUNCTION__))  cout << "Calculate XXX_DyDphi" << endl;
    reduce_n1EtaPhiN1EtaPhiOntoN1N1DetaDphi(part1BaseHistos.h_n1_phiY,part2BaseHistos.h_n1_phiY,h_n1n1_DyDphi,nBins_Dy,nBins_Dphi);
    h_rho2_DyDphi->Add(pairHistos.h_n2_DyDphi);
    h_C2_DyDphi->Add(h_rho2_DyDphi,h_n1n1_DyDphi, 1.0, -1.0);
    h_B2_DyDphi->Add(h_rho2_DyDphi);
    h_B2_DyDphi->Scale(1.0/yield2);
    h_A2_DyDphi->Add(h_C2_DyDphi);
    h_A2_DyDphi->Scale(1.0/yield2);
    calculateR2_H2H2H2(h_rho2_DyDphi,h_n1n1_DyDphi,h_R2_DyDphi,0,1.0,1.0);

    if (reportDebug(__FUNCTION__))  cout <<  " shift XXX_DyDphi" << endl;
    shiftY(*h_rho2_DyDphi,*h_rho2_DyDphi_shft,   nBins_Dphi_shft);
    shiftY(*h_A2_DyDphi,  *h_A2_DyDphi_shft,     nBins_Dphi_shft);
    shiftY(*h_B2_DyDphi,  *h_B2_DyDphi_shft,     nBins_Dphi_shft);
    shiftY(*h_C2_DyDphi,  *h_C2_DyDphi_shft,     nBins_Dphi_shft);
    shiftY(*h_R2_DyDphi,  *h_R2_DyDphi_shft,     nBins_Dphi_shft);

    if (fillP2)
      {
      avgPt1Y = avgValue(part1DerivedHistos.h_pt_y);
      avgPt2Y = avgValue(part2DerivedHistos.h_pt_y);
      scalingFactorY = 1.0/avgPt1Y/avgPt2Y;
      h_P2_yY->Add(h_DptDpt_yY,h_DptDpt_yY,1.0,0.0);
      h_P2_yY->Scale(scalingFactorY);
      
      if (reportDebug(__FUNCTION__))  cout <<  " shiftY(*h_R2_DyDphi,*h_R2_DyDphi_shft,nBins_Dphi_shft)" << endl;
      
      shiftY(*h_DptDpt_DyDphi, *h_DptDpt_DyDphi_shft,nBins_Dphi_shft);
      
      /// calculate average pt base on h_pt_eta histograms
      h_P2_DyDphi->Add(h_DptDpt_DyDphi,h_DptDpt_DyDphi,1.0,0.0);
      h_P2_DyDphi->Scale(scalingFactorY);
      //symmetrizeDeltaEtaDeltaPhi(h_P2_DetaDphi,ijNormalization);
      shiftY(*h_P2_DyDphi,*h_P2_DyDphi_shft, nBins_Dphi_shft);
      
      calculateG2_H2H2H2H2( h_DptDpt_yY,       h_n1n1_yY,       h_pt1pt1_yY,       h_G2_yY,       ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( h_DptDpt_phiPhi,   h_n1n1_phiPhi,   h_pt1pt1_phiPhi,   h_G2_phiPhi,   ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( h_DptDpt_DetaDphi, h_n1n1_DetaDphi, h_pt1pt1_DetaDphi, h_G2_DetaDphi, ijNormalization, bincorrection, 1.0);
      shiftY(*h_G2_DetaDphi,*h_G2_DetaDphi_shft,nBins_Dphi_shft);
      }
    }

  if (reportEnd(__FUNCTION__))
    ;
  setReportLevel(store);
}
