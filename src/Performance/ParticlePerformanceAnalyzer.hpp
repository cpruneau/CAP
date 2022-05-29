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
#ifndef CAP__ParticlePerformanceAnalyzer
#define CAP__ParticlePerformanceAnalyzer
#include "Task.hpp"
#include "ParticlePerformanceHistos.hpp"

//!
//! Task used for the analysis of single particle distributions. As for other tasks classes of this package, use event filters and particle filters to determine the
//! event selection and particle types and kinematic ranges across which single particle distributions are studied. Single particle distributions are computed
//! (histogrammed) with instances of the ParticleHistos class. The behavior of the class, i.e., what type and the range of filled histograms are determined by
//! class configuration provided at task object construction.
//!
//! The following configuration parameters determine what types of histograms are filled (default values in brackets):
//! - fillEta [true]: whether to fill histograms  vs. pseudorapidity "eta"
//! - fillY [false]: whether to fill histograms  vs. rapidity "y"
//! - fillP2 [false]: whether to fill histograms used in the determination of P2 and G2 pT correlators
//!
//! The following parameters specify  the configuration of histograms filled by this task (default values in brackets):
//!
//! - Multiplicity histogram
//!  + nBins_n1 [100]: Number of bins
//!  + min_n1 [0.0]: Minimum multiplicity
//!  + max_n1 [100]: Maximum multiplicity
//! - Particle energy histogram
//!  + nBins_eTot [100]: Number of bins
//!  + min_eTot [0.0]: Minimum value
//!  + max_eTot [100]: Maximum value
//! - Particle transverse momemtum
//!  + nBins_pt [100]: Number of bins
//!  + min_pt [0.0]: Minimum value
//!  + max_pt [100.0]: Maximum value
//! - Particle pseudorapidity
//!  + nBins_eta [20]: Number of bins
//!  + min_eta [-1.0]: Minimum value
//!  + max_eta [1.0]: Maximum value
//! - Particle rapidity
//!  + nBins_y [20]: Number of bins
//!  + min_y [-1.0]: Minimum value
//!  + max_y [1.0]: Maximum value
//! - Particle azimuthal angle
//!  + nBins_phi [36]: Number of bins
//!  + min_phi [0.0]: Minimum value
//!  + max_phi [2pi]: Maximum value
//!
class ParticlePerformanceAnalyzer : public Task
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
  ParticlePerformanceAnalyzer(const TString &         _name,
                              const Configuration &   _configuration,
                              vector<EventFilter*>    _eventFilters,
                              vector<ParticleFilter*> _particleFilters,
                              LogLevel                _selectedLevel=Info);
  
  //!
  //!DTOR
  //!
  virtual ~ParticlePerformanceAnalyzer() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute ..
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

protected:
  
  bool fillEta; //!< whether to fill pseudorapidity histograms (set from configuration at initialization)
  bool fillY;   //!< whether to fill rapidity histograms (set from configuration at initialization)
  
  ClassDef(ParticlePerformanceAnalyzer,0)
};


#endif /* CAP__ParticlePerformanceAnalyzer */
