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
using CAP::RunSubsample;

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


RunSubsample::RunSubsample(const String & _name,
                           const Configuration & _configuration)
:
Task(_name, _configuration)
{
  //  String includeBasePath = getenv("CAP_SRC");
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
  addParameter("HistogramsImportPath",  TString("./"));
  addParameter("HistogramsExportPath", TString("./"));
  addParameter("Bunched",             YES);
  addParameter("nBunches",            2);
  addParameter("BunchLabel",          TString("BUNCH"));
  addParameter("SubPathLabel",        TString("Output"));
  addParameter("MaximumDepth",        1);
}

void RunSubsample::addBaseSubSampleTask(const String & basePath,
                                        const String & bunchLabel,
                                        int   nBunches,
                                        const String & subPath,
                                        int   maximumDepth,
                                        const String & taskType)
{
  if (nBunches>0)
    {
    for (int k=1;k<=nBunches; k++)
      {
      String inputPathName  = TString(Form("%s/%s%02d/%s",basePath.Data(),bunchLabel.Data(),k,subPath.Data()));
      String outputPathName = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      Configuration & subConfig = * new Configuration();
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsImportPath"),inputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsExportPath"),outputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("Gen"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Reco"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("Derived"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern2"),TString("BalFct"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern3"),TString("Sum"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("Sum"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
      subConfig.printConfiguration(cout);
      addSubTask( new SubSampleStatCalculator(taskType,subConfig));
      }
    }
  else // not bunched
    {
    String inputPathName  = TString(Form("%s/",basePath.Data()));
    String outputPathName = inputPathName;
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsImportPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsExportPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern3"),TString("Sum"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("Sum"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(taskType,subConfig));
    }
}

void RunSubsample::addDerivedSubSampleTask(const String & basePath,
                                           const String & bunchLabel,
                                           int   nBunches,
                                           const String & subPath __attribute__((unused)),
                                           int   maximumDepth,
                                           const String & taskType)
{
  if (nBunches>0)
    {
    for (int k=1;k<=nBunches; k++)
      {
      String inputPathName  = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      String outputPathName = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      Configuration & subConfig = * new Configuration();
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsImportPath"),inputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsExportPath"),outputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("Gen"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern1"),TString("Derived"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Reco"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("BalFct"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("DerivedSum"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
      subConfig.printConfiguration(cout);
      addSubTask( new SubSampleStatCalculator(taskType,subConfig));
      }
    }
  else // not bunched
    {
    String inputPathName  = TString(Form("%s/",basePath.Data()));
    String outputPathName = inputPathName;
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsImportPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsExportPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("DerivedSum"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(taskType,subConfig));
    }
}

void RunSubsample::addBalFctSubSampleTask(const String & basePath,
                                          const String & bunchLabel,
                                          int   nBunches,
                                          const String & subPath __attribute__((unused)),
                                          int   maximumDepth,
                                          const String & taskType)
{
  if (nBunches>0)
    {
    for (int k=1;k<=nBunches; k++)
      {
      String inputPathName  = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      String outputPathName = TString(Form("%s/%s%02d/",basePath.Data(),bunchLabel.Data(),k));
      Configuration & subConfig = * new Configuration();
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsImportPath"),inputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsExportPath"),outputPathName);
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("RunPartPairAnalysisReco"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern1"),TString("BalFct"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Derived"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("Reco"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("BalFctSum"));
      subConfig.addParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
      subConfig.printConfiguration(cout);
      addSubTask( new SubSampleStatCalculator(taskType,subConfig));
      }
    }
  else // not bunched
    {
    String inputPathName  = TString(Form("%s/",basePath.Data()));
    String outputPathName = inputPathName;
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsImportPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":HistogramsExportPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern0"),TString("RunPartPairAnalysisReco"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":IncludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern0"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":ExcludedPattern1"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":AppendedString"),TString("BalFctSum"));
    subConfig.addParameter(TString("Run:")+taskType+TString(":MaximumDepth"),maximumDepth);
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(taskType,subConfig));
    }
}

void RunSubsample::configure()
{
  if (reportEnd(__FUNCTION__))
    ;
  Task::configure();
  String GlobalLabel      = getValueString("GlobalLabel");
  String SpherocityLabel  = getValueString("SpherocityLabel");
  String PartLabel        = getValueString("PartLabel");
  String PairLabel        = getValueString("PairLabel");
  String NuDynLabel       = getValueString("NuDynLabel");
  String GenLabel         = getValueString("GenLabel");
  String RecoLabel        = getValueString("RecoLabel");

  bool    RunGlobalAnalysisGen     = getValueBool("RunGlobalAnalysisGen");
  bool    RunSpherocityAnalysisGen = getValueBool("RunSpherocityAnalysisGen");
  bool    RunPartSingleAnalysisGen       = getValueBool("RunPartSingleAnalysisGen");
  bool    RunPartPairAnalysisReco       = getValueBool("RunPartPairAnalysisReco");
  bool    RunNuDynAnalysisGen      = getValueBool("RunNuDynAnalysisGen");

  bool    RunSubsampleBase     = getValueBool(  "SubsampleBase");
  bool    RunSubsampleDerived  = getValueBool(  "SubsampleDerivedGen");
  bool    RunSubsampleBalFct   = getValueBool(  "SubsampleBalFctGen");
  String inputPathName        = getValueString("HistogramsImportPath");
  String outputPathName       = getValueString("HistogramsExportPath");
  bool    bunched              = getValueBool(  "Bunched");
  int     nBunches             = getValueInt(   "nBunches");
  String bunchLabel           = getValueString("BunchLabel");
  String subPathLabel         = getValueString("SubPathLabel");
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
    cout << "HistogramsImportPath.........:" << inputPathName         << endl;
    cout << "HistogramsExportPath........:" << outputPathName        << endl;
    cout << "Bunched....................:" << bunched              << endl;
    cout << "nBunches...................:" << nBunches             << endl;
    cout << "BunchLabel.................:" << bunchLabel           << endl;
    cout << "SubPathLabel...............:" << subPathLabel         << endl;
    cout << "MaximumDepth...............:" << maximumDepth         << endl;
    cout << "RunGlobalAnalysisGen...............:" << RunGlobalAnalysisGen         << endl;
    cout << "RunSpherocityAnalysisGen...........:" << RunSpherocityAnalysisGen     << endl;
    cout << "RunPartSingleAnalysisGen.................:" << RunPartSingleAnalysisGen           << endl;
    cout << "RunPartPairAnalysisReco.................:" << RunPartPairAnalysisReco           << endl;
    cout << "RunNuDynAnalysisGen................:" << RunNuDynAnalysisGen          << endl;
    }

  if (RunSubsampleBase && RunGlobalAnalysisGen)     addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,GlobalLabel+GenLabel);
  if (RunSubsampleBase && RunSpherocityAnalysisGen) addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,SpherocityLabel+GenLabel);
  if (RunSubsampleBase && RunPartSingleAnalysisGen)       addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PartLabel+GenLabel);
  if (RunSubsampleBase && RunPartPairAnalysisReco)       addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PairLabel+GenLabel);
  if (RunSubsampleBase && RunNuDynAnalysisGen)      addBaseSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,NuDynLabel+GenLabel);

  if (RunSubsampleDerived && RunGlobalAnalysisGen)     addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,GlobalLabel+GenLabel);
  if (RunSubsampleDerived && RunSpherocityAnalysisGen) addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,SpherocityLabel+GenLabel);
  if (RunSubsampleDerived && RunPartSingleAnalysisGen)       addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PartLabel+GenLabel);
  if (RunSubsampleDerived && RunPartPairAnalysisReco)       addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PairLabel+GenLabel);
  if (RunSubsampleDerived && RunNuDynAnalysisGen)      addDerivedSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,NuDynLabel+GenLabel);

  if (RunSubsampleBalFct && RunPartPairAnalysisReco)       addBalFctSubSampleTask(inputPathName,bunchLabel,nBunches,subPathLabel,maximumDepth,PairLabel+GenLabel);

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Configuration Completed; Run Analysis" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
  if (hasSubTasks() && isTaskOk())
    for (unsigned int  iTask=0; iTask<subTasks.size(); iTask++)
      {
      subTasks[iTask]->configure();
      }
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

