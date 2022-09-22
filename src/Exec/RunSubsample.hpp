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
#ifndef CAP__RunSubsample
#define CAP__RunSubsample
#include "Task.hpp"

class RunSubsample : public Task
{
public:
  
  RunSubsample(const TString & _name,
              Configuration & _configuration);

  //!
  //! dtor
  //! 
  virtual ~RunSubsample() {}

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

  void addBaseSubSampleTask(const TString & basePath,
                            const TString & bunchLabel,
                            int   nBunches,
                            const TString & subPath,
                            int   maximumDepth,
                            const TString & taskType);

  void addDerivedSubSampleTask(const TString & basePath,
                               const TString & bunchLabel,
                               int   nBunches,
                               const TString & subPath,
                               int   maximumDepth,
                               const TString & taskType);

  void addBalFctSubSampleTask(const TString & basePath,
                              const TString & bunchLabel,
                              int   nBunches,
                              const TString & subPath,
                              int   maximumDepth,
                              const TString & taskType);


  ClassDef(RunSubsample,0)
};

#endif /* CAP_RunAnalysis */


