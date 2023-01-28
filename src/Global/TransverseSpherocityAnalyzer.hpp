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
#ifndef CAP__TransverseSpherocityAnalyzer
#define CAP__TransverseSpherocityAnalyzer
#include "Task.hpp"
#include "TransverseSpherocityHistos.hpp"

namespace CAP
{


//!
//!Task performing a transverse spherocity analysis of events of the incoming event stream.
//!
class TransverseSpherocityAnalyzer : public Task
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
  TransverseSpherocityAnalyzer(const String & _name,
                               Configuration & _configuration,
                               vector<EventFilter*> & _eventFilters,
                               vector<ParticleFilter*> & _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~TransverseSpherocityAnalyzer() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Initialize this task
  //!
  virtual void initialize();

  //!
  //! Execute the spherocity analysis on one event of the incoming event stream. Optionally set the EventProperty of the event for analysis by other tasks.
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
  
  bool setEvent;   //!< Whether this task instance sets spherocity properties stored in the EventProperty record of the current event.
  bool fillS0;     //!< Whether the regular spherocity observable S0 should be analyzed and filled by this task.
  bool fillS1;     //!< Whether the unit vector spherocity observable S1 should be analyzed and filled by this task.
  bool fillS1VsS0; //!< Whether two-dimensional S1 vs S0 histograms should be filled.
  int    nSteps;   //!< Number of azimuthal steps used in the calculation of the transverse spherocity.
  double stepSize; //!< Two-pi/nSteps.

  ClassDef(TransverseSpherocityAnalyzer,0)
};

} //namespace CAP


#endif /* CAP__TransverseSpherocityAnalyzer */
