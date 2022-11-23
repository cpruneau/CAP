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
                        vector<TGraph*> & integralGraphsHighRes,
                        double etaLow,
                        double etaHigh,
                        double etaStep,
                        int size=500)
{
  double * x  = new double[size];
  double * ex = new double[size];
  double * y  = new double[size];
  double * ey = new double[size];
  double sum, esum;
  const TAxis * xAxis  = h2->GetXaxis();
  const TAxis * yAxis  = h2->GetYaxis();
  int xLow  = xAxis->GetXmin();
  int xHigh = xAxis->GetXmax();
  int yLow  = yAxis->FindBin(-0.5*TMath::Pi());
  int yHigh = yAxis->FindBin( 1.5*TMath::Pi());

  int nPoints        = 0;
  int nPointsHighRes = 0;

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
    if (eta<3.0) nPointsHighRes++;
    }
  TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
  integralGraphs.push_back(g);
  TGraph * gHR = new TGraphErrors(nPointsHighRes, x,y,ex,ey);
  integralGraphsHighRes.push_back(gHR);
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
            vector<TString> & histoNames1Dx,
            vector<TString> & histoNames1Dy,
            vector<TString> & names,
            vector<TString> & titles,
            vector<TString> & widthEntryNames,
            int xAxisType,
            int b2Type,
            bool doWidth=false)
{
  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogX(CanvasConfiguration::Landscape,CanvasConfiguration::LogX);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  vector<GraphConfiguration*>  graphConfigurations2D = GraphConfiguration::createConfigurationPalette(10,2);

  for (int k=0;k<10;k++)
    {
    graphConfigurations1D[k]->setParameter("xTitleSize",   0.08);
    graphConfigurations1D[k]->setParameter("xTitleOffset", 0.8);
    graphConfigurations1D[k]->setParameter("yTitleSize",   0.08);
    graphConfigurations1D[k]->setParameter("yTitleOffset", 0.8);
    graphConfigurations1D[k]->setParameter("xLabelSize",   0.07);
    graphConfigurations1D[k]->setParameter("yLabelSize",   0.07);
    }
  graphConfigurations1D[3]->setParameter("markerColor",  kBlue);
  graphConfigurations1D[3]->setParameter("markerStyle",  kFullSquare);

  graphConfigurations1D[4]->setParameter("markerColor",  kBlue);
  graphConfigurations1D[4]->setParameter("markerStyle",  25);

  vector<TH2*>    histos2D;
  vector<TH1*>    histos1Dx;
  vector<TH1*>    histos1Dy;
  vector<TGraph*> integralGraphs;
  vector<TH1*>    histos1DxHighRes;
  vector<TH1*>    histos1DyHighRes;
  vector<TGraph*> integralGraphsHighRes;
  vector<double>  rmsWidths;
  vector<double>  rmsWidthErrors;
  TGraph* widthsGraph;
  TString widthHistoName     = "PythiaWidthsVsYaccept";
  TString b2XprojName        = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString b2YprojName        = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString i2Name             = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString b2XprojNameHighRes = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString b2YprojNameHighRes = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString i2NameHighRes      = "PythiaPPvsS_B2_1_2BarVsDeltaEta";
  TString widthName          = "rms";
  TString sName              = "system";

  double minX     = -20.0;
  double maxX     =  20.0;
  double minB2    = -0.01;
  double maxB2    =  0.1;
  double minB2x   = -0.2199;    

  double maxB2x   =  0.4999;
  double minB2y   = -0.25;
  double maxB2y   =  0.25;
  double minI2    = -1.199;
  double maxI2    =  3.199;
  double etaLow   =  0.1;
  double etaHigh  =  maxX-0.05;
  double etaStep  =  0.2;

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
      titleB2     = "B^{-+}"; // B^{#bar{1}2}";
      titleI2     = "I^{-+}"; // "I^{#bar{1}2}";
      b2XprojName = "PythiaPPvsS_B2_1Bar_2_VsDeltaX";
      b2YprojName = "PythiaPPvsS_B2_1Bar_2_VsDeltaY";
      i2Name      = "PythiaPPvsS_I2_1Bar_2_VsDeltaX";
      b2XprojNameHighRes = "PythiaPPvsS_B2_1Bar_2_VsDeltaXHighRes";
      b2YprojNameHighRes = "PythiaPPvsS_B2_1Bar_2_VsDeltaYHighRes";
      i2NameHighRes      = "PythiaPPvsS_I2_1Bar_2_VsDeltaXHighRes";
      break;
      case 1:
      titleB2 = "B^{+-}";
      titleI2 = "I^{+-}";
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
      cout << " In file " <<  histoInputFileNames[iFile] << endl;
      cout << " Could not load histogram named " << histoNames2D[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
   h2->Rebin2D(4,2);
   h2->Scale(0.25);

    histos2D.push_back(h2);

    TH1 * h1x = plotter->getHistogramCollection().loadH1(f,histoNames1Dx[iFile]);
    if (!h1x)
      {
      cout << " In file " <<  histoInputFileNames[iFile] << endl;
      cout << " Could not load histogram named " << histoNames1Dx[iFile] << endl;
      cout << " ABORT!!!!" << endl;
      return -1;
      }
    histos1Dx.push_back(h1x);
    histos1DxHighRes.push_back(h1x);

    TH1 * h1y = plotter->getHistogramCollection().loadH1(f,histoNames1Dy[iFile]);
    if (!h1y)
      {
      cout << " In file " <<  histoInputFileNames[iFile] << endl;
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
    calculateIntegrals(h2,integralGraphs,integralGraphsHighRes,etaLow,etaHigh,etaStep);

    TCanvas * c = new TCanvas();
    TH1 * projX = h2->ProjectionX();
    TF1 * f1 = new TF1("f1","gaus",-10.0,10.0);
    // set initial parameters (not really needed for gaus)
    f1->SetParameters(projX->GetMaximum(), projX->GetMean(), projX->GetRMS() );
    projX->Fit("f1");

    TF1 * fitResult = projX->GetFunction("f1");
    double p1 = fitResult->GetParameter(0);
    double p2 = fitResult->GetParameter(1);
    double p3 = fitResult->GetParameter(2);
    double e1 = fitResult->GetParError(0);
    double e2 = fitResult->GetParError(1);
    double e3 = fitResult->GetParError(2);
    cout << " p1: " << p1 << " +- " << e1 << endl;
    cout << " p2: " << p2 << " +- " << e2 << endl;
    cout << " p3: " << p3 << " +- " << e3 << endl;
    //calculateWidths(h2,minX, maxX, rmsWidths, rmsWidthErrors);
    rmsWidths.push_back(p3); rmsWidthErrors.push_back(e3);
    }

  cout << "widths" << endl;
  int nW = rmsWidths.size();
  cout << "nw:" << nW << endl;
  for (int k=0;k<nW;k++)
    {
    cout << "   k : " << k << "  " << rmsWidths[k] << " +- " << rmsWidthErrors[k] << endl;
    }

  cout << "Make width histogram" << endl;

  TH1 * widthsHisto = makeHistoWithNames(widthHistoName, widthEntryNames, rmsWidths, rmsWidthErrors);
  //widthsHisto->Draw(); // return 0;

  if (widthsHisto)
    {
    plotter->plot(widthsHisto,widthHistoName,
                  landscapeLinear,
                  *(graphConfigurations1D[0]),
                  "",4.0,-4.0,
                  "RMS",0.0,2.0,
                  "RMS",0.6, 0.3, 0.8, 0.4, 0.05);
    }
  else
    {
    cout << "widthsHisto is a null pointer...  " << endl;
    }

  cout << "Done with width histogram" << endl;



  // y dependence
  minX = -20.0; maxX = 20.0;
  //minX   = -6.0;  maxX = 6.0;
  //minB2x = -0.02; maxB2x = 0.5;
  minB2x = -0.149;  maxB2x = 0.5;
  minI2  =  0.0;   maxI2 = 1.19;
  //minI2  =  -1.2;   maxI2 = 3.2;
  double minInt = 0.02;
  double maxInt = 20.0;
  TLine * line = new TLine(0.0,1.0,  20.0,1.0);
  TLine * line2 = new TLine(0.0,1.0, 3.0,1.0);

  //->GetXaxis()->SetRangeUser(-6.0,6.0)
  switch (b2Type)
    {
      case 0:
      plotter->plot(histos1Dx,graphConfigurations1D,widthEntryNames,b2XprojName,landscapeLinear,titleX, minX, maxX,titleB2,minB2x,maxB2x, 0.65, 0.55, 0.8, 0.9, 0.05);
      plotter->plot(histos1Dy,graphConfigurations1D,widthEntryNames,b2YprojName,landscapeLinear,titleY, 1.0, -1.0, titleB2, 0.10, 0.25, 0.5, 0.55, 0.76, 0.85, 0.05);
      plotter->plot(integralGraphs,   graphConfigurations1D,widthEntryNames,i2Name,landscapeLogX,titleX, minInt,maxInt, titleI2,minI2,maxI2,    0.55, 0.2, 0.85, 0.50, 0.05);
      line->Draw();
      plotter->plot(histos1DxHighRes,graphConfigurations1D,widthEntryNames,b2XprojNameHighRes,landscapeLinear,titleX, -3.0, 3.0,titleB2,-0.02, 0.49, 0.40, 0.2, 0.60, 0.45, 0.04);
      plotter->plot(integralGraphsHighRes, graphConfigurations1D,widthEntryNames,i2NameHighRes,landscapeLinear,titleX, 0.0, 3.0, titleI2,0.0, 1.199,   0.49, 0.3, 0.85, 0.6, 0.04);
      line2->Draw();
      break;
      case 1:
      plotter->plot(histos1Dx,graphConfigurations1D,widthEntryNames,b2XprojName,landscapeLinear,titleX, minX, maxX,titleB2,minB2x,maxB2x, 0.65, 0.55, 0.8, 0.9, 0.05);
      plotter->plot(histos1Dy,graphConfigurations1D,widthEntryNames,b2YprojName,landscapeLinear,titleY, 1.0, -1.0, titleB2, 0.10, 0.25, 0.5, 0.55, 0.76, 0.85, 0.05);
      plotter->plot(integralGraphs,   graphConfigurations1D,widthEntryNames,i2Name,landscapeLogX,titleX, minInt,maxInt,  titleI2,minI2,maxI2,   0.55, 0.2, 0.85, 0.50, 0.05);
      line->Draw();
      plotter->plot(histos1DxHighRes,graphConfigurations1D,widthEntryNames,b2XprojNameHighRes,landscapeLinear,titleX, -3.0, 3.0,titleB2,-0.02, 0.49, 0.40, 0.2, 0.60, 0.45, 0.04);
      plotter->plot(integralGraphsHighRes, graphConfigurations1D,widthEntryNames,i2NameHighRes,landscapeLinear,titleX, 0.0, 3.0, titleI2,-0.02, 1.199,   0.52, 0.3, 0.85, 0.6, 0.04);
      line2->Draw();

      break;
      case 2:
      cout << " case 2 ----" << endl;
      plotter->plot(histos1Dx,graphConfigurations1D,widthEntryNames,b2XprojName,landscapeLinear,titleX, minX, maxX,titleB2, minB2x,maxB2x, 0.6, 0.5, 0.75, 0.9, 0.045);
      plotter->plot(histos1Dy,graphConfigurations1D,widthEntryNames,b2YprojName,landscapeLinear,titleY, 1.0, -1.0, titleB2, 0.10, 0.25, 0.5, 0.55, 0.76, 0.85, 0.05);
      plotter->plot(integralGraphs,   graphConfigurations1D,widthEntryNames,i2Name,landscapeLogX,titleX, minInt,maxInt,  titleI2,minI2,maxI2,   0.55, 0.2, 0.8, 0.45, 0.047);
      line->Draw();
      plotter->plot(histos1DxHighRes,graphConfigurations1D,widthEntryNames,b2XprojNameHighRes,landscapeLinear,titleX, -3.0, 3.0,titleB2,-0.02, 0.49,   0.65, 0.6, 0.80, 0.87, 0.05);
      //plotter->plot(integralGraphsHighRes, graphConfigurations1D,titles,i2NameHighRes,landscapeLinear,titleX, 0.0, 3.0, titleI2,-0.02, 1.199,   0.52, 0.3, 0.72, 0.6, 0.04);
      plotter->plot(integralGraphsHighRes, graphConfigurations1D,widthEntryNames,i2NameHighRes,landscapeLinear,titleX, 0.0, 3.0, titleI2,-0.02, 1.199,   0.45, 0.2, 0.75, 0.5, 0.04);
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
  vector<TString> widthLabels;

  Configuration plotConfig;

  Plotter * plotter = new Plotter("Plotter",plotConfig);
  plotter->setDefaultOptions(useColor);


  // vector<TGraph*>  graphs;

  switch (option)
    {
      case 8:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/900/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_0.9TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 0.9 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 0.9 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2Based_B2_1Bar_2"));
      widthLabels.push_back(TString("#sqrt{s} = 2.76 TeV"));
      titles.push_back(TString("(a) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5020/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("#sqrt{s} = 5.02 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 5.02 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("pp  #sqrt{s} = 13.0 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2Based_B2_1Bar_2"));
      titles.push_back(TString("#sqrt{s} = 13.0 TeV - p#bar{p}"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV  - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1Bar_2_y"));

      outputPath += "/PM/Y10/B2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,0);
      break;

      case 9:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/900/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_0.9TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 0.9 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 0.9 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2Based_B2_1_2Bar"));
      widthLabels.push_back(TString("#sqrt{s} = 2.76 TeV"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5020/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 5.02 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 5.02 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) pp  #sqrt{s} = 13.0 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV  - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_1_2Bar_y"));

      outputPath += "/PM/Y10/B2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,1);
      break;


      case 10:
      histoInputFileNames.push_back(TString("PP/900/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_0.9TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 0.9 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 0.9 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_B2Based_B2_12Sum"));
      widthLabels.push_back(TString("#sqrt{s} = 2.76 TeV"));
      titles.push_back(TString("(c) #sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/5020/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.2TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 5.02 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 5.02 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(c) pp  #sqrt{s} = 13.0 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_B2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV  - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_B2_DyDphi_shft_B2_12Sum_y"));

      outputPath += "/PM/Y10/B2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,2);
      break;



      // a2
      case 18:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/900/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 0.9 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 0.9 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 2.76 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/5020/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.02TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 5.02 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 5.02 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 13.0 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      outputPath += "/PM/Y10/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,0);
      break;

      case 19:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/900/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 0.9 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 0.9 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 2.76 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/5020/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.02TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 5.02 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 5.02 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 13.0 TeV"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));


      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_1_2Bar"));
      titles.push_back(TString("(b) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      widthLabels.push_back(TString("#sqrt{s} = 13.0 TeV - p#bar{p}"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar_y"));

      outputPath += "/PM/Y10/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,1);
      break;

      case 20:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/900/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 0.9 TeV"));
      widthLabels.push_back(TString("1.0 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/2700/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_2.7TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 2.76 TeV"));
      widthLabels.push_back(TString("2.76 TeV"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/5020/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_5.02TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 5.02 TeV"));
      widthLabels.push_back(TString("5.02 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 13.0 TeV"));
      widthLabels.push_back(TString("13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PPbar/13000/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_ppBar_13TeV_A2Based_B2_12Sum"));
      titles.push_back(TString("(c) #sqrt{s} = 13.0 TeV - p#bar{p}"));
      widthLabels.push_back(TString("p#bar{p} 13.0 TeV"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      outputPath += "/PM/Y10/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,2);
      break;


      // a2 nominal, shoving, ropes



      // a2 vs y range w/ filter
      case 41:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/13000/PM/Y1/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_Y1_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) y_{0} = 1"));
      widthLabels.push_back(TString("y_{0}=1"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y2/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_Y2_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) y_{0} = 2"));
      widthLabels.push_back(TString("y_{0}=2"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y4/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_13TeV_Y4_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) y_{0} = 4"));
      widthLabels.push_back(TString("y_{0}=4"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y10/PairGenBalFctSum0TO4.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y10_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) y_{0} = 10"));
      widthLabels.push_back(TString("y_{0}=10"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));


      outputPath += "/PM/Ydependence/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,0);
      break;

      case 42:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PM/Y1/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y1_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(a) y_{0} = 1"));
      widthLabels.push_back(TString("y_{0}=1"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/1000/PM/Y2/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y2_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) y_{0} = 2"));
      widthLabels.push_back(TString("y_{0}=2"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/1000/PM/Y4/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y4_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) y_{0} = 4"));
      widthLabels.push_back(TString("y_{0}=4"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));

      histoInputFileNames.push_back(TString("PP/1000/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y10_A2Based_B2_1Bar_2"));
      titles.push_back(TString("(b) y_{0} = 10"));
      widthLabels.push_back(TString("y_{0}=10"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_1Bar_2_y"));


      outputPath += "/PM/Ydependence/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,1);
      break;

      case 43:
      // Y10 -- A2
      histoInputFileNames.push_back(TString("PP/1000/PM/Y1/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y1_A2Based_B2_12Sum"));
      titles.push_back(TString("y_{0} = 1"));
      widthLabels.push_back(TString("y_{0}=1"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/1000/PM/Y2/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y2_A2Based_B2_12Sum"));
      titles.push_back(TString("y_{0} = 2"));
      widthLabels.push_back(TString("y_{0}=2"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/1000/PM/Y4/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y4_A2Based_B2_12Sum"));
      titles.push_back(TString("y_{0} = 4"));
      widthLabels.push_back(TString("y_{0}=4"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/1000/PM/Y10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y10_A2Based_B2_12Sum"));
      titles.push_back(TString("y_{0} = 10"));
      widthLabels.push_back(TString("y_{0}=10"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));


      outputPath += "/PM/Ydependence/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,2);
      break;

      case 53:

      histoInputFileNames.push_back(TString("PP/13000/PM/Y4/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_pT0.0To20_A2Based_B2_12Sum"));
      titles.push_back(TString("p_{T} > 0.0"));
      widthLabels.push_back(TString("p_{T} > 0.0"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y4_pT0.0To10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y4_pT0.0To10_A2Based_B2_12Sum"));
      titles.push_back(TString("0.0 < p_{T} < 10 GeV/c"));
      widthLabels.push_back(TString("0.0#leq p_{T} < 10 GeV/c"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y4_pT0.2To10/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y4_pT0.2To10_A2Based_B2_12Sum"));
      titles.push_back(TString("0.2#leq p_{T} < 10 GeV/c"));
      widthLabels.push_back(TString("0.2#leq p_{T} < 10 GeV/c"));
      histoNames2D.push_back(TString( "PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      histoInputFileNames.push_back(TString("PP/13000/PM/Y4_pT0.1To2/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y4_pT0.1To2_A2Based_B2_12Sum"));
      titles.push_back(TString("0.1#leq p_{T} < 2 GeV/c"));
      widthLabels.push_back(TString("0.1#leq p_{T} < 2 GeV/c"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));


      histoInputFileNames.push_back(TString("PP/13000/PM/Y4_pT0.2To2/PairGenBalFctSum0TO9.root"));
      names.push_back(TString("PYTHIA_pp_1TeV_Y4_pT0.2To2_A2Based_B2_12Sum"));
      titles.push_back(TString("0.2#leq p_{T} < 2 GeV/c"));
      widthLabels.push_back(TString("0.2#leq p_{T} < 2 GeV/c"));
      histoNames2D.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum"));
      histoNames1Dx.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_x"));
      histoNames1Dy.push_back(TString("PairGen_All_HP_HP_A2_DyDphi_shft_B2_12Sum_y"));

      outputPath += "/PM/pTdependence/A2Derived/";
      plotSet(plotter,inputPath,outputPath,histoInputFileNames,histoInputFiles,histoNames2D,histoNames1Dx,histoNames1Dy,names,titles,widthLabels,1,2);
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
