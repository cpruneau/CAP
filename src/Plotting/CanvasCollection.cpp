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
#include "CanvasCollection.hpp"
using CAP::CanvasCollection;
using CAP::Collection;

ClassImp(Collection<TCanvas>);

ClassImp(CanvasCollection);

CanvasCollection::CanvasCollection()
:
Collection<TCanvas>()
{
  /* no ops*/
}

CanvasCollection::~CanvasCollection()
{
  /* no ops*/
}

CanvasCollection::CanvasCollection(const CanvasCollection & source)
:
Collection<TCanvas>(source)
{
  /* no ops*/
}


CanvasCollection & CanvasCollection::operator=(const CanvasCollection & source)
{
  Collection<TCanvas>::operator=(source);
  return *this;
}



// =================================================
// Create a directory or folder in the file system
// If the directory alearfy exists, do nothing.
// =================================================
void CanvasCollection::createDirectory(const String & dirName)
{
  gSystem->mkdir(dirName,1);
}

// =================================================
// Create a canvas
// =================================================
TCanvas * CanvasCollection::createCanvas(const String & canvasName, const CanvasConfiguration & configuration, int inc)
{
  int xInc = inc*getCollectionSize();
  TCanvas * canvas = new TCanvas(canvasName,
                                 canvasName,
                                 xInc+configuration.getValueInt("windowXPosition"),
                                 configuration.getValueInt("windowYPosition"),
                                 configuration.getValueInt("windowWidth"),
                                 configuration.getValueInt("windowHeight") );
  canvas->SetLogx(        configuration.getValueBool("windowLogX") );
  canvas->SetLogy(        configuration.getValueBool("windowLogY") );
  canvas->SetLogz(        configuration.getValueBool("windowLogZ") );
  canvas->SetRightMargin( configuration.getValueDouble("windowRightMargin") );
  canvas->SetLeftMargin(  configuration.getValueDouble("windowLeftMargin") );
  canvas->SetBottomMargin(configuration.getValueDouble("windowBottomMargin") );
  canvas->SetTopMargin(   configuration.getValueDouble("windowTopMargin") );
//  canvas->SetTheta(       configuration.getValueDouble("windowTheta") );
//  canvas->SetPhi(         configuration.getValueDouble("windowPhi") );
//
//  canvas->SetFillColor(  configuration.getValueInt("windowFillColor") );
//  canvas->SetFillStyle(  configuration.getValueInt("windowFillStyle") );
//  canvas->SetBorderSize( configuration.getValueInt("windowBorderSize") );
//  canvas->SetBorderMode( configuration.getValueInt("windowBorderMode") );
  append(canvas);

  return canvas;
}

////////////////////////////////////////////////////
// Create a canvas
////////////////////////////////////////////////////
TCanvas * CanvasCollection::createCanvasXX(int nx, int ny, const String & canvasName, const CanvasConfiguration & configuration, int inc)
{
  int xInc = inc*getCollectionSize();
  TCanvas * canvas = new TCanvas(canvasName,
                                 canvasName,
                                 xInc+configuration.getValueInt("windowXPosition"),
                                 configuration.getValueInt("windowYPosition"),
                                 configuration.getValueInt("windowWidth"),
                                 configuration.getValueInt("windowHeight") );
  canvas->Divide(nx,ny,0,0);
  canvas->SetLogx(        configuration.getValueBool("windowLogX") );
  canvas->SetLogy(        configuration.getValueBool("windowLogY") );
  canvas->SetLogz(        configuration.getValueBool("windowLogZ") );
  canvas->SetRightMargin( configuration.getValueDouble("windowRightMargin") );
  canvas->SetLeftMargin(  configuration.getValueDouble("windowLeftMargin") );
  canvas->SetBottomMargin(configuration.getValueDouble("windowBottomMargin") );
  canvas->SetTopMargin(   configuration.getValueDouble("windowTopMargin") );
  canvas->SetTheta(       configuration.getValueDouble("windowTheta") );
  canvas->SetPhi(         configuration.getValueDouble("windowPhi") );

  canvas->SetFillColor(  configuration.getValueInt("windowFillColor") );
  canvas->SetFillStyle(  configuration.getValueInt("windowFillStyle") );
  canvas->SetBorderSize( configuration.getValueDouble("windowBorderSize") );
  canvas->SetBorderMode( configuration.getValueDouble("windowBorderMode") );
  
  append(canvas);
  return canvas;
}

////////////////////////////////////////////////////
// Print Canvas
////////////////////////////////////////////////////
///
void CanvasCollection::printCanvas(TCanvas * canvas, const String & directoryName, bool printGif, bool printPdf, bool printSvg, bool printPng, bool printC)
{
  createDirectory(directoryName);
  String fileName = directoryName;
  fileName += "/";
  fileName += canvas->GetName();
  if (printGif) canvas->Print(fileName+".gif");
  if (printPdf) canvas->Print(fileName+".pdf");
  if (printSvg) canvas->Print(fileName+".svg");
  if (printPng) canvas->Print(fileName+".png");
  if (printC)   canvas->Print(fileName+".C");
  //canvas->Print(fileName+".png");
}

// /////////////////////////////////////////////////
// Print Canvases
// /////////////////////////////////////////////////
void CanvasCollection::printAllCanvas(const String & outputPath, bool printGif, bool printPdf, bool printSvg, bool printPng, bool printC)
{
  for (int k=0; k<getNCanvas(); k++)
    {
    printCanvas(getObjectAt(k),outputPath,printGif,printPdf,printSvg,printPng,printC);
    }
}

////////////////////////////////////////////////////
// Create a label within an existing pad
////////////////////////////////////////////////////
TLatex * CanvasCollection::createLabel(double x, double y, int color, int fontType __attribute__((unused)), double fontSize, const String & text, bool doDraw)
{
  TLatex * label;
  label = new TLatex(x,y,text);
  label->SetTextColor(color);
  //label->SetTextFont(fontType);
  label->SetTextSize(fontSize);
  if (doDraw) label->Draw();
  return label;
}

////////////////////////////////////////////////////
// Create standard legend
////////////////////////////////////////////////////
TLegend * CanvasCollection::createLegend(float x1, float y1, float x2, float y2, int fontType __attribute__((unused)), float fontSize)
{
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  //legend->SetTextFont(fontType);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  return legend;
}

////////////////////////////////////////////////////
// Create simple line
////////////////////////////////////////////////////
TLine * CanvasCollection::createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw)
{
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
TArrow * CanvasCollection::createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw)
{
  TArrow *line = new TArrow(x1,y1,x2,y2,arrowSize,option);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  if (doDraw) line->Draw();
  return line;
}
