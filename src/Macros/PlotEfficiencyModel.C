
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

int PlotEfficiencyModel()
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
  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/EfficiencyModel/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/EfficiencyModel/";
  vector<TGraph*>  graphs;
  vector<TString>  legendTexts;

  int nCurves = 3;
  vector<double> peakPt;
  vector<double> peakAmp;
  vector<double> peakRms;
  vector<double> a1Pt;
  vector<double> a2Pt;

  peakPt.push_back(0.000);
  peakAmp.push_back(0.805);
  peakRms.push_back(0.01);
  a1Pt.push_back(0.000);
  a2Pt.push_back(0.000);
  legendTexts.push_back(TString("#varepsilon = 0.8"));

  peakPt.push_back(1.000);
  peakAmp.push_back(0.80);
  peakRms.push_back(0.500);
  a1Pt.push_back(0.000);
  a2Pt.push_back(0.000);
  legendTexts.push_back(TString("#varepsilon_{peak} = 0.80; p_{T,peak} = 1.0; #sigma = 0.5"));

  peakPt.push_back(1.000);
  peakAmp.push_back(0.80);
  peakRms.push_back(0.300);
  a1Pt.push_back(-0.050);
  a2Pt.push_back(0.000);
  legendTexts.push_back(TString("#varepsilon_{peak} = 0.80; p_{T,peak}= 1.0; #sigma = 0.3; a_{1}=-0.05"));

  int nPoints = 100;
  double * x;
  double * y;
  TGraph * g;
  double pt     = 0;
  double ptStep = 6.0/double(nPoints);
  double eff;
  for (int iCurve=0; iCurve<nCurves; iCurve++)
    {
    x  = new double[nPoints];
    y  = new double[nPoints];

    for (int k=0;k<nPoints; k++)
      {
      pt  = 0.001 + double(k)*ptStep;
      double arg1 = pt-peakPt[iCurve];
      double arg2 = arg1/peakRms[iCurve];
      eff = arg1<=0 ?  peakAmp[iCurve]*exp(-arg2*arg2/2.0) : peakAmp[iCurve] + a1Pt[iCurve]*arg1 + a2Pt[iCurve]*arg1*arg1;
      x[k]  = pt;
      y[k]  = eff;
      }
    g = new TGraph(nPoints, x,y);
    graphs.push_back(g);
    graphConfigurations1D[iCurve]->setParameter("markerSize",  0.1);
    graphConfigurations1D[iCurve]->setParameter("lineWidth",  4);
    }

  Plotter * plotter = new Plotter("Plotter",Configuration(),MessageLogger::Info);
  plotter->setDefaultOptions(useColor);
  plotter->plot(graphs,graphConfigurations1D,legendTexts,"EffVsPt",landscapeLinear,TString("p_{T}"), 0.0, 6.0,TString("#varepsilon(p_{T})"),0.0, 1.05,
                0.35, 0.2, 0.5, 0.5, 0.045);
  plotter->printAllCanvas(outputPath, printGif, printPdf, printSvg, printC);
  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Collection.hpp");
  gSystem->Load(includePath+"CanvasCollection.hpp");
  gSystem->Load(includePath+"GraphConfiguration.hpp");
  gSystem->Load(includePath+"CanvasConfiguration.hpp");
  gSystem->Load(includePath+"Plotter.hpp");
  gSystem->Load("libBase.dylib");
}
