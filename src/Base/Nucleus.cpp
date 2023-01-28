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
#include "Nucleus.hpp"
using CAP::Particle;
using CAP::Nucleus;

ClassImp(Nucleus);

Nucleus::Nucleus()
:
Particle(),
nProtons(0),
nNeutrons(0)
{
  type = ParticleType::getNucleusType();
  children.push_back(Particle::getProton());
  nProtons  = 1;
  nNeutrons = 0;
  live      = false;
}

Nucleus::Nucleus(const Nucleus & otherNucleus)
:
Particle(otherNucleus),
nProtons(otherNucleus.nProtons),
nNeutrons(otherNucleus.nNeutrons)
{
 // no ops
}

Nucleus & Nucleus::operator=(const Nucleus & otherNucleus)
{
  if (&otherNucleus != this)
    {
    Particle::operator=(otherNucleus);
    nProtons  = otherNucleus.nProtons;
    nNeutrons = otherNucleus.nNeutrons;
    }
  return *this;
}

// create the container but do not assign any positions or properties.
void Nucleus::defineAs(unsigned int z, unsigned int a)
{
//  cout << "Nucleus::defineAs(unsigned int z, unsigned int a)" << endl;
//  cout << " z = " << z << endl;
//  cout << " a = " << z << endl;
  live       = false;
  pid        = -1;
  ixEtaPhi   = -1;
  ixYPhi     = -1;
  momentum.SetPxPyPzE (0.0,0.0,0.0,0.0);
  position.SetXYZT    (0.0,0.0,0.0,0.0);
  truth = nullptr;
  type = ParticleType::getNucleusType();
  nProtons  = 0;
  nNeutrons = 0;
  children.clear();
  for (unsigned int iNucleon=0; iNucleon<z; iNucleon++)
    {
    children.push_back(Particle::getProton()); nProtons++;
    }
  for (unsigned int iNucleon=0; iNucleon<(a-z); iNucleon++)
    {
    children.push_back(Particle::getNeutron()); nNeutrons++;
    }
}

void Nucleus::clear()
{
  Particle::clear();
  nProtons  = 0;
  nNeutrons = 0;
}

// Keeps the nucleus defined but resets all the positions.
void Nucleus::reset()
{
  momentum.SetPxPyPzE (0.0,0.0,0.0,0.0);
  position.SetXYZT    (0.0,0.0,0.0,0.0);
  parents.clear();

  for (unsigned int iNucleon=0; iNucleon<children.size(); iNucleon++)
    {
    children[iNucleon]->reset();
    children[iNucleon]->setWounded(false);;
    }
  live = true;
}

unsigned int Nucleus::countWounded()
{
  unsigned int wounded = 0;
  for (unsigned int iNucleon=0; iNucleon<children.size(); iNucleon++)
    {
    wounded += children[iNucleon]->isWounded();
    }
  return wounded;
}

vector<Particle*> Nucleus::getWoundedNucleons()
{
  vector<Particle*> woundedNucleons;
  for (unsigned int iNucleon=0; iNucleon<children.size(); iNucleon++)
    {
    Particle * nucleon = children[iNucleon];
     if (nucleon->isWounded())
       {
       woundedNucleons.push_back(nucleon);
       }
    }
  return woundedNucleons;
}
