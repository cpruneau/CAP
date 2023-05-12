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
#include "ParticleDb.hpp"
using CAP::ParticleType;
using CAP::ParticleDecayMode;

ClassImp(CAP::Collection<CAP::ParticleType>);
ClassImp(ParticleType);

//!
//! Default CTOR
//!
ParticleType::ParticleType()
:
name(),
title(),
privateCode(-1),
pdgCode(-1),
mass(-1),
charge(0.0),
width(-1),
lifeTime(1.0E37),
spin(-1.0E100),
isospin(-1.0E100),
isospin3(-1.0E100),
nq(-1),
naq(-1),
ns(-1),
nas(-1),
nc(-1),
nac(-1),
nb(-1),
nab(-1),
baryonNumber(0.0),
strangeNumber(0.0),
charmNumber(0.0),
bottomNumber(0.0),
topNumber(0.0),
leptonElectron(0),
leptonMuon(0),
leptonTau(0),
stable(0),
weakStable(0),
gSpin(0.0),
gIsospin(0.0),
statistics(0),
decayModes(),
decayRndmSelector()
{}

//!
//! Detailed CTOR
//!
//! @param monval_in :
//! @param name_in : name of the particle (string)
//! @param title_in : name (title) of the particle displayed as a Latex word.
//! @param mass_in : mass of the particle (GeV/c^2)
//! @param width_in : width of the particle (GeV/c^2)
//! @param gSpin_in : spin degeneracy
//! @param baryon_in : baryon number
//! @param strange_in : net strangeness of the particle
//! @param charm_in : net charm of the particle
//! @param bottom_in : net bottomness of the particle
//! @param gIsospin_in : isospin generacy of the particle
//! @param charge_in : electric charge (|e| unit)
//!
ParticleType::ParticleType(int pdgCode_in, const String & name_in, const String & title_in, double mass_in,
                           double width_in, int gSpin_in, int baryon_in, int strange_in,
                           int charm_in, int bottom_in, int gIsospin_in, int charge_in)
:
name(name_in),
title(title_in),
privateCode(-1),    // Code used in this package only (convenience)
pdgCode(pdgCode_in),
mass(mass_in),
charge(charge_in),
width(width_in),
lifeTime( (width_in>0.0) ? (6.582E-25/width_in) : 1.0E37),
spin(-1.0E100),
isospin(-1.0E100),
isospin3(-1.0E100),
nq(0),
naq(0),
ns(0),
nas(0),
nc(0),
nac(0),
nb(0),
nab(0),
baryonNumber(baryon_in),
strangeNumber(strange_in),
charmNumber(charm_in),
bottomNumber(bottom_in),
topNumber(0.0),
leptonElectron(0),
leptonMuon(0),
leptonTau(0),
stable(true),
weakStable(true),
gSpin(gSpin_in),
gIsospin(gIsospin_in),
statistics(0),
decayModes(),
decayRndmSelector()
{
   // for now, we do not differentiate between stable and weakStable
  if (width<1.0E-4)
    {
    stable     = true;
    weakStable = true;
    }
  if (fabs(spin)<10.0)
    statistics = (spin - static_cast<int>(spin) ) < 0.01 ? -1.0 : +1.0;
  else if (abs(baryonNumber)==1 ||
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

//!
//! DTOR
//!
ParticleType::~ParticleType()
{   }

//!
//! COPY CTOR
//!
ParticleType::ParticleType(const ParticleType & source)
:
name(source.name),
title(source.title),
privateCode(source.privateCode),
pdgCode(source.pdgCode),
mass(source.mass),
charge(source.charge),
width(source.width),
lifeTime(source.lifeTime),
spin(source.spin),
isospin(source.isospin),
isospin3(source.isospin3),
nq(source.nq),
naq(source.naq),
ns(source.ns),
nas(source.nas),
nc(source.nc),
nac(source.nac),
nb(source.nb),
nab(source.nab),
baryonNumber(source.baryonNumber),
strangeNumber(source.strangeNumber),
charmNumber(source.charmNumber),
bottomNumber(source.bottomNumber),
leptonElectron(source.leptonElectron),
leptonMuon(source.leptonMuon),
leptonTau(source.leptonTau),
stable(source.stable),
weakStable(source.weakStable),
gSpin(source.gSpin),
gIsospin(source.gIsospin),
statistics(source.statistics), // determine Bose/Fermi statistic for ParticleType
decayModes(source.decayModes),
decayRndmSelector(source.decayRndmSelector)
{
// no ops
}

//!
//! Assignment operator. Copies all the properties of the source particle into this.
//!
ParticleType & ParticleType::operator=(const ParticleType & source)
{
  if (this!=&source)
    {
    pdgCode        =  source.pdgCode;
    name           =  source.name;
    title          =  source.title;
    mass           =  source.mass;
    width          =  source.width;
    lifeTime       =  source.lifeTime;
    spin           =  source.spin;
    isospin        =  source.isospin;
    isospin3       =  source.isospin3;
    nq             =  source.nq;
    naq            =  source.naq;
    ns             =  source.ns;
    nas            =  source.nas;
    nc             =  source.nc;
    nac            =  source.nac;
    nb             =  source.nb;
    nab            =  source.nab;
    baryonNumber   =  source.baryonNumber;
    strangeNumber  =  source.strangeNumber;
    charmNumber    =  source.charmNumber;
    bottomNumber   =  source.bottomNumber;
    leptonElectron =  source.leptonElectron;
    leptonMuon     =  source.leptonMuon;
    leptonTau      =  source.leptonTau;
    charge         =  source.charge;
    stable         =  source.stable;
    weakStable     =  source.weakStable;
    statistics     =  source.statistics;
    gSpin          =  source.gSpin;
    gIsospin       =  source.gIsospin;
    decayModes     =  source.decayModes;
    decayRndmSelector = source.decayRndmSelector;
    }
  return *this;
}

//!
//! Add a decay mode to this particle type based on the given branching fraction (branching ratio) and the given array of children
//! @param branchingRatio : fraction of all decays of this particle type into the given children
//! @param children : array of particle type index corresponding to the particle type this particle decays into.
//!
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

//!
//! Add a decay mode to this particle type based on the given decayModel object
//! @param decayMode : decay mode object encapsulating the branching fraction and the list of particle types this particle decays into.
//!
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


//!
//! Get the PDG code of the antiparticle of this particle.
//!
int ParticleType::getAntiParticlePdgCode() const
{
  if (baryonNumber == 0 && charge == 0 && strangeNumber == 0)
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
  decayRndmSelector.initializeWith(decayBranchingRatios);
}

ParticleDecayMode & ParticleType::generateDecayMode()
{
  int index = decayRndmSelector.generate();
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

int ParticleType::getIndex() const
{
  return index;
}

//!
//! Get the name of the particle
//!
String ParticleType::getName() const;
{
  return name;
}

//!
//! Get the title  of the particle
//!
String ParticleType::getTitle() const;
{
  return title;
}

//!
//! Get the PDG code of this particle.
//!
int ParticleType::getPdgCode() const
{
  return pdgCode;
}

//!
//! Get the private  code of this particle (user defined).
//!
int ParticleType::getPrivateCode() const
{
  return privateCode;
}

//!
//! Get the mass of this particle in GeV/c^2.
//!
double ParticleType::getMass() const
{
  return mass;  // GeV
}

//!
//! Get the mass of this particle in kilograms.
//!
double ParticleType::getMassInKg() const
{
  return mass*1.79E-27;
}

//!
//! Get the decay width of this particle in GeV/c^2.
//!
double ParticleType::getWidth() const
{
  return width; // GeV
}

//!
//! Get the spin of this particle in h-bar units
//!
double ParticleType::getSpin() const
{
  return spin; // hbar units
}


//!
//! Get the mean life time of this particle in seconds.
//!
double ParticleType::getLifeTime() const
{
  return lifeTime;
}

//!
//! Get the baryon number of this particle.
//!
int    ParticleType::getBaryonNumber() const
{
  return (nq+ns+nc+nb+nt - (naq+nas+nac+nab+nat) )/3.0;
}

//!
//! Get the electric charge of this particle.
//!
int    ParticleType::getCharge() const
{
  return charge;
  }

//!
//! Get the net strangeness of this particle.
//!
int    ParticleType::getNetStrangeness() const
{
  return (ns-nas);
}

//!
//! Get the net charm of this particle.
//!
int    ParticleType::getNetCharm() const
{
  return (nc-nac);
}

//!
//! Get the net bottomness  of this particle.
//!
int    ParticleType::getNetBottom() const
{
  return (nb-nab);
}

//!
//! Get the net topness  of this particle.
//!
int    ParticleType::getNetTop() const
{
  return (nt-nat);
}


//!
//! Get the lepton number (electron)  of this particle.
//!
int    ParticleType::getLeptonElectron() const
{ return leptonElectron;}

//!
//! Get the lepton number (muon)  of this particle.
//!
int    ParticleType::getLeptonMuon() const
{ return leptonMuon;}

//!
//! Get the lepton number (tau)  of this particle.
//!
int    ParticleType::getLeptonTau() const
{ return leptonTau;}

//!
//! Get the isopsin  of this particle.
//!
double ParticleType::getIsospin()   const
{ return isospin;}

//!
//! Get the 3rd component of the isopsin  of this particle.
//!
double ParticleType::getIsospin3()  const
{ return isospin3;}

//!
//! Get the number of light quarks (u,d) composing this particle
//!
int    ParticleType::getNumberQ()    const
{ return nq;  }

//!
//! Get the number of light anti-quarks (u-bar,d-bar) composing this particle
//!
int    ParticleType::getNumberAQ()   const
{ return naq; }

//!
//! Get the number of strange quarks (s) composing this particle
//!
int    ParticleType::getNumberS()    const
{ return ns;  }

//!
//! Get the number of anti-strange quarks (s-bar) composing this particle
//!
int    ParticleType::getNumberAS()   const
{ return nas; }

//!
//! Get the number of charm quarks (c) composing this particle
//!
int    ParticleType::getNumberC()    const
{ return nc;  }

//!
//! Get the number of anti-charm quarks (c-bar) composing this particle
//!
int    ParticleType::getNumberAC()   const
{ return nac; }

//!
//! Get the number of bottom quarks (b) composing this particle
//!
int    ParticleType::getNumberB()    const
{ return nb;  }

//!
//! Get the number of anti-bottom quarks (b-bar) composing this particle
//!
int    ParticleType::getNumberAB()    const
{ return nab; }

//!
//! Get the number of bottom quarks (b) composing this particle
//!
int    ParticleType::getNumberT()    const
{ return nt;  }

//!
//! Get the number of anti-bottom quarks (b-bar) composing this particle
//!
int    ParticleType::getNumberAT()    const
{ return nat; }

//!
//! Get the spin degeneracy factor of this particle.
//!
int    ParticleType::getSpinFactor() const
{ return 2.0*spin + 1.0; }

//!
//! Get the isospin degeneracy factor of this particle.
//!
int    ParticleType::getIsoSpinFactor() const
{ return 2.0*isospin + 1.0;   }

//!
//! Get the statistics of this particle used in the expression of the particle density (+ or -).
//!
int    ParticleType::getStatistics() const
{ return statistics; }


void ParticleType::setIndex(int _index)
{
  index = _index;
}

//!
//! Set the name of this particle.
//!
void ParticleType::setName(const String & _name)  { name    = _name;}

//!
//! Set the 'title'' of this particle.
//!
void ParticleType::setTitle(const String & _title){ title   = _title;}

void ParticleType::setSpin(double _spin)
{
  spin = _spin;
}

void ParticleType::setIsospin(double _isospin)
{
  isospin = _isospin;
}

void ParticleType::setIsospin3(double _isospin3)
{
  isospin3 = _isospin3;
}


//!
//! Set the PDG code of this particle.
//!
void ParticleType::setPdgCode(int value)
{
  pdgCode = value;
}

//!
//! Set the private (user defined) code of this particle.
//!
void ParticleType::setPrivateCode(int value)
{
  privateCode = value;
}

//!
//! Set the mass of this particle in GeV/c^2.
//!
void ParticleType::setMass(double _mass)
{
  mass = _mass;
}

//!
//! Set the width of this particle in GeV/c^2.
//!
void ParticleType::setWidth(double _width)
{
  width = _width;
}

void ParticleType::setLifeTime(double _lifeTime)
{
  width = (_lifeTime<1.0E37) ? (6.582E-25/_lifeTime) : 0.0;
}

//!
//! Set the electric charge of this particle.
//!
void ParticleType::setCharge(int value)
{ charge  = value;}

//!
//! Set the electron lepton value of this particle
//!
void ParticleType::setLeptonElectron(int value)
{ leptonElectron = value;}

//!
//! Set the muon lepton value of this particle
//!
void ParticleType::setLeptonMuon(int value)
{ leptonMuon   = value;}

//!
//! Set the tau lepton value of this particle
//!
void ParticleType::setLeptonTau(int value)
{ leptonTau    = value;}

//!
//! Set the spin dengeneracy factor  of this particle.
//!
void ParticleType::setSpinFactor(double _gSpin)
{
  spin  = (_gSpin-1.0)/2.0;
}

//!
//! Set the isospin dengeneracy factor  of this particle.
//!
void ParticleType::setIsoSpinFactor(double _gIsospin)
{
  isospin  = (_gIsospin-1.0)/2.0;
}

void  ParticleType::setNumberQ(int  value) { nq  = value;}
void  ParticleType::setNumberAQ(int value) { naq = value;}
void  ParticleType::setNumberS(int  value) { ns  = value;}
void  ParticleType::setNumberAS(int value) { nas = value;}
void  ParticleType::setNumberC(int  value) { nc  = value;}
void  ParticleType::setNumberAC(int value) { nac = value;}
void  ParticleType::setNumberB(int  value) { nb  = value;}
void  ParticleType::setNumberAB(int value) { nab = value;}

//!
//! Return true if this particle is a Higgs  boson
//!
bool ParticleType::isHiggs() const           { return 0; }

//!
//! Return true if this particle is a fermion
//!
bool ParticleType::isFermion() const         { return statistics==1; }

//!
//! Return true if this particle is a  boson
//!
bool ParticleType::isBoson() const           { return statistics==-1; }

//!
//! Return true if this particle is a gluon
//!
bool ParticleType::isGluon() const           { return pdgCode==21; }

//!
//! Return true if this particle is a photon
//!
bool ParticleType::isPhoton() const          { return pdgCode==22; }

//!
//! Return true if this particle is a Z boson
//!
bool ParticleType::isZBoson() const          { return pdgCode==23; }

//!
//! Return true if this particle is a W boson
//!
bool ParticleType::isWBoson() const          { return pdgCode==24; }

//!
//! Return true if this particle is a Higgs gauge boson
//!
bool ParticleType::isHiggsBoson() const      { return pdgCode==25; }

//!
//! Return true if this particle is a gauge boson
//!
bool ParticleType::isGaugeBoson() const      { return pdgCode==21 || pdgCode==22 || pdgCode==23 || pdgCode==24; }

//!
//! Return true if this particle is a lepton
//!
bool ParticleType::isLepton() const          { return leptonElectron!=0 || leptonMuon!=0 || leptonTau!=0; }

//!
//! Return true if this particle is an electron
//!
bool ParticleType::isElectron() const        { return pdgCode== 11; }

//!
//! Return true if this particle is a positron
//!
bool ParticleType::isPositron() const        { return pdgCode== 11; }

//!
//! Return true if this particle is a muon lepton
//!
bool ParticleType::isMuon() const            { return pdgCode==13;  }

//!
//! Return true if this particle is an anti muon lepton
//!
bool  ParticleType::isAntiMuon() const        { return pdgCode==13; }

//!
//! Return true if this particle is a  tau lepton
//!
bool ParticleType::isTau() const             { return pdgCode==15; }

//!
//! Return true if this particle is an anti tau lepton
//!
bool ParticleType::isAntiTau() const         { return pdgCode==-15; }

//!
//! Return true if this particle is a meson
//!
bool ParticleType::isMeson() const           { return (statistics==-1); }

//!
//! Return true if this particle is a  pion
//!
bool ParticleType::isPion() const            { return pdgCode ==211 || pdgCode==111 || pdgCode==-211;  }

//!
//! Return true if this particle is a positive pion
//!
bool ParticleType::isPionP() const           { return pdgCode ==211;   }

//!
//! Return true if this particle is a negative pion
//!
bool ParticleType::isPionM() const           { return pdgCode ==-211;  }

//!
//! Return true if this particle is a neutral pion
//!
bool ParticleType::isPion0() const           { return pdgCode ==111;   }

//!
//! Return true if this particle is a  kaon
//!
bool ParticleType::isKaon() const            { return pdgCode ==321 || pdgCode==-321 || pdgCode==311 || pdgCode==-311 || pdgCode==130 || pdgCode==310; }

//!
//! Return true if this particle is a positive kaon.
//!
bool ParticleType::isKaonP() const           { return pdgCode ==321; }

//!
//! Return true if this particle is a negative kaon
//!
bool ParticleType::isKaonM() const           { return pdgCode ==-321; }

//!
//! Return true if this particle is kaon (neutral)
//!
bool ParticleType::isKaon0() const           { return pdgCode ==311; }

//!
//! Return true if this particle is anti-kaon (neutral)
//!
bool ParticleType::isKaon0Bar() const        { return pdgCode ==-311; }

//!
//! Return true if this particle is kaon short.
//!
bool ParticleType::isKaon0S() const          { return pdgCode ==310; }

//!
//! Return true if this particle is kaon long.
//!
bool ParticleType::isKaon0L() const          { return pdgCode ==130; }

//!
//! Return true if this particle is electrically charged (non zero net charge).
//!
bool ParticleType::isCharged() const         { return charge!=0;     }

//!
//! Return true if this particle is a baryon (i.e., has a non zero baryon number)
//!
bool ParticleType::isBaryon() const
{
  return getBaryonNumber()!=0;
}

//!
//! Return true if this particle is a positive  baryon
//!
bool ParticleType::isBaryonPlus() const
{
  return getBaryonNumber()>0;
}

//!
//! Return true if this particle is a negative baryon
//!
bool ParticleType::isBaryonMinus() const
{ return baryonNumber<0;     }

//!
//! Return true if this particle is a hadron
//!
bool ParticleType::isHadron() const
{ return baryonNumber!=0  || ( (statistics==-1)&&(TMath::Abs(pdgCode)>40) );  }

//!
//! Return true if this particle has a non-zero net strangeness
//!
bool ParticleType::isStrange() const
{ return strangeNumber!=0;    }

//!
//! Return true if this particle has a positive strangeness
//!
bool ParticleType::isStrangePlus()  const
{ return strangeNumber>0;    }

//!
//! Return true if this particle has a negative strangeness
//!
bool ParticleType::isStrangeMinus() const
{ return strangeNumber<0;    }

//!
//! Return true if this particle contains a charm quark.
//!
bool ParticleType::isCharm() const
{
  if (charmNumber!=0) return true;
  bool accepting  = (pdgCode==411) ||
  (pdgCode==421) ||
  (pdgCode==10411) ||
  (pdgCode==10421) ||
  (pdgCode==413) ||
  (pdgCode==423) ||
  (pdgCode==10413) ||
  (pdgCode==10423) ||
  (pdgCode==415) ||
  (pdgCode==425) ||
  (pdgCode==431) ||
  (pdgCode==10431) ||
  (pdgCode==433) ||
  (pdgCode==10433) ||
  (pdgCode==20433) ||
  (pdgCode==435) ||
  (pdgCode==441) ||
  (pdgCode==10441) ||
  (pdgCode==100441) ||
  (pdgCode==443) ||
  (pdgCode==10443) ||
  (pdgCode==20443) ||
  (pdgCode==100433) ||
  (pdgCode==30443) ||
  (pdgCode==445) ||
  (pdgCode==100445) ||
  (pdgCode==4122) ||
  (pdgCode==4222) ||
  (pdgCode==4212) ||
  (pdgCode==4112) ||
  (pdgCode==4224) ||
  (pdgCode==4214) ||
  (pdgCode==4114) ||
  (pdgCode==4232) ||
  (pdgCode==4132) ||
  (pdgCode==4322) ||
  (pdgCode==4312) ||
  (pdgCode==4324) ||
  (pdgCode==4314) ||
  (pdgCode==4332) ||
  (pdgCode==4334) ||
  (pdgCode==4412) ||
  (pdgCode==4422) ||
  (pdgCode==4414) ||
  (pdgCode==4424) ||
  (pdgCode==4432) ||
  (pdgCode==4434) ||
  (pdgCode==4444);
  return accepting;
}

//!
//! Return true if this particle has a positive net charm
//!
bool ParticleType::isCharmPlus()  const
{  return charmNumber > 0; }

//!
//! Return true if this particle has a negative net charm
//!
bool ParticleType::isCharmMinus()  const
{  return charmNumber < 0; }

//!
//! Return true if this particle has a net non zero bottomness
//!
bool ParticleType::isBottom() const
{  return bottomNumber!=0; }

//!
//! Return true if this particle has a positive net bottomness
//!
bool ParticleType::isBottomPlus() const
{  return bottomNumber>0;  }

//!
//! Return true if this particle has a negative net bottomness
//!
bool ParticleType::isBottomMinus() const
{  return bottomNumber<0;  }

//!
//! Return true if this particle has a non zero net topness
//!
bool ParticleType::isTop() const
{  return topNumber!=0;    }

//!
//! Return true if this particle has a positive net topness
//!
bool ParticleType::isTopPlus() const
{  return topNumber>0;     }

//!
//! Return true if this particle has a negative net topness
//!
bool ParticleType::isTopMinus() const
{  return topNumber<0;     }

//!
//! Return true if this particle is a quark
//!
bool ParticleType::isQuark() const
{ return pdgCode==1 || pdgCode==2 || pdgCode==3 || pdgCode==4 || pdgCode==5 || pdgCode==6; }

//!
//! Return true if this particle is a proton
//!
bool ParticleType::isProton() const
{ return pdgCode==2212;  }

//!
//! Return true if this particle is a neutron
//!
bool ParticleType::isNeutron() const
{ return pdgCode==2112;  }

//!
//! Return true if this particle is a nucleon (i.e., either a proton or neutron)
//!
bool ParticleType::isNucleon() const
{ return pdgCode==2212 || pdgCode==2112;  }

//!
//! Return true if this particle is an anti-proton
//!
bool ParticleType::isAntiProton() const
{ return pdgCode==-2212;  }

//!
//! Return true if this particle is a anti-neutron
//!
bool ParticleType::isAntiNeutron() const
{ return pdgCode==-2112;  }

//!
//! Return true if this particle is an anti-nucleon
//!
bool ParticleType::isAntiNucleon() const
{ return pdgCode==-2212 || pdgCode==-2112;  }

//!
//! Return true if this particle is a decay mode
//!
bool ParticleType::isDecay() const
{ return pdgCode==1000001;  }

//!
//! Return true if this particle is an interaction vextex
//!
bool ParticleType::isInteraction() const
{ return pdgCode==1000010;  }

//!
//! Return true if this particle is a proton-proton interaction
//!
bool ParticleType::isPPInteraction() const
{ return pdgCode==1000011;  }

//!
//! Return true if this particle is a proton-neutron interaction
//!
bool ParticleType::isPNInteraction() const
{ return pdgCode==1000012;  }

//!
//! Return true if this particle is a neutron-neutron interaction
//!
bool ParticleType::isNNInteraction() const
{ return pdgCode==1000013;  }

//!
//! Return true if this particle is a Lambda baryon
//!
bool ParticleType::isLambda() const
{ return pdgCode== 3122;    }

//!
//! Return true if this particle is a anti-lambda baryon
//!
bool ParticleType::isAntiLambda() const
{ return pdgCode==-3122;    }

//!
//! Return true if this particle is a Sigma+ baryon
//!
bool ParticleType::isSigmaP() const
{ return pdgCode== 3222;    }

//!
//! Return true if this particle is a Sigma0 baryon
//!
bool ParticleType::isSigma0() const
{ return pdgCode== 3212;    }

//!
//! Return true if this particle is a Sigma- baryon
//!
bool ParticleType::isSigmaM() const
{ return pdgCode== 3112;    }

//!
//! Return true if this particle is a anti-Sigma+ (anti)baryon
//!
bool ParticleType::isAntiSigmaP() const
{ return pdgCode== -3222;   }

//!
//! Return true if this particle is a anti-Sigma0 (anti)baryon
//!
bool ParticleType::isAntiSigma0() const
{ return pdgCode== -3212;   }

//!
//! Return true if this particle is a anti-Sigma- (anti)baryon
//!
bool ParticleType::isAntiSigmaM() const
{ return pdgCode== -3112;   }

//!
//! Return true if this particle is a Xi0 baryon (Neutral Cascade)
//!
bool ParticleType::isXi0() const
{ return pdgCode== 3322;    }

//!
//! Return true if this particle is a Xi- baryon (Negative Cascade)
//!
bool ParticleType::isXiM() const
{ return pdgCode== 3312;    }

//!
//! Return true if this particle is a anti-Xi- baryon (Neutral anti-cascade)
//!
bool ParticleType::isAntiXi0() const
{ return pdgCode== -3322;    }

//!
//! Return true if this particle is a Xi- baryon (Negative Cascade)
//!
bool ParticleType::isAntiXiM() const
{ return pdgCode== -3312;    }

//!
//! Return true if this particle is an Omega- baryon
//!
bool ParticleType::isOmegaM() const
{ return pdgCode== 3334;     }

//!
//! Return true if this particle is an anti-Omega- (anti)baryon
//!
bool ParticleType::isAntiOmegaM() const
{ return pdgCode== -3334;    }

//!
//! Return the number of distinct decay modes of this particle
//!
int ParticleType::getNDecayModes() const
{ return decayModes.size();   }

//!
//! Return true if this particle is 'stable'
//!
bool ParticleType::isStable() const
{ return stable; }

//!
//! Return true if this particle is weakly 'stable'
//!
bool ParticleType::isWeakStable() const
{ return weakStable;}



//!
//! Generate decay mode of  this particle
//!
ParticleDecayMode & ParticleType::generateDecayMode();


//!
//! Print the properties of this particle on the given stream and return a reference to that stream.
//!
ostream & ParticleType::printProperties(ostream & os)
{
  //scientific
  os <<  fixed << setw(10) << setprecision(8) << pdgCode;         // Monte-Carlo number according PDG
  os <<  setw(20) << name;   // ParticleType name
  os <<  setw(20) << title;   // ParticleType title
  os <<  scientific << setw(20) << setprecision(4) <<  mass;        // ParticleType mass (GeV)
  os <<  scientific << setw(20) << setprecision(4) <<  width;       // decay width
  os <<  fixed << setw(7) << setprecision(3) <<  gSpin;          // spin degeneracy
  os <<  fixed << setw(5) << setprecision(3) <<  baryonNumber;         // baryonNumber number
  os <<  fixed << setw(5) << setprecision(3) <<  strangeNumber;        // strangeness
  os <<  fixed << setw(5) << setprecision(3) <<  charmNumber;          // charmness
  os <<  fixed << setw(5) << setprecision(3) <<  bottomNumber;         // bottomness
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

//!
//! Print the decay properties of this particle on the given stream and return a reference to that stream.
//!
ostream & ParticleType::printDecayProperties(ostream & os)
{
  String latex;
  latex = "$";
  latex += title;
  latex += "$";
  os <<  " PDG:" << fixed << setw(10) << setprecision(8) << pdgCode;         // Monte-Carlo number according PDG
  os <<  setw(20) << latex;
  os <<  " $m=$ " << scientific << setw(20) << setprecision(4) <<  mass << " GeV/$c^2$";
  os <<  " $#Gamma=$ " << scientific << setw(20) << setprecision(4) <<  width << " GeV/$c^2$ \\\\";       // decay width
  os <<  endl;
  unsigned int nModes = decayModes.size();
  for (unsigned int k=0;k<nModes;k++)
  {
    if (pdgCode != decayModes[0].getChildPdgCode(0))
      {
      os << "        Br(xx)= " << scientific << setw(20) << setprecision(4) << (100.0*decayModes[k].getBranchingRatio()) << "    ";
      os << "   $"<< title << "#rightarrow ";
      unsigned int nChildren = decayModes[k].getNChildren();
      for (unsigned int iChild=0; iChild<nChildren; iChild++)
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

//!
//! Get the ith  decay mode of  this particle
//! @param index : decay mode object encapsulating the branching fraction and the list of particle types this particle decays into.
//!
ParticleDecayMode & ParticleType::getDecayMode(int index) { return decayModes[index];}

//!
//! Get a decayMode type object.
//!
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

//!
//! Get an interaction  type object.
//!
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

//!
//! Get a PP interaction  type object.
//!
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

//!
//! Get a PN interaction  type object.
//!
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

//!
//! Get an NN interaction  type object.
//!
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

//!
//! Get a pointer to a particle type object coding a proton type
//!
ParticleType * ParticleType::getProtonType()
{
  if (protonType==nullptr)
    {
    protonType = ParticleDb::getDefaultParticleDb()->findPdgCode(2212);
    }
  return protonType;
}

//!
//! Get a pointer to a particle type object coding a neutron type
//!
ParticleType * ParticleType::getNeutronType()
{
  if (neutronType==nullptr)
    {
    neutronType = ParticleDb::getDefaultParticleDb()->findPdgCode(2112);
    }
  return neutronType;
}

//!
//! Get a pointer to a particle type object coding a nucleus type
//!
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

//!
//! Set the stable state of  this particle to the given value. This can be used to declare a particle as stable even if it nominally decays into other particles within a finite lifeTime.
//!
void   ParticleType::setStable(bool value)     { stable = value; }

//!
//! Set the weak stable state of  this particle to the given value. This can be used to declare a particle as stable even if it nominally decays into other particles within a finite lifeTime.
//!
void   ParticleType::setweakStable(bool value) { weakStable = value; }


std::vector<ParticleDecayMode> ParticleType::getDecayModes() const
{
  return decayModes; //!<Array of decay modes
}

