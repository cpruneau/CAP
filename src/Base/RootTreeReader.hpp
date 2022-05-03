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
#ifndef WAC_RootTreeReader
#define WAC_RootTreeReader
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "Task.hpp"

//!
//! Abstract base class defining a ROOT Tree reader. Subclass this class to read any ROOT tree.
//!
class RootTreeReader : public Task
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
  RootTreeReader(const TString &         _name,
                 const Configuration &   _configuration,
                 vector<EventFilter*>&   _eventFilters,
                 vector<ParticleFilter*>&_particleFilters,
                 LogLevel                _selectedLevel);
  
  //!
  //! DTOR
  //!
  virtual ~RootTreeReader();
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Generic initializer of the root reader class. It opens the necessary files and calls
  //! other functions to complete the initialization of the task. 
  //!
  virtual void initialize();
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //! This involves reading events from the input tree and filling CAP event streams based on the
  //! event and particle filters providing at construction.
  //!
  virtual void execute();
  
protected:
  
  void Init(TTree *tree);
 
  inline Bool_t Notify()
  {
    return kTRUE;
  }

  inline void Show(Long64_t entry)
  {
    if (!fChain) return;
    fChain->Show(entry);
  }

  inline Int_t Cut(Long64_t entry)
  {
    return 1;
  }


  
  Int_t GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
 
  
  ////////////////////////////////////////////////////////////////////////////////////////
  // Data members
  ////////////////////////////////////////////////////////////////////////////////////////
  TTree  *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t   fCurrent; //!current Tree number in a TChain
  TFile  *inputDataFile;
  Long64_t nentries;
  Long64_t nbytes;
  Long64_t nb;
  long jentry;
  
  static const int kMaxparticles = 2000;
  
  ClassDef(RootTreeReader,0)
};

#endif /* WAC_RootTreeReader */
