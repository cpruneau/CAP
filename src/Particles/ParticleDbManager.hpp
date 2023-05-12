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
#ifndef CAP__ParticleDbManager
#define CAP__ParticleDbManager
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
class ParticleDbManager : public EventTask
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  ParticleDbManager(const String & _name,
                    const Configuration & _configuration);
  
  //!
  //!DTOR
  //!
  virtual ~ParticleDbManager() {}
  
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
  virtual void importParticleDb();
  virtual void importParticleDbCAP();
  virtual void importParticleDbNative();
  virtual void initializeParticleDb();
  virtual void initializeParticleDbCAP();
  virtual void initializeParticleDbNative();
  virtual void convertParticleDbCAPToNative();
  virtual void convertParticleDbNativeToCAP();
  virtual void exportParticleDb();
  virtual void exportParticleDbCAP();
  virtual void exportParticleDbNative();


protected:

  double clebschGordan(double aJot,  double aEm,
                       double aJot1, double aEm1,
                       double aJot2, double aEm2);
  double deltaJ(double aJot1, double aJot2, double aJot);

  bool   particleDbImport;
  String particleDbImportPath;
  String particleDbImportFile;
  String particleDbImportDecaysFile;
  bool   particleDbImportNative;
  bool   particleDbImportCAP;
  bool   particleDbConvertCAPToNative;
  bool   particleDbConvertNativeToCAP;
  bool   particleDbExport;
  String particleDbExportPath;
  String particleDbExportFile;
  String particleDbExportDecaysFile;
  bool   particleDbExportNative;
  bool   particleDbExportCAP;

  vector<ParticleDb*> particleDbs;

  ClassDef(ParticleDbManager,0)
};

}

#endif /* CAP__ParticleDbManager */
