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
#include <iostream>
#include "TClass.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH3D.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TFile.h"
#include "TString.h"
#include "Task.hpp"
using namespace std;

class ClosureTest : public Task
{
public:

  // Ratio      : detectorLevel/generatorLevel
  // Difference : detectorLevel - generatorLevel
  // Relative   : (detectorLevel - generatorLevel)/generatorLevel

  enum ClosureTestMethod    { Ratio, Difference, Relative };


  TString inputPath;
  TString outputPath;
  TString generatorLevelFileName;  // Truth
  TString detectorLevelFileName;   // Measured
  TString closureFileName;         // Comparison
  ClosureTestMethod selectedMethod;

  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle filters to be used by this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  ClosureTest(const TString &   _inputPath,
              const TString &   _generatorLevel,
              const TString &   _detectorLevel,
              const TString &   _outputPath,
              const TString &   _closureFileName,
              ClosureTestMethod _selectedMethod=Ratio,
              MessageLogger::LogLevel debugLevel=Info);

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
