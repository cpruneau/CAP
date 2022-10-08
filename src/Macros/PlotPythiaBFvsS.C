
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

void calculateRmsWidth(TH1 * h, double lowEdge, double highEdge, double & mean, double & meanError, double & rmsWidth, double & rmsWidthError)
{
  int nBins = h->GetXaxis()->GetNbins();
  int lowBin  = h->GetXaxis()->FindBin(lowEdge);
  int highBin = h->GetXaxis()->FindBin(highEdge);
  if (lowBin<1) lowBin = 1;
  if (highBin>nBins)  highBin = nBins;
  double sum    = 0.0;
  double xSum   = 0.0;
  double x2Sum  = 0.0;
  double eXSum  = 0.0;
  double eX2Sum = 0.0;
  double m1  = 0.0;
  double m2  = 0.0;
  mean       = 0.0;
  meanError  = 0.0;
  rmsWidth   = 0.0;
  rmsWidthError  = 0.0;
  double x, v, ev, arg;

  // two passes: pass 1 to get the mean, pass 2 to get the rms error,
  for (int k=lowBin; k<highBin; k++ )
    {
    x  = h->GetXaxis()->GetBinCenter(k);
    v  = h->GetBinContent(k);
    sum   += v;
    xSum  += x*v;
    x2Sum += x*x*v;
    }
  if (sum>0)
    {
    m1 = xSum/sum;
    m2 = x2Sum/sum - m1*m1;
    if (m2>0.0) rmsWidth = sqrt(m2);

    for (int k=lowBin; k<highBin; k++ )
      {
      x  = h->GetXaxis()->GetBinCenter(k);
      v  = h->GetBinContent(k);
      ev = h->GetBinError(k);
      xSum  += x*x*ev*ev;
      arg = x*x - 2*m1*x;
      x2Sum += arg*arg * ev*ev;
      }
    meanError = sqrt(xSum)/sum;
    rmsWidthError = sqrt(x2Sum)/sum;
    }
}

int plotSet(Plotter * plotter,
            const TString & inputPath,
            const TString & outputPath,
            vector<TString> & histoInputFileNames,
            vector<TFile*>  & histoInputFiles,
            vector<TString> & histoNames2D,
            vector<TString> & histoNames1Dx,
            vector<TString> & histoNames1Dy,
            vector<TString> & names,
            vector<TString> & titles,
            int xAxisType,
            int b2Type,
            bool doWidth=false)
{

  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  vector<GraphConfiguration*>  graphConfigurations2D = GraphConfiguration::createConfigurationPalette(10,2);

  for (int k=0;k<10;k++)
    {
    graphConfigurations1D[k]->setParameter("xTitleSize", 0.08);
    graphConfigurations1D[k]->setParameter("xTitleOffset", 0.8);
    graphConfigurations1D[k]->setParameter("yTitleSize", 0.08);
    graphConfigurations1D[k]->setParameter("yTitleOffset", 0.8);
    graphConfigurations1D[k]->setParameter("xLabelSize", 0.07);
    graphConfigurations1D[k]->setParameter("yLabelSize", 0.07);
    }

  vector<TH2*>    histos2D;
  vector<TH1*>    histos1Dx;
  vector<TH1*>    histos1Dy;
  vector<TGraph*> integralGraphs;
  vector<TH1*>    histos1DxHighRes;
  vector<TH1*>    histos1DyHighRes;
  vector<TGraph*> integralGraphsHighRes;

  TGraph*  widthsGraph;


  TString b2XprojName = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString b2YprojName = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString i2Name      = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString b2XprojNameHighRes = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString b2YprojNameHighRes = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString i2NameHighRes      = "PythiaPPvsS_B2_1_2BarVsDeltaEta";

  TString widthName  = "rms";
  TString sName      = "system";

  double minX = -20.0;
  double maxX =  20.0;
  double minB2 = -0.05;
  double maxB2 =  0.1;
  double minB2x = -0.2199;
  double maxB2x =  0.4999;
  double minB2y = -0.025;
  double maxB2y =  0.5;
  double minI2  = -1.199;
  double maxI2 =  3.199;

  TString titleX;
  TString titleY;
  TString titleB2;
  TString titleI2;

  TLine * line = new TLine(0.0,1.0, 20.0,1.0);
  TLine * line2 = new TLine(0.0,1.0, 3.0,1.0);

  switch (xAxisType)
    {
      case 0: titleX  = "#Delta#eta"; break;
      case 1: titleX  = "#Delta y"; break;
    }
  titleY = "#Delta#varphi";
  switch (b2Type)
    {
      case 0:
      titleB2     = "B^{#bar{1}2}";
      titleI2     = "I^{#bar{1}2}";
      b2XprojName = "PythiaPPvsS_B2_1Bar_2_VsDeltaX";
      b2YprojName = "PythiaPPvsS_B2_1Bar_2_VsDeltaY";
      i2Name      = "PythiaPPvsS_I2_1Bar_2_VsDeltaX";
      b2XprojNameHighRes = "PythiaPPvsS_B2_1Bar_2_VsDeltaXHighRes";
      b2YprojNameHighRes = "PythiaPPvsS_B2_1Bar_2_VsDeltaYHighRes";
      i2NameHighRes      = "PythiaPPvsS_I2_1Bar_2_VsDeltaXHighRes";
      break;
      case 1:
      titleB2 = "B^{1#bar{2}}";
      titleI2 = "I^{1#bar{2}}";
      b2XprojName = "PythiaPPvsS_B2_1_2Bar_VsDeltaX";
      b2YprojName = "PythiaPPvsS_B2_1_2Bar_VsDeltaY";
      i2Name      = "PythiaPPvsS_I2_1_2Bar_VsDeltaX";
      b2XprojNameHighRes = "PythiaPPvsS_B2_1_2Bar_VsDeltaXHighRes";
      b2YprojNameHighRes = "PythiaPPvsS_B2_1_2Bar_VsDeltaYHighRes";
      i2NameHighRes      = "PythiaPPvsS_I2_1_2Bar_VsDeltaXHighRes";
      break;
      case 2:
      titleB2 = "B^{s}";
      titleI2 = "I^{s}";
      b2XprojName = "PythiaPPvsS_B2_12s_VsDeltaX";
      b2YprojName = "PythiaPPvsS_B2_12s_VsDeltaY";
      i2Name      = "PythiaPPvsS_I2_12s_VsDeltaX";
      b2XprojNameHighRes = "PythiaPPvsS_B2_12s_VsDeltaXHighRes";
      b2YprojNameHighRes = "PythiaPPvsS_B2_12s_VsDeltaYHighRes";
      i2NameHighRes      = "PythiaPPvsS_I2_12s_VsDeltaXHighRes";
      break;
    }
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

    TH1 * h1x = plotter->getHistogramCollection().loadH1(f,histoNames1Dx[iFile]);
    if (!h1x)
      {
      cout << " Could not load histogram named " << histoNames1Dx[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
    histos1Dx.push_back(h1x);
    histos1DxHighRes.push_back(h1x);

    TH1 * h1y = plotter->getHistogramCollection().loadH1(f,histoNames1Dy[iFile]);
    if (!h1y)
      {
      cout << " Could not load histogram named " << histoNames1Dy[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
    histos1Dy.push_back(h1y);

    double dx = histos2D[iFile]->GetXaxis()->GetBinWidth(1);
    double dy = histos2D[iFile]->GetYaxis()->GetBinWidth(1);
    //histos2D[iFile]->Scale(1.0/dx/dy);
    histos2D[iFile]->Scale(1.0/dy);
    float size = 0.07;
    graphConfigurations2D[iFile]->setParameter("xTitleOffset",  1.2);
    graphConfigurations2D[iFile]->setParameter("xTitleSize",   size);
    graphConfigurations2D[iFile]->setParameter("xLabelOffset", 0.01);
    graphConfigurations2D[iFile]->setParameter("xLabelSize",   size);

    graphConfigurations2D[iFile]->setParameter("yTitleOffset", 1.2);
    graphConfigurations2D[iFile]->setParameter("yTitleSize",   size);
    graphConfigurations2D[iFile]->setParameter("yLabelOffset", 0.01);
    graphConfigurations2D[iFile]->setParameter("yLabelSize",   size);

    graphConfigurations2D[iFile]->setParameter("zTitleOffset", 1.2);
    graphConfigurations2D[iFile]->setParameter("zTitleSize",   size);
    graphConfigurations2D[iFile]->setParameter("zLabelOffset", 0.01);
    graphConfigurations2D[iFile]->setParameter("zLabelSize",   size);


    plotter->plot(histos2D[iFile],names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                  titleX,minX,maxX,
                  titleY,1.0,-1.0,
                  titleB2,minB2,maxB2);
    //TString("B_{2}"),0.0,0.002);
    plotter->createLabel(titles[iFile], -1.1, 0.8, 0.0, 1, 0.07, true);

    double * x  = new double[500];
    double * ex = new double[500];
    double * y  = new double[500];
    double * ey = new double[500];
    double sum, esum;
    const TAxis * xAxis  = h2->GetXaxis();
    const TAxis * yAxis  = h2->GetYaxis();
    int xLow  = xAxis->GetXmin();
    int xHigh = xAxis->GetXmax();
    int yLow  = yAxis->FindBin(-0.5*TMath::Pi());
    int yHigh = yAxis->FindBin( 1.5*TMath::Pi());

    int nPoints = 0;
    double etaLow  =  0.1;
    double etaHigh =  maxX-0.05;
    double etaStep =  0.2;
    cout << "File: " << names[iFile] << endl;
    sum = 0;
    esum = 0;
    for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
      {
      int xLow  = xAxis->FindBin(-eta);
      int xHigh = xAxis->FindBin( eta);
      double factor = 1-eta/xHigh;
      sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
      x[nPoints]  = eta;
      ex[nPoints] = 0.001;
      y[nPoints]  = sum; ///TMath::TwoPi();
      ey[nPoints] = esum; ///TMath::TwoPi();
      nPoints++;
      }
    TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
    TGraph * gHR = new TGraphErrors(nPoints, x,y,ex,ey);
    integralGraphs.push_back(g);
    integralGraphsHighRes.push_back(gHR);
    }



  switch (b2Type)
    {
      case 0:
      plotter->plot(histos1Dx,graphConfigurations1D,titles,b2XprojName,landscapeLinear,titleX, minX, maxX,titleB2,minB2x,maxB2x, 0.57, 0.6, 0.75, 0.9, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,b2YprojName,landscapeLinear,titleY, 1.0, -1.0, titleB2,minB2y,maxB2y, 0.6, 0.6, 0.75, 0.9, 0.04);
      plotter->plot(integralGraphs,   graphConfigurations1D,titles,i2Name,     landscapeLinear,titleX, 0.0, maxX, titleI2,minI2,maxI2,   0.52, 0.6, 0.85, 0.89, 0.04);
      line->Draw();
      plotter->plot(histos1DxHighRes,graphConfigurations1D,titles,b2XprojNameHighRes,landscapeLinear,titleX, -3.0, 3.0,titleB2,-0.02, 0.49, 0.40, 0.2, 0.60, 0.45, 0.04);
      plotter->plot(integralGraphsHighRes, graphConfigurations1D,titles,i2NameHighRes,     landscapeLinear,titleX, 0.0, 3.0, titleI2,-0.02, 1.199,   0.52, 0.3, 0.85, 0.6, 0.04);
      line2->Draw();
      break;
      case 1:
      plotter->plot(histos1Dx,graphConfigurations1D,titles,b2XprojName,landscapeLinear,titleX, minX, maxX,titleB2,minB2x,maxB2x, 0.57, 0.6, 0.75, 0.9, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,b2YprojName,landscapeLinear,titleY, 1.0, -1.0, titleB2,minB2y,maxB2y, 0.6, 0.2, 0.75, 0.5, 0.04);
      plotter->plot(integralGraphs,   graphConfigurations1D,titles,i2Name,     landscapeLinear,titleX, 0.0, maxX, titleI2,minI2,maxI2,   0.52, 0.2, 0.85, 0.49, 0.04);
      line->Draw();
      plotter->plot(histos1DxHighRes,graphConfigurations1D,titles,b2XprojNameHighRes,landscapeLinear,titleX, -3.0, 3.0,titleB2,-0.02, 0.49, 0.40, 0.2, 0.60, 0.45, 0.04);
      plotter->plot(integralGraphsHighRes, graphConfigurations1D,titles,i2NameHighRes,     landscapeLinear,titleX, 0.0, 3.0, titleI2,-0.02, 1.199,   0.52, 0.3, 0.85, 0.6, 0.04);
      line2->Draw();

      break;
      case 2:
      plotter->plot(histos1Dx,graphConfigurations1D,titles,b2XprojName,landscapeLinear,titleX, minX, maxX,titleB2, minB2x,maxB2x, 0.57, 0.6, 0.75, 0.9, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,b2YprojName,landscapeLinear,titleY, 1.0, -1.0, titleB2, 0.0,0.04,      0.6, 0.6, 0.75, 0.9, 0.04);
      plotter->plot(integralGraphs,   graphConfigurations1D,titles,i2Name,     landscapeLinear,titleX, 0.0, maxX, titleI2,minI2,maxI2,    0.52, 0.2, 0.85, 0.49, 0.04);
      line->Draw();
      plotter->plot(histos1DxHighRes,graphConfigurations1D,titles,b2XprojNameHighRes,landscapeLinear,titleX, -3.0, 3.0,titleB2,-0.02, 0.49, 0.40, 0.2, 0.60, 0.45, 0.04);
      plotter->plot(integralGraphsHighRes, graphConfigurations1D,titles,i2NameHighRes,     landscapeLinear,titleX, 0.0, 3.0, titleI2,-0.02, 1.199,   0.52, 0.3, 0.85, 0.6, 0.04);
      line2->Draw();
      break;

    }

  return 0;
}

int PlotPythiaBFvsS(int option = 22)
{
  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 1;
  bool useColor = true;

  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  MessageLogger::LogLevel infoLevel = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Info;
  MessageLogger::LogLevel selectLevel = infoLevel;


  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/";

  vector<TString> histoInputFileNames;
  vector<TFile*>  histoInputFiles;
  vector<TString> histoNames2D;
  vector<TString> histoNames1Dx;
  vector<TString> histoNames1Dy;
  vector<TString> names;
  vector<TString> titles;

  Configuration plotConfig;

  Plotter * plotter = new Plotter("Plotter",plotConfig);
  plotter->setDefaultOptions(useColor);


 // vector<TGraph*>  graphs;

    switch (option)
    {
      case 8:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5200/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      //      histoInputFileNames.push_back(TString("PP/13000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      //      names.push_back(TString("PYTHIA_pp_13TeV_B2Based_B2_1Bar_2"));
      //      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      //      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      //      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      //      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      outputPath += "/B2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,0);
      break;



      case 9:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5200/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

//      histoInputFileNames.push_back(TString("PP/13000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
//      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_1Bar_2"));
//      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
//      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
//      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
//      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      outputPath += "/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,0);
      break;

      case 10:
      //
      histoInputFileNames.push_back(TString("PP/1000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(f) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(g) #sqrt{s} = 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5200/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(h) #sqrt{s} = 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(i) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(j) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      outputPath += "/A2Derived/";

      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,1);
      break;

      case 11:
      // B2_12Sum
      histoInputFileNames.push_back(TString("PP/1000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(k) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(l) #sqrt{s} = 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/5200/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(m) #sqrt{s} = 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(n) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(o) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      outputPath += "/A2Derived/";

      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,2);
      break;


      case 20:
      // Y10 -- b2
      histoInputFileNames.push_back(TString("PP/1000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5200/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      outputPath += "/B2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,0);
      break;

      case 21:
      // Y10 -- b2
      histoInputFileNames.push_back(TString("PP/1000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5200/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      outputPath += "/B2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,1);
      break;

      case 22:
      // Y10 -- b2
      histoInputFileNames.push_back(TString("PP/1000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/2700/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/5200/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      outputPath += "/B2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,2);
      break;

// a2
      case 23:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5200/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      outputPath += "/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,0);
      break;

      case 24:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5200/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      outputPath += "/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,1);
      break;

      case 25:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/2700/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/5200/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      outputPath += "/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,2);
      break;


// a2 nominal, shoving, ropes

      // a2
      case 31:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_Nominal_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 13.0 TeV Monash CR=ON"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/Ropes/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_Ropes_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) #sqrt{s} = 13.0 TeV Ropes"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/Shoving/PairGenBalFctSum0TO6.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_Shoving_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(c) #sqrt{s} = 13.0 TeV Shoving"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      outputPath += "/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,0);
      break;

      case 32:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5200/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      outputPath += "/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,1);
      break;

      case 33:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/2700/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/5200/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PMgrid/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PMgrid/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      outputPath += "/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,1,2);
      break;



    }

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
  gSystem->Load(includePath+"Plotter.hpp");
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
}
