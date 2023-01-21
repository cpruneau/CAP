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
#include <TMath.h>
#include "ParticlePerformanceSimulator.hpp"

ClassImp(ParticlePerformanceSimulator);

ParticlePerformanceSimulator::ParticlePerformanceSimulator(Task * _parent,
                                                           int _filterIndex,
                                                           const TString & _name,
                                                           Configuration & _configuration)
:
Histograms(_parent,_name,_configuration),
filterIndex(_filterIndex),
useSameSetForAll(true),
resolutionOption(0),
efficiencyOption(0),
biasPtHistogram(),
rmsPtHistogram(),
biasEtaHistogram(),
rmsEtaHistogram(),
biasPhiHistogram(),
rmsPhiHistogram(),
ptFunction(),
etaFunction(),
phiFunction()
{
  //  initialize();
}

void ParticlePerformanceSimulator::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & config = getConfiguration();
  useSameSetForAll = config.getValueBool(getParentTaskName(),"useSameSetForAll");
  resolutionOption = config.getValueInt(getParentTaskName(),"resolutionOption");
  efficiencyOption = config.getValueInt(getParentTaskName(),"efficiencyOption");
  
  if (reportDebug(__FUNCTION__))
    {
    cout << "  useSameSetForAll: " << useSameSetForAll << endl;
    cout << "  resolutionOption: " << resolutionOption << endl;
    cout << "  efficiencyOption: " << efficiencyOption << endl;
    }
  
  double biasAinv;
  double biasA0;
  double biasA1;
  double biasA2;
  double rmsAinv;
  double rmsA0;
  double rmsA1;
  double rmsA2;
  TString baseName = "Filter";
  baseName += filterIndex;
  
  switch (resolutionOption)
    {
      case 0:
      ptFunction  = nullptr;
      etaFunction = nullptr;
      phiFunction = nullptr;
      break;
      
      case 1:
      biasAinv = config.getValueDouble(getParentTaskName(),baseName+"_PtBiasAinv");
      biasA0   = config.getValueDouble(getParentTaskName(),baseName+"_PtBiasA0");
      biasA1   = config.getValueDouble(getParentTaskName(),baseName+"_PtBiasA1");
      biasA2   = config.getValueDouble(getParentTaskName(),baseName+"_PtBiasA2");
      rmsAinv  = config.getValueDouble(getParentTaskName(),baseName+"_PtRmsAinv");
      rmsA0    = config.getValueDouble(getParentTaskName(),baseName+"_PtRmsA0");
      rmsA1    = config.getValueDouble(getParentTaskName(),baseName+"_PtRmsA1");
      rmsA2    = config.getValueDouble(getParentTaskName(),baseName+"_PtRmsA2");
      if (reportDebug(__FUNCTION__))
        {
        cout << " pT smearer:" << endl;
        cout << "   biasAinv: " << biasAinv << endl;
        cout << "     biasA0: " << biasA0 << endl;
        cout << "     biasA1: " << biasA1 << endl;
        cout << "     biasA2: " << biasA2 << endl;
        cout << "    rmsAinv: " << rmsAinv << endl;
        cout << "      rmsA0: " << rmsA0 << endl;
        cout << "      rmsA1: " << rmsA1 << endl;
        cout << "      rmsA2: " << rmsA2 << endl;
        }
      ptFunction = new ResolutionFunction(0,biasAinv,biasA0,biasA1,biasA2,rmsAinv,rmsA0,rmsA1,rmsA2);
      
      biasAinv = config.getValueDouble(getParentTaskName(),baseName+"_EtaBiasAinv");
      biasA0   = config.getValueDouble(getParentTaskName(),baseName+"_EtaBiasA0");
      biasA1   = config.getValueDouble(getParentTaskName(),baseName+"_EtaBiasA1");
      biasA2   = config.getValueDouble(getParentTaskName(),baseName+"_EtaBiasA2");
      rmsAinv  = config.getValueDouble(getParentTaskName(),baseName+"_EtaRmsAinv");
      rmsA0    = config.getValueDouble(getParentTaskName(),baseName+"_EtaRmsA0");
      rmsA1    = config.getValueDouble(getParentTaskName(),baseName+"_EtaRmsA1");
      rmsA2    = config.getValueDouble(getParentTaskName(),baseName+"_EtaRmsA2");
      if (reportDebug(__FUNCTION__))
        {
        cout << " eta smearer:"<< endl;
        cout << "   biasAinv: " << biasAinv << endl;
        cout << "     biasA0: " << biasA0 << endl;
        cout << "     biasA1: " << biasA1 << endl;
        cout << "     biasA2: " << biasA2 << endl;
        cout << "    rmsAinv: " << rmsAinv << endl;
        cout << "      rmsA0: " << rmsA0 << endl;
        cout << "      rmsA1: " << rmsA1 << endl;
        cout << "      rmsA2: " << rmsA2 << endl;
        }
      etaFunction = new ResolutionFunction(1,biasAinv,biasA0,biasA1,biasA2,rmsAinv,rmsA0,rmsA1,rmsA2);
      
      biasAinv = config.getValueDouble(getParentTaskName(),baseName+"_PhiBiasAinv");
      biasA0   = config.getValueDouble(getParentTaskName(),baseName+"_PhiBiasA0");
      biasA1   = config.getValueDouble(getParentTaskName(),baseName+"_PhiBiasA1");
      biasA2   = config.getValueDouble(getParentTaskName(),baseName+"_PhiBiasA2");
      rmsAinv  = config.getValueDouble(getParentTaskName(),baseName+"_PhiRmsAinv");
      rmsA0    = config.getValueDouble(getParentTaskName(),baseName+"_PhiRmsA0");
      rmsA1    = config.getValueDouble(getParentTaskName(),baseName+"_PhiRmsA1");
      rmsA2    = config.getValueDouble(getParentTaskName(),baseName+"_PhiRmsA2");
      if (reportDebug(__FUNCTION__))
        {
        cout << " phi smearer:"<< endl;
        cout << "   biasAinv: " << biasAinv << endl;
        cout << "     biasA0: " << biasA0 << endl;
        cout << "     biasA1: " << biasA1 << endl;
        cout << "     biasA2: " << biasA2 << endl;
        cout << "    rmsAinv: " << rmsAinv << endl;
        cout << "      rmsA0: " << rmsA0 << endl;
        cout << "      rmsA1: " << rmsA1 << endl;
        cout << "      rmsA2: " << rmsA2 << endl;
        }
      phiFunction = new ResolutionFunction(0,biasAinv,biasA0,biasA1,biasA2,rmsAinv,rmsA0,rmsA1,rmsA2);
      break;
      
      case 2:
      ptFunction  = nullptr;
      etaFunction = nullptr;
      phiFunction = nullptr;
      break;
    }
  
  switch (efficiencyOption)
    {
      case 0:
      efficiencyFunction  = nullptr;
      break;
      
      case 1:
      {
      double effPeakAmp = config.getValueDouble(getParentTaskName(),baseName+"_EffPeakAmp");
      double effPeakPt  = config.getValueDouble(getParentTaskName(),baseName+"_EffPeakPt");
      double effPeakRms = config.getValueDouble(getParentTaskName(),baseName+"_EffPeakRms");
      double effA1      = config.getValueDouble(getParentTaskName(),baseName+"_EffA1");
      double effA2      = config.getValueDouble(getParentTaskName(),baseName+"_EffA2");
      if (reportDebug(__FUNCTION__))
        {
        cout << " efficiency:"  << endl;
        cout << "   effPeakAmp: " << effPeakAmp << endl;
        cout << "    effPeakPt: " << effPeakPt << endl;
        cout << "   effPeakRms: " << effPeakRms << endl;
        cout << "        effA1: " << effA1 << endl;
        cout << "        effA2: " << effA2 << endl;
        }
      efficiencyFunction = new EfficiencyFunction(effPeakAmp,effPeakPt,effPeakRms,effA1,effA2);
      }
      break;
      
      case 2:
      ptFunction  = nullptr;
      etaFunction = nullptr;
      phiFunction = nullptr;
      break;
    }
}

void ParticlePerformanceSimulator::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & config = getConfiguration();
  useSameSetForAll = config.getValueBool(getParentTaskName(),"useSameSetForAll");
  resolutionOption = config.getValueInt(getParentTaskName(),"resolutionOption");
  efficiencyOption = config.getValueInt(getParentTaskName(),"efficiencyOption");

  if (reportDebug(__FUNCTION__))
    {
    cout << "useSameSetForAll..........: " << useSameSetForAll << endl;
    cout << "resolutionOption..........: " << resolutionOption << endl;
    cout << "efficiencyOption..........: " << efficiencyOption << endl;
    }
  
  TString baseName = "Filter";
  baseName += filterIndex;
  
  switch (resolutionOption)
    {
      case 0:
      case 1:
      break;
      
      case 2:
      biasPtHistogram  = loadH1(inputFile,baseName+"_PtBias");
      rmsPtHistogram   = loadH1(inputFile,baseName+"_PtRms");
      biasEtaHistogram = loadH1(inputFile,baseName+"_EtaBias");
      rmsEtaHistogram  = loadH1(inputFile,baseName+"_EtaRms");
      biasPhiHistogram = loadH1(inputFile,baseName+"_PhiBias");
      rmsPhiHistogram  = loadH1(inputFile,baseName+"_PhiRms");
      break;
    }
  
  switch (efficiencyOption)
    {
      case 0:
      case 1:
      break;
      
      case 2:
      efficienyHistogram   = loadH1(inputFile,baseName+"_Eff");
      break;
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePerformanceSimulator::smearMomentum(const TLorentzVector &in, TLorentzVector & out)
{
  double smearedPt  = 0.0;
  double smearedEta = 0.0;
  double smearedPhi = 0.0;
  double pt   = in.Pt();
  double phi  = in.Phi();
  double eta  = in.Eta();
  double mass = in.M();
  smearMomentum(pt,eta,phi,smearedPt,smearedEta,smearedPhi);

  if (smearedPt<0.001) smearedPt = 0.001;

  out.SetPtEtaPhiM(smearedPt,smearedEta,smearedPhi,mass);
}

void ParticlePerformanceSimulator::smearMomentum(double pt, double eta, double phi,
                                                 double &smearedPt, double &smearedEta, double &smearedPhi)
{
  double bias;
  double rms;
  double zeroRms = 0.0001;
  
  switch (resolutionOption)
    {
      default:
      case 0:
      bias = 0.0;
      rms  = zeroRms;
      smearedPt  = pt;
      smearedEta = eta;
      smearedPhi = phi;
      break;
      
      case 1:
      smearFromFunction(pt, eta, phi, ptFunction, bias, rms);
      smearedPt  = gRandom->Gaus(pt+bias,rms);
      //cout << "pt:"<< pt<< " eta:" << eta << " phi:" << phi << " bias:" << bias << " rms:" << rms << " smearedPt" << smearedPt << endl;
      smearFromFunction(pt, eta, phi, etaFunction, bias, rms);
      smearedEta = gRandom->Gaus(eta+bias,rms);
      smearFromFunction(pt, eta, phi, phiFunction, bias, rms);
      smearedPhi = gRandom->Gaus(phi+bias,rms);
      break;
      
      case 2:
      smearFromHisto(pt, eta, phi, biasPtHistogram, rmsPtHistogram, bias, rms);
      smearedPt = gRandom->Gaus(pt+bias,rms);
      smearFromHisto(pt, eta, phi, biasEtaHistogram, rmsEtaHistogram, bias, rms);
      smearedEta = gRandom->Gaus(eta+bias,rms);
      smearFromHisto(pt, eta, phi, biasPhiHistogram, rmsPhiHistogram, bias, rms);
      smearedPhi = gRandom->Gaus(phi+bias,rms);
      break;
    }
}

void ParticlePerformanceSimulator::smearFromHisto(double pt, double eta, double phi,
                                                  TH1 * biasHisto, TH1 * rmsHisto,
                                                  double & bias, double & rms)
{
  int bin;
  if (biasHisto->IsA() == TH1::Class())
    {
    bin = biasHisto->FindBin(pt);
    bias = biasHisto->GetBinContent(bin);
    }
  else  if (biasHisto->IsA() == TH2::Class())
    {
    bin = biasHisto->FindBin(pt,eta);
    bias = biasHisto->GetBinContent(bin);
    }
  else if (biasHisto->IsA() == TH3::Class())
    {
    bin = biasHisto->FindBin(pt,eta,phi);
    bias = biasHisto->GetBinContent(bin);
    }
  if (rmsHisto->IsA() == TH1::Class())
    {
    bin = rmsHisto->FindBin(pt);
    rms = rmsHisto->GetBinContent(bin);
    }
  else  if (rmsHisto->IsA() == TH2::Class())
    {
    bin = rmsHisto->FindBin(pt,eta);
    rms = rmsHisto->GetBinContent(bin);
    }
  else if (rmsHisto->IsA() == TH3::Class())
    {
    bin = rmsHisto->FindBin(pt,eta,phi);
    rms = rmsHisto->GetBinContent(bin);
    }
}

void ParticlePerformanceSimulator::smearFromFunction(double pt, double eta, double phi,
                                                     ResolutionFunction* f,
                                                     double & bias, double & rms)
{
  bias = f->getBias(pt,eta,phi);
  rms  = f->getRms(pt,eta,phi);
}


bool ParticlePerformanceSimulator::accept(const TLorentzVector& momentum)
{
  bool accepting = false;
  double pt  = momentum.Pt();
  double eta = momentum.Eta();
  double phi = momentum.Phi();
  switch (efficiencyOption)
    {
      default:
      case 0:
      accepting = true;
      break;
      case 1:
      accepting = acceptFromFunction(pt, eta, phi);
      break;
      case 2:
      accepting = acceptFromHisto(pt, eta, phi);
      break;
    }
  return accepting;
}


bool ParticlePerformanceSimulator::accept(double pt, double eta, double phi)
{
  bool accepting = false;
  switch (efficiencyOption)
    {
      default:
      case 0:
      accepting = true;
      break;
      case 1:
      accepting = acceptFromFunction(pt, eta, phi);
      break;
      case 2:
      accepting = acceptFromHisto(pt, eta, phi);
      break;
    }
  return accepting;
}

bool ParticlePerformanceSimulator::acceptFromHisto(double pt, double eta, double phi)
{
  int bin;
  double efficiency = 1.0;
  bool   accepting  = false;
  if (efficienyHistogram->IsA() == TH1::Class())
    {
    bin = efficienyHistogram->FindBin(pt);
    efficiency = efficienyHistogram->GetBinContent(bin);
    }
  else  if (efficienyHistogram->IsA() == TH2::Class())
    {
    bin = efficienyHistogram->FindBin(pt,eta);
    efficiency = efficienyHistogram->GetBinContent(bin);
    }
  else if (efficienyHistogram->IsA() == TH3::Class())
    {
    bin = efficienyHistogram->FindBin(pt,eta,phi);
    efficiency = efficienyHistogram->GetBinContent(bin);
    }
  if (gRandom->Rndm()<efficiency) accepting = true;
  return accepting;
}

bool ParticlePerformanceSimulator::acceptFromFunction(double pt, double eta, double phi)
{
  bool   accepting  = false;
  double efficiency = efficiencyFunction->getEfficiency(pt,eta,phi);
  if (gRandom->Rndm()<efficiency) accepting = true;
  return accepting;
}


