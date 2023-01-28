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

#ifndef CAP__GlobalAnalyzer
#define CAP__GlobalAnalyzer
#include "Task.hpp"
namespace CAP
{

//!
//! This class implements a basic analyzer of the multiplicity, energy, net charge, net strangeness, and net baryon number of the particles composing an event.
//! A global analyzer task can be used to determine the multiplcity (or other characteristics) of an event and set the event record with these values. The values
//! then become available to other tasks, and the event filters they operate,  to decide whether the analysis of an event should be carried out. The task can be
//! operated with several event filters and it is then possible to determine the distribution of multiplicity, energy, net charge, etc, based on specific event filters
//! (e.g., multiplicity or collision centrality classes). The task can  be operated with one or more particle filters. It is then possible to determine the multiplicity of
//! specific types of particles or the yield in specific pseudorapidity, transverse momentum ranges (etc).  When several event filters are used, only the first (index zero)
//! is used to set the event property record of an event. Note that the createHistograms parameter must be set to YES (true) to fill histograms of this class.
//! This means that an instance of this class can be used to analyze the multiplicity (energy, etc) of events with or without filling corresponding histograms. It is
//! then possible to set the event property record only (base on event filter index 0), fill histograms only, or both set the record and fill histograms.
//!
class GlobalAnalyzer : public Task
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
  GlobalAnalyzer(const String & _name,
                 Configuration & _configuration,
                 vector<EventFilter*> & _eventFilters,
                 vector<ParticleFilter*> & _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~GlobalAnalyzer() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Initialize  this task. Effectively, this entails the initialization of the arrays used to contain the multiplicity, energy, net charge, etc corresponding to the different particle filters used by this task.
  //!
  virtual void initialize();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction. This involves the determination of multiplicities, total energies, etc based on the event filters
  //! and particle filters operated by this task. It is also involves the filling of corresponding histograms.
  //!
  virtual void execute();
  
  //!
  //!Create required histograms to be filled at run time.
  //!
  virtual void createHistograms();
  
  //!
  //!Load histograms previously produced by this task.
  //!
  virtual void loadHistograms(TFile * inputFile);

  virtual void createDerivedHistograms();

  virtual void loadDerivedHistograms(TFile * inputFile __attribute__((unused)));

  virtual void calculateDerivedHistograms();

protected:
  
  bool setEvent;   //!< bool controling whether this task instance sets the EventProperties record of events it analyzes.
  vector<double> n; //!< array of multiplicities corresponding in kinematic  different particle filters operated by this task.
  vector<double> e; //!< array of total energies corresponding to the different particle filters operated by this task.
  vector<double> q; //!< array of net charges corresponding to the different particle filters operated by this task.
  vector<double> s; //!< array of net strangeness corresponding to the different particle filters operated by this task.
  vector<double> b; //!< array of net baryon number corresponding to the different particle filters operated by this task.
  vector<double> ptSum; //!< array of transverse momentum sums  corresponding to the different particle filters operated by this task.

  ClassDef(GlobalAnalyzer,0)
};

}

#endif /* CAP__GlobalAnalyzer */
