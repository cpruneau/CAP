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

TFile * openRootFile(Plotter * plotter, const TString & inputPath, const TString & fileName)
{
  TFile * f =  plotter->openRootFile(inputPath,fileName,"OLD");
  if (!f)
    {
    cout << " Could not open file named " << fileName << endl;
    cout << " ABORT!!!!" << endl;
    }
  return f;
}

vector<GraphConfiguration*> createGraphConfigurationPalette(int n, int dim)
{
  vector<GraphConfiguration*>  gc = GraphConfiguration::createConfigurationPalette(n,dim);
  for (int k=0;k<n;k++)
    {
    gc[k]->setParameter("xTitleSize",   0.08);
    gc[k]->setParameter("xTitleOffset", 0.8);
    gc[k]->setParameter("yTitleSize",   0.08);
    gc[k]->setParameter("yTitleOffset", 0.8);
    gc[k]->setParameter("xLabelSize",   0.07);
    gc[k]->setParameter("yLabelSize",   0.07);
    gc[k]->setParameter("lineColor",    41+k);
    gc[k]->setParameter("markerColor",  41+k);
    gc[k]->setParameter("markerStyle",  kFullSquare);
    gc[k]->setParameter("markerSize",   0.9);
    }
  return gc;
}

GraphConfiguration* create1DGraphConfiguration(double size= 0.07)
{
  GraphConfiguration * gc = new GraphConfiguration(1,0);
  gc->setParameter("markerColor",  kBlack);
  gc->setParameter("markerStyle",  kFullSquare);
  gc->setParameter("markerSize",   1.2);
  gc->setParameter("xLabelSize",   size);
  gc->setParameter("xLabelSize",   0.06);
  gc->setParameter("yTitleSize",   size);
  gc->setParameter("yTitleOffset", 0.7);
  gc->setParameter("yLabelSize",   0.07);
  return gc;
}


GraphConfiguration* create2DGraphConfiguration(double size= 0.07)
{
  GraphConfiguration * gc = new GraphConfiguration(2,0);
  gc->setParameter("xTitleOffset",  1.2);
  gc->setParameter("xTitleSize",   size);
  gc->setParameter("xLabelOffset", 0.01);
  gc->setParameter("xLabelSize",   size);

  gc->setParameter("yTitleOffset", 1.2);
  gc->setParameter("yTitleSize",   size);
  gc->setParameter("yLabelOffset", 0.01);
  gc->setParameter("yLabelSize",   size);

  gc->setParameter("zTitleOffset", 1.2);
  gc->setParameter("zTitleSize",   size);
  gc->setParameter("zLabelOffset", 0.01);
  gc->setParameter("zLabelSize",   size);
  return gc;
}

TString makeName(const TString & s1, const TString & s2)
{
  TString s = s1;
  s += "_";
  s += s2;
  return s;
}

TString makeName(const TString & s1, const TString & s2, const TString & s3)
{
  TString s = s1;
  s += "_";
  s += s2;
  s += "_";
  s += s3;
  return s;
}

TString makeName(const TString & s1, const TString & s2, const TString & s3, const TString & s4)
{
  TString s = s1;
  s += "_";
  s += s2;
  s += "_";
  s += s3;
  s += "_";
  s += s4;
  return s;
}


void rebin2D(TH2 * h2)
{
  h2->Rebin2D(4,2);
  h2->Scale(0.25);
}

void createSumGraphs(vector<TGraph*> & source, vector<TGraph*> & target)
{
  TGraph * g;
  TGraph * gs;
  for (int iGraph=0; iGraph<source.size(); iGraph++)
    {
    g = source[iGraph];
    if (iGraph==0)
      {
      gs = (TGraph *)g->Clone();
      }
    else
      {
      gs = sumGraphs(gs, g);
      }
    target.push_back(gs);
    }
}



int plotSet(Plotter * plotter,
            const TString & inputPath,
            vector<TString> & histoInputFileNames,
            vector<int>     & balFct_Types,
            vector<TString> & balFct_DeltaYDeltaPhi_Histo_Names,
            vector<TString> & balFct_DeltaY_Histo_Names,
            vector<TString> & balFct_DeltaPhi_Histo_Names,
            vector<LegendConfiguration*> & balFct_LegendConfigs,
            vector<double>  & balFct_Minima,
            vector<double>  & balFct_Maxima,
            const TString   & outFileNameBase,
            bool rebin         = false,
            int  rapidityType  = 1,
            int  plot2D        = 1,
            int  plot1D        = 1,
            int  plot1DType    = 0,
            int  plot1DHighRes = 0,
            int  plotWidths    = 1,
            int  plotIntegralVsLogY=1)
{
  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogX(CanvasConfiguration::Landscape,CanvasConfiguration::LogX);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = createGraphConfigurationPalette(15,1);
  GraphConfiguration * graphConfiguration2D    = create2DGraphConfiguration(0.07);
  GraphConfiguration * widthGraphConfiguration = create1DGraphConfiguration();

  vector<TH2*>    balFct1Bar2_DeltaYDeltaPhi_Histos;
  vector<TH1*>    balFct1Bar2_DeltaY_Histos;
  vector<TH1*>    balFct1Bar2_DeltaPhi_Histos;
  vector<TGraph*> balFct1Bar2_Integral_DeltaY_Graphs;
  vector<TGraph*> balFct1Bar2_IntegralSum_DeltaY_Graphs;
  vector<TString> balFct1Bar2_DeltaYDeltaPhi_Names;
  vector<TString> balFct1Bar2_DeltaY_Names;
  vector<TString> balFct1Bar2_DeltaPhi_Names;
  vector<TString> balFct1Bar2_Integral_DeltaY_Names;

  vector<TH2*>    balFct12Bar_DeltaYDeltaPhi_Histos;
  vector<TH1*>    balFct12Bar_DeltaY_Histos;
  vector<TH1*>    balFct12Bar_DeltaPhi_Histos;
  vector<TGraph*> balFct12Bar_Integral_DeltaY_Graphs;
  vector<TGraph*> balFct12Bar_IntegralSum_DeltaY_Graphs;

  vector<TString> balFct12Bar_DeltaYDeltaPhi_Names;
  vector<TString> balFct12Bar_DeltaY_Names;
  vector<TString> balFct12Bar_DeltaPhi_Names;
  vector<TString> balFct12Bar_Integral_DeltaY_Names;

  vector<TH2*>    balFct12s_DeltaYDeltaPhi_Histos;
  vector<TH1*>    balFct12s_DeltaY_Histos;
  vector<TH1*>    balFct12s_DeltaPhi_Histos;
  vector<TGraph*> balFct12s_Integral_DeltaY_Graphs;
  vector<TGraph*> balFct12s_IntegralSum_DeltaY_Graphs;
  vector<TString> balFct12s_DeltaYDeltaPhi_Names;
  vector<TString> balFct12s_DeltaY_Names;
  vector<TString> balFct12s_DeltaPhi_Names;
  vector<TString> balFct12s_Integral_DeltaY_Names;

  TString DeltaY_Name    = "DeltaY";
  TString DeltaPhi_Name  = "DeltaPhi";
  TString DeltaY_Title   = "#Delta y";
  TString DeltaPhi_Title = "#Delta #varphi";

  TString balFct_Name;
  TString balFct_Integral_Name;
  TString balFct_IntegralSum_Name;
  TString balFct_DeltaYDeltaPhi_Name;
  TString balFct_DeltaY_Name;
  TString balFct_DeltaPhi_Name;
  TString balFct_Integral_DeltaY_Name;
  TString balFct_IntegralSum_DeltaY_Name;
  TString balFct_Title;
  TString balFct_Integral_Title;
  TString balFct_IntegralSum_Title;

  TString prefix = "";

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
  TLine * line = new TLine(0.1,1.0, maxRapidity,1.0);

  cout << "plot1DHighRes....:" << plot1DHighRes << endl;
  cout << "minRapidity......:" << minRapidity << endl;
  cout << "maxRapidity......:" << maxRapidity << endl;

  for (unsigned int iFile=0; iFile<histoInputFileNames.size(); iFile++)
    {
    cout << " iFile  ==  " << iFile << endl;
    TFile * f =  openRootFile(plotter,inputPath,histoInputFileNames[iFile]);
    if (!f) return -1;
    TH2 * h2  = plotter->getHistogramCollection().loadH2(f,balFct_DeltaYDeltaPhi_Histo_Names[iFile]);
    if (!h2)  return -1;
    if (rebin) rebin2D(h2);
    double dx = h2->GetXaxis()->GetBinWidth(1);
    double dy = h2->GetYaxis()->GetBinWidth(1);
    h2->Scale(1.0/dy);
    TH1 * h1x = plotter->getHistogramCollection().loadH1(f,balFct_DeltaY_Histo_Names[iFile]);
    if (!h1x)  return -1;
    TH1 * h1y = plotter->getHistogramCollection().loadH1(f,balFct_DeltaPhi_Histo_Names[iFile]);
    if (!h1y)  return -1;

    switch (balFct_Types[iFile])
      {
        case 0:
        balFct_Name                    = "B1Bar2";
        balFct_Integral_Name           = "I1Bar2";
        balFct_IntegralSum_Name        = "I1Bar2Sum";
        balFct_DeltaYDeltaPhi_Name     = makeName(prefix,balFct_Name, DeltaY_Name,DeltaPhi_Name);
        balFct_DeltaY_Name             = makeName(prefix,balFct_Name, DeltaY_Name);
        balFct_DeltaPhi_Name           = makeName(prefix,balFct_Name, DeltaPhi_Name);
        balFct_Integral_DeltaY_Name    = makeName(prefix,balFct_Integral_Name, DeltaY_Name);
        balFct_IntegralSum_DeltaY_Name = makeName(prefix,balFct_IntegralSum_Name, DeltaY_Name);

        balFct1Bar2_DeltaYDeltaPhi_Histos.push_back(h2);
        balFct1Bar2_DeltaYDeltaPhi_Names.push_back(balFct_DeltaYDeltaPhi_Name);
        balFct1Bar2_DeltaY_Histos.push_back(h1x);
        balFct1Bar2_DeltaY_Names.push_back(balFct_DeltaY_Name);
        balFct1Bar2_DeltaPhi_Histos.push_back(h1y);
        balFct1Bar2_DeltaPhi_Names.push_back(balFct_DeltaPhi_Name);
        calculateIntegrals(h2,balFct1Bar2_Integral_DeltaY_Graphs,etaLow,etaHigh,etaStep);
        break;

        case 1:
        balFct_Name                    = "B12Bar";
        balFct_Integral_Name           = "I12Bar";
        balFct_IntegralSum_Name        = "I12BarSum";
        balFct_DeltaYDeltaPhi_Name     = makeName(prefix,balFct_Name, DeltaY_Name,DeltaPhi_Name);
        balFct_DeltaY_Name             = makeName(prefix,balFct_Name, DeltaY_Name);
        balFct_DeltaPhi_Name           = makeName(prefix,balFct_Name, DeltaPhi_Name);
        balFct_Integral_DeltaY_Name    = makeName(prefix,balFct_Integral_Name, DeltaY_Name);
        balFct_IntegralSum_DeltaY_Name = makeName(prefix,balFct_IntegralSum_Name, DeltaY_Name);

        balFct12Bar_DeltaYDeltaPhi_Histos.push_back(h2);
        balFct12Bar_DeltaYDeltaPhi_Names.push_back(balFct_DeltaYDeltaPhi_Name);
        balFct12Bar_DeltaY_Histos.push_back(h1x);
        balFct12Bar_DeltaY_Names.push_back(balFct_DeltaY_Name);
        balFct12Bar_DeltaPhi_Histos.push_back(h1y);
        balFct12Bar_DeltaPhi_Names.push_back(balFct_DeltaPhi_Name);
        calculateIntegrals(h2,balFct12Bar_Integral_DeltaY_Graphs,etaLow,etaHigh,etaStep);
        break;

        case 2:
        balFct_Name                    = "B12s";
        balFct_Integral_Name           = "I12s";
        balFct_IntegralSum_Name        = "I12sSum";
        balFct_DeltaYDeltaPhi_Name     = makeName(prefix,balFct_Name, DeltaY_Name,DeltaPhi_Name);
        balFct_DeltaY_Name             = makeName(prefix,balFct_Name, DeltaY_Name);
        balFct_DeltaPhi_Name           = makeName(prefix,balFct_Name, DeltaPhi_Name);
        balFct_Integral_DeltaY_Name    = makeName(prefix,balFct_Integral_Name, DeltaY_Name);
        balFct_IntegralSum_DeltaY_Name = makeName(prefix,balFct_IntegralSum_Name, DeltaY_Name);

        balFct12s_DeltaYDeltaPhi_Histos.push_back(h2);
        balFct12s_DeltaYDeltaPhi_Names.push_back(balFct_DeltaYDeltaPhi_Name);
        balFct12s_DeltaY_Histos.push_back(h1x);
        balFct12s_DeltaY_Names.push_back(balFct_DeltaY_Name);
        balFct12s_DeltaPhi_Histos.push_back(h1y);
        balFct12s_DeltaPhi_Names.push_back(balFct_DeltaPhi_Name);
        calculateIntegrals(h2,balFct12s_Integral_DeltaY_Graphs,etaLow,etaHigh,etaStep);
        break;
      }

    //    if (plot2D)
    //      {
    //      plotter->plot(balFct_DeltaYDeltaPhi_Name,landscapeLinear,*graphConfiguration2D,legendConfig2D,
    //                    h2,
    //                    DeltaY_Title,minRapidity,maxRapidity,
    //                    DeltaPhi_Title,1.0,-1.0,
    //                    balFct_title,balFct2d_Min, balFct2d_Max);
    //      }
    cout << "Histo load of plots completed" << endl;
    }

  if (plotWidths)
    {
    vector<double>  rmsWidths;
    vector<double>  rmsWidthErrors;

//    balFct12s_Width_name = "PythiaWidthsVsAcceptance";
//    TH1 * balFct12s_Width_Histos = makeHistoWithNames(balFct12s_Width_name, widthTitles, rmsWidths, rmsWidthErrors);
//    plotter->plot(balFct12s_Width_name,landscapeLinear, *widthGraphConfiguration, balFct12s_Width_legendConfig,
//                  balFct12s_Width_Histos,
//                  "",4.0,-4.0,"RMS",0.0,2.0,
//                  "",0.6, 0.3, 0.8, 0.4, 0.05);

    }

  prefix = outFileNameBase;
  cout << "    prefix: " << prefix << endl;

  if (plot1D)
    {
    if (balFct1Bar2_DeltaY_Histos.size()>0 && plot1D)
      {
      balFct_Title                   = "B^{#bar{#alpha}#beta}";
      balFct_Integral_Title          = "I^{#bar{#alpha}#beta}";
      balFct_IntegralSum_Title       = "#sum_{#bar#alpha} I^{#bar{#alpha}#beta}";
      balFct_Name                    = "B1Bar2";
      balFct_Integral_Name           = "I1Bar2";
      balFct_IntegralSum_Name        = "I1Bar2Sum";
      balFct_DeltaYDeltaPhi_Name     = makeName(prefix,balFct_Name, DeltaY_Name,DeltaPhi_Name);
      balFct_DeltaY_Name             = makeName(prefix,balFct_Name, DeltaY_Name);
      balFct_DeltaPhi_Name           = makeName(prefix,balFct_Name, DeltaPhi_Name);
      balFct_Integral_DeltaY_Name    = makeName(prefix,balFct_Integral_Name, DeltaY_Name);
      balFct_IntegralSum_DeltaY_Name = makeName(prefix,balFct_IntegralSum_Name, DeltaY_Name);

      createSumGraphs(balFct1Bar2_Integral_DeltaY_Graphs,balFct1Bar2_IntegralSum_DeltaY_Graphs);

      plotter->plot(balFct_DeltaY_Name,landscapeLinear, graphConfigurations1D, *balFct_LegendConfigs[1],
                    balFct1Bar2_DeltaY_Histos,
                    DeltaY_Title, minRapidity, maxRapidity,
                    balFct_Title, balFct_Minima[1], balFct_Maxima[1]);
      plotter->plot(balFct_DeltaPhi_Name,landscapeLinear, graphConfigurations1D, *balFct_LegendConfigs[2],
                    balFct1Bar2_DeltaPhi_Histos,
                    DeltaPhi_Title, 1.0, -1.0,
                    balFct_Title, balFct_Minima[2], balFct_Maxima[2]);
      plotter->plot(balFct_Integral_DeltaY_Name,landscapeLogX, graphConfigurations1D, *balFct_LegendConfigs[3],
                    balFct1Bar2_Integral_DeltaY_Graphs,
                    DeltaY_Title, 0.1, maxRapidity,
                    balFct_Integral_Title, balFct_Minima[3], balFct_Maxima[3]);
      line->Draw();
      plotter->plot(balFct_IntegralSum_DeltaY_Name,landscapeLogX, graphConfigurations1D, *balFct_LegendConfigs[4],
                    balFct1Bar2_IntegralSum_DeltaY_Graphs,
                    DeltaY_Title, 0.1, maxRapidity,
                    balFct_IntegralSum_Title, balFct_Minima[4], balFct_Maxima[4]);
      line->Draw();

      }
    if (balFct12Bar_DeltaY_Histos.size()>0 && plot1D)
      {
      balFct_Title                   = "B^{#alpha#bar#beta}";
      balFct_Integral_Title          = "I^{#alpha#bar#beta}";
      balFct_IntegralSum_Title       = "#sum_{#alpha} I^{#alpha#bar#beta}";

      balFct_Name                    = "B12Bar";
      balFct_Integral_Name           = "I12Bar";
      balFct_IntegralSum_Name        = "I12BarSum";
      balFct_DeltaYDeltaPhi_Name     = makeName(prefix,balFct_Name, DeltaY_Name,DeltaPhi_Name);
      balFct_DeltaY_Name             = makeName(prefix,balFct_Name, DeltaY_Name);
      balFct_DeltaPhi_Name           = makeName(prefix,balFct_Name, DeltaPhi_Name);
      balFct_Integral_DeltaY_Name    = makeName(prefix,balFct_Integral_Name, DeltaY_Name);
      balFct_IntegralSum_DeltaY_Name = makeName(prefix,balFct_IntegralSum_Name, DeltaY_Name);

      createSumGraphs(balFct12Bar_Integral_DeltaY_Graphs,balFct12Bar_IntegralSum_DeltaY_Graphs);

      plotter->plot(balFct_DeltaY_Name,landscapeLinear, graphConfigurations1D, *balFct_LegendConfigs[1],
                    balFct12Bar_DeltaY_Histos,
                    DeltaY_Title, minRapidity, maxRapidity,
                    balFct_Title, balFct_Minima[1], balFct_Maxima[1]);
      plotter->plot(balFct_DeltaPhi_Name,landscapeLinear, graphConfigurations1D, *balFct_LegendConfigs[2],
                    balFct12Bar_DeltaPhi_Histos,
                    DeltaPhi_Title, 1.0, -1.0,
                    balFct_Title, balFct_Minima[2], balFct_Maxima[2]);
      plotter->plot(balFct_Integral_DeltaY_Name,landscapeLogX, graphConfigurations1D, *balFct_LegendConfigs[3],
                    balFct12Bar_Integral_DeltaY_Graphs,
                    DeltaY_Title, 0.1, maxRapidity,
                    balFct_Integral_Title, balFct_Minima[3], balFct_Maxima[3]);
      line->Draw();
      plotter->plot(balFct_IntegralSum_DeltaY_Name,landscapeLogX, graphConfigurations1D, *balFct_LegendConfigs[4],
                    balFct12Bar_IntegralSum_DeltaY_Graphs,
                    DeltaY_Title, 0.1, maxRapidity,
                    balFct_IntegralSum_Title, balFct_Minima[4], balFct_Maxima[4]);
      line->Draw();
      }
    if (balFct12s_DeltaY_Histos.size()>0 && plot1D)
      {
      balFct_Title                   = "B^{s}";
      balFct_Integral_Title          = "I^{s}";
      balFct_IntegralSum_Title       = "#sum I^{s}";

      balFct_Name                    = "B12s";
      balFct_Integral_Name           = "I12s";
      balFct_IntegralSum_Name        = "I12sSum";
      balFct_DeltaYDeltaPhi_Name     = makeName(prefix,balFct_Name, DeltaY_Name,DeltaPhi_Name);
      balFct_DeltaY_Name             = makeName(prefix,balFct_Name, DeltaY_Name);
      balFct_DeltaPhi_Name           = makeName(prefix,balFct_Name, DeltaPhi_Name);
      balFct_Integral_DeltaY_Name    = makeName(prefix,balFct_Integral_Name, DeltaY_Name);
      balFct_IntegralSum_DeltaY_Name = makeName(prefix,balFct_IntegralSum_Name, DeltaY_Name);

      createSumGraphs(balFct12s_Integral_DeltaY_Graphs,balFct12s_IntegralSum_DeltaY_Graphs);

      plotter->plot(balFct_DeltaY_Name,landscapeLinear, graphConfigurations1D, *balFct_LegendConfigs[1],
                    balFct12s_DeltaY_Histos,
                    DeltaY_Title, minRapidity, maxRapidity,
                    balFct_Title, balFct_Minima[1], balFct_Maxima[1]);
      plotter->plot(balFct_DeltaPhi_Name,landscapeLinear, graphConfigurations1D, *balFct_LegendConfigs[2],
                    balFct12s_DeltaPhi_Histos,
                    DeltaPhi_Title, 1.0, -1.0,
                    balFct_Title, balFct_Minima[2], balFct_Maxima[2]);
      plotter->plot(balFct_Integral_DeltaY_Name,landscapeLogX, graphConfigurations1D, *balFct_LegendConfigs[3],
                    balFct12s_Integral_DeltaY_Graphs,
                    DeltaY_Title, 0.1, maxRapidity,
                    balFct_Integral_Title, balFct_Minima[3], balFct_Maxima[3]);
      line->Draw();
      plotter->plot(balFct_IntegralSum_DeltaY_Name,landscapeLogX, graphConfigurations1D, *balFct_LegendConfigs[4],
                    balFct12s_IntegralSum_DeltaY_Graphs,
                    DeltaY_Title, 0.1, maxRapidity,
                    balFct_IntegralSum_Title, balFct_Minima[4], balFct_Maxima[4]);
      line->Draw();

      }
    }

  return 0;
}

int PlotBFPiKP_version2()
{
  bool doPrint  = true;
  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 1;
  bool useColor = true;
  bool rebin    = false;
  int  rapidityType       = 1;
  int  plot2D             = 0;
  int  plot1D             = 1;
  int  plot1DHighRes      = 0;
  int  plotWidths         = 0;
  int  plotIntegralVsLogY = 1;
  // 0 : HP, HM
  // 1 : Pi, K, P
  // 2 : Baryons
  // 3 : Strange particles
  int  speciesSet = 1;
  // 0 : B2 based correlations
  // 1 : A2 based correlations
  int  setOption = 1;


  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  MessageLogger::LogLevel infoLevel = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Info;
  MessageLogger::LogLevel selectLevel = infoLevel;


  TString inputPath       = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/";
  TString inFileName      = "PP/13000/PiKp/y10/PairGenBalFctSum0TO9.root";
  TString outputPathBase  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/";
  TString outputPath;
  TString titleBase       = "#sqrt{s}=13.0 TeV; ";
  TString histoNameBase   = "PairGen_All_";

  vector<TString> histoInputFileNames;
  vector<TString> balFct_DeltaYDeltaPhi_Histo_Names;
  vector<TString> balFct_DeltaY_Histo_Names;
  vector<TString> balFct_DeltaPhi_Histo_Names;
  vector<TString> balFct_Titles;
  vector<TString> balFct_Output_Names;
  vector<int>     balFct_Types; // 0, 1, 2 : B1Bar2, B12Bar, Bs
  vector<double>  balFct_Minima;
  vector<double>  balFct_Maxima;
  TString         outFileNameBase;
  TString         outFileNameBase2;
  TString         balFct_Output_Name;

  // create 15 legends:
  // B1Bar2(Y), B1Bar2(phi), I1Bar2, I1Bar2Sum, B1Bar2Width
  // B12Bar(Y), B12Bar(phi), I12Bar, I12BarSum, B12BarWidth
  // B12s(Y),   B12s(phi),   I12s,   I12sSum,   B12sWidth
  vector<LegendConfiguration*> balFct_LegendConfigs;

  vector<TString> speciesNames;
  vector<TString> speciesTitles;
  vector<TString> corrNames;

  switch (speciesSet)
    {
      case 0:
      break;

      case 1:
      speciesNames.push_back(TString("PiP"));  speciesTitles.push_back(TString("#pi"));
      speciesNames.push_back(TString("KP"));   speciesTitles.push_back(TString("K"));
      speciesNames.push_back(TString("PP"));   speciesTitles.push_back(TString("P"));
      break;

      case 2:
      break;
    }
  int nSpecies = speciesNames.size();

  Configuration plotterConfig;
  Plotter * plotter = new Plotter("Plotter",plotterConfig);
  plotter->setDefaultOptions(useColor);

  // setOption == 0 B2 based BalcFct
  // setOption == 1 A2 based BalcFct
  //
  switch (setOption)
    {
      case 0:
      {
      outFileNameBase = "PYTHIA_pp_13.0TeV_B2Based";
      corrNames.push_back(TString("_B2_DyDphi_shft_B2_1Bar_2"));
      corrNames.push_back(TString("_B2_DyDphi_shft_B2_1_2Bar"));
      corrNames.push_back(TString("_B2_DyDphi_shft_B2_12Sum"));
      outputPath = outputPathBase;
      outputPath += "/PiKP/Y10/B2Derived/";
      // B vs Y,Phi
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.4);
      // B vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.4);
      // B vs phi
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.24);
      // I vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.24);
      // Isum vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.24);
      // B width
      balFct_Minima.push_back( 0.00);
      balFct_Maxima.push_back( 2.00);
      }
      break;

      case 1:
      {
      outFileNameBase = "PYTHIA_pp_13.0TeV_A2Based";
      corrNames.push_back(TString("_A2_DyDphi_shft_B2_1Bar_2"));
      corrNames.push_back(TString("_A2_DyDphi_shft_B2_1_2Bar"));
      corrNames.push_back(TString("_A2_DyDphi_shft_B2_12Sum"));
      outputPath = outputPathBase;
      outputPath += "/PiKP/Y10/A2Derived/";
      // B vs Y,Phi
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.4);
      // B vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.4);
      // B vs phi
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.24);
      // I vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 1.19);
      // Isum vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 1.19);
      // B width
      balFct_Minima.push_back( 0.00);
      balFct_Maxima.push_back( 1.19);
      }
    }

  for (int iType=2; iType<3; iType++)
    {

    for (int iSpecies2=0; iSpecies2<nSpecies; iSpecies2++)
      {
      outFileNameBase2 = outFileNameBase;
      outFileNameBase2 += (speciesNames[iSpecies2]);
      outFileNameBase2 += "_";
      histoInputFileNames.clear();
      balFct_Types.clear();
      balFct_DeltaYDeltaPhi_Histo_Names.clear();
      balFct_DeltaY_Histo_Names.clear();
      balFct_DeltaPhi_Histo_Names.clear();
      balFct_Output_Name = makeName(outFileNameBase,speciesNames[iSpecies2]);
      balFct_LegendConfigs.clear();

      // B2D
      LegendConfiguration* lc;
      lc = new LegendConfiguration(0.65, 0.85, 0.5, 0.9, 0.05);
      lc->setParameter("useLegend",false);
      lc->setParameter("useLabels",true);
      lc->setParameter("useTitles",false);
      lc->setParameter("textIndex",42);
      balFct_LegendConfigs.push_back(lc);
      // B vs Delta y
      lc = new LegendConfiguration(0.65, 0.85, 0.5, 0.9, 0.06);
      lc->setParameter("useLegend",true);
      lc->setParameter("useLabels",true);
      lc->setParameter("useTitles",false);
      lc->setParameter("textIndex",42);
      balFct_LegendConfigs.push_back(lc);
      // B vs Delta phi
      lc = new LegendConfiguration(0.65, 0.85, 0.5, 0.9, 0.06);
      lc->setParameter("useLegend",true);
      lc->setParameter("useLabels",true);
      lc->setParameter("useTitles",false);
      lc->setParameter("textIndex",42);
      balFct_LegendConfigs.push_back(lc);
      // I vs Delta y
      lc = new LegendConfiguration(0.2, 0.45, 0.5, 0.8, 0.06);
      lc->setParameter("useLegend",true);
      lc->setParameter("useLabels",true);
      lc->setParameter("useTitles",false);
      lc->setParameter("textIndex",42);
      balFct_LegendConfigs.push_back(lc);
      // Isum vs Delta y
      lc = new LegendConfiguration(0.2, 0.45, 0.5, 0.8, 0.06);
      lc->setParameter("useLegend",true);
      lc->setParameter("useLabels",true);
      lc->setParameter("useTitles",false);
      lc->setParameter("textIndex",42);
      balFct_LegendConfigs.push_back(lc);

      for (int iSpecies1=0; iSpecies1<nSpecies; iSpecies1++)
        {
        TString pairTitle     = speciesTitles[iSpecies1] + speciesTitles[iSpecies2];
        TString pairName      = (speciesNames[iSpecies1]+"_")+speciesNames[iSpecies2];
        TString pairNameCorr  = pairName+corrNames[iType];
        for (int k=0;k<5;k++)
          {
          balFct_LegendConfigs[k]->addLegend(pairTitle);
          }
        histoInputFileNames.push_back(               inFileName);
        balFct_Titles.push_back(                     titleBase+pairTitle);
        balFct_Types.push_back(                      iType);
        balFct_DeltaYDeltaPhi_Histo_Names.push_back( histoNameBase+pairNameCorr     );
        balFct_DeltaY_Histo_Names.push_back(         histoNameBase+pairNameCorr+"_x");
        balFct_DeltaPhi_Histo_Names.push_back(       histoNameBase+pairNameCorr+"_y");
        }
      plotSet(plotter,
              inputPath,
              histoInputFileNames,
              balFct_Types,
              balFct_DeltaYDeltaPhi_Histo_Names,balFct_DeltaY_Histo_Names,balFct_DeltaPhi_Histo_Names,
              balFct_LegendConfigs,
              balFct_Minima, balFct_Maxima,
              balFct_Output_Name,
              rebin,rapidityType,plot2D,plot1D,plot1DHighRes,plotWidths,plotIntegralVsLogY);
      }
    }

  if (doPrint) plotter->printAllCanvas(outputPath, printGif, printPdf, printSvg, printC);
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

