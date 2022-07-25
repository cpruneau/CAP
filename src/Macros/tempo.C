
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
             int b2Type)
{
  vector<TH2*>    histos2D;
  vector<TH1*>    histos1Dx;
  vector<TH1*>    histos1Dy;
  vector<TGraph*> graphs;

  TString b2XprojName = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString b2YprojName = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString i2Name      = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  double minX = -10.0;
  double maxX =  10.0;
  double minB2x = -10.0;
  double maxB2x =  10.0;
  double minB2y = -10.0;
  double maxB2y =  10.0;
  double minI2  = -10.0;
  double maxI2 =  10.0;

  TString titleX;
  TString titleY;
  TString titleB2;
  TString titleI2;
  switch (xAxisType)
    {
      case 0: titleX  = "#Delta#eta"; break;
      case 1: titleX  = "#Delta y"; break;
    }
  titleY = "#Delta#varphi";
  switch (b2Type)
    {
      case 0:
      titleB2     = "B_{2}^{#bar{1}2}";
      titleI2     = "I_{2}^{#bar{1}2}";
      b2XprojName = "PythiaPPvsS_B2_1Bar_2_VsDeltaX";
      b2YprojName = "PythiaPPvsS_B2_1Bar_2_VsDeltaY";
      i2Name      = "PythiaPPvsS_I2_1Bar_2_VsDeltaX";
      break;
      case 1:
      titleB2 = "B_{2}^{1#bar{2}}";
      titleI2 = "I_{2}^{1#bar{2}}";
      b2XprojName = "PythiaPPvsS_B2_1_2Bar_VsDeltaX";
      b2YprojName = "PythiaPPvsS_B2_1_2Bar_VsDeltaY";
      i2Name      = "PythiaPPvsS_I2_1_2Bar_VsDeltaX";
      break;
      case 2:
      titleB2 = "B_{2}^{s}";
      titleI2 = "I_{2}^{s}";
      b2XprojName = "PythiaPPvsS_B2_12s_VsDeltaX";
      b2YprojName = "PythiaPPvsS_B2_12s_VsDeltaY";
      i2Name      = "PythiaPPvsS_I2_12s_VsDeltaX";
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
    histos2D[iFile]->Scale(1.0/dx/dy);
    plotter->plot(histos2D[iFile],names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                  titleDeltaEta,-4.0, 4.0,
                  titleDeltaPhi,1.0,-1.0,
                  titleB2_1_2Bar,-0.005,0.10);
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
    double etaHigh =  7.95;
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
      y[nPoints]  = sum;
      ey[nPoints] = esum;
      nPoints++;
      }
    TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
    graphs.push_back(g);
    }

  plotter->plot(histos1Dx,graphConfigurations1D,titles,b2XprojName,landscapeLinear,titleX, minX, maxX,titleB2,minB2x,maxB2x, 0.6, 0.5, 0.75, 0.8, 0.04);
  plotter->plot(histos1Dy,graphConfigurations1D,titles,b2YprojName,landscapeLinear,titleY, 1.0, -1.0, titleB2,minB2y,maxB2y, 0.6, 0.5, 0.75, 0.8, 0.04);
  plotter->plot(graphs,graphConfigurations1D,titles,i2Name,landscapeLinear,titleX, 0.0, maxX,titleB2,0.0, 1.1, 0.5, 0.2, 0.8, 0.5, 0.05);
  //line->Draw();
  return 0;
}

int PlotPythiaBFvsS()
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
  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  vector<GraphConfiguration*>  graphConfigurations2D = GraphConfiguration::createConfigurationPalette(10,2);

  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/";

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

  TString titleDeltaEta = "#Delta#eta";
  TString titleDeltaY   = "#Delta y";
  TString titleDeltaPhi = "#Delta#varphi";
  TString titleB2_1Bar_2 = "B_{2}^{#bar{1}2}";
  TString titleI2_1Bar_2 = "I_{2}^{#bar{1}2}";

  TString titleB2_1_2Bar = "B_{2}^{1#bar{2}}";
  TString titleI2_1_2Bar = "I_{2}^{1#bar{2}}";

  TString titleB2_12Sum  = "B_{2}^{S}";
  TString titleI2_12Sum  = "I_{2}^{S}";

  Plotter * plotter = new Plotter("Plotter",Configuration(),MessageLogger::Info);
  plotter->setDefaultOptions(useColor);

  TLine * line = new TLine(0.0,1.0, 8.0,1.0);

  vector<TGraph*>  graphs;

  int option = 6;
  switch (option)
    {
      case 0:
      //Eta2WidePt
      histoInputFileNames.push_back(TString("PP/1000/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1_2Bar+" - 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1_2Bar+" - 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5200/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1_2Bar+" - 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1_2Bar+" - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1_2Bar+" - p#bar{p} - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

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
        histos2D[iFile]->Scale(1.0/dx/dy);
        plotter->plot(histos2D[iFile],names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                      titleDeltaEta,-4.0, 4.0,
                      titleDeltaPhi,1.0,-1.0,
                      titleB2_1_2Bar,-0.005,0.10);
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
        double etaHigh =  7.95;
        double etaStep =  0.2;
        cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          //cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }
      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2BarVsDeltaEta",landscapeLinear,titleDeltaEta, -4.0, 4.0,titleB2_1Bar_2,0.0, 0.10,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2BarVsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_1Bar_2,0.0, 0.03,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_1_2BarVsEtaRange",landscapeLinear,titleDeltaEta, 0.0, 4.0,titleB2_1Bar_2,0.0, 1.1,
                    0.5, 0.2, 0.8, 0.5, 0.05);
      line->Draw();
      break;

      case 1:
      histoInputFileNames.push_back(TString("PP/1000/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 1.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 2.7 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5200/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Eta2WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - p#bar{p} - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));


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
        histos2D[iFile]->Scale(1.0/dx/dy);
        plotter->plot(histos2D[iFile],names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                      TString("#Delta#eta"),-4.0, 4.0,
                      TString("#Delta#varphi"),1.0,-1.0,
                      titleB2_1_2Bar,1.0,-1.0);
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
        double etaHigh =  7.95;
        double etaStep =  0.2;
        cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          // cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }

      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2Bar_VsDeltaEta",landscapeLinear,titleDeltaEta, -4.0, 4.0,titleB2_1_2Bar,0.0, 0.10,
                    0.6, 0.5, 0.75, 0.8, 0.04);

      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2Bar_VsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_1_2Bar,0.0, 0.03,
                    0.6, 0.5, 0.75, 0.8, 0.04);

      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_1_2Bar_VsEtaRange",landscapeLinear,titleDeltaEta, 0.0, 4.0,titleI2_1_2Bar,0.0, 1.1,
                    0.5, 0.2, 0.8, 0.5, 0.05);
      //TLine * line = new TLine(0.0,1.0, 8.0,1.0);
      line->Draw();
      break;


      case 2:
      //Eta4WidePt
      histoInputFileNames.push_back(TString("PP/1000/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5200/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - p#bar{p} - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

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
        histos2D[iFile]->Scale(1.0/dx/dy);
        plotter->plot(histos2D[iFile],names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                      titleDeltaEta,-4.0, 4.0,
                      titleDeltaPhi,1.0,-1.0,
                      titleB2_1_2Bar,-0.01,0.12);
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
        double etaHigh =  7.95;
        double etaStep =  0.2;
        cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          //cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }
      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_1Bar_2_VsDeltaEta",landscapeLinear,titleDeltaEta, -4.0, 4.0,titleB2_1Bar_2,0.0, 0.10,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_1Bar_2_VsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_1Bar_2,0.0, 0.03,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_1Bar_2_VsEtaRange",landscapeLinear,titleDeltaEta, 0.0, 4.0,titleB2_1Bar_2,0.0, 1.1,
                    0.5, 0.2, 0.8, 0.5, 0.05);
      //TLine * line = new TLine(0.0,1.0, 8.0,1.0);
      line->Draw();
      break;

      case 3:
      //Eta4WidePt
      histoInputFileNames.push_back(TString("PP/1000/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5200/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Eta4WidePt/PYTHIA_Pair_Gen_Derived_BalFct_Sum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - p#bar{p} - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

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
        histos2D[iFile]->Scale(1.0/dx/dy);
        plotter->plot(histos2D[iFile],names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                      titleDeltaEta,-4.0, 4.0,
                      titleDeltaPhi,1.0,-1.0,
                      titleB2_1_2Bar,-0.01,0.12);
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
        double etaHigh =  7.95;
        double etaStep =  0.2;
        cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }
      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2Bar_VsDeltaEta",landscapeLinear,titleDeltaEta, -4.0, 4.0,titleB2_1_2Bar,0.0, 0.10,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2Bar_VsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_1_2Bar,0.0, 0.03,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_1_2Bar_VsEtaRange",landscapeLinear,titleDeltaEta, 0.0, 4.0,titleB2_1_2Bar,0.0, 1.1,
                    0.5, 0.2, 0.8, 0.5, 0.05);
      //TLine * line = new TLine(0.0,1.0, 8.0,1.0);
      line->Draw();
      break;
      case 4:
      //Eta4WidePt
      histoInputFileNames.push_back(TString("PP/1000/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5200/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_1Bar_2"));
      titles.push_back(TString(titleB2_1Bar_2+" - p#bar{p} - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1Bar_2_y"));

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
        histos2D[iFile]->Scale(1.0/dx/dy);
        plotter->plot(histos2D[iFile],names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                      titleDeltaEta,-12.0, 12.0,
                      titleDeltaPhi,1.0,-1.0,
                      titleB2_1_2Bar,-0.01,0.12);
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
        double etaHigh = 11.95;
        double etaStep =  0.2;
        cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          //cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }
      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_1Bar_2_VsDeltaEta",landscapeLinear,titleDeltaEta, -12.0, 12.0,titleB2_1Bar_2,0.0, 0.10,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_1Bar_2_VsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_1Bar_2,0.0, 0.03,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_1Bar_2_VsEtaRange",landscapeLinear,titleDeltaEta, 0.0, 12.0,titleB2_1Bar_2,0.0, 1.1,
                    0.5, 0.2, 0.8, 0.5, 0.05);
      //TLine * line = new TLine(0.0,1.0, 8.0,1.0);
      line->Draw();
      break;

      case 5:
      //Eta4WidePt
      histoInputFileNames.push_back(TString("PP/1000/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5200/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Eta6WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_1_2Bar"));
      titles.push_back(TString(titleB2_1_2Bar+" - p#bar{p} - 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_rho2_DetaDphi_shft_B2_1_2Bar_y"));

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
        histos2D[iFile]->Scale(1.0/dx/dy);
        plotter->plot(histos2D[iFile],names[iFile],landscapeLinear,*(graphConfigurations2D[iFile]),
                      titleDeltaEta,-12.0, 12.0,
                      titleDeltaPhi,1.0,-1.0,
                      titleB2_1_2Bar,-0.01,0.12);
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
        double etaHigh =  11.95;
        double etaStep =  0.2;
        cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }
      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2Bar_VsDeltaEta",landscapeLinear,titleDeltaEta, -12.0, 12.0,titleB2_1_2Bar,0.0, 0.10,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2Bar_VsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_1_2Bar,0.0, 0.03,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_1_2Bar_VsEtaRange",landscapeLinear,titleDeltaEta, 0.0, 12.0,titleB2_1_2Bar,0.0, 2.1,
                    0.5, 0.2, 0.8, 0.5, 0.05);
      //TLine * line = new TLine(0.0,1.0, 8.0,1.0);
      line->Draw();
      break;

// vs y
      case 6:
      //Y6
      histoInputFileNames.push_back(TString("PP/1000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_1Bar_2"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5200/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_1Bar_2"));
      titles.push_back(TString("(c) #sqrt{s} = 5.20 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_1Bar_2"));
      titles.push_back(TString("(d) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_1Bar_2"));
      titles.push_back(TString("(e) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

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
        histos2D[iFile]->Scale(1.0/dx/dy);

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
                      titleDeltaY,-20.0, 20.0,
                      titleDeltaPhi,1.0,-1.0,
                      titleB2_1Bar_2,-0.025,0.10);
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
        double etaHigh = 19.95;
        double etaStep =  0.2;
        //cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          //cout << " etaMin: " << eta << " etaMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }
      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_1Bar_2_VsDeltaY",landscapeLinear,titleDeltaY, -20.0, 20.0,titleB2_1Bar_2,-0.05, 0.10,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_1Bar_2_VsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_1Bar_2,-0.1, 0.1,
                    0.6, 0.5, 0.75, 0.8, 0.04);
      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_1Bar_2_VsYRange",landscapeLinear,titleDeltaY, 0.0, 20.0,titleB2_1Bar_2,-2.0, 2.0,
                    0.6, 0.4, 0.8, 0.65, 0.04);
      //TLine * line = new TLine(0.0,1.0, 8.0,1.0);
      line->Draw();
      break;

      case 7:
      //Y6WidePt
      histoInputFileNames.push_back(TString("PP/1000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_1_2Bar"));
      titles.push_back(TString("(f) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_1_2Bar"));
      titles.push_back(TString("(g) #sqrt{s} = 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5200/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_1_2Bar"));
      titles.push_back(TString("(h) #sqrt{s} = 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_1_2Bar"));
      titles.push_back(TString("(i) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_1_2Bar"));
      titles.push_back(TString("(j) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

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
        histos2D[iFile]->Scale(1.0/dx/dy);

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
                      titleDeltaY,-20.0, 20.0,
                      titleDeltaPhi,1.0,-1.0,
                      titleB2_1_2Bar,-0.025,0.10);
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
        double etaHigh =  19.95;
        double etaStep =  0.2;
        cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          cout << " yMin: " << eta << " yMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }
      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2Bar_VsDeltaY",landscapeLinear,titleDeltaY, -20.0, 20.0,titleB2_1_2Bar,-0.05, 0.10,
                    0.6, 0.6, 0.75, 0.85, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_1_2Bar_VsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_1_2Bar,-0.05, 0.05,
                    0.6, 0.4, 0.75, 0.6, 0.04);
      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_1_2Bar_VsYRange",landscapeLinear,titleDeltaY, 0.0, 20.0,titleB2_1_2Bar,-2.0, 4.0,
                    0.5, 0.2, 0.8, 0.5, 0.05);
      //TLine * line = new TLine(0.0,1.0, 8.0,1.0);
      line->Draw();
      break;

      case 8:
      // B2_12Sum
      histoInputFileNames.push_back(TString("PP/1000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_B2_12Sum"));
      titles.push_back(TString("(k) #sqrt{s} = 1.0 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/2700/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2_12Sum"));
      titles.push_back(TString("(l) #sqrt{s} = 2.7 TeV"));
      histoNames2D.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/5200/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2_12Sum"));
      titles.push_back(TString("(m) #sqrt{s} = 5.2 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2_12Sum"));
      titles.push_back(TString("(n) #sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/Y10WidePt/PYTHIA_Pair_Gen_Derived_BalFct.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2_12Sum"));
      titles.push_back(TString("(n) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("Pair_Gen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

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
        histos2D[iFile]->Scale(1.0/dx/dy);
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
                      titleDeltaY,-20.0, 20.0,
                      titleDeltaPhi,1.0,-1.0,
                      titleB2_12Sum,-0.01,0.10);
        //TString("B_{2}"),0.0,0.002);
        plotter->createLabel(titles[iFile], -1.15, 0.8, 0.0, 1, 0.07, true);

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
        double etaHigh =  19.95;
        double etaStep =  0.2;
        cout << "File: " << names[iFile] << endl;

        sum = 0;
        esum = 0;

        for (double eta=etaLow; eta<etaHigh; eta+=etaStep)
          {
          int xLow  = xAxis->FindBin(-eta);
          int xHigh = xAxis->FindBin( eta);
          double factor = 1-eta/xHigh;
          //if (iFile<2)
          sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //else if (iFile==3 || iFile==5)
          //        {
          //        //sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
          //        int nx = h2->GetNbinsX();
          //        int ny = h2->GetNbinsY();
          //
          //        }

          cout << " yMin: " << eta << " yMax: " << -eta << " integral: " << sum << " error: " << esum << endl;
          x[nPoints]  = eta;
          ex[nPoints] = 0.001;
          y[nPoints]  = sum;
          ey[nPoints] = esum;
          nPoints++;
          }
        TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
        graphs.push_back(g);
        }
      plotter->plot(histos1Dx,graphConfigurations1D,titles,"PythiaPPvsS_B2_12Sum_VsDeltaY",landscapeLinear,titleDeltaY, -20.0, 20.0,titleB2_12Sum,-0.01, 0.10,
                    0.6, 0.6, 0.75, 0.89, 0.04);
      plotter->plot(histos1Dy,graphConfigurations1D,titles,"PythiaPPvsS_B2_12Sum_VsDeltaPhi",landscapeLinear,titleDeltaPhi, 4.0, -4.0,titleB2_12Sum,0.01, 0.025,
                    0.6, 0.5, 0.75, 0.85, 0.04);
      plotter->plot(graphs,graphConfigurations1D,titles,"PythiaPPvsS_I2_12Sum_VsYRange",landscapeLinear,titleDeltaY, 0.0, 20.0,titleI2_12Sum,-0.1, 1.2,
                    0.5, 0.3, 0.8, 0.65, 0.05);
      //TLine * line = new TLine(0.0,1.0, 8.0,1.0);
      line->Draw();
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
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
}
