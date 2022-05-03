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
#include "TaskIterator.hpp"
ClassImp(TaskIterator);

TaskIterator::TaskIterator(const TString & _name,
                           const Configuration & _configuration,
                           MessageLogger::LogLevel _reportLevel)
:
Task(_name,_configuration,_reportLevel),
timer(),
nEventRequested(1),
nEventReported(1),
nEventPartialSave(10),
nEventProcessed(0),
doPartialSave(0),
doSubsampleAnalysis(0)
{
  appendClassName("TaskIterator");
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void TaskIterator::setDefaultConfiguration()
{
  if (reportStart("TaskIterator",getName(),"setDefaultConfiguration()"))
    { }
  configuration.addParameter( "nEventsRequested",   nEventRequested);
  configuration.addParameter( "nEventReported",     nEventReported);
  configuration.addParameter( "nEventPartialSave",  nEventPartialSave);
  configuration.addParameter( "doPartialSave",      doPartialSave);
  configuration.addParameter( "doSubsampleAnalysis",doSubsampleAnalysis);
  if (reportEnd("TaskIterator",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}

void TaskIterator::run()
{
  timer.start();
  nEventRequested     = configuration.getValueLong( "nEventsRequested"   );
  nEventReported      = configuration.getValueLong( "nEventReported"     );
  nEventPartialSave   = configuration.getValueLong( "nEventPartialSave"  );
  doPartialSave       = configuration.getValueBool( "doPartialSave"      );
  doSubsampleAnalysis = configuration.getValueBool( "doSubsampleAnalysis");

  if (reportInfo("TaskIterator",getName(),"run(...)")) cout << "Running for nEvent: " << nEventRequested << endl;
  postTaskOk();
  initializeTasks();
  if (!isTaskOk())
    {
    if (reportWarning("TaskIterator",getName(),"run(...)")) cout << "Initialization failed. Abort." << endl;
    return;
    }
  nEventProcessed = 0;
  if (reportInfo("TaskIterator",getName(),"run(...)")) cout << "Starting with nEventRequested:" << nEventRequested << endl;
  for (long iEvent=1; iEvent<=nEventRequested; iEvent++)
    {
    executeTasks();
    if (!isTaskOk())
      {
      if (reportWarning("TaskIterator",getName(),"run(...)")) cout << "Error detected after event:" << iEvent << endl;
      break;
      }
    if (nEventProcessed%nEventReported==0 )
      {
      if (reportInfo("TaskIterator",getName(),"run(...)")) cout << "Completed event # " << iEvent << endl;
      timer.stop();
      timer.print(cout);
      }
    if ( (doSubsampleAnalysis||doPartialSave) && nEventProcessed%nEventPartialSave==0)
      {
      savePartialTasks();
      if (!isTaskOk()) break;
      if (doSubsampleAnalysis) resetTasks();
      postTaskOk();
      }
    }
  if (isTaskOk()) finalizeTasks();

  if (isTaskOk() && doSubsampleAnalysis)
    {
    subsampleAnalysisTasks();
    }

  timer.stop();
  if (reportInfo("TaskIterator",getName(),"run(...)"))
    {
    cout << endl;
    cout << "  Completed with status : " << getTaskStatusName() << endl;
    cout << "       Completed Events : " << nEventProcessed << endl;
    cout << "       Requested Events : " << nEventRequested << endl;
    cout << "            "; timer.print(cout);
    cout << endl << endl<< endl << endl;
    }
}


void TaskIterator::execute()
{
  ++nEventProcessed;
  ++nEventAccepted;
  Particle::resetFactory();
  Event::resetEventStreams();
}

void TaskIterator::subsampleAnalysis()
{
  // the master has no subsample to analyze...
}
