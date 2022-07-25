
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

int PlotPythiaClosure()
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
  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PythiaClosureTest/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/PythiaClosureTest/";

  vector<TString> histoInputFileNames;
  vector<TString> histoNames;
  vector<TFile*>  histoInputFiles;
  vector<TH2*>    histos;
  vector<TString> names;
  vector<TString> titles;
  vector<TString> yTitles;
  vector<double>  yMin;
  vector<double>  yMax;

  histoInputFileNames.push_back(TString("PythiaFlatEff/PYTHIA/PP/13000/PYTHIA_Pair_Gen_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaFlatEff/PYTHIA/PP/13000/PYTHIA_Pair_Reco_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaFlatEff/PYTHIA/PP/13000/PYTHIA_Pair_Closure_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaPtDepEff/PYTHIA/PP/13000/PYTHIA_Pair_Reco_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaPtDepEff/PYTHIA/PP/13000/PYTHIA_Pair_Closure_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaPtDepEff2/PYTHIA/PP/13000/PYTHIA_Pair_Closure_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaFlatEffRes/PYTHIA/PP/13000/PYTHIA_Pair_Reco_Derived.root"));
  histoInputFileNames.push_back(TString("PythiaFlatEffRes/PYTHIA/PP/13000/PYTHIA_Pair_Closure_Derived.root"));

  histoNames.push_back(TString("Pair_Gen_All_HM_HP_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Reco_All_HM_HP_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Ratio_All_HM_HP_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Reco_All_HM_HP_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Ratio_All_HM_HP_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Ratio_All_HM_HP_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Reco_All_HM_HP_R2_DetaDphi_shft"));
  histoNames.push_back(TString("Pair_Ratio_All_HM_HP_R2_DetaDphi_shft"));

  names.push_back(TString("PythiaFlatEff_Gen"));
  names.push_back(TString("PythiaFlatEff_Reco"));
  names.push_back(TString("PythiaFlatEff_Closure"));
  names.push_back(TString("PythiaPtDepEff_Reco"));
  names.push_back(TString("PythiaPtDepEff_Closure"));
  names.push_back(TString("PythiaPtDepEff2_Closure"));
  names.push_back(TString("PythiaFlatEffRes_Reco"));
  names.push_back(TString("PythiaFlatEffRes_Closure"));


  titles.push_back(TString("PYTHIA pp #sqrt{s}=13 TeV"));
  titles.push_back(TString("#varepsilon = 0.8"));
  titles.push_back(TString("#varepsilon = 0.8"));
  titles.push_back(TString("#varepsilon #equiv F_1(p_{T})"));
  titles.push_back(TString("#varepsilon #equiv F_1(p_{T})"));
  titles.push_back(TString("#varepsilon #equiv F_2(p_{T})"));
  titles.push_back(TString("#varepsilon = 0.8;  #delta p_{T},#delta#eta,#delta#varphi#equiv F_3(p_{T},#eta,#varphi)"));
  titles.push_back(TString("#varepsilon = 0.8;  #delta p_{T},#delta#eta,#delta#varphi#equiv F_3(p_{T},#eta,#varphi)"));

  yTitles.push_back(TString("R_{2}^{Gen}"));
  yTitles.push_back(TString("R_{2}^{Reco}"));
  yTitles.push_back(TString("R_{2}^{Reco}/R_{2}^{Gen}"));
  yTitles.push_back(TString("R_{2}^{Reco}"));
  yTitles.push_back(TString("R_{2}^{Reco}/R_{2}^{Gen}"));
  yTitles.push_back(TString("R_{2}^{Reco}/R_{2}^{Gen}"));
  yTitles.push_back(TString("R_{2}^{Reco}"));
  yTitles.push_back(TString("R_{2}^{Reco}/R_{2}^{Gen}"));

  yMin.push_back(0.8);   yMax.push_back(1.5);
  yMin.push_back(0.8);   yMax.push_back(1.5);
  yMin.push_back(0.90);  yMax.push_back(1.1);
  yMin.push_back(0.8);   yMax.push_back(1.8);
  yMin.push_back(0.90);  yMax.push_back(2.0);
  yMin.push_back(0.90);  yMax.push_back(2.0);
  yMin.push_back(0.8);   yMax.push_back(1.5);
  yMin.push_back(0.90);  yMax.push_back(1.1);

  Plotter * plotter = new Plotter("Plotter",Configuration(),MessageLogger::Info);
  plotter->setDefaultOptions(useColor);

  vector<TGraph*>  graphs;
  vector<TString>  legendTexts;
  landscapeLinear.setParameter("windowLeftMargin",0.15);
  landscapeLinear.setParameter("windowTopMargin",0.10);
  landscapeLinear.setParameter("windowRightMargin",0.05);

  for (unsigned int iFile=0; iFile<histoInputFileNames.size(); iFile++)
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
    histos.push_back(h2);
    graphConfigurations2D[iFile]->setParameter("yLabelOffset", 0.01);
    graphConfigurations2D[iFile]->setParameter("zTitleSize", 0.06);
    graphConfigurations2D[iFile]->setParameter("zTitleOffset", 1.0);

    plotter->plot(h2,names[iFile],landscapeLinear,*(graphConfigurations2D[0]),
                  TString("#Delta#eta"),-3.0,3.0,
                  TString("#Delta#varphi"),1.0,-1.0,
                  yTitles[iFile],yMin[iFile],yMax[iFile]);
    plotter->createLabel(titles[iFile], -1.0, 0.9, 0.0, 1, 0.07, true);

    }

  int k = 2;
  TH1 * hClosure1 = histos[k]->ProjectionX("Closure1");
  hClosure1->Scale(1.0/72.0);
  plotter->plot(hClosure1,names[k]+"ProjX1",landscapeLinear,*(graphConfigurations1D[0]),
                TString("#Delta#eta"),-3.0,3.0,
                yTitles[k],0.98,1.02,
                "Flat efficiency", -2.0, 0.985, 0.0, 0.99, 1.2 );
  hClosure1->Fit("pol0");


  k = 4;
  TH1 * hClosure2 = histos[k]->ProjectionX("Closure2");
  hClosure2->Scale(1.0/72.0);
  plotter->plot(hClosure2,names[k]+"ProjX2",landscapeLinear,*(graphConfigurations1D[0]),
                TString("#Delta#eta"),-3.0,3.0,
                yTitles[k],0.9,1.2,
                "pT dependent efficiency", -2.0, 0.985, 0.0, 0.99, 0.2 );
  hClosure2->Fit("pol0");

  k = 7;
  TH1 * hClosure3 = histos[k]->ProjectionX("Closure3");
  hClosure3->Scale(1.0/72.0);
  plotter->plot(hClosure3,names[k]+"ProjX3",landscapeLinear,*(graphConfigurations1D[0]),
                TString("#Delta#eta"),-3.0,3.0,
                yTitles[k],0.98,1.02,
                "w/ Finite Resolution", -2.0, 0.985, 0.0, 0.99, 0.2 );
  hClosure3->Fit("pol0");

  TString fileName = "PythiaFlatEffRes/PYTHIA/PP/13000/PYTHIA_SimAna.root";
  TFile * f =  plotter->openRootFile(inputPath,fileName,"OLD");
  if (!f)
    {
    cout << " Could not open file named " << fileName << endl;
    cout << " ABORT!!!!" << endl;
    return -1;
    }

  TString histoName = "SimAna_All_All_n1_dPt";
  TH1*  SimAna_All_All_n1_dPt = plotter->getHistogramCollection().loadH1(f,histoName);
  plotter->plot(SimAna_All_All_n1_dPt,histoName,landscapeLogY,*(graphConfigurations1D[0]),
                TString("#Delta p_{T}"),-0.4,0.4,
                "Counts",1E-4,30.0,
                "pT Resolution", -0.35, 1.0, 0.0, 2.0, 0.2 );

  histoName = "SimAna_All_All_n1_dEta";
  TH1*  SimAna_All_All_n1_dEta = plotter->getHistogramCollection().loadH1(f,histoName);
  plotter->plot(SimAna_All_All_n1_dEta,histoName,landscapeLogY,*(graphConfigurations1D[0]),
                TString("#Delta #eta"),-0.4,0.4,
                "Counts",1E-4,30.0,
                "#eta Resolution", -0.35, 1.0, 0.0, 2.0, 0.2 );

  histoName = "SimAna_All_All_n1_dPhi";
  TH1*  SimAna_All_All_n1_dPhi = plotter->getHistogramCollection().loadH1(f,histoName);
  plotter->plot(SimAna_All_All_n1_dPhi,histoName,landscapeLogY,*(graphConfigurations1D[0]),
                TString("#Delta #phi"),-0.4,0.4,
                "Counts",1E-4,30.0,
                "#eta Resolution", -0.35, 1.0, 0.0, 2.0, 0.2 );

  histoName = "SimAna_All_All_n1_dPtVsPt";
  landscapeLinear.setParameter("windowLogZ", true);
  TH2*  SimAna_All_All_n1_dPtVsPt = plotter->getHistogramCollection().loadH2(f,histoName);
  plotter->plot(SimAna_All_All_n1_dPtVsPt,histoName,landscapeLinear,*(graphConfigurations2D[0]),
                TString("p_{T}"),0.0,5.0,
                TString("#Delta p_{T}"),-0.4,0.4,
                "Counts",1E-4,1.0);



//    plotter->plot(graphs,graphConfigurations1D,titles,"IntegralVsEtaRange",landscapeLinear,TString("#Delta#eta"), 0.0, 4.0,TString("Integral"),0.0, 1.2,
//                  0.6, 0.6, 0.7, 0.9, 0.04);

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
