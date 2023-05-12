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

#include "NucleusType.hpp"
using CAP::NucleusType;

ClassImp(NucleusType);

NucleusType::NucleusType()
:
ParticleType(),
beta(0)
{}

NucleusType::NucleusType(const String & _name,
                         const String & _title,
                         int    _atomicNumber,
                         int    _massNumber,
                         int    _strangenessNumber,
                         double _mass,
                         double _spin,
                         double _isospin,
                         double _lifeTime,
                         double _deformationBeta)
:
ParticleType(),
beta(_deformationBeta)
{
  setName(_name);
  setTitle(_title);
  setCharge(_atomicNumber);
  setMass(_mass);
  setSpin(_spin);
  setIsospin(_isospin);
  setLifeTime(_lifeTime);
  if (_massNumber>0)
    setNumberQ(3.0*(_massNumber-_strangenessNumber));
  else
    setNumberAQ(3.0*fabs(_massNumber-_strangenessNumber));
  if (_strangenessNumber>0)
    setNumberS(_strangenessNumber);
  else
    setNumberAS(fabs(_strangenessNumber));
}


//!
//! Assignment operator. Copies all the properties of the source particle into this.
//!
NucleusType & NucleusType::operator=(const NucleusType & source)
{
  ParticleType::operator=(source);
  if (this!=&source)
    {
    beta = source.beta;
    }
  return *this;
}

//!
//! Get atomic number
//!
double NucleusType::getZ() const
{
  return getAtomicNumber();
}

//!
//! Get atomic number
//! Must contain at least one proton or antiproton
//!
double NucleusType::getAtomicNumber() const
{
  if (isBaryon())
    return fabs(charge);
  else
    return 0;
}

//!
//! Get mass number
//!
double NucleusType::getA() const
{
  return getMassNumber();
}

//!
//! Get mass number
//!  Must contain at least one proton or antiproton
//!
double NucleusType::getMassNumber() const
{
  return fabs(getBaryonNumber()) && charge!=0;
}

//!
//! Get neutron number
//!
double NucleusType::getN() const
{
  double z = getAtomicNumber();
  double a = getMassNumber();
  return a-z;
}

//!
//! Get neutron number
//!
double NucleusType::getNeutronNumber() const
{
  double z = getAtomicNumber();
  double a = getMassNumber();
  return a-z;
}

double NucleusType::getBeta() const
{
  return beta;
}

double NucleusType::getDeformationBeta() const
{
  return beta;
}

void NucleusType::setBeta(double _deformationBeta)
{
  beta = _deformationBeta;
}

void NucleusType::setDeformationBeta(double _deformationBeta)
{
  beta = _deformationBeta;
}

//!
//! Get the binding  of this nucleus  in GeV/c^2.
//!
double NucleusType::getBindingEnergy() const
{
  double z = fabs(charge);
  double a = getMassNumber();
  double bindingE = mass - fabs(z*protonMass + (a-z)*neutronMass);
  return bindingE;
}

//!
//! Return true if the object is a nucleus
//! i.e., has at least one proton.
//!
bool NucleusType::isNucleus() const
{
  double a = getMassNumber();
  return fabs(a)>0 && fabs(charge)>0;
}

bool NucleusType::isAntiNucleus() const
{
  return charge<0;
}


//!
//! 1 dalton = 1 Da = 1 u  = 1.66053906660(50)×10−27 kg
//!
double NucleusType::amu_kg = 5.6095861672249E+29;

//!
//! 1 dalton = 1 Da = 1 u  = 1.66053906660(50)×10−27 kg
//!
double NucleusType::amu_GeV = 0.9314941024228;


//!
//! 1 Kilogram = 5.6095861672249E+29 MeV/c2
//!
double NucleusType::kilogramToMeV = 5.6095861672249E+29;

//!
//! Proton Mass:
//!  1.67262192369(51)×10−27 kg
//!  938.27208816(29) MeV/c2
//!
double NucleusType::protonMass = 0.9382720881629;

//!
//! Neutron Mass:
//! 1.67492749804(95)×10−27 kg
//! 939.56542052(54) MeV/c2
//!
double NucleusType::neutronMass = 0.9395654205254;

//!
//!Deuteron Mass:
//! 2.013553212745(40) u
//! 1875.61292812  MeV/c2
//!
double NucleusType::deuteronMass = 1.87561292812;

//!
//!Triton Mass:
//! 5.007 356 7446 x 10-27 kg
//!
double NucleusType::tritonMass = 5.0073567446E-27 * kilogramToMeV/1000.0;

//!
//!helium3 Mass:
//! 3.0160293 u
//!
double NucleusType::helium3Mass = 3.0160293 * NucleusType::amu_GeV;

//!
//!helium4 Mass:
//! 4.00260 u
//!
double NucleusType::helium4Mass  = 4.00260 * NucleusType::amu_GeV;

//!
//!Gold 197
//!196.966552 u
//!
double NucleusType::au197Mass   = 196.966552 * NucleusType::amu_GeV;


//!
//!Lead 208
//!196.966552 u
//!
double NucleusType::pb208Mass = 207.97665 * NucleusType::amu_GeV;

NucleusType * NucleusType::protonType   = new NucleusType("Proton",   "p",  1, 1,   0, NucleusType::protonMass,   0.5,  0.5, 1.0E34, 0.0);
NucleusType * NucleusType::neutronType  = new NucleusType("Neutron",  "n",  0, 1,   0, NucleusType::protonMass,   0.5,  0.5, 1.0E34, 0.0);
NucleusType * NucleusType::deuteronType = new NucleusType("Deuteron", "d",  1, 2,   0, NucleusType::deuteronMass, 0.5,  0.5, 1.0E34, 0.0);
NucleusType * NucleusType::tritonType   = new NucleusType("Triton",   "t",  1, 3,   0, NucleusType::tritonMass,   0.5,  0.5, 1.0E34, 0.0);
NucleusType * NucleusType::he3Type      = new NucleusType("Helium3",  "3He",2, 3,   0, NucleusType::helium3Mass,  0.5,  0.5, 1.0E34, 0.0);
NucleusType * NucleusType::he4Type      = new NucleusType("Helium4",  "4He",2, 4,   0, NucleusType::helium4Mass,  0.5,  0.5, 1.0E34, 0.0);
NucleusType * NucleusType::auType       = new NucleusType("Gold",     "Au", 79,197, 0, NucleusType::au197Mass,    0.5,  0.5, 1.0E34, 0.0);
NucleusType * NucleusType::pbType       = new NucleusType("Lead",     "Pb", 82,208, 0, NucleusType::pb208Mass,    0.5,  0.5, 1.0E34, 0.0);


