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
#include "RunSubsample.hpp"
#include "SubSampleStatCalculator.hpp"

// we consider two scenarios
// (1) one master job with several sub-jobs or files
// (2) n bunches, each with m sub-jobs
// Variables of interest
// Bunched  yes/no
// nBunches number of bunches
// InputPathName  the path where all the files or subpaths are located
// PartialPath  partials
// nPartials  number of partials or sub-jobs
//
// Case 1
// Bunched         == NO
// InputPathName   == /Disc/.../MainOutput/
//
// Case 2
// Bunched         == YES
// InputPathName   == /Disc/.../MainOutput/
// nBunched        == a number provided by the user...
// PartialPath     == BUNCH##/Output/

ClassImp(RunSubsample);


RunSubsample::RunSubsample(const TString & _name,
                           Configuration & _configuration)
:
Task(_name, _configuration)
{
  //  TString includeBasePath = getenv("CAP_SRC");
  appendClassName("RunSubsample");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}


void RunSubsample::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  bool YES = true;
  bool NO  = false;
  addParameter("GlobalLabel",         TString("G"));
  addParameter("SpherocityLabel",     TString("S"));
  addParameter("PartLabel",           TString("Part"));
  addParameter("PairLabel",           TString("Pair"));
  addParameter("NuDynLabel",          TString("NuDyn"));
  addParameter("GenLabel",            TString("Gen"));
  addParameter("RecoLabel",           TString("Reco"));
  addParameter("Severity",            TString("DEBUG"));
  addParameter("Subsample",           YES);
  addParameter("SubsampleBase",       YES);
  addParameter("SubsampleDerived",    NO);
  addParameter("SubsampleBalFct",     NO);
  addParameter("HistogramInputPath",  TString("./"));
  addParameter("HistogramOutputPath", TString("./"));
  addParameter("Bunched",             YES);
  addParameter("nBunches",            2);
  addParameter("BunchLabel",          TString("BUNCH"));
  addParameter("SubPathLabel",        TString("Output"));
  addParameter("MaximumDepth",        1);
}

void RunSubsample::addBaseSubSampleTask(const TString & basePath,
                                        const TString & bunchLabel,
                                        int   nBunches,
                                        const TString & subPath,
                                        int   maximumDepth,
                                        const TString & taskType)
{
  if (nBunches>0)
    {
    for (int k=1;k<=nBunches; k++)
      {
      TString inputPathName  = TString(Form("%s/%s%02d/%s",basePath.Data(),bunchLabel.Data(),k,subPath.Data()));
      TString outputPathName = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      Configuration & subConfig = * new Configuration();
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramInputPath"),inputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramOutputPath"),outputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("Gen"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Reco"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("Derived"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern2"),TString("BalFct"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern3"),TString("Sum"));
      subConfig.setParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("Sum"));
      subConfig.setParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
      subConfig.printConfiguration(cout);
      addSubTask( new SubSampleStatCalculator(taskType,subConfig));
      }
    }
  else // not bunched
    {
    TString inputPathName  = TString(Form("%s/",basePath.Data()));
    TString outputPathName = inputPathName;
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern3"),TString("Sum"));
    subConfig.setParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("Sum"));
    subConfig.setParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(taskType,subConfig));
    }
}

void RunSubsample::addDerivedSubSampleTask(const TString & basePath,
                                           const TString & bunchLabel,
                                           int   nBunches,
                                           const TString & subPath,
                                           int   maximumDepth,
                                           const TString & taskType)
{
  if (nBunches>0)
    {
    for (int k=1;k<=nBunches; k++)
      {
      TString inputPathName  = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      TString outputPathName = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      Configuration & subConfig = * new Configuration();
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramInputPath"),inputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramOutputPath"),outputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("Gen"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern1"),TString("Derived"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Reco"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("BalFct"));
      subConfig.setParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("DerivedSum"));
      subConfig.setParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
      subConfig.printConfiguration(cout);
      addSubTask( new SubSampleStatCalculator(taskType,subConfig));
      }
    }
  else // not bunched
    {
    TString inputPathName  = TString(Form("%s/",basePath.Data()));
    TString outputPathName = inputPathName;
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.setParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("DerivedSum"));
    subConfig.setParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(taskType,subConfig));
    }
}

void RunSubsample::addBalFctSubSampleTask(const TString & basePath,
                                          const TString & bunchLabel,
                                          int   nBunches,
                                          const TString & subPath,
                                          int   maximumDepth,
                                          const TString & taskType)
{
  if (nBunches>0)
    {
    for (int k=1;k<=nBunches; k++)
      {
      TString inputPathName  = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      TString outputPathName = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      Configuration & subConfig = * new Configuration();
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramInputPath"),inputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramOutputPath"),outputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("PairGen"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern1"),TString("BalFct"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Derived"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("Reco"));
      subConfig.setParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("BalFctSum"));
      subConfig.setParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
      subConfig.printConfiguration(cout);
      addSubTask( new SubSampleStatCalculator(taskType,subConfig));
      }
    }
  else // not bunched
    {
    TString inputPathName  = TString(Form("%s/",basePath.Data()));
    TString outputPathName = inputPathName;
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("PairGen"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("Reco"));
    subConfig.setParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("BalFctSum"));
    subConfig.setParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(taskType,subConfig));
    }
}

void RunSubsample::configure()
{
  if (reportEnd(__FUNCTION__))
    ;
  setDefaultConfiguration();
  setConfiguration(requestedConfiguration);
  configuration.printConfiguration(cout);
  //exit(1);

  // assemble the task from here...

  MessageLogger::Severity selectedLevel = MessageLogger::Debug;
  TString reportLevel                   = getValueBool("Severity");
  if (reportLevel.EqualTo("Debug")) selectedLevel = MessageLogger::Debug;
  if (reportLevel.EqualTo("Info"))  selectedLevel = MessageLogger::Info;

  TString GlobalLabel      = getValueString("GlobalLabel");
  TString SpherocityLabel  = getValueString("SpherocityLabel");
  TString PartLabel        = getValueString("PartLabel");
  TString PairLabel        = getValueString("PairLabel");
  TString NuDynLabel       = getValueString("NuDynLabel");
  TString GenLabel         = getValueString("GenLabel");
  TString RecoLabel        = getValueString("RecoLabel");

  bool    RunGlobalGen     = getValueBool("GlobalGen");
  bool    RunSpherocityGen = getValueBool("SpherocityGen");
  bool    RunPartGen       = getValueBool("PartGen");
  bool    RunPairGen       = getValueBool("PairGen");
  bool    RunNuDynGen      = getValueBool("NuDynGen");

  bool    RunSubsampleBase     = getValueBool(  "SubsampleBase");
  bool    RunSubsampleDerived  = getValueBool(  "SubsampleDerivedGen");
  bool    RunSubsampleBalFct   = getValueBool(  "SubsampleBalFctGen");
  TString inputPathName        = getValueString("HistogramInputPath");
  TString outputPathName       = getValueString("HistogramOutputPath");
  bool    bunched              = getValueBool(  "Bunched");
  int     nBunches             = getValueInt(   "nBunches");
  TString bunchLabel           = getValueString("BunchLabel");
  TString subPathLabel         = getValueString("SubPathLabel");
  int     maximumDepth         = getValueInt(   "MaximumDepth");


  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "GlobalLabel................:" << GlobalLabel      << endl;
    cout << "SpherocityLabel............:" << SpherocityLabel  << endl;
    cout << "PartLabel..................:" << PartLabel        << endl;
    cout << "PairLabel..................:" << PairLabel        << endl;
    cout << "NuDynLabel.................:" << NuDynLabel       << endl;
    cout << "SubsampleBase..............:" << RunSubsampleBase      << endl;
    cout << "SubsampleDerived...........:" << RunSubsampleDerived   << endl;
    cout << "SubsampleBalFct............:" << RunSubsampleBalFct    << endl;
    cout << "HistogramInputPath.........:" << inputPathName         << endl;
    cout << "HistogramOutputPath........:" << outputPathName        << endl;
    cout << "Bunched....................:" << bunched              << endl;
    cout << "nBunches...................:" << nBunches             << endl;
    cout << "BunchLabel.................:" << bunchLabel           << endl;
    cout << "SubPathLabel...............:" << subPathLabel         << endl;
    cout << "MaximumDepth...............:" << maximumDepth         << endl;
    cout << "RunGlobalGen...............:" << RunGlobalGen         << endl;
    cout << "RunSpherocityGen...........:" << RunSpherocityGen     << endl;
    cout << "RunPartGen.................:" << RunPartGen           << endl;
    cout << "RunPairGen.................:" << RunPairGen           << endl;
    cout << "RunNuDynGen................:" << RunNuDynGen          << endl;
    }

  if (RunSubsampleBase && RunGlobalGen)     addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,GlobalLabel+GenLabel);
  if (RunSubsampleBase && RunSpherocityGen) addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,SpherocityLabel+GenLabel);
  if (RunSubsampleBase && RunPartGen)       addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PartLabel+GenLabel);
  if (RunSubsampleBase && RunPairGen)       addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PairLabel+GenLabel);
  if (RunSubsampleBase && RunNuDynGen)      addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,NuDynLabel+GenLabel);

  if (RunSubsampleDerived && RunGlobalGen)     addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,GlobalLabel+GenLabel);
  if (RunSubsampleDerived && RunSpherocityGen) addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,SpherocityLabel+GenLabel);
  if (RunSubsampleDerived && RunPartGen)       addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PartLabel+GenLabel);
  if (RunSubsampleDerived && RunPairGen)       addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PairLabel+GenLabel);
  if (RunSubsampleDerived && RunNuDynGen)      addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,NuDynLabel+GenLabel);

  if (RunSubsampleBalFct && RunPairGen)       addBalFctSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PairLabel+GenLabel);

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Configuration Completed; Run Analysis" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
  if (hasSubTasks() && isTaskOk()) configureSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void RunSubsample::execute()
{
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Subsample Analysis Started" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
  executeSubTasks();
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Subsample Analysis Completed" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
}

