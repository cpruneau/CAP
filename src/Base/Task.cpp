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
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TParameter.h"
#include "Task.hpp"

ClassImp(Task);

Task::Task()
:
MessageLogger(Info),
taskName                ( "Task" ),
configured              (false),
requestedConfiguration  (*new Configuration()),
configuration           (),
parent                  (nullptr),
particleFactory         (nullptr),
eventStreams            (),
nEventFilters           (0),
nParticleFilters        (0),
eventFilters            (),
particleFilters         (),
eventCountHistos        ( nullptr),
inputHistograms         (),
histograms              (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedHistograms       (),
derivedSingleHistograms (),
derivedPairHistograms   (),
combinedHistograms      (),
filteredParticles       (),
nTaskExecutedTotal      (0),
nTaskExecuted           (0),
nParticlesAcceptedEvent (),
nParticlesAcceptedTotal (),
subTasks                (),
particleTypeCollection  (nullptr),
subSampleIndex(0)
{
  setClassName("Task");
  setInstanceName(taskName);
}

Task::Task(const TString & _name,
           Configuration & _configuration)
:
MessageLogger(LogLevel::Info),
timer(),
taskName                (_name),
configured              (false),
requestedConfiguration  (_configuration),
configuration           (),
parent                  (nullptr),
particleFactory         (nullptr),
eventStreams            (),
nEventFilters           (0),
nParticleFilters        (0),
eventFilters            (),
particleFilters         (),
eventCountHistos        (nullptr),
inputHistograms         (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedHistograms       (),
derivedSingleHistograms (),
derivedPairHistograms   (),
combinedHistograms      (),
particleTypeCollection  (nullptr),
subSampleIndex(0)
{
  setClassName("Task");
  setInstanceName(_name);
}

Task::Task(const TString & _name,
           Configuration & _configuration,
           vector<EventFilter*> & _eventFilters,
           vector<ParticleFilter*>& _particleFilters)
:
MessageLogger(LogLevel::Info),
taskName                (_name),
configured              (false),
requestedConfiguration  (_configuration),
configuration           (),
parent                  (nullptr),
particleFactory         (nullptr),
eventStreams            (),
nEventFilters           (_eventFilters.size()),
nParticleFilters        (_particleFilters.size()),
eventFilters            (_eventFilters),
particleFilters         (_particleFilters),
eventCountHistos        (nullptr),
inputHistograms         (),
histograms              (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedHistograms       (),
derivedSingleHistograms (),
derivedPairHistograms   (),
combinedHistograms      (),
particleTypeCollection  (nullptr),
subSampleIndex(0)
{
  setClassName("Task");
  setInstanceName(_name);
}

void Task::setDefaultConfiguration()
{
  TString nullString("none");
  TString none("none");
  TString treeName("tree");
  configuration.clear();
  addParameter("LogLevel",                TString("Debug"));
  addParameter("UseEvents",               false);
  addParameter("UseParticles",            false);
  addParameter("UseEventStream0",         false);
  addParameter("UseEventStream1",         false);
  addParameter("UseEventStream2",         false);
  addParameter("UseEventStream3",         false);
  addParameter("CreateHistograms",        false);
  addParameter("LoadHistograms",          false);
  addParameter("SaveHistograms",          false);
  addParameter("ResetHistograms",         false);
  addParameter("ClearHistograms",         false);
  addParameter("ScaleHistograms",         false);
  addParameter("ForceHistogramsRewrite",  false);
  addParameter("DoSubsampleAnalysis",     false);
  addParameter("DoPartialReports",        false);
  addParameter("DoPartialSaves",          false);
  addParameter("HistogramInputPath",      none);
  addParameter("HistogramInputFile",      none);
  addParameter("HistogramOutputPath",     none);
  addParameter("HistogramOutputFile",     none);
  addParameter("DataInputUsed",           false);
  addParameter("DataInputPath",           nullString);
  addParameter("DataInputFile",       nullString);
  addParameter("DataInputTree",       treeName);
  addParameter("DataInputFileMinIndex",   -1);
  addParameter("DataInputFileMaxIndex",   -1);
  addParameter("DataOutputUsed",          false);
  addParameter("DataOutputPath",          nullString);
  addParameter("DataOutputFileName",      nullString);
  addParameter("DataOutputTreeName",      treeName);
  addParameter("DataConversionToWac",     true);
  generateKeyValuePairs("IncludedPattern", none, 20);
  generateKeyValuePairs("ExcludedPattern", none, 20);
}

void Task::setConfiguration(Configuration & _configuration)
{
  configuration.setParameters(_configuration);
  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

void Task::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  TString logOption = getValueString("LogLevel");
  MessageLogger::LogLevel selectedLevel  = MessageLogger::Info;
  if (logOption.Contains("Debug"))        selectedLevel = MessageLogger::Debug;
  else if (logOption.Contains("Info"))    selectedLevel = MessageLogger::Info;
  else if (logOption.Contains("Warning")) selectedLevel = MessageLogger::Warning;
  else selectedLevel = MessageLogger::Info;
  setReportLevel(selectedLevel);


  bool UseParticles     = getValueBool("UseParticles");
  bool CreateHistos     = getValueBool("CreateHistograms");
  bool LoadHistos       = getValueBool("LoadHistograms");
  bool UseEventStream0  = getValueBool("UseEventStream0");
  bool UseEventStream1  = getValueBool("UseEventStream1");
  bool UseEventStream2  = getValueBool("UseEventStream2");
  bool UseEventStream3  = getValueBool("UseEventStream3");
  TString reportLevel   = getValueString("LogLevel");
  if (reportLevel.Contains("Debug")) setReportLevel(MessageLogger::Debug);
  if (reportLevel.Contains("Info"))  setReportLevel(MessageLogger::Info);

  initializeTaskExecuted();
  if (UseParticles)
    {
    particleFactory  = Particle::getFactory();
    particleTypeCollection = ParticleTypeCollection::getMasterParticleCollection();
    if (nParticleFilters>0 && nEventFilters>0)
      {
      initializeNEventsAccepted();
      initializeNParticlesAccepted();
      }
    else
      {
      if (reportWarning(__FUNCTION__)) cout << "UseParticles==true but nParticleFilters=0 or nEventFilters==0." << endl;
      }
    }
  if (UseEventStream0)  addEventStream(Event::getEventStream(0));
  if (UseEventStream1)  addEventStream(Event::getEventStream(1));
  if (UseEventStream2)  addEventStream(Event::getEventStream(2));
  if (UseEventStream3)  addEventStream(Event::getEventStream(3));
  if (reportDebug(__FUNCTION__)) cout << " #event added streams: "  << getNEventStreams() << endl;
  if (LoadHistos   && isTaskOk())  loadHistograms();
  if (CreateHistos && isTaskOk())
    {
    //if (UseParticles && !hasEventCountHistograms()) createEventCountHistograms();
    createHistograms();
    }
  if (hasSubTasks() && isTaskOk()) initializeSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::savePartial()
{
  if (reportStart(__FUNCTION__))
    ;
  bool UseParticles = getValueBool("UseParticles");
  bool ScaleHistos  = getValueBool("ScaleHistograms");
  bool SaveHistos   = getValueBool("SaveHistograms");
  if (UseParticles  && reportInfo(__FUNCTION__)) printEventStatistics();
  if (ScaleHistos   && isTaskOk()) scaleHistograms();
  if (SaveHistos    && isTaskOk()) saveHistograms();
  if (hasSubTasks() && isTaskOk()) savePartialSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::finalize()
{
  if (reportStart(__FUNCTION__))
    ;
  bool UseParticles = getValueBool("UseParticles");
  bool ScaleHistos  = getValueBool("ScaleHistograms");
  bool SaveHistos   = getValueBool("SaveHistograms");

  // if a partial Save has been done and there are no additional
  // new data, no point in saving again..
  if (UseParticles)
    {
    if (getnTaskExecuted()>0)
      {
      if (reportDebug(__FUNCTION__)) printEventStatistics();
      if (ScaleHistos) scaleHistograms();
      if (SaveHistos) saveHistograms();
      if (hasSubTasks() && isTaskOk()) finalizeSubTasks();
      }
    }
  else
    {
    if (SaveHistos) saveHistograms();
    if (hasSubTasks() && isTaskOk()) finalizeSubTasks();
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::printEventStatistics() const
{
  if (reportStart(__FUNCTION__))
    ;
  cout << endl;
  cout << "Event Statistics : " << getName() <<  "/"  << getClassName()
  << "       #calls after Reset/total: " << nTaskExecuted << "/" << nTaskExecutedTotal << " #EventFilters : " << nEventFilters << " #ParticleFilters : " << nParticleFilters << endl;
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++)
    {
    cout << "Event filter:" << iEventFilter << " " << eventFilters[iEventFilter]->getName() << " #events : " << getNEventsAcceptedReset(iEventFilter) << endl;
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++)
      {
      cout
      << "Particle filter: "  << iParticleFilter
      << " " << particleFilters[iParticleFilter]->getName()
      << " (event): " << getNParticlesAccepted(iEventFilter,iParticleFilter)
      << " (total): " << getNParticlesAcceptedTotal(iEventFilter,iParticleFilter) << endl;
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::reset()
{
  if (reportStart(__FUNCTION__))
    ;
  bool UseParticles = getValueBool("UseParticles");
  bool CreateHistos = getValueBool("CreateHistograms");
  resetTaskExecuted();
  if (UseParticles) resetNEventsAccepted();
  if (UseParticles) resetNParticlesAccepted();
  //if (UseParticles  && CreateHistos) resetEventCountHistograms();
  if (CreateHistos  && isTaskOk()) resetHistograms();
  if (hasSubTasks() && isTaskOk()) resetSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::clear()
{
  if (reportStart(__FUNCTION__))
    ;
  bool UseParticles = getValueBool("UseParticles");
  bool CreateHistos = getValueBool("CreateHistograms");
  if (UseParticles) clearNEventsAccepted();
  if (UseParticles) clearNParticlesAccepted();
  //if (UseParticles) clearEventCountHistograms();
  if (CreateHistos  && isTaskOk()) clearHistograms();
  if (hasSubTasks() && isTaskOk()) clearSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::printConfiguration(ostream & output)
{
  output << "Task Name : " << taskName <<  endl;
  configuration.printConfiguration(output);
  if (hasSubTasks()) printConfigurationSubTasks(output);
}



void Task::loadHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  bool UseParticles          = getValueBool("UseParticles");
  TString histogramInputPath = getValueString("HistogramInputPath");
  TString histogramInputFile = getValueString("HistogramInputFile");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "HistogramInputPath..........: " << histogramInputPath << endl;
    cout << "HistogramInputFile..........: " << histogramInputFile << endl;
    }
  TFile * inputFile = openRootFile(histogramInputPath,histogramInputFile,"READ");
  if (!inputFile) return;
  //if (UseParticles)  loadEventCountHistograms(inputFile);
  if (UseParticles)  loadNEventsAccepted(inputFile);
  if (UseParticles)  loadNEexecutedTask(inputFile);
  loadHistograms(inputFile);
  if (reportEnd(__FUNCTION__))
    ;
}


void Task::loadDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  bool UseParticles          = getValueBool("UseParticles");
  TString histogramInputPath = getValueString("HistogramInputPath");
  TString histogramInputFile = getValueString("HistogramInputFile");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "HistogramInputPath..........: " << histogramInputPath << endl;
    cout << "HistogramInputFile..........: " << histogramInputFile << endl;
    }
  TFile * inputFile = openRootFile(histogramInputPath,histogramInputFile,"READ");
  if (!inputFile) return;
  //if (UseParticles)  loadEventCountHistograms(inputFile);
  if (UseParticles)  loadNEventsAccepted(inputFile);
  if (UseParticles)  loadNEexecutedTask(inputFile);
  loadDerivedHistograms(inputFile);
  if (reportEnd(__FUNCTION__))
    ;
}


bool Task::hasEventCountHistograms()
{
  return eventCountHistos!=nullptr;
}


void Task::createEventCountHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  eventCountHistos = new EventCountHistos(this,getName(),configuration,nEventFilters,nParticleFilters,getReportLevel());
  eventCountHistos->createHistograms();
  eventCountHistos->setOwnership(true);
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::fillEventCountHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  eventCountHistos->fill(nTaskExecuted,nEventsAccepted,nParticleAccepted);
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::loadEventCountHistograms(TFile * inputFile)
{

  if (reportStart(__FUNCTION__))
    ;
  eventCountHistos = new EventCountHistos(this,getName(),configuration,nEventFilters,nParticleFilters,getReportLevel());
  eventCountHistos->loadHistograms(inputFile);
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::saveEventCountHistograms(TFile * outputFile)
{

  if (reportStart(__FUNCTION__))
    ;
  eventCountHistos->saveHistograms(outputFile);
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::resetEventCountHistograms()
{

  if (reportStart(__FUNCTION__))
    ;
  eventCountHistos->reset();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::clearEventCountHistograms()
{

  if (reportStart(__FUNCTION__))
    ;
  if (eventCountHistos)
    {
    eventCountHistos->clear();
    delete eventCountHistos;
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::resetHistograms()
{

  if (reportStart(__FUNCTION__))
    ;
  for (unsigned int iHisto=0; iHisto<inputHistograms.size();        iHisto++) inputHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<histograms.size();             iHisto++) histograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<baseSingleHistograms.size();   iHisto++) baseSingleHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<basePairHistograms.size();     iHisto++) basePairHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<derivedHistograms.size();      iHisto++) derivedHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<derivedSingleHistograms.size();iHisto++) derivedSingleHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<derivedPairHistograms.size();  iHisto++) derivedPairHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<combinedHistograms.size();     iHisto++) combinedHistograms[iHisto]->reset();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::clearHistograms()
{

  if (reportStart(__FUNCTION__))
    ;
  for (unsigned int iHisto=0; iHisto<inputHistograms.size();        iHisto++) delete inputHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<histograms.size();             iHisto++) delete histograms[iHisto];
  for (unsigned int iHisto=0; iHisto<baseSingleHistograms.size();   iHisto++) delete baseSingleHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<basePairHistograms.size();     iHisto++) delete basePairHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<derivedHistograms.size();      iHisto++) delete derivedHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<derivedSingleHistograms.size();iHisto++) delete derivedSingleHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<derivedPairHistograms.size();  iHisto++) delete derivedPairHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<combinedHistograms.size();     iHisto++) delete combinedHistograms[iHisto];
  inputHistograms.clear();
  histograms.clear();
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  derivedHistograms.clear();
  derivedSingleHistograms.clear();
  derivedPairHistograms.clear();
  combinedHistograms.clear();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::scaleHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  double scalingFactor;
  int index;
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    long nEvents = nEventsAccepted[iEventFilter];
    if (nEvents>1)
      {
      scalingFactor = 1.0/double(nEvents);
      if (reportInfo(__FUNCTION__))
        {
        cout << endl;
        cout << "iEventFilter.............................: " <<  iEventFilter<< endl;
        cout << "nEventsAcceptedTotal[iEventFilter].......: " <<  nEvents<< endl;
        cout << "scalingFactor............................: " <<  scalingFactor << endl;
        }
      if (histograms.size()>0)
        {
          if (reportInfo(__FUNCTION__)) cout <<  "Scaling group named " <<  histograms[iEventFilter]->getName() << endl;
          histograms[iEventFilter]->scale(scalingFactor);
        }
      if (baseSingleHistograms.size()>0)
        {
        for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
          {
          index = iEventFilter*nParticleFilters + iParticleFilter1;
          if (reportInfo(__FUNCTION__))cout <<  "Scaling group index:" << index << " named " <<  baseSingleHistograms[index]->getName() << endl;
          baseSingleHistograms[index]->scale(scalingFactor);
          }
        }
      if (basePairHistograms.size()>0)
        {
        for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
          {
          for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
            {
            index = iEventFilter*nParticleFilters*nParticleFilters+iParticleFilter1*nParticleFilters+iParticleFilter2;
            if (reportInfo(__FUNCTION__))cout <<  "Scaling group index:" << index << " named " <<  baseSingleHistograms[index]->getName() << endl;
            basePairHistograms[index]->scale(scalingFactor);
            }
          }
        }
      }
    else
      {
      if (reportWarning(__FUNCTION__))
        {
        cout << endl;
        cout << "                            iEventFilter: " <<  iEventFilter<< endl;
        cout << "      nEventsAccepted[iEventFilter]: " <<  nEventsAccepted[iEventFilter]<< endl;
        cout << "                    no scaling performed: " <<  endl;
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::saveHistogramsAsText()
{

  if (reportStart(__FUNCTION__))
    ;
}


void Task::saveHistograms(TFile * outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__,outputFile)) return;
  outputFile->cd();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Global histogram(s).............:"  << histograms.size() << endl;
    cout << "Base single histogram(s)........:"  << baseSingleHistograms.size() << endl;
    cout << "Base pair histogram(s)..........:"  << basePairHistograms.size() << endl;
    cout << "Derived global histogram(s).....:"  << derivedHistograms.size() << endl;
    cout << "Derived single histogram(s).....:"  << derivedSingleHistograms.size() << endl;
    cout << "Derived pair histogram(s).......:"  << derivedPairHistograms.size() << endl;
    cout << "Combined histogram(s)...........:"  << combinedHistograms.size() << endl;
    }
  for (unsigned int iHisto=0; iHisto<histograms.size(); iHisto++)
    {
    if (reportDebug(__FUNCTION__)) cout << "Saving global histogram(s) group:" << iHisto << endl;
    histograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<baseSingleHistograms.size(); iHisto++)
    {
    if (reportDebug(__FUNCTION__)) cout << "Saving single histogram(s) group:" << iHisto << endl;
    baseSingleHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<basePairHistograms.size(); iHisto++)
    {
    if (reportDebug(__FUNCTION__)) cout << "Saving pair histogram(s) group:" << iHisto << endl;
    basePairHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<derivedHistograms.size(); iHisto++)
    {
    if (reportDebug(__FUNCTION__)) cout << "Saving global histogram(s) group:" << iHisto << endl;
    derivedHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<derivedSingleHistograms.size(); iHisto++)
    {
    if (reportDebug(__FUNCTION__)) cout << "Saving derived single histogram(s) group:" << iHisto  << endl;
    derivedSingleHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<derivedPairHistograms.size(); iHisto++)
    {
    if (reportDebug(__FUNCTION__)) cout << "Saving derived pair histogram(s) group:" << iHisto  << endl;
    derivedPairHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<combinedHistograms.size(); iHisto++)
    {
    if (reportDebug(__FUNCTION__)) cout << "Saving combined histogram(s) group:" <<  iHisto << endl;
    combinedHistograms[iHisto]->saveHistograms(outputFile);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::saveHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  bool UseParticles               = getValueBool("UseParticles");
  bool DoSubsampleAnalysis        = getValueBool("DoSubsampleAnalysis");
  bool DoPartialSave              = getValueBool("DoPartialSaves");
  bool ForceHistogramsRewrite     = getValueBool("ForceHistogramsRewrite");
  TString HistogramOutputPath     = getValueString("HistogramOutputPath");
  TString HistogramOutputFile     = getValueString("HistogramOutputFile");

  // rule: if an actual file name 'HistogramOutputFile' is provided, that is what is
  // used to Save the histograms. If not, assemble a name based on 'histoModelDataName' and 'histoAnalyzerName'
  // If the partial Save option is set, a sequence number will be added to the file name.

  if (HistogramOutputPath.Contains("null") || HistogramOutputPath.Contains("none")) HistogramOutputPath = "";
  if (HistogramOutputFile.Contains("null") || HistogramOutputFile.Contains("none")) HistogramOutputFile = taskName;
  if ((DoSubsampleAnalysis || DoPartialSave) &&  HistogramOutputPath.Length()>2)
    {
    HistogramOutputPath += "/Partial_";
    HistogramOutputPath += subSampleIndex++;
    HistogramOutputPath += "/";
    }

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "nTaskExecuted.............: " << nTaskExecuted  << endl;
    cout << "nTaskExecutedTotal........: " << nTaskExecutedTotal  << endl;
    cout << "HistogramOutputPath.......: " << HistogramOutputPath  << endl;
    cout << "HistogramOutputFile.......: " << HistogramOutputFile  << endl;
    cout << "Saving....................: " << HistogramOutputPath << "/" << HistogramOutputFile << endl;
    }
  gSystem->mkdir(HistogramOutputPath,1);
  TFile * outputFile;
  if (ForceHistogramsRewrite)
    outputFile = openRootFile(HistogramOutputPath,HistogramOutputFile,"RECREATE");
  else
    outputFile = openRootFile(HistogramOutputPath,HistogramOutputFile,"NEW");
  if (!outputFile) return;
  if (UseParticles)
    {
    // this next fill is done once per Save (after reset)
    //fillEventCountHistograms();  // do not do in finalize derived
    writeNEventsAccepted(outputFile);
    writeNEexecutedTask(outputFile);
    //saveEventCountHistograms(outputFile);
    }
  saveHistograms(outputFile);
  outputFile->Close();
 if (reportEnd(__FUNCTION__))
    ;
}

void Task::writeNEexecutedTask(TFile * outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  TString name = "nTaskExecuted";
  writeParameter(outputFile,name,nTaskExecuted);
  if (reportEnd(__FUNCTION__))
    ;
}

long Task::loadNEexecutedTask(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  TString name = "nTaskExecuted";
  nTaskExecuted = readParameter(inputFile,name);
  if (reportEnd(__FUNCTION__))
    ;
  return nTaskExecuted;
}

void Task::writeNEventsAccepted(TFile * outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  TString parameterName = "nEventFilters";
  writeParameter(outputFile,parameterName,nEventFilters);

  for (int iFilter=0; iFilter<nEventFilters; iFilter++)
    {
    parameterName = "EventFilter";
    parameterName += iFilter;
    writeParameter(outputFile,parameterName,nEventsAccepted[iFilter]);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::loadNEventsAccepted(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  TString parameterName = "nEventFilters";
  nEventFilters = readParameter(inputFile,parameterName);
  if (nEventsAccepted.size()<1) nEventsAccepted.assign(nEventFilters,0);
  for (int iFilter=0; iFilter<nEventFilters; iFilter++)
    {
    parameterName = "EventFilter";
    parameterName += iFilter;
    nEventsAccepted[iFilter] = readParameter(inputFile,parameterName);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::writeParameter(TFile * outputFile, const TString & parameterName, long value)
{

  if (reportStart(__FUNCTION__))
    ;
  outputFile->cd();
  TParameter<Long64_t>(parameterName,value,'+').Write();
}

long Task::readParameter(TFile * inputFile, const TString & parameterName)
{
  if (reportStart(__FUNCTION__))
    ;
  TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile->Get(parameterName);
  if (!par)
    {
    if (reportError(__FUNCTION__)) cout << "Parameter not found:" <<  parameterName << endl;
    postTaskError();
    return 1.0;
    }
  double value = par->GetVal();
  delete par;
  if (reportDebug(__FUNCTION__)) cout << "Parameter named " << parameterName << " has value : " << value << endl;
  return value;
}

TFile *  Task::openRootFile(const TString & inputPath, const TString & fileName, const TString & ioOption)
{
  if (reportStart(__FUNCTION__))
    ;
  TString inputFileName = inputPath;
  
  // make sure that if an inputPath is given, it ends with '/'
  int slash = inputFileName.First('/');
  int len = inputFileName.Length();
  //  cout << slash << endl;
  //  cout << len << endl;
  if (len>0 && (len-1)!=slash)
    inputFileName += "/";
  
  inputFileName += fileName;
  if (!inputFileName.Contains(".root")) inputFileName += ".root";
  
  if (reportDebug (__FUNCTION__))  cout << "Opening file: " << inputFileName << " with option: " << ioOption << endl;
  TFile * inputFile = new TFile(inputFileName,ioOption);
  if (!inputFile)
    {
    if (reportError(__FUNCTION__)) cout << "File named " << inputFileName << " not found." << endl;
    postTaskError();
    return nullptr;
    }
  else
    {
    if (!inputFile->IsOpen())
      {
      if (reportError(__FUNCTION__)) cout << "File named " << inputFileName << " not found." << endl;
      postTaskError();
      return nullptr;
      }
    else
      {
      if (reportDebug(__FUNCTION__)) cout << "File opened successfully." << endl;
      }
    }
  return inputFile;
}

void Task::configure()
{
  if (reportStart(__FUNCTION__))
    ;
  if (configured)
    {
    if (reportInfo(__FUNCTION__)) cout << "Task " << taskName << " already configured. Skip." << endl;
    }
  else
    {
    if (reportInfo(__FUNCTION__)) cout << "setDefaultConfiguration for task " << taskName  << endl;
    setDefaultConfiguration();
    if (reportInfo(__FUNCTION__)) cout << "Requested configuration is" <<  endl;
    requestedConfiguration.printConfiguration(cout);
    if (reportInfo(__FUNCTION__)) cout << "Now set requested configuration" <<  endl;
    setConfiguration(requestedConfiguration);
    if (reportInfo(__FUNCTION__)) cout << "Set requested configuration completed" <<  endl;

    if (hasSubTasks() && isTaskOk()) configureSubTasks();
    configured = true;
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::configureSubTasks()
{
  if (reportInfo(__FUNCTION__)) cout << " Configure task named:" << getName() << endl;
  unsigned int nSubTasks = subTasks.size();
  if (reportInfo(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++)
    {
    if (reportInfo(__FUNCTION__))  cout << " Calling configure method of task named:" << subTasks[iTask]->getName() << endl;
    subTasks[iTask]->configure();
    }
  if (reportEnd(__FUNCTION__)) cout << " Done configuring task named:" << getName() << endl;
}



void Task::initializeSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->initialize();
  if (reportEnd(__FUNCTION__))
    { }
}

void Task::executeSubTasks()
{
//
//  if (reportStart(__FUNCTION__))
//    ;
  unsigned int nSubTasks = subTasks.size();
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->execute();
}

void Task::finalizeSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->finalize();
  if (reportEnd(__FUNCTION__))
    { }
}

void Task::resetSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->reset();
  if (reportEnd(__FUNCTION__))
    { }
}

void Task::clearSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->clear();
  if (reportEnd(__FUNCTION__))
    { }
}


void Task::savePartialSubTasks()
{
   if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->savePartial();
  if (reportEnd(__FUNCTION__))
    { }
}

void Task::printConfigurationSubTasks(ostream & output)
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++) subTasks[iTask]->printConfiguration(output);
}


Task * Task::addSubTask(Task * task)
{
  if (!task)
    {
    if (reportFatal()) cout << "Given task pointer is null. Abort." << endl;
    postTaskFatal();
    return task;
    }
  if (task==this)
    {
    if (reportFatal()) cout << "Given task pointer is self. Abort." << endl;
    postTaskFatal();
    return task;
    }

  subTasks.push_back( task );
  if (task->parent == nullptr)  task->setParent(this);
  if (reportInfo(__FUNCTION__)) cout << "Added task " << task->getName() << " to task " << getName() << endl;
  return task;
}

void Task::addHistogramsToExtList(TList *list __attribute__((unused)) )
{
//
//  TString fctName = "addHistogramsToExtList(TList *list)";
//  if (reportStart("Task",getName(),fctName))
//    ;
//  if (!list)
//    {
//    if (reportError("Task",getName(),fctName))
//      {
//      cout << "Given list pointer is null." << endl;
//      }
//    postTaskWarning();
//    return;
//    }
//  list->Add(new TParameter<Long64_t>("nEventProcessed",getNEventProcessed(),'+'));
//
//  for (unsigned int k=0; eventFilters.size(); k++)
//    {
//    TString name = "nEventsAcceptedTotal"; name += k;
//    list->Add(new TParameter<Long64_t>(name,nEventsAcceptedTotal[k],'+'));
//    }
//  for (unsigned int k=1; k<baseSingleHistograms.size(); k++)
//    {
//    baseSingleHistograms[k]->addHistogramsToExtList(list);
//    }
//  for (unsigned int k=1; k<basePairHistograms.size(); k++)
//    {
//    basePairHistograms[k]->addHistogramsToExtList(list);
//    }
//  for (unsigned int k=1; k<derivedPairHistograms.size(); k++)
//    {
//    derivedPairHistograms[k]->addHistogramsToExtList(list);
//    }
}

const TString Task::makeHistoName(const TString & s0,
                                  const TString & s1)
{
  TString histoName = s0;
  histoName += "_";
  histoName += s1;
  return histoName;
}

const TString Task::makeHistoName(const TString & s0,
                                  const TString & s1,
                                  const TString & s2)
{
  TString histoName = s0;
  histoName += "_";
  histoName += s1;
  histoName += "_";
  histoName += s2;
  return histoName;
}

const TString Task::makeHistoName(const TString & s0,
                                        const TString & s1,
                                        const TString & s2,
                                        const TString & s3)
{
  TString histoName = s0;
  histoName += "_";
  histoName += s1;
  histoName += "_";
  histoName += s2;
  histoName += "_";
  histoName += s3;
  return histoName;
}


const TString Task::makeHistoName(const TString & s0,
                                        const TString & s1,
                                        const TString & s2,
                                        const TString & s3,
                                        const TString & s4)
{
  TString histoName = s0;
  histoName += "_";
  histoName += s1;
  histoName += "_";
  histoName += s2;
  histoName += "_";
  histoName += s3;
  histoName += "_";
  histoName += s4;
  return histoName;
}

const TString Task::makeHistoName(const TString & s0,
                                            const TString & s1,
                                            const TString & s2,
                                            const TString & s3,
                                            const TString & s4,
                                            const TString & s5)
{
  TString histoName  = s0;
  histoName += "_";
  histoName += s1;
  histoName += "_";
  histoName += s2;
  histoName += "_";
  histoName += s3;
  histoName += "_";
  histoName += s4;
  histoName += "_";
  histoName += s5;
  return histoName;
}


vector<TString> Task::listFilesInDir(const TString & pathname,
                               const TString & ext,
                               bool prependPath,
                               bool verbose,
                               int  maximumDepth,
                               int  currentDepth)
{
  TString dirname = pathname;
  int depth = currentDepth;
  if (!dirname.EndsWith("/")) dirname += "/";
  if (verbose) cout << " Searching: " << dirname << endl;
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  vector<TString>  fileNames;
  vector<TString>  subdirs;

  if (files)
    {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=(TSystemFile*)next()))
      {
      fname = file->GetName();
      if (file->IsDirectory() &&  !fname.BeginsWith(".") )
        {
        subdirs.push_back(fname);
        }
      else if (fname.EndsWith(ext))
        {
        if (prependPath)
          fileNames.push_back(dirname+fname);
        else
          fileNames.push_back(fname);
        //cout << fname.Data() << endl;
        if (verbose) cout << fname << endl;
        }
      }
    }
  int nSubdirs = subdirs.size();
  if (verbose) cout << " Number of subdir found: " << nSubdirs << endl;
  ++depth;

  if (nSubdirs>0 && depth<=maximumDepth)
    {

    for (int iDir=0; iDir<nSubdirs; iDir++)
      {
      vector<TString> additionalFiles;
      TString  subdirname = dirname;
      subdirname += "/";
      subdirname += subdirs[iDir];
      subdirname += "/";
      additionalFiles = listFilesInDir(subdirname,ext,prependPath,verbose,maximumDepth,depth);
      int nAdditionalfiles = additionalFiles.size();
      for (int iFile=0;iFile<nAdditionalfiles;iFile++)
        fileNames.push_back(additionalFiles[iFile]);
      }
    }
  if (verbose) cout << " Number of files  found: " << fileNames.size() << endl;
  return fileNames;
}

//!
//! Get file  names at the given location that match all  patterns in includePatterns and exclude patterns in excludePatterns
//!
vector<TString>  Task::listFilesInDir(const TString & pathName,
                                      vector<TString> includePatterns,
                                      vector<TString> excludePatterns,
                                      bool prependPath,
                                      bool verbose,
                                      int  maximumDepth,
                                      int  currentDepth)
{

  if (reportStart(__FUNCTION__))
    ;
  vector<TString> outputList;
  vector<TString> fileList = listFilesInDir(pathName,".root",prependPath,verbose,maximumDepth,currentDepth);
  unsigned int nNames = fileList.size();
  if (reportDebug(__FUNCTION__))
    {
    cout << "      nNames:" << nNames << endl;
    cout << "   nIncludes:" << includePatterns.size() << endl;
    cout << "   nExcludes:" << excludePatterns.size() << endl;
    }
  for (unsigned int k=0; k<fileList.size(); k++)
    {
    TString name = fileList[k];
    bool include = true;
    for (unsigned int kInclude=0; kInclude<includePatterns.size(); kInclude++)
      {
      TString pattern = includePatterns[kInclude];
      if (!name.Contains(pattern)) { include = false; break;}
      }
    if (!include)
      {
      continue;
      }
    for (unsigned int kExclude=0; kExclude<excludePatterns.size(); kExclude++)
      {
      TString pattern = excludePatterns[kExclude];
      if (name.Contains(pattern))
        {
        include = false;
        break;
        }
      }
    if (include)
      {
        if (name.Contains(".root"))
          {
          int dot = name.First('.');
          int len = name.Length();
          if (dot==len-5 )
          name.Remove(dot,len-dot);
          }
      //TString check = pathName+name;
      //cout << " CHECK:::::: " << check << endl;
      outputList.push_back(name);
      }
    }
  return outputList;
}

vector<TString> Task::getSelectedFileNamesFrom(const TString & folder)
{

  if (reportStart(__FUNCTION__))
    ;
  vector<TString> includePatterns;
  vector<TString> excludePatterns;
  vector<TString> selectedNames;
  for (int k=0; k<20; k++)
    {
    TString key = "IncludedPattern"; key += k;
    TString  value = getValueString(key);
    if (!value.Contains("none")) includePatterns.push_back(value);
    }
  for (int k=0; k<20; k++)
    {
    TString key = "ExcludedPattern"; key += k;
    TString  value = getValueString(key);
    if (!value.Contains("none")) excludePatterns.push_back(value);
    }
  return listFilesInDir(folder,includePatterns,excludePatterns);
}

TString Task::removeRootExtension(const TString fileName)
{
  TString name = fileName;
  int dot = name.Last('.');
  int len = name.Length();
  //  cout << dot << endl;
  //  cout << len << endl;
  if (dot==len-5 ) name.Remove(dot,len-dot);
  return name;
}

Task * Task::getTaskAt(int depth)
{
  //if (depth == 0) return this;
  Task * task = this;
  Task * parentTask = task->getParent();
  for (int k=0; k<depth; k++)
    {
    task = parentTask;
    if (task==nullptr) break;
    parentTask = task->getParent();
    }
  return task;
}


const Task * Task::getTaskAt(int depth) const
{
  //if (depth == 0) return this;
  const Task * task = this;
  const Task * parentTask = task->getParent();
  //cout <<  "0 : name: " << task->getName();
  for (int k=0; k<depth; k++)
    {
    task = parentTask;
    if (task==nullptr) break;
    //cout << k <<  " : name: " << task->getName();
    parentTask = task->getParent();
    }
  return task;
  }

TString Task::getReverseTaskPath(int depth) const
{
  TString result;
  TString work;
  TString colon = TString(":");
  for (int k=0; k<=depth; k++)
    {
    const Task * task = getTaskAt(k);
    if (task!=nullptr)
      {
      work = task->getName();
      work += colon;
      if (result.Length()>0)
        {
        result = work + result;
        }
      else
        {
        result = work;
        }
      }
    }
  return result;
}

vector<TString> Task::getTaskPathTokens() const
{
  vector<TString> paths;
  int n = getNAncestors();
  for (int k=n; k>=0; k--)
    {
    const Task * task= getTaskAt(k);
    if (task==nullptr)
      {
      cout << "<F> getTaskPathTokens()  Logic error." << endl;
      exit(1);
      }
    else
      {
      paths.push_back(task->getName());
      }
    }
  return paths;
}


TString Task::getTaskPath(int depth) const
{
  TString path = "";
  vector<TString> paths = getTaskPathTokens();
  for (int k=0; k<=depth; k++)
    {
    path += paths[k];
    path += ":";
    }
  return path;
}


TString Task::getFullTaskPath() const
{
  TString path;
  //cout << "getFullTaskPath() -- 1" << endl;
  int n = getNAncestors();
  //cout << "getFullTaskPath() -- 2" << endl;
  path = getReverseTaskPath(n);
  //cout << "getFullTaskPath() -- 3" << endl;
  return path;
}


int Task::getNAncestors() const
{
  int count = 0;
  if (parent==nullptr) return count;
  Task * parentTask = parent;
  while (parentTask!=nullptr)
    {
    count++;
    if (parentTask!=nullptr)
      parentTask = parentTask->getParent();
    else
      break;
    }
  return count;
}

bool Task::getValueBool(const TString & key)   const
{
  int n = getNAncestors();
  TString path;
  TString extKey;
  bool result = false;
  for (int k=n; k>=0; k--)
    {
    path   = getTaskPath(k);
    extKey = path + key;
    bool found = configuration.isBool(extKey);
    if (key.Contains("PairGen"))
      {
      cout << "Searching for PairGen" << endl;
      cout << "getValueBool()   path:" << path << endl;
      cout << "getValueBool() extKey:" << extKey << endl;
      cout << "getValueBool()  found:" << found << endl;
      }
//    if (key.Contains("ForceHistogramsRewrite"))
//      {
//      cout << "Searching for ForceHistogramsRewrite" << endl;
//      cout << "getValueBool()   path:" << path << endl;
//      cout << "getValueBool() extKey:" << extKey << endl;
//      cout << "getValueBool()  found:" << found << endl;
//      }
    if (found)
      {
      result = configuration.getValueBool(extKey);
      break;
      }
    }
//    if (key.Contains("ForceHistogramsRewrite")) exit(1);
  return result;
}

int Task::getValueInt(const TString & key)    const
{
  int n = getNAncestors();
  TString path;
  TString extKey;
  for (int k=n; k>=0; k--)
    {
    path   = getTaskPath(k);
    //cout << "getValueInt()   path:" << path << endl;
    extKey = path + key;
    //cout << "getValueInt() extKey:" << extKey << endl;
    bool found = configuration.isInt(extKey);
    //cout << "getValueInt()  found:" << found << endl;
    if (found) return configuration.getValueInt(extKey);
    }
  return -99999;
}

//!
//! Get the value of the parameter named 'name'
//!
long Task::getValueLong(const TString & key)    const
{
  int n = getNAncestors();
  TString path;
  TString extKey;
  for (int k=n; k>=0; k--)
    {
    path   = getTaskPath(k);
    //cout << "getValueLong()   path:" << path << endl;
    extKey = path + key;
    //cout << "getValueLong() extKey:" << extKey << endl;
    bool found = configuration.isLong(extKey);
    if (found) return configuration.getValueLong(extKey);
    }
  return -99999;
}

//!
//! Get the value of the parameter named 'name'
//!
double Task::getValueDouble(const TString & key) const
{
  int n = getNAncestors();
  TString path;
  TString extKey;
  for (int k=n; k>=0; k--)
    {
    path   = getTaskPath(k);
    //cout << "getValueDouble()   path:" << path << endl;
    extKey = path + key;
    //cout << "getValueDouble() extKey:" << extKey << endl;
    bool found = configuration.isDouble(extKey);
    if (found) return configuration.getValueDouble(extKey);
    }
  return -99999.0;
}

TString Task::getValueString(const TString & key) const
{
  int n = getNAncestors();
  TString path;
  TString extKey;
  for (int k=n; k>=0; k--)
    {
    path   = getTaskPath(k);
    //cout << "getValueString()   path:" << path << endl;
    extKey = path + key;
    //cout << "getValueString() extKey:" << extKey << endl;
    bool found = configuration.isString(extKey);
    if (found) return configuration.getValueString(extKey);
    }
  return TString("none");
  }


void Task::addParameter(const TString & name, bool value)
{
  TString path = getFullTaskPath();
  //cout << "addParameter() full path is: " << path << endl;
  configuration.addParameter(path,name,value);
}

void Task::addParameter(const TString & name, int value)
{
  TString path = getFullTaskPath();
  //cout << "addParameter() full path is: " << path << endl;
  configuration.addParameter(path,name,value);
}


void Task::addParameter(const TString & name, long value)
{
  TString path = getFullTaskPath();
  //cout << "addParameter() full path is: " << path << endl;
  configuration.addParameter(path,name,value);
}


void Task::addParameter(const TString & name, double value)
{
  TString path = getFullTaskPath();
  //cout << "addParameter() full path is: " << path << endl;
  configuration.addParameter(path,name,value);
}


void Task::addParameter(const TString & name, const TString &  value)
{
  TString path = getFullTaskPath();
  configuration.addParameter(path,name,value);
}



void Task::setParameter(const TString & name, bool value)
{
  TString path = getFullTaskPath();
  configuration.setParameter(path,name,value);
}

void Task::setParameter(const TString & name, int value)
{
  TString path = getFullTaskPath();
  configuration.setParameter(path,name,value);
}


void Task::setParameter(const TString & name, long value)
{
  TString path = getFullTaskPath();
  configuration.setParameter(path,name,value);
}


void Task::setParameter(const TString & name, double value)
{
  TString path = getFullTaskPath();
  configuration.setParameter(path,name,value);
}


void Task::setParameter(const TString & name, const TString &  value)
{
  TString path = getFullTaskPath();
  configuration.setParameter(path,name,value);
}

void Task::generateKeyValuePairs(const TString keyBaseName, const TString defaultValue, int nKeysToGenerate)
{
  TString path = getFullTaskPath();
  configuration.generateKeyValuePairs(path,keyBaseName,defaultValue,nKeysToGenerate);
}

vector<TString> Task::getSelectedValues(const TString keyBaseName, const TString defaultValue) const
{
  TString path = getFullTaskPath();
  return configuration.getSelectedValues(path,keyBaseName,defaultValue);
}
