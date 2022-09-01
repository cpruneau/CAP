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
#include "Task.hpp"
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
//!GeneratorType
//! - 0 : Uniform,
//! - 1 : WoodsSaxon
//! - 2 : Exponential
//! - 3 : Gaussian
//! - 4 : DoubleGaussian
//!
class NucleusGenerator  : public  Task
{

public:

  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task (currently a placeholder not used)
  //! @param _particleFilters Array of particle filters to be used by this task -- use this filter to limit the range of particles read into CAP arrays.
  //! @param _reportLevel Message log level to be used by this task.
  //!
  NucleusGenerator(const TString & _name,
                   Configuration & _configuration);

  virtual ~NucleusGenerator();

  //!
  //! Set default configuration parameters of this nucleus generator
  //!
  //!
  void setDefaultConfiguration();

  //!
  //! Initialize this nucleus generator
  //!
  //!
  virtual void initialize();

  virtual void execute();

  virtual void generate(Nucleus & nucleus, double xShift);
  virtual void generate(double & r, double & cosTheta, double & phi);
  virtual void saveHistograms();

protected:

  int    gType;
  int    nR;
  double minR;
  double maxR;
  double parA;
  double parB;
  double parC;
  bool   useRecentering;
  bool   useNucleonExclusion;
  double exclusionRadius;
  double exclusionRadiusSq;

  TH1 * rDensity;
  TH1 * rProfile;
  TH1 * rProfileGen;


  ClassDef(NucleusGenerator,0)
  
};

#endif /* NucleusGenerator_hpp */
