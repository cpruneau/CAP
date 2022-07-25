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
#ifndef CAP__ParticleFilter
#define CAP__ParticleFilter
#include "TString.h"
#include "Particle.hpp"
#include "Filter.hpp"

class ParticleFilter : public Filter<Particle>
{
public:

  ParticleFilter();
  ParticleFilter(const ParticleFilter & otherFilter);
  ParticleFilter & operator=(const ParticleFilter & otherFilter);
  virtual ~ParticleFilter() {}
  virtual bool accept(const Particle & particle);

  //!
  //!  Create a filter accepting only the given pdg code with the given kinematical parameters
  //!
  static  ParticleFilter * createBaryonFilter(int pdg, const TString & name, const TString & title,
                                              bool filteringOnPt,  double minPt,  double maxPt,
                                              bool filteringOnEta, double minEta, double maxEta,
                                              bool filteringOnY,   double minY,   double maxY);

  //!
  //!  Create pos and neg hadron  filters with the given kinematical parameters
  //!
  static vector<ParticleFilter*> createChargedHadronFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                            bool filteringOnEta, double minEta, double maxEta,
                                                            bool filteringOnY,   double minY,   double maxY);

  //!
  //!  Create pi+, K+, p+, pi-, K-, p- filters with the given kinematical parameters
  //!
  static vector<ParticleFilter*> createPlusMinusHadronFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                              bool filteringOnEta, double minEta, double maxEta,
                                                              bool filteringOnY,   double minY,   double maxY);

  //!
  //! Create low mass (detectable) +B and -B baryon filters with the given kinematical parameters
  //! Baryon (and their respective anti-baryons) selected include
  //! proton  : 2212
  //! neutron : 2112
  //! lambda  : 3122
  //! Sigma+  : 3222
  //! Sigma0  : 3212
  //! Sigma-  : 3112
  //! Xi0     : 3322
  //! XiM     : 3312
  //! Omega-  : 3334
  //!
  static vector<ParticleFilter*> createBaryonFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                     bool filteringOnEta, double minEta, double maxEta,
                                                     bool filteringOnY,   double minY,   double maxY);

  ClassDef(ParticleFilter,0)
};

#endif /* CAP__ParticleFilter */

