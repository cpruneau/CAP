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


CollisionGeometryGradientHistograms::CollisionGeometryGradientHistograms(Task * _parent,
                                                                         const TString & _name,
                                                                         Configuration & _configuration)
:
Histograms(_parent,_name,_configuration),
h_edge(0),
h_radius(0),
h_gx(0),
h_gy(0)
{
  // no ops
}

void CollisionGeometryGradientHistograms::createHistograms()
{
  if (reportStart("CollisionGeometryGradientHistograms",getName(),"createHistograms()"))
    ;
  TString bn = getParentTaskName();
  h_edge     = createHistogram(makeName(bn,"edge"),   200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "Edge");
  h_radius   = createHistogram(makeName(bn,"radius"), 200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "r/r_{max}");
  h_gx       = createHistogram(makeName(bn,"gx"),     200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "g_{x}");
  h_gy       = createHistogram(makeName(bn,"gy"),     200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "g_{y}");
  if (reportEnd("CollisionGeometryGradientHistograms",getName(),"createHistograms()"))
    ;
}

//________________________________________________________________________
void CollisionGeometryGradientHistograms::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  TString bn = getParentTaskName();
  h_edge   = loadH2(inputFile,makeName(bn,"edge"));
  h_radius = loadH2(inputFile,makeName(bn,"radius"));
  h_gx     = loadH2(inputFile,makeName(bn,"gx"));
  h_gy     = loadH2(inputFile,makeName(bn,"gy"));
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
