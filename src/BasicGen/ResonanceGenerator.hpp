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
#ifndef CAP__ResonanceGenerator
#define CAP__ResonanceGenerator
#include "Task.hpp"

//!
//!Task generates particles with a simple gaussian spectrum
//!
class ResonanceGenerator : public Task
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
  ResonanceGenerator(const TString  &        _name,
                     Configuration  &        _configuration,
                     vector<EventFilter*>&   _eventFilters,
                     vector<ParticleFilter*>&_particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~ResonanceGenerator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Initialize the random generators used by this task
  //!
  virtual void initialize();

  //!
  //! Executes this task. This generates one or several particles.
  //!
  virtual void execute();
  
  //!
  //! Generate and fill the given particle
  //!
  virtual void generate(Particle * parent);
  
protected:

  //!
  //! Whether this task is operated in 'standalone' mode or as slave to another particle generator task.
  //!
  bool  standaloneMode;
  
  int   nPartMinimum; //!< Minimum number of generated particles per event
  int   nPartMaximum; //!< Maximum number of generated particles per event
  int   nPartRange;  //!<  Convenience variable set to: nPartMaximum - nPartMinimum.
  float mass; //!< Mass of the generated 'rho-meson'
  float yMinimum;//!< Minimum number of generated particles per event
  float yMaximum;//!< Minimum number of generated particles per event
  float yRange;//!< Convenience variable set to:yMaximum - yMinimum
  float pTslope;//!< transverse momentum slope

  ClassDef(ResonanceGenerator,0)
};

#endif /* CAP__ResonanceGenerator */
