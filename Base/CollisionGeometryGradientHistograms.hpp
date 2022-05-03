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
#ifndef CAP__CollisionGeometryGradientHistograms
#define CAP__CollisionGeometryGradientHistograms
#include "Histograms.hpp"
#include "Configuration.hpp"

class CollisionGeometryGradientHistograms : public Histograms
{
public:

  CollisionGeometryGradientHistograms(const TString &       _name,
                                      const Configuration & _configuration,
                                      LogLevel              _debugLevel);
  virtual ~CollisionGeometryGradientHistograms() { }
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void getRadiusAndGradient(double x, double y, double & radius, double & gx, double & gy);

protected:

  TH2      * h_edge;
  TH2      * h_radius;
  TH2      * h_gx;
  TH2      * h_gy;

  ClassDef(CollisionGeometryGradientHistograms,0)

};

#endif /* CAP__CollisionGeometryGradientHistograms  */



