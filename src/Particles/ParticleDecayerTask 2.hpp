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
#ifndef CAP__ParticleDecayerTask
#define CAP__ParticleDecayerTask
#include "ParticleDecayer.hpp"
#include "EventTask.hpp"
#include "Event.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"

namespace CAP
{

//!
//! Task handles the decay of all decayable particles in the incoming event stream. At execution time, i.e., for each event encountered,
//! each particle of the input stream is examined to determine whether it should be decayed. Particle deemed decayable are passed to the
//! decay handler (decayer) and decayed.  Currently, the decayer handles two and three particle decays. Decayed particles are kept in the
//! event stream but tagged as decayed. Child particles are inserted in the stream and tagged as "live" (undecayed).  Child particles that
//! are themselves decayable are decayed iteratively. 
//!
class ParticleDecayerTask : public EventTask
{

protected:
  
  ParticleDecayer decayer; //!< Particle decay handler

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
  ParticleDecayerTask(const String & _name,
                      const Configuration &   _configuration,
                      vector<EventFilter*>    _eventFilters,
                      vector<ParticleFilter*> _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~ParticleDecayerTask() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();
  
  //!
  //!Getthe decay handle (decayer) used by this task.
  //!
  ParticleDecayer & getParticleDecayer() { return decayer;}

  ClassDef(ParticleDecayerTask,0)
};

}

#endif /* CAP__ParticleDecayerTask */
