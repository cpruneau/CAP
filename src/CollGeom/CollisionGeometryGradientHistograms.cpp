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


CollisionGeometryGradientHistograms::CollisionGeometryGradientHistograms(const TString &       _name,
                                                                         const Configuration & _configuration,
                                                                         LogLevel              _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
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
  TString bn = getHistoBaseName();
  h_edge     = createHistogram(bn+TString("edge"),   200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "Edge");
  h_radius   = createHistogram(bn+TString("radius"), 200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "r/r_{max}");
  h_gx       = createHistogram(bn+TString("gx"),     200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "g_{x}");
  h_gy       = createHistogram(bn+TString("gy"),     200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "g_{y}");
  if (reportEnd("CollisionGeometryGradientHistograms",getName(),"createHistograms()"))
    ;
}

//________________________________________________________________________
void CollisionGeometryGradientHistograms::loadHistograms(TFile * inputFile)
{
  if (reportStart("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  if (!inputFile)
    {
    if (reportFatal("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile) inputFile is a nullptr"))
      {
      exit(1);
      }
    }
  TString bn = getHistoBaseName();
  h_edge   = loadH2(inputFile,bn+TString("edge"));
  h_radius = loadH2(inputFile,bn+TString("radius"));
  if (!h_radius)
    {
    if (reportFatal("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile) h_radius is a nullptr"))
      {
      exit(1);
      }
    }
  h_gx     = loadH2(inputFile,bn+TString("gx"));
  if (!h_gx)
    {
    if (reportFatal("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile) h_gx is a nullptr"))
      {
      exit(1);
      }
    }
  h_gy     = loadH2(inputFile,bn+TString("gy"));
  if (!h_gy)
    {
    if (reportFatal("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile) h_gy is a nullptr"))
      {
      exit(1);
      }
    }
  if (reportEnd("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile)"))
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
