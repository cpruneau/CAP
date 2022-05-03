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
#include "NucleusGenerator.hpp"
#include <iostream>
using namespace std;

ClassImp(NucleusGenerator);

NucleusGenerator::NucleusGenerator()
:
generatorTypeName(),
generatorType(),
nR(0),
minR(0),
maxR(0),
parA(0),
parB(0),
parC(0),
rDensity(0),
rProfile(0),
rProfileGen(0),
useRecentering(1),
useNucleonExclusion(1),
exclusionRadius(0.4), // fm
exclusionRadiusSq(0.4*0.4)
{
}

NucleusGenerator::~NucleusGenerator()
{
  if (rDensity)    delete rDensity;
  if (rProfile)    delete rProfile;
  if (rProfileGen) delete rProfileGen;
}


// create the container but do not assign any positions or properties.
void NucleusGenerator::initialize(const TString & _generatorTypeName,
                                  int  _generatorType,
                                  double _parA, double _parB, double _parC,
                                  int _nR, double _minR, double _maxR,
                                  bool _useRecentering,
                                  bool _useNucleonExclusion,
                                  double _exclusionRadius)
{
  nR    = _nR;
  minR  = _minR;
  maxR  = _maxR;
  parA  = _parA;
  parB  = _parB;
  parC  = _parC;
  generatorTypeName   = _generatorTypeName;
  generatorType       = _generatorType;
  useRecentering      = _useRecentering;
  useNucleonExclusion = _useNucleonExclusion;
  exclusionRadius     = _exclusionRadius; // fm
  exclusionRadiusSq   = exclusionRadius*exclusionRadius;
  if (rDensity)    delete rDensity;    rDensity    = nullptr;
  if (rProfile)    delete rProfile;    rProfile    = nullptr;
  if (rProfileGen) delete rProfileGen; rProfileGen = nullptr;

  switch (generatorType)
    {
      case Uniform:        generatorTypeName += "_Uniform";        break;
      case WoodsSaxon:     generatorTypeName += "_WoodsSaxon";     break;
      case Exponential:    generatorTypeName += "_Exponential";    break;
      case Gaussian:       generatorTypeName += "_Gaussian";       break;
      case DoubleGaussian: generatorTypeName += "_DoubleGaussian"; break;
    }

  TString histoName;
  histoName   = generatorTypeName + "_rDensity";
  rDensity    = new TH1D(histoName,histoName,nR,minR, maxR);
  histoName   = generatorTypeName + "_rProfile";
  rProfile    = new TH1D(histoName,histoName,nR,minR, maxR);
  histoName   = generatorTypeName + "_rProfileGen";
  rProfileGen = new TH1D(histoName,histoName,nR,minR, maxR);

  double dr = (maxR-minR)/double(nR);
  double r  = minR + dr/2.0;
  double density, profile;
  for (int iR=0; iR<nR; iR++)
    {
    double r2 = r*r;
    switch (generatorType)
      {
        case Uniform: // uniform hard sphere
        density = (r*r*r<parA)? 1.0 : 0.0;
        break;
        case WoodsSaxon: // Woods-Saxon/Fermi
        density = 1.0/(1.0+exp((r-parA)/parB) );
        break;
        case Exponential: // exponential
        density = exp(-r/parA);
        break;
        case Gaussian: // gaussian
        density = exp(-r2/2.0/parA/parA);
        break;
        case DoubleGaussian: //double-gaussian
        density =  (1.0-parC)*exp(-r2/parA/parA)/parA/parA/parA;
        density += parC*exp(-r2/parB/parB)/parB/parB/parB;
        break;
      }
    profile = r2*density;
    rDensity->SetBinContent(iR+1,density);
    rDensity->SetBinError(iR+1,0.0);
    rProfile->SetBinContent(iR+1, profile);
    rProfile->SetBinError(iR+1,0.0);
    r += dr;
    }
}

void NucleusGenerator::generate(Nucleus & nucleus, double xShift)
{
  double r, cosTheta, phi;
  TLorentzVector position(0.0,0.0,0.0,0.0);
  //nucleus.reset(); already handled by the collision geometry
  unsigned int iNucleon = 0;
  unsigned int nNucleons = nucleus.getNNucleons();
  while (iNucleon < nNucleons)
    {
    Particle * nucleon = nucleus.getNucleonAt(iNucleon);
    generate(r, cosTheta, phi);
    nucleon->setRCosThetaPhiT(r,cosTheta,phi,0.0);
    //nucleon->printProperties(cout);
    int sanityCheck = 0;
    if (useNucleonExclusion)
      {
      bool reject = false;
      for (unsigned int jNucleon=0; jNucleon<iNucleon; jNucleon++)
        {
        Particle * otherNucleon = nucleus.getNucleonAt(jNucleon);
        if (nucleon->distanceXYZSq(otherNucleon) < exclusionRadiusSq)
          {
          reject = true;
          break;
          }
        }
      if (reject)
        {
        sanityCheck++;
        if (sanityCheck>200)
          {
          cout << "Nucleon rejected > 200 times" << endl;
          exit(1);
          }
        //cout << "reject  sanityCheck:" << sanityCheck << endl;
        continue;
        }
      }
    position += nucleon->getPosition();
    if (iNucleon<nucleus.getNProtons())
      nucleon->setType(ParticleType::getProtonType());
    else
      nucleon->setType(ParticleType::getNeutronType());
    iNucleon++;
    }
  // center of mass, recenter
  position *= 1.0/double(nNucleons);
  TLorentzVector shift(xShift -position.X(),-position.Y(),-position.Z(),0.0);

  for (unsigned int iNucleon=0; iNucleon<nNucleons; iNucleon++)
    {
    Particle * nucleon = nucleus.getNucleonAt(iNucleon);
    nucleon->shift(shift);
    }

}

void NucleusGenerator::generate(double & r, double & cosTheta, double & phi)
{
  cosTheta = -1 + 2.0*gRandom->Rndm();
  phi      = 2.0*3.1415927*gRandom->Rndm();
  r        = rProfile->GetRandom();
  rProfileGen->Fill(r);
}

void NucleusGenerator::saveHistograms()
{
  rDensity->Write();
  rProfile->Write();
  rProfileGen->Write();
}
