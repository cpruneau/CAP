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
#include "GaussianGeneratorTask.hpp"
#include <TVector2.h>
#include <TRandom.h>
#include "Event.hpp"

ClassImp(GaussianGeneratorTask);

GaussianGeneratorTask::GaussianGeneratorTask(const TString & _name,
                                             Configuration & _configuration)
:
Task(_name, _configuration),
profile(nullptr)
{
  appendClassName("GaussianGeneratorTask");
}

//!
//!
void GaussianGeneratorTask::setDefaultConfiguration()
{
  //Task::setDefaultConfiguration();
  setParameter("UseParticles", true);
  setParameter("UseEventStream0", true);
  setParameter("amplitude", 1.0);
  setParameter("gammaeta",  1.0);
  setParameter("gammaphi",  1.0);
  setParameter("omegaeta",  1.0);
  setParameter("omegaphi",  1.0);
}

void GaussianGeneratorTask::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  double amplitude = getValueDouble("amplitude");
  double gammaeta  = getValueDouble("gammaeta");
  double gammaphi  = getValueDouble("gammaphi");
  double omegaeta  = getValueDouble("omegaeta");
  double omegaphi  = getValueDouble("omegaphi");
  profile = new TF2("2DGenGauss","[0]*[1]*[2]/4.0/[3]/[4]/TMath::Gamma(1.0/[1])/TMath::Gamma(1.0/[2])*"
                    "TMath::Exp(-1.0*(TMath::Power(TMath::Abs(x/[3]),[1])+TMath::Power(TMath::Abs(y/[4]),[2])))");
  profile->SetParameters(amplitude,gammaeta,gammaphi,omegaeta,omegaphi);
  profile->SetRange(-4.0,-TMath::PiOver2(),4.0,TMath::PiOver2());
  if (reportEnd(__FUNCTION__))
    ;
}

GaussianGeneratorTask::~GaussianGeneratorTask()
{
  if (profile != nullptr) delete profile;
}

void GaussianGeneratorTask::execute()
{
  incrementTaskExecuted();
  Event & event = * eventStreams[0];

  int nParticles = event.getNParticles();
  for (int itrack = 0; itrack < nParticles; itrack++)
    {
    Particle *particle1 = event.getParticleAt(itrack);
    for (int jtrack = itrack+1; jtrack < nParticles; jtrack++)
      {
      Particle *particle2 = event.getParticleAt(jtrack);
      double deltaeta = particle1->getMomentum().Eta() - particle2->getMomentum().Eta();
      double deltaphi = TVector2::Phi_mpi_pi(particle1->getMomentum().Phi() - particle2->getMomentum().Phi());
      double weight; weight= profile->Eval(deltaeta,deltaphi) + 1.0;
      }
    }
}
