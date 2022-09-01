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
#ifndef CAP__ParticleTypeTableLoader
#define CAP__ParticleTypeTableLoader
#include "Task.hpp"
#include "ParticleType.hpp"
//!
//! Task used for the single purpose of loading a particle type table. The particle type and properties are loaded from a specially formatted file.
//! The task can optionally output a copy or reduced copy of the file to a location and file name to a user specified location.
//!
//! Configuration parameters include the input file path and name of the file containing the data to be read. If the task is meant to output a copy of the file,
//! the file path and name of the output file should also be specified. Configuration parameters are as follows
//!
//! - dataInputUsed [true] : whether particle type data will be read from file
//! - dataInputPath [getenv("CAP_DATA") : folder (file path) where to find  the file to be read.
//! - dataInputFileName ["ParticleTypeData.dat"] : name of the file containing particle type PDG data to be read in.
//! - dataOutputUsed [true] : whether particle type data will be written to file
//! - dataOutputPath [getenv("CAP_DATA") : folder (file path) where to write a particle type PDG data file.
//! - dataOutputFileName ["NewParticleTypeData.dat"] : name of the file containing on output particle type PDG data.
//!
//!
class ParticleTypeTableLoader : public Task
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  ParticleTypeTableLoader(const TString & _name,
                          Configuration & _configuration);
  
  //!
  //!DTOR
  //!
  virtual ~ParticleTypeTableLoader() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task.
  //! Configuration parameters include the input file path and name of the file containing the data to be read. If the task is meant to output a copy of the file,
  //! the file path and name of the output file should also be specified. Configuration parameters are as follows
  //!
  //! - dataInputUsed [true] : whether particle type data will be read from file
  //! - dataInputPath [getenv("CAP_DATA") : folder (file path) where to find  the file to be read.
  //! - dataInputFileName ["ParticleTypeData"] : name of the file containing particle type PDG data to be read in.
  //! - dataOutputUsed [true] : whether particle type data will be written to file
  //! - dataOutputPath [getenv("CAP_DATA") : folder (file path) where to write a particle type PDG data file.
  //! - dataOutputFileName ["NewParticleTypeData"] : name of the file containing on output particle type PDG data.
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Read or write a particle type data file as specificied by the configuration parameters.
  //!
  virtual void execute();
  
  ClassDef(ParticleTypeTableLoader,0)
};


#endif /* CAP__ParticleTypeTableLoader */
