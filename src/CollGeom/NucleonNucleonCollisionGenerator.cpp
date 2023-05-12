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
#include "NucleonNucleonCollisionGenerator.hpp"
#include "TDatabasePDG.h"
using CAP::NucleonNucleonCollisionGenerator;

ClassImp(NucleonNucleonCollisionGenerator);

NucleonNucleonCollisionGenerator::NucleonNucleonCollisionGenerator(const String & _name,
                                                                   const Configuration & _configuration,
                                                                   vector<EventFilter*>& _eventFilters,
                                                                   vector<ParticleFilter*>&_particleFilters)
:
EventTask(_name, _configuration, _eventFilters, _particleFilters)
{
  appendClassName("NucleonNucleonCollisionGenerator");
}

void NucleonNucleonCollisionGenerator::generate(Particle * parent)
{
  incrementTaskExecuted();
}

