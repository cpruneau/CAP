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
#ifndef CAP__CanvasCollection
#define CAP__CanvasCollection
#include <iostream>
#include "TString.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLegend.h"
#include "Collection.hpp"
#include "CanvasConfiguration.hpp"

class CanvasCollection : public Collection<TCanvas>
{
public:

  ////////////////////////////////////////////////////
  // Member functions
  ////////////////////////////////////////////////////
  CanvasCollection();
  CanvasCollection(const CanvasCollection & source);
  virtual ~CanvasCollection();
  CanvasCollection & operator=(const CanvasCollection & source);

  int getNCanvas()
  {
  return size();
  }

  void createDirectory(const TString & dirName);
  TCanvas * createCanvas(const TString & canvasName, const CanvasConfiguration & canvasConfig, int inc=200);
  TCanvas * createCanvasXX(int nx, int ny, const TString & canvasName, const CanvasConfiguration & canvasConfig, int inc=30);

  void printCanvas(TCanvas * canvas, const TString & directoryName="./", bool printGif=0, bool printPdf=1, bool printSvg=0, bool printPng=0, bool printC=0);
  void printAllCanvas(const TString & outputPath, bool printGif=0, bool printPdf=1, bool printSvg=0, bool printPng=0, bool printC=0);
  TLatex * createLabel(double x, double y, int color, int fontType, double fontSize, const TString & text, bool doDraw=true);
  TLegend * createLegend(float x1, float y1, float x2, float y2, int fontType, float fontSize);
  TLine * createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw=true);
  TArrow * createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw=true);

  ClassDef(CanvasCollection,0)

};
#endif /* CanvasCollection_hpp */
