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
#ifndef CAP__BalanceFunctionCalculator
#define CAP__BalanceFunctionCalculator
#include "Task.hpp"
using namespace std;

class BalanceFunctionCalculator : public Task
{
public:

  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle filters to be used by this task
  //! @param _sObservableNames Array containing the names of the single particle  observables involved in the balance function calculation
  //! @param _pObservableNames Array containing the names of the pair  observables involved in the balance function calculation
  //! @param _reportLevel Message log level to be used by this task.
  //!
  BalanceFunctionCalculator(const TString &          _name,
                            const Configuration &    _configuration,
                            vector<EventFilter*> &   _eventFilters,
                            vector<ParticleFilter*>& _particleFilters,
                            vector<TString> &        _sObservableNames,
                            vector<TString> &        _pObservableNames,
                            MessageLogger::LogLevel  _reportLevel);
  //!
  //! DTOR
  //!
  virtual ~BalanceFunctionCalculator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();

protected:
  
  //!
  //! Array containing the names of the single particle  observables involved in the balance function calculation
  //!
  vector<TString> & sObservableNames;

  //!
  //! Array containing the names of the pair observables involved in the balance function calculation
  //!
  vector<TString> & pObservableNames;

  ClassDef(BalanceFunctionCalculator,0)
};

#endif /* CAP__BalanceFunctionCalculator */
