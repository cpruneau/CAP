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
#ifndef CAP__CollisionGeometryAnalyzer
#define CAP__CollisionGeometryAnalyzer
#include "Task.hpp"
#include "CollisionGeometry.hpp"
#include "CollisionGeometryHistograms.hpp"

//!
//! Task to carry out the analysis of the geometry of MC collisions
//!
class CollisionGeometryAnalyzer : public Task
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
  CollisionGeometryAnalyzer(const TString & _name,
                            Configuration & _configuration,
                            vector<EventFilter*> &   _eventFilters,
                            vector<ParticleFilter*>  _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~CollisionGeometryAnalyzer() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
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
  
  //!
  //! Get the collision geometry object used by this task
  //!
  CollisionGeometry * getCollisionGeometry() { return collisionGeometry; }

protected:

  //!
  //! Collision geometry object used by this task
  //!
  CollisionGeometry * collisionGeometry;

  ClassDef(CollisionGeometryAnalyzer,0)
};

#endif /* CAP__CollisionGeometryAnalyzer */
