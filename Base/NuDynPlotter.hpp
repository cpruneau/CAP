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
#ifndef CAP__NuDynPlotter
#define CAP__NuDynPlotter
#include "Plotter.hpp"
#include "NuDynHistos.hpp"
#include "NuDynDerivedHistos.hpp"

class NuDynPlotter : public Plotter
{
public:
  
  NuDynPlotter();
  virtual ~NuDynPlotter();
  void makePlots(const TString & canvasNameBase,
                 NuDynHistos * nuDynHistosUsed,
                 NuDynDerivedHistos * nuDynDerivedHistosUsed,
                 CanvasConfiguration * canvasConfigurationUsed,
                 GraphConfiguration  ** graphConfigurationsUsed);

  void makeComparisonPlots(const TString & canvasNameBase,
                           CanvasConfiguration * canvasConfiguration,
                           int nGraphs,
                           NuDynHistos ** nuDynHistos,
                           NuDynDerivedHistos ** nuDynDerivedHistos,
                           TString ** histLabels,
                           GraphConfiguration  ** graphConfigurations);

  void makeNudynComparisonPlots(const TString & canvasNameBase,
                                CanvasConfiguration * canvasConfiguration,
                                int nGraphs,
                                NuDynHistos ** nuDynHistos,
                                NuDynDerivedHistos ** nuDynDerivedHistos,
                                TString ** histLabels,
                                GraphConfiguration  ** graphConfigurations);

  ClassDef(NuDynPlotter,0)
};

#endif /* CAP__NuDynPlotter */
