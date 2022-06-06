
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

int PlotPerformance()
{
  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  MessageLogger::LogLevel infoLevel = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Info;
  MessageLogger::LogLevel selectLevel = infoLevel;

  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 0;
  bool useColor = true;
  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  vector<GraphConfiguration*>  graphConfigurations2D = GraphConfiguration::createConfigurationPalette(10,2);

//  TString inputPath = "/Volumes/ClaudeDisc4/OutputFiles/longTest/";
//  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/longTest/";
//  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecayPt0.2-2.0/";
//  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecayPt0.2-2.0/";
  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/";

  vector<TString> histoInputFileNames;
  vector<TFile*>  histoInputFiles;
  vector<TString> names;
  vector<TString> titles;
  vector<TString> yTitles;
  vector<TString> histoNames;
  vector<double>  minY;
  vector<double>  maxY;

  histoInputFileNames.push_back(TString("PythiaFlatEff/PYTHIA/PP/13000//PYTHIA_Pair_Gen_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaFlatEff/PYTHIA/PP/13000//PYTHIA_Pair_Reco_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaFlatEff/PYTHIA/PP/13000//PYTHIA_Pair_Closure_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaPtDepEff/PYTHIA/PP/13000//PYTHIA_Pair_Reco_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaPtDepEff/PYTHIA/PP/13000/PYTHIA_Pair_Closure_Derived.root"));

  names.push_back(TString("GenLevel"));
  names.push_back(TString("RecoFlatEff"));
  names.push_back(TString("ClosureFlatEff"));
  names.push_back(TString("RecoPtDependentEff"));
  names.push_back(TString("ClosurePtDependentEff"));

  titles.push_back(TString("Gen Level"));
  titles.push_back(TString("Flat Efficiency"));
  titles.push_back(TString("Closure Flat Efficiency"));
  titles.push_back(TString("Pt Dependent Efficiency"));
  titles.push_back(TString("Closure Pt Dependent  Efficiency"));

  yTitles.push_back(TString("R_{2}"));
  yTitles.push_back(TString("R_{2}"));
  yTitles.push_back(TString("R_{2}^{Reco}/R_{2}^{Gen}"));
  yTitles.push_back(TString("R_{2}"));
  yTitles.push_back(TString("R_{2}^{Reco}/R_{2}^{Gen}"));


  histoNames.push_back(TString("Pair_Gen_All_HP_HM_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Reco_All_HP_HM_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Ratio_All_HM_HP_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Reco_All_HP_HM_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Ratio_All_HM_HP_R2_DetaDphi_shft"));

  minY.push_back(1.0);     maxY.push_back(2.0);
  minY.push_back(1.0);     maxY.push_back(2.0);
  minY.push_back(0.8);     maxY.push_back(1.4);
  minY.push_back(1.0);     maxY.push_back(2.0);
  minY.push_back(0.8);     maxY.push_back(1.4);

  Plotter * plotter = new Plotter("Plotter",Configuration(),MessageLogger::Info);
  plotter->setDefaultOptions(useColor);

  vector<TGraph*>  graphs;
  vector<TString>  legendTexts;

  for (unsigned int iFile=0; iFile<histoInputFileNames.size(); iFile++)
    //for (unsigned int iFile=0; iFile<1; iFile++)
    {
    TFile * f =  plotter->openRootFile(inputPath,histoInputFileNames[iFile],"OLD");
    if (!f)
      {
      cout << " Could not open file named " << histoInputFileNames[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
    histoInputFiles.push_back(f);
    TH2 * h2 = plotter->getHistogramCollection().loadH2(f,histoNames[iFile]);
    if (!h2)
      {
      cout << " Could not load histogram named " << histoNames[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
    plotter->plot(h2,names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                  TString("#Delta#eta"),-1.5,1.5,
                  TString("#Delta#varphi"),1.0,-1.0,
                  yTitles[iFile],minY[iFile],maxY[iFile]);
    plotter->createLabel(titles[iFile], -1.1, 0.8, 0.0, 1, 0.07, true);

    }


//  plotter->plot(graphs,graphConfigurations1D,massTitles,"IntegralVsEtaRange",landscapeLinear,TString("#Delta#eta"), 0.0, 4.0,TString("Integral"),0.0, 1.2,
//                0.6, 0.6, 0.7, 0.9, 0.04);

  plotter->printAllCanvas(outputPath, printGif, printPdf, printSvg, printC);
  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load(includePath+"TaskIterator.hpp");
  gSystem->Load(includePath+"Collection.hpp");
  gSystem->Load(includePath+"CanvasCollection.hpp");
  gSystem->Load(includePath+"GraphConfiguration.hpp");
  gSystem->Load(includePath+"CanvasConfiguration.hpp");
  gSystem->Load(includePath+"HistogramCollection.hpp");
  gSystem->Load(includePath+"Histograms.hpp");
  gSystem->Load(includePath+"Particle.hpp");
  gSystem->Load(includePath+"ParticleType.hpp");
  gSystem->Load(includePath+"ParticleTypeCollection.hpp");
  gSystem->Load(includePath+"ParticleDecayMode.hpp");
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
}
