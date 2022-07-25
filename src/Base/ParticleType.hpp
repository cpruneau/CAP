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

#ifndef CAP__ParticleType
#define CAP__ParticleType
#include <iostream>
#include <iomanip>
#include <vector>
#include "TString.h"
#include "SelectionGenerator.hpp"
#include "ParticleDecayMode.hpp"

using namespace std;
class ParticleDecayMode;

//statistics = 1;  // Fermi-Dirac
//statistics = -1;  // Bose-Einstein

//!
//!This class defines the notion of particle type used in CAP.  Virtually all elementary particle properties
//!are represented and stored. Property values are stored and read from a particle property file. Property values are based on
//!the particle data book maintained by the particle data group (PDG).
//!
class ParticleType
{
protected:

  TString name;       //!<ParticleType name: e.g., pip
  TString title;      //!<ParticleType title: e.g., #pi^{+}
  int privateCode;    //!<Code used in this package only (convenience)
  int pdgCode;        //!<Monte-Carlo number according PDG
  double mass;        //!<ParticleType mass (GeV)
  double width;       //!<decay width
  int gSpin;          //!<spin degeneracy
  int gIsospin;       //!<isospin degeneracy
  int baryon;         //!<baryon number
  int strange;        //!<strangeness
  int charm;          //!<charmness
  int bottom;         //!<bottomness
  int top;            //!<topness
  int leptonElectron; //!<Electron lepton number
  int leptonMuon;     //!<Muon lepton number
  int leptonTau;      //!<Tau lepton number
  int charge;         //!<charge
  bool stable;        //!<defines whether this ParticleType is stable
  bool weakStable;    //!<particle is weakly stable (i.e., decays by weak-decay)
  int statistics;     //!<Bosons or Fermions
  std::vector<ParticleDecayMode> decayModes; //!<Array of decay modes
  SelectionGenerator * decayRndmSelector; //!< Random selector of decay schemes.

public:
  
  //! Default CTOR
  ParticleType();
  
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
  ParticleType(int pdgCode_in, const TString & name_in, const TString & title_in, double mass_in,
               double width_in, int gSpin_in, int baryon_in, int strange_in,
               int charm_in, int bottom_in, int gIsospin_in, int charge_in);
  
  //!
  //! COPY CTOR
  //!
  ParticleType(const ParticleType & source);
  
  //!
  //! DTOR
  //!
  virtual ~ParticleType();
  
  //!
  //! Assignment operator. Copies all the properties of the source particle into this.
  //!
  ParticleType & operator=(const ParticleType & source);

  //!
  //! Sets up the decay generator used in CAP.
  //!
  void setupDecayGenerator();
  
  void generateDecay(int&n,int * pid);

  //!
  //! Get the name of the particle
  //!
  TString getName()     { return name;}

  //!
  //! Get the title  of the particle
  //!
  TString getTitle()    { return title;}

  //!
  //! Get the PDG code of the antiparticle of this particle.
  //!
  int    getAntiParticlePdgCode() const;

  //!
  //! Get the PDG code of this particle.
  //!
  int    getPdgCode() const        { return pdgCode;}

  //!
  //! Get the private  code of this particle (user defined).
  //!
  int    getPrivateCode() const    { return privateCode;}

  //!
  //! Get the mass of this particle in GeV/c^2.
  //!
  double getMass() const           { return mass;  } // GeV

  //!
  //! Get the width of this particle in GeV/c^2.
  //!
  double getWidth() const          { return width; } // GeV

  //!
  //! Get the mean life time of this particle in seconds.
  //!
  double getLifeTime() const       { return (width>0.0) ? (6.582E-25/width) : 1.0E37; }

  //!
  //! Get the baryon number of this particle.
  //!
  int    getBaryon() const         { return baryon;}

  //!
  //! Get the electric charge of this particle.
  //!
  int    getCharge() const         { return charge;}

  //!
  //! Get the net strangeness of this particle.
  //!
  int    getStrange() const        { return strange;}

  //!
  //! Get the net charm of this particle.
  //!
  int    getCharm() const          { return charm;}
  
  //!
  //! Get the net bottomness  of this particle.
  //!
  int    getBottom() const         { return bottom;}

  //!
  //! Get the lepton number (electron)  of this particle.
  //!
  int    getLeptonElectron() const { return leptonElectron;}

  //!
  //! Get the lepton number (muon)  of this particle.
  //!
  int    getLeptonMuon() const     { return leptonMuon;}

  //!
  //! Get the lepton number (tau)  of this particle.
  //!
  int    getLeptonTau() const      { return leptonTau;}

  //!
  //! Get the spin degeneracy factor of this particle.
  //!
  int    getSpinFactor() const     { return gSpin;}

  //!
  //! Get the isospin degeneracy factor of this particle.
  //!
  int    getIsoSpinFactor() const  { return gIsospin;      }

  //!
  //! Get the statistics of this particle used in the expression of the particle density (+ or -).
  //!
  int    getStatistics() const     { return statistics; }

  //!
  //! Set the name of this particle.
  //!
  void setName(const TString & _name)  { name    = _name;}

  //!
  //! Set the 'title'' of this particle.
  //!
  void setTitle(const TString & _title){ title   = _title;}

  //!
  //! Set the PDG code of this particle.
  //!
  void setPdgCode(int value)           { pdgCode = value; }

  //!
  //! Set the private (user defined) code of this particle.
  //!
  void setPrivateCode(int value)       { privateCode = value;}

  //!
  //! Set the mass of this particle in GeV/c^2.
  //!
  void setMass(double value)           { mass    = value;  }

  //!
  //! Set the width of this particle in GeV/c^2.
  //!
  void setWidth(double value)          { width   = value; }

  //!
  //! Set the baryon number of this particle.
  //!
  void setBaryon(int value)            { baryon  = value;}

  //!
  //! Set the electric charge of this particle.
  //!
  void setCharge(int value)            { charge  = value;}

  //!
  //! Set the net strangeness of this particle.
  //!
  void setStrange(int value)           { strange = value;}

  //!
  //! Set the net charm of this particle.
  //!
  void setCharm(int value)             { charm   = value;}

  //!
  //! Set the net bottomness of this particle.
  //!
  void setBottom(int value)            { bottom  = value;}

  //!
  //! Set the electron lepton value of this particle
  //!
  void setLeptonElectron(int value)    { leptonElectron = value;}

  //!
  //! Set the muon lepton value of this particle
  //!
  void setLeptonMuon(int value)        { leptonMuon   = value;}

  //!
  //! Set the tau lepton value of this particle
  //!
  void setLeptonTau(int value)         { leptonTau    = value;}

  //!
  //! Set the spin dengeneracy factor  of this particle.
  //!
  void setSpinFactor(double value)     { gSpin        = value;}

  //!
  //! Set the isospin dengeneracy factor  of this particle.
  //!
  void setIsoSpinFactor(double value)  { gIsospin     = value;}

  //!
  //! Set the statistics of this particle.
  //!
  void setStatistics(int value)        { statistics   = value;}

  //!
  //! Return true if this particle is a Higgs  boson
  //!
  bool     isHiggs() const           { return 0; }

  //!
  //! Return true if this particle is a fermion
  //!
  bool     isFermion() const         { return statistics==1; }

  //!
  //! Return true if this particle is a  boson
  //!
  bool     isBoson() const           { return statistics==-1; }

  //!
  //! Return true if this particle is a gluon
  //!
  bool     isGluon() const           { return pdgCode==21; }

  //!
  //! Return true if this particle is a photon
  //!
  bool     isPhoton() const          { return pdgCode==22; }

  //!
  //! Return true if this particle is a Z boson
  //!
  bool     isZBoson() const          { return pdgCode==23; }

  //!
  //! Return true if this particle is a W boson
  //!
  bool     isWBoson() const          { return pdgCode==24; }

  //!
  //! Return true if this particle is a Higgs gauge boson
  //!
  bool     isHiggsBoson() const      { return pdgCode==25; }

  //!
  //! Return true if this particle is a gauge boson
  //!
  bool     isGaugeBoson() const      { return pdgCode==21 || pdgCode==22 || pdgCode==23 || pdgCode==24; }

  //!
  //! Return true if this particle is a lepton
  //!
  bool     isLepton() const          { return leptonElectron!=0 || leptonMuon!=0 || leptonTau!=0; }

  //!
  //! Return true if this particle is an electron
  //!
  bool     isElectron() const        { return pdgCode== 11; }

  //!
  //! Return true if this particle is a positron
  //!
  bool     isPositron() const        { return pdgCode== 11; }

  //!
  //! Return true if this particle is a muon lepton
  //!
  bool     isMuon() const            { return pdgCode==13;  }

  //!
  //! Return true if this particle is an anti muon lepton
  //!
  bool     isAntiMuon() const        { return pdgCode==13; }

  //!
  //! Return true if this particle is a  tau lepton
  //!
  bool     isTau() const             { return pdgCode==15; }

  //!
  //! Return true if this particle is an anti tau lepton
  //!
  bool     isAntiTau() const         { return pdgCode==-15; }

  //!
  //! Return true if this particle is a meson
  //!
  bool     isMeson() const           { return (statistics==-1); }

  //!
  //! Return true if this particle is a  pion
  //!
  bool     isPion() const            { return pdgCode ==211 || pdgCode==111 || pdgCode==-211;  }

  //!
  //! Return true if this particle is a positive pion
  //!
  bool     isPionP() const           { return pdgCode ==211;   }

  //!
  //! Return true if this particle is a negative pion
  //!
  bool     isPionM() const           { return pdgCode ==-211;  }

  //!
  //! Return true if this particle is a neutral pion
  //!
  bool     isPion0() const           { return pdgCode ==111;   }

  //!
  //! Return true if this particle is a  kaon
  //!
  bool     isKaon() const            { return pdgCode ==321 || pdgCode==-321 || pdgCode==311 || pdgCode==-311 || pdgCode==130 || pdgCode==310; }

  //!
  //! Return true if this particle is a positive kaon.
  //!
  bool     isKaonP() const           { return pdgCode ==321; }

  //!
  //! Return true if this particle is a negative kaon
  //!
  bool     isKaonM() const           { return pdgCode ==-321; }

  //!
  //! Return true if this particle is kaon (neutral)
  //!
  bool     isKaon0() const           { return pdgCode ==311; }

  //!
  //! Return true if this particle is anti-kaon (neutral)
  //!
  bool     isKaon0Bar() const        { return pdgCode ==-311; }

  //!
  //! Return true if this particle is kaon short.
  //!
  bool     isKaon0S() const          { return pdgCode ==310; }

  //!
  //! Return true if this particle is kaon long.
  //!
  bool     isKaon0L() const          { return pdgCode ==130; }

  //!
  //! Return true if this particle is electrically charged (non zero net charge).
  //!
  bool     isCharged() const         { return charge!=0;     }

  //!
  //! Return true if this particle is a baryon (i.e., has a non zero baryon number)
  //!
  bool     isBaryon() const          { return baryon!=0;     }

  //!
  //! Return true if this particle is a positive  baryon
  //!
  bool     isBaryonPlus() const      { return baryon>0;     }

  //!
  //! Return true if this particle is a negative baryon
  //!
  bool     isBaryonMinus() const     { return baryon<0;     }

  //!
  //! Return true if this particle is a hadron
  //!
  bool     isHadron() const          { return baryon!=0  || ( (statistics==-1)&&(TMath::Abs(pdgCode)>40) );  }

  //!
  //! Return true if this particle has a non-zero net strangeness
  //!
  bool     isStrange() const         { return strange!=0;    }

  //!
  //! Return true if this particle has a positive strangeness
  //!
  bool     isStrangePlus()           { return strange>0;    }

  //!
  //! Return true if this particle has a negative strangeness
  //!
  bool     isStrangeMinus()          { return strange<0;    }

  //!
  //! Return true if this particle contains a charm quark.
  //!
  bool     isCharm() const
  {
    if (charm!=0) return true;
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
  bool     isCharmPlus()  const   {  return charm > 0; }

  //!
  //! Return true if this particle has a negative net charm
  //!
  bool     isCharmMinus()  const  {  return charm < 0; }

  //!
  //! Return true if this particle has a net non zero bottomness
  //!
  bool     isBottom() const       {  return bottom!=0; }

  //!
  //! Return true if this particle has a positive net bottomness
  //!
  bool     isBottomPlus()         {  return bottom>0;  }

  //!
  //! Return true if this particle has a negative net bottomness
  //!
  bool     isBottomMinus()        {  return bottom<0;  }

  //!
  //! Return true if this particle has a non zero net topness
  //!
  bool     isTop()                {  return top!=0;    }

  //!
  //! Return true if this particle has a positive net topness
  //!
  bool     isTopPlus()            {  return top>0;     }

  //!
  //! Return true if this particle has a negative net topness
  //!
  bool     isTopMinus()           {  return top<0;     }

  //!
  //! Return true if this particle is a quark
  //!
  bool     isQuark() const           { return pdgCode==1 || pdgCode==2 || pdgCode==3 || pdgCode==4 || pdgCode==5 || pdgCode==6; }

  //!
  //! Return true if this particle is a proton
  //!
  bool     isProton() const          { return pdgCode==2212;  }

  //!
  //! Return true if this particle is a neutron
  //!
  bool     isNeutron() const         { return pdgCode==2112;  }

  //!
  //! Return true if this particle is a nucleon (i.e., either a proton or neutron)
  //!
  bool     isNucleon() const         { return pdgCode==2212 || pdgCode==2112;  }

  //!
  //! Return true if this particle is an anti-proton
  //!
  bool     isAntiProton() const      { return pdgCode==-2212;  }

  //!
  //! Return true if this particle is a anti-neutron
  //!
  bool     isAntiNeutron() const     { return pdgCode==-2112;  }

  //!
  //! Return true if this particle is an anti-nucleon
  //!
  bool     isAntiNucleon() const     { return pdgCode==-2212 || pdgCode==-2112;  }

  //!
  //! Return true if this particle is a decay mode
  //!
  bool     isDecay() const           { return pdgCode==1000001;  }

  //!
  //! Return true if this particle is an interaction vextex
  //!
  bool     isInteraction() const     { return pdgCode==1000010;  }

  //!
  //! Return true if this particle is a proton-proton interaction
  //!
  bool     isPPInteraction() const   { return pdgCode==1000011;  }

  //!
  //! Return true if this particle is a proton-neutron interaction
  //!
  bool     isPNInteraction() const   { return pdgCode==1000012;  }

  //!
  //! Return true if this particle is a neutron-neutron interaction
  //!
  bool     isNNInteraction() const   { return pdgCode==1000013;  }

  //!
  //! Return true if this particle is a Lambda baryon
  //!
  bool     isLambda() const          { return pdgCode== 3122;    }

  //!
  //! Return true if this particle is a anti-lambda baryon
  //!
  bool     isAntiLambda() const      { return pdgCode==-3122;    }

  //!
  //! Return true if this particle is a Sigma+ baryon
  //!
  bool     isSigmaP() const          { return pdgCode== 3222;    }

  //!
  //! Return true if this particle is a Sigma0 baryon
  //!
  bool     isSigma0() const          { return pdgCode== 3212;    }

  //!
  //! Return true if this particle is a Sigma- baryon
  //!
  bool     isSigmaM() const          { return pdgCode== 3112;    }

  //!
  //! Return true if this particle is a anti-Sigma+ (anti)baryon
  //!
  bool     isAntiSigmaP() const      { return pdgCode== -3222;   }

  //!
  //! Return true if this particle is a anti-Sigma0 (anti)baryon
  //!
  bool     isAntiSigma0() const      { return pdgCode== -3212;   }

  //!
  //! Return true if this particle is a anti-Sigma- (anti)baryon
  //!
  bool     isAntiSigmaM() const      { return pdgCode== -3112;   }

  //!
  //! Return true if this particle is a Xi0 baryon (Neutral Cascade)
  //!
  bool     isXi0() const             { return pdgCode== 3322;    }

  //!
  //! Return true if this particle is a Xi- baryon (Negative Cascade)
  //!
  bool     isXiM() const             { return pdgCode== 3312;    }

  //!
  //! Return true if this particle is a anti-Xi- baryon (Neutral anti-cascade)
  //!
  bool     isAntiXi0() const         { return pdgCode== -3322;    }

  //!
  //! Return true if this particle is a Xi- baryon (Negative Cascade)
  //!
  bool     isAntiXiM() const         { return pdgCode== -3312;    }

  //!
  //! Return true if this particle is an Omega- baryon
  //!
  bool     isOmegaM() const          { return pdgCode== 3334;     }

  //!
  //! Return true if this particle is an anti-Omega- (anti)baryon
  //!
  bool     isAntiOmegaM() const      { return pdgCode== -3334;    }

  //!
  //! Return the number of distinct decay modes of this particle
  //!
  int    getNDecayModes() const    { return decayModes.size();   }
 
  //!
  //! Return true if this particle is 'stable'
  //!
  bool   isStable() const          { return stable; }

  //!
  //! Return true if this particle is weakly 'stable'
  //!
  bool   isWeakStable() const      { return weakStable;}

  //!
  //! Add a decay mode to this particle type based on the given branching fraction (branching ratio) and the given array of children
  //! @param branchingRatio : fraction of all decays of this particle type into the given children
  //! @param children : array of particle type index corresponding to the particle type this particle decays into.
  //!
  void   addDecayMode(double branchingRatio, std::vector<int> children);
  
  //!
  //! Add a decay mode to this particle type based on the given decayModel object
  //! @param decayMode : decay mode object encapsulating the branching fraction and the list of particle types this particle decays into.
  //!
  void   addDecayMode(ParticleDecayMode &decayMode);
  
  //!
  //! Get the ith  decay mode of  this particle
  //! @param index : decay mode object encapsulating the branching fraction and the list of particle types this particle decays into.
  //!
  ParticleDecayMode & getDecayMode(int index) { return decayModes[index];}
  
  //!
  //! Generate decay mode of  this particle
  //!
  ParticleDecayMode & generateDecayMode();

  //!
  //! Set the stable state of  this particle to the given value. This can be used to declare a particle as stable even if it nominally decays into other particles within a finite lifetime.
  //!
  void   setStable(bool value)     { stable = value; }

  //!
  //! Set the weak stable state of  this particle to the given value. This can be used to declare a particle as stable even if it nominally decays into other particles within a finite lifetime.
  //!
  void   setWeakStable(bool value) { weakStable = value; }


  std::vector<ParticleDecayMode> getDecayModes() const
  {
  return decayModes; //!<Array of decay modes
  }

  //!
  //! Print the properties of this particle on the given stream and return a reference to that stream.
  //!
  ostream & printProperties(ostream & os);

  //!
  //! Print the decay properties of this particle on the given stream and return a reference to that stream.
  //!
  ostream & printDecayProperties(ostream & os);

  //!
  //! Get a decayMode type object.
  //!
  static  ParticleType * getDecayModeType();

  //!
  //! Get an interaction  type object.
  //!
  static  ParticleType * getInteractionType();

  //!
  //! Get a PP interaction  type object.
  //!
  static  ParticleType * getPPInteractionType();

  //!
  //! Get a PN interaction  type object.
  //!
  static  ParticleType * getPNInteractionType();

  //!
  //! Get an NN interaction  type object.
  //!
  static  ParticleType * getNNInteractionType();

  //!
  //! Get a pointer to a particle type object coding a proton type
  //!
  static  ParticleType * getProtonType();

  //!
  //! Get a pointer to a particle type object coding a neutron type
  //!
  static  ParticleType * getNeutronType();

  //!
  //! Get a pointer to a particle type object coding a nucleus type
  //!
  static  ParticleType * getNucleusType();

protected:

  static  ParticleType * protonType;    //!< Pointer to a singleton object coding a proton type
  static  ParticleType * neutronType;   //!< Pointer to a singleton object coding a neutron type
  static  ParticleType * nucleusType;   //!< Pointer to a singleton object coding a nucleus type
  static  ParticleType * decayModeType; //!< Pointer to a singleton object coding a decay mode type
  static  ParticleType * interactionType; //!< Pointer to a singleton object coding an interaction type
  static  ParticleType * protonProtonInteractionType; //!< Pointer to a singleton object coding a proton proton interaction type
  static  ParticleType * protonNeutronInteractionType; //!< Pointer to a singleton object coding a proton neutron interaction type
  static  ParticleType * neutronNeutronInteractionType; //!< Pointer to a singleton object coding a neutron neutron type



  ClassDef(ParticleType,0)
};

#endif  

