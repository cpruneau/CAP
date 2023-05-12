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
#ifndef CAP__EventVertexRandomizerTask
#define CAP__EventVertexRandomizerTask
#include "EventTask.hpp"

namespace CAP
{


//!
//! \class Task
//! \ingroup CAP

//! This class implements a generic event vertex  randomizer.
//! The event vertex is shifted arbitrary from its (0,0,0,0) nominal position, in the center of momentum frame of a collision, to a random location (t,x,y,z) chosen randomly
//! according to a specific randomization model (Gaussian) and determined by parameters selected at initialization.  The Gaussian randomization produces means
//! xAvg, yAvg, zAvg, and tAvg and assumes rms values xRms, yRms, zRms, tRms. These parameter values must be set using a configuration object submitted at construction
//! of a class instance.
//!

class EventVertexRandomizerTask : public EventTask
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
  EventVertexRandomizerTask(const String & _name,
                            const Configuration & _configuration,
                            vector<EventFilter*> & _eventFilters,
                            vector<ParticleFilter*>& _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~EventVertexRandomizerTask() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Initialize this task
  //!
  virtual void initialize();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void createEvent();
  
protected:
  
  double rConversion;
  double tConversion;
  
  double xAvg; //!< mean vertex position along the x-axis
  double yAvg; //!< mean vertex position along the y-axis
  double zAvg; //!< mean vertex position along the z-axis
  double tAvg; //!< mean vertex position along the t-axis
  double xRms; //!< rms of the vertex distribution along the x-axis
  double yRms; //!< rms of the vertex distribution along the y-axis
  double zRms; //!< rms of the vertex distribution along the z-axis
  double tRms; //!< rms of the vertex distribution along the t-axis
  
  ClassDef(EventVertexRandomizerTask,0)
};

} // namespace CAP

#endif /* CAP__EventVertexRandomizerTask */
