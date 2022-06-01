
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

int PlotRhoDecayBF()
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
  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecayPt0.2-1.0/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecayPt0.2-1.0/";

  vector<TString> histoInputFileNames;
  vector<TFile*>  histoInputFiles;
  vector<TString> massNames;
  vector<TString> massTitles;

  //histoInputFileNames.push_back(TString("RhoDecay-Mass0.3GeV/RhoDecay_Pair__DerivedBalFct_Sum0TO9.root"));
  //histoInputFileNames.push_back(TString("RhoDecay-Mass0.4GeV/RhoDecay_Pair__Sum0TO9.root"));
  histoInputFileNames.push_back(TString("RhoDecay-Mass0.4GeV/RhoDecay_Pair__DerivedBalFct_Sum0TO9.root"));
  histoInputFileNames.push_back(TString("RhoDecay-Mass0.6GeV/RhoDecay_Pair__DerivedBalFct_Sum0TO9.root"));
  histoInputFileNames.push_back(TString("RhoDecay-Mass0.8GeV/RhoDecay_Pair__DerivedBalFct_Sum0TO9.root"));
  histoInputFileNames.push_back(TString("RhoDecay-Mass1.2GeV/RhoDecay_Pair__DerivedBalFct_Sum0TO9.root"));
  massNames.push_back(TString("Mass0.3GeVc2"));
  massNames.push_back(TString("Mass0.4GeVc2"));
  massNames.push_back(TString("Mass0.6GeVc2"));
  massNames.push_back(TString("Mass0.8GeVc2"));
  massNames.push_back(TString("Mass1.2GeVc2"));
  massTitles.push_back(TString("m = 0.3 GeV/c^{2}"));
  massTitles.push_back(TString("m = 0.4 GeV/c^{2}"));
  massTitles.push_back(TString("m = 0.6 GeV/c^{2}"));
  massTitles.push_back(TString("m = 0.8 GeV/c^{2}"));
  massTitles.push_back(TString("m = 1.2 GeV/c^{2}"));
  TString histoName = "Pair_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar";

  Plotter * plotter = new Plotter("Plotter",Configuration(),MessageLogger::Info);
  plotter->setDefaultOptions(useColor);

  vector<TGraph*>  graphs;
  vector<TString>  legendTexts;

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
    TH2 * h2 = plotter->getHistogramCollection().loadH2(f,histoName);
    if (!h2)
      {
      cout << " Could not load histogram named " << histoName << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
      plotter->plot(h2,massNames[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                  TString("#Delta#eta"),1.0,-1.0,
                  TString("#Delta#varphi"),1.0,-1.0,
                  TString("B_{2}"),1.0,-1.0);
    plotter->createLabel(massTitles[iFile], -1.1, 0.8, 0.0, 1, 0.07, true);

    double * x  = new double[30];
    double * ex = new double[30];
    double * y  = new double[30];
    double * ey = new double[30];
    double sum, esum;
    const TAxis * xAxis  = h2->GetXaxis();
    const TAxis * yAxis  = h2->GetYaxis();
    int yLow  = yAxis->FindBin(-0.5*TMath::Pi());
    int yHigh = yAxis->FindBin( 1.5*TMath::Pi());

    int nPoints = 0;
    double etaLow  =  0.1;
    double etaHigh =  3.05;
    double etaStep =  0.1;
    cout << "Mass: " << massNames[iFile] << endl;
    for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
      {
      int xLow  = xAxis->FindBin(-eta);
      int xHigh = xAxis->FindBin( eta);
      sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum);
      cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
      x[nPoints] = eta;
      ex[nPoints] = 0.001;
      y[nPoints] = sum;
      ey[nPoints] = esum;
      nPoints++;
      }
    TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
    graphs.push_back(g);
//    legendTexts.push_back(massNames[iFile]);

    }


  plotter->plot(graphs,graphConfigurations1D,massTitles,"IntegralVsEtaRange",landscapeLinear,TString("#Delta#eta"), 0.0, 4.0,TString("Integral"),0.0, 1.2,
                0.6, 0.6, 0.7, 0.9, 0.04);

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
