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
#include "RunDerivedCalculation.hpp"
#include "TaskIterator.hpp"
#include "DerivedHistoIterator.hpp"
#include "BalanceFunctionCalculator.hpp"
#include "SubSampleStatCalculator.hpp"
#include "ClosureIterator.hpp"
#include "ParticleDbManager.hpp"
//#include "HerwigEventReader.hpp"
#include "AmptEventReader.hpp"
//#include "EposEventReader.hpp"
//#include "UrqmdEventReader.hpp"
//#include "HijingEventReader.hpp"
#include "ResonanceGenerator.hpp"
#include "MeasurementPerformanceSimulator.hpp"
#include "ParticlePerformanceAnalyzer.hpp"
#include "GlobalAnalyzer.hpp"
#include "TransverseSpherocityAnalyzer.hpp"
#include "ParticleSingleAnalyzer.hpp"
#include "ParticlePairAnalyzer.hpp"
#include "NuDynAnalyzer.hpp"
using CAP::RunDerivedCalculation;

ClassImp(RunDerivedCalculation);

RunDerivedCalculation::RunDerivedCalculation(const String & _name,
                                             const Configuration & _configuration)
:
EventTask(_name, _configuration)
{
  appendClassName("RunDerivedCalculation");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}


void RunDerivedCalculation::setDefaultConfiguration()
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
  addParameter("DerivedLabel",    TString("Derived"));
  addParameter("SumLabel",        TString("Sum"));
  addParameter("BalFctLabel",     TString("BalFct"));
  addParameter("GenLabel",        TString("Gen"));
  addParameter("RecoLabel",       TString("Reco"));
  addParameter("Severity",                TString("Info"));
  addParameter("Derived",                 YES);
  addParameter("BalFct",                  YES);
  addParameter("RunGlobalAnalysisGen",               NO);
  addParameter("RunGlobalAnalysisReco",              NO);
  addParameter("RunSpherocityAnalysisGen",           NO);
  addParameter("RunSpherocityAnalysisReco",          NO);
  addParameter("RunPartSingleAnalysisGen",                 NO);
  addParameter("RunPartSingleAnalysisReco",                NO);
  addParameter("RunPartPairAnalysisReco",       NO);
  addParameter("RunPartPairAnalysisReco",       NO);
  addParameter("RunNuDynAnalysisGen",           NO);
  addParameter("RunNuDynAnalysisReco",          NO);
  addParameter("RunFillEta",                     NO);
  addParameter("RunFillY",                   YES);
  addParameter("HistogramsForceRewrite",  YES);
  addParameter("HistogramsImportPath",      TString("Input"));
  addParameter("HistogramsExportPath",     TString("Output"));

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


void RunDerivedCalculation::configure()
{
  if (reportEnd(__FUNCTION__))
;
  EventTask::configure();

  // assemble the task from here...
  DerivedHistoIterator      * derived   = nullptr;
  //BalanceFunctionCalculator * balFct    = nullptr;
  //MessageLogger::Severity selectedLevel = MessageLogger::Debug;
  String GlobalLabel      = getValueString("GlobalLabel");
  String SpherocityLabel  = getValueString("SpherocityLabel");
  String PartLabel        = getValueString("PartLabel");
  String PairLabel        = getValueString("PairLabel");
  String NuDynLabel       = getValueString("NuDynLabel");
  String SimAnaLabel      = getValueString("SimAnaLabel");
  String DerivedLabel     = getValueString("DerivedLabel");
  String SumLabel         = getValueString("SumLabel");
  String BalFctLabel      = getValueString("BalFctLabel");
  String GenLabel         = getValueString("GenLabel");
  String RecoLabel        = getValueString("RecoLabel");
  bool    runDerived                      = getValueBool("Derived");
  bool    runBalFct                       = getValueBool("RunBalFct");
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
  String modelPartFilterOption            = getValueString("ModelPartFilterOption");
  double modelPartFilterPt                = getValueBool(  "ModelPartFilterPt");
  double modelPartMinPt                   = getValueDouble("ModelPartMinPt");
  double modelPartMaxPt                   = getValueDouble("ModelPartMaxPt");
  bool   modelPartFilterEta               = getValueBool(  "ModelPartFilterEta");
  double modelPartMinEta                  = getValueDouble("ModelPartMinEta");
  double modelPartMaxEta                  = getValueDouble("ModelPartMaxEta");
  bool   modelPartFilterY                 = getValueBool(  "ModelPartFilterY");
  double modelPartMinY                    = getValueDouble("ModelPartMinY");
  double modelPartMaxY                    = getValueDouble("ModelPartMaxY");
  String anaPartFilterOption              = getValueString("AnaPartFilterOption");
  double anaPartFilterPt                  = getValueBool(  "AnaPartFilterPt");
  double anaPartMinPt                     = getValueDouble("AnaPartMinPt");
  double anaPartMaxPt                     = getValueDouble("AnaPartMaxPt");
  bool   anaPartFilterEta                 = getValueBool(  "AnaPartFilterEta");
  double anaPartMinEta                    = getValueDouble("AnaPartMinEta");
  double anaPartMaxEta                    = getValueDouble("AnaPartMaxEta");
  bool   anaPartFilterY                   = getValueBool(  "AnaPartFilterY");
  double anaPartMinY                      = getValueDouble("AnaPartMinY");
  double anaPartMaxY                      = getValueDouble("AnaPartMaxY");

  if (reportDebug(__FUNCTION__)) printConfiguration(cout);

//  if (reportInfo(__FUNCTION__))
//    {
//    cout << endl;
//    cout << "GlobalLabel..................................:" << GlobalLabel      << endl;
//    cout << "SpherocityLabel..............................:" << SpherocityLabel  << endl;
//    cout << "PartLabel....................................:" << PartLabel        << endl;
//    cout << "PairLabel....................................:" << PairLabel        << endl;
//    cout << "NuDynLabel...................................:" << NuDynLabel       << endl;
//    cout << "SimAnaLabel..................................:" << SimAnaLabel      << endl;
//    cout << "DerivedLabel.................................:" << DerivedLabel     << endl;
//    cout << "SumLabel.....................................:" << SumLabel         << endl;
//    cout << "BalFctLabel..................................:" << BalFctLabel      << endl;
//    cout << "GenLabel.....................................:" << GenLabel         << endl;
//    cout << "RecoLabel....................................:" << RecoLabel        << endl;
//    cout << "Derived......................................:" << runDerived       << endl;
//    cout << "BalFct.......................................:" << runBalFct        << endl;
//    cout << "RunGlobalAnalysisGen.........................:" << runGlobalAnalysisGen         << endl;
//    cout << "RunGlobalAnalysisReco........................:" << runGlobalAnalysisReco        << endl;
//    cout << "RunSpherocityAnalysisGen.....................:" << runSpherocityAnalysisGen     << endl;
//    cout << "RunSpherocityAnalysisReco....................:" << runSpherocityAnalysisGen     << endl;
//    cout << "RunPartSingleAnalysisGen.....................:" << runPartSingleAnalysisGen     << endl;
//    cout << "RunPartSingleAnalysisReco....................:" << runPartSingleAnalysisReco    << endl;
//    cout << "RunPartPairAnalysisGen.......................:" << runPartPairAnalysisGen       << endl;
//    cout << "RunPartPairAnalysisReco......................:" << runPartPairAnalysisReco      << endl;
//    cout << "RunNuDynAnalysisGen..........................:" << runNuDynAnalysisGen          << endl;
//    cout << "RunNuDynAnalysisReco.........................:" << runNuDynAnalysisReco         << endl;
//    cout << "FillEta......................................:" << runFillEta            << endl;
//    cout << "FillY........................................:" << runFillY              << endl;
//    cout << "HistogramsImportPath.........................:" << inputPathName         << endl;
//    cout << "HistogramsExportPath.........................:" << outputPathName        << endl;
//    cout << "ModelPartMinY................................:" << modelPartMinY         << endl;
//    cout << "ModelPartMaxY................................:" << modelPartMaxY         << endl;
//    cout << "ModelPartFilterOption........................:" << modelPartFilterOption << endl;
//    cout << "ModelPartFilterPt............................:" << modelPartFilterPt   << endl;
//    cout << "ModelPartMinPt...............................:" << modelPartMinPt      << endl;
//    cout << "ModelPartMaxPt...............................:" << modelPartMaxPt      << endl;
//    cout << "ModelPartFilterEta...........................:" << modelPartFilterEta  << endl;
//    cout << "ModelPartMinEta..............................:" << modelPartMinEta     << endl;
//    cout << "ModelPartMaxEta..............................:" << modelPartMaxEta     << endl;
//    cout << "ModelPartFilterY.............................:" << modelPartFilterY    << endl;
//    cout << "ModelPartMinY................................:" << modelPartMinY       << endl;
//    cout << "ModelPartMaxY................................:" << modelPartMaxY       << endl;
//    cout << "AnaPartFilterOption..........................:" << anaPartFilterOption << endl;
//    cout << "AnaPartFilterPt..............................:" << anaPartFilterPt     << endl;
//    cout << "AnaPartMinPt.................................:" << anaPartMinPt        << endl;
//    cout << "AnaPartMaxPt.................................:" << anaPartMaxPt        << endl;
//    cout << "AnaPartFilterEta.............................:" << anaPartFilterEta    << endl;
//    cout << "AnaPartMinEta................................:" << anaPartMinEta       << endl;
//    cout << "AnaPartMaxEta................................:" << anaPartMaxEta       << endl;
//    cout << "AnaPartFilterY...............................:" << anaPartFilterY      << endl;
//    cout << "AnaPartMinY..................................:" << anaPartMinY         << endl;
//    cout << "AnaPartMaxY..................................:" << anaPartMaxY         << endl;
//    }
  vector<EventFilter*> modelEventFilters;
  vector<EventFilter*> analysisEventFilters;
  vector<ParticleFilter*>  modelParticleFilters;
  vector<ParticleFilter*>  analysisParticleFilters;

  // =========================================
  // Setup all event filters
  // =========================================
  vector<double> modelBounds;
  String modelEventFilterOption = getValueString("ModelEventFilterOption");
  int n = getValueInt("ModelEventFilterNValues");
  for (int k=0; k<n; k++)
    {
    String key = "ModelEventFilterValue";
    key += k;
    modelBounds.push_back( getValueBool(key));
    }
  if (modelEventFilterOption.EqualTo("All"))                  modelEventFilters = EventFilter::createOpenEventFilter();
  else if (modelEventFilterOption.EqualTo("AliceMB"))          modelEventFilters = EventFilter::createAliceMBEventFilter();
  else if (modelEventFilterOption.EqualTo("ImpactParameter"))  modelEventFilters = EventFilter::createImpactParameterFilters(modelBounds);
  else if (modelEventFilterOption.EqualTo("V0Mult"))           modelEventFilters = EventFilter::createV0MultiplicityFilters(modelBounds);
  else if (modelEventFilterOption.EqualTo("TpcMult"))          modelEventFilters = EventFilter::createTpcMultiplicityFilters(modelBounds);

  vector<double> anaBounds;
  String anaEventFilterOption = getValueString("AnaEventFilterOption");
  n = getValueInt("AnaEventFilterNValues");
  for (int k=0; k<n; k++)
    {
    String key = "AnaEventFilterValue";
    key += k;
    anaBounds.push_back( getValueBool(key));
    }
  if (anaEventFilterOption.EqualTo("All"))                   analysisEventFilters = EventFilter::createOpenEventFilter();
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
    cout << "  RunDerivedCalculation:Filters" << endl;
    cout << "  modelEventFilters.size().............: " << modelEventFilters.size() << endl;
    for (unsigned int k=0; k<modelEventFilters.size(); k++)
      cout << "    " << k << "   " << modelEventFilters[k]->getName() << endl;
    cout << "  modelParticleFilters.size()..........: " << modelParticleFilters.size() << endl;
    for (unsigned int k=0; k<modelParticleFilters.size(); k++)
      cout << "    " << k << "   " << modelParticleFilters[k]->getName() << endl;
    cout << "  analysisEventFilters.size()..........: " << analysisEventFilters.size() << endl;
    for (unsigned int k=0; k<analysisEventFilters.size(); k++)
      cout << "    " << k << "   " << analysisEventFilters[k]->getName() << endl;
    cout << "  analysisParticleFilters.size().......: " << analysisParticleFilters.size() << endl;
    for (unsigned int k=0; k<analysisParticleFilters.size(); k++)
      cout << "    " << k << "   " << analysisParticleFilters[k]->getName() << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Configuration Completed - Proceed with loading analysis" << std::endl;
    cout << "==================================================================================" << std::endl;
    }

  if (runDerived)
    {
    derived = new DerivedHistoIterator("Analysis", configuration);
    addSubTask(derived);
    if (runGlobalAnalysisGen)          derived->addSubTask(new GlobalAnalyzer(GlobalLabel+GenLabel, configuration,analysisEventFilters, analysisParticleFilters));
    if (runSpherocityAnalysisGen)      derived->addSubTask(new TransverseSpherocityAnalyzer(SpherocityLabel+GenLabel, configuration,analysisEventFilters, analysisParticleFilters));
    if (runPartSingleAnalysisGen)      derived->addSubTask(new ParticleSingleAnalyzer(PartLabel+GenLabel, configuration,analysisEventFilters, analysisParticleFilters));
    if (runPartPairAnalysisGen)        derived->addSubTask(new ParticlePairAnalyzer(PairLabel+GenLabel, configuration,analysisEventFilters, analysisParticleFilters));
    if (runNuDynAnalysisGen)           derived->addSubTask(new NuDynAnalyzer(NuDynLabel+GenLabel,configuration,analysisEventFilters,analysisParticleFilters));

    if (runGlobalAnalysisReco)         derived->addSubTask(new GlobalAnalyzer(GlobalLabel+RecoLabel, configuration,analysisEventFilters, analysisParticleFilters));
    if (runSpherocityAnalysisReco)     derived->addSubTask(new TransverseSpherocityAnalyzer(SpherocityLabel+RecoLabel, configuration,analysisEventFilters, analysisParticleFilters));
    if (runPartSingleAnalysisReco)     derived->addSubTask(new ParticleSingleAnalyzer(PartLabel+RecoLabel, configuration,analysisEventFilters, analysisParticleFilters));
    if (runPartPairAnalysisReco)       derived->addSubTask(new ParticlePairAnalyzer(PairLabel+RecoLabel, configuration,analysisEventFilters, analysisParticleFilters));
    if (runNuDynAnalysisReco)          derived->addSubTask(new NuDynAnalyzer(NuDynLabel+RecoLabel,configuration,analysisEventFilters,analysisParticleFilters));
    }

  if (runBalFct && runPartPairAnalysisGen)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramsImportPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":HistogramsExportPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":IncludedPattern0"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":IncludedPattern1"),TString("Pair"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern1"),TString("Reco"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PairLabel+GenLabel+TString(":ExcludedPattern3"),TString("Part"));
    addSubTask(new BalanceFunctionCalculator("RunPartPairAnalysisReco",subConfig,analysisEventFilters, analysisParticleFilters));
    }

  if (runBalFct && runPartPairAnalysisReco)
    {
    Configuration & subConfig = * new Configuration();
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramsImportPath"),inputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":HistogramsExportPath"),outputPathName);
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":IncludedPattern0"),TString("Derived"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":IncludedPattern1"),TString("Pair"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern1"),TString("Gen"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern2"),TString("BalFct"));
    subConfig.addParameter(TString("Run:")+PairLabel+RecoLabel+TString(":ExcludedPattern3"),TString("Part"));
    addSubTask(new BalanceFunctionCalculator("RunPartPairAnalysisReco",subConfig,analysisEventFilters, analysisParticleFilters));
    }

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "==================================================================================" << std::endl;
    cout << "Configuration Completed; Run Analysis" << std::endl;
    cout << "==================================================================================" << std::endl;
    }
  gSystem->mkdir(outputPathName,1);


  if (hasSubTasks() && isTaskOk())
    for (unsigned int  iTask=0; iTask<subTasks.size(); iTask++)
      {
      subTasks[iTask]->configure();
      }

  if (reportEnd(__FUNCTION__))
;
}

void RunDerivedCalculation::execute()
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

