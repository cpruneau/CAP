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
configuration           (),
particleFactory         ( nullptr),
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
nTaskExecuted           (0),
nTaskExecutedReset      (0),
nEventsAcceptedTotal    (),
nParticlesAcceptedEvent (),
nParticlesAcceptedReset (),
nParticlesAcceptedTotal (),
subTasks                (),
particleTypeCollection  (nullptr),
subSampleIndex(0)
{
  setClassName("Task");
  setInstanceName(taskName);
  setDefaultConfiguration();
}

Task::Task(const TString &          _taskName,
           const Configuration &    _configuration __attribute__((unused)),
           MessageLogger::LogLevel  _reportLevel)
:
MessageLogger(_reportLevel),
timer(),
taskName                (_taskName),
configuration           (),
particleFactory         ( nullptr),
eventStreams            (),
nEventFilters           (0),
nParticleFilters        (0),
eventFilters            (),
particleFilters         (),
eventCountHistos        ( nullptr),
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
  setInstanceName(_taskName);
  setDefaultConfiguration();
  setConfiguration(_configuration);

}

Task::Task(const TString &          _taskName,
           const Configuration  &   _configuration,
           vector<EventFilter*> &   _eventFilters,
           vector<ParticleFilter*>& _particleFilters,
           MessageLogger::LogLevel  _reportLevel)
:
MessageLogger(_reportLevel),
taskName                (_taskName),
configuration           (),
particleFactory         ( nullptr),
eventStreams            (),
nEventFilters           (_eventFilters.size()),
nParticleFilters        (_particleFilters.size()),
eventFilters            (_eventFilters),
particleFilters         (_particleFilters),
eventCountHistos        ( nullptr),
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
  setInstanceName(_taskName);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void Task::setDefaultConfiguration()
{
  if (reportStart(__FUNCTION__))
    ;
  TString nullString("");
  TString none("none");
  TString treeName("tree");
  configuration.addParameter( "useEvents",               false);
  configuration.addParameter( "useParticles",            false);
  configuration.addParameter( "useEventStream0",         false);
  configuration.addParameter( "useEventStream1",         false);
  configuration.addParameter( "useEventStream2",         false);
  configuration.addParameter( "useEventStream3",         false);
  configuration.addParameter( "createHistograms",        false);
  configuration.addParameter( "createDerivedHistograms", false);
  configuration.addParameter( "createCombinedHistograms",false);
  configuration.addParameter( "loadHistograms",          false);
  configuration.addParameter( "saveHistograms",          false);
  configuration.addParameter( "resetHistograms",         false);
  configuration.addParameter( "clearHistograms",         false);
  configuration.addParameter( "scaleHistograms",         false);
  configuration.addParameter( "forceHistogramsRewrite",  false);
  configuration.addParameter( "doSubsampleAnalysis",     false);
  configuration.addParameter( "doPartialReports",        false);
  configuration.addParameter( "doPartialSaves",          false);
  configuration.addParameter( "histoInputPath",          nullString);
  configuration.addParameter( "histoInputFileName",      nullString);
  configuration.addParameter( "histoOutputPath",         nullString);
  configuration.addParameter( "histoOutputFileName",     nullString);
  configuration.addParameter( "histoOutputDataName",     nullString);
  configuration.addParameter( "histoOutputAnalyzerName", nullString);
  configuration.addParameter( "histoBaseName",           nullString);
  configuration.addParameter( "dataInputUsed",           false);
  configuration.addParameter( "dataInputPath",           nullString);
  configuration.addParameter( "dataInputFileName",       nullString);
  configuration.addParameter( "dataInputTreeName",       treeName);
  configuration.addParameter( "dataInputFileMinIndex",   -1);
  configuration.addParameter( "dataInputFileMaxIndex",   -1);
  configuration.addParameter( "dataOutputUsed",          false);
  configuration.addParameter( "dataOutputPath",          nullString);
  configuration.addParameter( "dataOutputFileName",      nullString);
  configuration.addParameter( "dataOutputTreeName",      treeName);
  configuration.addParameter( "dataConversionToWac",     true);
  configuration.generateKeyValuePairs("IncludedPattern", none, 20);
  configuration.generateKeyValuePairs("ExcludedPattern", none, 20);
  if (reportDebug(__FUNCTION__))    configuration.printConfiguration(cout);
}

void Task::setConfiguration(const Configuration & _configuration)
{
   if (reportStart(__FUNCTION__))
    ;
  configuration.setParameters(_configuration);
  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

void Task::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  bool useParticles     = configuration.getValueBool("useParticles");
  bool createHistos     = configuration.getValueBool("createHistograms");
  bool loadHistos       = configuration.getValueBool("loadHistograms");
  bool useEventStream0  = configuration.getValueBool("useEventStream0");
  bool useEventStream1  = configuration.getValueBool("useEventStream1");
  bool useEventStream2  = configuration.getValueBool("useEventStream2");
  bool useEventStream3  = configuration.getValueBool("useEventStream3");
  initializeTaskExecuted();
  if (useParticles)
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
      if (reportWarning(__FUNCTION__)) cout << "useParticles==true but nParticleFilters=0 or nEventFilters==0." << endl;
      }
    }
  if (useEventStream0)  addEventStream(Event::getEventStream(0));
  if (useEventStream1)  addEventStream(Event::getEventStream(1));
  if (useEventStream2)  addEventStream(Event::getEventStream(2));
  if (useEventStream3)  addEventStream(Event::getEventStream(3));
  if (reportDebug(__FUNCTION__)) cout << " #event added streams: "  << getNEventStreams() << endl;
  if (createHistos && isTaskOk())
    {
    createEventCountHistograms();
    createHistograms();
    }
  if (loadHistos && isTaskOk())  loadHistograms();
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
  bool useParticles = configuration.getValueBool("useParticles");
  bool scaleHistos  = configuration.getValueBool("scaleHistograms");
  bool saveHistos   = configuration.getValueBool("saveHistograms");
  if (useParticles  && reportInfo(__FUNCTION__)) printEventStatistics();
  if (scaleHistos   && isTaskOk()) scaleHistograms();
  if (saveHistos    && isTaskOk()) saveHistograms();
  if (hasSubTasks() && isTaskOk()) savePartialSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}



void Task::finalize()
{
  if (reportStart(__FUNCTION__))
    ;
  bool useParticles = configuration.getValueBool("useParticles");
  bool scaleHistos  = configuration.getValueBool("scaleHistograms");
  bool saveHistos   = configuration.getValueBool("saveHistograms");

  // if a partial save has been done and there are no additional
  // new data, no point in saving again...
  if (useParticles)
    {
    if (reportInfo(__FUNCTION__)) cout << "useParticles==true" << endl;
    if (getNTaskExecutedReset()>0)
      {
      if (reportInfo(__FUNCTION__)) cout << "getNTaskExecutedReset()>0" << endl;
      if (reportInfo(__FUNCTION__)) printEventStatistics();
      if (scaleHistos) scaleHistograms();
      if (saveHistos) saveHistograms();
      if (hasSubTasks() && isTaskOk()) finalizeSubTasks();
      }
    }
  else
    {
    if (reportInfo(__FUNCTION__)) cout << "useParticles==false" << endl;
    if (scaleHistos) scaleHistograms();
    if (saveHistos) saveHistograms();
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
  << "       #calls after reset/total: " << nTaskExecutedReset << "/" << nTaskExecuted << " #EventFilters : " << nEventFilters << " #ParticleFilters : " << nParticleFilters << endl;
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++)
    {
    cout << "       Event filter : " << iEventFilter << " name : " << eventFilters[iEventFilter]->getName() << " #events after reset/total : " << getNEventsAcceptedReset(iEventFilter) << "/" << getNEventsAcceptedTotal(iEventFilter) << endl;
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++)
      {
      cout << "       Particle filter : " << iParticleFilter << " name : " << particleFilters[iParticleFilter]->getName() << " #particles last/after reset/total : " << getNParticlesAcceptedEvent(iEventFilter,iParticleFilter) << "/"
      << getNParticlesAcceptedReset(iEventFilter,iParticleFilter) << "/" << getNParticlesAcceptedTotal(iEventFilter,iParticleFilter) << endl;
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}



void Task::reset()
{

  if (reportStart(__FUNCTION__))
    ;
  bool useParticles = configuration.getValueBool("useParticles");
  bool createHistos = configuration.getValueBool("createHistograms");
  resetTaskExecuted();
  if (useParticles) resetNEventsAccepted();
  if (useParticles) resetNParticlesAccepted();
  if (useParticles  && createHistos) resetEventCountHistograms();
  if (createHistos  && isTaskOk()) resetHistograms();
  if (hasSubTasks() && isTaskOk()) resetSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::clear()
{

  if (reportStart(__FUNCTION__))
    ;
  bool useParticles = configuration.getValueBool("useParticles");
  bool createHistos = configuration.getValueBool("createHistograms");
  if (useParticles) clearNEventsAccepted();
  if (useParticles) clearNParticlesAccepted();
  if (useParticles  && createHistos) clearEventCountHistograms();
  if (createHistos  && isTaskOk()) clearHistograms();
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
  bool useParticles          = configuration.getValueBool("useParticles");
  TString histoInputPath     = configuration.getValueString("histoInputPath");
  TString histoInputFileName = configuration.getValueString("histoInputFileName");

  TFile * inputFile          = openRootFile(histoInputPath,histoInputFileName,"READ");
  if (!inputFile) return;
  if (useParticles)  loadEventCountHistograms(inputFile);
  if (useParticles)  loadNEventsAccepted(inputFile);
  if (useParticles)  loadNEexecutedTask(inputFile);
  loadHistograms(inputFile);
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::createEventCountHistograms()
{

  if (reportStart(__FUNCTION__))
    ;
  eventCountHistos = new EventCountHistos(getName(),configuration,nEventFilters,nParticleFilters,getReportLevel());
  eventCountHistos->createHistograms();
  if (reportEnd(__FUNCTION__))
    ;
}


void Task::fillEventCountHistograms()
{

  if (reportStart(__FUNCTION__))
    ;
  eventCountHistos->fill(nTaskExecutedReset,nEventsAcceptedReset,nParticleAcceptedReset);
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::loadEventCountHistograms(TFile * inputFile)
{

  if (reportStart(__FUNCTION__))
    ;
  eventCountHistos = new EventCountHistos(getName(),configuration,nEventFilters,nParticleFilters,getReportLevel());
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
  eventCountHistos->clear();
  delete eventCountHistos;
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
    long nEvents = nEventsAcceptedReset[iEventFilter];
    if (nEvents>1)
      {
      scalingFactor = 1.0/double(nEvents);
      if (reportInfo(__FUNCTION__))
        {
        cout << endl;
        cout << "                            iEventFilter: " <<  iEventFilter<< endl;
        cout << "           nEventsAccepted[iEventFilter]: " <<  nEvents<< endl;
        cout << "                           scalingFactor: " <<  scalingFactor << endl;
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
        cout << "      nEventsAcceptedReset[iEventFilter]: " <<  nEventsAcceptedReset[iEventFilter]<< endl;
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
  outputFile->cd();

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "         global histogram(s):"  << histograms.size() << endl;
    cout << "    base single histogram(s):"  << baseSingleHistograms.size() << endl;
    cout << "      base pair histogram(s):"  << basePairHistograms.size() << endl;
    cout << " derived global histogram(s):"  << derivedHistograms.size() << endl;
    cout << " derived single histogram(s):"  << derivedSingleHistograms.size() << endl;
    cout << "   derived pair histogram(s):"  << derivedPairHistograms.size() << endl;
    cout << "       combined histogram(s):"  << combinedHistograms.size() << endl;
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
  bool useParticles               = configuration.getValueBool("useParticles");
  bool doSubsampleAnalysis        = configuration.getValueBool("doSubsampleAnalysis");
  bool doPartialSave              = configuration.getValueBool("doPartialSaves");
  bool forceHistogramsRewrite     = configuration.getValueBool("forceHistogramsRewrite");
  TString histoOutputPath         = configuration.getValueString("histoOutputPath");
  TString histoOutputFileName     = configuration.getValueString("histoOutputFileName");
  TString histoOutputDataName     = configuration.getValueString("histoOutputDataName");
  TString histoOutputAnalyzerName = configuration.getValueString("histoOutputAnalyzerName");

  // rule: if an actual file name 'histoOutputFileName' is provided, that is what is
  // used to save the histograms. If not, assemble a name based on 'histoOutputDataName' and 'histoOutputAnalyzerName'
  // If the partial save option is set, a sequence number will be added to the file name.
  //
  if (histoOutputFileName.IsNull())
    {
    histoOutputFileName  = histoOutputDataName;
    histoOutputFileName  += "_";
    histoOutputFileName  += histoOutputAnalyzerName;
    }

  if (doSubsampleAnalysis || doPartialSave )
    {
    histoOutputFileName += "_";
    histoOutputFileName += subSampleIndex++;
    }
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  nTaskExecutedReset : " << nTaskExecutedReset  << endl;
    cout << "       nTaskExecuted : " << nTaskExecuted  << endl;
    cout << "     histoOutputPath : " << histoOutputPath  << endl;
    cout << " histoOutputFileName : " << histoOutputFileName  << endl;
    }
  TFile * outputFile;
  if (forceHistogramsRewrite)
    outputFile = openRootFile(histoOutputPath,histoOutputFileName,"RECREATE");
  else
    outputFile = openRootFile(histoOutputPath,histoOutputFileName,"NEW");
  if (!outputFile) return;
  if (useParticles)
    {
    // this next fill is done once per save (after reset)
    fillEventCountHistograms();
    writeNEventsAccepted(outputFile);
    writeNEexecutedTask(outputFile);
    saveEventCountHistograms(outputFile);
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
  TString name = "ExecutedTaskReset";
  writeParameter(outputFile,name,nTaskExecutedReset);
  if (reportEnd(__FUNCTION__))
    ;
}

long Task::loadNEexecutedTask(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  TString name = "ExecutedTaskReset";
  nTaskExecutedReset = readParameter(inputFile,name);
  if (reportEnd(__FUNCTION__))
    ;
  return nTaskExecutedReset;
}

void Task::writeNEventsAccepted(TFile * outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  for (int iFilter=0; iFilter<nEventFilters; iFilter++)
    {
    TString name = "EventFilter";
    name += iFilter;
    writeParameter(outputFile,name,nEventsAcceptedReset[iFilter]);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::loadNEventsAccepted(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  for (int iFilter=0; iFilter<nEventFilters; iFilter++)
    {
    TString name = "EventFilter";
    name += iFilter;
    nEventsAcceptedReset[iFilter] = readParameter(inputFile,name);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::writeParameter(TFile * outputFile, const TString parameterName, long value)
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
    if (reportError(__FUNCTION__)) cout << "Parameter not found." << endl;
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
  if (reportStart(__FUNCTION__))
    ;
  if (!task)
    {
    if (reportFatal()) cout << "Given task pointer is null. Abort." << endl;
    postTaskFatal();
    return task;
    }
  subTasks.push_back( task );
  if (reportDebug(__FUNCTION__)) cout << "Added task " << task->getName() << " to task " << getName() << endl;
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
//    TString name = "nEventsAccepted"; name += k;
//    list->Add(new TParameter<Long64_t>(name,nEventsAccepted[k],'+'));
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

const TString Task::createHistogramName(const TString & baseName,
                                        const TString & evtFilterName,
                                        const TString & partFilterName1,
                                        const TString & observableName,
                                        const TString & suffix)
{
  TString histoName  = baseName;
  histoName += evtFilterName;
  histoName += "_";
  histoName += partFilterName1;
  histoName += observableName;
  histoName += suffix;
  return histoName;
}

const TString Task::createPairHistogramName(const TString & baseName,
                                            const TString & evtFilterName,
                                            const TString & partFilterName1,
                                            const TString & partFilterName2,
                                            const TString & observableName,
                                            const TString & suffix)
{
  TString histoName  = baseName;
  histoName += evtFilterName;
  histoName += "_";
  histoName += partFilterName1;
  histoName += partFilterName2;
  histoName += observableName;
  histoName += suffix;
  return histoName;
}

vector<TString> Task::listFilesInDir(const TString & dirname,
                                     const TString ext)
{

  if (reportStart(__FUNCTION__))
    ;
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  vector<TString>  fileNames;
  
  if (files)
    {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=(TSystemFile*)next()))
      {
      fname = file->GetName();
      if (!file->IsDirectory() && fname.EndsWith(ext))
        {
        fileNames.push_back(fname);
        //cout << fname.Data() << endl;
        //cout << fname << endl;
        }
      }
    }
  return fileNames;
}

//!
//! Get file  names at the given location that match all  patterns in includePatterns and exclude patterns in excludePatterns
//!
vector<TString>  Task::listFilesInDir(const TString & pathName,
                                      vector<TString> includePatterns,
                                      vector<TString> excludePatterns)
{

  if (reportStart(__FUNCTION__))
    ;
  vector<TString> outputList;
  vector<TString> fileList = listFilesInDir(pathName,".root");
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
      outputList.push_back(pathName+name);
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
    TString  value = configuration.getValueString(key);
    if (!value.Contains("none")) includePatterns.push_back(value);
    }
  for (int k=0; k<20; k++)
    {
    TString key = "ExcludedPattern"; key += k;
    TString  value = configuration.getValueString(key);
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



