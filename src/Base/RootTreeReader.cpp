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
#include "TDatabasePDG.h"
#include "RootTreeReader.hpp"
ClassImp(RootTreeReader);

RootTreeReader::RootTreeReader(const TString &         _name,
                               const Configuration &   _configuration,
                               vector<EventFilter*>&   _eventFilters,
                               vector<ParticleFilter*>&_particleFilters,
                               LogLevel                _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel),
fChain(nullptr),
fCurrent(0),
inputDataFile(nullptr),
nentries(0),
nbytes(0),
nb(0),
jentry(0)
{
}


RootTreeReader::~RootTreeReader()
{
  cout << "RootTreeReader::~RootTreeReader(...)" << endl;
  if (inputDataFile)
    {
    inputDataFile->Close();
    delete inputDataFile;
    }
}

void RootTreeReader::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  Configuration & config = getConfiguration();
  config.setParameter("useParticles",     true);
  config.setParameter("useEventStream0",  true);
  config.addParameter("clonesArraySize", 10000);
  config.addParameter("removePhotons",    true);
  config.addParameter("standaloneMode",   true);
}

void RootTreeReader::initialize()
{
  Task::initialize();
  if (reportStart("RootTreeReader",getName(),"initialize()"))
    ;
  
  const Configuration & config = getConfiguration();
  TString dataInputTreeName = config.getValueString("dataInputTreeName");
  TString inputFileName = config.getValueString("dataInputPath");
  inputFileName += "/";
  inputFileName += config.getValueString("dataInputFileName");
  if (reportInfo("RootTreeReader",getName(),"initialize()")) cout << "Opening file: " << inputFileName << endl;
  inputDataFile = TFile::Open(inputFileName);
  if (!inputDataFile)
    {
    if (reportFatal("RootTreeReader",getName(),"initialize()")) cout << "Unable to open file: " << inputFileName <<endl;
    postTaskFatal();
    return;
    }
  TTree * inputTree = nullptr;
  inputDataFile->GetObject(dataInputTreeName,inputTree);
  if (!inputTree)
    {
    if (reportFatal("RootTreeReader",getName(),"initialize()"))
      cout << "No inputTree named: " << dataInputTreeName << " in file: " << inputFileName << endl;
    postTaskFatal();
    return;
    }
  Init(inputTree);
  if (reportDebug("RootTreeReader",getName(),"initialize()")) cout << "RootTreeReader::initialize() Completed" << endl;
}


//!
//! Subclass this method to read an event for a tree and copy into CAP event streams.
//!
void RootTreeReader::execute()
{
  if (reportStart("RootTreeReader",getName(),"execute()"))
    ;
}

//!
//!Read contents of entry.
//!
Int_t RootTreeReader::GetEntry(Long64_t entry)
{
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

//!
//! Set the environment to read one entry from the tree.
//! Load the tree
//! Return the number of entries in the tree
//!
Long64_t RootTreeReader::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent)
    {
    fCurrent = fChain->GetTreeNumber();
  }
  return centry;
}

//!
//!initialize the input tree chain by mapping branches onto specific variables.
//!
//!This method must be implemented in a sub class of the RootTreeReader class.
//!
void RootTreeReader::Init(TTree *tree)
{
 // no ops.
}


