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
useParticles(false),
doPartialReports(false),
doPartialSaves(false),
doSubsampleAnalysis(false),
nIterationRequested(1),
nIterationReported(10000),
nIterationPartialSave(10000)
{
  appendClassName("TaskIterator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void TaskIterator::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  configuration.addParameter( "useParticles",          useParticles);
  configuration.addParameter( "doPartialReports",      doPartialReports);
  configuration.addParameter( "doPartialSaves",        doPartialSaves);
  configuration.addParameter( "doSubsampleAnalysis",   doSubsampleAnalysis);
  configuration.addParameter( "nIterationRequested",   nIterationRequested);
  configuration.addParameter( "nIterationReported",    nIterationReported);
  configuration.addParameter( "nIterationPartialSave", nIterationPartialSave);
//  if (reportEnd("TaskIterator",getName(),"setDefaultConfiguration()"))
//    {
//    configuration.printConfiguration(cout);
//    }
}

void TaskIterator::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  postTaskOk();
  useParticles           = configuration.getValueBool( "useParticles");
  doPartialReports       = configuration.getValueBool( "doPartialReports");
  doPartialSaves         = configuration.getValueBool( "doPartialSaves");
  doSubsampleAnalysis    = configuration.getValueBool( "doSubsampleAnalysis");
  nIterationRequested    = configuration.getValueLong( "nIterationRequested");
  nIterationReported     = configuration.getValueLong( "nIterationReported");
  nIterationPartialSave  = configuration.getValueLong( "nIterationPartialSave");

  Task::initialize();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl  << endl << endl << endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "                               Task named : " << getName() << endl;
    cout << "                      nIterationRequested : " << nIterationRequested << endl;
    cout << "                                nSubTasks : " << getNSubTasks() << endl;
    cout << "                         doPartialReports : " << doPartialReports << endl;
    cout << "                           doPartialSaves : " << doPartialSaves  << endl;
    cout << "                      nIterationRequested : " << nIterationRequested  << endl;
    cout << "                       nIterationReported : " << nIterationReported  << endl;
    cout << "                    nIterationPartialSave : " << nIterationPartialSave  << endl;
    cout << "                              Task status : " << getTaskStatusName() << endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
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
    if (useParticles)
      {
      Particle::resetFactory();
      Event::resetEventStreams();
      }
    unsigned int nSubTasks = subTasks.size();
    for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->execute();
    incrementTaskExecuted();
    if (doPartialReports  && getnTaskExecuted()%nIterationReported==0) cout << "Completed iteration # " << iter << endl;
    if (doPartialSaves    && getnTaskExecuted()%nIterationPartialSave==0 )
      {
      savePartial();
      if (doSubsampleAnalysis) reset();
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


