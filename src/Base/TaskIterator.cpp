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
                           Configuration & _configuration)
:
Task(_name,_configuration),
UseParticles(false),
DoPartialReports(false),
DoPartialSaves(false),
DoSubsampleAnalysis(false),
nIterationRequested(1),
nIterationReported(10000),
nIterationPartialSave(10000)
{
  appendClassName("TaskIterator");
}

void TaskIterator::setDefaultConfiguration()
{
  addParameter("UseParticles",          UseParticles);
  addParameter("DoPartialReports",      DoPartialReports);
  addParameter("DoPartialSaves",        DoPartialSaves);
  addParameter("DoSubsampleAnalysis",   DoSubsampleAnalysis);
  addParameter("nIterationRequested",   nIterationRequested);
  addParameter("nIterationReported",    nIterationReported);
  addParameter("nIterationPartialSave", nIterationPartialSave);
  addParameter("UseEventStream0",       true);
}

void TaskIterator::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  postTaskOk();
  UseParticles           = getValueBool("UseParticles");
  DoPartialReports       = getValueBool("DoPartialReports");
  DoPartialSaves         = getValueBool("DoPartialSaves");
  DoSubsampleAnalysis    = getValueBool("DoSubsampleAnalysis");
  nIterationRequested    = getValueLong("nIterationRequested");
  nIterationReported     = getValueLong("nIterationReported");
  nIterationPartialSave  = getValueLong("nIterationPartialSave");

  Task::initialize();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "  Task named..................: " << getName() << endl;
    cout << "  nIterationRequested.........: " << nIterationRequested << endl;
    cout << "  nSubTasks...................: " << getNSubTasks()      << endl;
    cout << "  DoPartialReports............: " << DoPartialReports    << endl;
    cout << "  DoPartialSaves..............: " << DoPartialSaves      << endl;
    cout << "  DoSubsampleAnalysis.........: " << DoSubsampleAnalysis << endl;
    cout << "  nIterationRequested.........: " << nIterationRequested << endl;
    cout << "  nIterationReported..........: " << nIterationReported  << endl;
    cout << "  nIterationPartialSave.......: " << nIterationPartialSave  << endl;
    cout << "  Task status.................: " << getTaskStatusName() << endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << endl;
    cout << endl;
    }
  if (!isTaskOk())
    {
    if (reportWarning(__FUNCTION__)) cout << "Initialization failed. Abort." << endl;
    return;
    }
  if (reportEnd(__FUNCTION__))
    { }
}

void TaskIterator::execute()
{
  timer.start();
  initialize();
  if (!isTaskOk()) return;
  for (long iter=0; iter<nIterationRequested; iter++)
    {
    if (UseParticles)
      {
      Particle::resetFactory();
      Event::resetEventStreams();
      }
    unsigned int nSubTasks = subTasks.size();
    for (unsigned int  iTask=0; iTask<nSubTasks; iTask++)  subTasks[iTask]->execute();
    incrementTaskExecuted();
    if (DoPartialReports  && getnTaskExecuted()%nIterationReported==0) cout << "Completed iteration # " << iter << endl;
    if (DoPartialSaves    && getnTaskExecuted()%nIterationPartialSave==0 )
      {
      savePartialSubTasks();
      if (DoSubsampleAnalysis) reset();
      }

    if (getnTaskExecutedTotal() >= nIterationRequested ) break;
    }
  timer.stop();
  finalize();
  clear(); // should delete everything..
}


void TaskIterator::finalize()
{
  finalizeSubTasks();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "                               Task named : " << getName()<< endl;
    cout << "                    Completed with status : " << getTaskStatusName() << endl;
    cout << "                     Completed iterations : " << getnTaskExecutedTotal() << endl;
    cout << "  Completed iterations since partial save : " << getnTaskExecuted() << endl;
    timer.print(cout);
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << endl << endl<< endl << endl;
    }
  if (reportEnd(__FUNCTION__))
    { }
}


