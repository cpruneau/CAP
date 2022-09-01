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

RootTreeReader::RootTreeReader(const TString & _name,
                               Configuration & _configuration,
                               vector<EventFilter*> & _eventFilters,
                               vector<ParticleFilter*> &_particleFilters)
:
Task(_name, _configuration, _eventFilters, _particleFilters),

dataInputPath(),
dataInputFileName("FOLDER"),
dataInputTreeName("tree"),
firstFile(-1),
lastFile(-1),
clonesMaxArraySize(1000),
randomizeEventPlane(false),
inputRootChain(nullptr),
inputRootTreeIndex(0),
inputDataFile(nullptr),
nEntries(0),
nBytes(0),
nb(0),
entryIndex(0)
{
  appendClassName("RootTreeReader");
}

RootTreeReader::~RootTreeReader()
{
  if (inputDataFile)
    {
    inputDataFile->Close();
    delete inputDataFile;
    }
}

void RootTreeReader::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  setParameter("UseParticles",          true);
  setParameter("UseEventStream0",       true);
  setParameter("DataInputUsed",         true);
  setParameter("DataInputPath",         TString("./"));
  setParameter("DataInputFile",         TString("FOLDER"));
  setParameter("DataInputTree",         TString("tree"));
  setParameter("dataInputFileMinIndex", -1);
  setParameter("dataInputFileMaxIndex", -1);
  addParameter("RemovePhotons",         true);
  addParameter("StandaloneMode",        true);
  addParameter("ClonesMaxArraySize",    10000);
  addParameter("RandomizeEventPlane",   true);
}

void RootTreeReader::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  dataInputPath       = getValueString("DataInputPath");
  dataInputFileName   = getValueString("DataInputFile");
  dataInputTreeName   = getValueString("DataInputTree");
  firstFile           = getValueInt(   "dataInputFileMinIndex");
  lastFile            = getValueInt(   "dataInputFileMaxIndex");
  clonesMaxArraySize  = getValueInt(   "ClonesMaxArraySize");
  randomizeEventPlane = getValueBool(  "RandomizeEventPlane");
  
  inputRootChain = new TChain(dataInputTreeName);
  if (!inputRootChain)
    {
    if (reportFatal()) cout << "Chain is a null pointer" << endl;
    postTaskFatal();
    return;
    }

  // If the dataInputFileName is equal to "folder", then the use has requested
  // the data be read from the folder identified by the dataInputPath parameter
  // and using IncludePattern and ExcludePattern to extract the data files to be
  // used in this particular analysis.
  vector<TString> selectedFileNames;
  if (dataInputFileName.Contains("FOLDER"))
    {
    selectedFileNames = getSelectedFileNamesFrom(dataInputPath);
    }
  else
    {
    selectedFileNames.push_back(dataInputPath+dataInputFileName);
    }
  if (selectedFileNames.size()<1)
    {
    if (reportError(__FUNCTION__)) cout << "No root data file selected for input" << endl;
    postTaskError();
    return;
    }
    
  // If the parameters firstFile and lastFile are less than zero, unspecified by the user,
  // we assume the user wants to read all files in the selection.
  if (firstFile < 0) firstFile = 0;
  if (lastFile < 0)  lastFile  = selectedFileNames.size();
  if (lastFile > int(selectedFileNames.size())) lastFile  = selectedFileNames.size();
  for(int iFile=firstFile; iFile<lastFile; iFile++)
    {
    TString fileName = selectedFileNames[iFile];
    if (!fileName.EndsWith(".root")) fileName += ".root";
    if (reportInfo(__FUNCTION__)) cout << "Adding input file:" << fileName << endl;
    inputRootChain->Add(fileName);
    }
  initInputTreeMapping();
  setInputRootTreeIndex(-1);
  entryIndex = 0;
  nEntries = inputRootChain->GetEntriesFast();
  if (nEntries < 1)
    {
    if (reportError(__FUNCTION__)) cout << "No data found: nEntries < 1. Abort job!" << endl;
    postTaskFatal();
    return;
    }
  else
    {
    if (reportInfo(__FUNCTION__)) cout << "nEntries: " << nEntries << endl;
    }
  nBytes = 0;
  nb = 0;
  if (reportEnd(__FUNCTION__))
    ;
}


//!
//! Subclass this method to read an event for a tree and copy into CAP event streams.
//!
void RootTreeReader::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
}

//!
//!Read contents of entry.
//!
Int_t RootTreeReader::GetEntry(Long64_t entry)
{
  if (!inputRootChain) return 0;
  return inputRootChain->GetEntry(entry);
}

//!
//! Set the environment to read one entry from the tree.
//! Load the tree
//! Return the number of entries in the tree
//!
Long64_t RootTreeReader::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!inputRootChain) return -5;
  Long64_t centry = inputRootChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (inputRootChain->GetTreeNumber() != inputRootTreeIndex)
    {
    inputRootTreeIndex = inputRootChain->GetTreeNumber();
  }
  return centry;
}

void RootTreeReader::initInputTreeMapping()
{
 // no ops.
}
