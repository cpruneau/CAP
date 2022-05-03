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
#ifndef CAP__DetectorFastSimulatorTask
#define CAP__DetectorFastSimulatorTask
#include "Task.hpp"

//! This class implements a generic fast simulator of the performance of a detection device.
//! The simulator is control by objects of the ParticleSmearer class which simulate the resolution of measurements devices.objects
//!
class DetectorFastSimulatorTask : public Task
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle filters to be used by this task
  //! @param _particleSmearers Array of particle  momentum/position resolution smearers to be used by this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  DetectorFastSimulatorTask(const TString &          _name,
                            Configuration &          _configuration,
                            vector<EventFilter*>     _eventFilters,
                            vector<ParticleFilter*>  _particleFilters,
                            vector<ParticleSmearer*> _particleSmearers,
                            LogLevel                 _selectedLevel=Info);
  
  //!
  //! DTOR
  //!
  virtual ~DetectorFastSimulatorTask() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();
 
protected:
  ClassDef(DetectorFastSimulatorTask,0)
};


#endif /* CAP__DetectorFastSimulatorTask */
