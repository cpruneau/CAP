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
isGrid(false),
nEventsPerSubbunch(100),
nSubbunchesPerBunch(10),
nBunches(10),
bunchLabel("BUNCH"),
subbunchLabel("")
{
  appendClassName("TaskIterator");
}

void TaskIterator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  addParameter("isGrid",                  isGrid);
  addParameter("nEventsPerSubbunch",      nEventsPerSubbunch);
  addParameter("nSubbunchesPerBunch",     nSubbunchesPerBunch);
  addParameter("nBunches",                nBunches);
  addParameter("BunchLabel",              bunchLabel);
  addParameter("SubbunchLabel",           subbunchLabel);
}

void TaskIterator::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  isGrid                 = getValueBool(  "isGrid");
  nEventsPerSubbunch     = getValueLong(  "nEventsPerSubbunch");
  nSubbunchesPerBunch    = getValueInt(   "nSubbunchesPerBunch");
  nBunches               = getValueInt(   "nBunches");
  bunchLabel             = getValueString("BunchLabel");
  subbunchLabel          = getValueString("SubbunchLabel");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "  Task named.......................: " << getName()           << endl;
    cout << "  isGrid...........................: " << isGrid              << endl;
    cout << "  nEventsPerSubbunch...............: " << nEventsPerSubbunch  << endl;
    cout << "  nSubbunchesPerBunch..............: " << nSubbunchesPerBunch << endl;
    cout << "  nBunches.........................: " << nBunches            << endl;
    cout << "  BunchLabel.......................: " << bunchLabel          << endl;
    cout << "  SubbunchLabel....................: " << subbunchLabel       << endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
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
  TString outputPathBase = getValueString("HistogramOutputPath");

  for (int iBunch=1; iBunch<=nBunches; iBunch++)
    {
    if (reportInfo(__FUNCTION__))  cout << " Beginning bunch index: " << iBunch << endl;
    for (int iSubbunch=0; iSubbunch<nSubbunchesPerBunch; iSubbunch++)
      {
      if (reportInfo(__FUNCTION__))  cout << " Beginning subbunch index: " << iSubbunch << endl;
      long iEvent=0;
      while (iEvent<nEventsPerSubbunch)
        {
        if (reportDebug(__FUNCTION__)) cout << " Beginning event index: " << iEvent << endl;
        if (useParticles)
          {
          Particle::resetFactory();
          Event::resetEventStreams();
          }
        unsigned int nSubTasks = subTasks.size();
        for (unsigned int  iTask=0; iTask<nSubTasks; iTask++)  subTasks[iTask]->execute();
        if (isTaskEod()) break;
        incrementTaskExecuted();
        if (reportDebug(__FUNCTION__))  cout << " Completed event index: " << iEvent << endl;
        iEvent++;
        }

      unsigned int nSubTasks = subTasks.size();
      if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
      for (unsigned int  iTask=0; iTask<nSubTasks; iTask++)
        {
        TString outputPath = outputPathBase;
        if (!isGrid)
          {
          outputPath += "/";
          outputPath += bunchLabel;
          outputPath += Form("%02d",iBunch);
          outputPath += "/";
          outputPath += subbunchLabel;
          outputPath +=  Form("%02d",iSubbunch);
          outputPath += "/";
          }
        subTasks[iTask]->setParameter("HistogramOutputPath",outputPath);
        subTasks[iTask]->savePartial();
        }
      reset();
      }
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
    cout << "                    Completed with status : " << StateManager::getStateManager()->getStateName() << endl;
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


