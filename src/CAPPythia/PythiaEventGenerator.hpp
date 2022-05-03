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
#ifndef CAP__PythiaEventGenerator
#define CAP__PythiaEventGenerator
#include "TParticle.h"
#include "TClonesArray.h"
#include "TPythia8.h"
#include "TFile.h"
#include "TTree.h"
#include "Task.hpp"

class PythiaEventGenerator : public Task
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
  PythiaEventGenerator(const TString &         _name,
                       const Configuration &   _configuration,
                       vector<EventFilter*>&   _eventFilters,
                       vector<ParticleFilter*>&_particleFilters,
                       LogLevel                _selectedLevel);
  
  //!
  //! DTOR
  //!
  virtual ~PythiaEventGenerator() {} 
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Initialize this task
  //!
  virtual void initialize();

  //!
  //! Finalize this task
  //!
  virtual void finalize();

  //!
  //! Execute this task. This involves the generation of one PYTHIA event copied to CAP event stream OR output to
  //! a root tree.
  //!
  virtual void execute();
  
  //!
  //!Generate one interaction.
  //!
  virtual void generate(Particle * parent);

protected:
  
  TPythia8* pythia8; // = new TPythia8();

  // For WAC analyses
  int nMaxClonesArray; //  = 10000;
  TClonesArray* particles; // = new TClonesArray("TParticle", nMax);

  // For TTree file output
  // Set up the ROOT TFile and TTree.
  TFile *outputFile;
  Pythia8::Event *outputEvent;
  TTree *outputTree;

  bool dataConversionToWac;
  bool dataOutputUsed;
  bool standaloneMode;

  ClassDef(PythiaEventGenerator,0)
};

#endif /* CAP__PythiaEventGenerator */
