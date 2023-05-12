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
using CAP::String;

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
leptonElectron(0),
leptonMuon(0),
leptonTau(0),
stable(1),
weakStable(1),
decayModes(),
decayRndmSelector(),
spinFactor(1.0),
isospinFactor(1.0),
statistics(-1.0)
{}

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
leptonElectron(source.leptonElectron),
leptonMuon(source.leptonMuon),
leptonTau(source.leptonTau),
stable(source.stable),
weakStable(source.weakStable),
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
    leptonElectron =  source.leptonElectron;
    leptonMuon     =  source.leptonMuon;
    leptonTau      =  source.leptonTau;
    charge         =  source.charge;
    stable         =  source.stable;
    weakStable     =  source.weakStable;
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
  if (branchingRatio<0.0  || branchingRatio>1.0)
    {
    cout << "<FATAL> ParticleType::addDecayMode() Attempting to create a decay mode with Br:" << branchingRatio<< endl;
    exit(1);
    }
  if (children.size()<2)
    {
    cout << "<FATAL> ParticleType::addDecayMode() Attempting to create a decay mode with no or only one particle"<< endl;
    exit(1);
    }
  ParticleDecayMode decayMode;
  decayMode.setBranchingRatio(branchingRatio);
  for (int k=0; k<int(children.size()); k++) decayMode.addChild(children[k]);
  addDecayMode(decayMode);
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
    weakStable = false; // need to figure how to this properly
    }
}


//!
//! Get the PDG code of the antiparticle of this particle.
//!
int ParticleType::getAntiParticlePdgCode() const
{
  if (getBaryonNumber() == 0 && charge == 0 && getNetStrangeness() == 0)
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

//!
//! Generate decay mode of  this particle
//!
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
String ParticleType::getName() const
{
  return name;
}

//!
//! Get the title  of the particle
//!
String ParticleType::getTitle() const
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
//! Get the mean life time of this particle in seconds.
//!
double ParticleType::getLifeTime() const
{
  return (width>0.0) ? (6.582E-25/width) : 1.0E37;
}

//!
//! Get the baryon number of this particle.
//!
double ParticleType::getBaryonNumber() const
{
  return (nq+ns+nc+nb+nt - (naq+nas+nac+nab+nat) )/3.0;
}


//!
//! Get the net strangeness of this particle.
//!
double ParticleType::getNetStrangeness() const
{
  return (ns-nas);
}

//!
//! Get the net charm of this particle.
//!
double ParticleType::getNetCharm() const
{
  return (nc-nac);
}

//!
//! Get the net bottomness  of this particle.
//!
double ParticleType::getNetBottom() const
{
  return (nb-nab);
}

//!
//! Get the net topness  of this particle.
//!
double ParticleType::getNetTop() const
{
  return (nt-nat);
}


//!
//! Get the lepton number (electron)  of this particle.
//!
double ParticleType::getLeptonElectron() const
{
  return leptonElectron;
}

//!
//! Get the lepton number (muon)  of this particle.
//!
double ParticleType::getLeptonMuon() const
{
  return leptonMuon;
}

//!
//! Get the lepton number (tau)  of this particle.
//!
double ParticleType::getLeptonTau() const
{
  return leptonTau;
}


void ParticleType::setStatistics(double _spin)
{
  statistics = (_spin - static_cast<int>(_spin) ) < 0.01 ? -1.0 : +1.0;
}


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
  spin       = _spin;
  spinFactor = 2.0*spin + 1;
  setStatistics(spin);
}

void ParticleType::setIsospin(double _isospin)
{
  isospin       = _isospin;
  isospinFactor = 2.0*isospin + 1;
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
bool ParticleType::isFermion() const
{
  return fabs(spin - static_cast<int>(spin)) > 0.01;
}

//!
//! Return true if this particle is a  boson
//!
bool ParticleType::isBoson() const
{
  return fabs(spin - static_cast<int>(spin)) < 0.01;
}

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
bool ParticleType::isMeson() const
{
  return (nq == naq ||
          nq == nas ||
          nq == nac ||
          nq == nab ||
          nq == nat ||

          ns == naq ||
          ns == nas ||
          ns == nac ||
          ns == nab ||
          ns == nat ||

          nc == naq ||
          nc == nas ||
          nc == nac ||
          nc == nab ||
          nc == nat ||

          nb == naq ||
          nb == nas ||
          nb == nac ||
          nb == nab ||
          nb == nat ||

          nt == naq ||
          nt == nas ||
          nt == nac ||
          nt == nab ||
          nt == nat);
}

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
{
  return getBaryonNumber()<0;
}

//!
//! Return true if this particle is a hadron
//!
bool ParticleType::isHadron() const
{
  return isBaryon() || isMeson();
}

//!
//! Return true if this particle has a non-zero net strangeness
//!
bool ParticleType::isStrange() const
{
  return (ns>0 || nas>0) && ns!=nas;
}

//!
//! Return true if this particle has a positive strangeness
//!
bool ParticleType::isStrangePlus()  const
{
  return ns>0 && nas==0;
}

//!
//! Return true if this particle has a negative strangeness
//!
bool ParticleType::isStrangeMinus() const
{
  return nas>0 && ns==0;
}
//!
//! Return true if this particle contains a charm quark.
//!
bool ParticleType::isCharm() const
{
  return (nc>0 || nac>0) && nc!=nac;
}

//!
//! Return true if this particle has a positive net charm
//!
bool ParticleType::isCharmPlus()  const
{
  return nc>0 && nac==0;
}

//!
//! Return true if this particle has a negative net charm
//!
bool ParticleType::isCharmMinus()  const
{
  return nac>0 && nc==0;
}

//!
//! Return true if this particle has a net non zero bottomness
//!
bool ParticleType::isBottom() const
{
  return (nb>0 || nab>0) && nb!=nab;
}

//!
//! Return true if this particle has a positive net bottomness
//!
bool ParticleType::isBottomPlus() const
{
  return nb>0 && nab==0;
}

//!
//! Return true if this particle has a negative net bottomness
//!
bool ParticleType::isBottomMinus() const
{
  return nab>0 && nb==0;
}

//!
//! Return true if this particle has a non zero net topness
//!
bool ParticleType::isTop() const
{
  return (nt>0 || nat>0) && nt!=nat;
}

//!
//! Return true if this particle has a positive net topness
//!
bool ParticleType::isTopPlus() const
{
  return nt>0 && nat==0;
}

//!
//! Return true if this particle has a negative net topness
//!
bool ParticleType::isTopMinus() const
{
  return nat>0 && nt==0;
}

//!
//! Return true if this particle is a quark
//!
bool ParticleType::isQuark() const
{
  return pdgCode==1 || pdgCode==2 || pdgCode==3 || pdgCode==4 || pdgCode==5 || pdgCode==6;
}

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
//! Print the properties of this particle on the given stream and return a reference to that stream.
//!
ostream & ParticleType::printProperties(ostream & os)
{
  //scientific
  os <<  fixed << setw(10) << setprecision(8) << pdgCode;         // Monte-Carlo number according PDG
  os <<  setw(12) << name;   // ParticleType name
  os <<  setw(12) << title;   // ParticleType title
  os <<  scientific << setw(12) << setprecision(4) <<  mass;
  os <<  scientific << setw(12) << setprecision(4) <<  width;
  os <<  fixed << setw(9) << setprecision(2) <<  charge;
  os <<  fixed << setw(9) << setprecision(2) <<  spin;
  os <<  fixed << setw(9) << setprecision(2) <<  isospin;
  os <<  fixed << setw(9) << setprecision(2) <<  isospin3;
  os <<  fixed << setw(5) << setprecision(1) <<  nq;
  os <<  fixed << setw(5) << setprecision(1) <<  naq;
  os <<  fixed << setw(5) << setprecision(1) <<  ns;
  os <<  fixed << setw(5) << setprecision(1) <<  nas;
  os <<  fixed << setw(5) << setprecision(1) <<  nc;
  os <<  fixed << setw(5) << setprecision(1) <<  nac;
  os <<  fixed << setw(5) << setprecision(1) <<  nb;
  os <<  fixed << setw(5) << setprecision(1) <<  nab;
  os <<  fixed << setw(5) << setprecision(1) <<  nt;
  os <<  fixed << setw(5) << setprecision(1) <<  nat;
  os <<  fixed << setw(5) << setprecision(1) <<  leptonElectron;
  os <<  fixed << setw(5) << setprecision(1) <<  leptonMuon;
  os <<  fixed << setw(5) << setprecision(1) <<  leptonTau;
  os <<  fixed << setw(5) << setprecision(1) <<  stable;
  os <<  fixed << setw(5) << setprecision(1) <<  weakStable;
  os <<  endl;
  for (int k=0;k<int(decayModes.size());k++)
  {
    cout << "      " << k << "    ";
    decayModes[k].printProperties(os);
  }
  return os;
}

//!
//! Print the decay properties of this particle on the given stream and return a reference to that stream.
//!
ostream & ParticleType::printDecayProperties(ostream & os)
{
  os <<  setw(12) << name;   // ParticleType name
  os <<  setw(12) << title;   // ParticleType title
  os <<  scientific << setw(12) << setprecision(4) <<  mass;
  os <<  scientific << setw(12) << setprecision(4) <<  width;
  os <<  fixed << setw(9) << setprecision(2) <<  charge;
  os <<  fixed << setw(9) << setprecision(2) <<  spin;
  unsigned int nModes = decayModes.size();
  for (unsigned int k=0;k<nModes;k++)
  {
    if (pdgCode != decayModes[0].getChildPdgCode(0))
      {
      os << "      " << k << " Br:" << scientific << setw(20) << setprecision(4) << (100.0*decayModes[k].getBranchingRatio()) << " to ";
      unsigned int nChildren = decayModes[k].getNChildren();
      for (unsigned int iChild=0; iChild<nChildren; iChild++)
        {
        ParticleType & type = decayModes[k].getChildType(iChild);
        os << type.getTitle();
        if (iChild < nChildren-1)  os << "+";
        }
      os << endl;
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

