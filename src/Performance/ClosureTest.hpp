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
#ifndef CAP__ClosureTest
#define CAP__ClosureTest
#include "Task.hpp"

class ClosureTest : public Task
{
  
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  ClosureTest(const TString &          _name,
              const Configuration &    _configuration,
              MessageLogger::LogLevel  _reportLevel);
  
  //!
  //! DTOR
  //!
  virtual ~ClosureTest() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();
  
  ClassDef(ClosureTest,0)
};

#endif /* CAP__ClosureTest */
