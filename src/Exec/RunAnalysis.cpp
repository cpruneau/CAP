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
#include "RunAnalysis.hpp"
#include "TaskIterator.hpp"
#include "DerivedHistoIterator.hpp"
#include "BalanceFunctionCalculator.hpp"
#include "SubSampleStatCalculator.hpp"
#include "ClosureIterator.hpp"
#include "ParticleTypeTableLoader.hpp"
#include "PythiaEventReader.hpp"
#include "PythiaEventGenerator.hpp"
#include "HerwigEventReader.hpp"
#include "AmptEventReader.hpp"
#include "EposEventReader.hpp"
#include "UrqmdEventReader.hpp"
#include "HijingEventReader.hpp"
#include "ResonanceGenerator.hpp"
#include "MeasurementPerformanceSimulator.hpp"
#include "ParticlePerformanceAnalyzer.hpp"
#include "GlobalAnalyzer.hpp"
#include "TransverseSpherocityAnalyzer.hpp"
#include "ParticleAnalyzer.hpp"
#include "ParticlePairAnalyzer.hpp"
#include "NuDynAnalyzer.hpp"

ClassImp(RunAnalysis);


//int RunAnalysis(int jobIndex, int seed)

RunAnalysis::RunAnalysis(const TString & _name,
                         Configuration & _configuration)
:
Task(_name, _configuration)
{
  //  TString includeBasePath = getenv("CAP_SRC");
  appendClassName("RunAnalysis");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}


void RunAnalysis::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  bool YES = true;
  bool NO  = false;
  addParameter("GlobalLabel",     TString("G"));
  addParameter("SpherocityLabel", TString("S"));
  addParameter("PartLabel",       TString("Part"));
  addParameter("PairLabel",       TString("Pair"));
  addParameter("NuDynLabel",      TString("NuDyn"));
  addParameter("SimAnaLabel",     TString("SimAna"));
  addParameter("RootExt",         TString(".root"));
  addParameter("DerivedLabel",    TString("_Derived"));
  addParameter("SumLabel",        TString("_Sum"));
  addParameter("BalFctLabel",     TString("_BalFct"));
  addParameter("ClosureLabel",    TString("_Closure"));
  addParameter("GenLabel",        TString("_Gen"));
  addParameter("RecoLabel",       TString("_Reco"));
  addParameter("PythiaLabel",     TString("PYTHIA"));
  addParameter("HerwigLabel",     TString("HERWIH"));
  addParameter("HijingLabel",     TString("HIJING"));
  addParameter("AmptLabel",       TString("AMPT"));
  addParameter("EposLabel",       TString("EPOST"));
  addParameter("UrqmdLabel",      TString("URQMD"));
  addParameter("ResoLabel",       TString("RESO"));
  addParameter("PerformLabel",    TString("Perform"));

  //  addParameter("histoModelDataName",      TString("none"));
  //  addParameter("histoAnalyzerName",       TString("none"));
  //  addParameter("histoBaseName",           TString("none"));
  addParameter("LogLevel",                TString("Info"));
  addParameter("EventAnalysis",           YES);
  addParameter("EventAnalysisGen",        YES);
  addParameter("EventAnalysisReco",       NO);
  addParameter("DerivedGen",              NO);
  addParameter("DerivedReco",             NO);
  addParameter("BalFctGen",               NO);
  addParameter("BalFctReco",              NO);
  addParameter("Subsample",               NO);
  addParameter("SubsampleBaseGen",        NO);
  addParameter("SubsampleBaseReco",       NO);
  addParameter("SubsampleDerivedGen",     NO);
  addParameter("SubsampleDerivedReco",    NO);
  addParameter("SubsampleBalFctGen",      NO);
  addParameter("SubsampleBalFctReco",     NO);
  addParameter("PerformanceSim",          NO);
  addParameter("PerformanceAna",          NO);
  addParameter("Closure",                 NO);
  addParameter("ClosureBase",             NO);
  addParameter("ClosureDerived",          NO);
  addParameter("ClosureBalFct",           NO);
  addParameter("PythiaGenerator",         NO);
  addParameter("PythiaReader",            NO);
  addParameter("HerwigGenerator",         NO);
  addParameter("HerwigReader",            NO);
  addParameter("AmptGenerator",           NO);
  addParameter("AmptReader",              NO);
  addParameter("EposGenerator",           NO);
  addParameter("EposReader",              NO);
  addParameter("UrqmdReader",             NO);
  addParameter("HijingReader",            NO);
  addParameter("ResonanceGenerator",      NO);
  addParameter("GlobalGen",               NO);
  addParameter("GlobalReco",              NO);
  addParameter("SpherocityGen",           NO);
  addParameter("SpherocityReco",          NO);
  addParameter("PartGen",                 NO);
  addParameter("PartReco",                NO);
  addParameter("PairGen",                 NO);
  addParameter("PairReco",                NO);
  addParameter("NuDynGen",                NO);
  addParameter("NuDynReco",               NO);
  addParameter("FillEta",                 NO);
  addParameter("FillY",                   YES);
  addParameter("nIterationRequested",     long(100));
  addParameter("nIterationReported",      long(10000));
  addParameter("nIterationPartialSave",   long(10000));
  addParameter("PartialReports",          YES);
  addParameter("PartialSaves",            NO);
  addParameter("ForceHistogramsRewrite",  NO);
  addParameter("ScaleHistograms",         YES);
  addParameter("Energy",                  double(13000.0));
  addParameter("Beam",                    int(2212));
  addParameter("Target",                  int(2212));
  addParameter("SystemLabel",             TString("pp"));
  addParameter("EnergyLabel",             TString("13TeV"));
  addParameter("Pdg:LoadTable",           YES);
  addParameter("Pdg:PathName",            TString(getenv("CAP_DATA")));
  addParameter("Pdg:TableName",           TString("/EOS/pdgPhysicalKaons.dat"));
  addParameter("HistogramInputPath",      TString("Input"));
  addParameter("HistogramOutputPath",     TString("Output"));
  addParameter("UseEventStream0",         true);
  addParameter("UseEventStream1",         false);

  addParameter("ModelEventFilterOption",     TString("All"));
  addParameter("ModelEventFilterNValues",    0);
  addParameter("ModelEventFilterValue0",     0.0);
  addParameter("ModelEventFilterValue1",     0.0);
  addParameter("ModelEventFilterValue2",     0.0);
  addParameter("ModelEventFilterValue3",     0.0);
  addParameter("ModelEventFilterValue4",     0.0);
  addParameter("ModelEventFilterValue5",     0.0);
  addParameter("ModelEventFilterValue6",     0.0);
  addParameter("ModelEventFilterValue7",     0.0);
  addParameter("ModelEventFilterValue8",     0.0);
  addParameter("ModelEventFilterValue9",     0.0);
  addParameter("ModelEventFilterValue10",     0.0);
  addParameter("ModelEventFilterValue11",     0.0);
  addParameter("ModelEventFilterValue12",     0.0);

  addParameter("AnaEventFilterOption",     TString("All"));
  addParameter("AnaEventFilterNValues",    0);
  addParameter("AnaEventFilterValue0",     0.0);
  addParameter("AnaEventFilterValue1",     0.0);
  addParameter("AnaEventFilterValue2",     0.0);
  addParameter("AnaEventFilterValue3",     0.0);
  addParameter("AnaEventFilterValue4",     0.0);
  addParameter("AnaEventFilterValue5",     0.0);
  addParameter("AnaEventFilterValue6",     0.0);
  addParameter("AnaEventFilterValue7",     0.0);
  addParameter("AnaEventFilterValue8",     0.0);
  addParameter("AnaEventFilterValue9",     0.0);
  addParameter("AnaEventFilterValue10",     0.0);
  addParameter("AnaEventFilterValue11",     0.0);
  addParameter("AnaEventFilterValue12",     0.0);

  addParameter("ModelPartFilterOption",     TString("All"));
  addParameter("ModelPartFilterPt",         true);
  addParameter("ModelPartMinPt",            0.2);
  addParameter("ModelPartMaxPt",            2.0);
  addParameter("ModelPartFilterEta",        false);
  addParameter("ModelPartMinEta",           -4.0);
  addParameter("ModelPartMaxEta",            4.0);
  addParameter("ModelPartFilterY",          true);
  addParameter("ModelPartMinY",             -4.0);
  addParameter("ModelPartMaxY",              4.0);

  addParameter("AnaPartFilterOption",       TString("All"));
  addParameter("AnaPartFilterPt",           true);
  addParameter("AnaPartMinPt",              0.2);
  addParameter("AnaPartMaxPt",              2.0);
  addParameter("AnaPartFilterEta",          false);
  addParameter("AnaPartMinEta",             -4.0);
  addParameter("AnaPartMaxEta",             4.0);
  addParameter("AnaPartFilterY",            true);
  addParameter("AnaPartMinY",               -4.0);
  addParameter("AnaPartMaxY",               4.0);
}


void RunAnalysis::configure()
{
  if (reportEnd(__FUNCTION__))
    ;
  setDefaultConfiguration();
  setConfiguration(requestedConfiguration);
  configuration.printConfiguration(cout);
  //exit(1);

  // assemble the task from here...
  TaskIterator              * eventAnalysis         = nullptr;
  DerivedHistoIterator      * derivedGen            = nullptr;
  DerivedHistoIterator      * derivedReco           = nullptr;
  BalanceFunctionCalculator * balFctGen             = nullptr;
  BalanceFunctionCalculator * balFctReco            = nullptr;
  //  ClosureIterator           * closureBasic          = nullptr;
  //  ClosureIterator           * closureDerived        = nullptr;
  //  ClosureIterator           * closureBalFct         = nullptr;
  Task                      * task                  = nullptr;

  MessageLogger::LogLevel selectedLevel = MessageLogger::Debug;
  TString reportLevel                   = getValueBool("LogLevel");
  if (reportLevel.EqualTo("Debug")) selectedLevel = MessageLogger::Debug;
  if (reportLevel.EqualTo("Info"))  selectedLevel = MessageLogger::Info;
  TString GlobalLabel      = getValueString("GlobalLabel");
  TString SpherocityLabel  = getValueString("SpherocityLabel");
  TString PartLabel        = getValueString("PartLabel");
  TString PairLabel        = getValueString("PairLabel");
  TString NuDynLabel       = getValueString("NuDynLabel");
  TString SimAnaLabel      = getValueString("SimAnaLabel");
  TString RootExt          = getValueString("RootExt");
  TString DerivedLabel     = getValueString("DerivedLabel");
  TString SumLabel         = getValueString("SumLabel");
  TString BalFctLabel      = getValueString("BalFctLabel");
  TString ClosureLabel     = getValueString("ClosureLabel");
  TString GenLabel         = getValueString("GenLabel");
  TString RecoLabel        = getValueString("RecoLabel");
  TString PythiaLabel      = getValueString("PythiaLabel");
  TString HerwigLabel      = getValueString("HerwigLabel");
  TString HijingLabel      = getValueString("HijingLabel");
  TString AmptLabel        = getValueString("AmptLabel");
  TString EposLabel        = getValueString("EposLabel");
  TString UrqmdLabel       = getValueString("UrqmdLabel");
  TString ResoLabel        = getValueString("ResoLabel");
  TString PerformLabel     = getValueString("PerformLabel");
  bool    RunEventAnalysis        = getValueBool("EventAnalysis");
  bool    RunEventAnalysisGen     = getValueBool("EventAnalysisGen");
  bool    RunEventAnalysisReco    = getValueBool("EventAnalysisReco");
  bool    RunDerivedGen           = getValueBool("DerivedGen");
  bool    RunDerivedReco          = getValueBool("DerivedReco");
  bool    RunBalFctGen            = getValueBool("BalFctGen");
  bool    RunBalFctReco           = getValueBool("BalFctReco");
  bool    RunSubsample            = getValueBool("Subsample");
  bool    RunSubsampleBaseGen     = getValueBool("SubsampleBaseGen");
  bool    RunSubsampleBaseReco    = getValueBool("SubsampleBaseReco");
  bool    RunSubsampleDerivedGen  = getValueBool("SubsampleDerivedGen");
  bool    RunSubsampleDerivedReco = getValueBool("SubsampleDerivedReco");
  bool    RunSubsampleBalFctGen   = getValueBool("SubsampleBalFctGen");
  bool    RunSubsampleBalFctReco  = getValueBool("SubsampleBalFctReco");
  bool    RunPerformanceSim       = getValueBool("PerformanceSim");
  bool    RunPerformanceAna       = getValueBool("PerformanceAna");
  bool    RunClosure              = getValueBool("Closure");
  bool    RunClosureBase          = getValueBool("ClosureBase");
  bool    RunClosureDerived       = getValueBool("ClosureDerived");
  bool    RunClosureBalFct        = getValueBool("ClosureBalFct");
  bool    RunPythiaGenerator      = getValueBool("PythiaGenerator");
  bool    RunPythiaReader         = getValueBool("PythiaReader");
  bool    RunHerwigGenerator      = getValueBool("HerwigGenerator");
  bool    RunHerwigReader         = getValueBool("HerwigReader");
  bool    RunAmptGenerator        = getValueBool("AmptGenerator");
  bool    RunAmptReader           = getValueBool("AmptReader");
  bool    RunEposGenerator        = getValueBool("EposGenerator");
  bool    RunEposReader           = getValueBool("EposReader");
  bool    RunUrqmdReader          = getValueBool("UrqmdReader");
  bool    RunHijingReader         = getValueBool("HijingReader");
  bool    RunResonanceGenerator   = getValueBool("ResonanceGenerator");
  bool    RunGlobalGen            = getValueBool("GlobalGen");
  bool    RunGlobalReco           = getValueBool("GlobalReco");
  bool    RunSpherocityGen        = getValueBool("SpherocityGen");
  bool    RunSpherocityReco       = getValueBool("SpherocityReco");
  bool    RunPartGen              = getValueBool("PartGen");
  bool    RunPartReco             = getValueBool("PartReco");
  bool    RunPairGen              = getValueBool("PairGen");
  bool    RunPairReco             = getValueBool("PairReco");
  bool    RunNuDynGen             = getValueBool("NuDynGen");
  bool    RunNuDynReco            = getValueBool("NuDynReco");
  bool    RunFillEta              = getValueBool("FillEta");
  bool    RunFillY                = getValueBool("FillY");
  bool    PdgLoadTable            = getValueBool("Pdg:LoadTable");
  int     beamCode                = getValueInt( "Beam");
  int     targetCode              = getValueInt( "Target");
  double  beamEnergy              = getValueDouble("Energy");
  TString inputPathName           = getValueString("HistogramInputPath");
  TString outputPathName          = getValueString("HistogramOutputPath");
  TString modelPartFilterOption   = getValueString("ModelPartFilterOption");
  double modelPartFilterPt        = getValueBool(  "ModelPartFilterPt");
  double modelPartMinPt           = getValueDouble("ModelPartMinPt");
  double modelPartMaxPt           = getValueDouble("ModelPartMaxPt");
  bool   modelPartFilterEta       = getValueBool(  "ModelPartFilterEta");
  double modelPartMinEta          = getValueDouble("ModelPartMinEta");
  double modelPartMaxEta          = getValueDouble("ModelPartMaxEta");
  bool   modelPartFilterY         = getValueBool(  "ModelPartFilterY");
  double modelPartMinY            = getValueDouble("ModelPartMinY");
  double modelPartMaxY            = getValueDouble("ModelPartMaxY");
  TString anaPartFilterOption     = getValueString("AnaPartFilterOption");
  double anaPartFilterPt          = getValueBool(  "AnaPartFilterPt");
  double anaPartMinPt             = getValueDouble("AnaPartMinPt");
  double anaPartMaxPt             = getValueDouble("AnaPartMaxPt");
  bool   anaPartFilterEta         = getValueBool(  "AnaPartFilterEta");
  double anaPartMinEta            = getValueDouble("AnaPartMinEta");
  double anaPartMaxEta            = getValueDouble("AnaPartMaxEta");
  bool   anaPartFilterY           = getValueBool(  "AnaPartFilterY");
  double anaPartMinY              = getValueDouble("AnaPartMinY");
  double anaPartMaxY              = getValueDouble("AnaPartMaxY");
  bool   setSeed                  = getValueDouble("SetSeed");
  int    seedValue                = getValueInt(   "SeedValue");

  if (reportInfo(__FUNCTION__))
    {
    cout << "GlobalLabel................:" << GlobalLabel      << endl;
    cout << "SpherocityLabel............:" << SpherocityLabel  << endl;
    cout << "PartLabel..................:" << PartLabel        << endl;
    cout << "PairLabel..................:" << PairLabel        << endl;
    cout << "NuDynLabel.................:" << NuDynLabel       << endl;
    cout << "SimAnaLabel................:" << SimAnaLabel      << endl;
    cout << "RootExt....................:" << RootExt          << endl;
    cout << "DerivedLabel...............:" << DerivedLabel     << endl;
    cout << "SumLabel...................:" << SumLabel         << endl;
    cout << "BalFctLabel................:" << BalFctLabel      << endl;
    cout << "ClosureLabel...............:" << ClosureLabel     << endl;
    cout << "GenLabel...................:" << GenLabel         << endl;
    cout << "RecoLabel..................:" << RecoLabel        << endl;
    cout << "PythiaLabel................:" << PythiaLabel      << endl;
    cout << "HerwigLabel................:" << HerwigLabel      << endl;
    cout << "HijingLabel................:" << HijingLabel      << endl;
    cout << "AmptLabel..................:" << AmptLabel        << endl;
    cout << "EposLabel..................:" << EposLabel        << endl;
    cout << "UrqmdLabel.................:" << UrqmdLabel       << endl;
    cout << "ResoLabel..................:" << ResoLabel        << endl;
    cout << "PerformLabel...............:" << PerformLabel     << endl;
    cout << "EventAnalysis..............:" << RunEventAnalysis << endl;
    cout << "EventAnalysisGen...........:" << RunEventAnalysisGen       << endl;
    cout << "EventAnalysisReco..........:" << RunEventAnalysisReco      << endl;
    cout << "DerivedGen.................:" << RunDerivedGen             << endl;
    cout << "DerivedReco................:" << RunDerivedReco            << endl;
    cout << "BalFctGen..................:" << RunBalFctGen              << endl;
    cout << "BalFctReco.................:" << RunBalFctReco             << endl;
    cout << "Subsample..................:" << RunSubsample              << endl;
    cout << "SubsampleBaseGen...........:" << RunSubsampleBaseGen       << endl;
    cout << "SubsampleBaseReco..........:" << RunSubsampleBaseReco      << endl;
    cout << "SubsampleDerivedGen........:" << RunSubsampleDerivedGen    << endl;
    cout << "SubsampleDerivedReco.......:" << RunSubsampleDerivedReco   << endl;
    cout << "SubsampleBalFctGen.........:" << RunSubsampleBalFctGen     << endl;
    cout << "SubsampleBalFctReco........:" << RunSubsampleBalFctReco    << endl;
    cout << "PerformanceSim.............:" << RunPerformanceSim     << endl;
    cout << "PerformanceAna.............:" << RunPerformanceAna     << endl;
    cout << "Closure....................:" << RunClosure            << endl;
    cout << "ClosureBase................:" << RunClosureBase        << endl;
    cout << "ClosureDerived.............:" << RunClosureDerived     << endl;
    cout << "ClosureBalFct..............:" << RunClosureDerived     << endl;
    cout << "PythiaGenerator............:" << RunPythiaGenerator    << endl;
    cout << "PythiaReader...............:" << RunPythiaReader       << endl;
    cout << "HerwigGenerator............:" << RunHerwigGenerator    << endl;
    cout << "HerwigReader...............:" << RunHerwigReader       << endl;
    cout << "AmptGenerator..............:" << RunAmptGenerator      << endl;
    cout << "AmptReader.................:" << RunAmptReader         << endl;
    cout << "EposGenerator..............:" << RunEposGenerator      << endl;
    cout << "EposReader.................:" << RunEposReader         << endl;
    cout << "UrqmdReader................:" << RunUrqmdReader        << endl;
    cout << "HijingReader...............:" << RunHijingReader       << endl;
    cout << "ResonanceGenerator.........:" << RunResonanceGenerator << endl;
    cout << "GlobalGen..................:" << RunGlobalGen          << endl;
    cout << "GlobalReco.................:" << RunGlobalReco         << endl;
    cout << "SpherocityGen..............:" << RunSpherocityGen      << endl;
    cout << "SpherocityReco.............:" << RunSpherocityGen      << endl;
    cout << "PartGen....................:" << RunPartGen            << endl;
    cout << "PartReco...................:" << RunPartReco           << endl;
    cout << "PairGen....................:" << RunPairGen            << endl;
    cout << "PairReco...................:" << RunPairReco           << endl;
    cout << "NuDynGen...................:" << RunNuDynGen           << endl;
    cout << "NuDynReco..................:" << RunNuDynReco          << endl;
    cout << "FillEta....................:" << RunFillEta            << endl;
    cout << "FillY......................:" << RunFillY              << endl;
    cout << "Pdg:LoadTable..............:" << PdgLoadTable          << endl;
    cout << "HistogramInputPath.........:" << inputPathName        << endl;
    cout << "HistogramOutputPath........:" << outputPathName        << endl;
    cout << "ModelPartMinY..............:" << modelPartMinY         << endl;
    cout << "ModelPartMaxY..............:" << modelPartMaxY         << endl;
    cout << "BeamPdgCode................:" << beamCode              << endl;
    cout << "TargetPdgCode..............:" << targetCode            << endl;
    cout << "BeamEnergy........... .....:" << beamEnergy            << endl;
    cout << "ModelPartFilterOption......:" << modelPartFilterOption << endl;
    cout << "ModelPartFilterPt..........:" << modelPartFilterPt   << endl;
    cout << "ModelPartMinPt.............:" << modelPartMinPt      << endl;
    cout << "ModelPartMaxPt.............:" << modelPartMaxPt      << endl;
    cout << "ModelPartFilterEta.........:" << modelPartFilterEta  << endl;
    cout << "ModelPartMinEta............:" << modelPartMinEta     << endl;
    cout << "ModelPartMaxEta............:" << modelPartMaxEta     << endl;
    cout << "ModelPartFilterY...........:" << modelPartFilterY    << endl;
    cout << "ModelPartMinY..............:" << modelPartMinY       << endl;
    cout << "ModelPartMaxY..............:" << modelPartMaxY       << endl;
    cout << "AnaPartFilterOption........:" << anaPartFilterOption << endl;
    cout << "AnaPartFilterPt............:" << anaPartFilterPt     << endl;
    cout << "AnaPartMinPt...............:" << anaPartMinPt        << endl;
    cout << "AnaPartMaxPt...............:" << anaPartMaxPt        << endl;
    cout << "AnaPartFilterEta...........:" << anaPartFilterEta    << endl;
    cout << "AnaPartMinEta..............:" << anaPartMinEta       << endl;
    cout << "AnaPartMaxEta..............:" << anaPartMaxEta       << endl;
    cout << "AnaPartFilterY.............:" << anaPartFilterY      << endl;
    cout << "AnaPartMinY................:" << anaPartMinY         << endl;
    cout << "AnaPartMaxY................:" << anaPartMaxY         << endl;
    cout << "SetSeed....................:" << setSeed             << endl;
    cout << "SeedValue..................:" << seedValue           << endl;
    }
  vector<EventFilter*> modelEventFilters;
  vector<EventFilter*> analysisEventFilters;
  vector<ParticleFilter*>  modelParticleFilters;
  vector<ParticleFilter*>  analysisParticleFilters;

  // =========================================
  // Setup all event filters
  // =========================================
  vector<double> modelBounds;
  TString modelEventFilterOption = getValueString("ModelEventFilterOption");
  int n = getValueInt("ModelEventFilterNValues");
  for (int k=0; k<n; k++)
    {
    TString key = "ModelEventFilterValue";
    key += k;
    modelBounds.push_back( getValueBool(key));
    }
  if (modelEventFilterOption.EqualTo("All"))                  modelEventFilters = EventFilter::createOpenEventFilter();
  else if (modelEventFilterOption.EqualTo("AliceMB"))          modelEventFilters = EventFilter::createAliceMBEventFilter();
  else if (modelEventFilterOption.EqualTo("ImpactParameter"))  modelEventFilters = EventFilter::createImpactParameterFilters(modelBounds);
  else if (modelEventFilterOption.EqualTo("V0Mult"))           modelEventFilters = EventFilter::createV0MultiplicityFilters(modelBounds);
  else if (modelEventFilterOption.EqualTo("TpcMult"))          modelEventFilters = EventFilter::createTpcMultiplicityFilters(modelBounds);

  vector<double> anaBounds;
  TString anaEventFilterOption = getValueString("AnaEventFilterOption");
  n = getValueInt("AnaEventFilterNValues");
  for (int k=0; k<n; k++)
    {
    TString key = "AnaEventFilterValue";
    key += k;
    anaBounds.push_back( getValueBool(key));
    }
  if (anaEventFilterOption.EqualTo("All"))                  analysisEventFilters = EventFilter::createOpenEventFilter();
  else if (anaEventFilterOption.EqualTo("AliceMB"))          analysisEventFilters = EventFilter::createAliceMBEventFilter();
  else if (anaEventFilterOption.EqualTo("ImpactParameter"))  analysisEventFilters = EventFilter::createImpactParameterFilters(anaBounds);
  else if (anaEventFilterOption.EqualTo("V0Mult"))           analysisEventFilters = EventFilter::createV0MultiplicityFilters(anaBounds);
  else if (anaEventFilterOption.EqualTo("TpcMult"))          analysisEventFilters = EventFilter::createTpcMultiplicityFilters(anaBounds);

  if (analysisEventFilters.size()<1)
    {
    if (reportFatal(__FUNCTION__))
      {
      cout << endl;
      cout << "==================================================================================" << std::endl;
      cout << "No Analysis Event Filter" << std::endl;
      cout << "==================================================================================" << std::endl;
      }
    exit(1);
    }


  // =========================================
  // Setup all particle filters
  // =========================================
  if (modelPartFilterOption.EqualTo("All"))                   modelParticleFilters = ParticleFilter::createOpenParticleFilter();
  else if (modelPartFilterOption.EqualTo("AliceV0"))          modelParticleFilters = ParticleFilter::createAliceV0Filter();
  else if (modelPartFilterOption.EqualTo("Neutral"))          modelParticleFilters = ParticleFilter::createNeutralParticleFilter(modelPartFilterPt,modelPartMinPt,modelPartMaxPt,modelPartFilterEta,modelPartMinEta,modelPartMaxEta,modelPartFilterY,modelPartMinY,modelPartMaxY);
  else if (modelPartFilterOption.EqualTo("Charged"))          modelParticleFilters = ParticleFilter::createChargedParticleFilter(modelPartFilterPt,modelPartMinPt,modelPartMaxPt,modelPartFilterEta,modelPartMinEta,modelPartMaxEta,modelPartFilterY,modelPartMinY,modelPartMaxY);
  else if (modelPartFilterOption.EqualTo("Negative"))         modelParticleFilters = ParticleFilter::createNegativeParticleFilter(modelPartFilterPt,modelPartMinPt,modelPartMaxPt,modelPartFilterEta,modelPartMinEta,modelPartMaxEta,modelPartFilterY,modelPartMinY,modelPartMaxY);
  else if (modelPartFilterOption.EqualTo("Positive"))         modelParticleFilters = ParticleFilter::createPositiveParticleFilter(modelPartFilterPt,modelPartMinPt,modelPartMaxPt,modelPartFilterEta,modelPartMinEta,modelPartMaxEta,modelPartFilterY,modelPartMinY,modelPartMaxY);
  else if (modelPartFilterOption.EqualTo("ChargedHadrons"))   modelParticleFilters = ParticleFilter::createChargedHadronFilters(modelPartFilterPt,modelPartMinPt,modelPartMaxPt,modelPartFilterEta,modelPartMinEta,modelPartMaxEta,modelPartFilterY,modelPartMinY,modelPartMaxY);
  else if (modelPartFilterOption.EqualTo("PlusMinusHadrons")) modelParticleFilters = ParticleFilter::createPlusMinusHadronFilters(modelPartFilterPt,modelPartMinPt,modelPartMaxPt,modelPartFilterEta,modelPartMinEta,modelPartMaxEta,modelPartFilterY,modelPartMinY,modelPartMaxY);
  else if (modelPartFilterOption.EqualTo("StrangeHadrons"))   modelParticleFilters = ParticleFilter::createStrangeHadronFilters(modelPartFilterPt,modelPartMinPt,modelPartMaxPt,modelPartFilterEta,modelPartMinEta,modelPartMaxEta,modelPartFilterY,modelPartMinY,modelPartMaxY);
  else if (modelPartFilterOption.EqualTo("Baryons"))          modelParticleFilters = ParticleFilter::createBaryonFilters(modelPartFilterPt,modelPartMinPt,modelPartMaxPt,modelPartFilterEta,modelPartMinEta,modelPartMaxEta,modelPartFilterY,modelPartMinY,modelPartMaxY);

  if (anaPartFilterOption.EqualTo("All"))                   analysisParticleFilters = ParticleFilter::createOpenParticleFilter();
  else if (anaPartFilterOption.EqualTo("AliceV0"))          analysisParticleFilters = ParticleFilter::createAliceV0Filter();
  else if (anaPartFilterOption.EqualTo("Neutral"))          analysisParticleFilters = ParticleFilter::createNeutralParticleFilter(anaPartFilterPt,anaPartMinPt,anaPartMaxPt,anaPartFilterEta,anaPartMinEta,anaPartMaxEta,anaPartFilterY,anaPartMinY,anaPartMaxY);
  else if (anaPartFilterOption.EqualTo("Charged"))          analysisParticleFilters = ParticleFilter::createChargedParticleFilter(anaPartFilterPt,anaPartMinPt,anaPartMaxPt,anaPartFilterEta,anaPartMinEta,anaPartMaxEta,anaPartFilterY,anaPartMinY,anaPartMaxY);
  else if (anaPartFilterOption.EqualTo("Negative"))         analysisParticleFilters = ParticleFilter::createNegativeParticleFilter(anaPartFilterPt,anaPartMinPt,anaPartMaxPt,anaPartFilterEta,anaPartMinEta,anaPartMaxEta,anaPartFilterY,anaPartMinY,anaPartMaxY);
  else if (anaPartFilterOption.EqualTo("Positive"))         analysisParticleFilters = ParticleFilter::createPositiveParticleFilter(anaPartFilterPt,anaPartMinPt,anaPartMaxPt,anaPartFilterEta,anaPartMinEta,anaPartMaxEta,anaPartFilterY,anaPartMinY,anaPartMaxY);
  else if (anaPartFilterOption.EqualTo("ChargedHadrons"))   analysisParticleFilters = ParticleFilter::createChargedHadronFilters(anaPartFilterPt,anaPartMinPt,anaPartMaxPt,anaPartFilterEta,anaPartMinEta,anaPartMaxEta,anaPartFilterY,anaPartMinY,anaPartMaxY);
  else if (anaPartFilterOption.EqualTo("PlusMinusHadrons")) analysisParticleFilters = ParticleFilter::createPlusMinusHadronFilters(anaPartFilterPt,anaPartMinPt,anaPartMaxPt,anaPartFilterEta,anaPartMinEta,anaPartMaxEta,anaPartFilterY,anaPartMinY,anaPartMaxY);
  else if (anaPartFilterOption.EqualTo("StrangeHadrons"))   analysisParticleFilters = ParticleFilter::createStrangeHadronFilters(anaPartFilterPt,anaPartMinPt,anaPartMaxPt,anaPartFilterEta,anaPartMinEta,anaPartMaxEta,anaPartFilterY,anaPartMinY,anaPartMaxY);
  else if (anaPartFilterOption.EqualTo("Baryons"))          analysisParticleFilters = ParticleFilter::createBaryonFilters(anaPartFilterPt,anaPartMinPt,anaPartMaxPt,anaPartFilterEta,anaPartMinEta,anaPartMaxEta,anaPartFilterY,anaPartMinY,anaPartMaxY);

  if (analysisParticleFilters.size()<1)
    {
    if (reportFatal(__FUNCTION__))
      {
      cout << endl;
      cout << "==================================================================================" << std::endl;
      cout << "No Analysis Particle Filter" << std::endl;
      cout << "==================================================================================" << std::endl;
      }
    exit(1);
    }
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "  RunAnalysis:Filters" << endl;
    cout << "  modelEventFilters.size().............: " << modelEventFilters.size() << endl;
    for (int k=0; k<modelEventFilters.size(); k++)
      cout << "    " << k << "   " << modelEventFilters[k]->getName() << endl;
    cout << "  modelParticleFilters.size()..........: " << modelParticleFilters.size() << endl;
    for (int k=0; k<modelParticleFilters.size(); k++)
      cout << "    " << k << "   " << modelParticleFilters[k]->getName() << endl;
    cout << "  analysisEventFilters.size()..........: " << analysisEventFilters.size() << endl;
    for (int k=0; k<analysisEventFilters.size(); k++)
      cout << "    " << k << "   " << analysisEventFilters[k]->getName() << endl;
    cout << "  analysisParticleFilters.size().......: " << analysisParticleFilters.size() << endl;
    for (int k=0; k<analysisParticleFilters.size(); k++)
      cout << "    " << k << "   " << analysisParticleFilters[k]->getName() << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Configuration Completed - Proceed with loading analysis" << std::endl;
    cout << "==================================================================================" << std::endl;
    }

  if (PdgLoadTable)  addSubTask(new ParticleTypeTableLoader("Pdg",configuration));
  if (RunEventAnalysis)
    {
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << "==================================================================================" << std::endl;
      cout << "Setting up event analysis" << std::endl;
      cout << "==================================================================================" << std::endl;
      }
    eventAnalysis = new TaskIterator("Analysis",configuration);
    addSubTask(eventAnalysis);

    if (RunPythiaReader)       eventAnalysis->addSubTask(new PythiaEventReader(PythiaLabel,configuration,modelEventFilters,modelParticleFilters));
    if (RunPythiaGenerator)    eventAnalysis->addSubTask(new PythiaEventGenerator(PythiaLabel,configuration,modelEventFilters,modelParticleFilters));
    if (RunHerwigReader)       eventAnalysis->addSubTask(new HerwigEventReader(HerwigLabel,configuration,modelEventFilters,modelParticleFilters));
    if (RunAmptReader)         eventAnalysis->addSubTask(new AmptEventReader(AmptLabel,configuration,modelEventFilters,modelParticleFilters));
    if (RunEposReader)         eventAnalysis->addSubTask(new EposEventReader(EposLabel,configuration,modelEventFilters,modelParticleFilters));
    if (RunUrqmdReader)        eventAnalysis->addSubTask(new UrqmdEventReader(UrqmdLabel,configuration,modelEventFilters,modelParticleFilters));
    if (RunHijingReader)       eventAnalysis->addSubTask(new HijingEventReader(HijingLabel,configuration,modelEventFilters,modelParticleFilters));
    if (RunResonanceGenerator) eventAnalysis->addSubTask(new ResonanceGenerator(ResoLabel,configuration,modelEventFilters,modelParticleFilters));
    if (RunPerformanceSim)     eventAnalysis->addSubTask(new MeasurementPerformanceSimulator(PerformLabel,configuration,modelEventFilters,modelParticleFilters));
    }

  if (RunDerivedGen)
    {
    derivedGen  = new DerivedHistoIterator("DerivedGen", configuration);
    addSubTask(derivedGen);
    }

  if (RunDerivedReco)
    {
    derivedReco = new DerivedHistoIterator("DerivedReco",configuration);
    addSubTask(derivedReco);
    }



  if (RunPerformanceAna)
    {
    task = new ParticlePerformanceAnalyzer(SimAnaLabel,configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysis)            eventAnalysis->addSubTask(task);
    if (RunDerivedReco)              derivedReco->addSubTask(task);
    }

  if (RunGlobalGen)
    {
    task = new GlobalAnalyzer(GlobalLabel+GenLabel, configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysisGen)               eventAnalysis->addSubTask(task);
    if (RunDerivedGen)                     derivedGen->addSubTask(task);
    //    if (RunClosure && RunClosureBase)      closureBasic->addSubTask(task);
    //    if (RunClosure && RunClosureDerived)   closureDerived->addSubTask(task);
    }
  if (RunGlobalReco)
    {
    task = new GlobalAnalyzer(GlobalLabel+RecoLabel, configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysisReco)         eventAnalysis->addSubTask(task);
    if (RunDerivedReco)               derivedReco->addSubTask(task);
    }

  if (RunSpherocityGen)
    {
    task = new TransverseSpherocityAnalyzer(SpherocityLabel+GenLabel, configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysisGen)               eventAnalysis->addSubTask(task);
    if (RunDerivedGen)                     derivedGen->addSubTask(task);
    //    if (RunClosure && RunClosureBase)      closureBasic->addSubTask(task);
    //    if (RunClosure && RunClosureDerived)   closureDerived->addSubTask(task);
    }
  if (RunSpherocityReco)
    {
    task = new TransverseSpherocityAnalyzer(SpherocityLabel+RecoLabel, configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysisReco)         eventAnalysis->addSubTask(task);
    if (RunDerivedReco)               derivedReco->addSubTask(task);
    }

  if (RunPartGen)
    {
    task = new ParticleAnalyzer(PartLabel+GenLabel, configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysisGen)             eventAnalysis->addSubTask(task);
    if (RunDerivedGen)                   derivedGen->addSubTask(task);
    //    if (RunClosure && RunClosureBase)    closureBasic->addSubTask(task);
    //    if (RunClosure && RunClosureDerived) closureDerived->addSubTask(task);
    }
  if (RunPartReco)
    {
    task = new ParticleAnalyzer(PartLabel+RecoLabel, configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysisReco)         eventAnalysis->addSubTask(task);
    if (RunDerivedReco)               derivedReco->addSubTask(task);
    }

  if (RunPairGen)
    {
    task = new ParticlePairAnalyzer(PairLabel+GenLabel, configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysisGen)         eventAnalysis->addSubTask(task);
    if (RunDerivedGen)               derivedGen->addSubTask(task);
    //    if (RunClosure && RunClosureBase)      closureBasic->addSubTask(task);
    //    if (RunClosure && RunClosureDerived)   closureDerived->addSubTask(task);
    //    if (RunClosure && RunClosureBalFct)    closureBalFct->addSubTask(task);
    }
  if (RunPairReco)
    {
    task = new ParticlePairAnalyzer(PairLabel+RecoLabel, configuration,analysisEventFilters, analysisParticleFilters);
    if (RunEventAnalysisReco)         eventAnalysis->addSubTask(task);
    if (RunDerivedReco)               derivedReco->addSubTask(task);
    }

  if (RunNuDynGen)
    {
    task = new NuDynAnalyzer(NuDynLabel+GenLabel,configuration,analysisEventFilters,analysisParticleFilters);
    if (RunEventAnalysisGen)              eventAnalysis->addSubTask(task);
    if (RunDerivedGen)                    derivedGen->addSubTask(task);
    //    if (RunClosure && RunClosureBase)     closureBasic->addSubTask(task);
    //    if (RunClosure && RunClosureBalFct)   closureDerived->addSubTask(task);
    }
  if (RunNuDynReco)
    {
    task = new NuDynAnalyzer(NuDynLabel+RecoLabel,configuration,analysisEventFilters,analysisParticleFilters);
    if (RunEventAnalysisReco)             eventAnalysis->addSubTask(task);
    if (RunDerivedReco)                   derivedReco->addSubTask(task);
    //    if (RunClosure && RunClosureBase)     closureBasic->addSubTask(task);
    //    if (RunClosure && RunClosureBalFct)   closureDerived->addSubTask(task);
    }

  // subsample calculations

  if (RunSubsample && RunSubsampleBaseGen && RunGlobalGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":ExcludedPattern3"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(GlobalLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseReco && RunGlobalReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":ExcludedPattern3"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(GlobalLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseGen && RunSpherocityGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":ExcludedPattern3"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(SpherocityLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseReco && RunSpherocityReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":ExcludedPattern3"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(SpherocityLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseGen && RunPartGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":ExcludedPattern3"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(PartLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseReco && RunPartReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":ExcludedPattern3"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(PartLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseGen && RunPairGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    //subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern3"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(PairLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseGen && RunPairReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    //subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern3"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(PairLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseGen && RunNuDynGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":ExcludedPattern3"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(PairLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBaseGen && RunNuDynReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":ExcludedPattern3"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(NuDynLabel+RecoLabel,subConfig));
    }

  // ------
  if (RunSubsample && RunSubsampleDerivedGen && RunGlobalGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+GenLabel+TString(":ExcludedPattern2"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(GlobalLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedReco && RunGlobalReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+GlobalLabel+RecoLabel+TString(":ExcludedPattern2"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(GlobalLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedGen && RunSpherocityGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+GenLabel+TString(":ExcludedPattern2"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(SpherocityLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedReco && RunSpherocityReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+SpherocityLabel+RecoLabel+TString(":ExcludedPattern2"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(SpherocityLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedGen && RunPartGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PartLabel+GenLabel+TString(":ExcludedPattern2"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(PartLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedReco && RunPartReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PartLabel+RecoLabel+TString(":ExcludedPattern2"),TString("Sum"));
    addSubTask( new SubSampleStatCalculator(PartLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedGen && RunPairGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    //subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":IncludedPattern0"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern2"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(PairLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedReco && RunPairReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern2"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(PairLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedGen && RunNuDynGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+GenLabel+TString(":ExcludedPattern2"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(PairLabel+GenLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleDerivedReco && RunNuDynReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":IncludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":ExcludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+NuDynLabel+RecoLabel+TString(":ExcludedPattern2"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(NuDynLabel+RecoLabel,subConfig));
    }

  if (RunSubsample && RunSubsampleBalFctGen && RunPairGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":IncludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":IncludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern2"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(PairLabel+GenLabel,subConfig));
    }

  // balance function

  if (RunSubsample && RunSubsampleBalFctReco && RunPairReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":IncludedPattern0"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":IncludedPattern1"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern0"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern1"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern2"),TString("Sum"));
    subConfig.printConfiguration(cout);
    addSubTask( new SubSampleStatCalculator(PairLabel+GenLabel,subConfig));
    }

  if (RunBalFctGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":IncludedPattern0"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern1"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    balFctGen = new BalanceFunctionCalculator("PairGen",subConfig,analysisEventFilters, analysisParticleFilters);
    addSubTask(balFctGen);
    }

  if (RunBalFctReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramInputPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramOutputPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":IncludedPattern0"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern1"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    balFctReco = new BalanceFunctionCalculator("PairReco",subConfig,analysisEventFilters, analysisParticleFilters);
    addSubTask(balFctReco);
    }

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Configuration Completed; Run Analysis" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
  gSystem->mkdir(outputPathName,1);


  if (hasSubTasks() && isTaskOk()) configureSubTasks();
  if (reportEnd(__FUNCTION__))
    ;
}

void RunAnalysis::execute()
{
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Analysis Started" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
  //initializeSubTasks();
  executeSubTasks();

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Analysis Completed" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
}

