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
#include "DataGraph.hpp"
using CAP::DataGraph;

ClassImp(DataGraph);

DataGraph::DataGraph(const String _graphName,
                     const String _xTitle,
                     const String _yTitle,
                     const String _legendText,
                     double _xMin, double _xMax,
                     double _yMin, double _yMax,
                     int _n,
                     double * _x, double *_exLow,  double *_exHigh,
                     double * _y, double *_eyLow,  double *_eyHigh, double *_eeyLow, double *_eeyHigh, int _opt)
: TH1D(_graphName,_graphName, 100, _xMin, _xMax),
legendText(_legendText),
nPoints(_n),
x(_x),
exLow(_exLow),
exHigh(_exHigh),
y(_y),
eyLow(_eyLow),
eyHigh(_eyHigh),
eeyLow(_eeyLow),
eeyHigh(_eeyHigh),
option(_opt)
{
  Fill(-100.0);
  SetName(_graphName);
  SetXTitle(_xTitle);
  SetYTitle(_yTitle);
  SetMinimum(_yMin);
  SetMaximum(_yMax);
  TGraph * g;
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eyLow, eyHigh);
  graphs.push_back(g);
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eeyLow, eeyHigh);
  graphs.push_back(g);
  visible = true;
}

DataGraph::DataGraph(const String _graphName,
                     const String _xTitle,
                     const String _yTitle,
                     const String _legendText,
                     double _xMin, double _xMax,
                     double _yMin, double _yMax,
                     int _n,
                     double * _x, double *_exLow,  double *_exHigh,
                     double * _y, double *_eyLow,  double *_eyHigh,
                     int _opt)
: TH1D(_graphName,_graphName, 100, _xMin, _xMax),
legendText(_legendText),
nPoints(_n),
x(_x),
exLow(_exLow),
exHigh(_exHigh),
y(_y),
eyLow(_eyLow),
eyHigh(_eyHigh),
eeyLow(nullptr),
eeyHigh(nullptr),
option(_opt)
{
  Fill(-100.0);
  SetName(_graphName);
  SetXTitle(_xTitle);
  SetYTitle(_yTitle);
  SetMinimum(_yMin);
  SetMaximum(_yMax);
  TGraph * g;
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eyLow, eyHigh);
  graphs.push_back(g);
  visible = true;
}

DataGraph::DataGraph(const String _graphName,
                     const String _xTitle,
                     const String _yTitle,
                     const String _legendText,
                     double _xMin, double _xMax,
                     double _yMin, double _yMax,
                     TH1 * histo, int _opt)
: TH1D(_graphName,_graphName, 100, _xMin, _xMax),
legendText(_legendText),
nPoints(0),
x(nullptr),
exLow(nullptr),
exHigh(nullptr),
y(nullptr),
eyLow(nullptr),
eyHigh(nullptr),
eeyLow(nullptr),
eeyHigh(nullptr),
option(_opt)
{
  Fill(-100.0);
  SetName(_graphName);
  SetXTitle(_xTitle);
  SetYTitle(_yTitle);
  SetMinimum(_yMin);
  SetMaximum(_yMax);

  nPoints = histo->GetNbinsX();
  x       = new double[nPoints];
  exLow   = new double[nPoints];
  exHigh  = new double[nPoints];
  y       = new double[nPoints];
  eyLow   = new double[nPoints];
  eyHigh  = new double[nPoints];
  double w, ey;
  for (int k=0; k<nPoints; k++)
    {
    x[k] = histo->GetXaxis()->GetBinCenter(k+1);
    w = 0.5 * histo->GetXaxis()->GetBinWidth(k+1);
    exLow[k]  = w;
    exHigh[k] = w;
    y[k] = histo->GetBinContent(k+1);
    ey   = histo->GetBinError(k+1);
    eyLow[k]  = ey;
    eyHigh[k] = ey;
    }

  TGraph * g;
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eyLow, eyHigh);
  graphs.push_back(g);
  visible = true;
}

DataGraph::DataGraph(const String _graphName,
                     const String _xTitle,
                     const String _yTitle,
                     const String _legendText __attribute__((unused)),
                     double _xMin, double _xMax,
                     double _yMin, double _yMax,
                     TH1 * histo, double *_eeyLow, double *_eeyHigh,  int _opt)
: TH1D(_graphName,_graphName, 100, _xMin, _xMax),
nPoints(0),
x(nullptr),
exLow(nullptr),
exHigh(nullptr),
y(nullptr),
eyLow(nullptr),
eyHigh(nullptr),
eeyLow(_eeyLow),
eeyHigh(_eeyHigh),
option(_opt)
{
  Fill(-100.0);
  SetName(_graphName);
  SetXTitle(_xTitle);
  SetYTitle(_yTitle);
  SetMinimum(_yMin);
  SetMaximum(_yMax);

  nPoints = histo->GetNbinsX();
  x       = new double[nPoints];
  exLow   = new double[nPoints];
  exHigh  = new double[nPoints];
  y       = new double[nPoints];
  eyLow   = new double[nPoints];
  eyHigh  = new double[nPoints];
  double w, ey;
  for (int k=0; k<nPoints; k++)
    {
    x[k] = histo->GetXaxis()->GetBinCenter(k+1);
    w = 0.5 * histo->GetXaxis()->GetBinWidth(k+1);
    exLow[k]  = w;
    exHigh[k] = w;
    y[k] = histo->GetBinContent(k+1);
    ey   = histo->GetBinError(k+1);
    eyLow[k]  = ey;
    eyHigh[k] = ey;
    }

  TGraph * g;
  g = new TGraphAsymmErrors (nPoints, x, y, exLow, exHigh, eyLow, eyHigh);
  graphs.push_back(g);
  visible = true;
}



void DataGraph::draw(bool same)
{
  if (!same) Draw();

  String graphOption;
  switch (option)
    {
      default:
      case 0:
      if (graphs.size()>1)
        {
        graphOption = "E2"; //graphOption = ".SAME";
        graphs[1]->Draw(graphOption);
        graphOption = "P"; //if (same) graphOption = ".SAME";
        graphs[0]->Draw(graphOption);
        }
      else
        {
        graphOption = "P"; //if (same) graphOption = ".SAME";
        graphs[0]->Draw(graphOption);
        }
     break;
    }
}

void DataGraph::setMinMax(double minY, double maxY)
{
  SetMinimum(minY);
  SetMaximum(maxY);
}

void DataGraph::setProperties(const GraphConfiguration & graphConfig)
{
  //cout << "<I> DataGraph::setProperties() Setting properties of graph: " << graphs[0]->GetName() << endl;
  graphs[0]->SetLineColor(  graphConfig.getValueInt(getName(),"lineColor"));
  graphs[0]->SetLineStyle(  graphConfig.getValueInt(getName(),"lineStyle"));
  graphs[0]->SetLineWidth(  graphConfig.getValueInt(getName(),"lineWidth"));
  graphs[0]->SetMarkerColor(graphConfig.getValueInt(getName(),"markerColor"));
  graphs[0]->SetMarkerStyle(graphConfig.getValueInt(getName(),"markerStyle"));
  graphs[0]->SetMarkerSize( graphConfig.getValueDouble("markerSize"));
  graphs[0]->SetFillColor(0);
  if (graphs.size()>1)
    {
    graphs[1]->SetLineColor(  graphConfig.getValueInt(getName(),"systsColor"));
    graphs[1]->SetLineStyle(  graphConfig.getValueInt(getName(),"systsStyle"));
    graphs[1]->SetLineWidth(  graphConfig.getValueInt(getName(),"systsWidth"));
    graphs[1]->SetMarkerColor(graphConfig.getValueInt(getName(),"markerColor"));
    graphs[1]->SetMarkerStyle(1);
    graphs[1]->SetMarkerSize(0.1);
    graphs[1]->SetFillStyle(graphConfig.getValueInt(getName(),"systsStyle"));
    graphs[1]->SetFillColor(graphConfig.getValueInt(getName(),"systsColor"));
    }

  TAxis * xAxis = (TAxis *) GetXaxis();
  xAxis->SetNdivisions( graphConfig.getValueDouble("nXDivisions"));
  xAxis->SetTitleSize(  graphConfig.getValueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfig.getValueDouble("xTitleOffset"));
  xAxis->SetLabelSize(  graphConfig.getValueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfig.getValueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) GetYaxis();
  yAxis->SetNdivisions( graphConfig.getValueInt(getName(),"nYDivisions"));
  yAxis->SetTitleSize(  graphConfig.getValueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfig.getValueDouble("yTitleOffset"));
  yAxis->SetLabelSize(  graphConfig.getValueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfig.getValueDouble("yLabelOffset"));
  //cout << "<I> DataGraph::::setProperties() Completed" << endl;
}

void DataGraph::printToFile(ostream & output)
{
  output << "========================================================================================" << endl;
  output << "   Graph Name:" << GetName() << endl;
  output << "  Graph Title:" << GetName() << endl;
  output << "      nPoints:" << nPoints << endl;
  for (int k=0; k<nPoints; k++)
    {

    output << "x:" << x[k];
    if (exLow && !exHigh)  output << "+-" << exLow[k];
    if (exLow && exHigh)   output << "-" << exLow[k];
    if (exLow && exHigh)   output << "+" << exHigh[k];
    if (eyLow && !eyHigh)  output << " (stat) +- " << eyLow[k];
    if (eyLow && eyHigh)   output << " (stat) - " << eyLow[k];
    if (eeyLow && eyHigh)   output << " (stat) + " << eyHigh[k];
    if (eeyLow && !eeyHigh)  output << " (sys) +- " << eeyLow[k];
    if (eeyLow && eeyHigh)   output << " (sys) - " << eeyLow[k];
    if (eeyLow && eeyHigh)   output << " (sys) + " << eeyHigh[k];
    }
  output << "========================================================================================" << endl;
}


DataGraph *  DataGraph::loadGraph(const String  graphName,
                          const String  xTitle,
                          const String  yTitle,
                          const String  legendText,
                          double xMin, double xMax,
                          double yMin, double yMax,
                          TFile * inputFile, String folderName, String h1Name, String h1e1Name, String h1e2Name, String g1Name )
{
  if (!inputFile) return nullptr;

  String name;
  name = folderName; name += "/"; name += h1Name;
  TH1 * h1 = (TH1*)inputFile->Get(name);
  if (!h1)
    {
    cout << "<E> DataGraph::loadGraph() Could not load histogram: " << name << " from file:" << inputFile->GetName() << endl;
    return nullptr;
    }
  name = folderName; name += "/"; name += h1e1Name;
  TH1 * h1e1 = (TH1*)inputFile->Get(name);
  if (!h1e1)
    {
    cout << "<E> DataGraph::loadGraph() Could not load histogram called: " << name << " from file:" << inputFile->GetName() << endl;
    return nullptr;
    }
  name = folderName; name += "/"; name += h1e2Name;
  TH1 * h1e2 = (TH1*)inputFile->Get(name);
  if (!h1e2)
    {
    cout << "<E> DataGraph::loadGraph() Could not load histogram called: " << name << " from file:" << inputFile->GetName() << endl;
    return nullptr;
    }
  name = folderName; name += "/"; name += g1Name;
  TGraphAsymmErrors* g = (TGraphAsymmErrors*)inputFile->Get(name);
  if (!g)
    {
    cout << "<E> DataGraph::loadGraph() Could not load graph called: " << name << " from file:" << inputFile->GetName() << endl;
    return nullptr;
    }
  //cout << "<I> DataGraph::loadGraph() Data loaded.." << endl;

  int n = h1->GetNbinsX();
  double * x       = new double[n];
  double * exLow   = new double[n];
  double * exHigh  = new double[n];
  double * y       = new double[n];
  double * eyLow   = new double[n];
  double * eyHigh  = new double[n];
  double * eeyLow  = new double[n];
  double * eeyHigh = new double[n];
  for (int k=0; k<n; k++)
    {
    x[k]       = g->GetPointX(k);
    exLow[k]   = g->GetErrorXlow(k);
    exHigh[k]  = g->GetErrorXhigh(k);
    y[k]       = g->GetPointY(k);
    eyLow[k]   = g->GetErrorYlow(k);
    eyHigh[k]  = g->GetErrorYhigh(k);
    eeyLow[k]  = h1e1->GetBinContent(k);
    eeyHigh[k] = h1e2->GetBinContent(k);
    }
  DataGraph * dg = new DataGraph(graphName, xTitle, yTitle, legendText, xMin, xMax, yMin, yMax, n,
                               x, exLow, exHigh,y,eyLow,eyHigh,eeyLow,eeyHigh,0);
  return dg;
}

DataGraph * DataGraph::loadGraph(const String graphName,
                             const String xTitle,
                             const String yTitle,
                             const String legendText,
                             double xMin, double xMax,
                             double yMin, double yMax,
                             TFile * inputFile, String hName, double scale, int rebin)
{
  if (!inputFile) return nullptr;
  String name;
  name = hName;
  TH1 * histo = (TH1*)inputFile->Get(name);
  if (!histo)
    {
    cout << "<E> DataGraph::loadGraph() Could not load histogram: " << name << " from file:" << inputFile->GetName() << endl;
    return nullptr;
    }

  if (rebin>1)
    {
    histo->Rebin(rebin);
    histo->Scale(1.0/double(rebin));
    }


  int n = histo->GetNbinsX();
  double * x       = new double[n];
  double * exLow   = new double[n];
  double * exHigh  = new double[n];
  double * y       = new double[n];
  double * eyLow   = new double[n];
  double * eyHigh  = new double[n];
  double w, ey;
  for (int k=0; k<n; k++)
    {
    x[k] = histo->GetXaxis()->GetBinCenter(k+1);
    w = 0.5 * histo->GetXaxis()->GetBinWidth(k+1);
    exLow[k]  = w;
    exHigh[k] = w;
    y[k] = scale * histo->GetBinContent(k+1);
    ey   = scale * histo->GetBinError(k+1);
    eyLow[k]  = ey;
    eyHigh[k] = ey;
    }
  DataGraph * dg = new DataGraph(graphName, xTitle, yTitle, legendText, xMin, xMax, yMin, yMax, n,
                               x, exLow, exHigh,y,eyLow,eyHigh);
  return dg;
}
