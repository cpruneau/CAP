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
#ifndef CAP__CollisionGeometryGenerator
#define CAP__CollisionGeometryGenerator
#include "EventTask.hpp"
#include "Event.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"
#include "NucleusGenerator.hpp"

namespace CAP
{


//!
//!This class implements a simple heavy-ion collision geometry generator. The geometry of the nuclei is randomized and can be set to follow various density profile (see class NucleusGenerator).
//!This collisions generator assumes the elementary objects (either partons or nucleons) travel on straight line trajectories parallel to the z-axis (the beam axis).
//!Use the configuration file to set values required at run time.
//!
class CollisionGeometryGenerator : public EventTask
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
  CollisionGeometryGenerator(const String & _name,
                             const Configuration & _configuration,
                             vector<EventFilter*> & _eventFilters,
                             vector<ParticleFilter*>& _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~CollisionGeometryGenerator();

  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Initialize this task based on the configuration and class variable specified at construction
  //!
  virtual void initialize();
  
  //!
  //! Clears this task thereby rendering it non initialized.
  //!
  virtual void clear();

  //!
  //! Reset this task and the objects it controls.
  //!
  virtual void reset();

  //!
  //! Execute this task
  //!
  virtual void createEvent();

  //!
  //! Get nucleus A generated (produced) by this task. Nucleus A changes event by event
  //!
  Nucleus & getNucleusA() { return eventStreams[0]->getNucleusA(); }

  //!
  //! Get nucleus B generated (produced) by this task. Nucleus B changes event by event
  //!
  Nucleus & getNucleusB() { return eventStreams[0]->getNucleusB(); }
  
  //!
  //! Get the nucleus generator used to generate nucleus A.
  //!
  NucleusGenerator  * getNucleusGeneratorA() { return nucleusGeneratorA; }

  //!
  //! Get the nucleus generator used to generate nucleus B.
  //!
  NucleusGenerator  * getNucleusGeneratorB() { return nucleusGeneratorB; }

protected:

  //!
  //! Nucleus generator used to generate nucleus A.
  //!
  NucleusGenerator  * nucleusGeneratorA;

  //!
  //! Nucleus generator used to generate nucleus B.
  //!
  NucleusGenerator  * nucleusGeneratorB;
  
  //!
  //! Min and max values of the impact parameter used by this generator instance
  //!
  double minB, minBSq, maxB, maxBSq;
  
  //!
  //! Nucleon-nulceon (or parton-parton) cross section used by this instance
  //!
  double nnCrossSection;

  //!
  //! Square of the maximum distance used internally in the generation of collisions.
  //!
  double maxNNDistanceSq;

  ClassDef(CollisionGeometryGenerator,0)
};

}


#endif /* CAP__CollisionGeometryGenerator */
