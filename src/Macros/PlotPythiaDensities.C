
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

TH1 * getProjectionX(TFile * inputFile, const TString & histoName2D)
{
  TH2 * h2 = (TH2*) inputFile->Get(histoName2D);
  if (!h2)
    {
    cout << "Histogram named " << histoName2D << " not found - abort" << endl;
    return nullptr;
    }
  TH1 * proj = h2->ProjectionX();
  return proj;
}

double scaleAndGetIntegral(TH1 * h)
{
  double binWidth = h->GetXaxis()->GetBinWidth(1);
  double scale = 1.0/binWidth;
  h->Scale(scale);
  double integral = h->Integral("WIDTH");
  cout << "Integral of " << h->GetName() << "  :   " << integral  << endl;
  return integral;
}

TH1 * computeRatio(TH1 * num, TH1* dem)
{
  TH1 * clone = (TH1*) num->Clone();
  clone->Divide(dem);
  return clone;
}


int PlotPythiaDensities()
{
  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  MessageLogger::LogLevel infoLevel = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Info;
  MessageLogger::LogLevel selectLevel = infoLevel;

  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 1;
  bool useColor = true;
  //CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLinear(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLinearTight(CanvasConfiguration::LandscapeTight,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  vector<GraphConfiguration*>  graphConfigurations2D = GraphConfiguration::createConfigurationPalette(10,2);
  //landscapeLinear.setParameter("windowHeight", 300);
  for (unsigned int iGraph=0; iGraph<8; iGraph++)
    {
    graphConfigurations1D[iGraph]->setParameter("yTitleOffset",  0.9);
    graphConfigurations1D[iGraph]->setParameter("xTitleSize",   0.08);
    graphConfigurations1D[iGraph]->setParameter("xTitleOffset", 0.8);
    graphConfigurations1D[iGraph]->setParameter("xLabelSize",   0.06);
    graphConfigurations1D[iGraph]->setParameter("xLabelOffset", 0.01);
    graphConfigurations1D[iGraph]->setParameter("yTitleSize",   0.08);
    graphConfigurations1D[iGraph]->setParameter("yTitleOffset", 0.7);
    graphConfigurations1D[iGraph]->setParameter("yLabelSize",   0.06);
    graphConfigurations1D[iGraph]->setParameter("yLabelOffset", 0.01);
    graphConfigurations1D[iGraph]->setParameter("markerSize",   0.5);
    graphConfigurations1D[iGraph]->setParameter("markerStyle",  20);
    if (iGraph<4)
      graphConfigurations1D[iGraph]->setParameter("markerStyle",  kFullSquare);
    else
      graphConfigurations1D[iGraph]->setParameter("markerStyle",  kOpenSquare);
    }
  graphConfigurations1D[0]->setParameter("markerColor", kBlack);
  graphConfigurations1D[0]->setParameter("lineColor",   kBlack);
  graphConfigurations1D[1]->setParameter("markerColor", kBlue);
  graphConfigurations1D[1]->setParameter("lineColor",   kBlue);
  graphConfigurations1D[2]->setParameter("markerColor", kRed);
  graphConfigurations1D[2]->setParameter("lineColor",   kRed);
  graphConfigurations1D[3]->setParameter("markerColor", kGreen);
  graphConfigurations1D[3]->setParameter("lineColor",   kGreen);
  graphConfigurations1D[4+0]->setParameter("markerColor", kBlack);
  graphConfigurations1D[4+0]->setParameter("lineColor",   kBlack);
  graphConfigurations1D[4+1]->setParameter("markerColor", kBlue);
  graphConfigurations1D[4+1]->setParameter("lineColor",   kBlue);
  graphConfigurations1D[4+2]->setParameter("markerColor", kRed);
  graphConfigurations1D[4+2]->setParameter("lineColor",   kRed);
  graphConfigurations1D[4+3]->setParameter("markerColor", kGreen);
  graphConfigurations1D[4+3]->setParameter("lineColor",   kGreen);



//  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/2700/PM/Y10WidePt/";
//  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/2700/PM/Y10WidePt/";
  TString inputPath1  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/13000/PM/Y10/";
  TString inputPath2  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/13000/PiKp/y10/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/13000/PiKp/y10/";

  vector<TH2*>    histos2D;
  vector<TH1*>    histos1Dx;
  vector<TH1*>    ratios1Dx;
  vector<TString> titles;
  vector<TString> ratiosTitles;

  Configuration taskConfig;
  Plotter * plotter = new Plotter("Plotter",taskConfig);
  plotter->setDefaultOptions(useColor);

  TFile * f1 =  plotter->openRootFile(inputPath1,"PairGenDerivedSum0TO4.root","OLD");
  if (!f1)
    {
    cout << " Could not open file named PairGenDerivedSum0TO4.root" <<   endl;
    cout << " ABORT!!!!" << endl;
    return -1;
    }
  TH1 * hadronPvsY  = getProjectionX(f1,"PairGen_All_HP_n1_phiY");
  TH1 * hadronMvsY  = getProjectionX(f1,"PairGen_All_HM_n1_phiY");
  if (!hadronPvsY || !hadronMvsY)
    {
    cout << " Histograms not found" <<   endl;
    cout << " ABORT!!!!" << endl;
    return -1;
    }

  TFile * f2 =  plotter->openRootFile(inputPath2,"PairGenBalFctSum0TO9.root","OLD");
  if (!f2)
    {
    cout << " Could not open file named PairGenBalFctSum0TO9.root" <<   endl;
    cout << " ABORT!!!!" << endl;
    return -1;
    }

  TH1 * PiPvsY  = (TH1*) f2->Get("PairGen_All_PiP_n1_y");
  TH1 * KPvsY   = (TH1*) f2->Get("PairGen_All_KP_n1_y");
  TH1 * PPvsY   = (TH1*) f2->Get("PairGen_All_PP_n1_y");
  TH1 * PiMvsY  = (TH1*) f2->Get("PairGen_All_PiM_n1_y");
  TH1 * KMvsY   = (TH1*) f2->Get("PairGen_All_KM_n1_y");
  TH1 * PMvsY   = (TH1*) f2->Get("PairGen_All_PM_n1_y");
  if (!PiPvsY || !KPvsY || !PPvsY || !PiMvsY || !KMvsY || !KMvsY)
    {
    cout << " Some Histograms not found" <<   endl;
    cout << " ABORT!!!!" << endl;
    return -1;
    }
  titles.push_back(TString("#rho_{1}^{+}"));
  titles.push_back(TString("#rho_{1}^{#pi^{+}}"));
  titles.push_back(TString("#rho_{1}^{K^{+}}"));
  titles.push_back(TString("#rho_{1}^{p^{+}}"));

  titles.push_back(TString("#rho_{1}^{-}"));
  titles.push_back(TString("#rho_{1}^{#pi^{-}}"));
  titles.push_back(TString("#rho_{1}^{K^{-}}"));
  titles.push_back(TString("#rho_{1}^{p^{-}}"));

  histos1Dx.push_back(hadronPvsY);
  histos1Dx.push_back(PiPvsY);
  histos1Dx.push_back(KPvsY);
  histos1Dx.push_back(PPvsY);

  histos1Dx.push_back(hadronMvsY);
  histos1Dx.push_back(PiMvsY);
  histos1Dx.push_back(KMvsY);
  histos1Dx.push_back(PMvsY);

  double integralHadronP = scaleAndGetIntegral(hadronPvsY);
  double integralHadronM = scaleAndGetIntegral(hadronMvsY);
  double integralPiP = scaleAndGetIntegral(PiPvsY);
  double integralKP  = scaleAndGetIntegral(KPvsY);
  double integralPP  = scaleAndGetIntegral(PPvsY);
  double integralPiM = scaleAndGetIntegral(PiMvsY);
  double integralKM  = scaleAndGetIntegral(KMvsY);
  double integralPM  = scaleAndGetIntegral(PMvsY);

  plotter->plot(histos1Dx,graphConfigurations1D,titles,"PYTHIA_pp_13.0TeV_rho1_y_HPHMPiKP",landscapeLinear,TString("y"), -10.0, 10.0,TString("#rho_{1}(y)"),0.0, 4.499,
                0.70, 0.6, 0.78, 0.92, 0.06);

  TH1 *ratioH   = computeRatio(hadronPvsY,hadronMvsY);
  TH1 *ratioPi  = computeRatio(PiPvsY,PiMvsY);
  TH1 *ratioK   = computeRatio(KPvsY,KMvsY);
  TH1 *ratioP   = computeRatio(PPvsY,PMvsY);

  ratios1Dx.push_back(ratioH);
  ratios1Dx.push_back(ratioPi);
  ratios1Dx.push_back(ratioK);
  ratios1Dx.push_back(ratioP);

  ratiosTitles.push_back(TString("#rho_{1}^{+}/#rho_{1}^{-}"));
  ratiosTitles.push_back(TString("#rho_{1}^{#pi^{+}}/#rho_{1}^{#pi^{-}}"));
  ratiosTitles.push_back(TString("#rho_{1}^{K^{+}}/#rho_{1}^{K^{-}}"));
  ratiosTitles.push_back(TString("#rho_{1}^{p^{+}}/#rho_{1}^{p^{-}}"));

  plotter->plot(ratios1Dx,graphConfigurations1D,ratiosTitles,"PYTHIA_pp_13.0TeV_ratios_y_HPHMPiKP",landscapeLogY,TString("y"), -10.0, 10.0,TString("Ratio"),0.5, 199.99,
                0.50, 0.45, 0.70, 0.85, 0.06);


//  GraphConfiguration config = *new GraphConfiguration(1,0);
//  config.setParameter("xTitleSize",   0.1);
//  config.setParameter("xTitleOffset", 0.7);
//  config.setParameter("xLabelSize",   0.08);
//  config.setParameter("xLabelOffset", 0.01);
//  config.setParameter("yTitleSize",   0.11);
//  config.setParameter("yTitleOffset", 0.5);
//  config.setParameter("yLabelSize",   0.085);
//  config.setParameter("yLabelOffset", 0.01);
//
//  plotter->plot(diff,"PYTHIA_pp_13.0TeV_rho1_y_Difference",landscapeLinear,config,
//                TString("y"), -10.0, 10.0,
//                TString("#rho_{1}^{+}(y)-#rho_{1}^{-}(y)"),-0.0990, 0.750,
//                "PYTHIA8 pp #sqrt{s}=13.0 TeV", 0.5, 0.77, 0.82, 0.90, 0.08);

  plotter->printAllCanvas(outputPath, printGif, printPdf, printSvg, printC);
  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Configuration.hpp");
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
