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
#ifndef CAP__HerwigEventReader
#define CAP__HerwigEventReader
#include "RootTreeReader.hpp"

//!
//! This class defines tasks capable of reading Herwig data file.
//!
//!
class HerwigEventReader : public RootTreeReader
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
  HerwigEventReader(const TString &          _name,
                  Configuration &          _configuration,
                  vector<EventFilter*>   & _eventFilters,
                  vector<ParticleFilter*>& _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~HerwigEventReader() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  void setDefaultConfiguration();
  
  //!
  //! Read one Herwig event and insert it in the CAP event stream.
  //!
  void execute();

  //!
  //! Initialize the mapping between the branches and the storage variables
  //! used in Herwig data files.
  //!
  virtual void initInputTreeMapping();

protected:
  

  static const int arraySize = 50000;
  
  // Declaration of leaf types
  Int_t           eventNo;
  Int_t           nParticles;
  Int_t           Nproj;
  Int_t           Ntarg;
  Float_t         impact;
  Int_t           nPartTotal;
  Int_t           pid[arraySize];   //[Mult]
  Float_t         px[arraySize];   //[Mult]
  Float_t         py[arraySize];   //[Mult]
  Float_t         pz[arraySize];   //[Mult]
  Float_t         m[arraySize];   //[Mult]
  Float_t         Nx[arraySize];   //[Mult]
  Float_t         Ny[arraySize];   //[Mult]

  // List of branches
  TBranch        *b_eventNo;   //!
  TBranch        *b_mult;   //!
  TBranch        *b_Nproj;   //!
  TBranch        *b_Ntarg;   //!
  TBranch        *b_impact;   //!
  TBranch        *b_nPartTotal;   //!
  TBranch        *b_pid;   //!
  TBranch        *b_px;   //!
  TBranch        *b_py;   //!
  TBranch        *b_pz;   //!
  TBranch        *b_m;   //!
  TBranch        *b_Nx;   //!
  TBranch        *b_Ny;   //!

  ClassDef(HerwigEventReader,0)
};

#endif /* CAP_HerwigEventReader */
