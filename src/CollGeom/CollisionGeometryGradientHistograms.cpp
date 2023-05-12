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
#include "CollisionGeometryGradientHistograms.hpp"
using CAP::CollisionGeometryGradientHistograms;

CollisionGeometryGradientHistograms::CollisionGeometryGradientHistograms(Task * _parent,
                                                                         const String & _name,
                                                                         const Configuration & _configuration)
:
HistogramGroup(_parent,_name,_configuration),
h_edge(0),
h_radius(0),
h_gx(0),
h_gy(0)
{
  // no ops
}

void CollisionGeometryGradientHistograms::createHistograms()
{
  if (reportStart(__FUNCTION__))
;
  String bn = getParentName( );
  h_edge     = createHistogram(createName(bn,"edge"),   200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "Edge");
  h_radius   = createHistogram(createName(bn,"radius"), 200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "r/r_{max}");
  h_gx       = createHistogram(createName(bn,"gx"),     200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "g_{x}");
  h_gy       = createHistogram(createName(bn,"gy"),     200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "g_{y}");
  if (reportEnd(__FUNCTION__))
;
}

//________________________________________________________________________
void CollisionGeometryGradientHistograms::importHistograms(TFile & inputFile)
{
  if (reportStart(__FUNCTION__))
;
  String bn = getParentName( );
  h_edge   = loadH2(inputFile,createName(bn,"edge"));
  h_radius = loadH2(inputFile,createName(bn,"radius"));
  h_gx     = loadH2(inputFile,createName(bn,"gx"));
  h_gy     = loadH2(inputFile,createName(bn,"gy"));
  if (reportEnd(__FUNCTION__))
;
}

void CollisionGeometryGradientHistograms::getRadiusAndGradient(double x, double y, double & radius, double & gx, double & gy)
{
  int iX = h_radius->GetXaxis()->FindBin(x);
  int iY = h_radius->GetYaxis()->FindBin(y);
  radius = h_radius->GetBinContent(iX,iY);
  gx     = h_gx->GetBinContent(iX,iY);
  gy     = h_gy->GetBinContent(iX,iY);
}
