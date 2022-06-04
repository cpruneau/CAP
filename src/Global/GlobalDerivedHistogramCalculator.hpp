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
#ifndef CAP__GlobalDerivedHistogramCalculator
#define CAP__GlobalDerivedHistogramCalculator
#include "DerivedHistogramCalculator.hpp"
#include "GlobalHistos.hpp"
#include "ParticleAnalyzer.hpp"

//!
//! Task to compute derived particle histograms, i.e., histograms derived from single particle histograms.
//!
//!  \see ParticleAnalyzer for a list of the configuration parameters of this class.
//!
class GlobalDerivedHistogramCalculator : public DerivedHistogramCalculator
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle filters to be used by this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  GlobalDerivedHistogramCalculator(const TString &          _name,
                                     Configuration &          _configuration,
                                     vector<EventFilter*>   & _eventFilters,
                                     vector<ParticleFilter*>& _particleFilters,
                                     LogLevel                 _selectedLevel=Info);
  
  //!
  //! DTOR
  //!
  virtual ~GlobalDerivedHistogramCalculator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //! This involves the calculation of the dereived histogram by calls to objects of the class GlobalDerivedHistos for each
  //! set of historgrams corresponding to event filters and particle filters identified at construction.
  //!
  virtual void execute();
  
  //!
  //! Creates the histograms  filled by this task at execution
  //!
  virtual void createHistograms();

  //!
  //! Loads the histograms retquired by this task at execution
  //!
  virtual void loadHistograms(TFile * inputFile);
  

  ClassDef(GlobalDerivedHistogramCalculator,0)
};


#endif /* CAP__GlobalDerivedHistogramCalculator */
