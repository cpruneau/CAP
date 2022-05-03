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
#ifndef CAP__NucleusGenerator
#define CAP__NucleusGenerator
#include "TString.h"
#include "TRandom.h"
#include "TMath.h"
#include "TH1.h"
#include "Nucleus.hpp"

//!
//! # NucleusGenerator
//!
//! nNucleons     : number of nucleons
//! nProtons      : number of protons
//! nNeutrons     : number of neutron
//!
//! x,y,z         : cartesian coordinates relative to its container (collision)
//! radius        : nominal radius of the NucleusGenerator used for generation purposes
//!
class NucleusGenerator  : public  TObject
{
public:

  enum GeneratorType   { Uniform, WoodsSaxon, Exponential, Gaussian, DoubleGaussian };

  TString generatorTypeName;
  int    generatorType;
  int    nR;
  double minR, maxR;
  double parA;
  double parB;
  double parC;
  TH1 * rDensity;
  TH1 * rProfile;
  TH1 * rProfileGen;

  bool   useRecentering;
  bool   useNucleonExclusion;
  double exclusionRadius;
  double exclusionRadiusSq;

public:

  NucleusGenerator();
  virtual ~NucleusGenerator();

  virtual void initialize(const TString & _generatorTypeName,
                          int  _generatorType,
                          double _parA, double _parB, double _parC,
                          int _nR, double _minR, double _maxR,
                          bool _useRecentering,
                          bool _useNucleonExclusion,
                          double _exclusionRadius);
  virtual void generate(Nucleus & nucleus, double xShift);
  virtual void generate(double & r, double & cosTheta, double & phi);
  virtual void saveHistograms();

  ClassDef(NucleusGenerator,0)
  
};

#endif /* NucleusGenerator_hpp */
