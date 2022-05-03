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

#include "FunctionCollection.hpp"

ClassImp(Collection<TF1>);

ClassImp(FunctionCollection);

FunctionCollection::FunctionCollection(const TString& _name,LogLevel logLevel)
:
Collection<TF1>(_name,false,logLevel)
{
  /* no ops*/
}


FunctionCollection::FunctionCollection(const FunctionCollection & source)
:
Collection<TF1>(source)
{
  /* no ops*/
}

FunctionCollection::~FunctionCollection()
{
  //deletefunctions();
}

FunctionCollection & FunctionCollection::operator=(const FunctionCollection & source)
{
  Collection<TF1>::operator=(source);
  return *this;
}



////////////////////////////////////////////////////
// Set Default Style for Plots
////////////////////////////////////////////////////
void FunctionCollection::setDefaultOptions(bool color)
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

////////////////////////////////////////////////////////////////////////
// Setting Function Properties
////////////////////////////////////////////////////////////////////////


void FunctionCollection::setFunctionProperties(TF1 * f, const GraphConfiguration & graphConfiguration)
{
  if (reportDebug("FunctionCollection",getName(),"setFunctionProperties(...)"))
    {
    cout << "Setting properties of function: " << f->GetTitle() << endl;
    }
  f->SetLineColor(graphConfiguration.lineColor);
  f->SetLineStyle(graphConfiguration.lineStyle);
  f->SetLineWidth(graphConfiguration.lineWidth);
  TAxis * xAxis = (TAxis *) f->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.nXDivisions);
  xAxis->SetTitleSize(graphConfiguration.xTitleSize);
  xAxis->SetTitleOffset(graphConfiguration.xTitleOffset);
  //xAxis->SetTitle(graphConfiguration.xTitle);
  xAxis->SetLabelSize(graphConfiguration.xLabelSize);
  xAxis->SetLabelOffset(graphConfiguration.xLabelOffset);
  TAxis * yAxis = (TAxis *) f->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.nYDivisions);
  yAxis->SetTitleSize(graphConfiguration.yTitleSize);
  yAxis->SetTitleOffset(graphConfiguration.yTitleOffset);
  yAxis->SetLabelSize(graphConfiguration.yLabelSize);
  yAxis->SetLabelOffset(graphConfiguration.yLabelOffset);
  //yAxis->SetTitle(graphConfiguration.yTitle);
  if (f->IsA() == TF2::Class() )
    {
    TAxis * zAxis = (TAxis *) f->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.nZDivisions);
    zAxis->SetTitleSize(graphConfiguration.zTitleSize);
    zAxis->SetTitleOffset(graphConfiguration.zTitleOffset);
    zAxis->SetLabelSize(graphConfiguration.zLabelSize);
    zAxis->SetLabelOffset(graphConfiguration.zLabelOffset);
    }
}


void FunctionCollection::plotAllFunctions(const TString & outputPath, bool doPrint)
{
  if (reportStart("FunctionCollection",getName(),"plotAllFunctions(...)"))
    ;
  GraphConfiguration  * gc1D = new GraphConfiguration(1,0);
  GraphConfiguration  * gc2D = new GraphConfiguration(2,0);
  CanvasConfiguration * cc1D = new CanvasConfiguration();
  CanvasConfiguration * cc2D = new CanvasConfiguration();
  CanvasCollection    * canvasCollection = new CanvasCollection();
  canvasCollection->createDirectory(outputPath);
  TString name;
  //cout << "-INFO- plotAllHistos(...) getNFunction() :" << getNFunction()  << " with capacity:" << nFunctionCapacity << endl;

  for (int iFunc=0; iFunc<getNFunction() ; iFunc++)
    {
    TF1* f = getObjectAt(iFunc);
    name = f->GetName();
    if (f->IsA() == TF1::Class())
      {
      if (reportInfo("FunctionCollection",getName(),"plotAllFunctions(...)"))
        {
        cout << "Plotting 1D function:" << iFunc << " named " << f->GetTitle() << endl;

        }
      canvasCollection->createCanvas(name, *cc1D, 30);
      setFunctionProperties(f, *gc1D);
      f->Draw();
      }
    else if (f->IsA() == TF2::Class())
      {
      if (reportInfo("FunctionCollection",getName(),"plotAllFunctions(...)"))
        {
        cout << "Plotting 2D function:" << iFunc << " named " << f->GetTitle() << endl;

        }
      canvasCollection->createCanvas(name, *cc2D, 30);
      setFunctionProperties(f, *gc2D);
      f->Draw("SURF3");
      }
    }
  if (doPrint) canvasCollection->printAllCanvas(outputPath);
}
