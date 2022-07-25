
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

int PlotPBF()
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
  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PythiaOpenPtWide/PYTHIA/PP/13000/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/PythiaOpenPtWide/PYTHIA/PP/13000/";

  //   /Volumes/ClaudeDisc4/OutputFiles/PythiaOpenPtWide/PYTHIA/PP/13000/PYTHIA_Pair_Gen_Derived_BalFct.root

  vector<TString> histoInputFileNames;
  vector<TFile*>  histoInputFiles;
  vector<TString> histoNames2D;
  vector<TString> histoNames1Dx;
  vector<TString> histoNames1Dy;
  vector<TString> names;
  vector<TString> titles;
  vector<TH2*>    histos2D;
  vector<TH1*>    histos1Dx;
  vector<TH1*>    histos1Dy;

  histoInputFileNames.push_back(TString("PYTHIA_Pair_Gen_Derived.root"));

//  names.push_back(TString("PYTHIA_pp_13TeV_B2_1_2Bar"));
//  titles.push_back(TString("B_{2}^{1#bar{2}}"));
//  histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
//  histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
//  histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

  Plotter * plotter = new Plotter("Plotter",Configuration(),MessageLogger::Info);
  plotter->setDefaultOptions(useColor);

  vector<TGraph*>  graphs;
  vector<TString>  legendTexts;

  int iFile = 0;
  TFile * f =  plotter->openRootFile(inputPath,histoInputFileNames[iFile],"OLD");
  if (!f)
    {
    cout << " Could not open file named " << histoInputFileNames[iFile] << endl;
    cout << " ABORT!!!!" << endl;
    return -1;
    }


  TH1 * Pair_Gen_All_HP_n1_eta = plotter->getHistogramCollection().loadH1(f,"Pair_Gen_All_HP_n1_eta");
  cout << "HP n1 Integral    : " << Pair_Gen_All_HP_n1_eta->Integral() << endl;
  cout << "HP n1 Integral(W) : " << Pair_Gen_All_HP_n1_eta->Integral("WIDTH") << endl;
  TH1 * Pair_Gen_All_HM_n1_eta = plotter->getHistogramCollection().loadH1(f,"Pair_Gen_All_HM_n1_eta");
  cout << "HM n1 Integral    : " << Pair_Gen_All_HM_n1_eta->Integral() << endl;
  cout << "HM n1 Integral(W) : " << Pair_Gen_All_HM_n1_eta->Integral("WIDTH") << endl;

  TH2 *  Pair_Gen_All_HP_HM_R2_DetaDphi_shft = plotter->getHistogramCollection().loadH2(f,"Pair_Gen_All_HP_HM_R2_DetaDphi_shft");
  TH2 *  Pair_Gen_All_HM_HM_R2_DetaDphi_shft = plotter->getHistogramCollection().loadH2(f,"Pair_Gen_All_HM_HM_R2_DetaDphi_shft");
  // Pair_Gen_All_HP_HM_R2_DetaDphi_shft->Draw("ZCOL");
  //Pair_Gen_All_HM_HM_R2_DetaDphi_shft->Draw("ZCOL");

  TH2 * bf = (TH2*) Pair_Gen_All_HP_HM_R2_DetaDphi_shft->Clone();
  bf->Add(Pair_Gen_All_HM_HM_R2_DetaDphi_shft,-1.0);
  bf->Draw("SURF3");
  double scale = Pair_Gen_All_HM_n1_eta->Integral("WIDTH");
  scale /= 8.0;
  bf->Scale(scale);
  cout << "HM BF Integral    : " << bf->Integral() << endl;
  cout << "HM BF Integral(W) : " << bf->Integral("WIDTH") << endl;


//  TH2 * h2 = plotter->getHistogramCollection().loadH2(f,histoNames2D[iFile]);
//
//
//  for (unsigned int iFile=0; iFile<histoInputFileNames.size(); iFile++)
//    {
//    TFile * f =  plotter->openRootFile(inputPath,histoInputFileNames[iFile],"OLD");
//    if (!f)
//      {
//      cout << " Could not open file named " << histoInputFileNames[iFile] << endl;
//      cout << " ABORT!!!!" << endl;
//      return -1;
//      }
//    histoInputFiles.push_back(f);
//
//    TH1 * Pair_Gen_All_HP_n1_eta = plotter->getHistogramCollection().loadH2(f,histoNames2D[iFile]);
//    TH2 * h2 = plotter->getHistogramCollection().loadH2(f,histoNames2D[iFile]);
//    if (!h2)
//      {
//      cout << " Could not load histogram named " << histoNames2D[iFile] << endl;
//      cout << " ABORT!!!!" << endl;
//      return -1;
//      }
//    histos2D.push_back(h2);

//    TH1 * h1x = plotter->getHistogramCollection().loadH2(f,histoNames1Dx[iFile]);
//    if (!h1x)
//      {
//      cout << " Could not load histogram named " << histoNames1Dx[iFile] << endl;
//      cout << " ABORT!!!!" << endl;
//      return -1;
//      }
//    histos1Dx.push_back(h1x);
//
//    TH1 * h1y = plotter->getHistogramCollection().loadH2(f,histoNames1Dy[iFile]);
//    if (!h1y)
//      {
//      cout << " Could not load histogram named " << histoNames1Dy[iFile] << endl;
//      cout << " ABORT!!!!" << endl;
//      return -1;
//      }
//    histos1Dy.push_back(h1y);
//
//
//
//      plotter->plot(h2,names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
//                  TString("#Delta#eta"),1.0,-1.0,
//                  TString("#Delta#varphi"),1.0,-1.0,
//                    TString("B_{2}"),0.0,0.01);
//    //TString("B_{2}"),0.0,0.002);
//    plotter->createLabel(titles[iFile], -1.1, 0.8, 0.0, 1, 0.07, true);
//
//    double * x  = new double[500];
//    double * ex = new double[500];
//    double * y  = new double[500];
//    double * ey = new double[500];
//    double sum, esum;
//    const TAxis * xAxis  = h2->GetXaxis();
//    const TAxis * yAxis  = h2->GetYaxis();
//    int yLow  = yAxis->FindBin(-0.5*TMath::Pi());
//    int yHigh = yAxis->FindBin( 1.5*TMath::Pi());
//
//    int nPoints = 0;
//    double etaLow  =  0.1;
//    double etaHigh =  7.95;
//    double etaStep =  0.2;
//    cout << "File: " << names[iFile] << endl;
//    for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
//      {
//      int xLow  = xAxis->FindBin(-eta);
//      int xHigh = xAxis->FindBin( eta);
//      sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum);
//      cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
//      x[nPoints]  = eta;
//      ex[nPoints] = 0.001;
//      y[nPoints]  = sum;
//      ey[nPoints] = esum;
//      nPoints++;
//      }
//    TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
//    graphs.push_back(g);
////    legendTexts.push_back(names[iFile]);

//    }

//  plotter->plot(histos1Dx,graphConfigurations1D,titles,"B2x",landscapeLinear,TString("#Delta#eta"), -8.0, 8.0,TString("B_{2}"),-0.01, 0.1,
//                0.67, 0.5, 0.85, 0.7, 0.06);
//  TLine * line1 = new TLine(-8.0,0.0, 8.0,0.0);
//  line1->Draw();
//
//  plotter->plot(histos1Dy,graphConfigurations1D,titles,"B2y",landscapeLinear,TString("#Delta#varphi"), -1.5, 4.7,TString("B_{2}"),-0.001, 0.03,
//                0.67, 0.6, 0.85, 0.8, 0.06);
//  TLine * line2 = new TLine(-1.5,0.0, 4.7,0.0);
//  line2->Draw();
//
//  plotter->plot(graphs,graphConfigurations1D,titles,"IntegralVsEtaRange",landscapeLinear,TString("#Delta#eta"), 0.0, 4.0,TString("Integral"),0.0, 1.2,
//                0.5, 0.3, 0.8, 0.5, 0.05);
//  TLine * line = new TLine(0.0,1.0, 8.0,1.0);
//  line->Draw();

  if (false) plotter->printAllCanvas(outputPath, printGif, printPdf, printSvg, printC);
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
