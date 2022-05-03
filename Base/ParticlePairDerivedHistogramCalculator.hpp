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

#ifndef CAP__ParticlePairDerivedHistogramCalculator
#define CAP__ParticlePairDerivedHistogramCalculator
#include "Task.hpp"
#include "ParticleHistos.hpp"
#include "ParticleDerivedHistos.hpp"
#include "ParticlePairHistos.hpp"
#include "ParticlePairDerivedHistos.hpp"
#include "ParticlePairAnalyzer.hpp"

//!
//! Task to compute derived particle pair histograms and correlation functions base on basic pair histograms.
//!
//! \see ParticlePairAnalyzer for a list of the configuration parameters of this class.
//! \see ParticlePairDerivedHistos
//!
class ParticlePairDerivedHistogramCalculator : public Task
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
  ParticlePairDerivedHistogramCalculator(const TString &          _name,
                                         const Configuration    & _configuration,
                                         vector<EventFilter*>   & _eventFilters,
                                         vector<ParticleFilter*>& _particleFilters,
                                         LogLevel _selectedLevel);

  ParticlePairDerivedHistogramCalculator(const TString &_name,
                                         const Configuration &  _configuration,
                                         ParticlePairAnalyzer * _analyzer,
                                         LogLevel               _selectedLevel);
  //!
  //!DTOR
  //!
  virtual ~ParticlePairDerivedHistogramCalculator() {}

  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //! This involves the calculation of the dereived histogram by calls to objects of the class ParticleDerivedHistos for each
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

  ClassDef(ParticlePairDerivedHistogramCalculator,0)
};


#endif /* CAP__ParticlePairDerivedHistogramCalculator */
