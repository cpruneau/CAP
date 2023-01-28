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
#ifndef CAP__EposEventReader
#define CAP__EposEventReader
#include "RootTreeReader.hpp"

namespace CAP
{


//!
//! Task to compute to read EPOS events and make them available for analysis in CAP. .
//!
//!
class EposEventReader : public RootTreeReader
{
public:

  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task (currently a placeholder not used)
  //! @param _particleFilters Array of particle filters to be used by this task -- use this filter to limit the range of particles read into CAP arrays.
  //! @param _reportLevel Message log level to be used by this task.
  //!
  EposEventReader(const String & _name,
                  Configuration & _configuration,
                  vector<EventFilter*> & _eventFilters,
                  vector<ParticleFilter*> & _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~EposEventReader() {}

  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  void setDefaultConfiguration();
  
  //!
  //! Read one AMPT event and insert it in the CAP event stream.
  //!
  void execute();
  
  //!
  //! Initialize the mapping between the branches and the storage variables
  //! used in EPOS data files.
  //!
  virtual void initInputTreeMapping();
  
protected:
  
  static const int arraySize = 40000;

   // Declaration of leaf types
   Int_t           events;
   Int_t           nParticles;
   Float_t         impact;
   Int_t           pid[arraySize];   //[Mult]
   Float_t         px[arraySize];   //[Mult]
   Float_t         py[arraySize];   //[Mult]
   Float_t         pz[arraySize];   //[Mult]
   Float_t         e[arraySize];   //[Mult]

   // List of branches
   TBranch        *b_Events;   //!
   TBranch        *b_Mult;   //!
   TBranch        *b_Impact;   //!
   TBranch        *b_PID;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_E;   //!
 
  ClassDef(EposEventReader,0)
};

}


#endif /* CAP__EposEventReader */
