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
#ifndef CAP__AACollisionGenerator
#define CAP__AACollisionGenerator
#include "EventTask.hpp"
#include "Event.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"
#include "CollisionGeometry.hpp"

namespace CAP
{


//!
//! \brief MC generator producing AA collisions as superpositions of nBinary nucleon-nucleon collisions
//! \details <h1> AACollisionGenerator </h1>
//! Task  designed to produce AA collisions as superpositions of nBinary nucleon-nucleon collisions.
//! The does not generate the nucleon-nucleon collisions but instead call another task class that does.
//! This task assumes an event is already loaded with "interactions" (instance of the class Particle) that carry a type for the
//! interactions, a momentum, and a position. Nucleon-nucleon collision positions can for instance be generated with the CollisionGeometry class.
//! \author Claude A. Pruneau, Wayne State University
//! \version 1.0
//! \date 2022
//! \bug  No known bug
//! \warning Improper use can crash your application
//! \copyright Claude A. Pruneau, All rights reserved.
//!
class AACollisionGenerator : public EventTask
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
  AACollisionGenerator(const String * _name,
                       const Configuration & _configuration,
                       vector<EventFilter*> & _eventFilters,
                       vector<ParticleFilter*>& _particleFilters);

  //!
  //! DTOR
  //!
  virtual ~AACollisionGenerator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Initialize this task
  //!
  virtual void initialize();
  
  //!
  //! Finalize this task
  //!
  virtual void finalize();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void createEvent();

  //!
  //! Get the collision generator object used by this task
  //!
  NucleonNucleonCollisionGenerator * getCollisionGenerator()
  {
  return nnCollisionGenerator;
  }

  //!
  //! Set the collision generator object used by this task
  //!
  void setNucleonNucleonCollisionGenerator(NucleonNucleonCollisionGenerator  * _nnCollisionGenerator);

protected:

  //!
  //! Collision generator object used by this task
  //!
  NucleonNucleonCollisionGenerator * nnCollisionGenerator;
  
  ClassDef(AACollisionGenerator,0)
};

}


#endif /*CAP__AACollisionGenerator*/
