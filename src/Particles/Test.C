#include <TString.h>
class XmlDocument;
using String = TString;

class ParticleType;
class ParticleDecayMode;
class ParticleDb;


class ParticleDecayMode
{
private:

  double branchingRatio;
  vector<int> childrenPdgCodes;
  vector<ParticleType*> childrenTypes;

public:
  ParticleDecayMode();
  ParticleDecayMode(const ParticleDecayMode & source);
  virtual ~ParticleDecayMode();
  ParticleDecayMode & operator=(const ParticleDecayMode & source);
  void resolveTypes(ParticleDb & collection);

  void   setBranchingRatio(double _branchingRatio);
  double getBranchingRatio() const;
  int    getNChildren() const;
  void   addChild(int pdgCode);
  void   addChild(ParticleType* type);
  inline const vector<int> & getChildrenPdgCodes() const;
  inline vector<int> & getChildrenPdgCodes();
  inline const vector<ParticleType*> & getChildrenTypes() const;
  inline vector<ParticleType*> & getChildrenTypes();
  inline ParticleType & getChildType(unsigned int index) const;
  inline int getChildPdgCode(int index) const ;
  ostream & printProperties(ostream & os);

  ClassDef(ParticleDecayMode,0)
};

//!
//!This class defines the notion of particle type used in CAP.  Virtually all elementary particle properties
//!are represented and stored. Property values are stored and read from a particle property file. Property values are based on
//!the particle data book maintained by the particle data group (PDG).
//!
class ParticleType
{
protected:

  String name;           //!<ParticleType name: e.g., pip
  String title;          //!<ParticleType title: e.g., #pi^{+}
  int    privateCode;    //!<Code used in this package only (convenience)
  int    pdgCode;        //!<Monte-Carlo number according PDG
  double mass;           //!<ParticleType mass (GeV)
  double charge;         //!<charge
  double width;          //!<decay width
  double lifeTime;       //!<lifeTime
  double spin;           //!<spin
  double isospin;        //!<isospin
  double isospin3;       //!<isospin 3rd component
  int    nq;             //!<number of Q  quarks
  int    naq;            //!<number of AQ quarks
  int    ns;             //!<number of S  quarks
  int    nas;            //!<number of AS quarks
  int    nc;             //!<number of C  quarks
  int    nac;            //!<number of AC quarks
  int    nb;             //!<number of B  quarks
  int    nab;            //!<number of AB quarks
  int    baryonNumber;   //!<baryon number
  int    strangeNumber;  //!<strangeness
  int    charmNumber;    //!<charmness
  int    bottomNumber;   //!<bottomness
  int    topNumber;      //!<topness
  int    leptonElectron; //!<Electron lepton number
  int    leptonMuon;     //!<Muon lepton number
  int    leptonTau;      //!<Tau lepton number
  bool   stable;         //!<defines whether this ParticleType is stable
  bool   weakStable;     //!<particle is weakly stable (i.e., decays by weak-decay)
  int    gSpin;          //!<spin degeneracy
  int    gIsospin;       //!<isospin degeneracy
  int    statistics;     //!<Bosons or Fermions
  std::vector<ParticleDecayMode> decayModes; //!<Array of decay modes
  //SelectionGenerator decayRndmSelector;
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
  ParticleType(int pdgCode_in, const String & name_in, const String & title_in, double mass_in,
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
  String getName()     { return name;}

  //!
  //! Get the title  of the particle
  //!
  String getTitle()    { return title;}

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
  double getLifeTime() const       { return lifeTime; }

  //!
  //! Get the baryon number of this particle.
  //!
  int    getBaryonNumber() const         { return baryonNumber;}

  //!
  //! Get the electric charge of this particle.
  //!
  int    getCharge() const         { return charge;}

  //!
  //! Get the net strangeness of this particle.
  //!
  int    getStrangeNumber() const        { return strangeNumber;}

  //!
  //! Get the net charm of this particle.
  //!
  int    getCharmNumber() const          { return charmNumber;}

  //!
  //! Get the net bottomness  of this particle.
  //!
  int    getBottomNumber() const         { return bottomNumber;}

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

  double getIsospin()   const    { return isospin;}
  double getIsospin3()  const    { return isospin3;}

  int    getNumberQ()    const   { return nq;  }
  int    getNumberAQ()   const   { return naq; }
  int    getNumberS()    const   { return ns;  }
  int    getNumberAS()   const   { return nas; }
  int    getNumberC()    const   { return nc;  }
  int    getNumberAC()   const   { return nac; }
  int    getNumberB()    const   { return nb;  }
  int    getNumberAB()   const   { return nab; }
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
  void setName(const String & _name)  { name    = _name;}

  //!
  //! Set the 'title'' of this particle.
  //!
  void setTitle(const String & _title){ title   = _title;}

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
  void setWidth(double value)
  {
  width    = value;
  lifeTime = (width>0.0) ? (6.582E-25/width) : 1.0E37;
  }

  void setLifeTime(double value)
  {
  lifeTime = value;
  width = (lifeTime<1.0E37) ? (6.582E-25/lifeTime) : 0.0;
  }

  //!
  //! Set the baryon number of this particle.
  //!
  void setBaryonNumber(int value)            { baryonNumber  = value;}

  //!
  //! Set the electric charge of this particle.
  //!
  void setCharge(int value)            { charge  = value;}

  //!
  //! Set the net strangeness of this particle.
  //!
  void setStrangeNumber(int value)           { strangeNumber = value;}

  //!
  //! Set the net charm of this particle.
  //!
  void setCharmNumber(int value)             { charmNumber   = value;}

  //!
  //! Set the net bottomness of this particle.
  //!
  void setBottomNumber(int value)            { bottomNumber  = value;}

  //!
  //! Set the electron lepton value of this particle
  //!
  void setLeptonElectron(int value)        { leptonElectron = value;}

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

  void  setNumberQ(int  value) { nq  = value;}
  void  setNumberAQ(int value) { naq = value;}
  void  setNumberS(int  value) { ns  = value;}
  void  setNumberAS(int value) { nas = value;}
  void  setNumberC(int  value) { nc  = value;}
  void  setNumberAC(int value) { nac = value;}
  void  setNumberB(int  value) { nb  = value;}
  void  setNumberAB(int value) { nab = value;}

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
  bool     isBaryon() const          { return baryonNumber!=0;     }

  //!
  //! Return true if this particle is a positive  baryon
  //!
  bool     isBaryonPlus() const      { return baryonNumber>0;     }

  //!
  //! Return true if this particle is a negative baryon
  //!
  bool     isBaryonMinus() const     { return baryonNumber<0;     }

  //!
  //! Return true if this particle is a hadron
  //!
  bool     isHadron() const          { return baryonNumber!=0  || ( (statistics==-1)&&(TMath::Abs(pdgCode)>40) );  }

  //!
  //! Return true if this particle has a non-zero net strangeness
  //!
  bool     isStrange() const         { return strangeNumber!=0;    }

  //!
  //! Return true if this particle has a positive strangeness
  //!
  bool     isStrangePlus()           { return strangeNumber>0;    }

  //!
  //! Return true if this particle has a negative strangeness
  //!
  bool     isStrangeMinus()          { return strangeNumber<0;    }

  //!
  //! Return true if this particle contains a charm quark.
  //!
  bool     isCharm() const
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
  bool     isCharmPlus()  const   {  return charmNumber > 0; }

  //!
  //! Return true if this particle has a negative net charm
  //!
  bool     isCharmMinus()  const  {  return charmNumber < 0; }

  //!
  //! Return true if this particle has a net non zero bottomness
  //!
  bool     isBottom() const       {  return bottomNumber!=0; }

  //!
  //! Return true if this particle has a positive net bottomness
  //!
  bool     isBottomPlus()         {  return bottomNumber>0;  }

  //!
  //! Return true if this particle has a negative net bottomness
  //!
  bool     isBottomMinus()        {  return bottomNumber<0;  }

  //!
  //! Return true if this particle has a non zero net topness
  //!
  bool     isTop()                {  return topNumber!=0;    }

  //!
  //! Return true if this particle has a positive net topness
  //!
  bool     isTopPlus()            {  return topNumber>0;     }

  //!
  //! Return true if this particle has a negative net topness
  //!
  bool     isTopMinus()           {  return topNumber<0;     }

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
  //! Set the stable state of  this particle to the given value. This can be used to declare a particle as stable even if it nominally decays into other particles within a finite lifeTime.
  //!
  void   setStable(bool value)     { stable = value; }

  //!
  //! Set the weak stable state of  this particle to the given value. This can be used to declare a particle as stable even if it nominally decays into other particles within a finite lifeTime.
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

  protected:



};

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
decayModes()
//decayRndmSelector()
{}

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
decayModes()
//decayRndmSelector()
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

ParticleType::~ParticleType()
{
}

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
decayModes(source.decayModes)//,
//decayRndmSelector(source.decayRndmSelector)
{
  // no ops
}

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
//    decayRndmSelector = source.decayRndmSelector;
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
  if (baryonNumber == 0 && charge == 0 && strangeNumber == 0)
    return(pdgCode);
  else
    return(-pdgCode);
}

void ParticleType::setupDecayGenerator()
{
//  int nDecayModes = decayModes.size();
//  vector<double> decayBranchingRatios;
//  for (int k=0; k<nDecayModes; k++)
//    {
//    decayBranchingRatios.push_back(decayModes[k].getBranchingRatio());
//    }
//  decayRndmSelector = new SelectionGenerator(decayBranchingRatios);
}

ParticleDecayMode & ParticleType::generateDecayMode()
{
//  if (!decayRndmSelector)
//    {
//    exit(0);
//    }
//  int index = decayRndmSelector->generate();
//  if (index<0)
//    {
//    exit(0);
//    }
//  else if (index>= int(decayModes.size()))
//    {
//    index = decayModes.size()-1;
//    }
  return ParticleDecayMode;// decayModes[index];
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


ParticleDecayMode::ParticleDecayMode()
:
branchingRatio(1.0),
childrenPdgCodes(),
childrenTypes()
{  }

ParticleDecayMode::ParticleDecayMode(const ParticleDecayMode & source)
:
branchingRatio(source.branchingRatio),
childrenPdgCodes(source.childrenPdgCodes),
childrenTypes(source.childrenTypes)
{  }

ParticleDecayMode::~ParticleDecayMode()
{
  childrenPdgCodes.clear();
  childrenTypes.clear();
}

ParticleDecayMode & ParticleDecayMode::operator=(const ParticleDecayMode & source)
{
  if (this!=&source)
    {
    branchingRatio    = source.branchingRatio;
    childrenTypes     = source.childrenTypes;
    }
  return *this;
}

// Assumes the mode was defined based on pdg codes
// build the table for types
void ParticleDecayMode::resolveTypes(ParticleDb & particleDb)
{
  int n = childrenPdgCodes.size();
  if (n<1) return;
  ParticleType * type;
  for (int k=0; k<n; k++)
    {
    type = particleDb.findPdgCode( childrenPdgCodes[k] );
    if (type)
      {
      childrenTypes.push_back(type);
      }
    else
      {
      cout << "ParticleDecayMode::resolveTypes(ParticleDb * particleDb) Invalid particleType." << endl;
      cout << "ParticleDecayMode::resolveTypes(ParticleDb * particleDb) Abort." << endl;
      exit(1);
      }
    }
}

void   ParticleDecayMode::setBranchingRatio(double _branchingRatio)
{
  branchingRatio = _branchingRatio;
}

double ParticleDecayMode::getBranchingRatio() const
{
  return branchingRatio;
}

int    ParticleDecayMode::getNChildren() const
{
  return childrenPdgCodes.size();
}

void   ParticleDecayMode::addChild(int pdgCode)
{
  childrenPdgCodes.push_back(pdgCode);
}

void   ParticleDecayMode::addChild(ParticleType* type)
{
  childrenPdgCodes.push_back(type.getPdgCode());
  childrenTypes.push_back(type);
}

const vector<int> & ParticleDecayMode::getChildrenPdgCodes() const
{
  return childrenPdgCodes;
}


vector<int> & ParticleDecayMode::getChildrenPdgCodes()
{
  return childrenPdgCodes;
}

const vector<ParticleType*> & ParticleDecayMode::getChildrenTypes() const
{
  return childrenTypes;
}

vector<ParticleType*> & ParticleDecayMode::getChildrenTypes()
{
  return childrenTypes;
}

ParticleType & ParticleDecayMode::getChildType(unsigned int index) const
{
  return * childrenTypes[index];
}

int ParticleDecayMode::getChildPdgCode(unsigned int index) const
{
  return childrenPdgCodes[index];
}

ostream & ParticleDecayMode::printProperties(ostream & os)
{
  os << " Br:" << branchingRatio << "  ";
  for (unsigned int k=0; k<childrenPdgCodes.size(); k++)
    {
    os << childrenPdgCodes[k]  << "  ";
    }
  os << endl;
  return os;
}


class MessageLogger
{
public:

  enum Severity   { Unknown, Trace, Debug, Info, Warning, Error, Fatal};

  Severity  reportLevel;      //!< Minimal error level reported by object instances inheriting from this class.
  Severity  holdReportLevel;  //!< Internal variable used internally to set/reset the nominal report level selected by the user.
  String   className;          //!< name of the class of this object instance
  String   instanceName;       //!< name given to this object instance
  String   fctName;            //!< name of the function initiating messages.

protected:

  static String unknownSeverityName;
  static String traceSeverityName;
  static String startSeverityString;
  static String endSeverityString;
  static String debugSeverityName;
  static String infoSeverityName;
  static String warningSeverityName;
  static String errorSeverityName;
  static String fatalSeverityName;

public:

  //! CTOR
  MessageLogger(Severity selectedLevel=Info) :
  reportLevel( selectedLevel ),
  holdReportLevel(selectedLevel),
  className("MessageLogger"),
  instanceName("MessageLogger"),
  fctName("CTOR")
  {   }


  //! DTOR
  virtual ~MessageLogger()
  { }

  //!
  //! Set the desired reported level used by instances (objects) of this class or subclasses.
  //!
  void setSeverityLevel(Severity requiredLevel)
  {
  reportLevel = requiredLevel;
  }

  //!
  //! Get the desired reported level used by instances (objects) of this class or subclasses.
  //!
  Severity getSeverityLevel() const
  {
  return reportLevel;
  }

  //!
  //! Set the hold level .
  //!
  void holdSeverityLevel(Severity requiredLevel)
  {
  holdReportLevel = reportLevel;
  reportLevel     = requiredLevel;
  }

  //!
  //!Restore the nominal report level requested by the user.
  //!
  void restoreSeverityLevel()
  {
  reportLevel = holdReportLevel;
  }

  //!
  //!Append the given string to the class name sequence of this object instance.
  //!
  inline void appendClassName(const String &  _className)
  {
  className += "/";
  className += _className;
  }

  //!
  //!Set the class name  of this object.
  //!
  inline void setClassName(const String &  _className)
  {
  className = _className;
  }

  //!
  //!Set the  name  of this object .
  //!
  inline void setInstanceName(const String &  _instanceName) { instanceName = _instanceName;  }

  //!
  //!Set the  name  of the function to report as message emitter.
  //!
  inline void setFunctionName(const String &  _fctName) { fctName = _fctName;   }

  //!
  //!Get the class name  of this object.
  //!
  inline const String &  getClassName()     const    { return className; }

  //!
  //!Get the  name  of this object.
  //!
  inline const String &  getInstanceName()  const    { return instanceName; }

  //!
  //!Get the  name  of the function reporting messages.
  //!
  inline const String &  getFunctionName() const    { return fctName; }

  //!
  //!Issue a debug message if the severity level is not below Debug.
  //!
  inline bool reportDebug(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Debug,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportTrace(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a function start message if the severity level is not below Trace.
  //!
  bool reportStart(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Trace.
  //!
  bool reportEnd(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Trace,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Info.
  //!
  bool reportInfo(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Info,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Warning.
  //!
  bool reportWarning(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Warning,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Error.
  //!
  bool reportError(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Error,className,instanceName,fctName,output);
  }

  //!
  //!Issue a debug message if the severity level is not below Fatal.
  //!
  bool reportFatal(const String &  fctName, std::ostream & output=std::cout) const
  {
  return report(Fatal,className,instanceName,fctName,output);
  }

  bool report(Severity severity, const String &  className, const String &  instanceName, const String &  fctName, std::ostream & output) const;

  ClassDef(MessageLogger,0)
};

String MessageLogger::unknownSeverityName   = "<Unknown>";
String MessageLogger::traceSeverityName     = "<Trace>";;
String MessageLogger::startSeverityString   = "<Start>";
String MessageLogger::endSeverityString     = "<End>";
String MessageLogger::debugSeverityName     = "<Debug>";
String MessageLogger::infoSeverityName      = "<Info>";
String MessageLogger::warningSeverityName   = "<Warning>";
String MessageLogger::errorSeverityName     = "<Error>";
String MessageLogger::fatalSeverityName     = "<Fatal>";

bool MessageLogger::report(Severity severity, const String &  className, const String &  instanceName, const String &  fctName, std::ostream & output) const
{
  if (reportLevel<=severity)
    {
    switch (severity)
      {
        default:
        case Unknown:  output << traceSeverityName; break;
        case Trace:    output << traceSeverityName; break;
        case Debug:    output << debugSeverityName; break;
        case Info:     output << infoSeverityName; break;
        case Warning:  output << warningSeverityName; break;
        case Error:    output << errorSeverityName; break;
        case Fatal:    output << fatalSeverityName; break;
      }
    std::cout << "  " << className << "[" << instanceName << "]::" << fctName << ": ";
    return true;
    }
  else
    return false;
}


class Parser : public MessageLogger
{
public:

  Parser();
  Parser(MessageLogger::Severity severity);

  virtual ~Parser() {}

  //!
  //! Open the root file named "fileName" located on the path "inputPath", using options specified by "ioOption".
  //! @param inputPath path where to find or create the file.
  //! @param fileName name of the root file to open for i/o.
  //! @param ioOption i/o options.
  //! @return Pointer to the file if successfully open or a null pointer if the file could not be opened.
  //!
  ifstream * openInputFile(const String & inputPath, const String & fileName, const String & extension, const String & ioOption="");
  ofstream * openOutputFile(const String & outputPath, const String & fileName, const String & extension, const String & ioOption="");

  String makeFileName(const String & inputPath,
                      const String & fileName,
                      const String & extension);

protected:

  //  String inputPath;
  //  String inputFile;
  //  String outputPath;
  //  String outputFile;

  ClassDef(Parser,0)

};


class ParticleDbParser : public Parser
{
public:

  ParticleDbParser();
  ParticleDbParser(MessageLogger::Severity _severity);
  virtual ~ParticleDbParser() {}

  virtual int  read(ParticleDb    & _particleDb,
                    const String  & _inputPath,
                    const String  & _inputFileName,
                    const String  & _extension = ".ini");

};

ParticleDbParser::ParticleDbParser()
:
Parser()
{ }

ParticleDbParser::ParticleDbParser(MessageLogger::Severity _severity)
:
Parser(_severity)
{ }

int  ParticleDbParser::readChunShen(Configuration & _configuration,
                                    const String  & _inputPath,
                                    const String  & _inputFileName,
                                    const String  & _extension)
{
  ifstream * inputDbFile = openInputAsciiFile(particleDbImportPath,particleDbImportFile,".dat");
  if (!inputDbFile)
    {
    if (reportError(__FUNCTION__))
      {
      cout << "Could not open ParticleDb file:" << particleDbImportPath << particleDbImportFile << endl;
      postTaskError();
      return;
      }
    }
  ifstream & inputFile = *inputDbFile;

  string name, title;
  double mass, width, decayBranchingRatio;
  int pdgCode, gSpin, gIsospin, baryonNumber, strangeNumber, charm, bottom, charge;
  int nDecayModes, decayNpart, dummy_int;
  int decayPart[5] = {0, 0, 0, 0, 0};
  ParticleType * particleType;
  ParticleType * antiParticleType;
  String theName;
  String theAntiName;
  String theTitle;
  String theAntiTitle;

  ParticleDb * particleDb = new ParticleDb();
  ParticleDb::setDefaultParticleDb(particleDb);

  while (1)
    {
    if (reportDebug(__FUNCTION__))
      cout << "Reading in ParticleType." << endl;
    inputFile >> pdgCode;
    if (inputFile.eof())
      {
      if (reportDebug(__FUNCTION__))
        cout << "Reached EOF." << endl;
      break;
      }
    inputFile >> name;
    inputFile >> mass;
    inputFile >> width;
    inputFile >> gSpin;
    inputFile >> baryonNumber;
    inputFile >> strangeNumber;
    inputFile >> charm;
    inputFile >> bottom;
    inputFile >> gIsospin;
    inputFile >> charge;
    inputFile >> nDecayModes;
    inputFile >> title;
    theName  = name;
    theTitle = title;
    particleType= new ParticleType(pdgCode, theName, theTitle, mass, width, gSpin, baryon, strangeNumber,
                                   charm, bottom, gIsospin, charge);
    particleDb->addParticleType(particleType);

    if (particleType->isFermion())
      {
      theAntiName  = "Anti-";
      theAntiName  += theName;
      theAntiTitle = "#bar ";
      theAntiTitle += theTitle;
      antiParticleType =  new ParticleType(-pdgCode, theAntiName, theAntiTitle, mass, width, gSpin,
                                           -baryon, -strangeNumber, -charm, -bottom, gIsospin,
                                           -charge);
      particleDb->addParticleType(antiParticleType);
      }
    // read decay information
    for (int j = 0; j < nDecayModes; j++)
      {
      inputFile >> dummy_int;
      inputFile >> decayNpart;
      inputFile >> decayBranchingRatio;
      inputFile >> decayPart[0];
      inputFile >> decayPart[1];
      inputFile >> decayPart[2];
      inputFile >> decayPart[3];
      inputFile >> decayPart[4];
      decayNpart = abs(decayNpart);
      std::vector<int> children;
      std::vector<int> antiChildren;
      for (int k=0; k<decayNpart; k++)
        {
        children.push_back(decayPart[k]);
        }
      if (!particleType->isFermion())
        {
        particleType->addDecayMode(decayBranchingRatio,children);
        }
      else
        {
        for (int k=0; k<decayNpart; k++)
          {
          antiChildren.push_back(particleDb->findPdgCode(decayPart[k])->getAntiParticlePdgCode());
          }
        particleType->addDecayMode(decayBranchingRatio,children);
        antiParticleType->addDecayMode(decayBranchingRatio,antiChildren);
        }
      }
    }
  //cout << "Reading in ParticleType resonance decay table.. - 5- " << endl;
  inputFile.close();

  if (reportInfo(__FUNCTION__))
    cout << "Total number of particles read: " <<  particleDb->getNumberOfTypes() << endl;
  particleDb->resolveTypes();
  particleDb->sortByMass();
}

int Test()
{
  ParticleDb  particleDb;
  ParticleDbParser parser;
  parser.readChunShen(particleDb,"/Users/aa7526/Documents/GitHub/CAP3.0/src/EOS","pdg.dat","");
  return 0;
}
