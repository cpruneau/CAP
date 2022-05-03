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
#include "SubSampleStatCalculator.hpp"

ClassImp(Task);

Task::Task()
:
MessageLogger(Info),
taskName                ( "Task" ),
configuration           (),
taskRandomGenerator     ( gRandom),
particleFactory         ( nullptr),
eventStreams            (),
eventFilters            (),
particleFilters         (),
particleSmearers        (),
eventCountHistos        ( nullptr),
inputHistograms         (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedSingleHistograms (),
derivedPairHistograms   (),
combinedHistograms      (),
nFilteredEventsAccepted (),
nEventProcessed         ( 0 ),
nEventAccepted          ( 0 ),
subSampleIndex          ( 0 ),
subtasks                (),
masterCollection        (nullptr)
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
taskRandomGenerator     ( gRandom),
particleFactory         ( nullptr),
eventStreams            (),
eventFilters            (),
particleFilters         (),
particleSmearers        (),
eventCountHistos        ( nullptr),
inputHistograms         (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedSingleHistograms (),
derivedPairHistograms   (),
combinedHistograms      (),
nFilteredEventsAccepted (),
nEventProcessed         ( 0 ),
nEventAccepted          ( 0 ),
subSampleIndex          ( 0 ),
subtasks                (),
masterCollection        (nullptr)
{
  setClassName("Task");
  setInstanceName(_taskName);
  setDefaultConfiguration();
}

Task::Task(const TString &          _taskName,
           const Configuration  &   _configuration __attribute__((unused)),
           vector<EventFilter*> &   _eventFilters,
           vector<ParticleFilter*>& _particleFilters,
           MessageLogger::LogLevel  _reportLevel)
:
MessageLogger(_reportLevel),
taskName                (_taskName),
configuration           (),
taskRandomGenerator     ( gRandom),
particleFactory         ( nullptr),
eventStreams            (),
eventFilters            (_eventFilters),
particleFilters         (_particleFilters),
particleSmearers        (),
eventCountHistos        ( nullptr),
inputHistograms         (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedSingleHistograms (),
derivedPairHistograms   (),
combinedHistograms      (),
nFilteredEventsAccepted (),
nEventProcessed         ( 0 ),
nEventAccepted          ( 0 ),
subSampleIndex          ( 0 ),
subtasks                (),
masterCollection        ( nullptr)
{
  setClassName("Task");
  setInstanceName(_taskName);
  setDefaultConfiguration();
}

Task::Task(const TString &           _taskName,
           const Configuration  &    _configuration  __attribute__((unused)),
           vector<EventFilter*> &    _eventFilters,
           vector<ParticleFilter*>&  _particleFilters,
           vector<ParticleSmearer*>& _particleSmearer,
           MessageLogger::LogLevel   _reportLevel)
:
MessageLogger           (_reportLevel),
taskName                (_taskName),
configuration           (),
taskRandomGenerator     ( gRandom),
particleFactory         ( nullptr),
eventStreams            (),
eventFilters            (_eventFilters),
particleFilters         (_particleFilters),
particleSmearers        (_particleSmearer),
eventCountHistos        (nullptr),
inputHistograms         (),
baseSingleHistograms    (),
basePairHistograms      (),
derivedSingleHistograms (),
derivedPairHistograms   (),
combinedHistograms      (),
nFilteredEventsAccepted (),
nEventProcessed         ( 0 ),
nEventAccepted          ( 0 ),
subSampleIndex          ( 0 ),
subtasks                (),
masterCollection        ( nullptr)
{
  setClassName("Task");
  setInstanceName(_taskName);
  setDefaultConfiguration();
}


void Task::setDefaultConfiguration()
{
  if (reportStart("Task",getName(),"setDefaultConfiguration()"))
    { }
  configuration.addParameter( "useParticles",       false);
  configuration.addParameter( "useEventStream0",    false);
  configuration.addParameter( "useEventStream1",    false);
  configuration.addParameter( "useEventStream2",    false);
  configuration.addParameter( "useEventStream3",    false);
  configuration.addParameter( "createHistograms",   false);
  configuration.addParameter( "createDerivedHistograms", false);
  configuration.addParameter( "createCombinedHistograms", false);
  configuration.addParameter( "loadHistograms",  false);
  configuration.addParameter( "saveHistograms",  false);
  configuration.addParameter( "resetHistograms", false);
  configuration.addParameter( "clearHistograms", false);
  configuration.addParameter( "scaleHistograms", false);
  configuration.addParameter( "forceHistogramsRewrite", false);
  configuration.addParameter( "doSubsampleAnalysis", false);
  configuration.addParameter( "doPartialSave",       false);
  configuration.addParameter( "histoInputPath",      TString("./")   );
  configuration.addParameter( "histoInputFileName",  TString("") );
  configuration.addParameter( "histoOutputPath",     TString("./")  );
  configuration.addParameter( "histoOutputFileName", TString("./")  );
  configuration.addParameter( "histoBaseName",       TString("BaseNameUndefined"));
  
  configuration.addParameter( "dataInputUsed",       false);
  configuration.addParameter( "dataInputPath",       TString("./"));
  configuration.addParameter( "dataInputFileName",   TString(""));
  configuration.addParameter( "dataInputTreeName",   TString("tree") );
  configuration.addParameter( "dataInputFileMinIndex", 0);
  configuration.addParameter( "dataInputFileMaxIndex", 1);
  
  configuration.addParameter( "dataOutputUsed",     false);
  configuration.addParameter( "dataOutputPath",     TString("./"));
  configuration.addParameter( "dataOutputFileName", TString(""));
  configuration.addParameter( "dataOutputTreeName", TString("tree"));
  
  configuration.addParameter( "dataConversionToWac", true);
  
  configuration.addParameter( "fileFromParent", false);
  configuration.addParameter( "histogramInputFileName",TString("AlternateIn"));
  configuration.addParameter( "histogramOuputFileName",TString("AlternateOut"));
  if (reportEnd("Task",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}

void Task::setConfiguration(const Configuration & _configuration)
{
  if (reportStart("Task",getName(),"setConfiguration(const Configuration & config)"))
    { }
  configuration.setParameters(_configuration);
  if (reportEnd("Task",getName(),"setConfiguration(const Configuration & config)"))
    {
    configuration.printConfiguration(cout);
    }
}

void Task::initialize()
{
  if (reportStart("Task",getName(),"initialize()"))
    { }
  if (isTaskOk() && configuration.getValueBool( "useParticles") )
    {
    particleFactory  = Particle::getFactory();
    if (!particleFactory)
      {
      if (reportFatal("Task",getName(),"initialize()"))
        {
        cout << " Particle factory required but not available. Must abort." << endl;
        }
      postTaskFatal();
      return;
      }
    masterCollection = ParticleTypeCollection::getMasterParticleCollection();
    if (!masterCollection)
      {
      if (reportFatal("Task",getName(),"initialize()"))
        {
        cout << " Particle master collection required but not available. Must abort." << endl;
        }
      postTaskFatal();
      return;
      }
    }
  if (!isTaskOk())
    {
    if (reportError("Task",getName(),"initialize()"))
      {
      cout << "Error posted - Abort immediately." << endl;
      }
    exit(1);
    }
  if (configuration.getValueBool( "useEventStream0") )  addEventStream(Event::getEventStream(0));
  if (configuration.getValueBool( "useEventStream1") )  addEventStream(Event::getEventStream(1));
  if (configuration.getValueBool( "useEventStream2") )  addEventStream(Event::getEventStream(2));
  if (configuration.getValueBool( "useEventStream3") )  addEventStream(Event::getEventStream(3));
  if (reportInfo("Task",getName(),"initialize()"))
    {
    cout << " #event added streams: "  << getNEventStreams() << endl;
    }
  if (configuration.getValueBool( "createHistograms") )
    {
    if (reportDebug("Task",getName(),"initialize()"))
      cout << "Create EventCountHistograms" << endl;
    unsigned int nEventFilters = eventFilters.size();
    if (nEventFilters>0)  nFilteredEventsAccepted.assign(nEventFilters,0.0);
    eventCountHistos = new EventCountHistos(getName(),configuration,getReportLevel());
    eventCountHistos->createHistograms();
    if (reportDebug("Task",getName(),"initialize()")) cout << "Done with EventCountHistograms" << endl;
    createHistograms();
    }
  nEventProcessed = 0;
  nEventAccepted  = 0;
  if (configuration.getValueBool( "loadHistograms") )
    {
    loadHistograms();
    }
  
  if (reportEnd("Task",getName(),"initialize()"))
    { }
}


void Task::finalize()
{
  if (reportStart("Task",getName(),"finalize()"))
    { }
  if (nEventAccepted<1 || nEventProcessed<1)
    {
    if (reportWarning("Task",getName(),"finalize()")) printEventCount();
    }
  if (reportInfo("Task",getName(),"finalize()")) printEventCount();
  if (isTaskOk() )
    {
    if (eventCountHistos) eventCountHistos->fill(nFilteredEventsAccepted);
    }
  if (isTaskOk() && configuration.getValueBool( "scaleHistograms") )  scaleHistograms();
  if (isTaskOk() && configuration.getValueBool( "saveHistograms")  )  saveHistograms();
  if (reportEnd("Task",getName(),"finalize()"))
    { }
}

void Task::printEventCount() const
{
  cout << endl
  << "    nEventProcessed:" << nEventProcessed << endl
  << "     nEventAccepted:" << nEventAccepted << endl;
  for (unsigned int k=0; k<nFilteredEventsAccepted.size(); k++)
    {
    cout << "      Event filter:" << k << "  nFiltered Events Accepted: " << nFilteredEventsAccepted[k] << endl;
    }
}

void Task::savePartial()
{
  if (reportStart("Task",getName(),"savePartial()"))
    { }
  if (nEventAccepted<1 || nEventProcessed<1)
    {
    if (reportWarning("Task",getName(),"savePartial()")) printEventCount();
    }
  if (reportInfo("Task",getName(),"savePartial()")) printEventCount();
  if (isTaskOk() )
    {
    if (eventCountHistos) eventCountHistos->fill(nFilteredEventsAccepted);
    }
  if (isTaskOk() && configuration.getValueBool( "scaleHistograms")  ) scaleHistograms();
  if (isTaskOk() && configuration.getValueBool( "saveHistograms")   ) saveHistograms();
  if (isTaskOk() && configuration.getValueBool( "createHistograms") ) resetHistograms();
  if (reportEnd("Task",getName(),"savePartial()"))
    { }
}

void Task::subsampleAnalysis()
{
  if (reportStart("Task",getName(),"subsampleAnalysis()"))
    { }
  bool doSubsampleAnalysis = configuration.getValueBool(   "doSubsampleAnalysis");
  TString inputPathName    = configuration.getValueString( "histoOutputPath"    ); // where files were saved
  TString inputFileName    = configuration.getValueString( "histoOutputFileName"); // base name of files in a job
  TString outputPathName   = inputPathName;     // where files were saved
  TString outputFileName   = inputFileName;
  int numberOfPartialSaves = getSubSampleIndex();                  // number of partial saves

  
  if (!doSubsampleAnalysis)
    {
    if (reportInfo("Task",getName(),"subsampleAnalysis(...)"))
      {
      cout << "No subsample analysis requested for task:" << getName() << endl;
      }
    return;
    }
  else
    {
    if (reportInfo("Task",getName(),"subsampleAnalysis(...)"))
      {
      cout << "Subsample analysis requested for task:" << getName() << endl;
      cout << "                        inputPathName:" << inputPathName << endl;
      cout << "                        inputFileName:" << inputFileName << endl;
      cout << "                 numberOfPartialSaves:" << numberOfPartialSaves << endl;
      cout << "                       outputPathName:" << outputPathName << endl;
      cout << "                       outputFileName:" << outputFileName << endl;
      }
    }
  Configuration statConfig("subSampleConfig");
  statConfig.addParameter( "forceHistogramsRewrite", true);
  statConfig.addParameter( "nInputFile",          numberOfPartialSaves-1);
  statConfig.addParameter( "histoInputPath",      inputPathName         );
  statConfig.addParameter( "histoOutputPath",     outputPathName        );
  statConfig.addParameter( "histoOutputFileName", outputFileName+"_Sum" );
  for (int k=0; k<=numberOfPartialSaves; k++  )
    {
    TString key("InputFile"); key += k;
    TString name = inputFileName;
    name += "_";
    name += k;
    cout << "   k: " << k << "    " << name << endl;
    statConfig.addParameter(key,name);
    }
  SubSampleStatCalculator * subSampleStatCalculator = new SubSampleStatCalculator("DefaultSubSampleStatCalculator",statConfig,getReportLevel());
  subSampleStatCalculator->execute();
  delete subSampleStatCalculator;
  if (reportEnd("Task",getName(),"subsampleAnalysis(...)"))
    { }
}

void Task::reset()
{
  if (reportStart("Task",getName(),"reset(...)"))
    { }
  nEventProcessed = 0;
  nEventAccepted  = 0;
  for (unsigned int k=0; k<nFilteredEventsAccepted.size(); k++) nFilteredEventsAccepted[k] = 0;
  if (isTaskOk())   resetHistograms();
  if (reportEnd("Task",getName(),"reset(...)"))
    { }
}

void Task::clear()
{
  if (reportStart("Task",getName(),"clear()"))
    { }
  
  // we keep the filters as guidelines for what is done but everything else is wiped out for
  // a clean start; i.e., all histograms...
  nEventProcessed = 0;
  nEventAccepted  = 0;
  clearHistograms();
  if (reportEnd("Task",getName(),"clear()"))
    { }
}


void Task::printConfiguration(ostream & output)
{
  output << "Task Name : " << taskName <<  endl;
  configuration.printConfiguration(output);
}

void Task::setRandomGenerator(TRandom * randomGenerator)
{
  if (!randomGenerator)
    {
    taskRandomGenerator = randomGenerator;
    if (reportDebug("Task",getName(),"setRandomGenerator(TRandom * randomGenerator)"))
      {
      cout << "Default random generator was changed." << endl;
      }
    }
  else
    {
    postTaskWarning();
    if (reportError("Task",getName(),"setConfiguration(...)"))
      {
      cout << "Null pointer. Random generator will not be set." << endl;
      }
    }
}

void Task::loadHistograms()
{
  if (reportStart("Task",getName(),"loadHistograms()"))
    { }
  TString inputFileName;
  
  if (configuration.getValueBool("fileFromParent") )
    {
    // this is used by the FileIterator to set the file name to be read
    inputFileName = configuration.getValueString( "histogramInputFileName");
    }
  else
    {
    // this is used normally to identify the file name to be used to save or load from.
    inputFileName = configuration.getValueString( "histoInputPath");
    inputFileName += configuration.getValueString( "histoInputFileName");
    }
  TFile * inputFile = openRootFile("",inputFileName,"READ");
  if (!inputFile)
    {
    if (reportError("Task",getName(),"loadHistograms()")) cout << "Could not open input file:"  << inputFileName << endl;
    postTaskError();
    return;
    }
  eventCountHistos = new EventCountHistos(getName(),configuration,getReportLevel());
  eventCountHistos->loadHistograms(inputFile);
  loadNEventProcessed(inputFile);
  loadNEventAccepted(inputFile);
  loadHistograms(inputFile);
  if (reportEnd("Task",getName(),"loadHistograms()"))
    { }
}


void Task::resetHistograms()
{
  if (reportStart("Task",getName(),"resetHistograms()"))
    { }
  if (eventCountHistos) eventCountHistos->reset();
  for (unsigned int iHisto=0; iHisto<inputHistograms.size();        iHisto++) inputHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<histograms.size();             iHisto++) histograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<baseSingleHistograms.size();   iHisto++) baseSingleHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<basePairHistograms.size();     iHisto++) basePairHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<derivedSingleHistograms.size();iHisto++) derivedSingleHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<derivedPairHistograms.size();  iHisto++) derivedPairHistograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<combinedHistograms.size();     iHisto++) combinedHistograms[iHisto]->reset();
  if (reportEnd("Task",getName(),"resetHistograms()"))
    { }
}

void Task::clearHistograms()
{
  if (reportStart("Task",getName(),"clearHistograms()"))
    { }
  if (eventCountHistos) delete eventCountHistos;
  for (unsigned int iHisto=0; iHisto<inputHistograms.size();        iHisto++) delete inputHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<histograms.size();             iHisto++) delete histograms[iHisto];
  for (unsigned int iHisto=0; iHisto<baseSingleHistograms.size();   iHisto++) delete baseSingleHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<basePairHistograms.size();     iHisto++) delete basePairHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<derivedSingleHistograms.size();iHisto++) delete derivedSingleHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<derivedPairHistograms.size();  iHisto++) delete derivedPairHistograms[iHisto];
  for (unsigned int iHisto=0; iHisto<combinedHistograms.size();     iHisto++) delete combinedHistograms[iHisto];
  inputHistograms.clear();
  histograms.clear();
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  derivedSingleHistograms.clear();
  derivedPairHistograms.clear();
  combinedHistograms.clear();
  if (reportEnd("Task",getName(),"clearHistograms()"))
    { }
}

void Task::scaleHistograms()
{
  if (reportStart("Task",getName(),"scaleHistograms()"))
    { }
  double scalingFactor;
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  int index;
  if (reportInfo("Task",getName(),"scaleHistograms()"))
    {
    cout << endl;
    cout << "              Accepted number of events: " <<  nEventAccepted << endl;
    cout << "                          nEventFilters: " <<  nEventFilters << endl;
    cout << "                       nParticleFilters: " <<  nParticleFilters << endl;
    cout << "            baseSingleHistograms.size(): " <<  baseSingleHistograms.size() << endl;
    cout << "              basePairHistograms.size(): " <<  basePairHistograms.size() << endl;
    cout << "         derivedSingleHistograms.size(): " <<  derivedSingleHistograms.size() << endl;
    cout << "           derivedPairHistograms.size(): " <<  derivedPairHistograms.size() << endl;
    cout << "              combinedHistograms.size(): " <<  combinedHistograms.size() << endl;
    cout << "--------------   Accumulated statistics: ----------------------------------" << endl;
    printEventCount();
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (nFilteredEventsAccepted[iEventFilter]>1)
      {
      scalingFactor = 1.0/double(nFilteredEventsAccepted[iEventFilter]);
      if (reportInfo("Task",getName(),"scaleHistograms()"))
        {
        cout << endl;
        cout << "                                    iEventFilter: " <<  iEventFilter<< endl;
        cout << "           nFilteredEventsAccepted[iEventFilter]: " <<  nFilteredEventsAccepted[iEventFilter]<< endl;
        cout << "                                   scalingFactor: " <<  scalingFactor << endl;
        }
      if (histograms.size()>0)
        {
          if (reportInfo("Task",getName(),"scaleHistograms()")) cout <<  "Scaling group named " <<  histograms[iEventFilter]->getName() << endl;
          histograms[iEventFilter]->scale(scalingFactor);
        }
      if (baseSingleHistograms.size()>0)
        {
        for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
          {
          index = iEventFilter*nParticleFilters + iParticleFilter1;
          if (reportInfo("Task",getName(),"scaleHistograms()"))cout <<  "Scaling group index:" << index << " named " <<  baseSingleHistograms[index]->getName() << endl;
          baseSingleHistograms[index]->scale(scalingFactor);
          }
        }
      if (basePairHistograms.size()>0)
        {
        for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
          {
          for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
            {
            index = iEventFilter*nParticleFilters*nParticleFilters+iParticleFilter1*nParticleFilters+iParticleFilter2;
            if (reportInfo("Task",getName(),"scaleHistograms()"))cout <<  "Scaling group index:" << index << " named " <<  baseSingleHistograms[index]->getName() << endl;
            basePairHistograms[index]->scale(scalingFactor);
            }
          }
        }
      }
    else
      {
      if (reportWarning("Task",getName(),"scaleHistograms()"))
        {
        cout << endl;
        cout << "                                    iEventFilter: " <<  iEventFilter<< endl;
        cout << "           nFilteredEventsAccepted[iEventFilter]: " <<  nFilteredEventsAccepted[iEventFilter]<< endl;
        cout << "                            no scaling performed: " <<  endl;
        }
      }
    }
  if (reportEnd("Task",getName(),"scaleHistograms()"))
    { }
}


void Task::saveHistogramsAsText()
{
  if (reportNoOps("Task",getName(),"saveHistogramsAsText()"))
    { }
}


void Task::saveHistograms(TFile * outputFile)
{
  TString fct = "saveHistograms(TFile * outputFile)";
  if (reportStart("Task",getName(),fct))
    {  }
  outputFile->cd();
  saveNEventProcessed(outputFile);
  saveNEventAccepted(outputFile);
  if (eventCountHistos) eventCountHistos->saveHistograms(outputFile);
  else
    {
    if (reportWarning("Task",getName(),fct)) cout << "eventCountHistos is not defined and thus not saved"<< endl;
    }
  if (reportInfo("Task",getName(),fct))
    {
    cout << endl;
    cout << "    base single histogram(s):"  << baseSingleHistograms.size() << endl;
    cout << "      base pair histogram(s):"  << basePairHistograms.size() << endl;
    cout << " derived single histogram(s):"  << derivedSingleHistograms.size() << endl;
    cout << "   derived pair histogram(s):"  << derivedPairHistograms.size() << endl;
    cout << "       combined histogram(s):"  << combinedHistograms.size() << endl;
    }
  for (unsigned int iHisto=0; iHisto<histograms.size(); iHisto++)
    {
    if (reportDebug("Task",getName(),fct)) cout << "Saving global histogram(s) group:" << iHisto << endl;
    histograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<baseSingleHistograms.size(); iHisto++)
    {
    if (reportDebug("Task",getName(),fct)) cout << "Saving single histogram(s) group:" << iHisto << endl;
    baseSingleHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<basePairHistograms.size(); iHisto++)
    {
    if (reportDebug("Task",getName(),fct)) cout << "Saving pair histogram(s) group:" << iHisto << endl;
    basePairHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<derivedSingleHistograms.size(); iHisto++)
    {
    if (reportDebug("Task",getName(),fct)) cout << "Saving derived single histogram(s) group:" << iHisto  << endl;
    derivedSingleHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<derivedPairHistograms.size(); iHisto++)
    {
    if (reportDebug("Task",getName(),fct)) cout << "Saving derived pair histogram(s) group:" << iHisto  << endl;
    derivedPairHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<combinedHistograms.size(); iHisto++)
    {
    if (reportDebug("Task",getName(),fct)) cout << "Saving combined histogram(s) group:" <<  iHisto << endl;
    combinedHistograms[iHisto]->saveHistograms(outputFile);
    }
}

void Task::saveHistograms()
{
  TString fct = "saveHistograms()";
  if (reportStart("Task",getName(),fct))
    {  }
  //TString outputPath;
  TString outputFileName;
  TFile * outputFile = nullptr;
  
  if (configuration.getValueBool("fileFromParent") )
    {
    // this is used by the FileIterator to set the file name to be read
    outputFileName = configuration.getValueString("histogramOuputFileName");
    }
  else
    {
    // this is used normally to identify the file name to be used to save or load from.
    outputFileName = configuration.getValueString( "histoOutputPath");
    outputFileName += configuration.getValueString( "histoOutputFileName");
    }
  
  if (configuration.getValueBool( "doSubsampleAnalysis") || configuration.getValueBool( "doPartialSave") )
    {
    outputFileName += "_";
    outputFileName += subSampleIndex++;
    }
  if (configuration.getValueBool( "forceHistogramsRewrite") || configuration.getValueBool("fileFromParent"))
    {
    if (reportInfo("Task",getName(),fct)) cout << "Opening output (RECREATE) file  " << outputFileName << endl;
    outputFile = openRootFile("",outputFileName,"RECREATE");
    if (!outputFile)
      {
      if (reportError("Task",getName(),"saveHistograms()")) cout << "Could not open (RECREATE) file:"  << outputFileName << endl;
      postTaskError();
      return;
      }
    }
  else
    {
    if (reportInfo("Task",getName(),fct)) cout << "Opening  output (NEW) file  " << outputFileName << endl;
    outputFile = openRootFile("",outputFileName,"NEW");
    if (!outputFile)
      {
      if (reportError("Task",getName(),fct)) cout << "Could not open (NEW) file  " << outputFileName << endl;
      postTaskWarning();
      return;
      }
    }
  saveHistograms(outputFile);
  outputFile->Close();
  if (reportEnd("Task",getName(),fct))
    { }
}

void Task::saveNEventProcessed(TFile * outputFile)
{
  outputFile->cd();
  TParameter<Long64_t>("EventProcessed",nEventProcessed,'+').Write();
}

void Task::saveNEventAccepted(TFile * outputFile)
{
  outputFile->cd();
  TParameter<Long64_t>("EventAccepted",nEventAccepted,'+').Write();
}

void Task::writeParameter(TFile * outputFile, const TString parameterName, long value)
{
  outputFile->cd();
  TParameter<Long64_t>(parameterName,value,'+').Write();
}


void Task::saveNFilteredEventsAccepted(TFile * outputFile, vector<unsigned int> & nFilteredEventsAccepted)
{
  outputFile->cd();
  for (unsigned int k=0; k<nFilteredEventsAccepted.size(); k++)
    {
    TString name = "nFilteredEventsAccepted_"; name += k;
    TParameter<Long64_t>(name,nFilteredEventsAccepted[k],'+').Write();
    }
}


void Task::loadNEventProcessed(TFile * inputFile)
{
  nEventProcessed = readParameter(inputFile,"EventProcessed");
}

void Task::loadNEventAccepted(TFile * inputFile)
{
  nEventAccepted = readParameter(inputFile,"EventAccepted");
}

void Task::loadNFilteredEventsAccepted(TFile * inputFile, vector<unsigned int> & nFilteredEventsAccepted)
{
  for (unsigned int k=0; k<nFilteredEventsAccepted.size(); k++)
    {
    TString name = "nFilteredEventsAccepted_"; name += k;
    TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile->Get(name);
    nFilteredEventsAccepted[k] = par->GetVal();
    delete par;
    }
}

long Task::readParameter(TFile * inputFile, const TString & parameterName)
{
  if (reportInfo ("Task",getName(),"readParameter(...)")) cout << "Reading parameter: " << parameterName << endl;
  TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile->Get(parameterName);
  if (!par)
    {
    if (reportError("Task",getName(),"readParameter()")) cout << "Parameter not found." << endl;
    postTaskError();
    return 1.0;
    }
  double value = par->GetVal();
  delete par;
  if (reportDebug ("Task",getName(),"readParameter(...)")) cout << "Parameter value : " << value << endl;
  return value;
}

TFile *  Task::openRootFile(const TString & inputPath, const TString & fileName, const TString & ioOption)
{
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
  
  if (reportDebug ("Task",getName(),"openRootFile(...)"))
    cout << "Opening file: " << inputFileName << " with option: " << ioOption << endl;
  TFile * inputFile = new TFile(inputFileName,ioOption);
  if (!inputFile)
    {
    if (reportError("Task",getName(),"openRootFile()")) cout << "file: " << inputFileName << " not found." << endl;
    postTaskError();
    return nullptr;
    }
  else
    {
    if (!inputFile->IsOpen())
      {
      if (reportError("Task",getName(),"openRootFile()")) cout << "file: " << inputFileName << " not found." << endl;
      postTaskError();
      return nullptr;
      }
    else
      {
      if (reportDebug ("Task",getName(),"openRootFile()")) cout << "file opened successfully." << endl;
      }
    }
  return inputFile;
}


void Task::initializeTasks()
{
  if (reportStart("Task",getName(),"initializeTasks()"))
    { }
  if (!isTaskOk()) return;
  initialize();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    {
    if (reportDebug("Task",getName(),"initializeTasks()"))
      {
      cout << "Initializing " << nSubtasks << " tasks." << endl;
      }
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      if (!isTaskOk()) break;
      if (reportDebug("Task",getName(),"initializeTasks()"))
        {
        cout << "Initializing task:" << subtasks[iTask]->getName() << endl;
        }
      subtasks[iTask]->initializeTasks();
      }
    }
  else
    {
    if (reportDebug("Task",getName(),"initializeTasks()"))
      {
      cout << "No subtask to initialize. " <<   endl;
      }
    }
  if (reportEnd("Task",getName(),"initializeTasks()"))
    { }
}

void Task::executeTasks()
{
  if (!isTaskOk()) return;
  execute();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)  { if (isTaskOk()) subtasks[iTask]->executeTasks(); }
}

void Task::finalizeTasks()
{
  if (reportStart("Task",getName(),"finalizeTasks()"))
    { }
  if (!isTaskOk()) return;
  finalize();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    {
    if (reportDebug("Task",getName(),"finalizeTasks()"))
      {
      cout << "Finalizing " << nSubtasks << " tasks." << endl;
      }
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      if (!isTaskOk()) break;
      if (reportDebug("Task",getName(),"finalizeTasks()"))
        {
        cout << "Finalizing task:" << subtasks[iTask]->getName() << endl;
        }
      subtasks[iTask]->finalizeTasks();
      }
    }
  else
    {
    if (reportDebug("Task",getName(),"finalizeTasks()"))
      {
      cout << "No subtasks to finalize."  << endl;
      }
    }
  if (reportEnd("Task",getName(),"finalizeTasks()"))
    { }
}

void Task::resetTasks()
{
  if (reportStart("Task",getName(),"resetTasks()"))
    { }
  if (!isTaskOk()) return;
  reset();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    {
    if (reportDebug("Task",getName(),"resetTasks()"))
      {
      cout << "Resetting " << nSubtasks << " tasks." << endl;
      }
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      if (!isTaskOk()) break;
      if (reportDebug("Task",getName(),"resetTasks()"))
        {
        cout << "Resetting task:" << subtasks[iTask]->getName() << endl;
        }
      subtasks[iTask]->resetTasks();
      }
    }
  else
    {
    if (reportDebug("Task",getName(),"resetTasks()"))
      {
      cout << "No subtasks to reset."  << endl;
      }
    }
  if (reportEnd("Task",getName(),"resetTasks()"))
    { }
}

void Task::clearTasks()
{
  if (reportStart("Task",getName(),"clearTasks()"))
    { }
  if (!isTaskOk()) return;
  clear();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    {
    if (reportDebug("Task",getName(),"clearTasks()"))
      {
      cout << "Clearing " << nSubtasks << " tasks." << endl;
      }
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      if (!isTaskOk()) break;
      if (reportDebug("Task",getName(),"clearTasks()"))
        {
        cout << "Clearing task:" << subtasks[iTask]->getName() << endl;
        }
      subtasks[iTask]->clearTasks();
      }
    }
  else
    {
    if (reportDebug("Task",getName(),"clearTasks()"))
      {
      cout << "No subtasks to clear."  << endl;
      }
    }
  if (reportEnd("Task",getName(),"clearTasks()"))
    { }
}


void Task::savePartialTasks()
{
  if (reportStart("Task",getName(),"savePartialTasks()"))
    { }
  if (!isTaskOk()) return;
  savePartial();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    {
    if (reportDebug("Task",getName(),"savePartialTasks()"))
      {
      cout << "Calling partial save for  " << nSubtasks << " tasks." << endl;
      }
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      if (!isTaskOk()) break;
      if (reportDebug("Task",getName(),"savePartialTasks()"))
        {
        cout << "Calling partial  for  :" << subtasks[iTask]->getName() << endl;
        }
      subtasks[iTask]->savePartialTasks();
      }
    }
  else
    {
    if (reportDebug("Task",getName(),"savePartialTasks()"))
      {
      cout << "No subtasks to partial save..."  << endl;
      }
    }
  if (reportEnd("Task",getName(),"savePartialTasks()"))
    { }
}

void Task::subsampleAnalysisTasks()
{
  if (reportStart("Task",getName(),"subsampleAnalysisTasks()"))
    { }
  if (!isTaskOk()) return;
  subsampleAnalysis();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    {
    if (reportDebug("Task",getName(),"subsampleAnalysisTasks()"))
      {
      cout << "Calling subsample analysis for  " << nSubtasks << " tasks." << endl;
      }
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      if (!isTaskOk()) break;
      if (reportDebug("Task",getName(),"subsampleAnalysisTasks()"))
        {
        cout << "Calling subsample analysis for  :" << subtasks[iTask]->getName() << endl;
        }
      subtasks[iTask]->subsampleAnalysis();
      }
    }
  else
    {
    if (reportDebug("Task",getName(),"subsampleAnalysisTasks()"))
      {
      cout << "No subtasks to carry out subsample analysis for..."  << endl;
      }
    }
  if (reportEnd("Task",getName(),"subsampleAnalysisTasks()"))
    { }
}


void Task::printConfigurationTasks(ostream & output)
{
  if (!isTaskOk()) return;
  printConfiguration(output);
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    {
    if (reportDebug("Task",getName(),"printConfigurationTasks()"))
      {
      cout << "Printing configurations for  " << nSubtasks << " tasks." << endl;
      }
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)
      {
      if (!isTaskOk()) break;
      if (reportDebug("Task",getName(),"printConfigurationTasks()"))
        {
        cout << "Calling subsample analysis for  :" << subtasks[iTask]->getName() << endl;
        }
      subtasks[iTask]->printConfigurationTasks(output);
      }
    }
  else
    {
    if (reportDebug("Task",getName(),"printConfigurationTasks()"))
      {
      cout << "No subtasks to print configuration for..."  << endl;
      }
    }
}


Task *  Task::addSubtask(Task * task)
{
  if (!task)
    {
    if (reportFatal("Task",getName(),"addSubtask(Task * task)")) cout << "Given task pointer is null. Abort." << endl;
    postTaskFatal();
    return task;
    }
  subtasks.push_back( task );
  if (reportDebug("Task",getName(),"addSubtask(Task * task)")) cout << "Added task " << task->getName() << " to task " << getName() << endl;
  return task;
}

void Task::addHistogramsToExtList(TList *list)
{
  TString fctName = "addHistogramsToExtList(TList *list)";
  if (reportStart("Task",getName(),fctName))
    ;
  if (!list)
    {
    if (reportError("Task",getName(),fctName))
      {
      cout << "Given list pointer is null." << endl;
      }
    postTaskWarning();
    return;
    }
  list->Add(new TParameter<Long64_t>("nEventProcessed",getNEventProcessed(),'+'));
  
  for (unsigned int k=0; eventFilters.size(); k++)
    {
    TString name = "nFilteredEventsAccepted"; name += k;
    list->Add(new TParameter<Long64_t>(name,nFilteredEventsAccepted[k],'+'));
    }
  for (unsigned int k=1; k<baseSingleHistograms.size(); k++)
    {
    baseSingleHistograms[k]->addHistogramsToExtList(list);
    }
  for (unsigned int k=1; k<basePairHistograms.size(); k++)
    {
    basePairHistograms[k]->addHistogramsToExtList(list);
    }
  for (unsigned int k=1; k<derivedPairHistograms.size(); k++)
    {
    derivedPairHistograms[k]->addHistogramsToExtList(list);
    }
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
  cout << "Task::addFileNames(const TString pathName,vector<TString> includePatterns,vector<TString> excludePatterns)" << endl;
  vector<TString> outputList;
  vector<TString> fileList = listFilesInDir(pathName,".root");
  unsigned int nNames = fileList.size();
  cout << "      nNames:" << nNames << endl;
  cout << "   nIncludes:" << includePatterns.size() << endl;
  cout << "   nExcludes:" << excludePatterns.size() << endl;
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
      TString check = pathName+name;
      cout << " CHECK:::::: " << check << endl;
      outputList.push_back(pathName+name);
      }
    }
  return outputList;
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
