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
#ifndef CAP__ClosureIterator
#define CAP__ClosureIterator
#include "Task.hpp"

namespace CAP
{

//!
//!Task performs a closure test
//!
class ClosureIterator : public Task
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  ClosureIterator(const String & _name,
                  const Configuration & _configuration);
  
  //!
  //! DTOR
  //!
  virtual ~ClosureIterator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //! The configuration stipulates the input and output paths, the file template names and the output file append string.
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Execute the calculation for all registered tasks and all the files they select.
  //!
  virtual void execute();

  ClassDef(ClosureIterator,0)
};

} // namespace CAP

#endif /* CAP__ClosureIterator */
