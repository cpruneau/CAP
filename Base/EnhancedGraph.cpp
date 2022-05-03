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
#include "EnhancedGraph.hpp"

ClassImp(EnhancedGraph);

EnhancedGraph::EnhancedGraph(const TString & graphName,
              const TString & xTitle,
              const TString & yTitle,
              double minX, double maxX,
              double minY, double maxY,
              int n, double * x, double *y, double *ex, double *ey, int opt)
: TH1D(graphName,graphName, 1, minX, maxX),
nPoints(n),
xPtr(x),
exPtr(ex),
yPtr(y),
eyPtr(ey),
ySysMPtr(0),
ySysPPtr(0),
yM(0),
yP(0),
sysAbsolute(false),
option(opt)
{
  SetXTitle(xTitle);
  SetYTitle(yTitle);
  SetMinimum(minY);
  SetMaximum(maxY);
  g = new TGraphErrors(n, x, y, ex, ey);
  gP = 0;
  gM = 0;
  gSys = 0;

  isVisible = true;
}

EnhancedGraph::EnhancedGraph(const TString & graphName,
              const TString & xTitle,
              const TString & yTitle,
              double minX, double maxX,
              double minY, double maxY,
              int n, double * x, double *y, double *ex, double *ey,
              double * yM, double * yP,
              int opt,
              bool absolute)
: TH1D(graphName,graphName, 100, minX, maxX),
nPoints(n),
xPtr(x),
exPtr(ex),
yPtr(y),
eyPtr(ey),
ySysMPtr(yM),
ySysPPtr(yP),
yM(0),
yP(0),
sysAbsolute(absolute),
option(opt)
{
  SetXTitle(xTitle);
  SetYTitle(yTitle);
  SetMinimum(minY);
  SetMaximum(maxY);
  // graph w/ stat error
  g = new TGraphErrors(n, x, y, ex, ey);

  switch (option)
  {
    case 0:
    yM = new double[nPoints];
    yP = new double[nPoints];
    for (int k=0; k<n; k++)
      {
      yM[k] = y[k] + ySysMPtr[k];
      yP[k] = y[k] + ySysPPtr[k];
      }
    gP = new TGraph(n, x, yP);
    gM = new TGraph(n, x, yM);
    gSys = 0;
    break;
    case 1:
    gP = 0;
    gM = 0;
    yM = new double[nPoints];
    yP = new double[nPoints];
    for (int k=0; k<n; k++)
      {
      yM[k] = ySysMPtr[k];
      yP[k] = ySysPPtr[k];
      }
    gSys = new TGraphAsymmErrors(n, x, y, ex, ex, yM, yP);
    break;
  }

  isVisible = true;
}


void EnhancedGraph::draw(bool same)
{
  if (!same) Draw();

  switch (option)
  {
    case 0:
    g->Draw("P");
    if (gP && gM)
      {
      gP->Draw("L");
      gM->Draw("L");
      }
    break;
    case 1:
    //gSys->SetFillColorAlpha(kYellow,0.5);
    gSys->Draw("P.e2");
    g->Draw("P");
    break;
  }
}

void EnhancedGraph::setMinMax(double minY, double maxY)
{
  SetMinimum(minY);
  SetMaximum(maxY);
}

void EnhancedGraph::setProperties(const GraphConfiguration & graphConfig)
{
  cout << "-INFO- EnhancedGraph::setProperties() Setting properties of graph: " << g->GetName() << endl;
  g->SetLineColor(graphConfig.lineColor);
  g->SetLineStyle(graphConfig.lineStyle);
  g->SetLineWidth(graphConfig.lineWidth);
  g->SetMarkerColor(graphConfig.markerColor);
  g->SetMarkerStyle(graphConfig.markerStyle);
  g->SetMarkerSize(graphConfig.markerSize);
  g->SetFillColor(0);

  if (gP)
    {
    gP->SetLineColor(graphConfig.lineColor);
    gP->SetLineStyle(graphConfig.lineStyle);
    gP->SetLineWidth(graphConfig.lineWidth);
    gM->SetLineColor(graphConfig.lineColor);
    gM->SetLineStyle(graphConfig.lineStyle);
    gM->SetLineWidth(graphConfig.lineWidth);

    }

  if (gSys)
    {
    gSys->SetLineColor(graphConfig.lineColor);
    gSys->SetLineStyle(graphConfig.lineStyle);
    gSys->SetLineWidth(graphConfig.lineWidth);
    gSys->SetMarkerColorAlpha(graphConfig.markerColor,0.1);
    gSys->SetMarkerStyle(graphConfig.markerStyle);
    gSys->SetMarkerSize(graphConfig.markerSize);
    gSys->SetFillColorAlpha(graphConfig.markerColor,0.3);
    }


  TAxis * xAxis = (TAxis *) GetXaxis();
  xAxis->SetNdivisions(graphConfig.nXDivisions);
  xAxis->SetTitleSize(graphConfig.xTitleSize);
  xAxis->SetTitleOffset(graphConfig.xTitleOffset);
  xAxis->SetLabelSize(graphConfig.xLabelSize);
  xAxis->SetLabelOffset(graphConfig.xLabelOffset);
  TAxis * yAxis = (TAxis *) GetYaxis();
  yAxis->SetNdivisions(graphConfig.nYDivisions);
  yAxis->SetTitleSize(graphConfig.yTitleSize);
  yAxis->SetTitleOffset(graphConfig.yTitleOffset);
  yAxis->SetLabelSize(graphConfig.yLabelSize);
  yAxis->SetLabelOffset(graphConfig.yLabelOffset);

  cout << "-INFO- EnhancedGraph::::setProperties() Completed" << endl;

}

void EnhancedGraph::printToFile(ostream & output)
{
  output << "========================================================================================" << endl;
  output << "   Graph Name:" << GetName() << endl;
  output << "  Graph Title:" << GetName() << endl;
  output << "      nPoints:" << nPoints << endl;
  for (int k=0; k<nPoints; k++)
    {
    output << "x:" << xPtr[k] << " +- " << exPtr[k] << " y:" << yPtr[k] << " +-(stat) " << eyPtr[k] << " (SYST) " << ySysMPtr[k] << "  " << ySysPPtr[k] << endl;
    }
  output << "========================================================================================" << endl;
}
