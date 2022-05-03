//==============================================================================
//  calculate the thermodynamic quantities of hadron resonance gas
//==============================================================================

#include<sys/time.h>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<cmath>
#include "TString.h"
#include "TSystem.h"
#include <TStyle.h>
#include <TROOT.h>
using namespace std;


int RunHadronGas()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  // "/Users/claudeapruneau/Documents/GitHub/WAC/Base/";
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
  gSystem->Load(includesPath+"ParticleAnalyzer.hpp");
  gSystem->Load(includesPath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includesPath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libBase.dylib");

//  includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/WacPythia/";
//  gSystem->Load(includesPath+"PythiaConfiguration.hpp");
//  gSystem->Load(includesPath+"PythiaEventGenerator.hpp");
//  gSystem->Load(includesPath+"PythiaEventReader.hpp");
//  gSystem->Load("libWacPythia.dylib");
  includesPath = getenv("WAC_SRC");
  includesPath += "/HadronGas/";
  //includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/HadronGas/";
  gSystem->Load(includesPath+"HadronGas.hpp");
  gSystem->Load(includesPath+"HadronGasConfiguration.hpp");
  gSystem->Load(includesPath+"HadronGasHistograms.hpp");
  gSystem->Load(includesPath+"HadronGasVsTempHistograms.hpp");
  gSystem->Load("libHadronGas.dylib");

  // ==================================================================================

  MessageLogger::LogLevel messageLevel = MessageLogger::Info; //MessageLogger::Debug; //
  unsigned long nEventRequested   = 1000;
  unsigned long nEventReported    = 100;
  unsigned long nEventPartialSave = 500;
  bool    partialSave             = true;
  bool    subsampleAnalysis       = true;
  double  beamEnergy              = 7000.0; // GeV
  double  minBias                 = true; // alternative is AliceV0
  TString outputFileNameBase      = "HG_160MeV_";
  TString inputPathName           = getenv("WAC_INPUT_PATH");
  TString outputPathName          = getenv("WAC_OUTPUT_PATH");
  inputPathName  += "/HG/";
  outputPathName += "/HG/SmallTest/";
  gSystem->mkdir(outputPathName,1);

  std::cout << "==================================================================================" << std::endl;
  std::cout << "Hadron Gas Model Analysis " << endl;
  std::cout << "==================================================================================" << std::endl;
  TString pdgDataTable = getenv("WAC_SRC");
  pdgDataTable += "/EOS/pdg.dat";
  ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection(); //ParticleTypeCollection::getMasterParticleCollection();
  particles->readFromFile(pdgDataTable);
  particles->printProperties(std::cout);

  ofstream decayList;
  decayList.open ("fullDecayList.tex");
  particles->printDecayProperties(decayList);
  decayList.close();

//  ParticleTypeCollection * mesons = particles->extractCollection(8);
//  ofstream mesonsList;
//  mesonsList.open ("mesonsList.tex");
//  mesons->printDecayProperties(mesonsList);
//  mesonsList.close();
//
//  ParticleTypeCollection * baryons = particles->extractCollection(5);
//  ofstream baryonsList;
//  baryonsList.open ("baryonsList.tex");
//  baryons->printDecayProperties(baryonsList);
//  baryonsList.close();


  ParticleTypeCollection * stableParticles = particles->extractCollection(1);
  stableParticles->printProperties(std::cout);
  HadronGas * hadronGas = new HadronGas(particles,stableParticles,MessageLogger::Info);
  double temperature;
  double muB = 0.0;
  double muS = 0.0;

    // return 0;
  HadronGasConfiguration hadronGasConfig;
  hadronGasConfig.pdgFileName  = pdgDataTable;
  hadronGasConfig.nT           = 10;
  hadronGasConfig.minT         = 0.100;
  hadronGasConfig.maxT         = 0.200;
  hadronGasConfig.nMass        = 120;
  hadronGasConfig.minMass      = 0.0;
  hadronGasConfig.maxMass      = 3.0;

  HadronGasVsTempHistograms * hgh = new HadronGasVsTempHistograms("HG_",&hadronGasConfig,MessageLogger::Warning);
  hgh->createHistograms();
  vector<HadronGasHistograms*> hadronGasHistos;
  vector<TString*> tempLabels;
  TString label;
  TString hName;
  HadronGasHistograms * hgHistos;
  double dT = (hadronGasConfig.maxT-hadronGasConfig.minT)/double(hadronGasConfig.nT);
  for (int iT=0;iT<=hadronGasConfig.nT;iT++)
  {
  temperature = hadronGasConfig.minT + double(iT)*dT;
  label = "T = ";
  label += int(1000*temperature);
  tempLabels.push_back( new TString(label+" MeV") ) ;
  hadronGas->calculateAllProperties(temperature,muB,muS);
  hgh->fill(*hadronGas);
  hName = "HGP_"; hName += iT;
  hgHistos = new HadronGasHistograms(hName,&hadronGasConfig,hadronGas,MessageLogger::Warning);
  hgHistos->createHistograms();
  hgHistos->fill(*hadronGas);
  hgHistos->calculateDerivedHistograms();
  hadronGasHistos.push_back(hgHistos);
  }

  CanvasConfiguration * canvasConfigurationPort = new CanvasConfiguration(CanvasConfiguration::Portrait,CanvasConfiguration::Linear);
  CanvasConfiguration * canvasConfiguration     = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  CanvasConfiguration * canvasConfigurationLogY = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::LogY);
  CanvasConfiguration * canvasConfigurationLogZ = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogZ);
  GraphConfiguration  ** graphConfigurations    = new GraphConfiguration*[40];
  GraphConfiguration  ** graphConfigurationsNoL = new GraphConfiguration*[40];
  GraphConfiguration  *  graphConfiguration2D   = new GraphConfiguration(2,1);
  for (int iGraph=0;iGraph<40;iGraph++)
    {
    graphConfigurations[iGraph] = new GraphConfiguration(1,iGraph%10);
    graphConfigurations[iGraph]->plotOption = "P0.L";
    graphConfigurationsNoL[iGraph] = new GraphConfiguration(1,iGraph%10);
    graphConfigurationsNoL[iGraph]->plotOption = "P";
    }

  TString inputPath  = "/Users/claudeapruneau/Documents/GitHub/run/PythiaStudies/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/WAC-DATA/OutputFiles/HGM/";
  graphConfigurations[0]->plotOption = "P1";
  Plotter * plotter = new Plotter("HadronGasPlotter", MessageLogger::Info);
  plotter->setDefaultOptions(1);

  plotter->plot("HGM_numberDensityVsT", canvasConfigurationLogY, graphConfigurations[0],
                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
                "#rho_{1}(fm^{-3})",0.01, 10.0,
                hgh->energyDensityVsT,
                "#rho_{1}",0.22, 0.7, 0.4, 0.76, 0.055);
  plotter->plot("HGM_energyDensityVsT", canvasConfigurationLogY, graphConfigurations[0],
                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
                "#varepsilon (GeV/fm^{3})",0.01, 10.0,
                hgh->energyDensityVsT,
                "#varepsilon (GeV/fm^{3})",0.22, 0.7, 0.4, 0.76, 0.055);
  plotter->plot("HGM_entropyDensityVsT", canvasConfigurationLogY, graphConfigurations[0],
                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
                "s",0.1, 100.0,
                hgh->entropyDensityVsT,
                "s",0.22, 0.7, 0.4, 0.76, 0.055);

  plotter->plot("HGM_PressureVsT", canvasConfigurationLogY, graphConfigurations[0],
                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
                "Pressure",0.001, 1.0,
                hgh->pressureVsT,
                "P",0.22, 0.7, 0.4, 0.76, 0.055);

  double minYield = 1.0E-7;
  double maxYield = 1.0;

  int nGraphs = 8;
  TString ** legends = new TString*[nGraphs];
  TH1 ** histograms = new TH1*[nGraphs];
  histograms[0] =  hgh->energyDensityVsT;    legends[0] = new TString("ALL");
  histograms[1] =  hgh->nDensityVsT[1];     legends[1]    = new TString(hadronGas->stableParticleTypes->getParticleType(1)->getTitle());
  histograms[2] =  hgh->nDensityVsT[3];     legends[2]    = new TString(hadronGas->stableParticleTypes->getParticleType(3)->getTitle());//partLabel[3];
  histograms[3] =  hgh->nDensityVsT[7];     legends[3]    = new TString(hadronGas->stableParticleTypes->getParticleType(7)->getTitle());//partLabel[7];
  histograms[4] =  hgh->nDensityVsT[11];    legends[4]    = new TString(hadronGas->stableParticleTypes->getParticleType(11)->getTitle());//partLabel[11];
  histograms[5] =  hgh->nDensityVsT[13];    legends[5]    = new TString(hadronGas->stableParticleTypes->getParticleType(13)->getTitle());//partLabel[13];
  histograms[6] =  hgh->nDensityVsT[17];    legends[6]    = new TString(hadronGas->stableParticleTypes->getParticleType(17)->getTitle());//partLabel[17];
  histograms[7] =  hgh->nDensityVsT[21];    legends[7]    = new TString(hadronGas->stableParticleTypes->getParticleType(21)->getTitle());//partLabel[21];


  plotter->plot(nGraphs, "HGM_numberDensityAllVsT", canvasConfigurationLogY, graphConfigurationsNoL,
                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
                "#rho_{1}(fm^{-3})",1.0E-8, 10.0,
                histograms,legends,0.6, 0.18, 0.75, 0.45, 0.04);

  nGraphs = 5;
  histograms[0] = hadronGasHistos[0]->h_rho1Th; legends[0] = tempLabels[0];
  histograms[1] = hadronGasHistos[2]->h_rho1Th; legends[1] = tempLabels[2];
  histograms[2] = hadronGasHistos[4]->h_rho1Th; legends[2] = tempLabels[4];
  histograms[3] = hadronGasHistos[6]->h_rho1Th; legends[3] = tempLabels[6];
  histograms[4] = hadronGasHistos[8]->h_rho1Th; legends[4] = tempLabels[8];

  plotter->plot(nGraphs, "HGM_Stablerho1ThVsT",canvasConfigurationLogY,graphConfigurations,
                        "Species", 0.0, 18.0,
                        "#rho_{1}^{Th} (fm^{-3})", minYield,maxYield,
                        histograms,legends,0.2, 0.20, 0.45, 0.45, 0.04);

  histograms[0] = hadronGasHistos[0]->h_rho1; legends[0] = tempLabels[0];
  histograms[1] = hadronGasHistos[2]->h_rho1; legends[1] = tempLabels[2];
  histograms[2] = hadronGasHistos[4]->h_rho1; legends[2] = tempLabels[4];
  histograms[3] = hadronGasHistos[6]->h_rho1; legends[3] = tempLabels[6];
  histograms[4] = hadronGasHistos[8]->h_rho1; legends[4] = tempLabels[8];

  std::cout << "============================================================================= 6 =====" << std::endl;

  plotter->plot(nGraphs, "HGM_StableDecayYieldsVsT",canvasConfigurationLogY,graphConfigurations,
                        "Species", 0.0, 23.0,
                        "#rho_{1} (fm^{-3})", minYield,maxYield,
                        histograms,legends,0.2, 0.20, 0.45, 0.45, 0.04);


  histograms[0] = hadronGasHistos[0]->h_rho1ToRho1Th; legends[0] = tempLabels[0];
  histograms[1] = hadronGasHistos[2]->h_rho1ToRho1Th; legends[1] = tempLabels[2];
  histograms[2] = hadronGasHistos[4]->h_rho1ToRho1Th; legends[2] = tempLabels[4];
  histograms[3] = hadronGasHistos[6]->h_rho1ToRho1Th; legends[3] = tempLabels[6];
  histograms[4] = hadronGasHistos[8]->h_rho1ToRho1Th; legends[4] = tempLabels[8];
  plotter->plot(nGraphs, "HGM_StableDecayToThermalVsT",canvasConfigurationLogY,graphConfigurations,
                        "Species", 0.0, 23.0,
                        "#rho_{1}/#rho_{1}^{Th}", 0.5,10.0,
                        histograms,legends,0.7, 0.60, 0.85, 0.85, 0.04);

  histograms[0] = (TH1*) hadronGasHistos[4]->h_rho1Th->Clone(); legends[0] = new TString(*tempLabels[4]+" MeV; Thermal Only");
  histograms[1] = (TH1*) hadronGasHistos[4]->h_rho1->Clone();   legends[1] = new TString(*tempLabels[4]+" MeV; Thermal + Decays");

  plotter->plot(2, "HGM_StableDecayVsrho1Th",canvasConfigurationLogY,graphConfigurations,
                        "Species", 0.0, 23.0,
                        "#rho_{1}", minYield,maxYield,
                        histograms,legends,0.2, 0.20, 0.45, 0.45, 0.04);
  canvasConfigurationLogZ->theta =  35.0;
  canvasConfigurationLogZ->phi   = -125.0;
  graphConfiguration2D->plotOption = "LEGO2";

  int jt = 9;

  hadronGasHistos[jt]->h_rho2Corr->GetXaxis()->SetNdivisions(23);
  hadronGasHistos[jt]->h_rho2Corr->GetYaxis()->SetNdivisions(23);
  plotter->plot("HGM_StableDecayCorrelatedPairYields",canvasConfigurationLogZ,graphConfiguration2D,
               "Species 1", 0.0, 23.0,
               "Species 2", 0.0, 23.0,
               "#rho_{2}^{Corr}", 1E-7, 1.0,
               hadronGasHistos[jt]->h_rho2Corr);


  hadronGasHistos[jt]->h_rho1rho1->GetXaxis()->SetNdivisions(23);
  hadronGasHistos[jt]->h_rho1rho1->GetYaxis()->SetNdivisions(23);
  plotter->plot("HGM_StableDecayPairYields",canvasConfigurationLogZ,graphConfiguration2D,
               "Species 1", 0.0, 23.0,
               "Species 2", 0.0, 23.0,
               "#rho_{1}#otimes#rho_{1}", 1E-10, 10.0,
               hadronGasHistos[jt]->h_rho1rho1);

  histograms[0] = hadronGasHistos[0]->h_rho1AllVsMass; legends[0] = tempLabels[0];
  histograms[1] = hadronGasHistos[2]->h_rho1AllVsMass; legends[1] = tempLabels[2];
  histograms[2] = hadronGasHistos[4]->h_rho1AllVsMass; legends[2] = tempLabels[4];
  histograms[3] = hadronGasHistos[6]->h_rho1AllVsMass; legends[3] = tempLabels[6];
  histograms[4] = hadronGasHistos[8]->h_rho1AllVsMass; legends[4] = tempLabels[8];
  plotter->plot(nGraphs,"HGM_AllYieldsVsMassVsT",canvasConfigurationLogY,graphConfigurations,
                        "Mass (GeV/c^{2})", 0.0, 2.50,
                        "#rho_{1}^{Th}/g", 1.0E-7,1.0E0,
                        histograms,legends,0.7, 0.60, 0.85, 0.85, 0.04);


  histograms[0] = hadronGasHistos[0]->h_rho1ThVsMass; legends[0] = tempLabels[0];
  histograms[1] = hadronGasHistos[2]->h_rho1ThVsMass; legends[1] = tempLabels[2];
  histograms[2] = hadronGasHistos[4]->h_rho1ThVsMass; legends[2] = tempLabels[4];
  histograms[3] = hadronGasHistos[6]->h_rho1ThVsMass; legends[3] = tempLabels[6];
  histograms[4] = hadronGasHistos[8]->h_rho1ThVsMass; legends[4] = tempLabels[8];
  plotter->plot(nGraphs,"HGM_Stablerho1ThVsMassVsT",canvasConfigurationLogY,graphConfigurations,
                        "Mass (GeV/c^{2})", 0.0, 2.50,
                        "#rho_{1}/g", 1.0E-7,1.0E0,
                        histograms,legends,0.7, 0.60, 0.85, 0.85, 0.04);


  graphConfigurations[0]->xLabelSize = 0.04;
  graphConfigurations[0]->plotOption = "COLZ";
  plotter->plot("HGM_BF_10", canvasConfiguration, graphConfigurations[0],
                "",  0.0, 18.0,
                "BF",-0.1, 0.5,
                hadronGasHistos[jt]->h_BF,
                "BF Integrals",0.65, 0.8, 0.8, 0.86, 0.035);

  plotter->plot("HGM_Rho2_10", canvasConfigurationLogZ, graphConfigurations[0],
                "Trigger Species", 0.0, 23.0,
                "Associate Species", 0.0, 23.0,
                "#rho_{2}", 1E-10, 1.0,
                hadronGasHistos[jt]->h_rho2);

  plotter->plot("HGM_rho1rho1_10", canvasConfigurationLogZ, graphConfigurations[0],
                "Trigger Species", 0.0, 23.0,
                "Associate Species", 0.0, 23.0,
                "#rho_{1}#otimes#rho_{1}", 1E-10, 1.0,
                hadronGasHistos[jt]->h_rho1rho1);

  plotter->plot("HGM_rho1thrho1th_10", canvasConfigurationLogZ, graphConfigurations[0],
                "Trigger Species", 0.0, 23.0,
                "Associate Species", 0.0, 23.0,
                "#rho_{1}^{th}#rho_{1}^{th}", 1E-10, 1.0,
                hadronGasHistos[jt]->h_rho1rho1);
  plotter->plot("HGM_C2_10", canvasConfigurationLogZ, graphConfigurations[0],
                "Trigger Species", 0.0, 23.0,
                "Associate Species", 0.0, 23.0,
                "C_{2}^{ij}", -1.0, 1.0,
                hadronGasHistos[jt]->h_C2);
  plotter->plot("HGM_R2_10", canvasConfigurationLogZ, graphConfigurations[0],
                "Trigger Species", 0.0, 23.0,
                "Associate Species", 0.0, 23.0,
                "R_{2}^{ij}", -1.0, 1.0,
                hadronGasHistos[jt]->h_R2);
  plotter->plot("HGM_UncorrelatedPairs", canvasConfigurationLogZ, graphConfigurations[0],
                "Trigger Species", 0.0, 23.0,
                "Associate Species", 0.0, 23.0,
                "Uncorrelated", -1.0, 1.0,
                hadronGasHistos[jt]->h_rho2Uncorr);

  plotter->printAllCanvas(outputPath);

  return 0;
  // =====================================
  // Configure iterator task and run it...
  // =====================================
//  TaskIterator * masterTask = new TaskIterator();
//  masterTask->setNEventRequested(nEventRequested);
//  masterTask->setNEventReported(nEventReported);
//  masterTask->setReportLevel(messageLevel);
//  masterTask->setNEventPartialSave(nEventPartialSave);
//  masterTask->setPartialSave(partialSave);
//  masterTask->setSubsampleAnalysis(subsampleAnalysis);
//
//
//  vector<EventFilter*> eventFilterGen;
//  eventFilterGen.push_back( new EventFilter(EventFilter::MinBias,0.0,0.0) );
//  vector<ParticleFilter*> particleFilterGen;
//  particleFilterGen.push_back( new ParticleFilter(0.001, 100.0, -6.0, 6.0, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::Charged));
//  vector<TString*> pythiaOptions;
//  pythiaOptions.push_back( new TString("Init:showChangedSettings = on") );      // list changed settings
//  pythiaOptions.push_back( new TString("Init:showChangedParticleData = off") ); // list changed particle data
//  pythiaOptions.push_back( new TString("Next:numberCount = 10000") );            // print message every n events
//  pythiaOptions.push_back( new TString("Next:numberShowInfo = 1") );            // print event information n times
//  pythiaOptions.push_back( new TString("Next:numberShowProcess = 0") );         // print process record n times
//  pythiaOptions.push_back( new TString("Next:numberShowEvent = 0") );
//  pythiaOptions.push_back( new TString("SoftQCD:inelastic = on") );             // All inelastic processes
//  //pythiaOptions.push_back( new TString("SoftQCD:all = on") );                   // Allow total sigma = elastic/SD/DD/ND
//                                                                                // pythiaOptions.push_back(  new TString("HardQCD:all = on");
//  PythiaConfiguration * pc = new PythiaConfiguration();
//  pc->beam                = 2212;  // PDG Code   proton is 2212
//  pc->target              = 2212;
//  pc->energy              = beamEnergy;
//  pc->options             = pythiaOptions;
//  pc->useEventStream0     = true;
//  pc->dataOutputUsed      = false;
//  pc->dataConversionToWac = true;
//  pc->dataOutputFileName  = "Pythia_pp_7000.root";
//  pc->dataOutputTreeName  = "PythiaTree";
//  pc->dataOutputPath      = outputPathName;
//  pc->dataInputUsed       = false;
//  pc->dataInputFileName   = "Pythia_pp_7000_10Mevents.root";
//  pc->dataInputTreeName   = "PythiaTree";
//  pc->dataInputPath       = getenv("WAC_INPUT_DATA_PATH");
//  pc->saveHistograms      = false;
//  PythiaEventGenerator * pythiaGenerator = new PythiaEventGenerator("PythiaEventGenerator",pc,eventFilterGen,particleFilterGen, messageLevel);
//  masterTask->addSubtask( pythiaGenerator );
//
//  GlobalAnalyzerConfiguration * ga = new GlobalAnalyzerConfiguration();
//  ga->useEventStream0             = true;
//  ga->loadHistograms              = false;
//  ga->createHistograms            = true;
//  ga->scaleHistograms             = true;
//  ga->calculateDerivedHistograms  = false;
//  ga->saveHistograms              = true;
//  ga->forceHistogramsRewrite      = true;
//  ga->subsampleAnalysis           = subsampleAnalysis;
//  ga->partialSave                 = partialSave;
//  ga->outputPath                  = outputPathName;
//  ga->rootOuputFileName           = outputFileNameBase;
//  ga->countParticles              = true;
//  ga->setEvent                    = true;
//  ga->fillCorrelationHistos       = true;
//  ga->nBins_n  = 400;
//  ga->nBins_n2 = 40;
//  ga->min_n    = 0.0;
//  ga->max_n    = 400;
//  ga->nBins_e  = 200;
//  ga->nBins_e2 = 20;
//  ga->min_e    = 0.0;
//  ga->max_e    = 1000.0;
//  ga->nBins_q  = 200;
//  ga->nBins_q2 = 20;
//  ga->min_q    = -20.0;
//  ga->max_q    = 20.0;
//  ga->nBins_b  = 200;
//  ga->nBins_b2 = 200;
//  ga->min_b    = -20.0;
//  ga->max_b    = 20.0;
//
//  ga->validate();
//  vector<EventFilter*>     globalEventFilters    = eventFilterGen;
//  vector<ParticleFilter*>  globalParticleFilters;
//  globalParticleFilters.push_back( new ParticleFilterAliceV0());
//  //globalParticleFilters.push_back( new ParticleFilter(0.001,100.0,-6.0,6.0,10.0,-10.0,ParticleFilter::Hadron, ParticleFilter::Charged) );
//  //globalParticleFilters.push_back( new ParticleFilter(0.200,100.0,-1.0,1.0,10.0,-10.0,ParticleFilter::Hadron, ParticleFilter::Charged) );
//  GlobalAnalyzer * globalAnalyzer = new GlobalAnalyzer("Global",ga,globalEventFilters,globalParticleFilters,messageLevel);
//  masterTask->addSubtask( globalAnalyzer );
//
//  ParticleAnalyzerConfiguration * ac = new ParticleAnalyzerConfiguration();
//  ac->useEventStream0             = true;
//  ac->loadHistograms              = false;
//  ac->createHistograms            = true;
//  ac->scaleHistograms             = true;
//  ac->calculateDerivedHistograms  = true;
//  ac->saveHistograms              = true;
//  ac->forceHistogramsRewrite      = true;
//  ac->partialSave                 = partialSave;
//  ac->subsampleAnalysis           = subsampleAnalysis;
//  ac->inputPath                   = inputPathName;
//  ac->outputPath                  = outputPathName;
//  ac->rootOuputFileName           = outputFileNameBase;
//  ac->nBins_pt    = 100;
//  ac->min_pt      = 0.00;
//  ac->max_pt      = 10.0;
//  ac->nBins_eta   = 80;
//  ac->min_eta     = -4;
//  ac->max_eta     = 4;
//  ac->nBins_y     = 80;
//  ac->min_y       = -4;
//  ac->max_y       = 4;
//  ac->nBins_phi   = 36;
//  ac->min_phi     = 0.0;
//  ac->max_phi     = 2.0*3.1415927;
//
//
//  vector<EventFilter*>     eventFilters;
//  vector<ParticleFilter*>  particleFilters;
//  vector<ParticleFilter*>  particleFilters2;
//  vector<double> limits;
//
//  // limits based on all charged particles in V0M
//
//
//  limits.push_back(0.0);
//  limits.push_back(7.0);   // 80-90%
//  limits.push_back(12.0);  // 60-70%
//  limits.push_back(20.0);  // 40-50%
//  limits.push_back(38.0);  // 20-30%
//  limits.push_back(400.0); // 0-10%
//  //eventFilters.push_back(  eventFilterGen[0] );                             // True minimum bias
//  eventFilters.push_back(  new EventFilter(EventFilter::ParticleFilter0,1.0,1000.0) );  // V0M based min bias at least one particle
//  vector<EventFilter*>  eventFilterPartitions = EventFilter::createEventFilterSet(EventFilter::ParticleFilter0, limits);
//  for (unsigned int k=0; k<eventFilterPartitions.size(); k++) eventFilters.push_back( eventFilterPartitions[k] );
//
//  particleFilters = ParticleFilter::createChargedHadronFilters(0.001, 10.0, -6.0, 6.0, 10.0, -10.0);
//  ParticleAnalyzer * particleAnalyzer  = new ParticleAnalyzer("P1", ac, eventFilters, particleFilters, messageLevel);
//  masterTask->addSubtask( particleAnalyzer );

//  ParticlePairAnalyzerConfiguration * ac2 = new ParticlePairAnalyzerConfiguration();
//  ac2->useEventStream0             = true;
//  ac2->createHistograms            = true;
//  ac2->scaleHistograms             = true;
//  ac2->calculateDerivedHistograms  = true;
//  ac2->calculateCombinedHistograms = true;
//  ac2->saveHistograms              = true;
//  ac2->forceHistogramsRewrite      = true;
//  ac2->partialSave                 = partialSave;
//  ac2->subsampleAnalysis           = subsampleAnalysis;
//  ac2->inputPath                   = inputPathName;
//  ac2->outputPath                  = outputPathName;
//  ac2->rootOuputFileName           = outputFileNameBase;
//  ac2->nBins_pt    = 18;
//  ac2->min_pt      = 0.200;
//  ac2->max_pt      = 2.0;
//  ac2->nBins_eta   = 40;
//  ac2->min_eta     = -2;
//  ac2->max_eta     = 2;
//  ac2->nBins_y     = 40;
//  ac2->min_y       = -2;
//  ac2->max_y       = 2;
//  ac2->nBins_phi   = 36;
//  ac2->min_phi     = 0.0;
//  ac2->max_phi     = 2.0*3.1415927;
//
//  vector<unsigned int> comb;
//  comb.push_back(0);comb.push_back(3); ac2->combinations.push_back( comb ); comb.clear();
//  comb.push_back(0);comb.push_back(1); comb.push_back(3);comb.push_back(4); ac2->combinations.push_back( comb );comb.clear();
//  comb.push_back(0);comb.push_back(2); comb.push_back(3);comb.push_back(5); ac2->combinations.push_back( comb );comb.clear();
//  comb.push_back(1);comb.push_back(0); comb.push_back(4);comb.push_back(3); ac2->combinations.push_back( comb );comb.clear();
//  comb.push_back(1);comb.push_back(4); ac2->combinations.push_back( comb );  comb.clear();
//  comb.push_back(1);comb.push_back(2); comb.push_back(4);comb.push_back(5); ac2->combinations.push_back( comb );comb.clear();
//  comb.push_back(2);comb.push_back(0); comb.push_back(5);comb.push_back(3); ac2->combinations.push_back( comb );comb.clear();
//  comb.push_back(2);comb.push_back(1); comb.push_back(5);comb.push_back(4); ac2->combinations.push_back( comb );comb.clear();
//  comb.push_back(2);comb.push_back(5); ac2->combinations.push_back( comb );
//  particleFilters2 = ParticleFilter::createPlusMinusHadronFilters(0.2, 2.0, -2.0, 2.0, 10.0, -10.0);
//  ParticlePairAnalyzer * particlePairAnalyzer  = new ParticlePairAnalyzer("P2", ac2, eventFilters, particleFilters2, messageLevel);
//  masterTask->addSubtask( particlePairAnalyzer );


//  masterTask->run();
//
//  SubSampleStatCalculator * subSampleStatCalculator;
//  if (subsampleAnalysis && ga->subsampleAnalysis)
//    {
//    TString inputFileName   = outputFileNameBase+"Global";
//    TString outputFileName  = outputFileNameBase+"Global_Sum";
//    subSampleStatCalculator = new SubSampleStatCalculator(outputPathName,inputFileName,globalAnalyzer->getSubSampleIndex(),outputPathName,outputFileName,MessageLogger::Info);
//    subSampleStatCalculator->execute();
//    }
//
//  if (subsampleAnalysis && ac->subsampleAnalysis)
//    {
//    TString inputFileName   = outputFileNameBase+"P1";
//    TString outputFileName  = outputFileNameBase+"P1_Sum";
//    subSampleStatCalculator = new SubSampleStatCalculator(outputPathName,inputFileName,particleAnalyzer->getSubSampleIndex(),outputPathName,outputFileName,MessageLogger::Info);
//    subSampleStatCalculator->execute();
//    }
//
//  if (subsampleAnalysis && ac2->subsampleAnalysis)
//    {
//    TString inputFileName   = outputFileNameBase+"P2";
//    TString outputFileName  = outputFileNameBase+"P2_Sum";
//    subSampleStatCalculator = new SubSampleStatCalculator(outputPathName,inputFileName,particlePairAnalyzer->getSubSampleIndex(),outputPathName,outputFileName,MessageLogger::Info);
//    subSampleStatCalculator->execute();
//    }

  return 0;
}


//
