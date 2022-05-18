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
#ifndef CAP__MeasurementPerformanceSimulator
#define CAP__MeasurementPerformanceSimulator
#include "Task.hpp"
#include "ParticleHistos.hpp"
#include "ParticlePerformanceSimulator.hpp"

//!
//! Task used to (fast) simulate the performance of measurement devices.
//! This is NOT a full performance simulator such as GEANT.  Instead, it uses
//! simplified models of (a) the efficiency of detection for the particles of interest in the measurement(s) and
//! (b) it simulates the measurement performance, i.e., the bias and resolution of the measurements
//! with fast simulators. The task first reviews which particles are accepted and considered "measured" using filters  implementing
//! the ParticleFilterEfficiency class. If accepted, particles are submitted to a smearer implementing the ParticlePerformanceSimulator
//! class and the momentum (pt, eta, and phi) of each accepted/detected particle is then smeared.
//! Particles that are accepted and smeared are inserted in event stream 1 (generator level particles are in stream 0).
//! It is then possible for performance analysis tasks to direct compare particles of the two stream,
//! carry out closure tests and so on.
//!
//!Configuration parameters of the MeasurementPerformanceSimulator class are as follows
//!
//!
//!
class MeasurementPerformanceSimulator : public Task
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle efficiency  filters to be used by this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  MeasurementPerformanceSimulator(const TString &         _name,
                                  const Configuration &   _configuration,
                                  vector<EventFilter*>    _eventFilters,
                                  vector<ParticleFilter*> _particleFilters,
                                  LogLevel                _selectedLevel=Info);
  
  //!
  //!DTOR
  //!
  virtual ~MeasurementPerformanceSimulator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Reviews all particles incoming on event stream 1, reviews which are accepted (acceptance and efficiency)
  //! and smears the momenta using the smearers associated with each of the particle filters. Smeared particles
  //! and events are inserted into event stream 2 for subsequent analysis by other tasks. 
  //!
  virtual void execute();

  //!
  //! Initialize this task
  //!
  virtual void initialize();

  //!
  //! Loads the histograms retquired by this task at execution
  //!
  virtual void loadHistograms(TFile * inputFile);

protected:
  
  bool  allEventsUseSameFilters;
  unsigned int nEventFilters;
  unsigned int nParticleFilters;
  ClassDef(MeasurementPerformanceSimulator,0)
};


#endif /* CAP__MeasurementPerformanceSimulator */
