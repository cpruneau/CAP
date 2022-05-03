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

#ifndef CAP__NuDynAnalyzer
#define CAP__NuDynAnalyzer
#include <TParameter.h>
#include "TFile.h"
#include "TList.h"
#include "Task.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "NuDynHistos.hpp"
#include "NuDynDerivedHistos.hpp"
#include "Configuration.hpp"
#include "ParticleFilter.hpp"
#include "Configuration.hpp"

//!
//!Task used for the determination of multiplicity moments of second, third, and fourth order. These moments
//!can then be used, in a "derived" stage, to compute cumulants of 2nd, 3rd, and 4th order, as well as other
//!variables such as the nu-dynamic variable.
//!As for other task classes of this package, the computation can be carried out for one or  several event class types (selected by the
//!user supplied event filters) and for one or several particle filters (selected by the user as particle filters). It is then possible to calculate
//!moments, cumulants, and various combinations of these for specific kinematic ranges or specific particle types or both.
//!The moments are computed as integrated averages and as function of some global event characteristic or property. The specific
//!property used as independent variable to compute (and eventually plot) the moments, cumulants, etc, is selected based on the  inputType
//!parameter selected at construction of an instance of this class.
//!
class NuDynAnalyzer : public Task
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
  NuDynAnalyzer(const TString &     _name,
            const Configuration &   _configuration,
            vector<EventFilter*>    _eventFilters,
            vector<ParticleFilter*> _particleFilters,
            LogLevel                _selectedLevel);
  //!
  //!DTOR
  //!
  virtual ~NuDynAnalyzer() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //!Initialize this task.
  //!
  virtual void initialize();
  
  //!
  //! Execute this task based on the configuration and class variables specified at construction
  //!
  virtual void execute();
  
  //!
  //! Creates the histograms  filled by this task at execution
  //!
  virtual void createHistograms();
  
  //!
  //! Loads the histograms retquired by this task at execution
  //!
  virtual void loadHistograms(TFile * inputFile);

protected:
  int multiplicityType; //!< variable used to select which event property is used to differential studies of the moments. This variable is set the class parameter "inputType".

  ClassDef(NuDynAnalyzer,0)
};


#endif /* CAP__NuDynAnalyzer */
