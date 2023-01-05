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

void calculateRmsWidth(TH2 * h, double lowEdge, double highEdge, double & mean, double & meanError, double & rmsWidth, double & rmsWidthError)
{
  int nBins   = h->GetXaxis()->GetNbins();
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

  int ny = h->GetNbinsY();

  // two passes: pass 1 to get the mean, pass 2 to get the rms error,
  for (int j=1; j<=ny; j++)
    {
    for (int k=lowBin; k<highBin; k++ )
      {
      x  = h->GetXaxis()->GetBinCenter(k);
      v  = h->GetBinContent(k,j);
      sum   += v;
      xSum  += x*v;
      x2Sum += x*x*v;
      }
    }

  if (sum>0)
    {
    m1 = xSum/sum;
    m2 = x2Sum/sum - m1*m1;
    if (m2>0.0) rmsWidth = sqrt(m2);

    for (int j=1; j<=ny; j++)
      {
      for (int k=lowBin; k<highBin; k++ )
        {
        x  = h->GetXaxis()->GetBinCenter(k);
        v  = h->GetBinContent(k);
        ev = h->GetBinError(k,j);
        xSum  += x*x*ev*ev;
        arg = x*x - 2*m1*x;
        x2Sum += arg*arg * ev*ev;
        }
      }
    meanError = sqrt(xSum)/sum;
    rmsWidthError = sqrt(x2Sum)/sum;
    }
}

void calculateWidths(TH2 * h, double lowEdge, double highEdge, vector<double> & rmsWidths, vector<double> & rmsWidthErrors)
{
  double mean          = 0;
  double meanError     = 0;
  double rmsWidth      = 0;
  double rmsWidthError = 0;
  calculateRmsWidth(h,lowEdge,highEdge,mean,meanError,rmsWidth,rmsWidthError);
  rmsWidths.push_back(rmsWidth);
  rmsWidthErrors.push_back(rmsWidthError);
}

void calculateIntegrals(TH2* h2,
                        vector<TGraph*> & integralGraphs,
                        double etaLow,
                        double etaHigh,
                        double etaStep,
                        int size=500)
{
  cout << "calculateIntegrals(...) Begins" << endl;
  double * x  = new double[size];
  double * ex = new double[size];
  double * y  = new double[size];
  double * ey = new double[size];

  const TAxis * xAxis  = h2->GetXaxis();
  const TAxis * yAxis  = h2->GetYaxis();
  int xLow  = xAxis->GetXmin();
  int xHigh = xAxis->GetXmax();
  int yLow  = yAxis->FindBin(-0.5*TMath::Pi());
  int yHigh = yAxis->FindBin( 1.5*TMath::Pi());
  int nPoints = 0;
  double sum  = 0;
  double esum = 0;
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
  integralGraphs.push_back(g);
  cout << "calculateIntegrals(...) Ends" << endl;
}


TGraph *  sumGraphs(TGraph * g1, TGraph * g2)
{
  cout << "sumGraphs(...) Begins" << endl;
  int  n1 = g1->GetN();
  int  n2 = g2->GetN();
  if (n1 != n2)
    {
    cout << "sumGraphs(TGraph * g1, TGraph * g2, TGraph * gSum) args do not have same number of points" << endl;
    return nullptr;
    }
  double * x  = new double[n1];
  double * ex = new double[n1];
  double * y  = new double[n1];
  double * ey = new double[n1];
  for (int index=0; index<n1; index++)
    {
    double x1  = g1->GetPointX(index);
    double ex1 = g1->GetErrorX(index);
    double y1  = g1->GetPointY(index);
    double ey1 = g1->GetErrorY(index);
    double x2  = g2->GetPointX(index);
    double ex2 = g2->GetErrorX(index);
    double y2  = g2->GetPointY(index);
    double ey2 = g2->GetErrorY(index);
    if (x1!=x2)
      {
      cout << "sumGraphs(TGraph * g1, TGraph * g2, TGraph * gSum) x1 ne x2" << endl;
      delete[] x;
      delete[] ex;
      delete[] y;
      delete[] ey;
      return nullptr;
      }
    x[index]  = x1;
    ex[index] = ex1;
    y[index]  = y1 + y2;
    ey[index] = sqrt(ey1*ey1 + ey2*ey2);
    }
  TGraph * g = new TGraphErrors(n1, x,y,ex,ey);
  cout << "sumGraphs(...) Ends" << endl;
  return g;
}



TGraph * makeGraph(vector<double> vx, vector<double> vex, vector<double> vy,  vector<double> vey)
{
  int n = vx.size();
  if ( vex.size()!=n || vy.size()!=n || vey.size()!=n )
    {
    cout << "<E> makeGraph(vx,vex,vy,vey) arguments provided have incompatible sizes)" << endl;
    return 0;
    }
  double * x  = new double[n];
  double * ex = new double[n];
  double * y  = new double[n];
  double * ey = new double[n];
  for (int k=0;k<n;k++)
    {
    x[k]  = vx[k];
    ex[k] = vex[k];
    y[k]  = vy[k];
    ey[k] = vey[k];
    }
  return new TGraphErrors(n,x,y,ex,ey);
}

TH1 * makeHistoWithNames(const TString & histoName, vector<TString> & entryNames, vector<double> & vy,  vector<double> & vey)
{
  cout << "<I> makeHistoWithNames(names,vy,vey) Starting the creation of histo called " << histoName << endl;

  int n = vy.size();
  int n1 = vey.size();
  cout << "<I> makeHistoWithNames(names,vy,vey) n:" << n << "  n1:" << n1 << endl;
  if ( n!=n1 || n<1 )
    {
    cout << "<E> makeHistoWithNames(names,vy,vey) arguments provided have incompatible sizes)" << endl;
    return nullptr;
    }
  TH1 * h = new TH1D(histoName,histoName,n,0.0, double(n));
  cout << "<I> makeHistoWithNames(names,vy,vey) Histogram created?" << endl;
  if (!h)
    {
    cout << "<E> makeHistoWithNames(names,vy,vey) Histogram pointer is null" << endl;
    return nullptr;
    }
  else
    {
    cout << "<E> makeHistoWithNames(names,vy,vey) Histogram pointer is NOT null" << endl;
    }

  for (int k=0;k<n;k++)
    {
    h->SetBinContent(k+1,vy[k]);
    h->SetBinError(k+1,vey[k]);
    h->GetXaxis()->SetBinLabel(k+1,entryNames[k]);
    }
  return h;
}

int plotSet(Plotter * plotter,
            const TString & inputPath,
            const TString & outputPath,
            vector<TString> & histoInputFileNames,
            vector<TFile*>  & histoInputFiles,
            vector<TString> & histoNames2D,
            vector<TString> & histoNames1DVsDeltaY,
            vector<TString> & histoNames1DVsDeltaPhi,
            vector<TString> & names,
            vector<TString> & titles,
            vector<int>     & histoTypes,
            const TString   & triggerName,
            double minB2D,
            double maxB2D,
            double minBVsDeltaRapidity,
            double maxBVsDeltaRapidity,
            double minBVsDeltaPhi,
            double maxBVsDeltaPhi,
            double minIVsDeltaRapidity,
            double maxIVsDeltaRapidity,
            bool rebin=false,
            int  rapidityType=1,
            int  plot2D=1,
            int  plot1D=1,
            int  plot1DType=0,
            int  plot1DHighRes=0,
            int  plotWidths=1,
            int  plotIntegralVsLogY=1)
{
  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogX(CanvasConfiguration::Landscape,CanvasConfiguration::LogX);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  //vector<GraphConfiguration*>  integralGraphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  GraphConfiguration *  graphConfigurations2D = new GraphConfiguration(2,0);
  GraphConfiguration *  widthGraphConfiguration = new GraphConfiguration(1,0);

  for (int k=0;k<10;k++)
    {
    graphConfigurations1D[k]->setParameter("xTitleSize",   0.08);
    graphConfigurations1D[k]->setParameter("xTitleOffset", 0.8);
    graphConfigurations1D[k]->setParameter("yTitleSize",   0.08);
    graphConfigurations1D[k]->setParameter("yTitleOffset", 0.8);
    graphConfigurations1D[k]->setParameter("xLabelSize",   0.07);
    graphConfigurations1D[k]->setParameter("yLabelSize",   0.07);
    graphConfigurations1D[k]->setParameter("lineColor",    41+k);
    graphConfigurations1D[k]->setParameter("markerColor",  41+k);
    graphConfigurations1D[k]->setParameter("markerStyle",  kFullSquare);
    graphConfigurations1D[k]->setParameter("markerSize",   0.9);
    }


  widthGraphConfiguration->setParameter("markerColor",  kBlack);
  widthGraphConfiguration->setParameter("markerStyle",  kFullSquare);
  widthGraphConfiguration->setParameter("markerSize",   1.2);
  // widthGraphConfiguration->setParameter("xLabelSize",   0.12);
  widthGraphConfiguration->setParameter("xLabelSize",   0.06);
  widthGraphConfiguration->setParameter("yTitleSize",   0.08);
  widthGraphConfiguration->setParameter("yTitleOffset", 0.7);
  widthGraphConfiguration->setParameter("yLabelSize",   0.07);


  float size = 0.07;
  graphConfigurations2D->setParameter("xTitleOffset",  1.2);
  graphConfigurations2D->setParameter("xTitleSize",   size);
  graphConfigurations2D->setParameter("xLabelOffset", 0.01);
  graphConfigurations2D->setParameter("xLabelSize",   size);

  graphConfigurations2D->setParameter("yTitleOffset", 1.2);
  graphConfigurations2D->setParameter("yTitleSize",   size);
  graphConfigurations2D->setParameter("yLabelOffset", 0.01);
  graphConfigurations2D->setParameter("yLabelSize",   size);

  graphConfigurations2D->setParameter("zTitleOffset", 1.2);
  graphConfigurations2D->setParameter("zTitleSize",   size);
  graphConfigurations2D->setParameter("zLabelOffset", 0.01);
  graphConfigurations2D->setParameter("zLabelSize",   size);


  vector<TH2*>    histos2DB12Bar;
  vector<TH2*>    histos2DB1Bar2;
  vector<TH2*>    histos2DB12s;
  vector<TH1*>    histosDeltaYProjB12Bar;
  vector<TH1*>    histosDeltaYProjB1Bar2;
  vector<TH1*>    histosDeltaYProjB12s;
  vector<TH1*>    histosDeltaPhiProjB12Bar;
  vector<TH1*>    histosDeltaPhiProjB1Bar2;
  vector<TH1*>    histosDeltaPhiProjB12s;
  vector<TGraph*> integralGraphsB12Bar;
  vector<TGraph*> integralGraphsB1Bar2;
  vector<TGraph*> integralGraphsB12s;
  vector<TGraph*> integralGraphsB12sSums;
  vector<TString> graphTitlesB12Bar;
  vector<TString> graphTitlesB1Bar2;
  vector<TString> graphTitlesB12s;
  vector<TString> widthTitles;

  vector<double>  rmsWidths;
  vector<double>  rmsWidthErrors;
  TGraph* widthsGraph;
  TString widthHistoName;
  TString bDeltaYProjName;
  TString bDeltaPhiProjName;
  TString integralName;
  TString integralSumName;
  TString widthName;
  TString sName;
  TString titleDeltaY;
  TString titleDeltaPhi = "#Delta#varphi";

  TString nameB12Bar   = "B12Bar";
  TString nameB1Bar2   = "B1Bar2";
  TString nameB12s     = "B12s";
  TString nameI12Bar   = "I12Bar";
  TString nameI1Bar2   = "I1Bar2";
  TString nameI12s     = "I12s";


  TString titleB12Bar   = "B^{+-}";
  TString titleB1Bar2   = "B^{-+}";
  TString titleB12s     = "B^{s}";
  TString titleI12Bar   = "I^{+-}";
  TString titleI1Bar2   = "I^{-+}";
  TString titleI12s     = "I^{s}";
  TString titleB        = "";
  TString titleI        = "";


  double minRapidity     = -20.0;
  double maxRapidity     =  20.0;
  double minRapidityInt  =  0.0;
  double maxRapidityInt  =  20.0;
  double etaLow   =  0.1;
  double etaHigh  =  maxRapidityInt-0.05;
  double etaStep  =  0.2;

  if (plot1DHighRes>0)
    {
    cout << "" << endl;
    minRapidity     = -3.0;
    maxRapidity     =  3.0;
    minRapidityInt  =  0.1;
    maxRapidityInt  =  8.0;
    etaLow   =  0.1;
    etaHigh  =  maxRapidityInt-0.05;
    etaStep  =  0.2;
    }

  cout << "plot1DHighRes....:" << plot1DHighRes << endl;
  cout << "minRapidity......:" << minRapidity << endl;
  cout << "maxRapidity......:" << maxRapidity << endl;



  switch (rapidityType)
    {
      case 0: titleDeltaY  = "#Delta#eta"; break;
      case 1: titleDeltaY  = "#Delta y"; break;
    }

  for (unsigned int iFile=0; iFile<histoInputFileNames.size(); iFile++)
    {

    cout << " iFile  ==  " << iFile << endl;

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
      cout << " In file " <<  histoInputFileNames[iFile] << endl;
      cout << " Could not load histogram named " << histoNames2D[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
    if (rebin)
      {
      h2->Rebin2D(4,2);
      h2->Scale(0.25);
      }
    double dx = h2->GetXaxis()->GetBinWidth(1);
    double dy = h2->GetYaxis()->GetBinWidth(1);
    h2->Scale(1.0/dy);

    cout << " iFile  ==  " << iFile << "   1D DeltaY" << endl;


    TH1 * h1x = plotter->getHistogramCollection().loadH1(f,histoNames1DVsDeltaY[iFile]);
    if (!h1x)
      {
      cout << " In file " <<  histoInputFileNames[iFile] << endl;
      cout << " Could not load histogram named " << histoNames1DVsDeltaY[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }

    cout << " iFile  ==  " << iFile << "   1D DeltaPhi" << endl;

    TH1 * h1y = plotter->getHistogramCollection().loadH1(f,histoNames1DVsDeltaPhi[iFile]);
    if (!h1y)
      {
      cout << " In file " <<  histoInputFileNames[iFile] << endl;
      cout << " Could not load histogram named " << histoNames1DVsDeltaPhi[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }

    cout << " iFile  ==  " << iFile << "   1D DeltaPhi" << endl;


    switch (histoTypes[iFile])
      {
        case 0:
        bDeltaYProjName   = "B2_1Bar_2_VsDeltaRapidity";
        bDeltaPhiProjName = "B2_1Bar_2_VsDeltaPhi";
        integralName      = "I2_1Bar_2";
        graphTitlesB1Bar2.push_back(titles[iFile]);
        if (plot2D)
          {
          plotter->plot(h2,names[iFile],landscapeLinear,*graphConfigurations2D,
                        titleDeltaY,minRapidity,maxRapidity,
                        titleDeltaPhi,1.0,-1.0,
                        titleB1Bar2,minB2D,maxB2D);
          plotter->createLabel(titles[iFile], -1.1, 0.8, 0.0, 1, 0.07, true);
          }

        histosDeltaYProjB1Bar2.push_back(h1x);
        histosDeltaPhiProjB1Bar2.push_back(h1y);
        calculateIntegrals(h2,integralGraphsB1Bar2,etaLow,etaHigh,etaStep);
        break;
        case 1:
        bDeltaYProjName   = "Pythia_PPvsS_B2_1_2Bar_VsDeltaRapidity";
        bDeltaPhiProjName = "Pythia_PPvsS_B2_1_2Bar_VsDeltaPhi";
        integralName      = "Pythia_PPvsS_I2_1_2Bar_VsDeltaRapidity";
        graphTitlesB12Bar.push_back(titles[iFile]);
        if (plot2D)
          {
          plotter->plot(h2,names[iFile],landscapeLinear,*graphConfigurations2D,
                        titleDeltaY,minRapidity,maxRapidity,
                        titleDeltaPhi,1.0,-1.0,
                        titleB12Bar,minB2D,maxB2D);
          plotter->createLabel(titles[iFile], -1.1, 0.8, 0.0, 1, 0.07, true);
          }
        histosDeltaYProjB12Bar.push_back(h1x);
        histosDeltaPhiProjB12Bar.push_back(h1y);
        calculateIntegrals(h2,integralGraphsB12Bar,etaLow,etaHigh,etaStep);
        break;
        case 2:
        bDeltaYProjName   = "Pythia_PPvsS_B2_12s_VsDeltaRapidity";
        bDeltaPhiProjName = "Pythia_PPvsS_B2_12s_VsDeltaPhi";
        integralName      = "Pythia_PPvsS_I2_12s_VsDeltaRapidity";
        graphTitlesB12s.push_back(titles[iFile]);
        widthTitles.push_back(titles[iFile]);
        if (plot2D)
          {
          plotter->plot(h2,names[iFile],landscapeLinear,*graphConfigurations2D,
                        titleDeltaY,minRapidity,maxRapidity,
                        titleDeltaPhi,1.0,-1.0,
                        titleB12s,minB2D,maxB2D);
          plotter->createLabel(titles[iFile], -1.1, 0.8, 0.0, 1, 0.07, true);
          }
        histosDeltaYProjB12s.push_back(h1x);
        histosDeltaPhiProjB12s.push_back(h1y);
        calculateIntegrals(h2,integralGraphsB12s,etaLow,etaHigh,etaStep);

        if (plotWidths==1)
          {
          TCanvas * c = new TCanvas();
          TH1 * projX = h2->ProjectionX();
          TF1 * f1 = new TF1("f1","gaus",-6.0,6.0);
          // set initial parameters (not really needed for gaus)
          f1->SetParameters(projX->GetMaximum(), 0.0, 1.0 );
          projX->Fit("f1");

          TF1 * fitResult = projX->GetFunction("f1");
          double p1 = fitResult->GetParameter(0);
          double p2 = fitResult->GetParameter(1);
          double p3 = fitResult->GetParameter(2);
          double e1 = fitResult->GetParError(0);
          double e2 = fitResult->GetParError(1);
          double e3 = fitResult->GetParError(2);
          cout << " widths ---------------------------------------------" << endl;
          cout << " p1: " << p1 << " +- " << e1 << endl;
          cout << " p2: " << p2 << " +- " << e2 << endl;
          cout << " p3: " << p3 << " +- " << e3 << endl;
          //calculateWidths(h2,minRapidity, maxRapidity, rmsWidths, rmsWidthErrors);
          rmsWidths.push_back(p3); rmsWidthErrors.push_back(e3);
          for (int k=0;k<rmsWidths.size();k++)
            {
            cout << "   k : " << k << "  " << rmsWidths[k] << " +- " << rmsWidthErrors[k] << endl;
            }

          cout << "Done with width histogram" << endl;
          }
        break;
      }

    cout << "2D plots are completed" << endl;


    }

  if (plotWidths)
    {
    widthHistoName = "PythiaWidthsVsAcceptance";
    TH1 * widthsHisto = makeHistoWithNames(widthHistoName, widthTitles, rmsWidths, rmsWidthErrors);
    plotter->plot(widthsHisto,widthHistoName,landscapeLinear,
                  *widthGraphConfiguration,
                  "",4.0,-4.0,"RMS",0.0,2.0,
                  "",0.6, 0.3, 0.8, 0.4, 0.05);
    }

  TLine * line = new TLine(0.0,1.0,  20.0,1.0);

  if (histosDeltaYProjB1Bar2.size()>0 && plot1D)
    {
    bDeltaYProjName     =  triggerName;
    bDeltaYProjName     += nameB1Bar2;
    bDeltaYProjName     += "_VsDeltaRapidity";
    if (plot1DHighRes) bDeltaYProjName += "HighRes";
    bDeltaPhiProjName   =  triggerName;
    bDeltaPhiProjName   += nameB1Bar2;
    bDeltaPhiProjName   += "_VsDeltaPhi";
    integralName        =  triggerName;
    integralName        += nameI1Bar2;
    integralName        += "_VsDeltaRapidity";
    titleB              = titleB1Bar2;
    titleI              = titleI1Bar2;

    if (plot1DType==0)
      {
      plotter->plot(histosDeltaYProjB1Bar2,graphConfigurations1D,graphTitlesB1Bar2,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, -0.05);
      if (plot1DHighRes==0) plotter->createLabel("(a)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->createLabel("(a)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(histosDeltaPhiProjB1Bar2,  graphConfigurations1D,graphTitlesB1Bar2,bDeltaPhiProjName,landscapeLinear,titleDeltaPhi, 1.0,         -1.0,       titleB,minBVsDeltaPhi,     maxBVsDeltaPhi,      0.5, 0.55, 0.76, 0.85, -0.05);
      plotter->createLabel("(a)", -1.0, 0.5, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB1Bar2,      graphConfigurations1D,graphTitlesB1Bar2,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(a)", 0.12, 2.8, 0.0, 1, 0.07, true);
      line->Draw();
      }
    if (plot1DType==1)
      {
      plotter->plot(histosDeltaYProjB1Bar2,graphConfigurations1D,graphTitlesB1Bar2,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, -0.05);
      if (plot1DHighRes==0) plotter->createLabel("(a)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->createLabel("(a)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(histosDeltaPhiProjB1Bar2,  graphConfigurations1D,graphTitlesB1Bar2,bDeltaPhiProjName,landscapeLinear,titleDeltaPhi, 1.0, -1.0,titleB,minBVsDeltaPhi,     maxBVsDeltaPhi,      0.5, 0.55, 0.76, 0.85, -0.05);
      plotter->createLabel("(a)", -1.0, 0.21, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB1Bar2,      graphConfigurations1D,graphTitlesB1Bar2,integralName,     landscapeLogX,  titleDeltaY,   0.1, maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(a)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();
      }
    if (plot1DType==2)
      {
      plotter->plot(histosDeltaYProjB1Bar2,graphConfigurations1D,graphTitlesB1Bar2,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, -0.05);
      if (plot1DHighRes==0) plotter->createLabel("(a)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->createLabel("(a)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB1Bar2,      graphConfigurations1D,graphTitlesB1Bar2,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(a)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();
      }
    if (plot1DType==3)
      {
      plotter->plot(histosDeltaYProjB1Bar2,graphConfigurations1D,graphTitlesB1Bar2,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, -0.05);
      if (plot1DHighRes==0) plotter->createLabel("(a)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->createLabel("(a)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB1Bar2,      graphConfigurations1D,graphTitlesB1Bar2,integralName,     landscapeLogX,  titleDeltaY,   0.1, maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(a)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();
      }
    }

  if (histosDeltaYProjB12Bar.size()>0 && plot1D)
    {
    bDeltaYProjName     =  triggerName;
    bDeltaYProjName     += nameB12Bar;
    bDeltaYProjName     += "_VsDeltaRapidity";
    if (plot1DHighRes) bDeltaYProjName += "HighRes";
    bDeltaPhiProjName   =  triggerName;
    bDeltaPhiProjName   += nameB12Bar;
    bDeltaPhiProjName   += "_VsDeltaPhi";
    integralName        =  triggerName;
    integralName        += nameI12Bar;
    integralName        += "_VsDeltaRapidity";
    titleB              = titleB12Bar;
    titleI              = titleI12Bar;

    if (plot1DType==0)
      {
      plotter->plot(histosDeltaYProjB12Bar,graphConfigurations1D,graphTitlesB12Bar,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, -0.05);
      if (plot1DHighRes==0) plotter->createLabel("(b)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->createLabel("(b)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(histosDeltaPhiProjB12Bar,  graphConfigurations1D,graphTitlesB12Bar,bDeltaPhiProjName,landscapeLinear,titleDeltaPhi, 1.0,         -1.0, titleB,minBVsDeltaPhi,     maxBVsDeltaPhi,      0.5, 0.55, 0.76, 0.85, -0.05);
      plotter->createLabel("(b)", -1.0, 0.5, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB12Bar,      graphConfigurations1D,graphTitlesB12Bar,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(b)", 0.12, 2.8, 0.0, 1, 0.07, true);
      line->Draw();
      }
    if (plot1DType==1)
      {
      plotter->plot(histosDeltaYProjB12Bar,graphConfigurations1D,graphTitlesB12Bar,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, -0.05);
      if (plot1DHighRes==0) plotter->createLabel("(b)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->createLabel("(b)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(histosDeltaPhiProjB12Bar,  graphConfigurations1D,graphTitlesB12Bar,bDeltaPhiProjName,landscapeLinear,titleDeltaPhi, 1.0,         -1.0,       titleB,minBVsDeltaPhi,     maxBVsDeltaPhi,      0.5, 0.55, 0.76, 0.85, -0.05);
      plotter->createLabel("(b)", -1.0, 0.21, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB12Bar,      graphConfigurations1D,graphTitlesB12Bar,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(b)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();
      }
    if (plot1DType==2)
      {
      plotter->plot(histosDeltaYProjB12Bar,graphConfigurations1D,graphTitlesB12Bar,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, -0.05);
      if (plot1DHighRes==0) plotter->createLabel("(b)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->createLabel("(b)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB12Bar,      graphConfigurations1D,graphTitlesB12Bar,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(b)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();
      }
    if (plot1DType==3)
      {
      plotter->plot(histosDeltaYProjB12Bar,graphConfigurations1D,graphTitlesB12Bar,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, -0.05);
      if (plot1DHighRes==0) plotter->createLabel("(b)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->createLabel("(b)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB12Bar,      graphConfigurations1D,graphTitlesB12Bar,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(b)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();
      }
    }


  if (histosDeltaYProjB12s.size()>0 && plot1D)
    {
    bDeltaYProjName     =  triggerName;
    bDeltaYProjName     += nameB12s;
    bDeltaYProjName     += "_VsDeltaRapidity";
    if (plot1DHighRes) bDeltaYProjName += "HighRes";
    bDeltaPhiProjName   =  triggerName;
    bDeltaPhiProjName   += nameB12s;
    bDeltaPhiProjName   += "_VsDeltaPhi";
    integralName        =  triggerName;
    integralName        += nameI12s;
    integralName        += "_VsDeltaRapidity";
    integralSumName     =  triggerName;
    integralSumName     +=  nameI12s;
    integralSumName     +=  "_SUM_VsDeltaRapidity";
    titleB              =  titleB12s;
    titleI              =  titleI12s;

    if (plot1DType==0)
      {
      if (plot1DHighRes==0) plotter->plot(histosDeltaYProjB12s,graphConfigurations1D,graphTitlesB12s,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.55, 0.55, 0.75, 0.9,  0.05);
      if (plot1DHighRes==0) plotter->createLabel("(c)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->plot(histosDeltaYProjB12s,graphConfigurations1D,graphTitlesB12s,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.56, 0.8, 0.91,  0.05);
      if (plot1DHighRes==1) plotter->createLabel("(c)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(histosDeltaPhiProjB12s,  graphConfigurations1D,graphTitlesB12s,bDeltaPhiProjName,landscapeLinear,titleDeltaPhi, 1.0,         -1.0,       titleB,minBVsDeltaPhi,     maxBVsDeltaPhi,      0.5, 0.55, 0.76, 0.85, 0.05);
      plotter->createLabel("(c)", -1.0, 0.5, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB12s,      graphConfigurations1D,graphTitlesB12s,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, 0.05);
      plotter->createLabel("(c)", 0.12, 2.8, 0.0, 1, 0.07, true);
      line->Draw();
      }
    if (plot1DType==1)
      {
      if (plot1DHighRes==0) plotter->plot(histosDeltaYProjB12s,graphConfigurations1D,graphTitlesB12s,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.55, 0.55, 0.8, 0.9, 0.05);
      if (plot1DHighRes==0) plotter->createLabel("(c)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->plot(histosDeltaYProjB12s,graphConfigurations1D,graphTitlesB12s,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.63, 0.55, 0.8, 0.9, 0.04);
      if (plot1DHighRes==1) plotter->createLabel("(c)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(histosDeltaPhiProjB12s,  graphConfigurations1D,graphTitlesB12s,bDeltaPhiProjName,landscapeLinear,titleDeltaPhi, 1.0,         -1.0,       titleB,minBVsDeltaPhi,     maxBVsDeltaPhi,      0.5, 0.55, 0.76, 0.85, 0.05);
      plotter->createLabel("(c)", -1.0, 0.21, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB12s,      graphConfigurations1D,graphTitlesB12s,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.20, 0.5, 0.3, 0.85, 0.05);
      plotter->createLabel("(c)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();

      TGraph * g;
      TGraph * gs;

      for (int iGraph=0; iGraph<integralGraphsB12s.size(); iGraph++)
        {
        g = integralGraphsB12s[iGraph];
        if (iGraph==0)
          {
          gs = (TGraph * )g->Clone();
          }
        else
          {
          gs = sumGraphs(gs, g);
          }
        integralGraphsB12sSums.push_back(gs);
        }
      plotter->plot(integralGraphsB12sSums,  graphConfigurations1D,graphTitlesB12s,integralSumName, landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.20, 0.5, 0.3, 0.85, 0.05);
      //plotter->createLabel("(c)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();

      }
    if (plot1DType==2)
      {
      if (plot1DHighRes==0) plotter->plot(histosDeltaYProjB12s,graphConfigurations1D,graphTitlesB12Bar,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, 0.05);
      if (plot1DHighRes==0) plotter->createLabel("(c)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->plot(histosDeltaYProjB12s,graphConfigurations1D,graphTitlesB12Bar,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, 0.05);
      if (plot1DHighRes==1) plotter->createLabel("(c)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB12Bar,      graphConfigurations1D,graphTitlesB12Bar,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(c)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();
      }
    if (plot1DType==3)
      {
      if (plot1DHighRes==0) plotter->plot(histosDeltaYProjB12s,graphConfigurations1D,graphTitlesB12Bar,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, 0.05);
      if (plot1DHighRes==0) plotter->createLabel("(c)", -18.0, 0.4, 0.0, 1, 0.07, true);
      if (plot1DHighRes==1) plotter->plot(histosDeltaYProjB12s,graphConfigurations1D,graphTitlesB12Bar,bDeltaYProjName,  landscapeLinear,titleDeltaY,   minRapidity, maxRapidity,titleB,minBVsDeltaRapidity,maxBVsDeltaRapidity, 0.65, 0.55, 0.8, 0.9, 0.05);
      if (plot1DHighRes==1) plotter->createLabel("(c)", -2.5, 0.4, 0.0, 1, 0.07, true);
      plotter->plot(integralGraphsB12Bar,      graphConfigurations1D,graphTitlesB12Bar,integralName,     landscapeLogX,  titleDeltaY,   0.1,      maxRapidityInt,titleI,minIVsDeltaRapidity,maxIVsDeltaRapidity, 0.55, 0.2, 0.85, 0.50, -0.05);
      plotter->createLabel("(c)", 0.12, 1.05, 0.0, 1, 0.07, true);
      line->Draw();
      }
    }

  return 0;
}

int PlotBFPiKP(int option = 1)
{
  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 1;
  bool useColor = true;

  bool rebin=false;
  int  rapidityType=1;
  int  plot2D=0;
  int  plot1D=1;

//  bool rebin=true;
//  int  rapidityType=1;
//  int  plot2D=1;
//  int  plot1D=0;
  int  plot1DHighRes      = 0;
  int  plotWidths         = 0;
  int  plotIntegralVsLogY = 1;

  gStyle->SetPalette(kLightTemperature);

  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  MessageLogger::LogLevel infoLevel = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Info;
  MessageLogger::LogLevel selectLevel = infoLevel;


  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/";
  TString outputPathBase = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/";
  TString outputPath;

  vector<TString> histoInputFileNames;
  vector<TFile*>  histoInputFiles;
  vector<TString> histoNames2D;
  vector<TString> histoNames1DVsDeltaY;
  vector<TString> histoNames1DVsDeltaPhi;
  vector<TString> names;
  vector<TString> titles;
  vector<int>     histoTypes; // 0, 1, 2
  Configuration plotConfig;

  Plotter * plotter = new Plotter("Plotter",plotConfig);
  plotter->setDefaultOptions(useColor);
  switch (option)
    {
      case 0:
      {
      TString inFileName = "PP/13000/PiKp/y10/PairGenBalFctSum0TO9.root";
      TString outFileNameBase = "PYTHIA_pp_13.0TeV_B2Based_";
      TString titleBase       = "#sqrt{s}=13.0 TeV; ";
      TString histoNameBase   = "PairGen_All_";
      vector<TString> speciesNames;
      speciesNames.push_back(TString("PiP"));
      speciesNames.push_back(TString("KP"));
      speciesNames.push_back(TString("PP"));
      vector<TString> speciesTitles;
      speciesTitles.push_back(TString("#pi"));
      speciesTitles.push_back(TString("K"));
      speciesTitles.push_back(TString("P"));
      vector<TString> corrNames;
      corrNames.push_back(TString("_B2_DyDphi_shft_B2_1Bar_2"));
      corrNames.push_back(TString("_B2_DyDphi_shft_B2_1_2Bar"));
      corrNames.push_back(TString("_B2_DyDphi_shft_B2_12Sum"));

      for (int iSpecies1=0; iSpecies1<3; iSpecies1++)
        {
        TString outFileNameBase2 = outFileNameBase;
        outFileNameBase2 += (speciesNames[iSpecies1]);
        outFileNameBase2 += "_";
        histoInputFileNames.clear();
        names.clear();
        titles.clear();
        histoTypes.clear();
        histoNames2D.clear();
        histoNames1DVsDeltaY.clear();
        histoNames1DVsDeltaPhi.clear();

        for (int iSpecies2=0; iSpecies2<3; iSpecies2++)
          {

          for (int iType=0; iType<3; iType++)
            {
            histoInputFileNames.push_back(inFileName);
            names.push_back(outFileNameBase2+(speciesNames[iSpecies2]+"_")+(speciesNames[iSpecies1])+(corrNames[iType]));
            titles.push_back(titleBase+(speciesTitles[iSpecies2])+(speciesTitles[iSpecies1]));
            histoTypes.push_back(iType);
            histoNames2D.push_back(          histoNameBase+(speciesNames[iSpecies2]+"_")+(speciesNames[iSpecies1])+(corrNames[iType]));
            histoNames1DVsDeltaY.push_back(  histoNameBase+(speciesNames[iSpecies2]+"_")+(speciesNames[iSpecies1])+(corrNames[iType])+"_x");
            histoNames1DVsDeltaPhi.push_back(histoNameBase+(speciesNames[iSpecies2]+"_")+(speciesNames[iSpecies1])+(corrNames[iType])+"_y");
            }
          }
        outputPath = outputPathBase;
        outputPath += "/PiKP/Y10/B2Derived/";
        plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1DVsDeltaY,histoNames1DVsDeltaPhi,names,titles,histoTypes,
                (speciesNames[iSpecies1]+"_"),
                -0.05, 0.1,
                -0.149, 0.49,
                -0.249, 0.59,
                -1.19, 3.19,
                rebin,rapidityType,plot2D,plot1D,option,plot1DHighRes,plotWidths,plotIntegralVsLogY);
        }
        break;
      }

      // a2
      case 1:
      {
      TString inFileName = "PP/13000/PiKp/y10/PairGenBalFctSum0TO9.root";
      TString outFileNameBase = "PYTHIA_pp_13.0TeV_B2Based_";
      TString titleBase       = "";
      TString histoNameBase   = "PairGen_All_";
      vector<TString> speciesNames;
      speciesNames.push_back(TString("PiP"));
      speciesNames.push_back(TString("KP"));
      speciesNames.push_back(TString("PP"));
      vector<TString> speciesTitles;
      speciesTitles.push_back(TString("#pi"));
      speciesTitles.push_back(TString("K"));
      speciesTitles.push_back(TString("P"));
      vector<TString> corrNames;
      corrNames.push_back(TString("_A2_DyDphi_shft_B2_1Bar_2"));
      corrNames.push_back(TString("_A2_DyDphi_shft_B2_1_2Bar"));
      corrNames.push_back(TString("_A2_DyDphi_shft_B2_12Sum"));

      for (int iSpecies1=0; iSpecies1<3; iSpecies1++)
        {
        TString outFileNameBase2 = outFileNameBase;
        outFileNameBase2 += (speciesNames[iSpecies1]);
        outFileNameBase2 += "_";
        histoInputFileNames.clear();
        names.clear();
        titles.clear();
        histoTypes.clear();
        histoNames2D.clear();
        histoNames1DVsDeltaY.clear();
        histoNames1DVsDeltaPhi.clear();

        for (int iSpecies2=0; iSpecies2<3; iSpecies2++)
          {

          for (int iType=0; iType<3; iType++)
            {
            histoInputFileNames.push_back(inFileName);
            names.push_back(outFileNameBase2+(speciesNames[iSpecies2]+"_")+(speciesNames[iSpecies1])+(corrNames[iType]));
            titles.push_back(titleBase+(speciesTitles[iSpecies2])+(speciesTitles[iSpecies1]));
            histoTypes.push_back(iType);
            histoNames2D.push_back(          histoNameBase+(speciesNames[iSpecies2]+"_")+(speciesNames[iSpecies1])+(corrNames[iType]));
            histoNames1DVsDeltaY.push_back(  histoNameBase+(speciesNames[iSpecies2]+"_")+(speciesNames[iSpecies1])+(corrNames[iType])+"_x");
            histoNames1DVsDeltaPhi.push_back(histoNameBase+(speciesNames[iSpecies2]+"_")+(speciesNames[iSpecies1])+(corrNames[iType])+"_y");
            }
          }
        outputPath = outputPathBase;
        outputPath += "/PiKP/Y10/A2Derived/";
        plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1DVsDeltaY,histoNames1DVsDeltaPhi,names,titles,histoTypes,
                (speciesNames[iSpecies1]+"_"),
                -0.05, 0.1,
                -0.02, 0.39,
                -0.02, 0.19,
                -0.19, 1.19,
                rebin,rapidityType,plot2D,plot1D,option,plot1DHighRes,plotWidths,plotIntegralVsLogY);
        }
      break;
      }

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


//      names.push_back(TString("PYTHIA_pp_13.0TeV_B2Based_PiP_PiP_B1Bar2"));
//      titles.push_back(TString("pp #sqrt{s} = 13.0 TeV; #pi#pi"));
//      histoTypes.push_back(0);
//        histoNames2D.push_back(TString("PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_1Bar_2"));
//        histoNames1DVsDeltaY.push_back(TString(  "PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_1Bar_2_x"));
//        histoNames1DVsDeltaPhi.push_back(TString("PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_1Bar_2_y"));
//
//      histoInputFileNames.push_back(TString("PP/13000/PiKp/y10/PairGenBalFctSum0TO9.root"));
//      names.push_back(TString("PYTHIA_pp_13.0TeV_B2Based_PiP_PiP_B12Bar"));
//      titles.push_back(TString("pp #sqrt{s} = 13.0 TeV; #pi#pi"));
//      histoTypes.push_back(1);
//      histoNames2D.push_back(TString("PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_1_2Bar"));
//      histoNames1DVsDeltaY.push_back(TString(  "PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_1_2Bar_x"));
//      histoNames1DVsDeltaPhi.push_back(TString("PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_1_2Bar_y"));
//
//      histoInputFileNames.push_back(TString("PP/13000/PiKp/y10/PairGenBalFctSum0TO9.root"));
//      names.push_back(TString("PYTHIA_pp_13.0TeV_B2Based_PiP_PiP_B12s"));
//      titles.push_back(TString("pp #sqrt{s} = 13.0 TeV; #pi#pi"));
//      histoTypes.push_back(2);
//      histoNames2D.push_back(TString("PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_12Sum"));
//      histoNames1DVsDeltaY.push_back(TString(  "PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_12Sum_x"));
//      histoNames1DVsDeltaPhi.push_back(TString("PairGen_All_PiP_PiP_B2_DyDphi_shft_B2_12Sum_y"));
//
//      // ---
//      histoInputFileNames.push_back(TString("PP/13000/PiKp/y10/PairGenBalFctSum0TO9.root"));
//      names.push_back(TString("PYTHIA_pp_13.0TeV_B2Based_KP_PiP_B1Bar2"));
//      titles.push_back(TString("pp #sqrt{s} = 13.0 TeV; K#pi"));
//      histoTypes.push_back(0);
//      histoNames2D.push_back(TString("PairGen_All_KP_PiP_B2_DyDphi_shft_B2_1Bar_2"));
//      histoNames1DVsDeltaY.push_back(TString(  "PairGen_All_KP_PiP_B2_DyDphi_shft_B2_1Bar_2_x"));
//      histoNames1DVsDeltaPhi.push_back(TString("PairGen_All_KP_PiP_B2_DyDphi_shft_B2_1Bar_2_y"));
//
//      histoInputFileNames.push_back(TString("PP/13000/PiKp/y10/PairGenBalFctSum0TO9.root"));
//      names.push_back(TString("PYTHIA_pp_13.0TeV_B2Based_KP_PiP_B12Bar"));
//      titles.push_back(TString("pp #sqrt{s} = 13.0 TeV; K#pi"));
//      histoTypes.push_back(1);
//      histoNames2D.push_back(TString("PairGen_All_KP_PiP_B2_DyDphi_shft_B2_1_2Bar"));
//      histoNames1DVsDeltaY.push_back(TString(  "PairGen_All_KP_PiP_B2_DyDphi_shft_B2_1_2Bar_x"));
//      histoNames1DVsDeltaPhi.push_back(TString("PairGen_All_KP_PiP_B2_DyDphi_shft_B2_1_2Bar_y"));
//
//      histoInputFileNames.push_back(TString("PP/13000/PiKp/y10/PairGenBalFctSum0TO9.root"));
//      names.push_back(TString("PYTHIA_pp_13.0TeV_B2Based_KP_PiP_B12s"));
//      titles.push_back(TString("pp #sqrt{s} = 13.0 TeV; K#pi"));
//      histoTypes.push_back(2);
//      histoNames2D.push_back(TString("PairGen_All_KP_PiP_B2_DyDphi_shft_B2_12Sum"));
//      histoNames1DVsDeltaY.push_back(TString(  "PairGen_All_KP_PiP_B2_DyDphi_shft_B2_12Sum_x"));
//      histoNames1DVsDeltaPhi.push_back(TString("PairGen_All_KP_PiP_B2_DyDphi_shft_B2_12Sum_y"));
