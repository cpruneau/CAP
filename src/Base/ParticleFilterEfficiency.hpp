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
#ifndef CAP__ParticleFilterEfficiency
#define CAP__ParticleFilterEfficiency
#include "TMath.h"
#include "TH1.h"
#include "TString.h"
#include "Filter.hpp"
#include "Particle.hpp"

class ParticleFilterEfficiency : public Filter<Particle>
{
public:

  ParticleFilterEfficiency();
  ParticleFilterEfficiency(const ParticleFilterEfficiency & otherFilter);
  ParticleFilterEfficiency & operator=(const ParticleFilterEfficiency & otherFilter);
  virtual ~ParticleFilterEfficiency();
  virtual bool accept(const Particle & particle);

protected:

  TH1 * efficiency;

  ClassDef(ParticleFilterEfficiency,0)
};

#endif /* CAP__ParticleFilterEfficiency */
