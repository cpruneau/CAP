/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * Copyright (C) 2016, Chun Shen
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <TMath.h>
#include "Math/SpecFunc.h"
#include "ParticleType.hpp"
#include "SelectionGenerator.hpp"
#include "ParticleTypeCollection.hpp"

ClassImp(ParticleType);

ParticleType::ParticleType()
:
name(),
title(),
privateCode(-1),
pdgCode(-1),
mass(0.0),
width(0.0),
gSpin(0.0),
gIsospin(0.0),
baryon(0.0),
strange(0.0),
charm(0.0),
bottom(0.0),
leptonElectron(0),
leptonMuon(0),
leptonTau(0),
charge(0.0),
stable(0),
weakStable(0),
statistics(0),
decayModes(),
decayRndmSelector(nullptr)
{}

ParticleType::ParticleType(int pdgCode_in, const TString & name_in, const TString & title_in,
                           double mass_in,
                           double width_in, int gSpin_in, int baryon_in, int strange_in,
                           int charm_in, int bottom_in, int gIsospin_in, int charge_in)
:
name(name_in),
title(title_in),
privateCode(-1),    // Code used in this package only (convenience)
pdgCode(pdgCode_in),
mass(mass_in),
width(width_in),
gSpin(gSpin_in),
gIsospin(gIsospin_in),
baryon(baryon_in),
strange(strange_in),
charm(charm_in),
bottom(bottom_in),
leptonElectron(0),
leptonMuon(0),
leptonTau(0),
charge(charge_in),
stable(true),
weakStable(true),
statistics(0),
decayModes(),
decayRndmSelector(nullptr)
{
  // for now, we do not differentiate between stable and weakStable
  if (width<1.0E-4)
    {
    stable     = true;
    weakStable = true;
    }
  if (abs(baryon)==1 ||
      abs(leptonElectron)==1 ||
      abs(leptonMuon)==1 ||
      abs(leptonTau)==1)
    {
    statistics = 1;  // Fermi-Dirac
    }
  else
    {
    statistics = -1;  // Bose-Einstein
    }
}

ParticleType::~ParticleType()
{
  delete decayRndmSelector;
}

ParticleType::ParticleType(const ParticleType & source)
:
name(source.name),
title(source.title),
privateCode(source.privateCode),
pdgCode(source.pdgCode),
mass(source.mass),
width(source.width),
gSpin(source.gSpin),
gIsospin(source.gIsospin),
baryon(source.baryon),
strange(source.strange),
charm(source.charm),
bottom(source.bottom),
leptonElectron(source.leptonElectron),
leptonMuon(source.leptonMuon),
leptonTau(source.leptonTau),
charge(source.charge),
stable(source.stable),
weakStable(source.weakStable),
statistics(source.statistics), // determine Bose/Fermi statistic for ParticleType
decayModes(source.decayModes),
decayRndmSelector(source.decayRndmSelector)
{
// no ops
}

ParticleType & ParticleType::operator=(const ParticleType & source)
{
  if (this!=&source)
    {
    pdgCode  =  source.pdgCode;
    name     =  source.name;
    title    =  source.title;
    mass     =  source.mass;
    width    =  source.width;
    gSpin    =  source.gSpin;
    gIsospin =  source.gIsospin;
    baryon   =  source.baryon;
    strange  =  source.strange;
    charm    =  source.charm;
    bottom   =  source.bottom;
    leptonElectron =  source.leptonElectron;
    leptonMuon     =  source.leptonMuon;
    leptonTau      =  source.leptonTau;
    charge         =  source.charge;
    stable         =  source.stable;
    weakStable     =  source.weakStable;
    statistics  =  source.statistics;
    decayModes     =  source.decayModes;
    decayRndmSelector    = source.decayRndmSelector;
    }
  return *this;
}


void ParticleType::addDecayMode(double branchingRatio,
                                std::vector<int> children)
{
  ParticleDecayMode decayMode;
  decayMode.setBranchingRatio(branchingRatio);
  for (int k=0; k<int(children.size()); k++)
  {
  decayMode.addChild(children[k]);
  }
  decayModes.push_back(decayMode);
  if (decayModes.size() > 1)
    {
    stable     = false;
    weakStable = false;
    }
  else
    {
    if (pdgCode != decayModes[0].getChildPdgCode(0))
      {
      stable     = false;
      weakStable = false;
      }
    }
}

void ParticleType::addDecayMode(ParticleDecayMode &decayMode)
{
  decayModes.push_back(decayMode);
  if (decayModes.size() > 1)
    {
    stable     = false;
    weakStable = false;
    }
  else
    {
    if (pdgCode != decayModes[0].getChildPdgCode(0))
      {
      stable = false;
      weakStable = false;
      }
    }
}


// ======================================================================
// Return the corresponding anti-ParticleType' Monte-Carlo value
// ======================================================================
int ParticleType::getAntiParticlePdgCode() const
{
  if (baryon == 0 && charge == 0 && strange == 0)
    return(pdgCode);
  else
    return(-pdgCode);
}

void ParticleType::setupDecayGenerator()
{
  int nDecayModes = decayModes.size();
  vector<double> decayBranchingRatios;
  for (int k=0; k<nDecayModes; k++)
  {
  decayBranchingRatios.push_back(decayModes[k].getBranchingRatio());
  }
  decayRndmSelector = new SelectionGenerator(decayBranchingRatios);
}

ParticleDecayMode & ParticleType::generateDecayMode()
{
  if (!decayRndmSelector)
    {
    exit(0);
    }
  int index = decayRndmSelector->generate();
  if (index<0)
    {
    exit(0);
    }
  else if (index>= int(decayModes.size()))
    {
    index = decayModes.size()-1;
    }
  return decayModes[index];
}

ostream & ParticleType::printProperties(ostream & os)
{
  //scientific
  os <<  fixed << setw(10) << setprecision(8) << pdgCode;         // Monte-Carlo number according PDG
  os <<  setw(20) << name;   // ParticleType name
  os <<  setw(20) << title;   // ParticleType title
  os <<  scientific << setw(20) << setprecision(4) <<  mass;        // ParticleType mass (GeV)
  os <<  scientific << setw(20) << setprecision(4) <<  width;       // decay width
  os <<  fixed << setw(7) << setprecision(3) <<  gSpin;          // spin degeneracy
  os <<  fixed << setw(5) << setprecision(3) <<  baryon;         // baryon number
  os <<  fixed << setw(5) << setprecision(3) <<  strange;        // strangeness
  os <<  fixed << setw(5) << setprecision(3) <<  charm;          // charmness
  os <<  fixed << setw(5) << setprecision(3) <<  bottom;         // bottomness
  os <<  fixed << setw(5) << setprecision(3) <<  gIsospin;       // isospin degeneracy
  os <<  fixed << setw(5) << setprecision(3) <<  charge;         // charge
  os <<  fixed << setw(5) << setprecision(6) <<  decayModes.size();         // amount of decays listed for this resonance
  os <<  fixed << setw(5) << setprecision(3) <<  stable;         // defines whether this ParticleType is stable
  os <<  fixed << setw(5) << setprecision(3) <<  weakStable;         // defines whether this ParticleType is stable
  os <<  fixed << setw(5) << setprecision(3) <<  statistics;
  os <<  endl;
  for (int k=0;k<int(decayModes.size());k++)
  {
    if (pdgCode != decayModes[0].getChildPdgCode(0))
      {
        cout << "                     " << k << "    ";
        decayModes[k].printProperties(os);
      }
  }
  return os;
}

ostream & ParticleType::printDecayProperties(ostream & os)
{
  TString latex;
  latex = "$";
  latex += title;
  latex += "$";
  os <<  " PDG:" << fixed << setw(10) << setprecision(8) << pdgCode;         // Monte-Carlo number according PDG
  os <<  setw(20) << latex;
  os <<  " $m=$ " << scientific << setw(20) << setprecision(4) <<  mass << " GeV/$c^2$";
  os <<  " $#Gamma=$ " << scientific << setw(20) << setprecision(4) <<  width << " GeV/$c^2$ \\\\";       // decay width
  os <<  endl;
  for (int k=0;k<int(decayModes.size());k++)
  {
    if (pdgCode != decayModes[0].getChildPdgCode(0))
      {
      os << "        Br(xx)= " << scientific << setw(20) << setprecision(4) << (100.0*decayModes[k].getBranchingRatio()) << "    ";
      os << "   $"<< title << "#rightarrow ";
      int nChildren = decayModes[k].getNChildren();
      for (int iChild=0; iChild<nChildren; iChild++)
        {
        ParticleType & type = decayModes[k].getChildType(iChild);
        os << type.getTitle();
        if (iChild < nChildren-1)  os << "+";
        }
      os << "$ \\\\"<< endl;
      }
  }
  return os;
}

ParticleType * ParticleType::decayModeType     = nullptr;
ParticleType * ParticleType::interactionType   = nullptr;
ParticleType * ParticleType::protonProtonInteractionType   = nullptr;
ParticleType * ParticleType::protonNeutronInteractionType  = nullptr;
ParticleType * ParticleType::neutronNeutronInteractionType = nullptr;
ParticleType * ParticleType::protonType  = nullptr;
ParticleType * ParticleType::neutronType = nullptr;
ParticleType * ParticleType::nucleusType = nullptr;

ParticleType * ParticleType::getDecayModeType()
{
  if (decayModeType==nullptr)
    {
    decayModeType = new ParticleType();
    decayModeType->setPdgCode(1000001);
    decayModeType->setName("decayMode");
    decayModeType->setTitle("decay mode");
    }
  return decayModeType;
}

ParticleType * ParticleType::getInteractionType()
{
  if (interactionType==nullptr)
    {
    interactionType = new ParticleType();
    interactionType->setPdgCode(1000010);
    interactionType->setName("int");
    interactionType->setTitle("int");
    }
  return interactionType;
}

ParticleType * ParticleType::getPPInteractionType()
{
  if (protonProtonInteractionType==nullptr)
    {
    protonProtonInteractionType = new ParticleType();
    protonProtonInteractionType->setPdgCode(1000011);
    protonProtonInteractionType->setName("pp");
    protonProtonInteractionType->setTitle("pp");
    }
  return protonProtonInteractionType;
}


ParticleType * ParticleType::getPNInteractionType()
{
  if (protonNeutronInteractionType==nullptr)
    {
    protonNeutronInteractionType = new ParticleType();
    protonNeutronInteractionType->setPdgCode(1000012);
    protonNeutronInteractionType->setName("pn");
    protonNeutronInteractionType->setTitle("pn");
    }
  return protonNeutronInteractionType;
}

ParticleType * ParticleType::getNNInteractionType()
{
  if (neutronNeutronInteractionType==nullptr)
    {
    neutronNeutronInteractionType = new ParticleType();
    neutronNeutronInteractionType->setPdgCode(1000013);
    neutronNeutronInteractionType->setName("nn");
    neutronNeutronInteractionType->setTitle("nn");
    }
  return neutronNeutronInteractionType;
}


ParticleType * ParticleType::getProtonType()
{
  if (protonType==nullptr)
    {
    protonType = ParticleTypeCollection::getMasterParticleCollection()->findPdgCode(2212);
    }
  return protonType;
}

ParticleType * ParticleType::getNeutronType()
{
  if (neutronType==nullptr)
    {
    neutronType = ParticleTypeCollection::getMasterParticleCollection()->findPdgCode(2112);
    }
  return neutronType;
}

ParticleType * ParticleType::getNucleusType()
{
  if (nucleusType==nullptr)
    {
    nucleusType = new ParticleType();
    nucleusType->setPdgCode(1000020);
    nucleusType->setName("Nucleus");
    nucleusType->setTitle("Nucleus");
    }
  return nucleusType;
}

