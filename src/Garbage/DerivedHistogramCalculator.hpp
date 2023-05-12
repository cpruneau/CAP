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
#ifndef CAP__DerivedHistogramCalculator
#define CAP__DerivedHistogramCalculator
#include "EventTask.hpp"
#include "Event.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"

namespace CAP
{


//!
//! Base class for derived histogram calculators. Derived histogram calculators handle saving histograms
//! slightly differently than regular event analysis task. This subclass thus implement a slightly modified
//! version of the exportHistograms() method. //!
//!
class DerivedHistogramCalculator : public EventTask
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
  DerivedHistogramCalculator(const String & _name,
                             const Configuration &    _configuration,
                             vector<EventFilter*>   & _eventFilters,
                             vector<ParticleFilter*>& _particleFilters);

  //!
  //! DTOR
  //!
  virtual ~DerivedHistogramCalculator() {}

  //!
  //! Saves the histograms computed by this task. 
  //!
  virtual void exportHistograms();


  ClassDef(DerivedHistogramCalculator,0)
};

} // namespace CAP

#endif /* CAP__DerivedHistogramCalculator */
