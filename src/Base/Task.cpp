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

using namespace CAP;

ClassImp(Task);

Task::Task()
:
MessageLogger(Info),
timer(),
taskName                ( "Task" ),
configured              (false),
configuration           (),
requestedConfiguration  (*new Configuration()),
parent                  (nullptr),
particleFactory         (nullptr),
eventStreams            (),
nEventFilters           (0),
nParticleFilters        (0),
eventFilters            (),
particleFilters         (),
inputHistograms         (),
histograms              (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedHistograms       (),
derivedSingleHistograms (),
derivedPairHistograms   (),
filteredParticles       (),
useEventStream0         (false),
useEventStream1         (false),
useEventStream2         (false),
useEventStream3         (false),
useEvents               (false),
useParticles            (false),
scaleHistos             (false),
createHistos            (false),
loadHistos              (false),
saveHistos              (false),
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

Task::Task(const String & _name,
           Configuration & _configuration)
:
MessageLogger(Severity::Info),
timer(),
taskName                (_name),
configured              (false),
configuration           (),
requestedConfiguration  (_configuration),
parent                  (nullptr),
particleFactory         (nullptr),
eventStreams            (),
nEventFilters           (0),
nParticleFilters        (0),
eventFilters            (),
particleFilters         (),
inputHistograms         (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedHistograms       (),
derivedSingleHistograms (),
derivedPairHistograms   (),
filteredParticles       (),
useEventStream0         (false),
useEventStream1         (false),
useEventStream2         (false),
useEventStream3         (false),
useEvents               (false),
useParticles            (false),
scaleHistos             (false),
createHistos            (false),
loadHistos              (false),
saveHistos              (false),
nTaskExecutedTotal      (0),
nTaskExecuted           (0),
nParticlesAcceptedEvent (),
nParticlesAcceptedTotal (),
subTasks                (),
particleTypeCollection  (nullptr),
subSampleIndex(0)
{
  setClassName("Task");
  setInstanceName(_name);
}

Task::Task(const String & _name,
           Configuration & _configuration,
           vector<EventFilter*> & _eventFilters,
           vector<ParticleFilter*>& _particleFilters)
:
MessageLogger(Severity::Info),
timer(),
taskName                (_name),
configured              (false),
configuration           (),
requestedConfiguration  (_configuration),
parent                  (nullptr),
particleFactory         (nullptr),
eventStreams            (),
nEventFilters           (_eventFilters.size()),
nParticleFilters        (_particleFilters.size()),
eventFilters            (_eventFilters),
particleFilters         (_particleFilters),
inputHistograms         (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedHistograms       (),
derivedSingleHistograms (),
derivedPairHistograms   (),
filteredParticles       (),
useEventStream0         (false),
useEventStream1         (false),
useEventStream2         (false),
useEventStream3         (false),
useEvents               (false),
useParticles            (false),
scaleHistos             (false),
createHistos            (false),
loadHistos              (false),
saveHistos              (false),
forceHistogramsRewrite  (true),
nTaskExecutedTotal      (0),
nTaskExecuted           (0),
nParticlesAcceptedEvent (),
nParticlesAcceptedTotal (),
subTasks                (),
particleTypeCollection  (nullptr),
subSampleIndex(0)
{
  setClassName("Task");
  setInstanceName(_name);
}

void Task::setDefaultConfiguration()
{
  String nullString("none");
  String none("none");
  String treeName("tree");
  configuration.clear();
  addParameter("Severity",                String("Debug"));
  addParameter("UseEvents",               useEvents);
  addParameter("UseParticles",            useParticles);
  addParameter("UseEventStream0",         useEventStream0);
  addParameter("UseEventStream1",         useEventStream1);
  addParameter("UseEventStream2",         useEventStream2);
  addParameter("UseEventStream3",         useEventStream3);
  addParameter("CreateHistograms",        createHistos);
  addParameter("LoadHistograms",          loadHistos);
  addParameter("SaveHistograms",          saveHistos);
  addParameter("ResetHistograms",         false);
  addParameter("ClearHistograms",         false);
  addParameter("ScaleHistograms",         scaleHistos);
  addParameter("ForceHistogramsRewrite",  forceHistogramsRewrite);
  addParameter("HistogramInputPath",      none);
  addParameter("HistogramInputFile",      none);
  addParameter("HistogramOutputPath",     none);
  addParameter("HistogramOutputFile",     none);
  addParameter("DataInputUsed",           false);
  addParameter("DataInputPath",           nullString);
  addParameter("DataInputFile",           nullString);
  addParameter("DataInputTree",           treeName);
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
  useEventStream0 = getValueBool("UseEventStream0");
  useEventStream1 = getValueBool("UseEventStream1");
  useEventStream2 = getValueBool("UseEventStream2");
  useEventStream3 = getValueBool("UseEventStream3");
  useEvents       = getValueBool("UseEvents");
  useParticles    = getValueBool("UseParticles");
  scaleHistos     = getValueBool("ScaleHistograms");
  createHistos    = getValueBool("CreateHistograms");
  loadHistos      = getValueBool("LoadHistograms");
  saveHistos      = getValueBool("SaveHistograms");
  forceHistogramsRewrite = getValueBool("ForceHistogramsRewrite");
  MessageLogger::Severity selectedLevel;
  String logOption = getValueString("Severity");
  if (logOption.Contains("Debug"))        selectedLevel = MessageLogger::Debug;
  else if (logOption.Contains("Info"))    selectedLevel = MessageLogger::Info;
  else if (logOption.Contains("Warning")) selectedLevel = MessageLogger::Warning;
  else selectedLevel = MessageLogger::Info;
  setSeverityLevel(selectedLevel);
  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}

void Task::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  initializeTaskExecuted();
  if (useParticles)
    {
    particleFactory        = Particle::getFactory();
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
  if (useEventStream0)  addEventStream(Event::getEventStream(0));
  if (useEventStream1)  addEventStream(Event::getEventStream(1));
  if (useEventStream2)  addEventStream(Event::getEventStream(2));
  if (useEventStream3)  addEventStream(Event::getEventStream(3));
  if (reportDebug(__FUNCTION__)) cout << " #event added streams: "  << getNEventStreams() << endl;
  if (loadHistos    && isTaskOk())  loadHistograms();
  if (createHistos  && isTaskOk())  createHistograms();
  if (hasSubTasks() && isTaskOk())  initializeSubTasks();
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
  if (useParticles  && reportInfo(__FUNCTION__)) printEventStatistics();
  if (scaleHistos)   scaleHistograms();
  if (saveHistos)    saveHistograms();
  if (hasSubTasks()) savePartialSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::finalize()
{
  if (reportStart(__FUNCTION__))
    ;
  // if a partial Save has been done and there are no additional
  // new data, no point in saving again..
  if (useParticles)
    {
    if (getnTaskExecuted()>0)
      {
      if (reportDebug(__FUNCTION__)) printEventStatistics();
      if (scaleHistos)   scaleHistograms();
      if (saveHistos)    saveHistograms();
      if (hasSubTasks()) finalizeSubTasks();
      }
    }
  else
    {
    if (saveHistos)    saveHistograms();
    if (hasSubTasks()) finalizeSubTasks();
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::printEventStatistics() const
{
  if (reportStart(__FUNCTION__))
    ;
  cout << endl;
  cout << " Event Statistics.....: " << getName() <<  "/"  << getClassName()
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
  resetTaskExecuted();
  if (useParticles)  resetNEventsAccepted();
  if (useParticles)  resetNParticlesAccepted();
  if (createHistos)  resetHistograms();
  if (hasSubTasks()) resetSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::clear()
{
  if (reportStart(__FUNCTION__))
    ;
  if (useParticles)  clearNEventsAccepted();
  if (useParticles)  clearNParticlesAccepted();
  if (createHistos)  clearHistograms();
  if (hasSubTasks()) clearSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::printConfiguration(ostream & output)
{
  output << "--------------------------------------------------------" << endl;
  output << "--------------------------------------------------------" << endl;
  output << "Task Name : " << taskName <<  endl;
  configuration.printConfiguration(output);
  if (hasSubTasks()) printConfigurationSubTasks(output);
  output << "--------------------------------------------------------" << endl;
  output << "--------------------------------------------------------" << endl;
}



void Task::loadHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  String histogramInputPath = getValueString("HistogramInputPath");
  String histogramInputFile = getValueString("HistogramInputFile");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "HistogramInputPath..........: " << histogramInputPath << endl;
    cout << "HistogramInputFile..........: " << histogramInputFile << endl;
    }
  TFile * inputFile = openRootFile(histogramInputPath,histogramInputFile,"READ");
  if (!inputFile) return;
  if (useParticles)  loadNEventsAccepted(inputFile);
  if (useParticles)  loadNEexecutedTask(inputFile);
  loadHistograms(inputFile);
  if (reportEnd(__FUNCTION__))
    ;
}


void Task::loadDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  String histogramInputPath = getValueString("HistogramInputPath");
  String histogramInputFile = getValueString("HistogramInputFile");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "HistogramInputPath..........: " << histogramInputPath << endl;
    cout << "HistogramInputFile..........: " << histogramInputFile << endl;
    }
  TFile * inputFile = openRootFile(histogramInputPath,histogramInputFile,"READ");
  if (!inputFile) return;
  if (useParticles)  loadNEventsAccepted(inputFile);
  if (useParticles)  loadNEexecutedTask(inputFile);
  loadDerivedHistograms(inputFile);
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
  inputHistograms.clear();
  histograms.clear();
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  derivedHistograms.clear();
  derivedSingleHistograms.clear();
  derivedPairHistograms.clear();
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


void Task::saveHistograms(TFile * outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!outputFile)
    {
    if (reportError(__FUNCTION__)) cout << "Given file pointer is null" << endl;
    postTaskError();
    return;
    }
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
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::saveHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  String histogramOutputPath     = getValueString("HistogramOutputPath");
  String histogramOutputFile     = getValueString("HistogramOutputFile");
  if (histogramOutputPath.Contains("null") || histogramOutputPath.Contains("none")) histogramOutputPath = "";
  if (histogramOutputFile.Contains("null") || histogramOutputFile.Contains("none")) histogramOutputFile = taskName;
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "nTaskExecuted.............: " << nTaskExecuted  << endl;
    cout << "nTaskExecutedTotal........: " << nTaskExecutedTotal  << endl;
    cout << "HistogramOutputPath.......: " << histogramOutputPath  << endl;
    cout << "HistogramOutputFile.......: " << histogramOutputFile  << endl;
    cout << "Saving....................: " << histogramOutputPath << "/" << histogramOutputFile << endl;
    }
  gSystem->mkdir(histogramOutputPath,1);
  TFile * outputFile;
  if (forceHistogramsRewrite)
    outputFile = openRootFile(histogramOutputPath,histogramOutputFile,"RECREATE");
  else
    outputFile = openRootFile(histogramOutputPath,histogramOutputFile,"NEW");
  if (!outputFile)
    {
    postTaskError();
    return;
    }
  if (useParticles)
    {
    writeNEventsAccepted(outputFile);
    writeNEexecutedTask(outputFile);
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
  String name = "nTaskExecuted";
  writeParameter(outputFile,name,nTaskExecuted);
  if (reportEnd(__FUNCTION__))
    ;
}

long Task::loadNEexecutedTask(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  String name = "nTaskExecuted";
  nTaskExecuted = readParameter(inputFile,name);
  if (reportEnd(__FUNCTION__))
    ;
  return nTaskExecuted;
}

void Task::writeNEventsAccepted(TFile * outputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  String parameterName = "nEventFilters";
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
  String parameterName = "nEventFilters";
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

void Task::writeParameter(TFile * outputFile, const String & parameterName, long value)
{

  if (reportStart(__FUNCTION__))
    ;
  outputFile->cd();
  TParameter<Long64_t>(parameterName,value,'+').Write();
}

long Task::readParameter(TFile * inputFile, const String & parameterName)
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

TFile *  Task::openRootFile(const String & inputPath, const String & fileName, const String & ioOption)
{
  if (reportStart(__FUNCTION__))
    ;
  String inputFileName = inputPath;
  
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
    setDefaultConfiguration();
    setConfiguration(requestedConfiguration);
    if (hasSubTasks()) configureSubTasks();
    configured = true;
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void Task::configureSubTasks()
{
  if (reportStart(__FUNCTION__))
    ;
  unsigned int nSubTasks = subTasks.size();
  if (reportDebug(__FUNCTION__))  cout << "SubTasks Count: " << nSubTasks  << endl;
  for (unsigned int  iTask=0; iTask<nSubTasks; iTask++)
    {
    subTasks[iTask]->configure();
    }
  if (reportEnd(__FUNCTION__))
    ;
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
    ;
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
    if (reportFatal(__FUNCTION__)) cout << "Given task pointer is null. Abort." << endl;
    postTaskFatal();
    return task;
    }
  if (task==this)
    {
    if (reportFatal(__FUNCTION__)) cout << "Given task pointer is self. Abort." << endl;
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
}


vector<String> Task::listFilesInDir(const String & pathname,
                               const String & ext,
                               bool prependPath,
                               bool verbose,
                               int  maximumDepth,
                               int  currentDepth)
{
  String dirname = pathname;
  int depth = currentDepth;
  if (!dirname.EndsWith("/")) dirname += "/";
  if (verbose) cout << " Searching................: " << dirname << endl;
  if (verbose) cout << " maximumDepth.............: " << maximumDepth << endl;
  if (verbose) cout << " currentDepth.............: " << depth << endl;
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  vector<String>  fileNames;
  vector<String>  subdirs;

  if (files)
    {
    TSystemFile *file;
    String fname;
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
  if (verbose) cout << " Number of subdir found...: " << nSubdirs << endl;
  ++depth;

  if (nSubdirs>0 && depth<=maximumDepth)
    {

    for (int iDir=0; iDir<nSubdirs; iDir++)
      {
      vector<String> additionalFiles;
      String  subdirname = dirname;
      subdirname += "/";
      subdirname += subdirs[iDir];
      subdirname += "/";
      additionalFiles = listFilesInDir(subdirname,ext,prependPath,verbose,maximumDepth,depth);
      int nAdditionalfiles = additionalFiles.size();
      for (int iFile=0;iFile<nAdditionalfiles;iFile++)
        fileNames.push_back(additionalFiles[iFile]);
      }
    }
  if (verbose) cout << " Number of files  found...: " << fileNames.size() << endl;
  if (verbose) cout << " Returning up one level.... " <<  endl;

  return fileNames;
}

//!
//! Get file  names at the given location that match all  patterns in includePatterns and exclude patterns in excludePatterns
//!
vector<String>  Task::listFilesInDir(const String & pathName,
                                      vector<String> includePatterns,
                                      vector<String> excludePatterns,
                                      bool prependPath,
                                      bool verbose,
                                      int  maximumDepth,
                                      int  currentDepth)
{

  if (reportStart(__FUNCTION__))
    ;
  vector<String> outputList;
  vector<String> fileList = listFilesInDir(pathName,".root",prependPath,verbose,maximumDepth,currentDepth);
  unsigned int nNames = fileList.size();
  if (reportDebug(__FUNCTION__))
    {
    cout << "      nNames:" << nNames << endl;
    cout << "   nIncludes:" << includePatterns.size() << endl;
    cout << "   nExcludes:" << excludePatterns.size() << endl;
    }
  for (unsigned int k=0; k<fileList.size(); k++)
    {
    String name = fileList[k];
    bool include = true;
    for (unsigned int kInclude=0; kInclude<includePatterns.size(); kInclude++)
      {
      String pattern = includePatterns[kInclude];
      if (!name.Contains(pattern)) { include = false; break;}
      }
    if (!include)
      {
      continue;
      }
    for (unsigned int kExclude=0; kExclude<excludePatterns.size(); kExclude++)
      {
      String pattern = excludePatterns[kExclude];
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
      //String check = pathName+name;
      //cout << " CHECK:::::: " << check << endl;
      outputList.push_back(name);
      }
    }
  return outputList;
}

vector<String> Task::getSelectedFileNamesFrom(const String & folder)
{

  if (reportStart(__FUNCTION__))
    ;
  vector<String> includePatterns;
  vector<String> excludePatterns;
  vector<String> selectedNames;
  for (int k=0; k<20; k++)
    {
    String key = "IncludedPattern"; key += k;
    String  value = getValueString(key);
    if (!value.Contains("none")) includePatterns.push_back(value);
    }
  for (int k=0; k<20; k++)
    {
    String key = "ExcludedPattern"; key += k;
    String  value = getValueString(key);
    if (!value.Contains("none")) excludePatterns.push_back(value);
    }
  return listFilesInDir(folder,includePatterns,excludePatterns);
}

String Task::removeRootExtension(const String fileName)
{
  String name = fileName;
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

String Task::getReverseTaskPath(int depth) const
{
  String result;
  String work;
  String colon = String(":");
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

vector<String> Task::getTaskPathTokens() const
{
  vector<String> paths;
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


String Task::getTaskPath(int depth) const
{
  String path = "";
  vector<String> paths = getTaskPathTokens();
  for (int k=0; k<=depth; k++)
    {
    path += paths[k];
    path += ":";
    }
  return path;
}


String Task::getFullTaskPath() const
{
  String path;
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

bool Task::getValueBool(const String & key)   const
{
  int n = getNAncestors();
  String path;
  String extKey;
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
//    if (key.Contains("forceHistogramsRewrite"))
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

int Task::getValueInt(const String & key)    const
{
  int n = getNAncestors();
  String path;
  String extKey;
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
long Task::getValueLong(const String & key)    const
{
  int n = getNAncestors();
  String path;
  String extKey;
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
double Task::getValueDouble(const String & key) const
{
  int n = getNAncestors();
  String path;
  String extKey;
  for (int k=n; k>=0; k--)
    {
    path   = getTaskPath(k);
    //cout << "getValueDouble()   path:" << path << endl;
    extKey = path + key;
    //cout << "getValueDouble() extKey:" << extKey << endl;
    bool found = configuration.isDouble(extKey);
    if (key.Contains("Energy"))
      {
      cout << "Searching for Energy" << endl;
      cout << "getValueBool()   path:" << path << endl;
      cout << "getValueBool() extKey:" << extKey << endl;
      cout << "getValueBool()  found:" << found << endl;
      }

    if (found) return configuration.getValueDouble(extKey);
    }
  return -99999.0;
}

String Task::getValueString(const String & key) const
{
  int n = getNAncestors();
  String path;
  String extKey;
  for (int k=n; k>=0; k--)
    {
    path   = getTaskPath(k);
    //cout << "getValueString()   path:" << path << endl;
    extKey = path + key;
    //cout << "getValueString() extKey:" << extKey << endl;
    bool found = configuration.isString(extKey);
    if (found) return configuration.getValueString(extKey);
    }
  return String("none");
  }


void Task::addParameter(const String & name, bool value)
{
  String path = getFullTaskPath();
  if (reportDebug("addParameter(const String & name, bool value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.addParameter(path,name,value);
}

void Task::addParameter(const String & name, int value)
{
  String path = getFullTaskPath();
  if (reportDebug("addParameter(const String & name, int value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.addParameter(path,name,value);
}


void Task::addParameter(const String & name, long value)
{
  String path = getFullTaskPath();
  if (reportDebug("addParameter(const String & name, long value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.addParameter(path,name,value);
}


void Task::addParameter(const String & name, double value)
{
  String path = getFullTaskPath();
  if (reportDebug("addParameter(const String & name, double value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.addParameter(path,name,value);
}


void Task::addParameter(const String & name, const String &  value)
{
  String path = getFullTaskPath();
  if (reportDebug("addParameter(const String & name, string value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.addParameter(path,name,value);
}



void Task::setParameter(const String & name, bool value)
{
  String path = getFullTaskPath();
  if (reportDebug("setParameter(const String & name, bool value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.setParameter(path,name,value);
}

void Task::setParameter(const String & name, int value)
{
  String path = getFullTaskPath();
  if (reportDebug("setParameter(const String & name, int value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.setParameter(path,name,value);
}


void Task::setParameter(const String & name, long value)
{
  String path = getFullTaskPath();
  if (reportDebug("setParameter(const String & name, long value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.setParameter(path,name,value);
}

void Task::setParameter(const String & name, double value)
{
  String path = getFullTaskPath();
  if (reportDebug("setParameter(const String & name, double value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.setParameter(path,name,value);
}

void Task::setParameter(const String & name, const String &  value)
{
  String path = getFullTaskPath();
  if (reportDebug("setParameter(const String & name, String value)"))
    cout << " path: " << path << " name: " << name << " value: " << value << endl;
  configuration.setParameter(path,name,value);
}

void Task::generateKeyValuePairs(const String keyBaseName, const String defaultValue, int nKeysToGenerate)
{
  String path = getFullTaskPath();
  configuration.generateKeyValuePairs(path,keyBaseName,defaultValue,nKeysToGenerate);
}

vector<String> Task::getSelectedValues(const String keyBaseName, const String defaultValue) const
{
  String path = getFullTaskPath();
  return configuration.getSelectedValues(path,keyBaseName,defaultValue);
}
