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
#include "ParticleSmearer.hpp"

ClassImp(ParticleSmearer);

// ==========================================================================================
// CTOR
// Note: To turn off filtering on a specific variable, set the min large than the max.
// ==========================================================================================
ParticleSmearer::ParticleSmearer(TH1 * _avgPt,
                                 TH1 * _rmsPt,
                                 TH1 * _avgPhi,
                                 TH1 * _rmsPhi,
                                 TH1 * _avgEta,
                                 TH1 * _rmsEta)
:
avgPt (_avgPt),
rmsPt (_rmsPt),
avgPhi(_avgPhi),
rmsPhi(_rmsPhi),
avgEta(_avgEta),
rmsEta(_rmsEta)
{
  // no ops
}

ParticleSmearer::ParticleSmearer(const ParticleSmearer & otherFilter)
:
avgPt((TH1 *)otherFilter.avgPt->Clone()),
rmsPt((TH1 *)otherFilter.rmsPt->Clone()),
avgPhi((TH1 *)otherFilter.avgPhi->Clone()),
rmsPhi((TH1 *)otherFilter.rmsPhi->Clone()),
avgEta((TH1 *)otherFilter.avgEta->Clone()),
rmsEta((TH1 *)otherFilter.rmsEta->Clone())
{
  // no ops
}

ParticleSmearer & ParticleSmearer::operator=(const ParticleSmearer & otherFilter)
{
  if (this!=&otherFilter)
    {
    avgPt  = (TH1 *) otherFilter.avgPt->Clone();
    rmsPt  = (TH1 *) otherFilter.rmsPt->Clone();
    avgPhi = (TH1 *) otherFilter.avgPhi->Clone();
    rmsPhi = (TH1 *) otherFilter.rmsPhi->Clone();
    avgEta = (TH1 *) otherFilter.avgEta->Clone();
    rmsEta = (TH1 *) otherFilter.rmsEta->Clone();
    }
  return *this;
}

ParticleSmearer::~ParticleSmearer()
{
  if (avgPt)  delete avgPt;
  if (rmsPt)  delete rmsPt;
  if (avgPhi) delete avgPhi;
  if (rmsPhi) delete rmsPhi;
  if (avgEta) delete avgEta;
  if (rmsEta) delete rmsEta;
}

void ParticleSmearer::smear(const TLorentzVector &in, TLorentzVector & out)
{
  double pt   = in.Pt();
  double phi  = in.Phi();
  double eta  = in.Eta();
  double mass = in.M();
  double smearedPt;
  double smearedPhi;
  double smearedEta;
  smearPt (pt, phi, eta, smearedPt);
  smearEta(pt, phi, eta, smearedEta);
  smearPhi(pt, phi, eta, smearedPhi);
  out.SetPtEtaPhiM(smearedPt,smearedEta,smearedPhi,mass);
}

void ParticleSmearer::smearPt(double pt, double phi, double eta, double & smearedPt)
{
  int bin;
  double avg;
  double rms;
  if (avgPt)
    {
    if (avgPt->IsA() == TH1::Class()) // pt only
      {
      bin = avgPt->FindBin(pt);
      avg = avgPt->GetBinContent(bin);
      }
    else  if (avgPt->IsA() == TH2::Class())
      {
      bin = avgPt->FindBin(eta,pt);
      avg = avgPt->GetBinContent(bin);
      }
    else if (avgPt->IsA() == TH3::Class())
      {
      bin = avgPt->FindBin(eta,phi,pt);
      avg = avgPt->GetBinContent(bin);
      }
    }
  else
    {
    avg = 1.0;
    }
  if (rmsPt)
    {
    if (rmsPt->IsA() == TH1::Class()) // pt only
      {
      bin = rmsPt->FindBin(pt);
      rms = rmsPt->GetBinContent(bin);
      }
    else  if (rmsPt->IsA() == TH2::Class())
      {
      bin = rmsPt->FindBin(eta,pt);
      rms = rmsPt->GetBinContent(bin);
      }
    else if (rmsPt->IsA() == TH3::Class())
      {
      bin = rmsPt->FindBin(eta,phi,pt);
      rms = rmsPt->GetBinContent(bin);
      }
    }
  else
    {
    rms = 0.000001;
    }
  smearedPt = gRandom->Gaus(avg*pt,rms*pt);
}

void ParticleSmearer::smearEta(double pt, double phi, double eta, double & smearedEta)
{
  int bin;
  double avg;
  double rms;
  if (avgEta)
    {
    if (avgEta->IsA() == TH1::Class()) // pt only
      {
      bin = avgEta->FindBin(pt);
      avg = avgEta->GetBinContent(bin);
      }
    else  if (avgEta->IsA() == TH2::Class())
      {
      bin = avgEta->FindBin(eta,pt);
      avg = avgEta->GetBinContent(bin);
      }
    else if (avgEta->IsA() == TH3::Class())
      {
      bin = avgEta->FindBin(eta,phi,pt);
      avg = avgEta->GetBinContent(bin);
      }
    }
  else
    {
    avg = 1.0;
    }
  if (rmsEta)
    {
    if (rmsEta->IsA() == TH1::Class()) // pt only
      {
      bin = rmsEta->FindBin(pt);
      rms = rmsEta->GetBinContent(bin);
      }
    else  if (rmsEta->IsA() == TH2::Class())
      {
      bin = rmsEta->FindBin(eta,pt);
      rms = rmsEta->GetBinContent(bin);
      }
    else if (rmsEta->IsA() == TH3::Class())
      {
      bin = rmsEta->FindBin(eta,phi,pt);
      rms = rmsEta->GetBinContent(bin);
      }
    }
  else
    {
    rms = 0.000001;
    }
  smearedEta = gRandom->Gaus(avg*eta,rms);
}

void ParticleSmearer::smearPhi(double pt, double phi, double eta, double & smearedPhi)
{
  int bin;
  double avg;
  double rms;
  if (avgPhi)
    {
    if (avgPhi->IsA() == TH1::Class()) // pt only
      {
      bin = avgPhi->FindBin(pt);
      avg = avgPhi->GetBinContent(bin);
      }
    else  if (avgPhi->IsA() == TH2::Class())
      {
      bin = avgPhi->FindBin(eta,pt);
      avg = avgPhi->GetBinContent(bin);
      }
    else if (avgPhi->IsA() == TH3::Class())
      {
      bin = avgPhi->FindBin(eta,phi,pt);
      avg = avgPhi->GetBinContent(bin);
      }
    }
  else
    {
    avg = 1.0;
    }
  if (rmsPhi)
    {
    if (rmsPhi->IsA() == TH1::Class()) // pt only
      {
      bin = rmsPhi->FindBin(pt);
      rms = rmsPhi->GetBinContent(bin);
      }
    else  if (rmsEta->IsA() == TH2::Class())
      {
      bin = rmsPhi->FindBin(eta,pt);
      rms = rmsPhi->GetBinContent(bin);
      }
    else if (rmsPhi->IsA() == TH3::Class())
      {
      bin = rmsPhi->FindBin(eta,phi,pt);
      rms = rmsPhi->GetBinContent(bin);
      }
    }
  else
    {
    rms = 0.000001;
    }
  smearedPhi = gRandom->Gaus(avg*phi,rms);
}


//static vector<ParticleSmearer*> createParticleSmearer();

