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
#include "ParticleSingleDerivedHistos.hpp"
using CAP::ParticleSingleDerivedHistos;

ClassImp(ParticleSingleDerivedHistos);

ParticleSingleDerivedHistos::ParticleSingleDerivedHistos(Task * _parent,
                                             const String & _name,
                                             Configuration & _configuration)
:
HistogramGroup(_parent,_name,_configuration),
fillEta(false),
fillY(false),
fillP2(false),
nBins_n1(0),
min_n1(0.0),
max_n1(0.0),
nBins_pt(0),
min_pt(0.0),
max_pt(0.0),
scale_pt(0.0),
nBins_phi(0),
min_phi(0.0),
max_phi(0.0),
scale_phi(0.0),
nBins_eta(0),
min_eta(0.0),
max_eta(0.0),
range_eta(0.0),
nBins_y(0),
min_y(0.0),
max_y(0.0),
range_y(0.0),
h_n1_phi(nullptr),
h_n1_eta(nullptr),
h_n1_y(nullptr),
h_spt_phi(nullptr),
h_spt_eta(nullptr),
h_spt_y(nullptr),
h_pt_phi(nullptr),
h_pt_eta(nullptr),
h_pt_phiEta(nullptr),
h_pt_y(nullptr),
h_pt_phiY(nullptr)
{
  appendClassName("ParticleSingleDerivedHistos");
}

void ParticleSingleDerivedHistos::createHistograms()
{
  if ( reportStart(__FUNCTION__))
    { }
  const String & bn  = getName();
  const String & ptn = getParentTaskName();
  const String & ppn = getParentPathName();
  Configuration & configuration = getConfiguration();
  fillEta   = configuration.getValueBool(ppn,"FillEta");
  fillY     = configuration.getValueBool(ppn,"FillY");
  fillP2    = configuration.getValueBool(ppn,"FillP2");
  nBins_pt  = configuration.getValueInt(ppn,"nBins_pt");
  min_pt    = configuration.getValueDouble(ppn,"Min_pt");
  max_pt    = configuration.getValueDouble(ppn,"Max_pt");
  nBins_phi = configuration.getValueInt(ppn,"nBins_phi");
  min_phi   = configuration.getValueDouble(ppn,"Min_phi");
  max_phi   = configuration.getValueDouble(ppn,"Max_phi");
  nBins_eta = configuration.getValueInt(ppn,"nBins_eta");
  min_eta   = configuration.getValueDouble(ppn,"Min_eta");
  max_eta   = configuration.getValueDouble(ppn,"Max_eta");
  nBins_y   = configuration.getValueInt(ppn,"nBins_y");
  min_y     = configuration.getValueDouble(ppn,"Min_y");
  max_y     = configuration.getValueDouble(ppn,"Max_y");

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "  Part:Parent Task Name....................: " << ptn << endl;
    cout << "  Part:Parent Path Name....................: " << ppn << endl;
    cout << "  Part:Histo Base Name.....................: " << bn << endl;
    cout << "  Part:nBins_pt............................: " << nBins_pt << endl;
    cout << "  Part:Min_pt..............................: " << min_pt << endl;
    cout << "  Part:Max_pt..............................: " << max_pt << endl;
    cout << "  Part:nBins_phi...........................: " << nBins_phi << endl;
    cout << "  Part:Min_phi.............................: " << min_phi << endl;
    cout << "  Part:Max_phi.............................: " << max_phi << endl;
    cout << "  Part:nBins_eta...........................: " << nBins_eta << endl;
    cout << "  Part:Min_eta.............................: " << min_eta << endl;
    cout << "  Part:Max_eta.............................: " << max_eta << endl;
    cout << "  Part:nBins_y.............................: " << nBins_y << endl;
    cout << "  Part:Min_y...............................: " << min_y << endl;
    cout << "  Part:Max_y...............................: " << max_y << endl;
    cout << "  Part:FillEta.............................: " << fillEta << endl;
    cout << "  Part:FillY...............................: " << fillY << endl;
    cout << "  Part:FillP2..............................: " << fillP2 << endl;
    }

  h_n1_phi     = createHistogram(createName(bn,"n1_phi"), nBins_phi, min_phi, max_phi, "#varphi","#rho_{1}(#varphi)");
  if (fillP2)
    {
    h_spt_phi    = createHistogram(createName(bn,"sumpt1_phi"), nBins_phi, min_phi, max_phi, "#varphi","#sum p_{T}");
    h_pt_phi     = createHistogram(createName(bn,"pt_phi"),     nBins_phi, min_phi, max_phi, "#varphi","<p_{T}>");
    }

  if (fillEta)
    {
    h_n1_eta     = createHistogram(createName(bn,"n1_eta"),     nBins_eta, min_eta, max_eta, "#eta",   "#rho_{1}(#eta)");
    if (fillP2)
      {
      h_spt_eta     = createHistogram(createName(bn,"sumpt1_eta"),  nBins_eta, min_eta, max_eta, "#eta",   "#sum p_{T}");
      h_pt_eta     = createHistogram(createName(bn,"pt_eta"),     nBins_eta, min_eta, max_eta, "#eta",   "<p_{T}>");
      h_pt_phiEta  = createHistogram(createName(bn,"pt_phiEta"),  nBins_eta, min_eta, max_eta, nBins_phi, min_phi, max_phi, "#eta", "#varphi","N");
      }
    }
  if (fillY)
    {
    h_n1_y      = createHistogram(createName(bn,"n1_y"),     nBins_y, min_y,   max_y, "y",   "#rho_{1}(y)");
    if (fillP2)
      {
      h_spt_y     = createHistogram(createName(bn,"sumpt1_y"),  nBins_eta, min_eta, max_eta, "y",   "#sum p_{T}");
      h_pt_y      = createHistogram(createName(bn,"pt_y"),     nBins_y,   min_y,   max_y,    "y",   "<p_{T}>");
      h_pt_phiY   = createHistogram(createName(bn,"pt_phiY"),  nBins_y,   min_y,   max_y, nBins_phi, min_phi, max_phi, "y", "#varphi","N");
      }
    }
  if ( reportEnd(__FUNCTION__))
    ;
}

//________________________________________________________________________
void ParticleSingleDerivedHistos::loadHistograms(TFile * inputFile)
{
  if (!ptrFileExist(__FUNCTION__,inputFile)) return;
  const String & bn  = getName();
  const String & ptn = getParentTaskName();
  const String & ppn = getParentPathName();
  Configuration & configuration = getConfiguration();

  fillEta  = configuration.getValueBool(ppn,"FillEta");
  fillY    = configuration.getValueBool(ppn,"FillY");
  fillP2   = configuration.getValueBool(ppn,"FillP2");
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "  Part:FillEta.............: " << fillEta << endl;
    cout << "  Part:FillY...............: " << fillY << endl;
    cout << "  Part:FillP2..............: " << fillP2 << endl;
    }
  h_n1_phi  = loadH1(inputFile,  createName(bn,"n1_phi"));
  h_spt_phi = loadH1(inputFile,  createName(bn,"spt_phi"));
  h_pt_phi  = loadH1(inputFile,  createName(bn,"pt_phi"));

  if (fillEta)
    {
    h_n1_eta     = loadH1(inputFile,  createName(bn,"n1_eta"));
    if (fillP2)
      {
      h_spt_eta     = loadH1(inputFile,  createName(bn,"spt_eta"));
      h_pt_eta      = loadH1(inputFile,  createName(bn,"pt_eta"));
      h_pt_phiEta   = loadH2(inputFile,  createName(bn,"pt_phiEta"));
      }
    }
  if (fillY)
    {
    h_n1_y     = loadH1(inputFile,  createName(bn,"n1_y"));
    if (fillP2)
      {
      h_spt_y     = loadH1(inputFile,  createName(bn,"spt_y"));
      h_pt_y      = loadH1(inputFile,  createName(bn,"pt_y"));
      h_pt_phiY   = loadH2(inputFile,  createName(bn,"pt_phiY"));
      }
    }
  if ( reportEnd(__FUNCTION__))
    ;
}

//!
//! Calculates derived histograms. This is currently limited to pt averages vs eta and phi.
//!
void ParticleSingleDerivedHistos::calculateDerivedHistograms(ParticleSingleHistos * baseHistos)
{
  if (reportStart(__FUNCTION__))
    ;
  String bn = getParentTaskName();
  TH1* hTemp;

  if (baseHistos->h_n1_phiEta)
    {
    if (reportDebug(__FUNCTION__)) cout << "Compute h_n1_phi" << endl;
    hTemp = baseHistos->h_n1_phiEta->ProjectionY();
    h_n1_phi->Add(hTemp);
    delete hTemp;

    if (reportDebug(__FUNCTION__)) cout << "Compute h_n1_eta" << endl;
    hTemp = baseHistos->h_n1_phiEta->ProjectionX();
    h_n1_eta->Add(hTemp);
    delete hTemp;

    if (fillP2)
      {
      if (reportDebug(__FUNCTION__)) cout << "Compute sumpt1_phi" << endl;
      hTemp = baseHistos->h_spt_phiEta->ProjectionY();
      h_spt_phi->Add(hTemp);
      delete hTemp;

      if (reportDebug(__FUNCTION__)) cout << "Compute pt_phi" << endl;
      calculateAveragePt(h_spt_phi, h_n1_phi, h_pt_phi);

      if (reportDebug(__FUNCTION__)) cout << "Compute h_spt_eta" << endl;
      hTemp = baseHistos->h_spt_phiEta->ProjectionX();
      h_spt_eta->Add(hTemp);
      delete hTemp;

      if (reportDebug(__FUNCTION__)) cout << "Compute h_pt_eta" << endl;
      calculateAveragePt(h_spt_eta, h_n1_eta, h_pt_eta);
      }
    }

  if (baseHistos->h_n1_phiY)
    {
    if (reportDebug(__FUNCTION__)) cout << "Compute h_n1_phi" << endl;
    hTemp = baseHistos->h_n1_phiY->ProjectionY();
    h_n1_phi->Reset();
    h_n1_phi->Add(hTemp);
    delete hTemp;


    if (reportDebug(__FUNCTION__)) cout << "Compute h_n1_y" << endl;
    hTemp = baseHistos->h_n1_phiY->ProjectionX();
    h_n1_y->Add(hTemp);
    delete hTemp;

    if (fillP2)
      {
      if (!baseHistos->h_n1_phiEta)
        {
        if (reportDebug(__FUNCTION__)) cout << "Compute h_spt_phi" << endl;
        hTemp = baseHistos->h_spt_phiY->ProjectionY();
        h_spt_phi->Add(hTemp);
        delete hTemp;

        if (reportDebug(__FUNCTION__)) cout << "Compute h_pt_phi" << endl;
        calculateAveragePt(h_spt_phi, h_n1_phi, h_pt_phi);
        }

      if (reportDebug(__FUNCTION__)) cout << "Compute h_spt_y" << endl;
      hTemp = baseHistos->h_spt_phiY->ProjectionX();
      h_spt_y->Add(hTemp);
      delete hTemp;

      if (reportDebug(__FUNCTION__)) cout << "Compute h_pt_y" << endl;
      calculateAveragePt(h_spt_y, h_n1_y, h_pt_y);
      }
    }

  //restoreSeverity();
}

