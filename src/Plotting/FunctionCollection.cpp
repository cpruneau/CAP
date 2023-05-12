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
using CAP::Collection;
using CAP::FunctionCollection;

ClassImp(Collection<TF1>);

ClassImp(FunctionCollection);

FunctionCollection::FunctionCollection(const String& _name,Severity logLevel)
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


void FunctionCollection::setFunctionProperties(TF1 * f, GraphConfiguration & graphConfiguration)
{
  if (reportDebug(__FUNCTION__))
    {
    cout << "Setting properties of function: " << f->GetTitle() << endl;
    }
  f->SetLineColor(graphConfiguration.getValueInt(getName(),"lineColor"));
  f->SetLineStyle(graphConfiguration.getValueInt(getName(),"lineStyle"));
  f->SetLineWidth(graphConfiguration.getValueInt(getName(),"lineWidth"));
  TAxis * xAxis = (TAxis *) f->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.getValueDouble(getName(),"nXDivisions"));
  xAxis->SetTitleSize(graphConfiguration.getValueDouble(getName(),"xTitleSize"));
  xAxis->SetTitleOffset(graphConfiguration.getValueDouble(getName(),"xTitleOffset"));
  //xAxis->SetTitle(graphConfiguration.xTitle);
  xAxis->SetLabelSize(graphConfiguration.getValueDouble(getName(),"xLabelSize"));
  xAxis->SetLabelOffset(graphConfiguration.getValueDouble(getName(),"xLabelOffset"));
  TAxis * yAxis = (TAxis *) f->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.getValueInt(getName(),"nYDivisions"));
  yAxis->SetTitleSize(graphConfiguration.getValueDouble(getName(),"yTitleSize"));
  yAxis->SetTitleOffset(graphConfiguration.getValueDouble(getName(),"yTitleOffset"));
  yAxis->SetLabelSize(graphConfiguration.getValueDouble(getName(),"yLabelSize"));
  yAxis->SetLabelOffset(graphConfiguration.getValueDouble(getName(),"yLabelOffset"));
  //yAxis->SetTitle(graphConfiguration.yTitle);
  if (f->IsA() == TF2::Class() )
    {
    TAxis * zAxis = (TAxis *) f->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.getValueInt(getName(),"nZDivisions"));
    zAxis->SetTitleSize(graphConfiguration.getValueDouble(getName(),"zTitleSize"));
    zAxis->SetTitleOffset(graphConfiguration.getValueDouble(getName(),"zTitleOffset"));
    zAxis->SetLabelSize(graphConfiguration.getValueDouble(getName(),"zLabelSize"));
    zAxis->SetLabelOffset(graphConfiguration.getValueDouble(getName(),"zLabelOffset"));
    }
}


void FunctionCollection::plotAllFunctions(const String & outputPath, bool doPrint)
{
  if (reportStart(__FUNCTION__))
;
  GraphConfiguration  * gc1D = new GraphConfiguration(1,0);
  GraphConfiguration  * gc2D = new GraphConfiguration(2,0);
  CanvasConfiguration * cc1D = new CanvasConfiguration();
  CanvasConfiguration * cc2D = new CanvasConfiguration();
  CanvasCollection    * canvasCollection = new CanvasCollection();
  canvasCollection->createDirectory(outputPath);
  String name;
  //cout << "-INFO- plotAllHistos(..) getNFunction() :" << getNFunction()  << " with capacity:" << nFunctionCapacity << endl;

  for (int iFunc=0; iFunc<getNFunction(); iFunc++)
    {
    TF1* f = getObjectAt(iFunc);
    name = f->GetName();
    if (f->IsA() == TF1::Class())
      {
      if (reportInfo(__FUNCTION__))
        {
        cout << "Plotting 1D function:" << iFunc << " named " << f->GetTitle() << endl;

        }
      canvasCollection->createCanvas(name, *cc1D, 30);
      setFunctionProperties(f, *gc1D);
      f->Draw();
      }
    else if (f->IsA() == TF2::Class())
      {
      if (reportInfo(__FUNCTION__)) cout << "Plotting 2D function:" << iFunc << " named " << f->GetTitle() << endl;
      canvasCollection->createCanvas(name, *cc2D, 30);
      setFunctionProperties(f, *gc2D);
      f->Draw("SURF3");
      }
    }
  if (doPrint) canvasCollection->printAllCanvas(outputPath);
}
