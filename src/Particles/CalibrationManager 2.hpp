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
#ifndef CAP__CalibrationManager
#define CAP__CalibrationManager
#include "EventTask.hpp"
#include "ParticleDb.hpp"

namespace CAP
{

//!
//! Task used for the single purpose of loading a particle type table. The particle type and properties are loaded from a specially formatted file.
//! The task can optionally output a copy or reduced copy of the file to a location and file name to a user specified location.
//!
//! Configuration parameters include the input file path and name of the file containing the data to be read. If the task is meant to output a copy of the file,
//! the file path and name of the output file should also be specified. Configuration parameters are as follows
//!
//! - EventsImport [true] : whether particle type data will be read from file
//! - EventsImportPath [getenv("CAP_DATA") : folder (file path) where to find  the file to be read.
//! - EventsImportFileName ["ParticleTypeData.dat"] : name of the file containing particle type PDG data to be read in.
//! - EventsExport [true] : whether particle type data will be written to file
//! - EventsExportPath [getenv("CAP_DATA") : folder (file path) where to write a particle type PDG data file.
//! - EventsExportFileName ["NewParticleTypeData.dat"] : name of the file containing on output particle type PDG data.
//!
//!
class CalibrationManager : public EventTask
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  CalibrationManager(const String & _name,
                          const Configuration & _configuration);
  
  //!
  //!DTOR
  //!
  virtual ~CalibrationManager() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task.
  //! Configuration parameters include the input file path and name of the file containing the data to be read. If the task is meant to output a copy of the file,
  //! the file path and name of the output file should also be specified. Configuration parameters are as follows
  //!
  //! - EventsImport [true] : whether particle type data will be read from file
  //! - EventsImportPath [getenv("CAP_DATA") : folder (file path) where to find  the file to be read.
  //! - EventsImportFileName ["ParticleTypeData"] : name of the file containing particle type PDG data to be read in.
  //! - EventsExport [true] : whether particle type data will be written to file
  //! - EventsExportPath [getenv("CAP_DATA") : folder (file path) where to write a particle type PDG data file.
  //! - EventsExportFileName ["NewParticleTypeData"] : name of the file containing on output particle type PDG data.
  //!
  virtual void setDefaultConfiguration();
  virtual void configure();

  //!
  //! Read or write a particle type data file as specificied by the configuration parameters.
  //!
  virtual void execute();
  virtual void importCalibration();
  virtual void exportCalibration();


protected:

  bool   calibsCreate;
  bool   calibsReset;
  bool   calibsClear;
  bool   calibsForceRewrite;
  bool   calibsImport;
  String calibsImportPath;
  String calibsImportFile;
  bool   calibsExport;
  bool   calibsExportAsRoot;
  bool   calibsExportAsText;
  String calibsExportPath;
  String calibsExportFile;



  ClassDef(CalibrationManager,0)
};

}

#endif /* CAP__CalibrationManager */
