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

namespace CAP
{


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
  String legendText;

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
  DataGraph(const String graphName,
            const String xTitle,
            const String yTitle,
            const String legendText,
            double xMin, double xMax,
            double yMin, double yMax,
            int n, double * x, double *exLow,  double *exHigh,  double *y, double *eyLow, double *eyHigh, double *eeyLow, double *eeyHigh, int opt=0);

  //!
  //! Construct a DataGraph with statistical  errors only.
  //!
  DataGraph(const String _graphName,
            const String _xTitle,
            const String _yTitle,
            const String _legendText,
            double _xMin, double _xMax,
            double _yMin, double _yMax,
            int _n,
            double * _x, double *_exLow,  double *_exHigh,
            double * _y, double *_eyLow,  double *_eyHigh,
            int _opt=0);

  //!
  //! Construct a DataGraph based on the given histogram.
  //!
  DataGraph(const String _graphName,
            const String _xTitle,
            const String _yTitle,
            const String _legendText,
            double _xMin, double _xMax,
            double _yMin, double _yMax,
            TH1 * histo, int _opt=0);

  //!
  //! Construct a DataGraph based on the given histogram and the given systematic errors
  //!
  DataGraph(const String graphName,
            const String xTitle,
            const String yTitle,
            const String legendText,
            double xMin, double xMax,
            double yMin, double yMax,
            TH1 * histo, double *eeyLow, double *eeyHigh, int opt=0);



  void draw(bool same);
  void setMinMax(double minY, double maxY);
  void setProperties(GraphConfiguration & graphConfig);
  void printToFile(ostream & output);

  static DataGraph * loadGraph(const String graphName,
                               const String xTitle,
                               const String yTitle,
                               const String legendText,
                               double xMin, double xMax,
                               double yMin, double yMax,
                               TFile * inputFile, String  folderName, String  h1Name, String  h1e1Name, String  h1e2Name, String g1Name );

  static DataGraph * loadGraph(const String graphName,
                               const String xTitle,
                               const String yTitle,
                               const String legendText,
                               double xMin, double xMax,
                               double yMin, double yMax,
                               TFile * inputFile, String hName, double scale=1.0, int rebin=1);



  String getName() const { return GetName(); }
  String getTitle() const { return GetTitle(); }
  String getTitleX() const { return GetXaxis()->GetTitle(); }
  String getTitleY() const { return GetYaxis()->GetTitle(); }
  String getLegendText() const { return legendText; }

  void setName(const String & _name) { SetName(_name); }
  void setTitleX(const String & _name) { GetXaxis()->SetTitle(_name); }
  void setTitleY(const String & _name) { GetYaxis()->SetTitle(_name); }
  void setLegendText(const String & _name) { legendText = _name; }
  bool hasSystematics() const { return graphs.size()>1; }
  TGraph * getDataGraph() {  return graphs[0]; }
  TGraph * getSystGraph() {  return graphs[1]; }
  int getNpoints() { return nPoints; }

  ClassDef(DataGraph,0)
  
};

} // namespace CAP

#endif /* DataGraph_hpp */
