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
#include "ParticleDbManager.hpp"
//#include "PythiaEventReader.hpp"
#include "PythiaEventGenerator.hpp"
//#include "HerwigEventReader.hpp"
#include "AmptEventReader.hpp"
//#include "EposEventReader.hpp"
//#include "UrqmdEventReader.hpp"
//#include "HijingEventReader.hpp"
#include "TherminatorGenerator.hpp"
#include "ResonanceGenerator.hpp"
#include "MeasurementPerformanceSimulator.hpp"
#include "ParticlePerformanceAnalyzer.hpp"
#include "GlobalAnalyzer.hpp"
#include "TransverseSpherocityAnalyzer.hpp"
#include "ParticleSingleAnalyzer.hpp"
#include "ParticlePairAnalyzer.hpp"
#include "NuDynAnalyzer.hpp"
using CAP::RunAnalysis;

ClassImp(RunAnalysis);

RunAnalysis::RunAnalysis(const String & _name,
                         const Configuration & _configuration)
:
EventTask(_name, _configuration)
{
  appendClassName("RunAnalysis");
}


void RunAnalysis::setDefaultConfiguration()
{
  EventTask::setDefaultConfiguration();
  bool YES = true;
  bool NO  = false;
  addParameter("GlobalLabel",     TString("G"));
  addParameter("SpherocityLabel", TString("S"));
  addParameter("PartLabel",       TString("Part"));
  addParameter("PairLabel",       TString("Pair"));
  addParameter("NuDynLabel",      TString("NuDyn"));
  addParameter("SimAnaLabel",     TString("SimAna"));
  addParameter("RootExt",         TString(".root"));
  addParameter("DerivedLabel",    TString("Derived"));
  addParameter("SumLabel",        TString("Sum"));
  addParameter("BalFctLabel",     TString("BalFct"));
  addParameter("ClosureLabel",    TString("Closure"));
  addParameter("GenLabel",        TString("Gen"));
  addParameter("RecoLabel",       TString("Reco"));
  addParameter("PythiaLabel",     TString("PYTHIA"));
  addParameter("HerwigLabel",     TString("HERWIG"));
  addParameter("HijingLabel",     TString("HIJING"));
  addParameter("AmptLabel",       TString("AMPT"));
  addParameter("EposLabel",       TString("EPOST"));
  addParameter("UrqmdLabel",      TString("URQMD"));
  addParameter("TherminatorLabel",TString("Therminator"));
  addParameter("ResoLabel",       TString("RESO"));
  addParameter("PerformLabel",    TString("Perform"));

  //  addParameter("histoModelDataName",      TString("none"));
  //  addParameter("histoAnalyzerName",       TString("none"));
  //  addParameter("histoBaseName",           TString("none"));
  addParameter("Severity",                TString("Info"));
  addParameter("RunParticleDbManager",       YES);
  addParameter("RunEventAnalysis",           YES);
  addParameter("RunEventAnalysisGen",        YES);
  addParameter("RunEventAnalysisReco",       NO);
  addParameter("RunDerivedGen",              NO);
  addParameter("RunDerivedReco",             NO);
  addParameter("RunBalFctGen",               NO);
  addParameter("RunBalFctReco",              NO);
  addParameter("RunPerformanceSim",          NO);
  addParameter("RunPerformanceAna",          NO);
  addParameter("RunClosure",                 NO);
  addParameter("RunPythiaGenerator",         NO);
  addParameter("RunPythiaReader",            NO);
  addParameter("RunHerwigGenerator",         NO);
  addParameter("RunHerwigReader",            NO);
  addParameter("RunAmptGenerator",           NO);
  addParameter("RunAmptReader",              NO);
  addParameter("RunEposGenerator",           NO);
  addParameter("RunEposReader",              NO);
  addParameter("RunUrqmdReader",             NO);
  addParameter("RunTherminatorGenerator",    NO);
  addParameter("RunHijingReader",            NO);
  addParameter("RunResonanceGenerator",      NO);
  addParameter("RunGlobalAnalysisGen",       NO);
  addParameter("RunGlobalAnalysisReco",      NO);
  addParameter("RunSpherocityAnalysisGen",   NO);
  addParameter("RunSpherocityAnalysisReco",  NO);
  addParameter("RunPartSingleAnalysisGen",   NO);
  addParameter("RunPartSingleAnalysisReco",  NO);
  addParameter("RunPartPairAnalysisGen",     NO);
  addParameter("RunPartPairAnalysisReco",    NO);
  addParameter("RunNuDynAnalysisGen",        NO);
  addParameter("RunNuDynAnalysisReco",       NO);
  addParameter("RunFillEta",                 NO);
  addParameter("RunFillY",                   YES);
  addParameter("nEventsPerSubbunch",      long(100));
  addParameter("nSubbunchesPerBunch",     int(10));
  addParameter("nBunches",                int(10));
  addParameter("BunchLabel",              "Bunch");
  addParameter("SubbunchLabel",           "");
  addParameter("EventsUseStream0",         true);
  addParameter("EventsUseStream1",         false);
  addParameter("SetSeed",                 true);
  addParameter("SeedValue",               long(121212121));

  addParameter("EventFilterModelOption",     TString("All"));
  addParameter("EventFilterModelNValues",    0);
  addParameter("EventFilterModelValue0",     0.0);
  addParameter("EventFilterModelValue1",     0.0);
  addParameter("EventFilterModelValue2",     0.0);
  addParameter("EventFilterModelValue3",     0.0);
  addParameter("EventFilterModelValue4",     0.0);
  addParameter("EventFilterModelValue5",     0.0);
  addParameter("EventFilterModelValue6",     0.0);
  addParameter("EventFilterModelValue7",     0.0);
  addParameter("EventFilterModelValue8",     0.0);
  addParameter("EventFilterModelValue9",     0.0);
  addParameter("EventFilterModelValue10",     0.0);
  addParameter("EventFilterModelValue11",     0.0);
  addParameter("EventFilterModelValue12",     0.0);

  addParameter("EventFilterAnaOption",     TString("All"));
  addParameter("EventFilterAnaNValues",    0);
  addParameter("EventFilterAnaValue0",     0.0);
  addParameter("EventFilterAnaValue1",     0.0);
  addParameter("EventFilterAnaValue2",     0.0);
  addParameter("EventFilterAnaValue3",     0.0);
  addParameter("EventFilterAnaValue4",     0.0);
  addParameter("EventFilterAnaValue5",     0.0);
  addParameter("EventFilterAnaValue6",     0.0);
  addParameter("EventFilterAnaValue7",     0.0);
  addParameter("EventFilterAnaValue8",     0.0);
  addParameter("EventFilterAnaValue9",     0.0);
  addParameter("EventFilterAnaValue10",     0.0);
  addParameter("EventFilterAnaValue11",     0.0);
  addParameter("EventFilterAnaValue12",     0.0);

  addParameter("PartFilterModelOption",     TString("All"));
  addParameter("PartFilterModelPt",         true);
  addParameter("PartFilterModelMinPt",            0.2);
  addParameter("PartFilterModelMaxPt",            2.0);
  addParameter("PartFilterModelEta",        false);
  addParameter("PartFilterModelMinEta",           -4.0);
  addParameter("PartFilterModelMaxEta",            4.0);
  addParameter("PartFilterModelY",          true);
  addParameter("PartFilterModelMinY",             -4.0);
  addParameter("PartFilterModelMaxY",              4.0);

  addParameter("PartFilterAnaOption",       TString("All"));
  addParameter("PartFilterAnaFilterPt",           true);
  addParameter("PartFilterAnaMinPt",              0.2);
  addParameter("PartFilterAnaMaxPt",              2.0);
  addParameter("PartFilterAnaEta",          false);
  addParameter("PartFilterAnaMinEta",             -4.0);
  addParameter("PartFilterAnaMaxEta",             4.0);
  addParameter("PartFilterAnaY",            true);
  addParameter("PartFilterAnaMinY",               -4.0);
  addParameter("PartFilterAnaMaxY",               4.0);
}


void RunAnalysis::configure()
{
  if (reportStart(__FUNCTION__))
;
  EventTask::configure();
  setSeverity();
  // assemble the task from here...
  TaskIterator            * eventAnalysis = nullptr;
  String GlobalLabel      = getValueString("GlobalLabel");
  String SpherocityLabel  = getValueString("SpherocityLabel");
  String PartLabel        = getValueString("PartLabel");
  String PairLabel        = getValueString("PairLabel");
  String NuDynLabel       = getValueString("NuDynLabel");
  String SimAnaLabel      = getValueString("SimAnaLabel");
  String RootExt          = getValueString("RootExt");
  String DerivedLabel     = getValueString("DerivedLabel");
  String SumLabel         = getValueString("SumLabel");
  String BalFctLabel      = getValueString("BalFctLabel");
  String ClosureLabel     = getValueString("ClosureLabel");
  String GenLabel         = getValueString("GenLabel");
  String RecoLabel        = getValueString("RecoLabel");
  String PythiaLabel      = getValueString("PythiaLabel");
  String HerwigLabel      = getValueString("HerwigLabel");
  String HijingLabel      = getValueString("HijingLabel");
  String AmptLabel        = getValueString("AmptLabel");
  String EposLabel        = getValueString("EposLabel");
  String UrqmdLabel       = getValueString("UrqmdLabel");
  String TherminatorLabel = getValueString("TherminatorLabel");
  String ResoLabel        = getValueString("ResoLabel");
  String PerformLabel     = getValueString("PerformLabel");
  bool    runParticleDbManager            = getValueBool("RunParticleDbManager");
  bool    runEventAnalysis                = getValueBool("RunEventAnalysis");
  bool    runEventAnalysisGen             = getValueBool("RunEventAnalysisGen");
  bool    runEventAnalysisReco            = getValueBool("RunEventAnalysisReco");
  bool    runDerivedGen                   = getValueBool("RunDerivedGen");
  bool    runDerivedReco                  = getValueBool("RunDerivedReco");
  bool    runBalFctGen                    = getValueBool("RunBalFctGen");
  bool    runBalFctReco                   = getValueBool("RunBalFctReco");
  bool    runSubsample                    = getValueBool("RunSubsample");
//  bool    runSubsampleBaseGen             = getValueBool("RunSubsampleBaseGen");
//  bool    runSubsampleBaseReco            = getValueBool("RunSubsampleBaseReco");
//  bool    runSubsampleDerivedGen          = getValueBool("RunSubsampleDerivedGen");
//  bool    runSubsampleDerivedReco         = getValueBool("RunSubsampleDerivedReco");
//  bool    runSubsampleBalFctGen           = getValueBool("RunSubsampleBalFctGen");
//  bool    runSubsampleBalFctReco          = getValueBool("RunSubsampleBalFctReco");
  bool    runPerformanceSim               = getValueBool("RunPerformanceSim");
  bool    runPerformanceAna               = getValueBool("RunPerformanceAna");
  bool    runClosure                      = getValueBool("RunClosure");
  bool    runPythiaGenerator              = getValueBool("RunPythiaGenerator");
  bool    runPythiaReader                 = getValueBool("RunPythiaReader");
  bool    runHerwigGenerator              = getValueBool("RunHerwigGenerator");
  bool    runHerwigReader                 = getValueBool("RunHerwigReader");
  bool    runAmptGenerator                = getValueBool("RunAmptGenerator");
  bool    runAmptReader                   = getValueBool("RunAmptReader");
  bool    runEposGenerator                = getValueBool("RunEposGenerator");
  bool    runEposReader                   = getValueBool("RunEposReader");
  bool    runUrqmdReader                  = getValueBool("RunUrqmdReader");
  bool    runTherminatorGenerator         = getValueBool("RunTherminatorGenerator");
  bool    runHijingReader                 = getValueBool("RunHijingReader");
  bool    runResonanceGenerator           = getValueBool("RunResonanceGenerator");
  bool    runGlobalAnalysisGen            = getValueBool("RunGlobalAnalysisGen");
  bool    runGlobalAnalysisReco           = getValueBool("RunGlobalAnalysisReco");
  bool    runSpherocityAnalysisGen        = getValueBool("RunSpherocityAnalysisGen");
  bool    runSpherocityAnalysisReco       = getValueBool("RunSpherocityAnalysisReco");
  bool    runPartSingleAnalysisGen        = getValueBool("RunPartSingleAnalysisGen");
  bool    runPartSingleAnalysisReco       = getValueBool("RunPartSingleAnalysisReco");
  bool    runPartPairAnalysisGen          = getValueBool("RunPartPairAnalysisGen");
  bool    runPartPairAnalysisReco         = getValueBool("RunPartPairAnalysisReco");
  bool    runNuDynAnalysisGen             = getValueBool("RunNuDynAnalysisGen");
  bool    runNuDynAnalysisReco            = getValueBool("RunNuDynAnalysisReco");
  bool    runFillEta                      = getValueBool("RunFillEta");
  bool    runFillY                        = getValueBool("RunFillY");
  String inputPathName                    = getValueString("HistogramsImportPath");
  String outputPathName                   = getValueString("HistogramsExportPath");
  String partFilterModelOption            = getValueString("PartFilterModelOption");
  double partFilterModelPt                = getValueBool(  "PartFilterModelPt");
  double partFilterModelMinPt             = getValueDouble("PartFilterModelMinPt");
  double partFilterModelMaxPt             = getValueDouble("PartFilterModelMaxPt");
  bool   partFilterModelEta               = getValueBool(  "PartFilterModelEta");
  double partFilterModelMinEta            = getValueDouble("PartFilterModelMinEta");
  double partFilterModelMaxEta            = getValueDouble("PartFilterModelMaxEta");
  bool   partFilterModelY                 = getValueBool(  "PartFilterModelY");
  double partFilterModelMinY              = getValueDouble("PartFilterModelMinY");
  double partFilterModelMaxY              = getValueDouble("PartFilterModelMaxY");
  String partFilterAnaOption              = getValueString("PartFilterAnaOption");
  double partFilterAnaFilterPt            = getValueBool(  "PartFilterAnaFilterPt");
  double partFilterAnaMinPt               = getValueDouble("PartFilterAnaMinPt");
  double partFilterAnaMaxPt               = getValueDouble("PartFilterAnaMaxPt");
  bool   partFilterAnaEta                 = getValueBool(  "PartFilterAnaEta");
  double partFilterAnaMinEta              = getValueDouble("PartFilterAnaMinEta");
  double partFilterAnaMaxEta              = getValueDouble("PartFilterAnaMaxEta");
  bool   partFilterAnaY                   = getValueBool(  "PartFilterAnaY");
  double partFilterAnaMinY                = getValueDouble("PartFilterAnaMinY");
  double partFilterAnaMaxY                = getValueDouble("PartFilterAnaMaxY");
  bool   setSeed                          = getValueBool(  "SetSeed");
  long   seedValue                        = getValueLong(  "SeedValue");

//  if (reportDebug(__FUNCTION__)) printConfiguration(cout);

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("GlobalLabel",GlobalLabel);
    printItem("SpherocityLabel",SpherocityLabel);
    printItem("PartLabel",PartLabel);
    printItem("PairLabel",PairLabel);
    printItem("NuDynLabel",NuDynLabel);
    printItem("SimAnaLabel",SimAnaLabel);
    printItem("RootExt",RootExt);
    printItem("DerivedLabel",DerivedLabel);
    printItem("SumLabel",SumLabel);
    printItem("BalFctLabel",BalFctLabel);
    printItem("ClosureLabel",ClosureLabel);
    printItem("GenLabel",GenLabel);
    printItem("RecoLabel",RecoLabel);
    printItem("PythiaLabel",PythiaLabel);
    printItem("HerwigLabel",HerwigLabel);
    printItem("HijingLabel",HijingLabel);
    printItem("AmptLabel",AmptLabel);
    printItem("EposLabel",EposLabel);
    printItem("UrqmdLabel",UrqmdLabel);
    printItem("TherminatorLabel",TherminatorLabel);
    printItem("ResoLabel",ResoLabel);
    printItem("PerformLabel",PerformLabel);
    printItem("RunParticleDbManager",runParticleDbManager);
    printItem("RunEventAnalysis",runEventAnalysis);
    printItem("RunEventAnalysisGen",runEventAnalysisGen);
    printItem("RunEventAnalysisReco",runEventAnalysisReco);
    printItem("RunDerivedGen",runDerivedGen);
    printItem("RunDerivedReco",runDerivedReco);
    printItem("RunBalFctGen",runBalFctGen);
    printItem("RunBalFctReco",runBalFctReco);
    printItem("RunSubsample",runSubsample);
    printItem("RunPerformanceSim",runPerformanceSim);
    printItem("RunPerformanceAna",runPerformanceAna);
    printItem("RunClosure",runClosure);
    printItem("RunPythiaGenerator",runPythiaGenerator);
    printItem("RunPythiaReader",runPythiaReader);
    printItem("RunHerwigGenerator",runHerwigGenerator);
    printItem("RunHerwigReader",runHerwigReader);
    printItem("RunAmptGenerator",runAmptGenerator);
    printItem("RunAmptReader",runAmptReader);
    printItem("RunEposGenerator",runEposGenerator);
    printItem("RunEposReader",runEposReader);
    printItem("RunUrqmdReader",runUrqmdReader);
    printItem("RunTherminatorGenerator",runTherminatorGenerator);
    printItem("RunHijingReader",runHijingReader);
    printItem("RunResonanceGenerator",runResonanceGenerator);
    printItem("RunGlobalAnalysisGen",runGlobalAnalysisGen);
    printItem("RunGlobalAnalysisReco",runGlobalAnalysisReco);
    printItem("RunSpherocityAnalysisGen",runSpherocityAnalysisGen);
    printItem("RunSpherocityAnalysisReco",runSpherocityAnalysisReco);
    printItem("RunPartSingleAnalysisGen",runPartSingleAnalysisGen);
    printItem("RunPartSingleAnalysisReco",runPartSingleAnalysisReco);
    printItem("RunPartPairAnalysisGen",runPartPairAnalysisGen);
    printItem("RunPartPairAnalysisReco",runPartPairAnalysisReco);
    printItem("RunNuDynAnalysisGen",runNuDynAnalysisGen);
    printItem("RunNuDynAnalysisReco",runNuDynAnalysisReco);
    printItem("RunFillEta",runFillEta);
    printItem("RunFillY",runFillY);
    printItem("HistogramsImportPath",inputPathName);
    printItem("HistogramsExportPath",outputPathName);
    printItem("PartFilterModelOption",partFilterModelOption);
    printItem("PartFilterModelPt",partFilterModelPt);
    printItem("PartFilterModelMinPt",partFilterModelMinPt);
    printItem("PartFilterModelMaxPt",partFilterModelMaxPt);
    printItem("PartFilterModelEta",partFilterModelEta);
    printItem("PartFilterModelMinEta",partFilterModelMinEta);
    printItem("PartFilterModelMaxEta",partFilterModelMaxEta);
    printItem("PartFilterModelY",partFilterModelY);
    printItem("PartFilterModelMinY",partFilterModelMinY);
    printItem("PartFilterModelMaxY",partFilterModelMaxY);
    printItem("PartFilterAnaOption",partFilterAnaOption);
    printItem("PartFilterAnaFilterPt",partFilterAnaFilterPt);
    printItem("PartFilterAnaMinPt",partFilterAnaMinPt);
    printItem("PartFilterAnaMaxPt",partFilterAnaMaxPt);
    printItem("PartFilterAnaEta",partFilterAnaEta);
    printItem("PartFilterAnaMinEta",partFilterAnaMinEta);
    printItem("PartFilterAnaMaxEta",partFilterAnaMaxEta);
    printItem("PartFilterAnaY",partFilterAnaY);
    printItem("PartFilterAnaMinY",partFilterAnaMinY);
    printItem("PartFilterAnaMaxY",partFilterAnaMaxY);
    printItem("SetSeed",setSeed);
    printItem("SeedValue",seedValue);
    cout << endl;
    }

  vector<EventFilter*>     eventFilterModels;
  vector<EventFilter*>     eventFilterAnalysis;
  vector<ParticleFilter*>  particleFiltersModels;
  vector<ParticleFilter*>  particleFiltersAnalysis;

  // =========================================
  // Setup all event filters
  // =========================================
  vector<double> modelBounds;
  String eventFilterModelOption = getValueString("EventFilterModelOption");
  int n = getValueInt("EventFilterModelNValues");
  for (int k=0; k<n; k++)
    {
    String key = "EventFilterModelValue";
    key += k;
    modelBounds.push_back( getValueBool(key));
    }
  if (eventFilterModelOption.EqualTo("All"))                   eventFilterModels = EventFilter::createOpenEventFilter();
  else if (eventFilterModelOption.EqualTo("AliceMB"))          eventFilterModels = EventFilter::createAliceMBEventFilter();
  else if (eventFilterModelOption.EqualTo("ImpactParameter"))  eventFilterModels = EventFilter::createImpactParameterFilters(modelBounds);
  else if (eventFilterModelOption.EqualTo("V0Mult"))           eventFilterModels = EventFilter::createV0MultiplicityFilters(modelBounds);
  else if (eventFilterModelOption.EqualTo("TpcMult"))          eventFilterModels = EventFilter::createTpcMultiplicityFilters(modelBounds);

  vector<double> anaBounds;
  String eventFilterAnaOption = getValueString("EventFilterAnaOption");
  n = getValueInt("EventFilterAnaNValues");
  for (int k=0; k<n; k++)
    {
    String key = "EventFilterAnaValue";
    key += k;
    anaBounds.push_back( getValueBool(key));
    }
  if (eventFilterAnaOption.EqualTo("All"))                   eventFilterAnalysis = EventFilter::createOpenEventFilter();
  else if (eventFilterAnaOption.EqualTo("AliceMB"))          eventFilterAnalysis = EventFilter::createAliceMBEventFilter();
  else if (eventFilterAnaOption.EqualTo("ImpactParameter"))  eventFilterAnalysis = EventFilter::createImpactParameterFilters(anaBounds);
  else if (eventFilterAnaOption.EqualTo("V0Mult"))           eventFilterAnalysis = EventFilter::createV0MultiplicityFilters(anaBounds);
  else if (eventFilterAnaOption.EqualTo("TpcMult"))          eventFilterAnalysis = EventFilter::createTpcMultiplicityFilters(anaBounds);

  if (eventFilterAnalysis.size()<1)
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
  // Setup all particle filter
  // =========================================
  if (partFilterModelOption.EqualTo("All"))                   particleFiltersModels = ParticleFilter::createOpenParticleFilter();
  else if (partFilterModelOption.EqualTo("AliceV0"))          particleFiltersModels = ParticleFilter::createAliceV0Filter();
  else if (partFilterModelOption.EqualTo("Neutral"))          particleFiltersModels = ParticleFilter::createNeutralParticleFilter(partFilterModelPt,partFilterModelMinPt,partFilterModelMaxPt,partFilterModelEta,partFilterModelMinEta,partFilterModelMaxEta,partFilterModelY,partFilterModelMinY,partFilterModelMaxY);
  else if (partFilterModelOption.EqualTo("Charged"))          particleFiltersModels = ParticleFilter::createChargedParticleFilter(partFilterModelPt,partFilterModelMinPt,partFilterModelMaxPt,partFilterModelEta,partFilterModelMinEta,partFilterModelMaxEta,partFilterModelY,partFilterModelMinY,partFilterModelMaxY);
  else if (partFilterModelOption.EqualTo("Negative"))         particleFiltersModels = ParticleFilter::createNegativeParticleFilter(partFilterModelPt,partFilterModelMinPt,partFilterModelMaxPt,partFilterModelEta,partFilterModelMinEta,partFilterModelMaxEta,partFilterModelY,partFilterModelMinY,partFilterModelMaxY);
  else if (partFilterModelOption.EqualTo("Positive"))         particleFiltersModels = ParticleFilter::createPositiveParticleFilter(partFilterModelPt,partFilterModelMinPt,partFilterModelMaxPt,partFilterModelEta,partFilterModelMinEta,partFilterModelMaxEta,partFilterModelY,partFilterModelMinY,partFilterModelMaxY);
  else if (partFilterModelOption.EqualTo("ChargedHadrons"))   particleFiltersModels = ParticleFilter::createChargedHadronFilters(partFilterModelPt,partFilterModelMinPt,partFilterModelMaxPt,partFilterModelEta,partFilterModelMinEta,partFilterModelMaxEta,partFilterModelY,partFilterModelMinY,partFilterModelMaxY);
  else if (partFilterModelOption.EqualTo("PlusMinusHadrons")) particleFiltersModels = ParticleFilter::createPlusMinusHadronFilters(partFilterModelPt,partFilterModelMinPt,partFilterModelMaxPt,partFilterModelEta,partFilterModelMinEta,partFilterModelMaxEta,partFilterModelY,partFilterModelMinY,partFilterModelMaxY);
  else if (partFilterModelOption.EqualTo("StrangeHadrons"))   particleFiltersModels = ParticleFilter::createStrangeHadronFilters(partFilterModelPt,partFilterModelMinPt,partFilterModelMaxPt,partFilterModelEta,partFilterModelMinEta,partFilterModelMaxEta,partFilterModelY,partFilterModelMinY,partFilterModelMaxY);
  else if (partFilterModelOption.EqualTo("Baryons"))          particleFiltersModels = ParticleFilter::createBaryonFilters(partFilterModelPt,partFilterModelMinPt,partFilterModelMaxPt,partFilterModelEta,partFilterModelMinEta,partFilterModelMaxEta,partFilterModelY,partFilterModelMinY,partFilterModelMaxY);

  if (partFilterAnaOption.EqualTo("All"))                   particleFiltersAnalysis = ParticleFilter::createOpenParticleFilter();
  else if (partFilterAnaOption.EqualTo("AliceV0"))          particleFiltersAnalysis = ParticleFilter::createAliceV0Filter();
  else if (partFilterAnaOption.EqualTo("Neutral"))          particleFiltersAnalysis = ParticleFilter::createNeutralParticleFilter(partFilterAnaFilterPt,partFilterAnaMinPt,partFilterAnaMaxPt,partFilterAnaEta,partFilterAnaMinEta,partFilterAnaMaxEta,partFilterAnaY,partFilterAnaMinY,partFilterAnaMaxY);
  else if (partFilterAnaOption.EqualTo("Charged"))          particleFiltersAnalysis = ParticleFilter::createChargedParticleFilter(partFilterAnaFilterPt,partFilterAnaMinPt,partFilterAnaMaxPt,partFilterAnaEta,partFilterAnaMinEta,partFilterAnaMaxEta,partFilterAnaY,partFilterAnaMinY,partFilterAnaMaxY);
  else if (partFilterAnaOption.EqualTo("Negative"))         particleFiltersAnalysis = ParticleFilter::createNegativeParticleFilter(partFilterAnaFilterPt,partFilterAnaMinPt,partFilterAnaMaxPt,partFilterAnaEta,partFilterAnaMinEta,partFilterAnaMaxEta,partFilterAnaY,partFilterAnaMinY,partFilterAnaMaxY);
  else if (partFilterAnaOption.EqualTo("Positive"))         particleFiltersAnalysis = ParticleFilter::createPositiveParticleFilter(partFilterAnaFilterPt,partFilterAnaMinPt,partFilterAnaMaxPt,partFilterAnaEta,partFilterAnaMinEta,partFilterAnaMaxEta,partFilterAnaY,partFilterAnaMinY,partFilterAnaMaxY);
  else if (partFilterAnaOption.EqualTo("ChargedHadrons"))   particleFiltersAnalysis = ParticleFilter::createChargedHadronFilters(partFilterAnaFilterPt,partFilterAnaMinPt,partFilterAnaMaxPt,partFilterAnaEta,partFilterAnaMinEta,partFilterAnaMaxEta,partFilterAnaY,partFilterAnaMinY,partFilterAnaMaxY);
  else if (partFilterAnaOption.EqualTo("PlusMinusHadrons")) particleFiltersAnalysis = ParticleFilter::createPlusMinusHadronFilters(partFilterAnaFilterPt,partFilterAnaMinPt,partFilterAnaMaxPt,partFilterAnaEta,partFilterAnaMinEta,partFilterAnaMaxEta,partFilterAnaY,partFilterAnaMinY,partFilterAnaMaxY);
  else if (partFilterAnaOption.EqualTo("StrangeHadrons"))   particleFiltersAnalysis = ParticleFilter::createStrangeHadronFilters(partFilterAnaFilterPt,partFilterAnaMinPt,partFilterAnaMaxPt,partFilterAnaEta,partFilterAnaMinEta,partFilterAnaMaxEta,partFilterAnaY,partFilterAnaMinY,partFilterAnaMaxY);
  else if (partFilterAnaOption.EqualTo("Baryons"))          particleFiltersAnalysis = ParticleFilter::createBaryonFilters(partFilterAnaFilterPt,partFilterAnaMinPt,partFilterAnaMaxPt,partFilterAnaEta,partFilterAnaMinEta,partFilterAnaMaxEta,partFilterAnaY,partFilterAnaMinY,partFilterAnaMaxY);

  if (particleFiltersAnalysis.size()<1)
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
    cout << "  EventFilterModels.size().............: " << eventFilterModels.size() << endl;
    for (unsigned int k=0; k<eventFilterModels.size(); k++)
      cout << "    " << k << "   " << eventFilterModels[k]->getName() << endl;
    cout << "  particleFiltersModels.size()..........: " << particleFiltersModels.size() << endl;
    for (unsigned int k=0; k<particleFiltersModels.size(); k++)
      cout << "    " << k << "   " << particleFiltersModels[k]->getName() << endl;
    cout << "  eventFilterAnalysis.size()..........: " << eventFilterAnalysis.size() << endl;
    for (unsigned int k=0; k<eventFilterAnalysis.size(); k++)
      cout << "    " << k << "   " << eventFilterAnalysis[k]->getName() << endl;
    cout << "  particleFiltersAnalysis.size().......: " << particleFiltersAnalysis.size() << endl;
    for (unsigned int k=0; k<particleFiltersAnalysis.size(); k++)
      cout << "    " << k << "   " << particleFiltersAnalysis[k]->getName() << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Configuration Completed - Proceed with loading analysis" << std::endl;
    cout << "==================================================================================" << std::endl;
    }

  if (runParticleDbManager)
    {
    ParticleDbManager * particleDbManager = new ParticleDbManager("ParticleDb",*requestedConfiguration);
    addSubTask(particleDbManager);
    }


  if (runEventAnalysis)
    {
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << "==================================================================================" << std::endl;
      cout << "Setting up event analysis" << std::endl;
      cout << "==================================================================================" << std::endl;
      }
    eventAnalysis = new TaskIterator("Analysis",*requestedConfiguration);
    addSubTask(eventAnalysis);

    //if (runPythiaReader)         eventAnalysis->addSubTask(new PythiaEventReader(PythiaLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));
    if (runPythiaGenerator)      eventAnalysis->addSubTask(new PythiaEventGenerator(PythiaLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));
//    if (runHerwigReader)         eventAnalysis->addSubTask(new HerwigEventReader(HerwigLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));
    if (runAmptReader)           eventAnalysis->addSubTask(new AmptEventReader(AmptLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));
//    if (runEposReader)           eventAnalysis->addSubTask(new EposEventReader(EposLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));
//    if (runUrqmdReader)          eventAnalysis->addSubTask(new UrqmdEventReader(UrqmdLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));
//    if (runHijingReader)         eventAnalysis->addSubTask(new HijingEventReader(HijingLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));
    if (runTherminatorGenerator) eventAnalysis->addSubTask(new TherminatorGenerator(TherminatorLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));
    if (runResonanceGenerator)   eventAnalysis->addSubTask(new ResonanceGenerator(ResoLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));

    if (runPerformanceSim)       eventAnalysis->addSubTask(new MeasurementPerformanceSimulator(PerformLabel,*requestedConfiguration,eventFilterModels,particleFiltersModels));

    if (runGlobalAnalysisGen)       eventAnalysis->addSubTask(new GlobalAnalyzer(GlobalLabel+GenLabel, *requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    if (runSpherocityAnalysisGen)   eventAnalysis->addSubTask(new TransverseSpherocityAnalyzer(SpherocityLabel+GenLabel, *requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    if (runPartSingleAnalysisGen)   eventAnalysis->addSubTask(new ParticleSingleAnalyzer(PartLabel+GenLabel, *requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    if (runPartPairAnalysisGen)     eventAnalysis->addSubTask(new ParticlePairAnalyzer(PairLabel+GenLabel, *requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    if (runNuDynAnalysisGen)        eventAnalysis->addSubTask(new NuDynAnalyzer(NuDynLabel+GenLabel,*requestedConfiguration,eventFilterAnalysis,particleFiltersAnalysis));

    if (runGlobalAnalysisReco)      eventAnalysis->addSubTask(new GlobalAnalyzer(GlobalLabel+RecoLabel,*requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    if (runSpherocityAnalysisReco)  eventAnalysis->addSubTask(new TransverseSpherocityAnalyzer(SpherocityLabel+RecoLabel, *requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    if (runPartSingleAnalysisReco)  eventAnalysis->addSubTask(new ParticleSingleAnalyzer(PartLabel+RecoLabel, *requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    if (runPartPairAnalysisReco)    eventAnalysis->addSubTask(new ParticlePairAnalyzer(PairLabel+RecoLabel, *requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    if (runNuDynAnalysisReco)       eventAnalysis->addSubTask(new NuDynAnalyzer(NuDynLabel+RecoLabel,*requestedConfiguration,eventFilterAnalysis,particleFiltersAnalysis));
    if (runPerformanceAna)          eventAnalysis->addSubTask(new ParticlePerformanceAnalyzer(SimAnaLabel,*requestedConfiguration,eventFilterAnalysis, particleFiltersAnalysis));
    }

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Configuration Completed; Run Analysis" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
  gSystem->mkdir(outputPathName,1);
  if (hasSubTasks())
    for (unsigned int  iTask=0; iTask<subTasks.size(); iTask++)
      {
      subTasks[iTask]->configure();
      }
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

