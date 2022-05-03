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
#ifndef CAP__StatStudyPlotter
#define CAP__StatStudyPlotter
#include "Plotter.hpp"
#include "StatStudyHistograms.hpp"

class StatStudyPlotter : public Plotter
{
public:
  
  StatStudyPlotter(const TString& _name,LogLevel _logLevel=Info);
  virtual ~StatStudyPlotter();
  void makePlots(const TString & canvasNameBase,
                 StatStudyHistograms * statStudyHistogramsUsed,
                 CanvasConfiguration * canvasConfigurationUsed,
                 GraphConfiguration  ** graphConfigurationsUsed);

  void makeComparisonPlots(const TString & canvasNameBase,
                           CanvasConfiguration * canvasConfiguration,
                           int nGraphs,
                           StatStudyHistograms ** statStudyHistograms,
                           TString ** hLegends,
                           GraphConfiguration  ** graphConfigurations);

  ClassDef(StatStudyPlotter,0)
};

#endif /* CAP__StatStudyPlotter */
