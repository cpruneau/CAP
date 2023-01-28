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
#ifndef CAP__RadialBoostTask
#define CAP__RadialBoostTask
#include <TParameter.h>
#include "TFile.h"
#include "Task.hpp"
#include "Event.hpp"
#include "CollisionGeometry.hpp"
#include "RadialBoostHistos.hpp"

namespace CAP
{


//!
//!Task operates a radial boost on all the particles in its incoming data stream.
//!
class RadialBoostTask : public Task
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
  RadialBoostTask(const String & _name,
                  Configuration & _configuration,
                  vector<EventFilter*>&    _eventFilters,
                  vector<ParticleFilter*>& _particleFilters);
  
  //!
  //!DTOR
  //!
  virtual ~RadialBoostTask() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();
  
  //!
  //! Creates the histograms  filled by this task at execution
  //!
  virtual void createHistograms();

  //!
  //! Loads the histograms retquired by this task at execution
  //!
  virtual void loadHistograms(TFile * inputFile);
  

protected:

  double param_b;
  double param_a;
  double betaMaximum;

  ClassDef(RadialBoostTask,0)
};

}

#endif /* CAP__RadialBoostTask */
