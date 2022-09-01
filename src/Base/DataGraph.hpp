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
#ifndef CAP__DataGraph
#define CAP__DataGraph

#include <vector>
#include "TString.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "GraphConfiguration.hpp"
#include "MessageLogger.hpp"

using std::vector;

//!
//! Class to draw graphs with statistical and systematic errors
//! Use  the TGraph and derived classes to carry out the drawing..
//!
class DataGraph : public TH1D, public MessageLogger
{
protected:

  //!
  //! Text used to describe this graph in legends
  //!
  TString legendText;

  //!
  //! Graphs used to do the actual plotting.
  //!
  vector<TGraph*>  graphs;

  //!
  //! Number of points
  //!
  int nPoints;

  //!
  //! Independent variable.
  //!
  double * x;
  //!
  //! Uncertainties on x (low side)
  //!
  double * exLow;
  //!
  //! Uncertainties on x (high side)
  //!
  double * exHigh;

  //!
  //! Dependent variable.
  //!
  double * y;

  //!
  //! Statististical uncertainty on y (low side)
  //!
  double * eyLow;

  //!
  //! Statististical uncertainty on y (high side)
  //!
  double * eyHigh;

  //!
  //! Systematic uncertainty on y (low side)
  //!
  double * eeyLow;

  //!
  //! Systematic uncertainty on y (high side)
  //!
  double * eeyHigh;


  int  option;
  
  bool visible;

public:

  //!
  //! Construct a DataGraph with statistical and systematic errors
  //!
  DataGraph(const TString graphName,
            const TString xTitle,
            const TString yTitle,
            const TString legendText,
            double xMin, double xMax,
            double yMin, double yMax,
            int n, double * x, double *exLow,  double *exHigh,  double *y, double *eyLow, double *eyHigh, double *eeyLow, double *eeyHigh, int opt=0);

  //!
  //! Construct a DataGraph with statistical  errors only.
  //!
  DataGraph(const TString _graphName,
            const TString _xTitle,
            const TString _yTitle,
            const TString _legendText,
            double _xMin, double _xMax,
            double _yMin, double _yMax,
            int _n,
            double * _x, double *_exLow,  double *_exHigh,
            double * _y, double *_eyLow,  double *_eyHigh,
            int _opt=0);

  //!
  //! Construct a DataGraph based on the given histogram.
  //!
  DataGraph(const TString _graphName,
            const TString _xTitle,
            const TString _yTitle,
            const TString _legendText,
            double _xMin, double _xMax,
            double _yMin, double _yMax,
            TH1 * histo, int _opt=0);

  //!
  //! Construct a DataGraph based on the given histogram and the given systematic errors
  //!
  DataGraph(const TString graphName,
            const TString xTitle,
            const TString yTitle,
            const TString legendText,
            double xMin, double xMax,
            double yMin, double yMax,
            TH1 * histo, double *eeyLow, double *eeyHigh, int opt=0);



  void draw(bool same);
  void setMinMax(double minY, double maxY);
  void setProperties(const GraphConfiguration & graphConfig);
  void printToFile(ostream & output);

  static DataGraph * loadGraph(const TString graphName,
                               const TString xTitle,
                               const TString yTitle,
                               const TString legendText,
                               double xMin, double xMax,
                               double yMin, double yMax,
                               TFile * inputFile, TString  folderName, TString  h1Name, TString  h1e1Name, TString  h1e2Name, TString g1Name );

  static DataGraph * loadGraph(const TString graphName,
                               const TString xTitle,
                               const TString yTitle,
                               const TString legendText,
                               double xMin, double xMax,
                               double yMin, double yMax,
                               TFile * inputFile, TString hName, double scale=1.0, int rebin=1);



  TString getName() const { return GetName(); }
  TString getTitle() const { return GetTitle(); }
  TString getTitleX() const { return GetXaxis()->GetTitle(); }
  TString getTitleY() const { return GetYaxis()->GetTitle(); }
  TString getLegendText() const { return legendText; }

  void setName(const TString & _name) { SetName(_name); }
  void setTitleX(const TString & _name) { GetXaxis()->SetTitle(_name); }
  void setTitleY(const TString & _name) { GetYaxis()->SetTitle(_name); }
  void setLegendText(const TString & _name) { legendText = _name; }
  bool hasSystematics() const { return graphs.size()>1; }
  TGraph * getDataGraph() {  return graphs[0]; }
  TGraph * getSystGraph() {  return graphs[1]; }
  int getNpoints() { return nPoints; }

  ClassDef(DataGraph,0)
  
};

#endif /* DataGraph_hpp */
