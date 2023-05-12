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

ClassImp(NucleusType);

NucleusType::NucleusType()
:
ParticleType(),
z(0),
a(0),
s(0),
beta(0)
{}

NucleusType::NucleusType(int _z, int _a,
                         const TString & _name,
                         const TString & _title,
                         double _mass,
                         int _gSpin,
                         int _gIsospin)
:
ParticleType(_a*10000000+_z*10000, _name, _title, _mass, 0.0, _gSpin, _a, 0, 0, 0, _gIsospin, _z),
z(_z),
a(_a),
s(0),
beta(0.0)
{
}

NucleusType::NucleusType(int _z, int _a, int _s,
                         const TString & _name,
                         const TString & _title,
                         double _mass,
                         int _gSpin,
                         int _gIsospin)
:
ParticleType(_a*10000000+_z*10000+_s*10, _name, _title, _mass, 0.0, _gSpin, _a, _s, 0, 0, _gIsospin, _z),
z(_z),
a(_a),
s(_s),
beta(0.0)
{
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



NucleusType * NucleusType::protonType = new NucleusType(1,1, "Proton", "p",NucleusType::protonMass, 2, 2);
NucleusType * NucleusType::neutronType  = new NucleusType(0,1, "Neutron", "n",NucleusType::protonMass, 2, 2);;
NucleusType * NucleusType::deuteronType = new NucleusType(1,2, "Deuteron", "d",NucleusType::deuteronMass, 2, 1);
NucleusType * NucleusType::tritonType = new NucleusType(1,3, "Triton", "t",NucleusType::tritonMass, 2, 2);
NucleusType * NucleusType::he3Type = new NucleusType(2,3, "Helium3", "3He",NucleusType::helium3Mass, 2, 2);
NucleusType * NucleusType::he4Type = new NucleusType(2,4, "Helium4", "4He",NucleusType::helium4Mass, 2, 1);
NucleusType * NucleusType::auType  = new NucleusType(79,197, "Gold", "Au",NucleusType::au197Mass, 3, 1);
NucleusType * NucleusType::pbType  = new NucleusType(82,208, "Lead", "Pb",NucleusType::pb208Mass, 2, 1);


