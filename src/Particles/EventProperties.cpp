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
#include "EventProperties.hpp"
using CAP::EventProperties;

ClassImp(EventProperties);


EventProperties::EventProperties()
:
zProjectile(0),
aProjectile(0),
nPartProjectile(0),
zTarget(0),
aTarget(0),
nPartTarget(0),
nParticipantsTotal(0),
nBinaryTotal(0),
impactParameter(0),
fractionalXSection(0),
refMultiplicity(0),
other(0),
nFilters(0),
nFiltered(),
eFiltered(),
qFiltered(),
sFiltered(),
bFiltered(),
s0Filtered(),
s1Filtered()
{
// no ops
}

EventProperties::EventProperties(const EventProperties & source)
:
zProjectile(source.zProjectile),
aProjectile(source.aProjectile),
nPartProjectile(source.nPartProjectile),
zTarget(source.zTarget),
aTarget(source.aTarget),
nPartTarget(source.nPartTarget),
nParticipantsTotal(source.nParticipantsTotal),
nBinaryTotal(source.nBinaryTotal),
impactParameter(source.impactParameter),
fractionalXSection(source.fractionalXSection),
refMultiplicity(source.refMultiplicity),
other(source.other),
nFilters(source.nFilters),
nFiltered(source.nFiltered),
eFiltered(source.eFiltered),
qFiltered(source.qFiltered),
sFiltered(source.sFiltered),
bFiltered(source.bFiltered),
s0Filtered(source.s0Filtered),
s1Filtered(source.s1Filtered)
{
// no ops
}

EventProperties & EventProperties::operator=(const EventProperties & source)
{
  if (this!=&source)
    {
    zProjectile        =  source.zProjectile;
    aProjectile        =  source.aProjectile;
    nPartProjectile    =  source.nPartProjectile;
    zTarget            =  source.zTarget;
    aTarget            =  source.aTarget;
    nPartTarget        =  source.nPartTarget;
    nParticipantsTotal =  source.nParticipantsTotal;
    nBinaryTotal       =  source.nBinaryTotal;
    impactParameter    =  source.impactParameter;
    fractionalXSection =  source.fractionalXSection;
    refMultiplicity    =  source.refMultiplicity;
    other              =  source.other;
    nFilters           =  source.nFilters;
    nFiltered          =  source.nFiltered;
    eFiltered          =  source.eFiltered;
    qFiltered          =  source.qFiltered;
    sFiltered          =  source.sFiltered;
    bFiltered          =  source.bFiltered;
    s0Filtered         =  source.s0Filtered;
    s1Filtered         =  source.s1Filtered;
    }
  return *this;
}

void EventProperties::clear()
{
  zProjectile        = 0;
  aProjectile        = 0;
  nPartProjectile    = 0;
  zTarget            = 0;
  aTarget            = 0;
  nPartTarget        = 0;
  nParticipantsTotal = 0;
  nBinaryTotal       = 0;
  impactParameter    = 0;
  fractionalXSection = 0;
  refMultiplicity    = 0;
  other              = 0;
  nFilters           = 0;
  nFiltered.clear();
  eFiltered.clear();
  qFiltered.clear();
  sFiltered.clear();
  bFiltered.clear();
  s0Filtered.clear();
  s1Filtered.clear();
}

void EventProperties::reset()
{
  zProjectile        = 0;
  aProjectile        = 0;
  nPartProjectile    = 0;
  zTarget            = 0;
  aTarget            = 0;
  nPartTarget        = 0;
  nParticipantsTotal = 0;
  nBinaryTotal       = 0;
  impactParameter    = 0;
  fractionalXSection = 0;
  refMultiplicity    = 0;
  other              = 0;
  nFilters           = 0;
  nFiltered.clear();
  eFiltered.clear();
  qFiltered.clear();
  sFiltered.clear();
  bFiltered.clear();
  s0Filtered.clear();
  s1Filtered.clear();
}

void EventProperties::fill(vector<double> & n,  // number of particles accepted by filter #i
                           vector<double> & ptSum,  // total pT of particles accepted by filter #i
                           vector<double> & e,  // total energy of particles accepted by filter #i
                           vector<double> & q,  // total charge of particles accepted by filter #i
                           vector<double> & s,  // total strangeness of particles accepted by filter #i
                           vector<double> & b)
{
  int nFilters = n.size();
  for (int iFilter=0; iFilter<nFilters; iFilter++)
    {
    nFiltered.push_back(    n[iFilter] );
    ptSumFiltered.push_back(ptSum[iFilter] );
    eFiltered.push_back(    e[iFilter] );
    qFiltered.push_back(    q[iFilter] );
    sFiltered.push_back(    s[iFilter] );
    bFiltered.push_back(    b[iFilter] );
    }
}

void EventProperties::fillSpherocity(vector<double> & s0, vector<double> & s1)
{
  for (unsigned int iFilter=0; iFilter<s0.size(); iFilter++) s0Filtered.push_back( s0[iFilter] );
  for (unsigned int iFilter=0; iFilter<s1.size(); iFilter++) s1Filtered.push_back( s1[iFilter] );
}


void EventProperties::printProperties(ostream & output)
{
  nFilters = nFiltered.size();
  output << "===============================================" << endl;
  output << "Event Properties" << endl;
  output << "===============================================" << endl;
  output << "           zProjectile : " << zProjectile << endl;     // atomic number projectile
  output << "           aProjectile : " << aProjectile << endl;     // mass number projectile
  output << "       nPartProjectile : " << nPartProjectile << endl; // number of participants  projectile
  output << "               zTarget : " << zTarget << endl;         // atomic number target
  output << "               aTarget : " << aTarget << endl;         // mass number target
  output << "           nPartTarget : " << nPartTarget << endl;     // number of participants  target
  output << "    nParticipantsTotal : " << nParticipantsTotal << endl;      // total number of participants
  output << "          nBinaryTotal : " << nBinaryTotal << endl;    // total number of binary collisions
  output << "       impactParameter : " << impactParameter << endl; // nucleus-nucleus center distance in fm
  output << "    fractionalXSection : " << fractionalXSection << endl;      // fraction cross section value
  output << "       refMultiplicity : " << refMultiplicity << endl;    // nominal multiplicity in the reference range
  output << "                 other : " << other << endl;           // other value of interest
  output << "       particlesCounted: " << particlesCounted << endl;
  output << "      particlesAccepted: " << particlesAccepted << endl;
  output << "              nFilters : " << nFilters << endl;
  for (int k=0;k<nFilters;k++)
  {
  output 
    << setw(5) << k << " "
  << scientific << setw(15)<< setprecision(5) << nFiltered[k]
  << scientific << setw(15)<< setprecision(5) << ptSumFiltered[k]
  << scientific << setw(15)<< setprecision(5) << eFiltered[k]
  << scientific << setw(15)<< setprecision(5) << qFiltered[k]
  << scientific << setw(15)<< setprecision(5) << sFiltered[k]
  << scientific << setw(15)<< setprecision(5) << bFiltered[k]
  << scientific << setw(15)<< setprecision(5) << s0Filtered[k]
  << scientific << setw(15)<< setprecision(5) << s1Filtered[k] << endl;
  }
}
