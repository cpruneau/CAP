
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

  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecay/PtSlope1000/Mass500/WideGenY6WidePt/";
  TString outputPath = inputPath;

  vector<TString> histoInputFileNames;
  vector<TString> histoNames1D;
  vector<TString> histoNames2D;
  vector<TFile*>  histoInputFiles;
  vector<TString> names;
  vector<TString> titles;
  vector<TH1*>    histos1D;
  vector<TH2*>    histos2D;

  histoInputFileNames.push_back(TString("RhoDecay_Pair_Gen_Derived_BalFct.root"));
  histoInputFileNames.push_back(TString("RhoDecay_Pair_Gen_Derived_BalFct.root"));
  names.push_back(TString("Slope1.0GeV_Mass0.5GeVc2_B2_1Bar_2"));
  names.push_back(TString("Slope1.0GeV_Mass0.5GeVc2B2_1_2Bar"));
  titles.push_back(TString("m = 0.5 GeV/c^{2} B_{2}^{#bar{1}2}"));
  titles.push_back(TString("m = 0.5 GeV/c^{2} B_{2}^{1#bar{2}}"));

  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
  histoNames1D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
  histoNames1D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));

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
    TH2 * h2 = plotter->getHistogramCollection().loadH2(f,histoNames2D[iFile]);
    if (!h2)
      {
      cout << " Could not load histogram named " << histoNames2D[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
    histos2D.push_back(h2);
    plotter->plot(h2,names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                  TString("#Delta#eta"),1.0,-1.0,
                  TString("#Delta#varphi"),1.0,-1.0,
                  TString("B_{2}"),1.0,-1.0);
    plotter->createLabel(titles[iFile], -1.1, 0.8, 0.0, 1, 0.05, true);

    TH2 * h1 = plotter->getHistogramCollection().loadH2(f,histoNames1D[iFile]);
    if (!h1)
      {
      cout << " Could not load histogram named " << histoNames1D[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
    histos1D.push_back(h1);

    double * x  = new double[300];
    double * ex = new double[300];
    double * y  = new double[300];
    double * ey = new double[300];
    double sum, esum;
    const TAxis * xAxis  = h2->GetXaxis();
    const TAxis * yAxis  = h2->GetYaxis();
    int yLow  = yAxis->FindBin(-0.5*TMath::Pi());
    int yHigh = yAxis->FindBin( 1.5*TMath::Pi());

    int nPoints = 0;
    double etaLow  =  0.1;
    double etaHigh =  9.9;
    double etaStep =  0.2;
    cout << "Name: " << names[iFile] << endl;
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
    //    legendTexts.push_back(names[iFile]);

    }


  plotter->plot(graphs,graphConfigurations1D,titles,"IntegralVsEtaRangeVsMass",landscapeLinear,TString("#Delta y"), 0.0, 4.0,TString("Integral"),0.0, 1.2,
                0.4, 0.25, 0.6, 0.45, 0.06);

  plotter->plot(histos1D,graphConfigurations1D,titles,"B2vsY-Projections",landscapeLinear,TString("#Delta y"), 0.0, 4.0,TString("B_{2}"),-0.02, 0.25,
                0.5, 0.55, 0.65, 0.75, 0.06);



  plotter->printAllCanvas(outputPath, printGif, printPdf, printSvg, printC);
  return 0;
}



int PlotRhoDecayBF2()
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
//  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecayPt0.2-1.0/";
//  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecayPt0.2-1.0/";
  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecay/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/RhoDecay/";

  vector<TString> histoInputFileNames;
  vector<TString> histoNames2D;
  vector<TFile*>  histoInputFiles;
  vector<TString> names;
  vector<TString> titles;
  vector<TH2*>    histos2D;

  histoInputFileNames.push_back(TString("PtSlope1000/Mass300/RhoDecay_Pair_Gen_Derived_BalFct.root"));
  histoInputFileNames.push_back(TString("PtSlope1000/Mass500/RhoDecay_Pair_Gen_Derived_BalFct.root"));
  histoInputFileNames.push_back(TString("PtSlope1000/Mass600/RhoDecay_Pair_Gen_Derived_BalFct.root"));
  histoInputFileNames.push_back(TString("PtSlope1000/Mass700/RhoDecay_Pair_Gen_Derived_BalFct.root"));
  histoInputFileNames.push_back(TString("PtSlope1000/Mass800/RhoDecay_Pair_Gen_Derived_BalFct.root"));
  histoInputFileNames.push_back(TString("PtSlope1000/Mass1000/RhoDecay_Pair_Gen_Derived_BalFct.root"));
  histoInputFileNames.push_back(TString("PtSlope1000/Mass1400/RhoDecay_Pair_Gen_Derived_BalFct.root"));
  histoInputFileNames.push_back(TString("PtSlope1000/Mass1800/RhoDecay_Pair_Gen_Derived_BalFct.root"));
  names.push_back(TString("Slope1.0GeV_Mass0.3GeVc2"));
  names.push_back(TString("Slope1.0GeV_Mass0.5GeVc2"));
  names.push_back(TString("Slope1.0GeV_Mass0.6GeVc2"));
  names.push_back(TString("Slope1.0GeV_Mass0.7GeVc2"));
  names.push_back(TString("Slope1.0GeV_Mass0.8GeVc2"));
  names.push_back(TString("Slope1.0GeV_Mass1.0GeVc2"));
  names.push_back(TString("Slope1.0GeV_Mass1.4GeVc2"));
  names.push_back(TString("Slope1.0GeV_Mass1.8GeVc2"));
  titles.push_back(TString("m = 0.3 GeV/c^{2}"));
  titles.push_back(TString("m = 0.5 GeV/c^{2}"));
  titles.push_back(TString("m = 0.6 GeV/c^{2}"));
  titles.push_back(TString("m = 0.7 GeV/c^{2}"));
  titles.push_back(TString("m = 0.8 GeV/c^{2}"));
  titles.push_back(TString("m = 1.0 GeV/c^{2}"));
  titles.push_back(TString("m = 1.4 GeV/c^{2}"));
  titles.push_back(TString("m = 1.8 GeV/c^{2}"));

//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 0.3 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 0.5 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 0.6 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 0.7 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 0.8 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 1.0 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 1.4 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 1.8 GeV/c^{2}"));


  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));

  // vs <pT>
//  histoInputFileNames.push_back(TString("PtSlope1200/Mass800/RhoDecay_Pair_Gen_Derived_BalFct.root"));
//  histoInputFileNames.push_back(TString("PtSlope1000/Mass800/RhoDecay_Pair_Gen_Derived_BalFct.root"));
//  histoInputFileNames.push_back(TString("PtSlope700/Mass800/RhoDecay_Pair_Gen_Derived_BalFct.root"));
//  histoInputFileNames.push_back(TString("PtSlope500/Mass800/RhoDecay_Pair_Gen_Derived_BalFct.root"));
//  histoInputFileNames.push_back(TString("PtSlope400/Mass800/RhoDecay_Pair_Gen_Derived_BalFct.root"));
//  names.push_back(TString("Slope1.2GeV_Mass0.8GeVc2"));
//  names.push_back(TString("Slope1.0GeV_Mass0.8GeVc2"));
//  names.push_back(TString("Slope0.7GeV_Mass0.8GeVc2"));
//  names.push_back(TString("Slope0.5GeV_Mass0.8GeVc2"));
//  names.push_back(TString("Slope0.4GeV_Mass0.8GeVc2"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.2 GeV/c; m = 0.8 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 1.0 GeV/c; m = 0.8 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 0.7 GeV/c; m = 0.8 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 0.5 GeV/c; m = 0.8 GeV/c^{2}"));
//  titles.push_back(TString("#LTp_{T}#GT= 0.4 GeV/c; m = 0.8 GeV/c^{2}"));
//  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
//  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
//  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
//  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
//  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));


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
    TH2 * h2 = plotter->getHistogramCollection().loadH2(f,histoNames2D[iFile]);
    if (!h2)
      {
      cout << " Could not load histogram named " << histoNames2D[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
      plotter->plot(h2,names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                  TString("#Delta#eta"),1.0,-1.0,
                  TString("#Delta#varphi"),1.0,-1.0,
                  TString("B_{2}"),1.0,-1.0);
    plotter->createLabel(titles[iFile], -1.1, 0.8, 0.0, 1, 0.05, true);

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
    cout << "Name: " << names[iFile] << endl;
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
//    legendTexts.push_back(names[iFile]);

    }


  plotter->plot(graphs,graphConfigurations1D,titles,"IntegralVsEtaRangeVsMass",landscapeLinear,TString("#Delta#eta"), 0.0, 4.0,TString("Integral"),0.0, 1.2,
                0.4, 0.25, 0.6, 0.45, 0.035);

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
