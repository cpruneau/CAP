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
#ifndef CAP__RunDerivedCalculation
#define CAP__RunDerivedCalculation
#include "Task.hpp"

namespace CAP
{


class RunDerivedCalculation : public Task
{
public:
  
  RunDerivedCalculation(const String & _name,
                        Configuration & _configuration);

  //!
  //! dtor
  //! 
  virtual ~RunDerivedCalculation() {}

  //!
  //! Initialize the configuration parameter of the task to their default value;
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Configure  this analysis task
  //!
  void configure();

  //!
  //! Execute this task
  //!
  void execute();

  ClassDef(RunDerivedCalculation,0)
};

}

#endif /* CAP_RunDerivedCalculation */


