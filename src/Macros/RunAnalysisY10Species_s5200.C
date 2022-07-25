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
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>

void loadBase(const TString & includeBasePath);
void loadPythia(const TString & includeBasePath);
void loadPerformance(const TString & includeBasePath);
void loadAmpt(const TString & includeBasePath);
void loadEpos(const TString & includeBasePath);
void loadBasicGen(const TString & includeBasePath);
void loadGlobal(const TString & includeBasePath);
void loadParticle(const TString & includeBasePath);
void loadPair(const TString & includeBasePath);
void loadNuDyn(const TString & includeBasePath);
void loadSubSample(const TString & includeBasePath);


int RunAnalysisY10Species_s5200()
{
  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  loadPythia(includeBasePath);
  loadPerformance(includeBasePath);
  loadAmpt(includeBasePath);
  loadEpos(includeBasePath);
  loadBasicGen(includeBasePath);
  loadGlobal(includeBasePath);
  loadParticle(includeBasePath);
  loadPair(includeBasePath);
  loadNuDyn(includeBasePath);
  loadSubSample(includeBasePath);

  std::cout << "==================================================================================" << std::endl;
  std::cout << "MC Model Analysis" << endl;
  std::cout << "==================================================================================" << std::endl;

  
  bool YES = true;
  bool NO  = false;

  TString modelLabel;
  TString systemLabel;
  TString energyLabel;
  TString extraLabel;

  TString globalLabel     = "G";
  TString spherocityLabel = "S";
  TString partLabel       = "Part";
  TString pairLabel       = "Pair";
  TString nuDynLabel      = "NuDyn";
  TString simAnaLabel     = "SimAna";
  TString rootExt         = ".root";
  TString derivedLabel    = "_Derived";
  TString sumLabel        = "_Sum";
  TString balFctLabel     = "_BalFct";
  TString closureLabel    = "_Closure";
  TString genLabel        = "_Gen";
  TString recoLabel       = "_Reco";

  TString histoModelDataName;
  TString histoAnalyzerName;
  TString histoBaseName;

  MessageLogger::LogLevel infoLevel  = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Debug;
  MessageLogger::LogLevel selectedLevel = infoLevel;
  
  bool    runEventAnalysis               = YES;
  bool    runGenLevelAnalysis            = YES;
  bool    runRecoLevelAnalysis           = NO;
  bool    runDerivedHistoCalculation     = YES;
  bool    runBalFctCalculation           = YES;
  bool    runSubsampleAnalysis           = YES;
  bool    runBasicSubsampleAnalysis      = NO;
  bool    runDerivedSubsampleAnalysis    = YES;
  bool    runBalFctSubsampleAnalysis     = YES;
  bool    runPerformSimulator            = NO;
  bool    runPerformAnalysis             = NO;
  bool    runClosureAnalysis             = NO;
  bool    runBasicClosureAnalysis        = NO;
  bool    runDerivedClosureAnalysis      = NO;
  bool    runBalFctClosureAnalysis       = NO;

  bool    runPythiaGenerator             = YES;
  bool    runHerwigGenerator             = NO;
  bool    runAmptReader                  = NO;
  bool    runEposGenerator               = NO;
  bool    runUrqmdGenerator              = NO;
  bool    runHijingGenerator             = NO;
  bool    runGausResGenerator            = NO;
  bool    runGlobalAnalysis              = YES;
  bool    runSpherocityAnalysis          = NO;
  bool    runPartAnalysis                = YES;
  bool    runPairAnalysis                = YES;
  bool    runNuDynAnalysis               = NO;
  bool    loadPdgTable                   = YES;
  bool    fillEta                        = NO;
  bool    fillY                          = YES;

  long    nIterationRequested            = 10000000;
  long    nIterationReported             = 100000;
  long    nIterationPartialSave          = 1000000;
  bool    doPartialReports               = YES;
  bool    doPartialSaves                 = YES;
  bool    forceHistogramsRewrite         = YES;
  bool    scaleHistograms                = YES;

  int     globalEventFilterOptions       = 0;
  int     analysisParticleFilterOption   = 5;
  int     analysisEventFilterOptions     = 0;


  TString inputPathBase   = "/Volumes/ClaudeDisc4/OutputFiles/";
  TString outputPathBase  = "/Volumes/ClaudeDisc4/OutputFiles/";
  TString pathSuffix      = "Y10WidePt/Species/";

  // TString input FileNameBase;
  TString outputFileNameBase;
  TString inputPathName   = inputPathBase;
  TString outputPathName  = outputPathBase;

  // =========================================
  // Setup all event filters
  // =========================================
  vector<EventFilter*> generatorEventFilters;
  vector<EventFilter*> analysisEventFilters;

  EventFilter * openEventFilter  = new EventFilter();
  openEventFilter->setName("All");
  openEventFilter->setLongName("All");

  EventFilter * aliceMinBiasEventFilter  = new EventFilter();
  aliceMinBiasEventFilter->setName("V0MB");
  aliceMinBiasEventFilter->setLongName("V0MinBias");
  aliceMinBiasEventFilter->addCondition(1, 0, 1.0, 1.0E10); // v0 multiplicity
  aliceMinBiasEventFilter->addCondition(1, 1, 1.0, 1.0E10); // TPC multiplicity

  switch (analysisEventFilterOptions)
    {
      default:
      case 0: analysisEventFilters.push_back( openEventFilter); break;
      case 1: analysisEventFilters.push_back( aliceMinBiasEventFilter); break;
      case 2:
      EventFilter * f;
      f = new EventFilter();
      f->setName("V0M1To10");
      f->setLongName("V0M1To10");
      f->addCondition(1, 0, 1.0, 10.0); // v0 multiplicity
      f->addCondition(1, 1, 1.0, 1.0E10); // TPC multiplicity
      analysisEventFilters.push_back( f );
      f = new EventFilter();
      f->setName("V0M11To500");
      f->setLongName("V0M11To500");
      f->addCondition(1, 0, 11.0, 500.0); // v0 multiplicity
      f->addCondition(1, 1, 1.0, 1.0E10); // TPC multiplicity
      analysisEventFilters.push_back( f );
    }

  // =========================================
  // Setup all particle filters
  // =========================================

  vector<ParticleFilter*>  generatorParticleFilters;
  vector<ParticleFilter*>  analysisParticleFilters;

  ParticleFilter* openParticleFilter = new ParticleFilter(); // no conditions -- accepts all
  openParticleFilter->setName("All");
  openParticleFilter->setLongName("All");
  openParticleFilter->setTitle("All");
  openParticleFilter->setLongTitle("All");
  openParticleFilter->addCondition(0, 1,  0.0, 0.0);  // live particles only

  ParticleFilter* chargedParticleFilter = new ParticleFilter(); // no conditions -- accepts all
  chargedParticleFilter->setName("CH");
  chargedParticleFilter->setLongName("CH");
  chargedParticleFilter->setTitle("CH");
  chargedParticleFilter->setLongTitle("CH");
  chargedParticleFilter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  chargedParticleFilter->addCondition(4, 1102, 0.0, 0.0);   // low mass charged hadron

  ParticleFilter* aliceV0ParticleFilter  = new ParticleFilter();
  aliceV0ParticleFilter->setName("V0M");
  aliceV0ParticleFilter->setLongName("V0M");
  aliceV0ParticleFilter->setTitle("V0M");
  aliceV0ParticleFilter->setLongTitle("V0M");
  aliceV0ParticleFilter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  aliceV0ParticleFilter->addCondition(4, 1102, 0.0, 0.0);   // low mass charged hadron
  aliceV0ParticleFilter->addCondition(5, 1, 0.05,  1000.0);  // pT
  aliceV0ParticleFilter->addCondition(5, 7, -3.7, -1.7, 2.8, 5.1); // pseudorapidity in V0A and V0C

  ParticleFilter* aliceTpcParticleFilter = new ParticleFilter();
  aliceTpcParticleFilter->setName("TpcAll");
  aliceTpcParticleFilter->setLongName("TpcAll");
  aliceTpcParticleFilter->setTitle("TpcAll");
  aliceTpcParticleFilter->setLongTitle("TpcAll");
  aliceTpcParticleFilter->addCondition(0, 1,  0.0, 0.0);     // live particles only
  aliceTpcParticleFilter->addCondition(4, 1102, 0.0, 0.0);   // low mass charged hadron
  aliceTpcParticleFilter->addCondition(5, 1, 0.15,  100.0);  // pT
  aliceTpcParticleFilter->addCondition(5, 7, -0.8, 0.8);     // pseudorapidity in  TPC

  ParticleFilter*  chargedWideParticleFilter = new ParticleFilter();
  chargedWideParticleFilter->setName("CHW");
  chargedWideParticleFilter->setLongName("CHW");
  chargedWideParticleFilter->setTitle("CHW");
  chargedWideParticleFilter->setLongTitle("CHW");
  chargedWideParticleFilter->addCondition(0, 1,  0.0, 0.0);  // live particles only
  chargedWideParticleFilter->addCondition(4, 1102, 0.0, 0.0);   // low mass charged hadron
  chargedWideParticleFilter->addCondition(5, 1, 0.05,  1000.0);  // pT


  switch (analysisParticleFilterOption)
    {
      default:
      case 0:  analysisParticleFilters.push_back( openParticleFilter      ); break;
      case 1:  analysisParticleFilters.push_back( aliceV0ParticleFilter   ); break;
      case 2:  analysisParticleFilters.push_back( aliceTpcParticleFilter ); break;
      case 3:  analysisParticleFilters = ParticleFilter::createChargedHadronFilters(YES,0.0, 20.0, YES,-2.0, 2.0, NO, 10.0, -10.0); break;
      case 4:  analysisParticleFilters = ParticleFilter::createChargedHadronFilters(YES,0.000, 20.0, NO,-6.0, 6.0, YES, -10.0, 10.0); break;
      case 5:  analysisParticleFilters = ParticleFilter::createPlusMinusHadronFilters(YES,0.0, 20.0, NO,-2.0, 2.0, YES,  -10.0, 10.0); break;
      case 6:  analysisParticleFilters = ParticleFilter::createPlusMinusHadronFilters(YES,0.0, 100.0, YES,-4.0, 4.0, NO, 10.0, -10.0); break;
      case 7:  analysisParticleFilters = ParticleFilter::createChargedHadronFilters(YES,0.2, 1.0, YES,-4.0, 4.0, NO, 10.0, -10.0); break;
      //case 6:  analysisParticleFilters = ParticleFilter::createBaryonFilters(YES,0.2, 10.0, YES, -1.0, 1.0, NO, 10.0, -10.0); break;
    }

  
  std::cout << "==================================================================================" << std::endl;
  std::cout << "Setup Completed - Proceed with loading analysis" << std::endl;
  std::cout << "==================================================================================" << std::endl;

  TaskIterator              * eventAnalysis;
  DerivedHistoIterator      * derivedHistoIterator;
  BalanceFunctionCalculator * balFctHistoIterator;
  SubSampleStatIterator     * basicSubsampleIterator;
  SubSampleStatIterator     * derivedSubsampleIterator;
  SubSampleStatIterator     * balFctSubsampleIterator;
  ClosureIterator           * basicClosureIterator;
  ClosureIterator           * derivedClosureIterator;
  ClosureIterator           * balFctClosureIterator;
  Task                      * task;

  if (loadPdgTable)
    {
    Configuration pdgDataConfig("ParticleTypeTableLoader Configuration");
    pdgDataConfig.addParameter("dataInputUsed",     true);
    pdgDataConfig.addParameter("dataInputPath",     TString(getenv("CAP_SRC")));
    pdgDataConfig.addParameter("dataInputFileName", TString("/EOS/pdgPhysicalKaons.dat"));
    task= new ParticleTypeTableLoader("ParticleTypeTableLoader",pdgDataConfig,selectedLevel);
    task->execute();

    //    TString pdgDataTable = getenv("CAP_SRC");
    //    pdgDataTable += "/EOS/pdgPhysicalKaons.dat"; // this table has k-short and k-long in addition to k0 and k0-bar
    //    ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection(); //ParticleTypeCollection::getMasterParticleCollection();
    //    particles->readFromFile(pdgDataTable);
    //    if (selectedLevel == debugLevel) particles->printProperties(std::cout);
    }

  if (runEventAnalysis)
    {
    Configuration eventAnalysisConfig("Event Iterator Configuration");
    eventAnalysisConfig.addParameter("doPartialReports",      doPartialReports);
    eventAnalysisConfig.addParameter("doPartialSaves",        doPartialSaves);
    eventAnalysisConfig.addParameter("nIterationRequested",   nIterationRequested);
    eventAnalysisConfig.addParameter("nIterationReported",    nIterationReported);
    eventAnalysisConfig.addParameter("nIterationPartialSave", nIterationPartialSave);
    eventAnalysisConfig.addParameter("doSubsampleAnalysis",   runSubsampleAnalysis);
    eventAnalysisConfig.addParameter("useParticles",          YES);
    eventAnalysisConfig.addParameter("useEventStream0",       YES);
    if (runPerformSimulator) eventAnalysisConfig.addParameter("useEventStream1",  YES);
    eventAnalysis = new TaskIterator("EventIterator",eventAnalysisConfig,selectedLevel);
    }

  if (runPythiaGenerator)
    {
    double  beamEnergy                     = 5200.0; // GeV
    int     beamPdgCode                    = 2212;    // proton
    int     targetPdgCode                  = 2212;

    histoModelDataName  = "PYTHIA";
    TString pythiaLabel = "PYTHIA";
    TString systemLabel = "";
    TString energyLabel = "";
    if (beamPdgCode    == 2212)  systemLabel  += "P";
    if (beamPdgCode    == -2212) systemLabel  += "Pbar";
    if (targetPdgCode  == 2212)  systemLabel  += "P";
    if (targetPdgCode  == -2212) systemLabel  += "Pbar";
    energyLabel += int(beamEnergy);

    inputPathName  += pythiaLabel;
    inputPathName  += "/";
    inputPathName  += systemLabel;
    inputPathName  += "/";
    inputPathName  += energyLabel;
    inputPathName  += "/";
    //input FileNameBase  = "PYTHIA_";

    outputPathName  += pythiaLabel;
    outputPathName  += "/";
    outputPathName  += systemLabel;
    outputPathName  += "/";
    outputPathName  += energyLabel;
    outputPathName  += "/";
    outputPathName  += pathSuffix;
    //outputFileNameBase = "PYTHIA_";

    Configuration pythiaConfig("Pythia Configuration");
    pythiaConfig.addParameter("beam",    beamPdgCode);  // PDG Code   proton is 2212
    pythiaConfig.addParameter("target",  targetPdgCode);
    pythiaConfig.addParameter("energy",  beamEnergy);
    pythiaConfig.addParameter("useEventStream0",    YES);
    pythiaConfig.addParameter("standaloneMode",     YES);   // use interactions generated by the geometry generator
    pythiaConfig.addParameter("dataOutputUsed",      NO);
    pythiaConfig.addParameter("dataConversionToWac",YES);
    pythiaConfig.addParameter("dataInputUsed",    NO);
    pythiaConfig.addParameter("saveHistograms",   NO);
    pythiaConfig.addParameter("option0",  TString("Init:showChangedSettings = off") );      // list changed settings
    pythiaConfig.addParameter("option1",  TString("Init:showChangedParticleData = off") ); // list changed particle data
    pythiaConfig.addParameter("option2",  TString("Next:numberCount = 10000") );            // print message every n events
    pythiaConfig.addParameter("option3",  TString("Next:numberShowInfo = 0") );            // print event information n times
    pythiaConfig.addParameter("option4",  TString("Next:numberShowProcess = 0") );         // print process record n times
    pythiaConfig.addParameter("option5",  TString("Next:numberShowEvent = 0") );
    pythiaConfig.addParameter("option6",  TString("SoftQCD:inelastic = on") );             // All inelastic processes
    pythiaConfig.addParameter("option7",  TString("ColourReconnection:reconnect = on") );
    pythiaConfig.addParameter("option8",  TString("130:mayDecay = no") ); //K0s decay off
    pythiaConfig.addParameter("option9",  TString("310:mayDecay = no") ); //K0s decay off
    pythiaConfig.addParameter("option10", TString("311:mayDecay = no") ); //K0  decay off
    pythiaConfig.addParameter("option11", TString("3112:mayDecay = no") );
    pythiaConfig.addParameter("option12", TString("3122:mayDecay = no") );
    pythiaConfig.addParameter("option13", TString("3222:mayDecay = no") );
    pythiaConfig.addParameter("option14", TString("3212:mayDecay = no") );
    pythiaConfig.addParameter("option15", TString("3322:mayDecay = no") );
    pythiaConfig.addParameter("option16", TString("3312:mayDecay = no") );
    pythiaConfig.addParameter("option17", TString("3334:mayDecay = no") );
    pythiaConfig.addParameter("option18", TString("ParticleDecays:limitTau0 = on") );
    pythiaConfig.addParameter("option19", TString("ParticleDecays:tau0Max = 1"   ) );   // particles decay is c*tau less than 1 mm
    vector<EventFilter*> pythiaEventFilters;
    pythiaEventFilters.push_back( openEventFilter);
    vector<ParticleFilter*> pythiaParticleFilters;
    pythiaParticleFilters.push_back( openParticleFilter );
    task  = new PythiaEventGenerator("PythiaEventGenerator",pythiaConfig,pythiaEventFilters,pythiaParticleFilters,selectedLevel);
    if (runEventAnalysis) eventAnalysis->addSubTask(task);
    }
  if (runHerwigGenerator)
    {
    histoModelDataName  = "Herwig";
    std::cout << "Option runHerwigGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runAmptReader )
    {
    histoModelDataName  = "AMPT";
    Configuration amptConfig("Ampt Reader Configuration");
    amptConfig.addParameter("useEventStream0",    YES);
    amptConfig.addParameter("standaloneMode",     YES);   // use interactions generated by the geometry generator
    amptConfig.addParameter("dataOutputUsed",      NO);
    amptConfig.addParameter("dataConversionToWac",YES);
    amptConfig.addParameter("dataInputUsed",      YES);
    amptConfig.addParameter("dataInputPath",       inputPathName);
    amptConfig.addParameter("dataInputTreeName",   TString("tree") );
    amptConfig.addParameter("dataInputFileMinIndex", 0);
    amptConfig.addParameter("dataInputFileMaxIndex", -1);
    vector<EventFilter*> amptEventFilters;
    amptEventFilters.push_back( openEventFilter);
    vector<ParticleFilter*> amptParticleFilters;
    amptParticleFilters.push_back( chargedWideParticleFilter );
    task = new AmptEventReader("AmptEventReader",amptConfig,amptEventFilters,amptParticleFilters,selectedLevel);
    if (runEventAnalysis) eventAnalysis->addSubTask(task);
    }
  if (runEposGenerator)
    {
    histoModelDataName  = "EPOS";
    std::cout << "Option runEposGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runUrqmdGenerator)
    {
    histoModelDataName  = "URQMD";
    std::cout << "Option runUrqmdGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runHijingGenerator)
    {
    histoModelDataName  = "HIJING";
    std::cout << "Option runHijingGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runGausResGenerator)
    {
    histoModelDataName  = "RhoDecay";
    TString gausResLabel = "RhoDecay";

    double pTslope = 1.0;
    double rhoMass = 0.5;
    outputPathName += gausResLabel;
    outputPathName += "/PtSlope";
    outputPathName += int(1000*pTslope);
    outputPathName += "/Mass";
    outputPathName += int(1000*rhoMass);
    outputPathName += "/";
    outputPathName += pathSuffix;
    inputPathName  = outputPathName;

    Configuration resConfig("Resonance Generator Configuration");
    resConfig.addParameter("useEventStream0",      YES);
    resConfig.addParameter("standaloneMode",       YES);
    resConfig.addParameter("nParticlesMinimum",     2);
    resConfig.addParameter("nParticlesMaximum",     50);
    resConfig.addParameter("yMinimum",            -0.5);
    resConfig.addParameter("yMaximum",             0.5);
    resConfig.addParameter("pTslope",              1.0);
    resConfig.addParameter("mass",                0.800);
    vector<EventFilter*> eventFiltersR;
    eventFiltersR.push_back( openEventFilter);
    vector<ParticleFilter*>  particleFiltersR;
    particleFiltersR.push_back( openParticleFilter );
    task  = new ResonanceGenerator(  "ResonanceGenerator",resConfig,    eventFiltersR,  particleFiltersR,  selectedLevel);
    if (runEventAnalysis) eventAnalysis->addSubTask(task);
    }
  if (runPerformSimulator)
    {
    Configuration performConfig("Performance Simulator Configuration");
    performConfig.addParameter("createHistograms",  NO);
    performConfig.addParameter("saveHistograms",    NO);
    performConfig.setParameter("loadHistograms",    NO);
    performConfig.addParameter("allEventsUseSameFilters", YES);
    performConfig.addParameter("useParticles",      YES);
    performConfig.addParameter("useEventStream0",   YES);
    performConfig.addParameter("useEventStream1",   YES);

    performConfig.addParameter("useSameSetForAll",  YES);
    performConfig.addParameter("resolutionOption",  1);
    performConfig.addParameter("efficiencyOption",  1);
    TString baseName = "Filter0";
    //
    performConfig.addParameter(baseName+"_PtBiasAinv",0.0);
    performConfig.addParameter(baseName+"_PtBiasA0",  0.0);
    performConfig.addParameter(baseName+"_PtBiasA1",  0.0);
    performConfig.addParameter(baseName+"_PtBiasA2",  0.0);
    performConfig.addParameter(baseName+"_PtRmsAinv", 0.000);
    performConfig.addParameter(baseName+"_PtRmsA0",   0.00);
    performConfig.addParameter(baseName+"_PtRmsA1",   0.00);
    performConfig.addParameter(baseName+"_PtRmsA2",   0.00);
    performConfig.addParameter(baseName+"_EtaBiasA0",  0.0);
    performConfig.addParameter(baseName+"_EtaBiasA1",  0.0);
    performConfig.addParameter(baseName+"_EtaBiasA2",  0.0);
    performConfig.addParameter(baseName+"_EtaRmsAinv", 0.0);
    performConfig.addParameter(baseName+"_EtaRmsA0",   0.0);
    performConfig.addParameter(baseName+"_EtaRmsA1",   0.0);
    performConfig.addParameter(baseName+"_EtaRmsA2",   0.0);

    performConfig.addParameter(baseName+"_PhiBiasA0",  0.0);
    performConfig.addParameter(baseName+"_PhiBiasA1",  0.0);
    performConfig.addParameter(baseName+"_PhiBiasA2",  0.0);
    performConfig.addParameter(baseName+"_PhiRmsAinv", 0.0);
    performConfig.addParameter(baseName+"_PhiRmsA0",   0.0);
    performConfig.addParameter(baseName+"_PhiRmsA1",   0.0);
    performConfig.addParameter(baseName+"_PhiRmsA2",   0.0);

    //      performConfig.addParameter(baseName+"_PtBiasAinv",0.0);
    //      performConfig.addParameter(baseName+"_PtBiasA0",  0.0);
    //      performConfig.addParameter(baseName+"_PtBiasA1",  0.0);
    //      performConfig.addParameter(baseName+"_PtBiasA2",  0.0);
    //      performConfig.addParameter(baseName+"_PtRmsAinv", 0.0005);
    //      performConfig.addParameter(baseName+"_PtRmsA0",   0.00);
    //      performConfig.addParameter(baseName+"_PtRmsA1",   0.00);
    //      performConfig.addParameter(baseName+"_PtRmsA2",   0.005);
    //      performConfig.addParameter(baseName+"_EtaBiasA0",  0.0);
    //      performConfig.addParameter(baseName+"_EtaBiasA1",  0.0);
    //      performConfig.addParameter(baseName+"_EtaBiasA2",  0.0);
    //      performConfig.addParameter(baseName+"_EtaRmsAinv", 0.0);
    //      performConfig.addParameter(baseName+"_EtaRmsA0",   0.01);
    //      performConfig.addParameter(baseName+"_EtaRmsA1",   0.01);
    //      performConfig.addParameter(baseName+"_EtaRmsA2",   0.0);
    //
    //      performConfig.addParameter(baseName+"_PhiBiasA0",  0.0);
    //      performConfig.addParameter(baseName+"_PhiBiasA1",  0.0);
    //      performConfig.addParameter(baseName+"_PhiBiasA2",  0.0);
    //      performConfig.addParameter(baseName+"_PhiRmsAinv", 0.01);
    //      performConfig.addParameter(baseName+"_PhiRmsA0",   0.034);
    //      performConfig.addParameter(baseName+"_PhiRmsA1",   0.035);
    //      performConfig.addParameter(baseName+"_PhiRmsA2",   0.0);

    performConfig.addParameter(baseName+"_EffPeakAmp",0.8);
    performConfig.addParameter(baseName+"_EffPeakPt",0.01);
    performConfig.addParameter(baseName+"_EffPeakRms",0.5);
    performConfig.addParameter(baseName+"_EffA1",0.0);
    performConfig.addParameter(baseName+"_EffA2",0.0);

    vector<EventFilter*>     performEventFilters;
    vector<ParticleFilter*>  performParticleFilters ;
    performEventFilters.push_back( openEventFilter);
    performParticleFilters.push_back( openParticleFilter  );
    eventAnalysis->addSubTask( new MeasurementPerformanceSimulator("Sim",performConfig,performEventFilters,performParticleFilters,selectedLevel) );
    }


  if (runDerivedHistoCalculation)
    {
    Configuration derivedHistoConfig("DerivedHistoCalculator Configuration");
    derivedHistoConfig.addParameter("histoInputPath",        outputPathName);
    derivedHistoConfig.addParameter("histoOutputPath",       outputPathName);
    derivedHistoConfig.addParameter("histoModelDataName",    histoModelDataName);
    derivedHistoConfig.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
    derivedHistoConfig.addParameter("IncludedPattern0",rootExt);
    derivedHistoConfig.addParameter("ExcludedPattern0",derivedLabel);
    derivedHistoConfig.addParameter("ExcludedPattern1",sumLabel);
    derivedHistoConfig.addParameter("ExcludedPattern2",balFctLabel);
    derivedHistoConfig.addParameter("appendedString",  derivedLabel);
    derivedHistoIterator = new DerivedHistoIterator("DerivedHistoIterator",derivedHistoConfig,selectedLevel);
    }

  if (runBasicSubsampleAnalysis)
    {
    TString s("Sum_");
    Configuration config("Event Iterator Configuration");
    config.addParameter("forceHistogramsRewrite",forceHistogramsRewrite);
    config.addParameter("appendedString",        sumLabel);
    config.addParameter("IncludedPattern0",      histoModelDataName);
    config.addParameter("ExcludedPattern0",      sumLabel);
    config.addParameter("ExcludedPattern1",      derivedLabel);
    config.addParameter("ExcludedPattern2",      balFctLabel);
    config.addParameter("histoInputPath",        outputPathName);
    config.addParameter("histoOutputPath",       outputPathName);
    config.addParameter("histoModelDataName",    histoModelDataName);
    basicSubsampleIterator = new SubSampleStatIterator("SubsampleBasicHistos",config,selectedLevel);
    }

  if (runDerivedSubsampleAnalysis)
    {
    Configuration config("Event Iterator Configuration");
    config.addParameter( "forceHistogramsRewrite",forceHistogramsRewrite);
    config.addParameter( "appendedString",        derivedLabel+sumLabel);
    config.addParameter( "IncludedPattern0",      histoModelDataName);
    config.addParameter( "IncludedPattern1",      derivedLabel);
    config.addParameter( "ExcludedPattern0",      balFctLabel);
    config.addParameter( "histoInputPath",        outputPathName);
    config.addParameter( "histoOutputPath",       outputPathName);
    config.addParameter( "histoModelDataName",    histoModelDataName);
    derivedSubsampleIterator = new SubSampleStatIterator("SubsampleBasicHistos",config,selectedLevel);
    }

  if (runBalFctSubsampleAnalysis)
    {
    cout << "runBalFctSubsampleAnalysis" << endl;
    Configuration config("BalFct Calculation Configuration");
    config.addParameter( "forceHistogramsRewrite",forceHistogramsRewrite);
    config.addParameter( "appendedString",        derivedLabel+balFctLabel+sumLabel);
    config.addParameter( "IncludedPattern0",      histoModelDataName);
    config.addParameter( "IncludedPattern1",      derivedLabel);
    config.addParameter( "IncludedPattern2",      balFctLabel);
    config.addParameter( "histoInputPath",        outputPathName);
    config.addParameter( "histoOutputPath",       outputPathName);
    config.addParameter( "histoModelDataName",    histoModelDataName);
    balFctSubsampleIterator = new SubSampleStatIterator("balFctSubsampleIterator",config,debugLevel);
    }


  if (runClosureAnalysis && runBasicClosureAnalysis)
    {
    cout << "runClosureAnalysis" << endl;
    Configuration config("Basic Closure Analysis Configuration");
    config.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
    config.addParameter("selectedMethod",          1);
    config.addParameter("histoModelDataName",      histoModelDataName);
    config.addParameter("histoInputPath",          outputPathName);
    config.addParameter("histoOutputPath",         outputPathName);
    config.addParameter("ExcludedPattern0",        derivedLabel);
    config.addParameter("ExcludedPattern1",        balFctLabel);
    config.addParameter("ExcludedPattern2",        closureLabel);
    basicClosureIterator = new ClosureIterator("BasicClosureAnalysis", config,debugLevel);
    }
  if (runClosureAnalysis && runDerivedClosureAnalysis)
    {
    cout << "runDerivedHistoCalculation" << endl;
    Configuration config("Derived Closure Analysis Configuration");
    config.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
    config.addParameter("selectedMethod",          1);
    config.addParameter("histoModelDataName",      histoModelDataName);
    config.addParameter("histoInputPath",          outputPathName);
    config.addParameter("histoOutputPath",         outputPathName);
    config.addParameter("IncludedPattern0",        derivedLabel);
    config.addParameter("ExcludedPattern0",        balFctLabel);
    config.addParameter("ExcludedPattern1",        closureLabel);
    derivedClosureIterator = new ClosureIterator("DerivedClosureAnalysis", config,infoLevel);
    }
  if (runClosureAnalysis && runBalFctClosureAnalysis)
    {
    cout << "runDerivedHistoCalculation" << endl;
    Configuration config("Derived Closure Analysis Configuration");
    config.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
    config.addParameter("selectedMethod",          1);
    config.addParameter("histoModelDataName",      histoModelDataName);
    config.addParameter("histoInputPath",          outputPathName);
    config.addParameter("histoOutputPath",         outputPathName);
    config.addParameter("IncludedPattern0",        derivedLabel);
    config.addParameter("ExcludedPattern0",        balFctLabel);
    config.addParameter("ExcludedPattern1",        closureLabel);
    balFctClosureIterator = new ClosureIterator("BalFctClosureAnalysis", config,infoLevel);
    }


  if (runPerformAnalysis)
    {
    Configuration performAnaConfig("Performance Analyzer Configuration");
    performAnaConfig.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
    performAnaConfig.addParameter("runSubsampleAnalysis",    runSubsampleAnalysis);
    performAnaConfig.addParameter("doPartialSaves",          doPartialSaves);
    performAnaConfig.addParameter("scaleHistograms",         scaleHistograms);
    performAnaConfig.addParameter("histoOutputPath",         outputPathName);
    performAnaConfig.addParameter("histoModelDataName",      histoModelDataName);
    performAnaConfig.addParameter("histoAnalyzerName",       simAnaLabel);
    performAnaConfig.addParameter("histoBaseName",           simAnaLabel);
    performAnaConfig.addParameter("fillEta",                 fillEta);
    performAnaConfig.addParameter("fillY",                   fillY);
    vector<EventFilter*>     performAnaEventFilters;
    vector<ParticleFilter*>  performAnaParticleFilters ;
    performAnaEventFilters.push_back( openEventFilter);
    performAnaParticleFilters.push_back( openParticleFilter  );
    task = new ParticlePerformanceAnalyzer(simAnaLabel,performAnaConfig,performAnaEventFilters,performAnaParticleFilters,selectedLevel);
    if (runEventAnalysis)    eventAnalysis->addSubTask(task);
    if (runBasicSubsampleAnalysis) basicSubsampleIterator->addSubTask(task);
    if (runDerivedHistoCalculation) derivedHistoIterator->addSubTask(task);
    if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
    }

  if (runGlobalAnalysis)
    {
    Configuration config("Global Configuration");
    config.addParameter("useEventStream0",         YES);
    config.addParameter("useEventStream1",         NO);
    config.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
    config.addParameter("runSubsampleAnalysis",    runSubsampleAnalysis);
    config.addParameter("doPartialSaves",          doPartialSaves);
    config.addParameter("scaleHistograms",         scaleHistograms);
    config.addParameter("histoOutputPath",         outputPathName);
    config.addParameter("histoModelDataName",      histoModelDataName);
    config.addParameter("histoAnalyzerName",       globalLabel+genLabel);
    config.addParameter("histoBaseName",           globalLabel+genLabel);
    config.addParameter("fillCorrelationHistos",   NO);
    config.addParameter("fill2D",                  YES);
    config.addParameter("nBins_n",        100);
    config.addParameter("nBins_n2",       100);
    config.addParameter("min_n",         -0.5);
    config.addParameter("max_n",         99.5);
    config.addParameter("nBins_e",        200);
    config.addParameter("nBins_e2",        20);
    config.addParameter("min_e",          0.0);
    config.addParameter("max_e",       1000.0);
    config.addParameter("nBins_q",        201);
    config.addParameter("nBins_q2",        20);
    config.addParameter("min_q",       -100.5);
    config.addParameter("max_q",        100.5);
    config.addParameter("nBins_b",        41);
    config.addParameter("nBins_b2",       41);
    config.addParameter("min_b",        -20.5);
    config.addParameter("max_b",         20.5);
    config.addParameter("nBins_ptSum",  100);
    config.addParameter("nBins_ptSum2", 100);
    config.addParameter("min_ptSum",    0.0);
    config.addParameter("max_ptSum",  100.0);
    config.addParameter("nBins_ptAvg",  100);
    config.addParameter("nBins_ptAvg2", 100);
    config.addParameter("min_ptAvg",    0.0);
    config.addParameter("max_ptAvg",    4.0);

    vector<EventFilter*>     globalEventFilters;
    vector<ParticleFilter*>  globalParticleFilters ;
    switch (globalEventFilterOptions)
      {
        case 0: // totally open filters
        globalEventFilters.push_back( openEventFilter);
        globalParticleFilters.push_back( openParticleFilter  );
        break;

        case 1: // setups an ALICE minbias filter
        globalEventFilters.push_back( openEventFilter);
        globalParticleFilters.push_back( aliceV0ParticleFilter   );
        globalParticleFilters.push_back( aliceTpcParticleFilter  );
        break;
      }

    if (runGenLevelAnalysis)
      {
      task = new GlobalAnalyzer(globalLabel+genLabel,config,globalEventFilters,globalParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      if (runClosureAnalysis && runBasicClosureAnalysis)     basicClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runDerivedClosureAnalysis)   derivedClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runBalFctClosureAnalysis)    balFctClosureIterator->addSubTask(task);
      }
    if (runRecoLevelAnalysis)
      {
      Configuration configR(config);
      configR.addParameter("useEventStream0",   NO);
      configR.addParameter("useEventStream1",   YES);
      configR.addParameter("histoAnalyzerName", globalLabel+recoLabel);
      configR.addParameter("histoBaseName",     globalLabel+recoLabel);
      task = new GlobalAnalyzer(globalLabel+recoLabel,configR,globalEventFilters,globalParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      }

    }

  if (runSpherocityAnalysis)
    {
    Configuration config("Global Configuration");
    config.addParameter("useEventStream0",        YES);
    config.addParameter("forceHistogramsRewrite", forceHistogramsRewrite);
    config.addParameter("runSubsampleAnalysis",   runSubsampleAnalysis);
    config.addParameter("doPartialSaves",         doPartialSaves);
    config.addParameter("scaleHistograms",        scaleHistograms);
    config.addParameter("histoOutputPath",        outputPathName);
    config.addParameter("histoOutputFileName",    outputFileNameBase+spherocityLabel);
    config.addParameter("histoModelDataName",     histoModelDataName);
    config.addParameter("histoAnalyzerName",      spherocityLabel+genLabel);
    config.addParameter("histoBaseName",          spherocityLabel+genLabel);
    config.addParameter("setEvent",               NO);
    config.addParameter("fillCorrelationHistos",  YES);
    config.addParameter("nSteps",                 360);
    config.addParameter("fillS0",                 YES);
    config.addParameter("fillS1",                 NO);
    config.addParameter("fillS1VsS0",             NO);
    config.addParameter("nBins_spherocity",       100);
    config.addParameter("min_spherocity",         0.0);
    config.addParameter("max_spherocity",         1.0);
    vector<EventFilter*>     spherocityEventFilters;
    vector<ParticleFilter*>  spherocityParticleFilters ;
    switch (globalEventFilterOptions)
      {
        case 0: // totally open filters
        spherocityEventFilters.push_back( openEventFilter);
        spherocityParticleFilters.push_back( openParticleFilter  );
        break;

        case 1: // setups an ALICE minbias filter
        spherocityEventFilters.push_back(    aliceMinBiasEventFilter );
        spherocityParticleFilters.push_back( aliceTpcParticleFilter  );
        spherocityParticleFilters.push_back( aliceV0ParticleFilter   );
        spherocityParticleFilters.push_back( openParticleFilter  );
        spherocityParticleFilters.push_back( chargedParticleFilter );
        break;
      }
    if (runGenLevelAnalysis)
      {
      task = new TransverseSpherocityAnalyzer(spherocityLabel+genLabel,config,spherocityEventFilters,spherocityParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      if (runClosureAnalysis && runBasicClosureAnalysis)     basicClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runDerivedClosureAnalysis)   derivedClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runBalFctClosureAnalysis)    balFctClosureIterator->addSubTask(task);
      }
    if (runRecoLevelAnalysis)
      {
      Configuration configR(config);
      configR.addParameter("useEventStream0",   NO);
      configR.addParameter("useEventStream1",   YES);
      configR.addParameter("histoAnalyzerName", spherocityLabel+recoLabel);
      configR.addParameter("histoBaseName",     spherocityLabel+recoLabel);
      task = new TransverseSpherocityAnalyzer(spherocityLabel+recoLabel,configR,spherocityEventFilters,spherocityParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      }
    }

  if (runPartAnalysis)
    {
    Configuration config("Part Spectra Configuration");
    config.addParameter("useEventStream0",         YES);
    config.addParameter("useEventStream1",         NO);
    config.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
    config.addParameter("runSubsampleAnalysis",     runSubsampleAnalysis);
    config.addParameter("doPartialSaves",          doPartialSaves);
    config.addParameter("scaleHistograms",         scaleHistograms);
    config.addParameter("histoOutputPath",         outputPathName);
    config.addParameter("histoModelDataName",      histoModelDataName);
    config.addParameter("histoAnalyzerName",       partLabel+genLabel);
    config.addParameter("histoBaseName",           partLabel+genLabel);
    config.addParameter("nBins_n1",    100);
    config.addParameter("min_n1",      0.0);
    config.addParameter("max_n1",      100.0);
    config.addParameter("nBins_eTot",  100);
    config.addParameter("min_eTot",    0.0);
    config.addParameter("max_eTot",    100.0);
    config.addParameter("nBins_pt",    100);
    config.addParameter("min_pt",      0.0);
    config.addParameter("max_pt",      10.0);
    config.addParameter("nBins_eta",   80);
    config.addParameter("min_eta",     -4.0);
    config.addParameter("max_eta",     4.0);
    config.addParameter("nBins_y",     60);
    config.addParameter("min_y",       -6.0);
    config.addParameter("max_y",       6.0);
    config.addParameter("nBins_phi",   72);
    config.addParameter("min_phi",     0.0);
    config.addParameter("max_phi",     TMath::TwoPi());
    config.addParameter("fillEta",     fillEta);
    config.addParameter("fillY",       fillY);
    config.addParameter("fillP2",      NO);

    if (runGenLevelAnalysis)
      {
      task = new ParticleAnalyzer(partLabel+genLabel,config,analysisEventFilters,analysisParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      if (runClosureAnalysis && runBasicClosureAnalysis)     basicClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runDerivedClosureAnalysis)   derivedClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runBalFctClosureAnalysis)    balFctClosureIterator->addSubTask(task);
      }
    if (runRecoLevelAnalysis)
      {
      Configuration configR(config);
      configR.addParameter("useEventStream0",   NO);
      configR.addParameter("useEventStream1",   YES);
      configR.addParameter("histoAnalyzerName", partLabel+recoLabel);
      configR.addParameter("histoBaseName",     partLabel+recoLabel);
      task = new ParticleAnalyzer(partLabel+recoLabel,configR,analysisEventFilters,analysisParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      }
    }

  if (runPairAnalysis)
    {
    Configuration config("Pair Correlation Configuration");
    config.addParameter("useEventStream0",         YES);
    config.addParameter("useEventStream1",         NO);
    config.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
    config.addParameter("runSubsampleAnalysis",    runSubsampleAnalysis);
    config.addParameter("doPartialSaves",          doPartialSaves);
    config.addParameter("scaleHistograms",         scaleHistograms);
    config.addParameter("histoOutputPath",         outputPathName);
    config.addParameter("histoModelDataName",      histoModelDataName);
    config.addParameter("histoAnalyzerName",       pairLabel+genLabel);
    config.addParameter("histoBaseName",           pairLabel+genLabel);
    config.addParameter("nBins_n1",        100);
    config.addParameter("min_n1",          0.0);
    config.addParameter("max_n1",          100.0);
    config.addParameter("nBins_n2",        100);
    config.addParameter("min_n2",          0.0);
    config.addParameter("max_n2",          1000.0);
    config.addParameter("nBins_pt",        40);
    config.addParameter("min_pt",          0.00);
    config.addParameter("max_pt",          20.0);
    config.addParameter("nBins_eta",       60);
    config.addParameter("min_eta",         -6.0);
    config.addParameter("max_eta",         6.0);
    config.addParameter("nBins_y",         100);
    config.addParameter("min_y",           -10.0);
    config.addParameter("max_y",           10.0);
    config.addParameter("nBins_phi",       72);
    config.addParameter("min_phi",         0.0);
    config.addParameter("max_phi",         TMath::TwoPi());
    config.addParameter("fillEta",         fillEta);
    config.addParameter("fillY",           fillY);
    config.addParameter("fillP2",          NO);

    if (runGenLevelAnalysis)
      {
      task = new ParticlePairAnalyzer(pairLabel+genLabel, config,analysisEventFilters, analysisParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      if (runBalFctSubsampleAnalysis)  balFctSubsampleIterator->addSubTask(task);

      if (runClosureAnalysis && runBasicClosureAnalysis)     basicClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runDerivedClosureAnalysis)   derivedClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runBalFctClosureAnalysis)    balFctClosureIterator->addSubTask(task);
      }
    if (runRecoLevelAnalysis)
      {
      Configuration configR(config);
      configR.addParameter("useEventStream0",   NO);
      configR.addParameter("useEventStream1",   YES);
      configR.addParameter("histoAnalyzerName", pairLabel+recoLabel);
      configR.addParameter("histoBaseName",     pairLabel+recoLabel);
      task = new ParticlePairAnalyzer(pairLabel+recoLabel, configR,analysisEventFilters, analysisParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      }
    }
  if (runNuDynAnalysis)
    {
    Configuration config("NuDyn Correlation Configuration");
    config.addParameter("useEventStream0",         YES);
    config.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
    config.addParameter("runSubsampleAnalysis",     runSubsampleAnalysis);
    config.addParameter("doPartialSaves",          doPartialSaves);
    config.addParameter("scaleHistograms",         scaleHistograms);
    config.addParameter("histoOutputPath",         outputPathName);
    config.addParameter("histoOutputFileName",     outputFileNameBase+nuDynLabel);
    config.addParameter("histoModelDataName",      histoModelDataName);
    config.addParameter("histoAnalyzerName",       nuDynLabel+genLabel);
    config.addParameter("histoBaseName",           nuDynLabel+genLabel);
    config.addParameter("inputType",               1);
    config.addParameter("pairOnly",                true);
    config.addParameter("nBins_mult",              200);
    config.addParameter("min_mult",                0.0);
    config.addParameter("max_mult",                200.0);

    if (runGenLevelAnalysis)
      {
      task = new NuDynAnalyzer(nuDynLabel+genLabel,config,analysisEventFilters,analysisParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      if (runClosureAnalysis && runBasicClosureAnalysis)     basicClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runDerivedClosureAnalysis)   derivedClosureIterator->addSubTask(task);
      if (runClosureAnalysis && runBalFctClosureAnalysis)    balFctClosureIterator->addSubTask(task);
      }
    if (runRecoLevelAnalysis)
      {
      Configuration configR(config);
      configR.addParameter("useEventStream0",     NO);
      configR.addParameter("useEventStream1",     YES);
      configR.addParameter("histoAnalyzerName",   nuDynLabel+recoLabel);
      configR.addParameter("histoBaseName",       nuDynLabel+recoLabel);
      task = new NuDynAnalyzer(nuDynLabel+recoLabel,configR,analysisEventFilters,analysisParticleFilters,selectedLevel);
      if (runEventAnalysis)            eventAnalysis->addSubTask(task);
      if (runBasicSubsampleAnalysis)   basicSubsampleIterator->addSubTask(task);
      if (runDerivedHistoCalculation)  derivedHistoIterator->addSubTask(task);
      if (runDerivedSubsampleAnalysis) derivedSubsampleIterator->addSubTask(task);
      }
    }

  std::cout << "==================================================================================" << std::endl;
  std::cout << "Configuration Completed - Run analysis" << std::endl;
  std::cout << "==================================================================================" << std::endl;
  gSystem->mkdir(outputPathName,1);

  if (runEventAnalysis)             eventAnalysis->execute();
  if (runBasicSubsampleAnalysis)    basicSubsampleIterator->execute();
  if (runDerivedHistoCalculation)   derivedHistoIterator->execute();
  if (runDerivedSubsampleAnalysis)  derivedSubsampleIterator->execute();
  if (runBalFctCalculation)
    {
    vector<TString>  sObservableNames;
    vector<TString>  pObservableNames;
    int observableSelection = 5;
    switch (observableSelection)
      {
        default:
        case 0: // eta based observables, full complement
        sObservableNames.push_back("n1_eta");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("R2_ptpt");
        pObservableNames.push_back("R2_phiPhi");
        pObservableNames.push_back("R2_etaEta");
        pObservableNames.push_back("R2_DetaDphi_shft");
        break;

        case 1: // eta based observables, only DeltaEta vs DeltaPhi
        sObservableNames.push_back("n1_eta");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("rho2_DetaDphi_shft");
        break;

        case 2: // y based observables, full complement
        sObservableNames.push_back("n1_y");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("R2_ptpt");
        pObservableNames.push_back("R2_phiPhi");
        pObservableNames.push_back("R2_yY");
        pObservableNames.push_back("R2_DyDphi_shft");
        break;

        case 3: // y based observables, only DeltaY vs DeltaPhi
        sObservableNames.push_back("n1_y");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("R2_DyDphi_shft");
        break;

        case 4: // eta based observables, only DeltaEta vs DeltaPhi
        sObservableNames.push_back("n1_eta");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("rho2_DetaDphi_shft");
        pObservableNames.push_back("R2_DetaDphi_shft");
        //pObservableNames.push_back("B2AB_DetaDphi_shft");
        //pObservableNames.push_back("B2BA_DetaDphi_shft");
        //        pObservableNames.push_back("n2_phiPhi");
        break;

        case 5: // y based observables, only DeltaEta vs DeltaPhi
        sObservableNames.push_back("n1_y");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("A2_DyDphi_shft");
        pObservableNames.push_back("B2_DyDphi_shft");
        pObservableNames.push_back("C2_DyDphi_shft");
        pObservableNames.push_back("R2_DyDphi_shft");
        pObservableNames.push_back("B2_yY");
        pObservableNames.push_back("B2_phiPhi");
        break;
      }
    //outputPathName = "/Volumes/ClaudeDisc4/OutputFiles/longTest/RhoDecay/";
    //TString histoInputFileName = "RhoDecay_Pair_Derived.root";
    //TString histoOutputFileName = "RhoDecay_Pair_BalFct.root";
    Configuration balFctCalcConfig("BalFctCalc Configuration");
    balFctCalcConfig.addParameter( "forceHistogramsRewrite",  forceHistogramsRewrite);
    balFctCalcConfig.addParameter( "appendedString",          balFctLabel);
    balFctCalcConfig.addParameter( "IncludedPattern0",        histoModelDataName);
    balFctCalcConfig.addParameter( "IncludedPattern1",        pairLabel);
    balFctCalcConfig.addParameter( "IncludedPattern2",        derivedLabel);
    balFctCalcConfig.addParameter( "ExcludedPattern0",        balFctLabel);
    balFctCalcConfig.addParameter( "ExcludedPattern1",        closureLabel);
    balFctCalcConfig.addParameter( "ExcludedPattern2",        recoLabel);
    balFctCalcConfig.addParameter( "histoInputPath",          outputPathName);
    balFctCalcConfig.addParameter( "histoOutputPath",         outputPathName);
    balFctCalcConfig.addParameter( "histoInputFileName",      TString(""));
    balFctCalcConfig.addParameter( "histoOutputFileName",     TString(""));
    balFctCalcConfig.addParameter( "histoModelDataName",      histoModelDataName);
    balFctCalcConfig.addParameter( "histoAnalyzerName",       pairLabel+"_Gen");
    balFctCalcConfig.addParameter( "fillEta",                 fillEta);
    balFctCalcConfig.addParameter( "fillY",                   fillY);
    balFctCalcConfig.addParameter( "fillP2",                  NO);

    balFctHistoIterator = new BalanceFunctionCalculator("BalFctCalc",
                                                        balFctCalcConfig,
                                                        analysisEventFilters,
                                                        analysisParticleFilters,
                                                        sObservableNames,
                                                        pObservableNames,
                                                        selectedLevel);
    balFctHistoIterator->execute();
    //delete balFctHistoIterator;
    }
  if (runBalFctSubsampleAnalysis) balFctSubsampleIterator->execute();
  if (runClosureAnalysis && runBasicClosureAnalysis)    basicClosureIterator->execute();
  if (runClosureAnalysis && runDerivedClosureAnalysis)  derivedClosureIterator->execute();
  if (runClosureAnalysis && runBalFctClosureAnalysis)   derivedClosureIterator->execute();

  return 0;
}




/*
 To generate enhanced charm production -- received from Pet Christianssen. Jul 16-2021.
 // Initialize PYTHIA minbias Generator.
 Pythia8::Pythia pythia;
 pythia.readString("Beams:eCM = 5000."); // 5 TeV pp
 //  pythia.readString("Beams:eCM = 13000."); // 13 TeV pp
 pythia.readString("HardQCD:gg2ccbar = on");
 pythia.readString("HardQCD:qqbar2ccbar = on");

 // Prevent charm hadrons from decaying.
 pythia.readString("ParticleDecays:limitTau0 = on");
 pythia.readString("ParticleDecays:tau0Max = 0");

 pythia.init();


 I select the final state hadrons like this:
 // Final
 if (!pythia.event[i].isFinal())
 continue;

 // Hadron
 if(!pythia.event[i].isHadron())
 continue;

 // print out all charm hadrons
 const int pdg = TMath::Abs(pythia.event[i].id()%10000); // only 3 digits meson
 // or 4 digits baryon
 if(pdg < 1000) { // meson
 if(int(pdg/100) != 4)
 continue;
 } else {

 if(int(pdg/1000) != 4)
 continue;
 }
 //pythia.readString("310:mayDecay = no");//K0s decay off
 //pythia.readString("ParticleDecays:limitTau0 = on");
 //pythia.readString("ParticleDecays:tau0Max = 10");
 //pythiaOptions.push_back( new TString("SoftQCD:all = on") );                   // Allow total sigma = elastic/SD/DD/ND

 // pythiaOptions.push_back(  new TString("HardQCD:all = on");
 */


void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load(includePath+"TaskIterator.hpp");
  gSystem->Load(includePath+"Collection.hpp");
  gSystem->Load(includePath+"CanvasCollection.hpp");
  gSystem->Load(includePath+"HistogramCollection.hpp");
  gSystem->Load(includePath+"Histograms.hpp");
  gSystem->Load(includePath+"Particle.hpp");
  gSystem->Load(includePath+"ParticleType.hpp");
  gSystem->Load(includePath+"ParticleTypeCollection.hpp");
  gSystem->Load(includePath+"ParticleDecayMode.hpp");
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
}

void loadPythia(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/CAPPythia/";
  gSystem->Load(includePath+"PythiaEventGenerator.hpp");
  gSystem->Load(includePath+"PythiaEventReader.hpp");
  gSystem->Load("libCAPPythia.dylib");
}

void loadPerformance(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Performance/";
  gSystem->Load(includePath+"ClosureCalculator.hpp");
  gSystem->Load(includePath+"ClosureIterator.hpp");
  gSystem->Load(includePath+"MeasurementPerformanceSimulator.hpp");
  gSystem->Load(includePath+"ParticlePerformanceSimulator.hpp");
  gSystem->Load(includePath+"ParticlePerformanceAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePerformanceHistos.hpp");
  gSystem->Load("libPerformance.dylib");
}

void loadAmpt(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Ampt/";
  gSystem->Load(includePath+"AmptEventReader.hpp");
  gSystem->Load("libAmpt.dylib");
}

void loadEpos(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Epos/";
  gSystem->Load(includePath+"EposEventReader.hpp");
  gSystem->Load("libEpos.dylib");
}

void loadBasicGen(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/BasicGen/";
  gSystem->Load(includePath+"GaussianGeneratorTask.hpp");
  gSystem->Load(includePath+"RadialBoostHistos.hpp");
  gSystem->Load(includePath+"RadialBoostTask.hpp");
  gSystem->Load(includePath+"RapidityGenerator.hpp");
  gSystem->Load("libBasicGen.dylib");
}

void loadGlobal(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Global/";
  gSystem->Load(includePath+"GlobalAnalyzer.hpp");
  gSystem->Load(includePath+"GlobalHistos.hpp");
  gSystem->Load(includePath+"TransverseSpherocityHistos.hpp");
  gSystem->Load(includePath+"TransverseSpherocityAnalyzer.hpp");
  gSystem->Load("libBasicGen.dylib");
}

void loadParticle(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Particle/";
  gSystem->Load(includePath+"ParticleHistos.hpp");
  gSystem->Load(includePath+"ParticleDerivedHistos.hpp");
  gSystem->Load(includePath+"ParticleAnalyzer.hpp");
  gSystem->Load(includePath+"ParticleDerivedHistogramCalculator.hpp");
  gSystem->Load("libParticle.dylib");
}

void loadPair(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Pair/";
  gSystem->Load(includePath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includePath+"ParticlePairHistos.hpp");
  gSystem->Load(includePath+"ParticlePairDerivedHistos.hpp");
  gSystem->Load(includePath+"ParticlePairCombinedHistos.hpp");
  gSystem->Load(includePath+"ParticlePairDerivedHistogramCalculator.hpp");
  gSystem->Load(includePath+"BalanceFunctionCalculator.hpp");
  gSystem->Load("libPair.dylib");
}

void loadNuDyn(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/NuDyn/";
  gSystem->Load(includePath+"NuDynAnalyzer.hpp");
  gSystem->Load(includePath+"NuDynDerivedHistogramCalculator.hpp");
  gSystem->Load(includePath+"NuDynDerivedHistos.hpp");
  gSystem->Load(includePath+"NuDynHistos.hpp");
  gSystem->Load("libNuDyn.dylib");
}

void loadSubSample(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/SubSample/";
  gSystem->Load(includePath+"SubSampleStatCalculator.hpp");
  gSystem->Load(includePath+"SubSampleStatIterator.hpp");
  gSystem->Load("libSubSample.dylib");
}
