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
#ifndef CAP__RunAnalysis
#define CAP__RunAnalysis
#include "EventTask.hpp"
#include "Event.hpp"
#include "Particle.hpp"
#include "ParticleType.hpp"

namespace CAP
{


class RunAnalysis : public EventTask
{
public:
  
  RunAnalysis(const String & _name,
              const Configuration & _configuration);

  //!
  //! dtor
  //! 
  virtual ~RunAnalysis() {}

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

  ClassDef(RunAnalysis,0)
};

}

#endif /* CAP_RunAnalysis */


