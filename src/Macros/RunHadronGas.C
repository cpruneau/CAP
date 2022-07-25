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
void loadHadronGas(const TString & includeBasePath);


int RunHadronGas()
{
  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  loadHadronGas(includeBasePath);

  cout << "==================================================================================" << endl;
  cout << "==================================================================================" << endl;
  cout << "                             HADRON GAS CALCULATION" << endl;
  cout << "==================================================================================" << endl;
  cout << "==================================================================================" << endl;

  MessageLogger::LogLevel infoLevel  = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Debug;
  MessageLogger::LogLevel selectedLevel = infoLevel;

  bool YES = true;
  bool NO  = false;
  bool loadPdgTable           = YES;
  bool doCalculation          = YES;
  bool doPlots                = NO;
  bool doBasicPlots           = NO;
  bool doVsTempPlots          = NO;
  bool forceHistogramsRewrite = YES;

  TString modelLabel    = "HG";
  TString inputPathName   = "/Volumes/ClaudeDisc4/OutputFiles/HG/";
  TString outputPathName  = "/Volumes/ClaudeDisc4/OutputFiles/HG/";
  gSystem->mkdir(outputPathName,1);

  if (loadPdgTable)
    {
    Configuration pdgDataConfig("ParticleTypeTableLoader Configuration");
    pdgDataConfig.addParameter("dataInputUsed",     true);
    pdgDataConfig.addParameter("dataInputPath",     TString(getenv("CAP_SRC")));
    pdgDataConfig.addParameter("dataInputFileName", TString("/EOS/pdgPhysicalKaons.dat"));
    Task * task= new ParticleTypeTableLoader("ParticleTypeTableLoader",pdgDataConfig,selectedLevel);
    task->execute();
    }

  //  ParticleTypeCollection * stableParticles = particles->extractCollection(1);
  //  stableParticles->printProperties(std::cout);

  Configuration hadronGasGeneratorConfig("HG Task Generator Configuration");
  hadronGasGeneratorConfig.addParameter("forceHistogramsRewrite",  forceHistogramsRewrite);
  hadronGasGeneratorConfig.addParameter("histoOutputPath",         outputPathName);
  hadronGasGeneratorConfig.addParameter("histoModelDataName",      modelLabel);
  hadronGasGeneratorConfig.addParameter("histoAnalyzerName",       "");
  hadronGasGeneratorConfig.addParameter("histoBaseName",           modelLabel);
  hadronGasGeneratorConfig.addParameter("nChemicalTemp",           4);
  hadronGasGeneratorConfig.addParameter("minChemicalTemp",         0.1500);
  hadronGasGeneratorConfig.addParameter("maxChemicalTemp",         0.1800);
  hadronGasGeneratorConfig.addParameter("nMass",                   30);
  hadronGasGeneratorConfig.addParameter("minMass",                 0.0);
  hadronGasGeneratorConfig.addParameter("maxMass",                 3.0);
  hadronGasGeneratorConfig.addParameter("doTempDependentHistos",   YES);

  if (doCalculation)
    {

    Task * hgTask = new HadronGasGeneratorTask(modelLabel,hadronGasGeneratorConfig,selectedLevel);
    hgTask->initialize();
    hgTask->execute();
    hgTask->finalize();
    }

  if (doPlots)
    {
    CanvasConfiguration * canvasConfigurationPort = new CanvasConfiguration(CanvasConfiguration::Portrait,CanvasConfiguration::Linear);
    CanvasConfiguration * canvasConfiguration     = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
    CanvasConfiguration * canvasConfigurationLogY = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::LogY);
    CanvasConfiguration * canvasConfigurationLogZ = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogZ);
    vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
    vector<GraphConfiguration*>  graphConfigurations2D = GraphConfiguration::createConfigurationPalette(10,2);

    int nT      = hadronGasGeneratorConfig.getValueInt("nChemicalTemp");
    double minT = hadronGasGeneratorConfig.getValueDouble("minChemicalTemp");
    double maxT = hadronGasGeneratorConfig.getValueDouble("maxChemicalTemp");

    Configuration plotterConfig("plotter");
    Plotter * plotter = new Plotter("HadronGasPlotter",plotterConfig, MessageLogger::Info);
    plotter->setDefaultOptions(1);

    if (doBasicPlots)
      {
      TProfile * h_rho1AllVsMass;
      TProfile * h_rho1ThVsMass;
      TProfile * h_rho1VsMass;
      TH1 * h_rho1All;
      TH1 * h_rho1Th;
      TH1 * h_rho1;
      TH1 * h_rho1ToRho1Th;

      TH2 * h_rho1thrho1th;
      TH2 * h_rho1rho1;
      TH2 * h_rho2Corr;
      TH2 * h_rho2Uncorr;
      TH2 * h_rho2;
      TH2 * h_C2;
      TH2 * h_R2;
      TH1 * h_BF;

      TH1 ** h_rho1ThVsP;

      }
    if (doVsTempPlots)
      {

      }

    //  plotter->printAllCanvas(outputPath);

    }

  return 0;
}

//
//  plotter->plot("HGM_numberDensityVsT", canvasConfigurationLogY, graphConfigurations[0],
//                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
//                "#rho_{1}(fm^{-3})",0.01, 10.0,
//                hgh->energyDensityVsT,
//                "#rho_{1}",0.22, 0.7, 0.4, 0.76, 0.055);
//  plotter->plot("HGM_energyDensityVsT", canvasConfigurationLogY, graphConfigurations[0],
//                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
//                "#varepsilon (GeV/fm^{3})",0.01, 10.0,
//                hgh->energyDensityVsT,
//                "#varepsilon (GeV/fm^{3})",0.22, 0.7, 0.4, 0.76, 0.055);
//  plotter->plot("HGM_entropyDensityVsT", canvasConfigurationLogY, graphConfigurations[0],
//                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
//                "s",0.1, 100.0,
//                hgh->entropyDensityVsT,
//                "s",0.22, 0.7, 0.4, 0.76, 0.055);
//
//  plotter->plot("HGM_PressureVsT", canvasConfigurationLogY, graphConfigurations[0],
//                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
//                "Pressure",0.001, 1.0,
//                hgh->pressureVsT,
//                "P",0.22, 0.7, 0.4, 0.76, 0.055);

//  double minYield = 1.0E-7;
//  double maxYield = 1.0;
//
//  int nGraphs = 8;
//  TString ** legends = new TString*[nGraphs];
//  TH1 ** histograms = new TH1*[nGraphs];
//  histograms[0] =  hgh->energyDensityVsT;    legends[0] = new TString("ALL");
//  histograms[1] =  hgh->nDensityVsT[1];     legends[1]    = new TString(hadronGas->stableParticleTypes->getParticleType(1)->getTitle());
//  histograms[2] =  hgh->nDensityVsT[3];     legends[2]    = new TString(hadronGas->stableParticleTypes->getParticleType(3)->getTitle());//partLabel[3];
//  histograms[3] =  hgh->nDensityVsT[7];     legends[3]    = new TString(hadronGas->stableParticleTypes->getParticleType(7)->getTitle());//partLabel[7];
//  histograms[4] =  hgh->nDensityVsT[11];    legends[4]    = new TString(hadronGas->stableParticleTypes->getParticleType(11)->getTitle());//partLabel[11];
//  histograms[5] =  hgh->nDensityVsT[13];    legends[5]    = new TString(hadronGas->stableParticleTypes->getParticleType(13)->getTitle());//partLabel[13];
//  histograms[6] =  hgh->nDensityVsT[17];    legends[6]    = new TString(hadronGas->stableParticleTypes->getParticleType(17)->getTitle());//partLabel[17];
//  histograms[7] =  hgh->nDensityVsT[21];    legends[7]    = new TString(hadronGas->stableParticleTypes->getParticleType(21)->getTitle());//partLabel[21];
//
//
//  plotter->plot(nGraphs, "HGM_numberDensityAllVsT", canvasConfigurationLogY, graphConfigurationsNoL,
//                "T (GeV)",  hadronGasConfig.minT, hadronGasConfig.maxT,
//                "#rho_{1}(fm^{-3})",1.0E-8, 10.0,
//                histograms,legends,0.6, 0.18, 0.75, 0.45, 0.04);
//
//  nGraphs = 5;
//  histograms[0] = hadronGasHistos[0]->h_rho1Th; legends[0] = tempLabels[0];
//  histograms[1] = hadronGasHistos[2]->h_rho1Th; legends[1] = tempLabels[2];
//  histograms[2] = hadronGasHistos[4]->h_rho1Th; legends[2] = tempLabels[4];
//  histograms[3] = hadronGasHistos[6]->h_rho1Th; legends[3] = tempLabels[6];
//  histograms[4] = hadronGasHistos[8]->h_rho1Th; legends[4] = tempLabels[8];
//
//  plotter->plot(nGraphs, "HGM_Stablerho1ThVsT",canvasConfigurationLogY,graphConfigurations,
//                        "Species", 0.0, 18.0,
//                        "#rho_{1}^{Th} (fm^{-3})", minYield,maxYield,
//                        histograms,legends,0.2, 0.20, 0.45, 0.45, 0.04);
//
//  histograms[0] = hadronGasHistos[0]->h_rho1; legends[0] = tempLabels[0];
//  histograms[1] = hadronGasHistos[2]->h_rho1; legends[1] = tempLabels[2];
//  histograms[2] = hadronGasHistos[4]->h_rho1; legends[2] = tempLabels[4];
//  histograms[3] = hadronGasHistos[6]->h_rho1; legends[3] = tempLabels[6];
//  histograms[4] = hadronGasHistos[8]->h_rho1; legends[4] = tempLabels[8];
//
//  std::cout << "============================================================================= 6 =====" << std::endl;
//
//  plotter->plot(nGraphs, "HGM_StableDecayYieldsVsT",canvasConfigurationLogY,graphConfigurations,
//                        "Species", 0.0, 23.0,
//                        "#rho_{1} (fm^{-3})", minYield,maxYield,
//                        histograms,legends,0.2, 0.20, 0.45, 0.45, 0.04);
//
//
//  histograms[0] = hadronGasHistos[0]->h_rho1ToRho1Th; legends[0] = tempLabels[0];
//  histograms[1] = hadronGasHistos[2]->h_rho1ToRho1Th; legends[1] = tempLabels[2];
//  histograms[2] = hadronGasHistos[4]->h_rho1ToRho1Th; legends[2] = tempLabels[4];
//  histograms[3] = hadronGasHistos[6]->h_rho1ToRho1Th; legends[3] = tempLabels[6];
//  histograms[4] = hadronGasHistos[8]->h_rho1ToRho1Th; legends[4] = tempLabels[8];
//  plotter->plot(nGraphs, "HGM_StableDecayToThermalVsT",canvasConfigurationLogY,graphConfigurations,
//                        "Species", 0.0, 23.0,
//                        "#rho_{1}/#rho_{1}^{Th}", 0.5,10.0,
//                        histograms,legends,0.7, 0.60, 0.85, 0.85, 0.04);
//
//  histograms[0] = (TH1*) hadronGasHistos[4]->h_rho1Th->Clone(); legends[0] = new TString(*tempLabels[4]+" MeV; Thermal Only");
//  histograms[1] = (TH1*) hadronGasHistos[4]->h_rho1->Clone();   legends[1] = new TString(*tempLabels[4]+" MeV; Thermal + Decays");
//
//  plotter->plot(2, "HGM_StableDecayVsrho1Th",canvasConfigurationLogY,graphConfigurations,
//                        "Species", 0.0, 23.0,
//                        "#rho_{1}", minYield,maxYield,
//                        histograms,legends,0.2, 0.20, 0.45, 0.45, 0.04);
//  canvasConfigurationLogZ->theta =  35.0;
//  canvasConfigurationLogZ->phi   = -125.0;
//  graphConfiguration2D->plotOption = "LEGO2";
//
//  int jt = 9;
//
//  hadronGasHistos[jt]->h_rho2Corr->GetXaxis()->SetNdivisions(23);
//  hadronGasHistos[jt]->h_rho2Corr->GetYaxis()->SetNdivisions(23);
//  plotter->plot("HGM_StableDecayCorrelatedPairYields",canvasConfigurationLogZ,graphConfiguration2D,
//               "Species 1", 0.0, 23.0,
//               "Species 2", 0.0, 23.0,
//               "#rho_{2}^{Corr}", 1E-7, 1.0,
//               hadronGasHistos[jt]->h_rho2Corr);
//
//
//  hadronGasHistos[jt]->h_rho1rho1->GetXaxis()->SetNdivisions(23);
//  hadronGasHistos[jt]->h_rho1rho1->GetYaxis()->SetNdivisions(23);
//  plotter->plot("HGM_StableDecayPairYields",canvasConfigurationLogZ,graphConfiguration2D,
//               "Species 1", 0.0, 23.0,
//               "Species 2", 0.0, 23.0,
//               "#rho_{1}#otimes#rho_{1}", 1E-10, 10.0,
//               hadronGasHistos[jt]->h_rho1rho1);
//
//  histograms[0] = hadronGasHistos[0]->h_rho1AllVsMass; legends[0] = tempLabels[0];
//  histograms[1] = hadronGasHistos[2]->h_rho1AllVsMass; legends[1] = tempLabels[2];
//  histograms[2] = hadronGasHistos[4]->h_rho1AllVsMass; legends[2] = tempLabels[4];
//  histograms[3] = hadronGasHistos[6]->h_rho1AllVsMass; legends[3] = tempLabels[6];
//  histograms[4] = hadronGasHistos[8]->h_rho1AllVsMass; legends[4] = tempLabels[8];
//  plotter->plot(nGraphs,"HGM_AllYieldsVsMassVsT",canvasConfigurationLogY,graphConfigurations,
//                        "Mass (GeV/c^{2})", 0.0, 2.50,
//                        "#rho_{1}^{Th}/g", 1.0E-7,1.0E0,
//                        histograms,legends,0.7, 0.60, 0.85, 0.85, 0.04);
//
//
//  histograms[0] = hadronGasHistos[0]->h_rho1ThVsMass; legends[0] = tempLabels[0];
//  histograms[1] = hadronGasHistos[2]->h_rho1ThVsMass; legends[1] = tempLabels[2];
//  histograms[2] = hadronGasHistos[4]->h_rho1ThVsMass; legends[2] = tempLabels[4];
//  histograms[3] = hadronGasHistos[6]->h_rho1ThVsMass; legends[3] = tempLabels[6];
//  histograms[4] = hadronGasHistos[8]->h_rho1ThVsMass; legends[4] = tempLabels[8];
//  plotter->plot(nGraphs,"HGM_Stablerho1ThVsMassVsT",canvasConfigurationLogY,graphConfigurations,
//                        "Mass (GeV/c^{2})", 0.0, 2.50,
//                        "#rho_{1}/g", 1.0E-7,1.0E0,
//                        histograms,legends,0.7, 0.60, 0.85, 0.85, 0.04);
//
//
//  graphConfigurations[0]->xLabelSize = 0.04;
//  graphConfigurations[0]->plotOption = "COLZ";
//  plotter->plot("HGM_BF_10", canvasConfiguration, graphConfigurations[0],
//                "",  0.0, 18.0,
//                "BF",-0.1, 0.5,
//                hadronGasHistos[jt]->h_BF,
//                "BF Integrals",0.65, 0.8, 0.8, 0.86, 0.035);
//
//  plotter->plot("HGM_Rho2_10", canvasConfigurationLogZ, graphConfigurations[0],
//                "Trigger Species", 0.0, 23.0,
//                "Associate Species", 0.0, 23.0,
//                "#rho_{2}", 1E-10, 1.0,
//                hadronGasHistos[jt]->h_rho2);
//
//  plotter->plot("HGM_rho1rho1_10", canvasConfigurationLogZ, graphConfigurations[0],
//                "Trigger Species", 0.0, 23.0,
//                "Associate Species", 0.0, 23.0,
//                "#rho_{1}#otimes#rho_{1}", 1E-10, 1.0,
//                hadronGasHistos[jt]->h_rho1rho1);
//
//  plotter->plot("HGM_rho1thrho1th_10", canvasConfigurationLogZ, graphConfigurations[0],
//                "Trigger Species", 0.0, 23.0,
//                "Associate Species", 0.0, 23.0,
//                "#rho_{1}^{th}#rho_{1}^{th}", 1E-10, 1.0,
//                hadronGasHistos[jt]->h_rho1rho1);
//  plotter->plot("HGM_C2_10", canvasConfigurationLogZ, graphConfigurations[0],
//                "Trigger Species", 0.0, 23.0,
//                "Associate Species", 0.0, 23.0,
//                "C_{2}^{ij}", -1.0, 1.0,
//                hadronGasHistos[jt]->h_C2);
//  plotter->plot("HGM_R2_10", canvasConfigurationLogZ, graphConfigurations[0],
//                "Trigger Species", 0.0, 23.0,
//                "Associate Species", 0.0, 23.0,
//                "R_{2}^{ij}", -1.0, 1.0,
//                hadronGasHistos[jt]->h_R2);
//  plotter->plot("HGM_UncorrelatedPairs", canvasConfigurationLogZ, graphConfigurations[0],
//                "Trigger Species", 0.0, 23.0,
//                "Associate Species", 0.0, 23.0,
//                "Uncorrelated", -1.0, 1.0,
//                hadronGasHistos[jt]->h_rho2Uncorr);
//


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


void loadHadronGas(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/HadronGas/";
  gSystem->Load(includePath+"HadronGas.hpp");
  gSystem->Load(includePath+"HadronGasGeneratorTask.hpp");
  gSystem->Load(includePath+"HadronGasHistograms.hpp");
  gSystem->Load(includePath+"HadronGasParticle.hpp");
  gSystem->Load(includePath+"HadronGasVsTempHistograms.hpp");
  gSystem->Load("libHadronGas.dylib");
}

