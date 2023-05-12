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
//using CAP::Task;
//using CAP::TaskIterator;
//using CAP::Configuration;
using namespace CAP;


ClassImp(TaskIterator);

TaskIterator::TaskIterator(const String & _name,
                           const Configuration & _configuration)
:
Task::Task(_name,_configuration),
isGrid(false),
nEventsPerSubbunch(1),
nSubbunchesPerBunch(1),
nBunches(1),
nEventsRequested(1),
bunchLabel("BUNCH"),
subbunchLabel(""),
iEvent(0),
iSubBunch(0),
iBunch(0)
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
  addParameter("nEventsRequested",        nEventsRequested);
  addParameter("BunchLabel",              bunchLabel);
  addParameter("SubbunchLabel",           subbunchLabel);
}

void TaskIterator::configure()
{
  Task::configure();
  isGrid                 = getValueBool(  "isGrid");
  nEventsPerSubbunch     = getValueInt(   "nEventsPerSubbunch");
  nSubbunchesPerBunch    = getValueInt(   "nSubbunchesPerBunch");
  nBunches               = getValueInt(   "nBunches");
  nEventsRequested       = getValueLong(  "nEventsRequested");
  bunchLabel             = getValueString("BunchLabel");
  subbunchLabel          = getValueString("SubbunchLabel");

  if (reportInfo(__FUNCTION__))
    {
    cout <<  endl;
    printItem("isGrid" ,isGrid);
    printItem("nEventsPerSubbunch" ,nEventsPerSubbunch);
    printItem("nSubbunchesPerBunch" ,nSubbunchesPerBunch);
    printItem("nBunches" ,nBunches);
    printItem("nEventsRequested" ,nEventsRequested);
    printItem("bunchLabel" ,bunchLabel);
    printItem("subbunchLabel" ,subbunchLabel);
    }
}

void TaskIterator::partial(const String & outputPathBase)
{
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Partial save of histograms" << endl;
    }
  String outputPath = outputPathBase;
  if (!isGrid)
    {
    outputPath += "/";
    outputPath += bunchLabel;
    outputPath += Form("%02d",iBunch);
    outputPath += "/";
    outputPath += subbunchLabel;
    outputPath +=  Form("%02d",iSubBunch);
    outputPath += "/";
    }
  for (unsigned int  iTask=0; iTask<getNSubTasks(); iTask++)  subTasks[iTask]->partial(outputPath);
}

void TaskIterator::execute()
{
  timer.start();
  initialize();
  iEvent           = 0;
  iSubBunch        = 0;
  iBunch           = 0;
  bool working     = true;
  while (working)
    {
    for (unsigned int  iTask=0; iTask<getNSubTasks(); iTask++)  subTasks[iTask]->execute();
    iEvent++;
    if (isTaskEod())
      {
      working = false; break;
      }
    if (iEvent>=nEventsRequested)
      {
      if (reportInfo(__FUNCTION__))
        {
        cout << endl;
        printItem("iEvent",iEvent);
        printItem("nEventsRequested",nEventsRequested);
        cout << endl;
        }
      working = false; break;
      }

    if (iEvent%nEventsPerSubbunch==0)
      {
      // subbunch is completed
      if (histosExportPartial) partial(getValueString("HistogramsExportPath"));
      iSubBunch++;
      if (iSubBunch==nSubbunchesPerBunch)
        {
        // bunch is completed
        iSubBunch=0;
        iBunch++;
        if (iBunch==nBunches) working = false;
        }
      }
    }
  if (histosExportPartial && (iEvent%(nBunches*nSubbunchesPerBunch*nEventsPerSubbunch)!=0))
    partial(getValueString("HistogramsExportPath"));

  timer.stop();
  finalize();
  clear(); // should delete everything..
}


void TaskIterator::finalize()
{
  finalizeSubTasks();

  if (reportInfo(__FUNCTION__))
    {
    cout <<  endl<<  endl<<  endl<<  endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    printItem("Task named" ,getName());
    printItem("Completed with status" ,StateManager::getStateManager()->getStateName());
    printItem("isGrid" ,isGrid);
    printItem("nEventsPerSubbunch" ,nEventsPerSubbunch);
    printItem("nSubbunchesPerBunch" ,nSubbunchesPerBunch);
    printItem("nBunches" ,nBunches);
    printItem("nEventsRequested" ,nEventsRequested);
    printItem("bunchLabel" ,bunchLabel);
    printItem("subbunchLabel" ,subbunchLabel);
    printItem("event completed" ,iEvent);
    printItem("subBunches completed" ,iSubBunch);
    printItem("bunches completed" ,iBunch);
    timer.print(cout);
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << "---------------------------------------------------------------------------------------- " <<   endl;
    cout << endl << endl<< endl << endl;
    }

}

