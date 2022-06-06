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
#include "ParticleDerivedHistos.hpp"
ClassImp(ParticleDerivedHistos);

ParticleDerivedHistos::ParticleDerivedHistos(const TString & _name,
                                             const Configuration & _configuration,
                                             LogLevel  _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
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
  appendClassName("ParticleDerivedHistos");
  setInstanceName(name);
}

ParticleDerivedHistos::~ParticleDerivedHistos()
{
  //deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void ParticleDerivedHistos::createHistograms()
{
  if ( reportStart("ParticleDerivedHistos",getName(),"createHistograms()"))
    { }
  TString bn = getHistoBaseName();
  const Configuration & configuration = getConfiguration();
  fillEta  = configuration.getValueBool("fillEta");
  fillY    = configuration.getValueBool("fillY");
  fillP2   = configuration.getValueBool("fillP2");
  nBins_pt = configuration.getValueInt("nBins_pt");
  min_pt   = configuration.getValueDouble("min_pt");
  max_pt   = configuration.getValueDouble("max_pt");
  nBins_phi = configuration.getValueInt("nBins_phi");
  min_phi = configuration.getValueDouble("min_phi");
  max_phi = configuration.getValueDouble("max_phi");
  nBins_eta = configuration.getValueInt("nBins_eta");
  min_eta = configuration.getValueDouble("min_eta");
  max_eta = configuration.getValueDouble("max_eta");
  nBins_y = configuration.getValueInt("nBins_y");
  min_y = configuration.getValueDouble("min_y");
  max_y = configuration.getValueDouble("max_y");
   
  h_n1_phi     = createHistogram(makeName(bn,"n1_phi"), nBins_phi, min_phi, max_phi, "#varphi","#rho_{1}(#varphi)");

  if (fillP2)
    {
    h_spt_phi    = createHistogram(makeName(bn,"sumpt1_phi"), nBins_phi, min_phi, max_phi, "#varphi","#sum p_{T}");
    h_pt_phi     = createHistogram(makeName(bn,"pt_phi"),     nBins_phi, min_phi, max_phi, "#varphi","<p_{T}>");
    }

  if (fillEta)
    {
    h_n1_eta     = createHistogram(makeName(bn,"n1_eta"),     nBins_eta, min_eta, max_eta, "#eta",   "#rho_{1}(#eta)");
    if (fillP2)
      {
      h_spt_eta     = createHistogram(makeName(bn,"sumpt1_eta"),  nBins_eta, min_eta, max_eta, "#eta",   "#sum p_{T}");
      h_pt_eta     = createHistogram(makeName(bn,"pt_eta"),     nBins_eta, min_eta, max_eta, "#eta",   "<p_{T}>");
      h_pt_phiEta  = createHistogram(makeName(bn,"pt_phiEta"),  nBins_eta, min_eta, max_eta, nBins_phi, min_phi, max_phi, "#eta", "#varphi","N");
      }
    }
  if (fillY)
    {
    h_n1_y      = createHistogram(makeName(bn,"n1_y"),     nBins_y, min_y,   max_y, "y",   "#rho_{1}(y)");
    if (fillP2)
      {
      h_spt_y     = createHistogram(makeName(bn,"sumpt1_y"),  nBins_eta, min_eta, max_eta, "y",   "#sum p_{T}");
      h_pt_y      = createHistogram(makeName(bn,"pt_y"),     nBins_y,   min_y,   max_y,    "y",   "<p_{T}>");
      h_pt_phiY   = createHistogram(makeName(bn,"pt_phiY"),  nBins_y,   min_y,   max_y, nBins_phi, min_phi, max_phi, "y", "#varphi","N");
      }
    }
  if ( reportEnd(__FUNCTION__))
    { }
}

//________________________________________________________________________
void ParticleDerivedHistos::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__,inputFile)) return;
  TString bn = getHistoBaseName();
  h_n1_phi  = loadH1(inputFile,  makeName(bn,"n1_phi"));
  h_spt_phi = loadH1(inputFile,  makeName(bn,"spt_phi"));
  h_pt_phi  = loadH1(inputFile,  makeName(bn,"pt_phi"));

  if (fillEta)
    {
    h_n1_eta     = loadH1(inputFile,  makeName(bn,"n1_eta"));
    if (fillP2)
      {
      h_spt_eta     = loadH1(inputFile,  makeName(bn,"spt_eta"));
      h_pt_eta      = loadH1(inputFile,  makeName(bn,"pt_eta"));
      h_pt_phiEta   = loadH2(inputFile,  makeName(bn,"pt_phiEta"));
      }
    }
  if (fillY)
    {
    h_n1_y     = loadH1(inputFile,  makeName(bn,"n1_y"));
    if (fillP2)
      {
      h_spt_y     = loadH1(inputFile,  makeName(bn,"spt_y"));
      h_pt_y      = loadH1(inputFile,  makeName(bn,"pt_y"));
      h_pt_phiY   = loadH2(inputFile,  makeName(bn,"pt_phiY"));
      }
    }

  if ( reportEnd(__FUNCTION__))
    ;
}

//!
//! Calculates derived histograms. This is currently limited to pt averages vs eta and phi.
//!
void ParticleDerivedHistos::calculateDerivedHistograms(ParticleHistos * baseHistos)
{
  if (reportStart(__FUNCTION__))
    ;
  TString bn = getHistoBaseName();
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

  //restoreLogLevel();
}

