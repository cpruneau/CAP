//==============================================================================
//  calculate the thermodynamic quantities of hadron resonance gas
//==============================================================================
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
using namespace std;


int RunHadronGasSimulation()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskIterator.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"Collection.hpp");
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"Particle.hpp");
  gSystem->Load(includesPath+"ParticleType.hpp");
  gSystem->Load(includesPath+"ParticleTypeCollection.hpp");
  gSystem->Load(includesPath+"ParticleDecayMode.hpp");
  gSystem->Load(includesPath+"ParticleDecayerTask.hpp");
  gSystem->Load(includesPath+"ParticleAnalyzerV2.hpp");
  gSystem->Load(includesPath+"ParticlePairAnalyzerV2.hpp");
  gSystem->Load(includesPath+"RadialBoostTask.hpp");
  gSystem->Load(includesPath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libBase.dylib");

  //  includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/WacPythia/";
  //  gSystem->Load(includesPath+"PythiaConfiguration.hpp");
  //  gSystem->Load(includesPath+"PythiaEventGenerator.hpp");
  //  gSystem->Load(includesPath+"PythiaEventReader.hpp");
  //  gSystem->Load("libWacPythia.dylib");

  includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/HadronGas/";
  gSystem->Load(includesPath+"HadronGas.hpp");
  gSystem->Load(includesPath+"HadronGasGeneratorConfiguration.hpp");
  gSystem->Load(includesPath+"HadronGasHistograms.hpp");
  gSystem->Load(includesPath+"HadronGasVsTempHistograms.hpp");
  gSystem->Load("libHadronGas.dylib");

  MessageLogger::LogLevel messageLevel =  MessageLogger::Info; // MessageLogger::Debug; //
  unsigned long nEventRequested   = 10000000-1;
  unsigned long nEventReported    =  1000000;
  unsigned long nEventPartialSave =  1000000;
  bool    partialSave             = true;
  bool    subsampleAnalysis       = true;
  double  minBias                 = true; // alternative is AliceV0
  TString outputFileNameBase      = "HG_Isotropic_200MeV_";
  TString inputPathName           = getenv("WAC_INPUT_PATH");
  TString outputPathName          = getenv("WAC_OUTPUT_PATH");
  inputPathName  += "/HG/";
  outputPathName = "/Volumes/ClaudeDisc4/OutputFiles/HG/Wide/";
  gSystem->mkdir(outputPathName,1);

  std::cout << "==================================================================================" << std::endl;
  std::cout << "Hadron Gas Model Analysis " << endl;
  std::cout << "==================================================================================" << std::endl;
  TString pdgDataTable = getenv("WAC_SRC");
  pdgDataTable += "/EOS/pdg.dat";
  ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection(); //ParticleTypeCollection::getMasterParticleCollection();
  particles->readFromFile(pdgDataTable);
  //particles->printProperties(std::cout);
  ParticleTypeCollection * stableParticles = particles->extractCollection(1);
  //stableParticles->printProperties(std::cout);

//  ofstream decayList;
//  decayList.open ("fullDecayList.tex");
//  particles->printDecayProperties(decayList);
//  decayList.close();

  CollisionGeometryConfiguration * cggConfig = new CollisionGeometryConfiguration();
  cggConfig->forceHistogramsRewrite = true;
  cggConfig->nnCrossSection         = 6.4; // cross section must be adjusted for beam energy.
  cggConfig->outputPath             = outputPathName;
  cggConfig->rootOuputFileName      = outputFileNameBase;

  HadronGasGeneratorConfiguration * hggc = new HadronGasGeneratorConfiguration();
  hggc->nKineticTemp    = 1;
  hggc->minKineticTemp  = 0.160;
  hggc->maxKineticTemp  = 0.160;
  hggc->kineticTemp     = 0.160;
  hggc->nChemicalTemp   = 1;
  hggc->minChemicalTemp = 0.200;
  hggc->maxChemicalTemp = 0.200;
  hggc->chemicalTemp    = 0.200;
  hggc->nMuB            = 1;
  hggc->minMuB          = 0.0;
  hggc->maxMuB          = 0.0;
  hggc->muB             = 0.0;
  hggc->nMuS            = 0.0;
  hggc->minMuS          = 0.0;
  hggc->maxMuS          = 0.0;
  hggc->muS             = 0.0;
  hggc->volume          = 0.0;
  hggc->minTotalMult    = 20;
  hggc->maxTotalMult    = 50;
  hggc->minY            = -4.0;
  hggc->maxY            =  4.0;
  hggc->generatorType   = MomentumGenerator::CylindricalMaxwellPtUniformY;
  hggc->standaloneMode      = true;
  hggc->useEventStream0     = true;
  hggc->dataInputUsed       = false;
  hggc->dataInputFileName   = "";
  hggc->createHistograms    = false;
  hggc->scaleHistograms     = false;
  hggc->saveHistograms      = true;
  hggc->subsampleAnalysis   = false;
  hggc->partialSave         = false;
  hggc->outputPath          = outputPathName;
  hggc->rootOuputFileName   = outputFileNameBase;


  GlobalAnalyzerConfiguration * gConfig = new GlobalAnalyzerConfiguration();
  gConfig->useEventStream0             = true;
  gConfig->loadHistograms              = false;
  gConfig->createHistograms            = true;
  gConfig->scaleHistograms             = true;
  gConfig->calculateDerivedHistograms  = false;
  gConfig->saveHistograms              = true;
  gConfig->forceHistogramsRewrite      = true;
  gConfig->subsampleAnalysis           = subsampleAnalysis;
  gConfig->partialSave                 = partialSave;
  gConfig->outputPath                  = outputPathName;
  gConfig->rootOuputFileName           = outputFileNameBase;
  gConfig->countParticles              = true;
  gConfig->setEvent                    = true;
  gConfig->fillCorrelationHistos       = false;
  gConfig->nBins_n  = 400;
  gConfig->nBins_n2 = 40;
  gConfig->min_n    = 0.0;
  gConfig->max_n    = 400;
  gConfig->nBins_e  = 200;
  gConfig->nBins_e2 = 20;
  gConfig->min_e    = 0.0;
  gConfig->max_e    = 1000.0;
  gConfig->nBins_q  = 200;
  gConfig->nBins_q2 = 20;
  gConfig->min_q    = -20.0;
  gConfig->max_q    = 20.0;
  gConfig->nBins_b  = 200;
  gConfig->nBins_b2 = 200;
  gConfig->min_b    = -20.0;
  gConfig->max_b    = 20.0;
  gConfig->validate();
  GlobalAnalyzerConfiguration * gConfig2 = new GlobalAnalyzerConfiguration(*gConfig);
  gConfig2->setEvent  = false;


  ParticleAnalyzerConfiguration * p1Config = new ParticleAnalyzerConfiguration();
  p1Config->useEventStream0             = true;
  p1Config->loadHistograms              = false;
  p1Config->createHistograms            = true;
  p1Config->scaleHistograms             = true;
  p1Config->calculateDerivedHistograms  = true;
  p1Config->saveHistograms              = true;
  p1Config->forceHistogramsRewrite      = true;
  p1Config->partialSave                 = partialSave;
  p1Config->subsampleAnalysis           = subsampleAnalysis;
  p1Config->inputPath                   = inputPathName;
  p1Config->outputPath                  = outputPathName;
  p1Config->rootOuputFileName           = outputFileNameBase;

  p1Config->fillEta     = false;
  p1Config->fillY       = true;
  p1Config->fillP2      = false;
  p1Config->nBins_n1    = 200;
  p1Config->min_n1      = 0.0;
  p1Config->max_n1      = 200.0;
  p1Config->nBins_eTot  = 100;
  p1Config->min_eTot    = 0.0;
  p1Config->max_eTot    = 1000.0;
  p1Config->nBins_pt    = 400;
  p1Config->min_pt      = 0.00;
  p1Config->max_pt      = 4.0;
  p1Config->nBins_eta   = 40;
  p1Config->min_eta     = -2;
  p1Config->max_eta     =  2;
  p1Config->nBins_y     = 40;
  p1Config->min_y       = -2;
  p1Config->max_y       = 2;
  p1Config->nBins_phi   = 36;
  p1Config->min_phi     = 0.0;
  p1Config->max_phi     = 2.0*3.1415927;
  p1Config->validate();

//  nBins_n1(100),          min_n1(0.0),    max_n1(10000.0), range_n1(10000.0),
//  nBins_eTot(100),        min_eTot(0.0),  max_eTot(1.0E6), range_eTot(1.0E6),
  RadialBoostConfiguration * rbc = new RadialBoostConfiguration();
  rbc->useEventStream0   = true;
  rbc->loadHistograms    = true;
  rbc->createHistograms  = true;
  rbc->scaleHistograms   = true;
  rbc->saveHistograms         = true;
  rbc->forceHistogramsRewrite = true;
  rbc->param_a     = 0.4;
  rbc->param_b     = 1.0;
  rbc->betaMaximum = 0.999;
  rbc->baseName    = "CGGA";// CG/PbPb/PbPb_Geom_Gradients.root";
  rbc->inputPath   = "/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles/CG/PbPb/"; //  PbPb_Geom_Gradients.root;
  rbc->rootInputFileName = "Geom_CGGA_Gradients.root";
  rbc->outputPath        = outputPathName;
  rbc->rootOuputFileName = outputFileNameBase;

  ParticlePairAnalyzerConfiguration * p2Config = new ParticlePairAnalyzerConfiguration();
  p2Config->useEventStream0             = true;
  p2Config->createHistograms            = true;
  p2Config->scaleHistograms             = true;
  p2Config->calculateDerivedHistograms  = true;
  p2Config->calculateCombinedHistograms = false;
  p2Config->saveHistograms              = true;
  p2Config->forceHistogramsRewrite      = true;
  p2Config->partialSave                 = partialSave;
  p2Config->subsampleAnalysis           = subsampleAnalysis;
  p2Config->inputPath                   = inputPathName;
  p2Config->outputPath                  = outputPathName;
  p2Config->rootOuputFileName           = outputFileNameBase;
  p2Config->fillEta     = false;
  p2Config->fillY       = true;
  p2Config->fillP2      = false;
  p2Config->nBins_n1    = 100;
  p2Config->min_n1      = 0.0;
  p2Config->max_n1      = 100.0;
  p2Config->nBins_n2    = 100;
  p2Config->min_n2      = 0.0;
  p2Config->max_n2      = 400.0;
  p2Config->nBins_pt    = 400;
  p2Config->min_pt      = 0.00;
  p2Config->max_pt      = 4.0;
  p2Config->nBins_eta   = 40;
  p2Config->min_eta     = -2;
  p2Config->max_eta     = 2;
  p2Config->nBins_y     = 40;
  p2Config->min_y       = -2;
  p2Config->max_y       = 2;
  p2Config->nBins_phi   = 36;
  p2Config->min_phi     = 0.0;
  p2Config->max_phi     = 2.0*3.1415927;
  p2Config->validate();

///
  ParticlePairAnalyzerConfiguration * p2ConfigHadron = new ParticlePairAnalyzerConfiguration();
  p2ConfigHadron->useEventStream0             = true;
  p2ConfigHadron->createHistograms            = true;
  p2ConfigHadron->scaleHistograms             = true;
  p2ConfigHadron->calculateDerivedHistograms  = true;
  p2ConfigHadron->calculateCombinedHistograms = false;
  p2ConfigHadron->saveHistograms              = true;
  p2ConfigHadron->forceHistogramsRewrite      = true;
  p2ConfigHadron->partialSave                 = partialSave;
  p2ConfigHadron->subsampleAnalysis           = subsampleAnalysis;
  p2ConfigHadron->inputPath                   = inputPathName;
  p2ConfigHadron->outputPath                  = outputPathName;
  p2ConfigHadron->rootOuputFileName           = outputFileNameBase;
  p2ConfigHadron->fillEta     = true;
  p2ConfigHadron->fillY       = false;
  p2ConfigHadron->fillP2      = false;
  p2ConfigHadron->nBins_n1    = 100;
  p2ConfigHadron->min_n1      = 0.0;
  p2ConfigHadron->max_n1      = 100.0;
  p2ConfigHadron->nBins_n2    = 100;
  p2ConfigHadron->min_n2      = 0.0;
  p2ConfigHadron->max_n2      = 400.0;
  p2ConfigHadron->nBins_pt    = 400;
  p2ConfigHadron->min_pt      = 0.00;
  p2ConfigHadron->max_pt      = 4.0;
  p2ConfigHadron->nBins_eta   = 40;
  p2ConfigHadron->min_eta     = -2;
  p2ConfigHadron->max_eta     = 2;
  p2ConfigHadron->nBins_y     = 40;
  p2ConfigHadron->min_y       = -2;
  p2ConfigHadron->max_y       = 2;
  p2ConfigHadron->nBins_phi   = 36;
  p2ConfigHadron->min_phi     = 0.0;
  p2ConfigHadron->max_phi     = 2.0*3.1415927;
  p2ConfigHadron->validate();


  cout << "Now, let's setup the pdg list" << endl;

  vector<int> allPdgCodes;     allPdgCodes = particles->getListOfPdgCodes();
  vector<int> stablePdgCodes;  stablePdgCodes = stableParticles->getListOfPdgCodes();

  EventFilter * openEventFilter = new EventFilter(EventFilter::MinBias,0.0,0.0);
  vector<EventFilter*>    eventFilters;
  eventFilters.push_back( openEventFilter);

  double minPt  =  0.001;
  double maxPt  = 10.0;
  double minEta = -2.0;
  double maxEta =  2.0;

  ParticleFilter * openPartFilter  = new ParticleFilter(false, 0.0000, 100.0, false, -4.0, 4.0, false, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::AllCharges,ParticleFilter::Live);
  vector<ParticleFilter*> particleFilterSet0;
  particleFilterSet0.push_back( openPartFilter);
  vector<ParticleFilter*>  particleFiltersAllHG       = ParticleFilter::createMultiplePdgFilters(    true, minPt, maxPt, true, minEta, maxEta, false, minEta, maxEta,allPdgCodes);
  vector<ParticleFilter*>  particleFiltersStable      = ParticleFilter::createMultiplePdgFilters(    true, minPt, maxPt, true, minEta, maxEta, false, minEta, maxEta,stablePdgCodes);
  vector<ParticleFilter*>  particleFiltersChargedH    = ParticleFilter::createChargedHadronFilters(  true, minPt, maxPt, true, minEta, maxEta, false, minEta, maxEta);
  vector<ParticleFilter*>  particleFiltersChargedPiKP = ParticleFilter::createPlusMinusHadronFilters(true, minPt, maxPt,false, minEta, maxEta, true,  minEta, maxEta);

//  vector<EventFilter*> eventFiltersRadialBoost;
//  vector<double> limits = { 0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
//  eventFiltersRadialBoost = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);

  cout << "Now, instantiating tasks..." << endl;

  //CollisionGeometryGenerator * collisionGeomGenerator = new CollisionGeometryGenerator("CGG", cggConfig,eventFilters,messageLevel);
  HadronGasGeneratorTask     * hadronGasGeneratorTask = new HadronGasGeneratorTask("HadronGasGenerator",hggc,messageLevel);
  ParticleDecayerTask        * decayer                = new ParticleDecayerTask();
  RadialBoostTask            * r1                     = new RadialBoostTask ("R1", rbc,       eventFilters, messageLevel);
  GlobalAnalyzer             * gND                    = new GlobalAnalyzer  ("gND",gConfig,   eventFilters, particleFilterSet0, messageLevel);  // generated
  GlobalAnalyzer             * gWD                    = new GlobalAnalyzer  ("gWD",gConfig2,  eventFilters, particleFilterSet0, messageLevel);  // boosted
  //GlobalAnalyzer   * g1Bs    = new GlobalAnalyzer  ("G1Bs",ga,  eventFilters, particleFilterSet0, messageLevel);  // boosted
  //ParticleAnalyzerV2 * p1All             = new ParticleAnalyzerV2("P1All",        p1Config,  eventFilters, particleFiltersAllHG,         messageLevel);  // analysis w/o decays
  ParticleAnalyzerV2 * p1Stable0         = new ParticleAnalyzerV2("P1Stable0",    p1Config,  eventFilters, particleFiltersStable,        messageLevel);  // analysis w/o decays
  ParticleAnalyzerV2 * p1Stable          = new ParticleAnalyzerV2("P1Stable",     p1Config,  eventFilters, particleFiltersStable,        messageLevel);  // analysis w/o decays
  ParticleAnalyzerV2 * p1ChargedH0       = new ParticleAnalyzerV2("P1ChargedH0",  p1Config,  eventFilters, particleFiltersChargedH,      messageLevel);
  ParticleAnalyzerV2 * p1ChargedH        = new ParticleAnalyzerV2("P1ChargedH",   p1Config,  eventFilters, particleFiltersChargedH,      messageLevel);
  ParticleAnalyzerV2 * p1ChargedPiKP0    = new ParticleAnalyzerV2("P1ChargedPiKP0",p1Config,  eventFilters, particleFiltersChargedPiKP,   messageLevel);
  ParticleAnalyzerV2 * p1ChargedPiKP     = new ParticleAnalyzerV2("P1ChargedPiKP",p1Config,  eventFilters, particleFiltersChargedPiKP,   messageLevel);
  //ParticlePairAnalyzerV2 * p2Stable      = new ParticlePairAnalyzerV2("P2Stable",      p2Config, eventFilters, particleFiltersStable,      messageLevel);
  ParticlePairAnalyzerV2 * p2ChargedH    = new ParticlePairAnalyzerV2("P2ChargedH",    p2ConfigHadron, eventFilters, particleFiltersChargedH,    messageLevel);
  ParticlePairAnalyzerV2 * p2ChargedPiKP = new ParticlePairAnalyzerV2("P2ChargedPiKP", p2Config, eventFilters, particleFiltersChargedPiKP, messageLevel);

  TaskIterator * masterTask = new TaskIterator();
  masterTask->setName("IteratorTask");
  masterTask->setNEventRequested(nEventRequested);
  masterTask->setNEventReported(nEventReported);
  masterTask->setReportLevel(messageLevel);
  masterTask->setNEventPartialSave(nEventPartialSave);
  masterTask->setPartialSave(partialSave);
  masterTask->setSubsampleAnalysis(subsampleAnalysis);
  //masterTask->addSubtask( collisionGeomGenerator );
  masterTask->addSubtask( hadronGasGeneratorTask );
  masterTask->addSubtask( gND      );
  //masterTask->addSubtask( p1ChargedPiKP0 );
  masterTask->addSubtask( decayer     );
  //masterTask->addSubtask( gWD      );
  //masterTask->addSubtask( p1ChargedPiKP  );
  //masterTask->addSubtask( p2Stable      );
  masterTask->addSubtask( p2ChargedH    );
  //masterTask->addSubtask( p2ChargedPiKP );
  masterTask->run();
  return 0;
}


//
