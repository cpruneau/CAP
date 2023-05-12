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
using CAP::NucleusGenerator;

ClassImp(NucleusGenerator);

NucleusGenerator::NucleusGenerator(const String & _name,
                                   const Configuration & _configuration)
:
EventTask(_name, _configuration),
gType(0),
nR(0),
minR(0),
maxR(0),
parA(0),
parB(0),
parC(0),
useRecentering(true),
useNucleonExclusion(true),
exclusionRadius(0.4), // fm
exclusionRadiusSq(0.4*0.4),
rDensity(nullptr),
rProfile(nullptr),
rProfileGen(nullptr)
{
}

NucleusGenerator::~NucleusGenerator()
{
  if (rDensity)    delete rDensity;
  if (rProfile)    delete rProfile;
  if (rProfileGen) delete rProfileGen;
}


void NucleusGenerator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  addParameter("UseParticles",    true);
  addParameter("EventsUseStream0", true);
  addParameter("generatorType",    0);
  addParameter("nRadiusBins",    100);
  addParameter("MinimumRadius",  0.0);
  addParameter("MaximumRadius", 10.0);
  addParameter("parA", 0);
  addParameter("parB", 0);
  addParameter("parC", 0);
  addParameter("useRecentering",     true);
  addParameter("useNucleonExclusion",true);
  addParameter("exclusionRadius",     0.4);
}

// create the container but do not assign any positions or properties.
void NucleusGenerator::initialize()
{
  
  if (reportStart(__FUNCTION__))
;
  gType = configuration.getValueInt(getName(),"generatorType");
  nR    = configuration.getValueInt(getName(),"nRadiusBins");
  minR  = configuration.getValueDouble(getName(),"MinimumRadius");
  maxR  = configuration.getValueDouble(getName(),"MaximumRadius");
  parA  = configuration.getValueDouble(getName(),"parA");
  parB  = configuration.getValueDouble(getName(),"parB");;
  parC  = configuration.getValueDouble(getName(),"parC");
  useRecentering      = configuration.getValueInt(getName(),"useRecentering");
  useNucleonExclusion = configuration.getValueInt(getName(),"useNucleonExclusion");
  exclusionRadius     = configuration.getValueInt(getName(),"exclusionRadius"); // fm
  exclusionRadiusSq   = exclusionRadius*exclusionRadius;
  double dr = (maxR-minR)/double(nR);
  double r  = minR + dr/2.0;
  double density;
  double profile;

  if (rDensity)    delete rDensity;    rDensity    = nullptr;
  if (rProfile)    delete rProfile;    rProfile    = nullptr;
  if (rProfileGen) delete rProfileGen; rProfileGen = nullptr;

  String baseName;
  String histoName;
  switch (gType)
    {
      case 0: baseName += "_Uniform";        break;
      case 1: baseName += "_WoodsSaxon";     break;
      case 2: baseName += "_Exponential";    break;
      case 3: baseName += "_Gaussian";       break;
      case 4: baseName += "_DoubleGaussian"; break;
    }


  histoName   = baseName + "_rDensity";
  rDensity    = new TH1D(histoName,histoName,nR,minR, maxR);
  histoName   = baseName + "_rProfile";
  rProfile    = new TH1D(histoName,histoName,nR,minR, maxR);
  histoName   = baseName + "_rProfileGen";
  rProfileGen = new TH1D(histoName,histoName,nR,minR, maxR);


  for (int iR=0; iR<nR; iR++)
    {
    double r2 = r*r;
    switch (gType)
      {
        case 0: // uniform hard sphere
        density = (r*r*r<parA)? 1.0 : 0.0;
        break;
        case 1: // Woods-Saxon/Fermi
        density = 1.0/(1.0+exp((r-parA)/parB) );
        break;
        case 2: // exponential
        density = exp(-r/parA);
        break;
        case 3: // gaussian
        density = exp(-r2/2.0/parA/parA);
        break;
        case 4: //double-gaussian
        density =  (1.0-parC)*exp(-r2/parA/parA)/parA/parA/parA;
        density += parC*exp(-r2/parB/parB)/parB/parB/parB;
        break;
      }
    profile = r2*density;
    rDensity->SetBinContent(iR+1,density);
    rDensity->SetBinError(iR+1,0.0);
    rProfile->SetBinContent(iR+1,profile);
    rProfile->SetBinError(iR+1,0.0);
    r += dr;
    }
  if (reportEnd(__FUNCTION__))
;
}

void NucleusGenerator::createEvent()
{
// this should generate the nucleus. 
}

void NucleusGenerator::generate(Nucleus & nucleus, double xShift)
{
  double r, cosTheta, phi;
  LorentzVector position(0.0,0.0,0.0,0.0);
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
  LorentzVector shift(xShift -position.X(),-position.Y(),-position.Z(),0.0);

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

void NucleusGenerator::exportHistograms()
{
  rDensity->Write();
  rProfile->Write();
  rProfileGen->Write();
}
