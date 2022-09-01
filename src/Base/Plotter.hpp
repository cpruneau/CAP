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
#ifndef CAP__Plotter
#define CAP__Plotter
#include "HistogramCollection.hpp"
#include "CanvasCollection.hpp"
#include "CanvasConfiguration.hpp"
#include "GraphConfiguration.hpp"
#include "DataGraph.hpp"
#include "Task.hpp"

class Plotter : public Task
{
public:

  Plotter(const TString & _name,
          Configuration & _configuration);
  
  virtual ~Plotter() {} 

  //!
  //! Function to plot a single 1D histogram in a new canvas with the given canvas configuration, graph configuration, and a legend. If the text of the legend is a null or empty
  //! string, no legend will be shown on the graph.
  //!
  //! @param canvasName Name to be given to the canvas
  //! @param cc   configuration of the canvas
  //! @param gc   configuration of the graph (histogram)
  //! @param xTitle title of the x-axis
  //! @param xMin mimum of the x-axis
  //! @param xMax maximum of the x-axis
  //! @param yTitle title of the y-axis
  //! @param yMin mimum of the y-axis
  //! @param yMax maximum of the y-axis
  //! @param h    pointer  to the histogram to be plotted
  //! @param legendText  text of the legend entry associated with the graph if any (OK to supply a null string)
  //! @param xMinLeg  x position of the left corner of the legend box
  //! @param yMinLeg  y position of the left corner of the legend box
  //! @param xMaxLeg  x position of the left corner of the legend box
  //! @param yMaxLeg  y position of the left corner of the legend box
  //! @param legendSize  size used to plot the legend's text.
  //!
  TCanvas *  plot(TH1 * h,
                  const TString & canvasName,
                  const CanvasConfiguration & cc,
                  const GraphConfiguration  & gc,
                  const TString & xTitle,  double xMin, double xMax,
                  const TString & yTitle,  double yMin, double yMax,
                  const TString & legendText,
                  double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,
                  double legendSize);

  //!
  //! Function to plot a single 2D histogram in a new canvas with the given canvas configuration, graph configuration, and a legend. If the text of the legend is a null or empty
  //! string, no legend will be shown on the graph.
  //!
  //! @param canvasName Name to be given to the canvas
  //! @param cc   configuration of the canvas
  //! @param gc   configuration of the graph (histogram)
  //! @param xTitle title of the x-axis
  //! @param xMin mimum of the x-axis
  //! @param xMax maximum of the x-axis
  //! @param yTitle title of the y-axis
  //! @param yMin mimum of the y-axis
  //! @param yMax maximum of the y-axis
  //! @param zTitle title of the z-axis
  //! @param zMin mimum of the z-axis
  //! @param zMax maximum of the z-axis
  //! @param h    pointer  to the histogram to be plotted
  //! @param legendText  text of the legend entry associated with the graph if any (OK to supply a null string)
  //! @param xMinLeg  x position of the left corner of the legend box
  //! @param yMinLeg  y position of the left corner of the legend box
  //! @param xMaxLeg  x position of the left corner of the legend box
  //! @param yMaxLeg  y position of the left corner of the legend box
  //! @param legendSize  size used to plot the legend's text.
  //!
  TCanvas *  plot(TH2 * h,
                  const TString & canvasName,
                  const CanvasConfiguration & cc,
                  const GraphConfiguration  & gc,
                  const TString & xTitle,  double xMin, double xMax,
                  const TString & yTitle,  double yMin, double yMax,
                  const TString & zTitle,  double zMin, double zMax);

  TCanvas *  plot(vector<TH1*> histograms,
                  const vector<GraphConfiguration*> & graphConfigurations,
                  const vector<TString>  &  legendTexts,
                  const TString & canvasName,
                  const CanvasConfiguration & canvasConfiguration,
                  const TString & xTitle,  double xMin, double xMax,
                  const TString & yTitle,  double yMin, double yMax,
                  double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,
                  double legendSize);

  TCanvas *  plot(vector<TGraph*> graphs,
                  const vector<GraphConfiguration*> & graphConfigurations,
                  const vector<TString>  &  legendTexts,
                  const TString & canvasName,
                  const CanvasConfiguration & canvasConfiguration,
                  const TString & xTitle,  double xMin, double xMax,
                  const TString & yTitle,  double yMin, double yMax,
                  double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,
                  double legendSize);


  //  // ================================================================================================
  //  // Function to plot nHists 1D histogram
  //  // h       : vector of nHists pointers to histograms
  //  // legends : vector of nHists pointers to labels used as legends in the body of the plot
  //  // ================================================================================================
  //  TCanvas *  plot(TString  canvasName, CanvasConfiguration & cc, vector<GraphConfiguration*> gc,
  //                  TString  xTitle,  double xMin, double xMax,
  //                  TString  yTitle,  double yMin, double yMax,
  //                  vector<TH1*> histograms,
  //                  vector<TString> legends,
  //                  double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,double legendSize,
  //                  bool label1=0, TString text1="", double x1=0.0, double y1=0.0, int color1=1,  double fontSize1=0.05,
  //                  bool label2=0, TString text2="", double x2=0.0, double y2=0.0, int color2=1,  double fontSize2=0.05);
  //
  //  TCanvas * plot(TString  canvasName, CanvasConfiguration & cc,
  //                 TString  xTitle,  double xMin, double xMax,
  //                 TString  yTitle,  double yMin, double yMax,
  //                 vector<DataGraph*> graphs,
  //                 double xMinLeg, double yMinLeg, double xMaxLeg, double yMaxLeg,double legendSize);

  ////////////////////////////////////////////////////////////////////////
  // Setting Histogram Properties
  ////////////////////////////////////////////////////////////////////////

  void findMinMax(TH1* histogram, double & minimum, double & maximum);

  void setProperties(TH1 * h, const GraphConfiguration & graphConfiguration);
  void setProperties(TH1 * h, const GraphConfiguration & graphConfiguration, const TString & xTitle, const TString & yTitle, const TString & zTitle="");

  void setProperties(TGraph * g, const GraphConfiguration & graphConfiguration);
  void setProperties(TGraph * g, const GraphConfiguration & graphConfiguration, const TString & xTitle, const TString & yTitle);

  TLatex  * createLabel(const TString & text, double x, double y, double angle, int color,  double fontSize, bool doDraw=true);

  TLegend * createLegend(double x1, double y1, double x2, double y2, double fontSize);
  TLegend * createLegend(TH1*histogram, const TString & legendText, double x1, double y1, double x2, double y2, double fontSize, bool doDraw=true);
  TLegend * createLegend(vector<TH1*> h,vector<TString> legendTexts,double x1, double y1, double x2, double y2, double fontSize, bool doDraw=true);
  TLegend * createLegend(vector<TGraph*> h,vector<TString> legendTexts,double x1, double y1, double x2, double y2, double fontSize, bool doDraw=true);
  TLegend * createLegend(vector<DataGraph*> graphs,double x1, double y1, double x2, double y2, double fontSize, bool doDraw=true);


  TLine   * createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw=true);
  TArrow  * createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw=true);
  void setDefaultOptions(bool color);

  void printAllCanvas(const TString & outputPath, bool printGif=0, bool printPdf=1, bool printSvg=0, bool printC=0)
  {
  canvasCollection.printAllCanvas(outputPath, printGif, printPdf, printSvg, printC);
  }

  CanvasCollection & getCanvases()
  {
  return canvasCollection;
  }

  HistogramCollection & getHistogramCollection()
  {
  return histogramCollection;
  }

protected:

  CanvasCollection canvasCollection;
  HistogramCollection histogramCollection;

  ClassDef(Plotter,0)
};

#endif /* CAP__Plotter */
