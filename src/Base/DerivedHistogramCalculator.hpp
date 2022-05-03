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
#include "FileTaskIterator.hpp"
using namespace std;

class DerivedHistogramCalculator : public FileTaskIterator
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
  DerivedHistogramCalculator(const TString &          _name,
                             Configuration &          _configuration,
                             vector<EventFilter*>     _eventFilters,
                             vector<ParticleFilter*>  _particleFilters,
                             LogLevel                 _selectedLevel);
  
  //!
  //! DTOR
  //!
  virtual ~DerivedHistogramCalculator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration()
  
  //!
  //! Run this iterator task
  //!
  virtual void run();

  ClassDef(DerivedHistogramCalculator,0)
};

#endif /* CAP__DerivedHistogramCalculator */
