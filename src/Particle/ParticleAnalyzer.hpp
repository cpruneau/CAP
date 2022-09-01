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
#ifndef CAP__ParticleAnalyzer
#define CAP__ParticleAnalyzer
#include "Task.hpp"
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
class ParticleAnalyzer : public Task
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
  ParticleAnalyzer(const TString & _name,
                   Configuration & _configuration,
                   vector<EventFilter*> & _eventFilters,
                   vector<ParticleFilter*> & _particleFilters);
  
  //!
  //!DTOR
  //!
  virtual ~ParticleAnalyzer() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute a single particle analysis  based on event and particle fileter operated with this task instance.
  //! Two options are implemented and chosen automatically by the code. 
  //! If more than one particle filter is registered with this task, it is assumed that each filter substantially reduced the number of
  //! particles to be used to fill histogram(s). Reduced lists of particles are thus assembled separately (and sequentially) and the filling of the
  //! histogram(s) is delagated to the ParticleHistos class used by this task. If instead, a single particle filter is registered with this task
  //! instance than there is little justification to produced a sublist and the fill method of the ParticleHistos class is called for each particle.
  //! Note that in the case that many reduced lists are used, they are constituted (currently) of vectors or ParticleDigit objects which
  //! are essentially structs containing predigitized information about the momentum, the azimuth, the rapidity, and the pseudorapidity.
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

  virtual void createDerivedHistograms();

  virtual void loadDerivedHistograms(TFile * inputFile __attribute__((unused)));

  virtual void calculateDerivedHistograms();

protected:
  
  bool fillEta; //!< whether to fill pseudorapidity histograms (set from configuration at initialization)
  bool fillY;   //!< whether to fill rapidity histograms (set from configuration at initialization)
  bool fillP2;  //!< whether to fill P2 and G2 related histograms  (set from configuration at initialization)
  
  ClassDef(ParticleAnalyzer,0)
};


#endif /* CAP__ParticleAnalyzer */
