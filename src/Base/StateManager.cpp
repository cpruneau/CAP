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
#include "StateManager.hpp"

ClassImp(StateManager);

//! CTOR
StateManager::StateManager()
:
state(OK)
{   }

TString StateManager::getStateName() const
 {
  TString statusName;
  switch (state)
    {
      case UNKNOWN:      statusName = "Unknown"; break;
      case CONFIGURED:   statusName = "EOF"; break;
      case INITIALIZED:  statusName = "EOF"; break;
      case COMPLETED:    statusName = "EOF"; break;
      case EOFILE:       statusName = "EOF"; break;
      case EODATA:       statusName = "EOD"; break;
      case OK:           statusName = "OK";  break;
      case WARNING: statusName = "WARNING"; break;
      case ERROR:   statusName = "ERROR";   break;
      case FATAL:   statusName = "FATAL";   break;
    }
  return statusName;
}


StateManager * StateManager::stateManagerSingleton = nullptr;

StateManager * StateManager::getStateManager()
{
  if (stateManagerSingleton==nullptr)
    {
    stateManagerSingleton = new StateManager();
    }
  return stateManagerSingleton;
}
