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

  static vector<ParticleFilter*> createChargedHadronFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                            bool filteringOnEta, double minEta, double maxEta,
                                                            bool filteringOnY,   double minY,   double maxY);
  static vector<ParticleFilter*> createPlusMinusHadronFilters(bool filteringOnPt,  double minPt,  double maxPt,
                                                              bool filteringOnEta, double minEta, double maxEta,
                                                              bool filteringOnY,   double minY,   double maxY);
//  static vector<ParticleFilter*> createMultiplePdgFilters(bool filteringOnPt,  double minPt,  double maxPt,
//                                                          bool filteringOnEta, double minEta, double maxEta,
//                                                          bool filteringOnY,   double minY,   double maxY,
//                                                          vector<int> & pdgAccepted);
//
//  static vector<ParticleFilter*> createBaryonFilters(bool filteringOnPt,  double minPt,  double maxPt,
//                                                     bool filteringOnEta, double minEta, double maxEta,
//                                                     bool filteringOnY,   double minY,   double maxY);

  ClassDef(ParticleFilter,0)
};

#endif /* CAP__ParticleFilter */

