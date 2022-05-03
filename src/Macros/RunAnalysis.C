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

int RunAnalysis()
{
  TString includesPath = getenv("CAP_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"Timer.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskIterator.hpp");
  gSystem->Load(includesPath+"Collection.hpp");
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load(includesPath+"Particle.hpp");
  gSystem->Load(includesPath+"ParticleType.hpp");
  gSystem->Load(includesPath+"ParticleTypeCollection.hpp");
  gSystem->Load(includesPath+"ParticleDecayMode.hpp");
  gSystem->Load(includesPath+"ParticleAnalyzer.hpp");
  gSystem->Load(includesPath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includesPath+"NuDynAnalyzer.hpp");
  gSystem->Load(includesPath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libBase.dylib");

  includesPath = getenv("CAP_SRC");
  includesPath += "/CAPPythia/";
  gSystem->Load(includesPath+"PythiaEventGenerator.hpp");
  gSystem->Load("libCAPPythia.dylib");
  // ==================================================================================

  std::cout << "==================================================================================" << std::endl;
  std::cout << "MC Model Analysis" << endl;
  std::cout << "==================================================================================" << std::endl;

  
  bool YES = true;
  bool NO  = false;
  TString globalLabel     = "G";
  TString spherocityLabel = "S";
  TString partLabel       = "Part";
  TString pairLabel       = "Pair";
  TString nuDynLabel      = "NuDyn";

  MessageLogger::LogLevel infoLevel  = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Debug;
  MessageLogger::LogLevel selectedLevel = infoLevel;
  
  long    nEventRequested     = 100000-1;
  long    nEventReported      = 1000000;
  long    nEventPartialSave   = 1000000;
  bool    doPartialSave       = NO;
  bool    doSubsampleAnalysis = NO;
  double  beamEnergy          = 13000.0; // GeV
  int     beamPdgCode         = 2212;    // proton
  int     targetPdgCode       = 2212;
  int     globalEventFilterOptions   = 1;
  int     analysisParticleFilterOption = 6;
  int     analysisEventFilterOptions   = 2;

  bool runPythiaGenerator    = YES;
  bool runHerwigGenerator    = NO;
  bool runAmptGenerator      = NO;
  bool runEposGenerator      = NO;
  bool runUrqmdGenerator     = NO;
  bool runHijingGenerator    = NO;
  bool runGausResGeneator    = NO;
  bool runGlobalAnalysis     = YES;
  bool runSpherocityAnalysis = YES;
  bool runPartAnalysis       = NO;
  bool runPairAnalysis       = NO;
  bool runNuDynAnalysis      = NO;
  bool loadPdgTable          = YES;
  
  TString inputPathBase   = "/Volumes/ClaudeDisc4/OutputFiles/";
  TString outputPathBase  = "/Volumes/ClaudeDisc4/OutputFiles/";
  TString inputFileNameBase;
  TString outputFileNameBase;
  TString fileNameSuffix;

  TString inputPathName   = inputPathBase;
  TString outputPathName  = outputPathBase;
 
  if (runPythiaGenerator)
    {
    TString pythiaLabel = "PYTHIA_";
    TString systemLabel = "";
    TString energyLabel = "";
    if (beamPdgCode    == 2212) systemLabel += "P";
    if (targetPdgCode  == 2212) systemLabel += "P";
    energyLabel += int(beamEnergy);
    energyLabel += "/";
    inputPathName  += pythiaLabel;
    inputPathName  += "/";
    inputPathName  += systemLabel;
    inputPathName  += "/";
    inputPathName  += energyLabel;
    inputPathName  += "/";
    outputPathName  += pythiaLabel;
    outputPathName  += "/";
    inputPathName  += systemLabel;
    inputPathName  += "/";
    outputPathName  += energyLabel;
    outputPathName  += "/";
    inputFileNameBase  = "PYTHIA_";
    outputFileNameBase = "PYTHIA_";
    }
  if (runHerwigGenerator)
    {
    std::cout << "Option runHerwigGenerator not currently available" << std::endl;
    return 1;
    }
  if (runAmptGenerator )
    {
    std::cout << "Option runAmptGenerator not currently available" << std::endl;
    return 1;
    }
  if (runEposGenerator)
    {
    std::cout << "Option runEposGenerator not currently available" <<  std::endl;
    return 1;
    }
  if (runUrqmdGenerator)
    {
    std::cout << "Option runUrqmdGenerator not currently available" <<  std::endl;
    return 1;
    }
  if (runHijingGenerator)
    {
    std::cout << "Option runHijingGenerator not currently available" <<  std::endl;
    return 1;
    }
  if (runGausResGeneator)
    {
    TString gausResLabel = "RhoDecay";
    inputPathName  += gausResLabel;
    inputPathName  += "/";
    outputPathName += gausResLabel;
    outputPathName += "/";
    inputFileNameBase  = "RhoDecay_";
    outputFileNameBase = "RhoDecay_";
    }
  gSystem->mkdir(outputPathName,1);
  
  if (loadPdgTable)
    {
    TString pdgDataTable = getenv("CAP_SRC");
    pdgDataTable += "/EOS/pdg.dat";
    ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection(); //ParticleTypeCollection::getMasterParticleCollection();
    particles->readFromFile(pdgDataTable);
    if (selectedLevel == debugLevel) particles->printProperties(std::cout);
    }
 
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

 
  switch (analysisParticleFilterOption)
    {
      default:
      case 0:  analysisParticleFilters.push_back( openParticleFilter      ); break;
      case 1:  analysisParticleFilters.push_back( aliceV0ParticleFilter   ); break;
      case 2:  analysisParticleFilters.push_back( aliceTpcParticleFilter ); break;
      case 3:  analysisParticleFilters = ParticleFilter::createChargedHadronFilters(YES,0.2, 10.0, YES,-6.0, 6.0, NO, 10.0, -10.0); break;
      case 4:  analysisParticleFilters = ParticleFilter::createChargedHadronFilters(YES,0.2, 2.0, YES,-1.0, 1.0, NO, 10.0, -10.0); break;
      case 5:  analysisParticleFilters = ParticleFilter::createPlusMinusHadronFilters(YES,0.2, 2.0, YES,-2.0, 2.0, NO, 10.0, -10.0); break;
      case 6:  analysisParticleFilters = ParticleFilter::createPlusMinusHadronFilters(YES,0.0, 100.0, YES,-4.0, 4.0, NO, 10.0, -10.0); break;
      //case 6:  analysisParticleFilters = ParticleFilter::createBaryonFilters(YES,0.2, 10.0, YES, -1.0, 1.0, NO, 10.0, -10.0); break;
    }

  
  std::cout << "==================================================================================" << std::endl;
  std::cout << "Setup Completed - Proceed with loading analysis" << std::endl;
  std::cout << "==================================================================================" << std::endl;

  Configuration iteratorConfig("Event Iterator Configuration");
  iteratorConfig.addParameter( "nEventsRequested",   nEventRequested);
  iteratorConfig.addParameter( "nEventReported",     nEventReported);
  iteratorConfig.addParameter( "nEventPartialSave",  nEventPartialSave);
  iteratorConfig.addParameter( "doPartialSave",      doPartialSave);
  iteratorConfig.addParameter( "doSubsampleAnalysis",doSubsampleAnalysis);
  TaskIterator * masterTask = new TaskIterator("MultiTaskIterator",iteratorConfig,infoLevel);

  if (runPythiaGenerator)
    {
    Configuration pythiaConfig("Pythia Configuration");
    pythiaConfig.addParameter("beam",    2212);  // PDG Code   proton is 2212
    pythiaConfig.addParameter("target",  2212);
    pythiaConfig.addParameter("energy",  beamEnergy);
    pythiaConfig.addParameter("useEventStream0",    YES);
    pythiaConfig.addParameter("standaloneMode",     YES);   // use interactions generated by the geometry generator
    pythiaConfig.addParameter("dataOutputUsed",      NO);
    pythiaConfig.addParameter("dataConversionToWac",YES);
    pythiaConfig.addParameter("dataInputUsed",    NO);
    pythiaConfig.addParameter("saveHistograms",   NO);
    pythiaConfig.addParameter("option0",  TString("Init:showChangedSettings = on") );      // list changed settings
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
    //if (selectedLevel == debugLevel) pythiaConfig.printConfiguration(cout);
    vector<EventFilter*> pythiaEventFilters;
    pythiaEventFilters.push_back( openEventFilter);
    vector<ParticleFilter*> pythiaParticleFilters;
    pythiaParticleFilters.push_back( openParticleFilter );
    PythiaEventGenerator   * pythiaEventGen   = new PythiaEventGenerator("PythiaEventGenerator",pythiaConfig,pythiaEventFilters,pythiaParticleFilters,selectedLevel);
    masterTask->addSubtask( pythiaEventGen  );
    }
  if (runHerwigGenerator)
    {
    std::cout << "Option runHerwigGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runAmptGenerator )
    {
    std::cout << "Option runAmptGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runEposGenerator)
    {
    std::cout << "Option runEposGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runUrqmdGenerator)
    {
    std::cout << "Option runUrqmdGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runHijingGenerator)
    {
    std::cout << "Option runHijingGenerator not currently available"  << std::endl;
    return 1;
    }
  if (runGausResGeneator)
    {
    Configuration resConfig("Resonance Generator Confguration");
    resConfig.addParameter("useEventStream0",      YES);
    resConfig.addParameter("standaloneMode",       YES);
    resConfig.addParameter("nParticlesMinimum",     10);
    resConfig.addParameter("nParticlesMaximum",     10);
    resConfig.addParameter("yMinimum",            -1.0);
    resConfig.addParameter("yMaximum",             1.0);
    resConfig.addParameter("pTslope",              1.0);
    resConfig.addParameter("mass",               0.400);
    vector<EventFilter*> eventFiltersR;
    eventFiltersR.push_back( openEventFilter);
    vector<ParticleFilter*>  particleFiltersR;
    particleFiltersR.push_back( openParticleFilter );
    ResonanceGenerator   * resGen  = new ResonanceGenerator(  "ResonanceGenerator",resConfig,    eventFiltersR,  particleFiltersR,  selectedLevel);
    masterTask->addSubtask( resGen );
    }
  
  if (runGlobalAnalysis)
    {
    Configuration globalConfig("Global Configuration");
    globalConfig.addParameter("useEventStream0",       YES);
    globalConfig.addParameter("forceHistogramsRewrite",YES);
    globalConfig.addParameter("doSubsampleAnalysis",   doSubsampleAnalysis);
    globalConfig.addParameter("doPartialSave",         doPartialSave);
    globalConfig.addParameter("scaleHistograms",       YES);
    globalConfig.addParameter("histoOutputPath",       outputPathName);
    globalConfig.addParameter("histoOutputFileName",   outputFileNameBase+globalLabel);
    globalConfig.addParameter("countParticles",        YES);
    globalConfig.addParameter("setEvent",              YES);
    globalConfig.addParameter("fillCorrelationHistos", NO);
    globalConfig.addParameter("nBins_n",        400);
    globalConfig.addParameter("nBins_n2",       400);
    globalConfig.addParameter("min_n",         -0.5);
    globalConfig.addParameter("max_n",        399.5);
    globalConfig.addParameter("nBins_e",        200);
    globalConfig.addParameter("nBins_e2",        20);
    globalConfig.addParameter("min_e",          0.0);
    globalConfig.addParameter("max_e",    1000000.0);
    globalConfig.addParameter("nBins_q",        201);
    globalConfig.addParameter("nBins_q2",        20);
    globalConfig.addParameter("min_q",       -100.5);
    globalConfig.addParameter("max_q",        100.5);
    globalConfig.addParameter("nBins_b",        41);
    globalConfig.addParameter("nBins_b2",       41);
    globalConfig.addParameter("min_b",        -20.5);
    globalConfig.addParameter("max_b",         20.5);
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
    
    GlobalAnalyzer * globalEventAna = new GlobalAnalyzer(globalLabel,globalConfig,globalEventFilters,globalParticleFilters,selectedLevel);
    masterTask->addSubtask( globalEventAna  );
    }
    
  if (runSpherocityAnalysis)
    {
    Configuration spherocityConfig("Global Configuration");
    spherocityConfig.addParameter("useEventStream0",       YES);
    spherocityConfig.addParameter("forceHistogramsRewrite",YES);
    spherocityConfig.addParameter("doSubsampleAnalysis",   doSubsampleAnalysis);
    spherocityConfig.addParameter("doPartialSave",         doPartialSave);
    spherocityConfig.addParameter("scaleHistograms",       YES);
    spherocityConfig.addParameter("histoOutputPath",       outputPathName);
    spherocityConfig.addParameter("histoOutputFileName",   outputFileNameBase+spherocityLabel);
    spherocityConfig.addParameter("countParticles",        YES);
    spherocityConfig.addParameter("setEvent",              NO);
    spherocityConfig.addParameter("fillCorrelationHistos", YES);
    
    spherocityConfig.addParameter("nSteps", 360);
    spherocityConfig.addParameter("fillS0", YES);
    spherocityConfig.addParameter("fillS1", NO);
    spherocityConfig.addParameter("fillS1VsS0",NO);

    spherocityConfig.addParameter("nBins_spherocity", 100);
    spherocityConfig.addParameter("min_spherocity",   0.0);
    spherocityConfig.addParameter("max_spherocity",   1.0);
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
    TransverseSpherocityAnalyzer * spherocityAna = new TransverseSpherocityAnalyzer(spherocityLabel,spherocityConfig,spherocityEventFilters,spherocityParticleFilters,selectedLevel);
    masterTask->addSubtask( spherocityAna  );
    }
  
  if (runPartAnalysis)
    {
    Configuration partConfig("Part Spectra Configuration");
    partConfig.addParameter("useEventStream0",         YES);
    partConfig.addParameter("forceHistogramsRewrite",  YES);
    partConfig.addParameter("doSubsampleAnalysis",     doSubsampleAnalysis);
    partConfig.addParameter("doPartialSave",           doPartialSave);
    partConfig.addParameter("scaleHistograms",         YES);
    partConfig.addParameter("histoOutputPath",         outputPathName);
    partConfig.addParameter("histoOutputFileName",     outputFileNameBase+partLabel);
    partConfig.addParameter("nBins_n1",  100);
    partConfig.addParameter("min_n1",    0.0);
    partConfig.addParameter("max_n1",  100.0);

    partConfig.addParameter("nBins_eTot",  100);
    partConfig.addParameter("min_eTot",    0.0);
    partConfig.addParameter("max_eTot",  100.0);

    partConfig.addParameter("nBins_pt",  1000);
    partConfig.addParameter("min_pt",    0.0);
    partConfig.addParameter("max_pt",  100.0);

    partConfig.addParameter("nBins_eta",  80);
    partConfig.addParameter("min_eta",   -4.0);
    partConfig.addParameter("max_eta",    4.0);
   
    partConfig.addParameter("nBins_phi",  72);
    partConfig.addParameter("min_phi",    0.0);
    partConfig.addParameter("max_phi",    TMath::TwoPi());

    partConfig.addParameter("fillEta",  YES);
    partConfig.addParameter("fillY",    NO);
    partConfig.addParameter("fillP2",   NO);

    ParticleAnalyzer * partAna = new ParticleAnalyzer(partLabel,partConfig,analysisEventFilters,analysisParticleFilters,selectedLevel);
    masterTask->addSubtask(partAna);
    }
  
  if (runPairAnalysis)
    {
    Configuration pairConfig("Pair Correlation Configuration");
    pairConfig.addParameter("useEventStream0",         YES);
    pairConfig.addParameter("forceHistogramsRewrite",  YES);
    pairConfig.addParameter("doSubsampleAnalysis",     doSubsampleAnalysis);
    pairConfig.addParameter("doPartialSave",           doPartialSave);
    pairConfig.addParameter("scaleHistograms",         YES);
    pairConfig.addParameter("histoOutputPath",         outputPathName);
    pairConfig.addParameter("histoOutputFileName",     outputFileNameBase+pairLabel);
    pairConfig.addParameter("nBins_n1",       100);
    pairConfig.addParameter("min_n1",         0.0);
    pairConfig.addParameter("max_n1",       100.0);
    pairConfig.addParameter("nBins_n2",       100);
    pairConfig.addParameter("min_n2",         0.0);
    pairConfig.addParameter("max_n2",       400.0);
    pairConfig.addParameter("nBins_pt",        40);
    pairConfig.addParameter("min_pt",        0.00);
    pairConfig.addParameter("max_pt",        20.0);
    pairConfig.addParameter("nBins_eta",       40);
    pairConfig.addParameter("min_eta",       -2.0); // 1;
    pairConfig.addParameter("max_eta",        2.0);  // 1;
    pairConfig.addParameter("nBins_phi",       72);
    pairConfig.addParameter("min_phi",        0.0);
    pairConfig.addParameter("max_phi",TMath::TwoPi());
    pairConfig.addParameter("fillEta",  YES);
    pairConfig.addParameter("fillY",    NO);
    pairConfig.addParameter("fillP2",   NO);
    ParticlePairAnalyzer * pairAna = new ParticlePairAnalyzer(pairLabel, pairConfig,analysisEventFilters, analysisParticleFilters,selectedLevel);
    masterTask->addSubtask( pairAna  );
    }
  
  if (runNuDynAnalysis)
    {
    Configuration nuDynConfig("NuDyn Correlation Configuration");
    nuDynConfig.addParameter("useEventStream0",         YES);
    nuDynConfig.addParameter("forceHistogramsRewrite",  YES);
    nuDynConfig.addParameter("doSubsampleAnalysis",     doSubsampleAnalysis);
    nuDynConfig.addParameter("doPartialSave",           doPartialSave);
    nuDynConfig.addParameter("scaleHistograms",         YES);
    nuDynConfig.addParameter("histoOutputPath",         outputPathName);
    nuDynConfig.addParameter("histoOutputFileName",     outputFileNameBase+nuDynLabel);
    nuDynConfig.addParameter("inputType",1);
    nuDynConfig.addParameter("pairOnly",true);
    nuDynConfig.addParameter("nBins_mult",200);
    nuDynConfig.addParameter("min_mult", 0.0);
    nuDynConfig.addParameter("max_mult",200.0);
    NuDynAnalyzer * nuDynAna = new NuDynAnalyzer(nuDynLabel,nuDynConfig,analysisEventFilters,analysisParticleFilters,selectedLevel);
    masterTask->addSubtask( nuDynAna );
    }
  
  std::cout << "==================================================================================" << std::endl;
  std::cout << "Configuration Completed - Run analysis" << std::endl;
  std::cout << "==================================================================================" << std::endl;


  masterTask->run();
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
