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
#include "TDatabasePDG.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include "Aliases.hpp"
#include "EventTask.hpp"
//#include "Event.hpp"
//#include "Particle.hpp"
//#include "ParticleType.hpp"

namespace CAP
{

//!
//! Abstract base class defining a ROOT Tree reader. Subclass this class to read any ROOT tree.
//!
//template <class Event, class Particle, class ParticleType>
class RootTreeReader : public EventTask
{

//  using EventFilter = Filter<Event>;
//  using ParticleFilter = Filter<Particle>;

public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle filters to be used by this task
  //!
  RootTreeReader(const TString & _name,
                 const Configuration & _configuration,
                 vector<EventFilter*>&   _eventFilters,
                 vector<ParticleFilter*>&_particleFilters)
  :
  EventTask::EventTask(_name, _configuration, _eventFilters, _particleFilters),
  eventsImportPath(),
  eventsImportFileName("FOLDER"),
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
  TString s = "RootTreeReader";
  MessageLogger::appendClassName(s);
  }
  
  //!
  //! DTOR
  //!
  virtual ~RootTreeReader()
  {
  if (inputDataFile)
    {
    inputDataFile->Close();
    delete inputDataFile;
    }
  }
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration()
  {
  Task::setDefaultConfiguration();
  Task::addParameter("EventsUseStream0",       true);
  Task::addParameter("EventsImport",           true);
  Task::addParameter("EventsImportPath",       TString("./"));
  Task::addParameter("EventsImportFile",       TString("FOLDER"));
  Task::addParameter("DataInputTree",          TString("tree"));
  Task::addParameter("EventsImportFileMinIndex", -1);
  Task::addParameter("EventsImportFileMaxIndex", -1);
  Task::addParameter("RemovePhotons",         true);
  Task::addParameter("StandaloneMode",        true);
  Task::addParameter("ClonesMaxArraySize",    10000);
  Task::addParameter("RandomizeEventPlane",   true);
  }
  
  //!
  //! Generic initializer of the root reader class. It opens the necessary files and calls
  //! other functions to complete the initialization of the task. 
  //!
  virtual void initialize()
  {
  if (reportStart(__FUNCTION__))
;
  Task::initialize();
  eventsImportPath      = Task::getValueString("EventsImportPath");
  eventsImportFileName  = Task::getValueString("EventsImportFile");
  dataInputTreeName     = Task::getValueString("DataInputTree");
  firstFile             = Task::getValueInt(   "EventsImportFileMinIndex");
  lastFile              = Task::getValueInt(   "EventsImportFileMaxIndex");
  clonesMaxArraySize    = Task::getValueInt(   "ClonesMaxArraySize");
  randomizeEventPlane   = Task::getValueBool(  "RandomizeEventPlane");

  inputRootChain = new TChain(dataInputTreeName);
  if (!inputRootChain)
    {
    if (reportFatal(__FUNCTION__)) cout << "Chain is a null pointer" << endl;
    postTaskFatal();
    return;
    }

  // If the eventsImportFileName is equal to "folder", then the user has requested
  // the data be read from the folder identified by the EventsImportPath parameter
  // and using IncludePattern and ExcludePattern to extract the data files to be
  // used in this particular analysis.
  VectorString  selectedFileNames;
  if (eventsImportFileName.Contains("FOLDER"))
    {
    selectedFileNames = getSelectedFileNamesFrom(eventsImportPath);
    }
  else
    {
    selectedFileNames.push_back(eventsImportPath+eventsImportFileName);
    }
  if (selectedFileNames.size()<1)
    {
    if (reportError(__FUNCTION__)) cout << "No root data file selected for input" << endl;
    postTaskError();
    exit(1);
    }

  // If the parameters firstFile and lastFile are less than zero, unspecified by the user,
  // we assume the user wants to read all files in the selection.
  if (firstFile < 0) firstFile = 0;
  if (lastFile < 0)  lastFile  = selectedFileNames.size();
  if (lastFile > int(selectedFileNames.size())) lastFile  = selectedFileNames.size();
  for(int iFile=firstFile; iFile<lastFile; iFile++)
    {
    String fileName = selectedFileNames[iFile];
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
  //! Execute this task based on the configuration and class variable specified at construction
  //! This involves reading events from the input tree and filling CAP event streams based on the
  //! event and particle filters providing at construction.
  //!
  virtual void importEvent()
  {
  if (reportStart(__FUNCTION__))
;
  }

  //!
  //!Initialize the input tree chain by mapping branches onto specific variables.
  //!This method must be implemented in a sub class of the RootTreeReader class.
  //!
  virtual void initInputTreeMapping()
  {
  // no ops.
  }


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

  Int_t GetEntry(Long64_t entry)
  {
  if (!inputRootChain) return 0;
  return inputRootChain->GetEntry(entry);
  }


  Long64_t LoadTree(Long64_t entry)
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
  String eventsImportPath;
  String eventsImportFileName;
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
