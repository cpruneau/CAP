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
#ifndef CAP__TaskIterator
#define CAP__TaskIterator
#include "Task.hpp"

//!
//! This class implements a task iterator task, i.e., a task that repeated calls other tasks (subtasks) to carry out the same operation(s) on a sequence of events (event stream).
//!  Use the 'run' method to execute the job on all selected events. Internally, 'run' calls the 'execute' method repeatedly on each event in the event stream.
//!  Do not call the 'execute' method of this class directly unless you know what you are doing...
//!
//!  Optionally, the iterator may call a subsampleAnalysis method to carry out a sub-sample analysis of all the sub tasks operated by this iterator.
//!
class TaskIterator : public Task
{
public:
  
  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  TaskIterator(const TString &         _name = "TaskIterator",
               const Configuration &   _configuration = Configuration(),
               MessageLogger::LogLevel _reportLevel = MessageLogger::Info);

  //!
  //! DTOR
  //!
  virtual ~TaskIterator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  //!
  //! Run this iterator task.
  //!
  virtual void run();
  
  //!
  //! Execute the subtasks on one event
  //!
  virtual void execute();
  
  //!
  //! Perform a subsample analysis on the output of this iterator task.
  //!
  virtual void subsampleAnalysis();
  
  //!
  //! Externally sets the number of event requested in the analysis. Deprecated: Use the configuration of the task to set this value.
  //!
  inline void setNEventRequested(long _nEventsRequested)    { nEventRequested    = _nEventsRequested; }

  //!
  //! Externally sets the number of reported events. Deprecated: Use the configuration of the task to set this value.
  //!
  inline void setNEventReported(long _nEventsReported)      { nEventReported     = _nEventsReported; }

  //!
  //! Externally sets the number of events required to produce a partial save. Deprecated: Use the configuration of the task to set this value.
  //!
  inline void setNEventPartialSave(long _nEventsPartialSave){ nEventPartialSave  = _nEventsPartialSave; }

  //!
  //! Externally sets whether partial saves should be performed.  Deprecated: Use the configuration of the task to set this value.
  //!
  inline void setPartialSave(bool _doPartialSave)              { doPartialSave         = _doPartialSave; }

  //!
  //! Externally sets whether a subsample analysis should be performed at the end of the job.  Deprecated: Use the configuration of the task to set this value.
  //!
  inline void setSubsampleAnalysis(bool _doSubsampleAnalysis)  { doSubsampleAnalysis   = _doSubsampleAnalysis; }

  //!
  //! Get the number of events requested
  //!
  inline long getNEventRequested() const   { return nEventRequested; }

  //!
  //! Get the number of events reported period
  //!
  inline long getNEventReported() const    { return nEventReported; }

  //!
  //! Get the number of events required to produce a partial save.
  //!
  inline long getNEventPartialSave() const { return nEventPartialSave; }

  inline bool getPartialSave() const       { return doPartialSave; }
  inline bool getSubsampleAnalysis() const { return doSubsampleAnalysis; }

protected:

  Timer timer;
  long nEventRequested;
  long nEventReported;
  long nEventPartialSave;
  long nEventProcessed;
//  int  nTotalGridJob;
//  int  n
  bool gridJob;
  bool processEvents;
  bool createDerivedHistograms;
  bool doPartialSave;
  bool doSubsampleAnalysis;

  ClassDef(TaskIterator,0)
};

#endif /* CAP__TaskIterator */
