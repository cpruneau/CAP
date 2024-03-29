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
#ifndef CAP__NucleonNucleonCollisionGenerator
#define CAP__NucleonNucleonCollisionGenerator
#include "EventTask.hpp"
#include "Event.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ParticleFilter.hpp"
#include "CollisionGeometry.hpp"

namespace CAP
{


//!
//! Generic and abstract task used for the generation of NN collisions
//!
class NucleonNucleonCollisionGenerator : public EventTask
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle filters to be used by this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  NucleonNucleonCollisionGenerator(const String & _name,
                                   const Configuration & _configuration,
                                   vector<EventFilter*>& _eventFilters,
                                   vector<ParticleFilter*>&_particleFilters);
  //!
  //! DTOR
  //!
  virtual ~NucleonNucleonCollisionGenerator(){}

  //!
  //! Generate a particle according to the task parameters set at construction and fill the given particle instance. 
  //!
  virtual void generate(Particle * parent);

  ClassDef(NucleonNucleonCollisionGenerator,0)
};

}

#endif /*CAP__NucleonNucleonCollisionGenerator */
