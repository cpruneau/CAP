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
#ifndef CAP__GaussianGeneratorTask
#define CAP__GaussianGeneratorTask
#include <TF2.h>
#include "Task.hpp"

//!
//!This class implements a simple weight calculator that can be used to simulate two particle correlations. 
//!
class GaussianGeneratorTask : public Task
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  GaussianGeneratorTask(const TString & _name,
                        Configuration & _configuration);

  //!
  //! DTOR
  //!
  virtual ~GaussianGeneratorTask();

  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Initialize this task. Initialization involves the instantiation of the 2D (TF2) function used in the particle generation.
  //!
  virtual void initialize();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();
  
  //!
  //!Get a pointer to the profile function used by this task.
  //!
  TF2  * getProfile() { return profile; }

protected:

  TF2 *profile;

    ClassDef(GaussianGeneratorTask,0)
};

#endif /* CAP__GaussianGeneratorTask */
