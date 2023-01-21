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
#ifndef CAP__StateManager
#define CAP__StateManager
#include "TClass.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
using namespace std;
using std::cout;
using std::endl;




class StateManager
{
public:

  enum State { UNKNOWN, CONFIGURED, INITIALIZED, COMPLETED, EOFILE, EODATA, OK, WARNING, ERROR, FATAL};


protected:

  //!
  //! Status of the ongoing tasks. Since this is a static variable, it reflects a global state or status of an ongoing analysis. The task status may be set or read by
  //! all objects of class deriving from this class.
  State state;

  //! CTOR
  StateManager();

public:

  //!
  //! Singleton instance of this StateManager class.
  //!
  static StateManager * stateManagerSingleton;

  //!
  //! Get a pointer to the singleton instance of this StateManager class.
  //!
  static StateManager * getStateManager();


  //!
  //! Returns a string corresponding to the current status of all ongoing tasks in this job.
  //!
  TString getStateName() const;

  //!
  //! Set the  state of this job
  //!
  void setState(State newState)  {  state = newState;   }

  //!
  //! Set the  state of this job
  //!
  State getState() const  {  return state;   }

  inline bool  isConfigured() const  { return state == CONFIGURED; }
  inline bool  isInitialized() const { return state == INITIALIZED; }
  inline bool  isOK() const          { return state == OK; }
  inline bool  isRunning() const     { return state == OK; }
  inline bool  isEof() const         { return state == EOFILE; }
  inline bool  isEod() const         { return state == EODATA; }
  inline bool  isCompleted() const   { return state == COMPLETED; }

  //! DTOR
  virtual ~StateManager()
  { }


public:

  ClassDef(StateManager,0)
};







#endif /* CAP__MessageLogger */
