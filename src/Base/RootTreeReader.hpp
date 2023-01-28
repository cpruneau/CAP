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
#ifndef CAP_RootTreeReader
#define CAP_RootTreeReader
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "Task.hpp"

namespace CAP
{

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
  RootTreeReader(const String & _name,
                 Configuration & _configuration,
                 vector<EventFilter*>&   _eventFilters,
                 vector<ParticleFilter*>&_particleFilters);
  
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

  
  //!
  //!Initialize the input tree chain by mapping branches onto specific variables.
  //!This method must be implemented in a sub class of the RootTreeReader class.
  //!
  virtual void initInputTreeMapping();

  //!
  //!Get pointer to the root input chain
  //!
  inline TChain  * rootInputTreeChain()
  {
  return inputRootChain;
  }


  inline void setInputRootTreeIndex(int value)
  {
  inputRootTreeIndex = value;
  }
  
  inline int getInputRootTreeIndex() const
  {
  return inputRootTreeIndex;
  }
  
protected:
  
 
  inline void Show(Long64_t entry)
  {
    if (!inputRootChain) return;
    inputRootChain->Show(entry);
  }

  Int_t GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);

  inline bool useRandomizeEventPlane() const
  {
  return randomizeEventPlane;
  }

  inline void setRandomizeEventPlane(bool value)
  {
  randomizeEventPlane = value;
  }
  
  double getRandomEventPlaneAngle() const
  {
  return  TMath::TwoPi() * gRandom->Rndm();
  }
   
  int getClonesMaxArraySize() const
  {
  return clonesMaxArraySize;
  }
  
  void getClonesMaxArraySize(int value)
  {
  clonesMaxArraySize = value;
  }

  //
  // Data members
  //
  String dataInputPath;
  String dataInputFileName;
  String dataInputTreeName;
  int  firstFile;
  int  lastFile;
  int  clonesMaxArraySize;
  bool randomizeEventPlane;
  
  TChain  *inputRootChain;     //!pointer to the analyzed (input)  TTree or TChain
  Int_t    inputRootTreeIndex; //!current Tree number in an input  TChain
  TFile   *inputDataFile;
  Long64_t nEntries; //! number of entries in an input TTree or TChain 
  Long64_t nBytes;
  Long64_t nb;
  long entryIndex;

  ClassDef(RootTreeReader,0)
};

}
#endif /* CAP_RootTreeReader */
