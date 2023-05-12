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
#ifndef CAP__EventPlaneRandomizerTask
#define CAP__EventPlaneRandomizerTask
#include "EventTask.hpp"

namespace CAP
{


//!
//! This class implements a generic event plane randomizer. All particles composing an event are azimuthally rotated by a randome angle $\varphi$ generated event by event in the range $[0,2\pi]$.
//!  Event filters and particles filters may be used to tailor the bahvior of this randomizer.
//!
class EventPlaneRandomizerTask : public EventTask
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
  EventPlaneRandomizerTask(const String & _name,
                           const Configuration & _configuration,
                           vector<EventFilter*> & _eventFilters,
                           vector<ParticleFilter*> & _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~EventPlaneRandomizerTask() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void createEvent();

  ClassDef(EventPlaneRandomizerTask,0)
};

} // namespace CAP

#endif /* CAP__EventPlaneRandomizerTask */
