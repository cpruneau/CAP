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

#ifndef CAP__NucleusType
#define CAP__NucleusType
#include "ParticleType.hpp"

namespace CAP
{

//!
//!This class defines the notion of nucleus type used in CAP. It is based on the ParticleType
//!class to avoid repetition of properties even though many elementary particle properties are not
//! relevant to nuclei. A nucleus type features a "z"  corresponding to the atomic number of the
//! element and an "a" corresponding to the mass number of the nucleus. Although nuclei
//! feature a large  number of important properties, these are here limited to the deformation
//! parameter $\beta$ sometimes used in the studies of the impact of the shape (departure
//! from spherocity) on global event properties, including in particular flow properties.
//!
//! Within this package, particle types are given a definite identification code based on the  PDG MC particle numbering
//! scheme. This scheme  uses integer numbers smaller than 10 million. Nuclei are thus identified and indexed
//! based on an extension of the PDG scheme yielding integers larger than 10E6 as follows
//!
//! pdgId = A*10,000,000 + Z*10,000 + S*10.
//!
//!where A is the mass number, Z the atomic number, and S the strangeness of the nucleus.  The mass number
//!is thence rapidly obtained by integer arithmetic as
//!  A = pdgId/10000000
//!  whereas the atomic mass is obtained as
//!  Z =  (pdgId%10000000)/10000
//!  and the stangeness is given by
//!  S =  (pdgId%10000000)%10000
//!
//!Note: Positive integers are used for "matter", i.e., normal nuclei, whereas negative integers are used for anti-nuclei.
//!
//!To do: fix values of spin and isospin degeneracies
//!
class NucleusType : ParticleType
{

public:
  
  //! Default CTOR
  NucleusType();

  //!
  //! Detailed CTOR
  //!
  //! @param _name  name of nucleus
  //! @param _title  symbol of the nucleus (displayed as a Latex word).
  //! @param _atomicNumber   atomic  number
  //! @param _massNumber  mass number
  //! @param _strangenessNumber  strangeness number (hypernuclei)
  //! @param _mass  mass of the particle (GeV/c^2)
  //! @param _spin  spin degeneracy
  //! @param _isospin isospin generacy
  //! @param _lifeTime isospin generacy
  //!
  NucleusType(const String & _name,
              const String & _title,
              int    _atomicNumber,
              int    _massNumber,
              int    _strangenessNumber,
              double _mass,
              double _spin,
              double _isospin,
              double _lifeTime,
              double _deformationBeta=0);


  //!
  //! COPY CTOR
  //!
  NucleusType(const NucleusType & source);
  
  //!
  //! DTOR
  //!
  virtual ~NucleusType() {}
  
  NucleusType & operator=(const NucleusType & source);
  double getZ() const;
  double getAtomicNumber() const;
  double getA() const;
  double getMassNumber() const;
  double getN() const;
  double getNeutronNumber() const;
  double getBeta() const;
  double getDeformationBeta() const;

  void setZ(double _atomicNumber);
  void setAtomicNumber(double _atomicNumber);
  void setA(double _massNumber);
  void setMassNumber(double _massNumber);
  void setS(double _strangenessNumber);
  void setStrangenessNumber(double _strangenessNumber);

  void setBeta(double _deformationBeta);
  void setDeformationBeta(double _deformationBeta);
  double getBindingEnergy() const;

  bool isNucleus() const;
  bool isAntiNucleus() const;

protected:

  double beta;    //!< deformation parameter

  static double protonMass;
  static double neutronMass;
  static double deuteronMass;
  static double tritonMass;
  static double helium3Mass;
  static double helium4Mass;
  static double au197Mass;
  static double pb208Mass;
  static double kilogramToMeV;
  static double amu_kg;
  static double amu_GeV;

  static  NucleusType * protonType;      //!< Pointer to a singleton object coding a proton type
  static  NucleusType * neutronType;     //!< Pointer to a singleton object coding a neutron type
  static  NucleusType * deuteronType;    //!< Pointer to a singleton object coding a deuteron type
  static  NucleusType * tritonType;      //!< Pointer to a singleton object coding a triton type
  static  NucleusType * he3Type;         //!< Pointer to a singleton object coding a 3He nucleus type
  static  NucleusType * he4Type;         //!< Pointer to a singleton object coding a 4He nucleus type
  static  NucleusType * auType;          //!< Pointer to a singleton object coding an 197Au (gold) nucleus type
  static  NucleusType * pbType;          //!< Pointer to a singleton object coding a 208Pb  (lead) nucleus type

  ClassDef(NucleusType,0)
};

} // namespace CAP

#endif  

