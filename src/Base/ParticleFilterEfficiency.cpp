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
#include "ParticleFilterEfficiency.hpp"

ClassImp(ParticleFilterEfficiency);

ParticleFilterEfficiency::ParticleFilterEfficiency()
:
Filter<Particle>(),
efficiency(nullptr)
{
  // no ops
}



ParticleFilterEfficiency::ParticleFilterEfficiency(const ParticleFilterEfficiency & otherFilter)
:
Filter<Particle>(otherFilter),
efficiency(otherFilter.efficiency)
{
  // no ops
}

ParticleFilterEfficiency & ParticleFilterEfficiency::operator=(const ParticleFilterEfficiency & otherFilter)
{
  if (this!=&otherFilter)
    {
    Filter<Particle>::operator=(otherFilter);
    efficiency = (TH1*) otherFilter.efficiency->Clone();
    }
  return *this;
}


//!
//! DTOR
//!
ParticleFilterEfficiency::~ParticleFilterEfficiency()
{
  if (efficiency) delete efficiency;
}

//!
//! Filter particles with a parameterized efficiency
//!
//!  First establish this filter would accept the given particle. If it does, apply an efficiency dependent on the kinematics of the particle.
//!  pt     : accept conditionally if min_pt < pt <= max_pt  OR  if min_pt >= max_pt
//!  eta    : accept conditionally if min_eta< eta<= max_eta OR  if min_eta>= max_eta
//!  y      : accept conditionally if   min_y< y  <= max_y OR    if min_y>  = max_y
// ==========================================================================================
bool ParticleFilterEfficiency::accept(const Particle & particle __attribute__((unused)))
{
  bool accept = false;
//  if (!ParticleFilter::accept(particle) ) return accept;
//  TLorentzVector & momentum = particle.getMomentum();
//  double r = gRandom->Rndm();
//  double eff;
//  int bin;
//  if (efficiency->IsA() == TH1::Class()) // pt only
//    {
//    bin = efficiency->FindBin(momentum.Pt());
//    eff = efficiency->GetBinContent(bin);
//    if (r<=eff) accept = true;
//    }
//  else  if (efficiency->IsA() == TH2::Class())
//    {
//    bin = efficiency->FindBin(momentum.Eta(),momentum.Pt());
//    eff = efficiency->GetBinContent(bin);
//    if (r<=eff) accept = true;
//    }
//  else if (efficiency->IsA() == TH3::Class())
//    {
//    bin = efficiency->FindBin(momentum.Eta(),momentum.Phi(),momentum.Pt());
//    eff = efficiency->GetBinContent(bin);
//    if (r<=eff) accept = true;
//    }
  return accept;
}

