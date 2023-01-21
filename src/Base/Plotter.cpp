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

Plotter::Plotter(const TString & _name,
                 Configuration & _configuration)
:
Task(_name,_configuration),
canvasCollection(),
histogramCollection("HistoS",Severity::Info)
{
  appendClassName("Plotter");
}

void Plotter::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  bool YES = true;
  bool NO  = false;
  addParameter("DoPrint",                NO);
  addParameter("DoPrintGif",             NO);
  addParameter("DoPrintPdf",             NO);
  addParameter("DoPrintSvg",             NO);
  addParameter("DoPrintPng",             YES);
  addParameter("UseColor",               YES);
}

//!
//! Configure  this analysis task
//!
void Plotter::configure()
{
  setDefaultConfiguration();
  setConfiguration(requestedConfiguration);
  //configuration.printConfiguration(cout);
  MessageLogger::Severity selectedLevel = MessageLogger::Debug;
  TString reportLevel                   = getValueBool("Severity");
  if (reportLevel.EqualTo("Debug")) selectedLevel = MessageLogger::Debug;
  if (reportLevel.EqualTo("Info"))  selectedLevel = MessageLogger::Info;
}


TCanvas *  Plotter::plot(TH1 * h,
                         const TString & canvasName,
                         const CanvasConfiguration & cc,
                         const GraphConfiguration  & gc,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax,
                         const TString & legendText,
                         double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,
                         double legendSize)
{
  if (reportDebug(__FUNCTION__)) cout << "Creating canvas named:" << canvasName << endl;
  TCanvas * canvas  = canvasCollection.createCanvas(canvasName,cc);
  canvas->SetTicky(1);
  canvas->SetTickx(1);
  setProperties(h,gc);
  h->GetXaxis()->SetTitle(xTitle);
  h->GetYaxis()->SetTitle(yTitle);
  h->SetMinimum(yMin);
  h->SetMaximum(yMax);
  h->GetXaxis()->SetRangeUser(xMin,xMax);
  h->DrawCopy(gc.getValueString("PlotOption"));
//  TString plotOption = graphConfigurations[0]->getValueString("PlotOption");
//  h->DrawCopy(plotOption);
  if (!legendText.IsNull() && legendSize>0) createLegend(h,legendText,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,0, legendSize);
  return canvas;
}

TCanvas *  Plotter::plot(TH2 * h,
                         const TString & canvasName,
                         const CanvasConfiguration & cc,
                         const GraphConfiguration  & gc,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax,
                         const TString & zTitle,  double zMin, double zMax)
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
  h->DrawCopy(gc.getValueString("PlotOption"));
//  if (!text1.IsNull()) createLabel(text1, x1,y1,color1, fontSize1, true);
//  if (!text2.IsNull()) createLabel(text2, x2,y2,color2, fontSize2, true);
  return canvas;
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
  canvas->SetTicky(1);
  canvas->SetTickx(1);
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
  TString plotOption = graphConfigurations[0]->getValueString("PlotOption");
  h->DrawCopy(plotOption);
  for (unsigned int iGraph=1; iGraph<nGraphs; iGraph++)
    {
    plotOption = graphConfigurations[iGraph]->getValueString("PlotOption");
    histograms[iGraph]->DrawCopy(plotOption+" SAME");
    }
  if (nGraphs<6 && legendSize>0)
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
    if (legendSize>0)
      {
      createLegend(h1,lg1,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,legendSize);
      createLegend(h2,lg2,xMaxLeg, yMinLeg, 2.0*xMaxLeg-xMinLeg, yMaxLeg,legendSize);
      }
    }

//  if (label1) createLabel(text1, x1,y1,color1,true);
//  if (label2) createLabel(text2, x2,y2,color2,true);

  return canvas;
}



TCanvas *  Plotter::plot(vector<TGraph*> graphs,
                         const vector<GraphConfiguration*> & graphConfigurations,
                         const vector<TString>  &  legendTexts,
                         const TString & canvasName,
                         const CanvasConfiguration & canvasConfiguration,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax,
                         double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,
                         double legendSize)
{
  if (reportInfo(__FUNCTION__)) cout << "Creating canvas named:" << canvasName << endl;
  TCanvas * canvas = canvasCollection.createCanvas(canvasName,canvasConfiguration);
  unsigned int nGraphs = graphs.size();
  TGraph * h;
  canvas->SetTicky(1);
  canvas->SetTickx(1);
  if (reportInfo(__FUNCTION__)) cout << "nGraphs:" << nGraphs << endl;

  for (unsigned int iGraph=0; iGraph<nGraphs; iGraph++)
    {
    if (reportInfo(__FUNCTION__)) cout << "iGraph:" << iGraph << endl;
    h = graphs[iGraph];
    setProperties(h,*(graphConfigurations[iGraph]) );
    h->GetXaxis()->SetTitle(xTitle);
    h->GetYaxis()->SetTitle(yTitle);
//    if (yMin>=yMax) findMinMax(h,yMin,yMax);
//    if (yMin>=yMax) yMax = yMin + 1.0E1;
    }
  h = graphs[0];
  h->SetMinimum(yMin);
  h->SetMaximum(yMax);
  if (xMin<xMax) h->GetXaxis()->SetLimits(xMin,xMax);
  //if (xMin<xMax) h->GetXaxis()->SetRangeUser(xMin,xMax);
  TString plotOption = "ALP"; // graphConfigurations[0]->getValueString(getName(),"PlotOption");
  h->Draw(plotOption);
  //nGraphs = 1;
  for (unsigned int iGraph=1; iGraph<nGraphs; iGraph++)
    {
    plotOption = "SAME LP"; //graphConfigurations[iGraph]->getValueString(getName(),"PlotOption");
    graphs[iGraph]->Draw(plotOption);
    }
  if (nGraphs<6 && legendSize>0)
    createLegend(graphs,legendTexts,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,legendSize);
  else
    {
    unsigned int n1 = nGraphs/2;
    unsigned int n2 = nGraphs - n1;
    vector<TGraph*> h1;
    vector<TGraph*> h2;
    vector<TString> lg1;
    vector<TString> lg2;
    for (unsigned int k=0; k<n1; k++)
      {
      h1.push_back( graphs[k]);
      lg1.push_back( legendTexts[k]);
      }
    for (unsigned int k=0; k<n2; k++)
      {
      h2.push_back( graphs[n1+k]);
      lg2.push_back( legendTexts[n1+k]);
      }
    if (legendSize>0)
      {
      createLegend(h1,lg1,xMinLeg, yMinLeg, xMaxLeg, yMaxLeg,legendSize);
      createLegend(h2,lg2,xMaxLeg, yMinLeg, 2.0*xMaxLeg-xMinLeg, yMaxLeg,legendSize);
      }
    }

  //  if (label1) createLabel(text1, x1,y1,color1,true);
  //  if (label2) createLabel(text2, x2,y2,color2,true);

  return canvas;
}

//// new stuff
///
TCanvas *  Plotter::plot(const TString & canvasName,
                         const CanvasConfiguration  & cc,
                         const GraphConfiguration   & gc,
                         LegendConfiguration  & lc,
                         TH1 * h,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax)
{
  if (reportDebug(__FUNCTION__)) cout << "Creating canvas named:" << canvasName << endl;
  TCanvas * canvas  = canvasCollection.createCanvas(canvasName,cc);
  canvas->SetTicky(1);
  canvas->SetTickx(1);
  setProperties(h,gc);
  h->GetXaxis()->SetTitle(xTitle);
  h->GetYaxis()->SetTitle(yTitle);
  h->SetMinimum(yMin);
  h->SetMaximum(yMax);
  if (xMin<xMax) h->GetXaxis()->SetRangeUser(xMin,xMax);
  TString plotOption = gc.getValueString("PlotOption");
  h->DrawCopy(plotOption);

  if (lc.useLegend()) createLegend(h,lc);
  if (lc.useLabels()) lc.drawLabels();
  return canvas;
}

TCanvas *  Plotter::plot(const TString & canvasName,
                         const CanvasConfiguration  & cc,
                         const GraphConfiguration   & gc,
                         LegendConfiguration  & lc,
                         TH2 * h,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax,
                         const TString & zTitle,  double zMin, double zMax)
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
    h->SetMinimum(zMin);
    h->SetMaximum(zMax);
    }
  h->DrawCopy(gc.getValueString("PlotOption"));
  if (lc.useLegend()) createLegend(h,lc);
  if (lc.useLabels()) lc.drawLabels();
  return canvas;
}


TCanvas *  Plotter::plot(const TString & canvasName,
                         const CanvasConfiguration  & cc,
                         const vector<GraphConfiguration*> & gc,
                         LegendConfiguration  & lc,
                         vector<TH1*> histograms,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax)
{
  if (reportDebug(__FUNCTION__)) cout << "Creating canvas named:" << canvasName << endl;
  TCanvas * canvas = canvasCollection.createCanvas(canvasName,cc);
  canvas->SetTicky(1);
  canvas->SetTickx(1);
  TH1 * h;
  unsigned int nGraphs = histograms.size();
  for (unsigned int iGraph=0; iGraph<nGraphs; iGraph++)
    {
    h = histograms[iGraph];
    setProperties(h,*gc[iGraph]);
    h->GetXaxis()->SetTitle(xTitle);
    h->GetYaxis()->SetTitle(yTitle);
    if (yMin>=yMax) findMinMax(h,yMin,yMax);
    if (yMin>=yMax) yMax = yMin + 1.0E1;
    }
  h = histograms[0];
  h->SetMinimum(yMin);
  h->SetMaximum(yMax);
  if (xMin<xMax) h->GetXaxis()->SetRangeUser(xMin,xMax);
  TString plotOption = gc[0]->getValueString("PlotOption");
  h->DrawCopy(plotOption);
  for (unsigned int iGraph=1; iGraph<nGraphs; iGraph++)
    {
    plotOption = gc[iGraph]->getValueString("PlotOption");
    histograms[iGraph]->DrawCopy(plotOption+" SAME");
    }
  if (lc.useLegend()) createLegend(histograms,lc);
  if (lc.useLabels()) lc.drawLabels();
  return canvas;
}

TCanvas *  Plotter::plot(const TString & canvasName,
                         const CanvasConfiguration  & cc,
                         const vector<GraphConfiguration*> & gc,
                         LegendConfiguration  & lc,
                         vector<TGraph*> graphs,
                         const TString & xTitle,  double xMin, double xMax,
                         const TString & yTitle,  double yMin, double yMax)
{
  if (reportDebug(__FUNCTION__)) cout << "Creating canvas named:" << canvasName << endl;
  TCanvas * canvas = canvasCollection.createCanvas(canvasName,cc);
  TGraph * h;
  canvas->SetTicky(1);
  canvas->SetTickx(1);
  unsigned int nGraphs = graphs.size();
  //if (reportInfo(__FUNCTION__)) cout << "nGraphs:" << nGraphs << endl;

  for (unsigned int iGraph=0; iGraph<nGraphs; iGraph++)
    {
    //if (reportInfo(__FUNCTION__)) cout << "iGraph:" << iGraph << endl;
    h = graphs[iGraph];
    setProperties(h,*gc[iGraph]);
    h->GetXaxis()->SetTitle(xTitle);
    h->GetYaxis()->SetTitle(yTitle);
     }
  h = graphs[0];
  h->SetMinimum(yMin);
  h->SetMaximum(yMax);
  if (xMin<xMax) h->GetXaxis()->SetLimits(xMin,xMax);
  TString plotOption = "ALP";
  h->Draw(plotOption);
  for (unsigned int iGraph=1; iGraph<nGraphs; iGraph++)
    {
    plotOption = "SAME LP";
    graphs[iGraph]->Draw(plotOption);
    }
  if (lc.useLegend()) createLegend(graphs,lc);
  if (lc.useLabels()) lc.drawLabels();
  return canvas;
}

void Plotter::printAllCanvas(const TString & outputPath, bool printGif, bool printPdf, bool printSvg, bool printPng, bool printC)
{
  canvasCollection.printAllCanvas(outputPath, printGif, printPdf, printSvg, printPng, printC);
}

void Plotter::printAllCanvas(const TString & outputPath)
{
  bool printGif = getValueBool("getVa");
  bool printPdf = getValueBool("DoPrintPdf");
  bool printSvg = getValueBool("DoPrintSvg");
  bool printPng = getValueBool("DoPrintPng");
  bool printC   = getValueBool("DoPrintC");
  canvasCollection.printAllCanvas(outputPath, printGif, printPdf, printSvg, printPng, printC);
}



void Plotter::findMinMax(TH1* histogram, double & minimum, double & maximum)
{
  minimum = histogram->GetBinContent(histogram->GetMinimumBin());
  maximum = histogram->GetBinContent(histogram->GetMaximumBin());
}


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
  if (reportDebug(__FUNCTION__)) cout << "Create legend for several histograms" << endl;
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

TLegend * Plotter::createLegend(vector<TGraph*> graphs,vector<TString> legendTexts,double x1, double y1, double x2, double y2, double fontSize, bool doDraw)
{
  if (reportDebug(__FUNCTION__)) cout << "Create legend for several graphs" << endl;
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  for (unsigned int iGraph=0; iGraph<graphs.size(); iGraph++)
    {
    legend->AddEntry(graphs[iGraph],legendTexts[iGraph]);
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

TLegend * Plotter::createLegend(const LegendConfiguration & legendConfig)
{
  int    nColumns    = legendConfig.getValueInt("nColumns");
  int    borderColor = legendConfig.getValueInt("borderColor");
  int    borderSize  = legendConfig.getValueInt("borderSize");
  int    fillColor   = legendConfig.getValueInt("fillColor");
  int    fillStyle   = legendConfig.getValueInt("fillStyle");
  int    fontIndex   = legendConfig.getValueInt("textIndex");
  int    textColor   = legendConfig.getValueInt("textColor");
  double textSize    = legendConfig.getValueDouble("textSize");
  int    textAlign   = legendConfig.getValueInt("textAlign");
  double xLeft       = legendConfig.getValueDouble("xLeft");
  double xRight      = legendConfig.getValueDouble("xRight");
  double yLow        = legendConfig.getValueDouble("yLow");
  double yHigh       = legendConfig.getValueDouble("yHigh");
  TString header     = legendConfig.getValueString("header");
  TLegend *legend = new TLegend(xLeft,yLow,xRight,yHigh);
  if (nColumns>1) legend->SetNColumns(nColumns);
  legend->SetLineColor(borderColor);
  legend->SetBorderSize(borderSize);
  legend->SetFillColor(fillColor);
  legend->SetFillStyle(fillStyle);
  legend->SetTextFont(fontIndex);
  legend->SetTextColor(textColor);
  legend->SetTextSize(textSize);
  legend->SetTextAlign(textAlign);
  if (header.Length()>0) legend->SetHeader(header);
  return legend;
}

TLegend * Plotter::createLegend(TH1*histogram, const LegendConfiguration & legendConfig)
{
  TLegend * legend = createLegend(legendConfig);
  if (legendConfig.getValueBool("useTitles"))
    legend->AddEntry(histogram,histogram->GetTitle());
  else
    legend->AddEntry(histogram,legendConfig.getLegendAt(0));
  if (legendConfig.getValueBool("drawLegend")) legend->Draw();
  return legend;
}


TLegend * Plotter::createLegend(vector<TH1*> histograms,const LegendConfiguration & legendConfig)
{
  TLegend * legend = createLegend(legendConfig);
  for (unsigned int iGraph=0; iGraph<histograms.size(); iGraph++)
    {
    if (legendConfig.getValueBool("useTitles"))
      {
      //cout << "Using histo title : " << histograms[iGraph]->GetTitle()  << endl;
      legend->AddEntry(histograms[iGraph],histograms[iGraph]->GetTitle());
      }
    else
      {
      //cout << "Using external legend  : " << legendConfig.getLegendAt(iGraph) << endl;
      legend->AddEntry(histograms[iGraph],legendConfig.getLegendAt(iGraph));
      }
    }
  if (legendConfig.getValueBool("drawLegend")) legend->Draw();
  return legend;
}

TLegend * Plotter::createLegend(TGraph*graph, const LegendConfiguration & legendConfig)
{
  TLegend * legend = createLegend(legendConfig);
  if (legendConfig.getValueBool("useTitles"))
    legend->AddEntry(graph,graph->GetTitle());
  else
    legend->AddEntry(graph,legendConfig.getLegendAt(0));
  if (legendConfig.getValueBool("drawLegend")) legend->Draw();
  return legend;
}

TLegend * Plotter::createLegend(vector<TGraph*> graphs,const LegendConfiguration & legendConfig)
{
  TLegend * legend = createLegend(legendConfig);
  for (unsigned int iGraph=0; iGraph<graphs.size(); iGraph++)
    {
    if (legendConfig.getValueBool("useTitles"))
      legend->AddEntry(graphs[iGraph],graphs[iGraph]->GetTitle());
    else
      legend->AddEntry(graphs[iGraph],legendConfig.getLegendAt(iGraph));
    }
  if (legendConfig.getValueBool("drawLegend")) legend->Draw();
  return legend;
}

TLegend * Plotter::createLegend(vector<DataGraph*> graphs,const LegendConfiguration & legendConfig)
{
  TLegend * legend = createLegend(legendConfig);
  for (unsigned int iGraph=0; iGraph<graphs.size(); iGraph++)
    {
    if (legendConfig.getValueBool("useTitles"))
      legend->AddEntry(graphs[iGraph]->getDataGraph(),graphs[iGraph]->getLegendText());
    else
      legend->AddEntry(graphs[iGraph]->getDataGraph(),legendConfig.getLegendAt(iGraph));
    }
  if (legendConfig.getValueBool("drawLegend")) legend->Draw();
  return legend;
}


////////////////////////////////////////////////////
// Create simple line
////////////////////////////////////////////////////
TLine * Plotter::createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw)
{
  if (reportDebug(__FUNCTION__))
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
  if (reportDebug(__FUNCTION__))
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
  if (reportDebug(__FUNCTION__))
    {
    cout << "Setting properties of histo: " << h->GetName() << endl;
    }
  if (true)
    {
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
  else
    {
    h->SetLineColor(graphConfiguration.getValueInt(getName(),"lineColor"));
    h->SetLineStyle(graphConfiguration.getValueInt(getName(),"lineStyle"));
    h->SetLineWidth(graphConfiguration.getValueInt(getName(),"lineWidth"));
    h->SetMarkerColor(graphConfiguration.getValueInt(getName(),"markerColor"));
    h->SetMarkerStyle(graphConfiguration.getValueInt(getName(),"markerStyle"));
    h->SetMarkerSize (graphConfiguration.getValueDouble(getName(),"markerSize"));
    TAxis * xAxis = (TAxis *) h->GetXaxis();
    xAxis->SetNdivisions(graphConfiguration.getValueInt(getName(),"nXDivisions"));
    xAxis->SetTitleSize(graphConfiguration.getValueDouble(getName(),"xTitleSize"));
    xAxis->SetTitleOffset(graphConfiguration.getValueDouble(getName(),"xTitleOffset"));
    xAxis->SetLabelSize(graphConfiguration.getValueDouble(getName(),"xLabelSize"));
    xAxis->SetLabelOffset(graphConfiguration.getValueDouble(getName(),"xLabelOffset"));
    TAxis * yAxis = (TAxis *) h->GetYaxis();
    yAxis->SetNdivisions(graphConfiguration.getValueInt(getName(),"nYDivisions"));
    yAxis->SetTitleSize(graphConfiguration.getValueDouble(getName(),"yTitleSize"));
    yAxis->SetTitleOffset(graphConfiguration.getValueDouble(getName(),"yTitleOffset"));
    yAxis->SetLabelSize(graphConfiguration.getValueDouble(getName(),"yLabelSize"));
    yAxis->SetLabelOffset(graphConfiguration.getValueDouble(getName(),"yLabelOffset"));
    if (h->IsA() == TH2::Class()  || h->IsA() == TH2F::Class() || h->IsA() == TH2F::Class() )
      {
      if (reportDebug(__FUNCTION__)) cout << "Setting properties as 2D histo: " << h->GetTitle() << endl;
      TAxis * zAxis = (TAxis *) h->GetZaxis();
      zAxis->SetNdivisions(graphConfiguration.getValueInt(getName(),"nZDivisions"));
      zAxis->SetTitleSize(graphConfiguration.getValueDouble(getName(),"zTitleSize"));
      zAxis->SetTitleOffset(graphConfiguration.getValueDouble(getName(),"zTitleOffset"));
      zAxis->SetLabelSize(graphConfiguration.getValueDouble(getName(),"zLabelSize"));
      zAxis->SetLabelOffset(graphConfiguration.getValueDouble(getName(),"zLabelOffset"));
      }
    }
}

void Plotter::setProperties(TH1 * h, const GraphConfiguration & graphConfiguration,
                            const TString & xTitle, const TString & yTitle, const TString & zTitle)
{
  if (reportDebug(__FUNCTION__))
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
  if (reportDebug(__FUNCTION__))
    {
    cout << "Setting properties of graph "<< g->GetTitle()  << endl;
    }
  if (true)
    {
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
  else
    {
    g->SetLineColor(graphConfiguration.getValueInt(getName(),"lineColor"));
    g->SetLineStyle(graphConfiguration.getValueInt(getName(),"lineStyle"));
    g->SetLineWidth(graphConfiguration.getValueInt(getName(),"lineWidth"));
    g->SetMarkerColor(graphConfiguration.getValueInt(getName(),"markerColor"));
    g->SetMarkerStyle(graphConfiguration.getValueInt(getName(),"markerStyle"));
    g->SetMarkerSize (graphConfiguration.getValueDouble(getName(),"markerSize"));
    TAxis * xAxis = (TAxis *) g->GetXaxis();
    xAxis->SetNdivisions(graphConfiguration.getValueInt(getName(),"nXDivisions"));
    xAxis->SetTitleSize(graphConfiguration.getValueDouble(getName(),"xTitleSize"));
    xAxis->SetTitleOffset(graphConfiguration.getValueDouble(getName(),"xTitleOffset"));
    xAxis->SetLabelSize(graphConfiguration.getValueDouble(getName(),"xLabelSize"));
    xAxis->SetLabelOffset(graphConfiguration.getValueDouble(getName(),"xLabelOffset"));
    TAxis * yAxis = (TAxis *) g->GetYaxis();
    yAxis->SetNdivisions(graphConfiguration.getValueInt(getName(),"nYDivisions"));
    yAxis->SetTitleSize(graphConfiguration.getValueDouble(getName(),"yTitleSize"));
    yAxis->SetTitleOffset(graphConfiguration.getValueDouble(getName(),"yTitleOffset"));
    yAxis->SetLabelSize(graphConfiguration.getValueDouble(getName(),"yLabelSize"));
    yAxis->SetLabelOffset(graphConfiguration.getValueDouble(getName(),"yLabelOffset"));
    }

}


void Plotter::setProperties(TGraph * g, const GraphConfiguration & graphConfiguration, const TString & xTitle, const TString & yTitle)
{
  if (reportInfo(__FUNCTION__))
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
