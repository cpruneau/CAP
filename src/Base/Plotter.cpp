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
#include "Plotter.hpp"
ClassImp(Plotter);

Plotter::Plotter(const TString& _name,
                 const Configuration _configuration,
                 LogLevel _logLevel)
:
Task(_name,_configuration,_logLevel),
canvasCollection(),
histogramCollection("HISTOS",_logLevel)
{
  appendClassName("Plotter");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

Plotter::~Plotter()
{

}

TCanvas *  Plotter::plot(TH1 * h,
                         const TString & canvasName,
                         const CanvasConfiguration & cc,
                         const GraphConfiguration  & gc,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax,
                         const TString &  legendText,
                         double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,
                         double legendSize)
{
  if (reportDebug(__FUNCTION__)) cout << "Creating canvas named:" << canvasName << endl;
  TCanvas * canvas  = canvasCollection.createCanvas(canvasName,cc);
  setProperties(h,gc);
  h->GetXaxis()->SetTitle(xTitle);
  h->GetYaxis()->SetTitle(yTitle);
  h->SetMinimum(yMin);
  h->SetMaximum(yMax);
  h->GetXaxis()->SetRangeUser(xMin,xMax);
  h->DrawCopy(gc.getValueString("plotOption"));
  if (!legendText.IsNull()) createLegend(h,legendText,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,0, legendSize);
  return canvas;
}

TCanvas *  Plotter::plot(TH2 * h,
                         const TString & canvasName,
                         const CanvasConfiguration & cc,
                         const GraphConfiguration  & gc,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax,
                         const TString & zTitle,  double zMin, double zMax)
//,
//                         const TString & text1, double x1, double y1, int color1,  double fontSize1,
//                         const TString & text2, double x2, double y2, int color2,  double fontSize2)
{
  if (reportDebug(__FUNCTION__)) cout << "Creating canvas named:" << canvasName << endl;
  TCanvas * canvas  = canvasCollection.createCanvas(canvasName,cc);
  setProperties(h,gc);
  h->GetXaxis()->SetTitle(xTitle);
  h->GetYaxis()->SetTitle(yTitle);
  h->GetZaxis()->SetTitle(zTitle);

  if (xMin < xMax) h->GetXaxis()->SetRangeUser(xMin,xMax);
  if (yMin < yMax) h->GetYaxis()->SetRangeUser(yMin,yMax);
  if (zMin < zMax)
    {
    //cout << "Setting z range: " << zMin << " to: " << zMax << endl;
    h->SetMinimum(zMin);
    h->SetMaximum(zMax);
    }
  h->DrawCopy(gc.getValueString("plotOption"));
//  if (!text1.IsNull()) createLabel(text1, x1,y1,color1, fontSize1, true);
//  if (!text2.IsNull()) createLabel(text2, x2,y2,color2, fontSize2, true);
  return canvas;
}

void Plotter::findMinMax(TH1* histogram, double & minimum, double & maximum)
{
  minimum = histogram->GetBinContent(histogram->GetMinimumBin());
  maximum = histogram->GetBinContent(histogram->GetMaximumBin());
}

TCanvas *  Plotter::plot(vector<TH1*> histograms,
                         const vector<GraphConfiguration*> & graphConfigurations,
                         const vector<TString>  &  legendTexts,
                         const TString & canvasName,
                         const CanvasConfiguration & canvasConfiguration,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax,
                         double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,
                         double legendSize)
{
  if (reportDebug(__FUNCTION__)) cout << "Creating canvas named:" << canvasName << endl;
  TCanvas * canvas = canvasCollection.createCanvas(canvasName,canvasConfiguration);
  unsigned int nGraphs = histograms.size();
  TH1 * h;
  for (unsigned int iGraph=0; iGraph<nGraphs; iGraph++)
    {
    h = histograms[iGraph];
    setProperties(h,*(graphConfigurations[iGraph]) );
    h->GetXaxis()->SetTitle(xTitle);
    h->GetYaxis()->SetTitle(yTitle);
    if (yMin>=yMax) findMinMax(h,yMin,yMax);
    if (yMin>=yMax) yMax = yMin + 1.0E1;
    }
  h = histograms[0];
  h->SetMinimum(yMin);
  h->SetMaximum(yMax);
  if (xMin<xMax) h->GetXaxis()->SetRangeUser(xMin,xMax);
  TString plotOption = graphConfigurations[0]->getValueString("plotOption");
  h->DrawCopy(plotOption);
  for (unsigned int iGraph=1; iGraph<nGraphs; iGraph++)
    {
    plotOption = graphConfigurations[iGraph]->getValueString("plotOption");
    histograms[iGraph]->DrawCopy(plotOption+" SAME");
    }
  if (nGraphs<6)
    createLegend(histograms,legendTexts,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,legendSize);
  else
    {
    unsigned int n1 = nGraphs/2;
    unsigned int n2 = nGraphs - n1;
    vector<TH1*> h1;
    vector<TH1*> h2;
    vector<TString> lg1;
    vector<TString> lg2;
    for (unsigned int k=0; k<n1; k++)
      {
      h1.push_back( histograms[k]);
      lg1.push_back( legendTexts[k]);
      }
    for (unsigned int k=0; k<n2; k++)
      {
      h2.push_back( histograms[n1+k]);
      lg2.push_back( legendTexts[n1+k]);
      }
    createLegend(h1,lg1,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,legendSize);
    createLegend(h2,lg2,xMaxLeg, yMinLeg, 2.0*xMaxLeg-xMinLeg, yMaxLeg,legendSize);
    }

//  if (label1) createLabel(text1, x1,y1,color1,true);
//  if (label2) createLabel(text2, x2,y2,color2,true);

  return canvas;
}

////!
////! Function to plot n DataGraphs on a single canvas
////!
//TCanvas *  Plotter::plot(TString  canvasName, CanvasConfiguration * cc,
//                         TString  xTitle,  double xMin, double xMax,
//                         TString  yTitle,  double yMin, double yMax,
//                         vector<DataGraph*> graphs,
//                         double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,double legendSize)
//{
//  if (reportInfo("Plotter",getName(),"plot(..)")) cout << "Creating canvas named:" << canvasName << endl;
//  TCanvas * canvas = createCanvas(canvasName,*cc);
//  graphs[0]->setTitleX(xTitle);
//  graphs[0]->setTitleY(yTitle);
//  double min = -1.0;
//  double max =  1.0;
//  unsigned int nGraphs = graphs.size();
//  if (reportInfo("Plotter",getName(),"plot(..)")) cout << "nGraphs:" << nGraphs << endl;
//
//  if (yMin < yMax)
//    {
//    min = yMin;
//    max = yMax;
//    }
//  else if (yMin >= yMax)
//    {
//    min =  1.0E100;
//    max = -1.0E100;
//    }
//    cout << "Histo: " << graphs[0]->getName() << endl;
//
//  TString plotOption;
//  for (unsigned int iGraph=0; iGraph<nGraphs; iGraph++)
//    {
//    graphs[iGraph]->draw(iGraph>0);
//    }
//  if (nGraphs<6)
//    createLegend(graphs,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,0, legendSize);
//  else
//    {
//    unsigned int n1 = nGraphs/2;
//    unsigned int n2 = nGraphs - n1;
//    vector<DataGraph*>  h1;
//    vector<DataGraph*>  h2;
//    for (unsigned int k=0; k<n1; k++) h1.push_back( graphs[k]);
//    for (unsigned int k=0; k<n2; k++) h2.push_back( graphs[n1+k]);
//    createLegend(h1,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,0, legendSize);
//    createLegend(h2,xMaxLeg, yMinLeg, 2.0*xMaxLeg-xMinLeg, yMaxLeg,0, legendSize);
//    }
//  return canvas;
//}
//
//

TLatex * Plotter::createLabel(const TString & text, double x, double y, double angle, int color,  double fontSize, bool doDraw)
{
  if (reportDebug(__FUNCTION__)) cout << "Creating label with text:" << text << endl;
  TLatex * label;
  label = new TLatex(x,y,text);
  label->SetTextColor(color);
  label->SetTextAngle(angle);
  label->SetTextSize(fontSize);
  if (doDraw) label->Draw();
  return label;
}

TLegend * Plotter::createLegend(double x1, double y1, double x2, double y2, double fontSize)
{
  if (reportDebug(__FUNCTION__)) cout << "Create legend" << endl;
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  return legend;
}

TLegend * Plotter::createLegend(TH1*histogram, const TString & legendText, double x1, double y1, double x2, double y2, double fontSize, bool doDraw)
{
  if (reportDebug(__FUNCTION__)) cout << "Create legend for one histogram" << endl;
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  legend->AddEntry(histogram,legendText);
  if (doDraw) legend->Draw();
  return legend;
}

TLegend * Plotter::createLegend(vector<TH1*> histograms, vector<TString> legendTexts, double x1, double y1, double x2, double y2, double fontSize, bool doDraw)
{
  if (reportDebug(__FUNCTION__)) cout << "Create legend for several histogram" << endl;
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  for (unsigned int iGraph=0; iGraph<histograms.size(); iGraph++)
    {
    legend->AddEntry(histograms[iGraph],legendTexts[iGraph]);
    }
  if (doDraw) legend->Draw();
  return legend;
}

TLegend * Plotter::createLegend(vector<DataGraph*> graphs,double x1, double y1, double x2, double y2, double fontSize, bool doDraw)
{
  if (reportDebug(__FUNCTION__)) cout << "Create legend for several histogram" << endl;
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  for (unsigned int iGraph=0; iGraph<graphs.size(); iGraph++)
    {
    legend->AddEntry(graphs[iGraph]->getDataGraph(),graphs[iGraph]->getLegendText());
    }
  if (doDraw) legend->Draw();
  return legend;
}

////////////////////////////////////////////////////
// Create simple line
////////////////////////////////////////////////////
TLine * Plotter::createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw)
{
  if (reportDebug("Plotter",getName(),"createLine(..)"))
    ;
  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  if (doDraw) line->Draw();
  return line;
}

////////////////////////////////////////////////////
// Create Arrow Line
////////////////////////////////////////////////////
TArrow * Plotter::createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw)
{
  if (reportDebug("Plotter",getName(),"createArrow(..)"))
    ;
  TArrow *line = new TArrow(x1,y1,x2,y2,arrowSize,option);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  if (doDraw) line->Draw();
  return line;
}


////////////////////////////////////////////////////////////////////////
// Setting Histogram Properties
////////////////////////////////////////////////////////////////////////
void Plotter::setProperties(TH1 * h, const GraphConfiguration & graphConfiguration)
{
  if (reportDebug("Plotter",getName(),"setProperties(TH1 * h,..)"))
    {
    cout << "Setting properties of histo: " << h->GetName() << endl;
    }

  h->SetLineColor(graphConfiguration.getValueInt("lineColor"));
  h->SetLineStyle(graphConfiguration.getValueInt("lineStyle"));
  h->SetLineWidth(graphConfiguration.getValueInt("lineWidth"));
  h->SetMarkerColor(graphConfiguration.getValueInt("markerColor"));
  h->SetMarkerStyle(graphConfiguration.getValueInt("markerStyle"));
  h->SetMarkerSize (graphConfiguration.getValueDouble("markerSize"));
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.getValueInt("nXDivisions"));
  xAxis->SetTitleSize(graphConfiguration.getValueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfiguration.getValueDouble("xTitleOffset"));
  xAxis->SetLabelSize(graphConfiguration.getValueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfiguration.getValueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.getValueInt("nYDivisions"));
  yAxis->SetTitleSize(graphConfiguration.getValueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfiguration.getValueDouble("yTitleOffset"));
  yAxis->SetLabelSize(graphConfiguration.getValueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfiguration.getValueDouble("yLabelOffset"));
  if (h->IsA() == TH2::Class()  || h->IsA() == TH2F::Class() || h->IsA() == TH2F::Class() )
    {
    if (reportDebug(__FUNCTION__)) cout << "Setting properties as 2D histo: " << h->GetTitle() << endl;
    TAxis * zAxis = (TAxis *) h->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.getValueInt("nZDivisions"));
    zAxis->SetTitleSize(graphConfiguration.getValueDouble("zTitleSize"));
    zAxis->SetTitleOffset(graphConfiguration.getValueDouble("zTitleOffset"));
    zAxis->SetLabelSize(graphConfiguration.getValueDouble("zLabelSize"));
    zAxis->SetLabelOffset(graphConfiguration.getValueDouble("zLabelOffset"));
    }
}

void Plotter::setProperties(TH1 * h, const GraphConfiguration & graphConfiguration,
                            const TString & xTitle, const TString & yTitle, const TString & zTitle)
{
  if (reportDebug("Plotter",getName(),"setProperties(TH1 * h,..)"))
    {
    cout << "Setting properties of histo: " << h->GetTitle() << endl;
    }
  setProperties(h, graphConfiguration);
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetTitle(xTitle);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetTitle(yTitle);
  if (h->IsA() == TH2::Class()  || h->IsA() == TH2F::Class() || h->IsA() == TH2F::Class() )
    {
    TAxis * zAxis = (TAxis *) h->GetZaxis();
    zAxis->SetTitle(zTitle);
    }
}

void Plotter::setProperties(TGraph * g, const GraphConfiguration & graphConfiguration)
{
  if (reportDebug("Plotter",getName(),"setProperties(TGraph * g,..)"))
    {
    cout << "Setting properties of graph "<< g->GetTitle()  << endl;
    }
  g->SetLineColor(graphConfiguration.getValueInt("lineColor"));
  g->SetLineStyle(graphConfiguration.getValueInt("lineStyle"));
  g->SetLineWidth(graphConfiguration.getValueInt("lineWidth"));
  g->SetMarkerColor(graphConfiguration.getValueInt("markerColor"));
  g->SetMarkerStyle(graphConfiguration.getValueInt("markerStyle"));
  g->SetMarkerSize (graphConfiguration.getValueDouble("markerSize"));
  TAxis * xAxis = (TAxis *) g->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.getValueInt("nXDivisions"));
  xAxis->SetTitleSize(graphConfiguration.getValueDouble("xTitleSize"));
  xAxis->SetTitleOffset(graphConfiguration.getValueDouble("xTitleOffset"));
  xAxis->SetLabelSize(graphConfiguration.getValueDouble("xLabelSize"));
  xAxis->SetLabelOffset(graphConfiguration.getValueDouble("xLabelOffset"));
  TAxis * yAxis = (TAxis *) g->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.getValueInt("nYDivisions"));
  yAxis->SetTitleSize(graphConfiguration.getValueDouble("yTitleSize"));
  yAxis->SetTitleOffset(graphConfiguration.getValueDouble("yTitleOffset"));
  yAxis->SetLabelSize(graphConfiguration.getValueDouble("yLabelSize"));
  yAxis->SetLabelOffset(graphConfiguration.getValueDouble("yLabelOffset"));
}


void Plotter::setProperties(TGraph * g, const GraphConfiguration & graphConfiguration, const TString & xTitle, const TString & yTitle)
{
  if (reportDebug("Plotter",getName(),"setProperties(TGraph * g,..)"))
    {
    cout << "Setting properties of graph " << g->GetTitle() << endl;
    }
  setProperties(g,graphConfiguration);
  if (reportDebug(__FUNCTION__)) cout << "Setting properties of histo: " << g->GetTitle() << endl;
  TAxis * xAxis = (TAxis *) g->GetXaxis();
  xAxis->SetTitle(xTitle);
  TAxis * yAxis = (TAxis *) g->GetYaxis();
  yAxis->SetTitle(yTitle);
}


// ======================================================================
// Set Default Style for Plots
// ======================================================================
void Plotter::setDefaultOptions(bool color)
{
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);

}
