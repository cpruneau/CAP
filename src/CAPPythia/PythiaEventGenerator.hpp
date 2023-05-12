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
//#include "TTree.h"
//#include "TParticle.h"
//#include "TClonesArray.h"
#include "Pythia.h"
#include "EventTask.hpp"
//#include "Event.hpp"
//#include "Particle.hpp"
//#include "ParticleType.hpp"

namespace CAP
{


class PythiaEventGenerator : public EventTask
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
  PythiaEventGenerator(const String & _name,
                       const Configuration & _configuration,
                       vector<EventFilter*>&   _eventFilters,
                       vector<ParticleFilter*>&_particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~PythiaEventGenerator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();

  virtual void configure();

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
  virtual void createEvent();


protected:
  
  Pythia8::Pythia * pythia;

  // For TTree file output
  // Set up the ROOT TFile and TTree.
  TFile *outputFile;
//  Pythia8::Event *outputEvent;
//  TTree *outputTree;

  bool   standaloneMode;
  bool   printBanner;
  bool   printStatistics;
  int    printNEvents;
  int    beamsPdgA;
  int    beamsPdgB;
  int    beamsFrameType;
  double beamsECMS;
  double beamsEA;
  double beamsEB;
  bool   setSeed;
  long   seedValue;
  bool   useQCDCR;
  bool   useRopes;
  bool   useShoving;
  String xmlInputPath;
  int    nEventsPrinted;


  ClassDef(PythiaEventGenerator,0)
};

}

#endif /* CAP__PythiaEventGenerator */
