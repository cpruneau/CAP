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
#ifndef CAP__EnhancedGraph
#define CAP__EnhancedGraph

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// EnhancedGraph                                                       //
//                                                                     //
// Class used to draw graphs with stat and syst error bars             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "GraphConfiguration.hpp"

////////////////////////////////////////////////////
// Class EnhancedGraph
////////////////////////////////////////////////////
class EnhancedGraph : public TH1D
{
public:
  ////////////////////////////////////////////////////
  // Data Members
  ////////////////////////////////////////////////////
  TGraph * g;
  TGraph * gP;
  TGraph * gM;
  TGraph * gSys;

  int nPoints;
  double * xPtr;
  double * exPtr;
  double * yPtr;
  double * eyPtr;
  double * ySysMPtr;
  double * ySysPPtr;
  double * yM;
  double * yP;
  double sysAbsolute;
  int    option;
  
  bool isVisible;
  ////////////////////////////////////////////////////
  // Member functions
  ////////////////////////////////////////////////////
  EnhancedGraph(const TString & graphName,
                const TString & xTitle,
                const TString & yTitle,
                double minX, double maxX,
                double minY, double maxY,
                int n, double * x, double *y, double *ex, double *ey, int opt=0);
  EnhancedGraph(const TString & graphName,
                const TString & xTitle,
                const TString & yTitle,
                double minX, double maxX,
                double minY, double maxY,
                int n, double * x, double *y, double *ex, double *ey,
                double * yM, double * yP,
                int opt=0,
                bool absolute=true);
  void draw(bool same);
  void setMinMax(double minY, double maxY);
  void setProperties(const GraphConfiguration & graphConfig);
  void printToFile(ostream & output);

  ClassDef(EnhancedGraph,0)
  
};

#endif /* EnhancedGraph_hpp */
