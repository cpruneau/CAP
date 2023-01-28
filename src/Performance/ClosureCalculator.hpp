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
#ifndef CAP__ClosureCalculator
#define CAP__ClosureCalculator
#include "Task.hpp"


namespace CAP
{

class ClosureCalculator : public Task
{
  
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  ClosureCalculator(const String & _name,
                    Configuration & _configuration);
  
  //!
  //! DTOR
  //!
  virtual ~ClosureCalculator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();
  
  ClassDef(ClosureCalculator,0)
};

} // namespace CAP

#endif /* CAP__ClosureCalculator */
