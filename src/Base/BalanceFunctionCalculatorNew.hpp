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
#ifndef CAP__BalanceFunctionCalculator
#define CAP__BalanceFunctionCalculator
#include "Task.hpp"

//!
//!Task calculates combinations of histograms obtained with different filters to compute balance functions.
//!
class BalanceFunctionCalculator : public Task
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  BalanceFunctionCalculator(const TString &       _name,
                        const Configuration & _configuration,
                        MessageLogger::LogLevel debugLevel);
  
  //!
  //! DTOR
  //!
  virtual ~BalanceFunctionCalculator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //! The configuration stipulates the input and output paths, the file template names and the output file append string.
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Execute the calculation for all registered tasks and all the files they select.
  //!
  virtual void execute();

  ClassDef(BalanceFunctionCalculator,0)
};

#endif /* CAP__BalanceFunctionCalculator */
