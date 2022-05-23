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
#ifndef CAP__DerivedHistoIterator
#define CAP__DerivedHistoIterator
#include "Task.hpp"

//!
//!Task calculates the statistical errors of all histograms of the selected files based on the subsample method.
//!The input file are selected based on a name template. All files read are assumed to have the same histogram structure (named objects).
//!They are read sequentially and added into a new histogram collection. The summed collection is then used to computed rms deviates
//!which are then set as errors in the histograms saved on output. The name of the output file is generated based on the template name
//!and a selected appendString name. This class should NOT be run as a subtask of a more complex task in its current form.
//!
class DerivedHistoIterator : public Task
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  DerivedHistoIterator(const TString &       _name,
                        const Configuration & _configuration,
                        MessageLogger::LogLevel debugLevel);
  
  //!
  //! DTOR
  //!
  virtual ~DerivedHistoIterator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //! The configuration stipulates the input and output paths, the file template names and the output file append string.
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Execute the calculation for all registered tasks and all the files they select.
  //!
  virtual void execute();

  ClassDef(DerivedHistoIterator,0)
};

#endif /* CAP__DerivedHistoIterator */
