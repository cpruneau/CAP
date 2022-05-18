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

ClassImp(NucleonNucleonCollisionGenerator);

NucleonNucleonCollisionGenerator::NucleonNucleonCollisionGenerator(const TString &         _name,
                                                                   const Configuration &   _configuration,
                                                                   vector<EventFilter*>&   _eventFilters,
                                                                   vector<ParticleFilter*>&_particleFilters,
                                                                   LogLevel                _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  appendClassName("NucleonNucleonCollisionGenerator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void NucleonNucleonCollisionGenerator::generate(Particle * parent)
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
}

