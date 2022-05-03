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
#include "StatStudyPlotter.hpp"
ClassImp(StatStudyPlotter);

StatStudyPlotter::StatStudyPlotter(const TString& _name,LogLevel _logLevel)
:
Plotter(_name,_logLevel)
{  }

StatStudyPlotter::~StatStudyPlotter()
{
}



void StatStudyPlotter::makePlots(const TString & canvasNameBase,
                                 StatStudyHistograms * statStudyHistograms,
                                 CanvasConfiguration * canvasConfiguration,
                                 GraphConfiguration  ** graphConfigurations)
{
TH1 ** histograms = new TH1* [10];
TString ** hLegends = new TString * [10];

canvasConfiguration->setAxes(CanvasConfiguration::Linear);
//canvasConfiguration->setAxes(CanvasConfiguration::LogY);

histograms[0] = statStudyHistograms->f1_1;     hLegends[0] = new TString("n_{1}^{(+)}");
histograms[1] = statStudyHistograms->f1_2;     hLegends[1] = new TString("n_{1}^{(-)}");
histograms[2] = statStudyHistograms->f1_1Eff;  hLegends[2] = new TString("n_{1}^{(+,Det)}");
histograms[3] = statStudyHistograms->f1_2Eff;  hLegends[3] = new TString("n_{1}^{(-,Det)}");
plot(4, canvasNameBase+"f1",canvasConfiguration,graphConfigurations,
     "n_{1}", 0.0, 0.0,
     "P(n_{1})", 0.0, 0.0,
     histograms,hLegends,0.7, 0.5, 0.85, 0.85, 0.055);

histograms[0] = statStudyHistograms->f1_1Ratio;  hLegends[0] = new TString("f_{1}^{(+,Det)}/f_{1}^{(+)}");
histograms[1] = statStudyHistograms->f1_2Ratio;  hLegends[1] = new TString("f_{1}^{(-,Det)}/f_{1}^{(-)}");
plot(2, canvasNameBase+"f1Ratio",canvasConfiguration,graphConfigurations,
     "f_{1}^{(Det)}/f_{1}", 0.84,0.89,
     "P(f_{1}^{(Det)}/f_{1})", 0.0, 0.0,
     histograms,hLegends,0.7, 0.6, 0.85, 0.85, 0.065);

histograms[0] = statStudyHistograms->f2_11;     hLegends[0] = new TString("n_{2}^{(++)}");
histograms[1] = statStudyHistograms->f2_12;     hLegends[1] = new TString("n_{2}^{(+-)}");
histograms[2] = statStudyHistograms->f2_22;     hLegends[2] = new TString("n_{2}^{(--)}");
histograms[3] = statStudyHistograms->f2_11Eff;  hLegends[3] = new TString("n_{2}^{(++,Det)}");
histograms[4] = statStudyHistograms->f2_12Eff;  hLegends[4] = new TString("n_{2}^{(+-,Det)}");
histograms[5] = statStudyHistograms->f2_22Eff;  hLegends[5] = new TString("n_{2}^{(--,Det)}");
plot(6, canvasNameBase+"f2",canvasConfiguration,graphConfigurations,
     "n_{2}",  0.0, 0.0,
     "P(n_{2})", 0.0, 0.0,
     histograms,hLegends,0.7, 0.4, 0.85, 0.85, 0.055);


histograms[0] = statStudyHistograms->f2_11Ratio;  hLegends[0] = new TString("f_{2}^{(++,Det)}/f_{2}^{(++)}");
histograms[1] = statStudyHistograms->f2_12Ratio;  hLegends[1] = new TString("f_{2}^{(+-,Det)}/f_{2}^{(+-)}");
histograms[2] = statStudyHistograms->f2_22Ratio;  hLegends[2] = new TString("f_{2}^{(--,Det)}/f_{2}^{(--)}");
plot(3, canvasNameBase+"f2Ratio",canvasConfiguration,graphConfigurations,
     "f_{2}^{(Det)}/f_{2}", 0.70,0.80,
     "P(f_{2}^{(Det)}/f_{2})", 0.0, 0.0,
     histograms,hLegends,0.65, 0.6, 0.85, 0.9, 0.065);

histograms[0] = statStudyHistograms->r2_11;     hLegends[0] = new TString("R_{2}^{(++)}");
histograms[1] = statStudyHistograms->r2_12;     hLegends[1] = new TString("R_{2}^{(+-)}");
histograms[2] = statStudyHistograms->r2_22;     hLegends[2] = new TString("R_{2}^{(--)}");
histograms[3] = statStudyHistograms->r2_11Eff;  hLegends[3] = new TString("R_{2}^{(++,Det)}");
histograms[4] = statStudyHistograms->r2_12Eff;  hLegends[4] = new TString("R_{2}^{(+-,Det)}");
histograms[5] = statStudyHistograms->r2_22Eff;  hLegends[5] = new TString("R_{2}^{(--,Det)}");
plot(6, canvasNameBase+"R2",canvasConfiguration,graphConfigurations,
     "R_{2}",  0.0, 0.003,
     "P(R_{2})", 0.0, 0.0,
     histograms,hLegends,0.70, 0.3, 0.85, 0.90, 0.065);

histograms[0] = statStudyHistograms->r2_11Ratio;  hLegends[0] = new TString("R_{2}^{(++,Det)}/R_{2}^{(++)}");
histograms[1] = statStudyHistograms->r2_12Ratio;  hLegends[1] = new TString("R_{2}^{(+-,Det)}/R_{2}^{(+-)}");
histograms[2] = statStudyHistograms->r2_22Ratio;  hLegends[2] = new TString("R_{2}^{(--,Det)}/R_{2}^{(--)}");
plot(3, canvasNameBase+"R2Ratio",canvasConfiguration,graphConfigurations,
     "R_{2}^{(Det)}/R_{2}", 0.8,1.2,
     "P(R_{2}^{(Det)}/R_{2})", 0.0, 0.04,
     histograms,hLegends,0.60, 0.6, 0.85, 0.9, 0.065);
//  statStudyHistograms->r2_11Ratio->Fit("gaus");
//  statStudyHistograms->r2_12Ratio->Fit("gaus");
//  statStudyHistograms->r2_22Ratio->Fit("gaus");


//statStudyHistograms->nudyn_12->Fit("gaus");
//statStudyHistograms->nudyn_12Det->Fit("gaus");
histograms[0] = statStudyHistograms->nudyn_12;     hLegends[0] = new TString("#nu_{Dyn}");
histograms[1] = statStudyHistograms->nudyn_12Eff;  hLegends[1] = new TString("#nu_{Dyn}^{(Det)}");
plot(2, canvasNameBase+"NuDyn",canvasConfiguration,graphConfigurations,
     "#nu_{Dyn}", -0.005,0.005,
     "P(#nu_{Dyn})", 0.0, 0.0,
     histograms,hLegends,0.70, 0.6, 0.85, 0.9, 0.065);

//statStudyHistograms->nudyn_12Ratio->Fit("gaus");
histograms[0] = statStudyHistograms->nudyn_12Ratio;     hLegends[0] = new TString("#nu_{dyn}^{(Det)}/#nu_{dyn}");
plot(1, canvasNameBase+"NuDynRatio",canvasConfiguration,graphConfigurations,
     "#nu_{dyn}^{(Det)}/#nu_{dyn}", 0.5,1.5,
     "P(#nu_{Dyn}^{(Det)}/#nu_{Dyn})", 0.0, 0.0,
     histograms,hLegends,0.70, 0.6, 0.85, 0.9, 0.065);

  int keep = graphConfigurations[1]->lineWidth ;
  graphConfigurations[1]->lineWidth = 6;

histograms[0] = statStudyHistograms->f1_1Ratio_vsSub;     hLegends[0] = new TString("Quad");
histograms[1] = statStudyHistograms->f1_1RatioOptB_vsSub;     hLegends[1] = new TString("Binomial");
plot(2, canvasNameBase+"f1_1Ratio_vsSub-zoom",canvasConfiguration,graphConfigurations,
     "sub-sample", 950.0, 1020.0,
     "f_{1}^{(Det)}/f_{1}", 0.868, 0.872,
     histograms,hLegends,0.7, 0.8, 0.85, 0.9, 0.055);

histograms[0] = statStudyHistograms->r2_11_vsSub;     hLegends[0] = new TString("R_{2}^{(++)}");
plot(1, canvasNameBase+"r2_vsSub-zoom",canvasConfiguration,graphConfigurations,
    "sub-sample", 950.0, 1020.0,
     "R_{2}",  0.0006, 0.0014,
     histograms,hLegends,0.75, 0.8, 0.85, 0.9, 0.055);

histograms[0] = statStudyHistograms->r2_11Ratio_vsSub;     hLegends[0] = new TString("Quad");
histograms[1] = statStudyHistograms->r2_11RatioOptB_vsSub;     hLegends[1] = new TString("Binomial");
plot(2, canvasNameBase+"R2ratio_vsSub-zoom",canvasConfiguration,graphConfigurations,
    "sub-sample", 950.0, 1020.0,
     "R_{2}^{(Det)}/R_{2}", 0.8, 1.2,
     histograms,hLegends,0.70, 0.8, 0.85, 0.9, 0.055);

  histograms[0] = statStudyHistograms->nudyn_12_vsSub;     hLegends[0] = new TString("#nu_{Dyn}^{12,Det}");
  plot(1, canvasNameBase+"nudyn_12Eff_vsSub-zoom",canvasConfiguration,graphConfigurations,
      "sub-sample", 950.0, 1020.0,
       "#nu_{Dyn}^{12,Det}", -0.0015, -0.0006,
       histograms,hLegends,0.70, 0.8, 0.85, 0.9, 0.055);

histograms[0] = statStudyHistograms->nudyn_12Ratio_vsSub;     hLegends[0] = new TString("Quad");
histograms[1] = statStudyHistograms->nudyn_12RatioOptB_vsSub;     hLegends[1] = new TString("Binomial");
plot(2, canvasNameBase+"nudyn_12Ratio_vsSub-zoom",canvasConfiguration,graphConfigurations,
    "sub-sample", 950.0, 1020.0,
     "#nu_{Dyn}^{12,Det}/#nu_{Dyn}^{(+-)}", 0.5, 1.5,
     histograms,hLegends,0.70, 0.8, 0.85, 0.9, 0.055);

   graphConfigurations[1]->lineWidth =keep;
canvasConfiguration->setAxes(CanvasConfiguration::LogX);

histograms[0] = statStudyHistograms->ef1_1_vsSub;     hLegends[0] = new TString("#delta(f_{1}^{(+)})/f_{1}^{(+)}");
histograms[1] = statStudyHistograms->ef1_2_vsSub;     hLegends[1] = new TString("#delta(f_{1}^{(-)})/f_{1}^{(-)}");
histograms[2] = statStudyHistograms->ef1_1Eff_vsSub;  hLegends[2] = new TString("#delta(f_{1}^{(+,Det)})/f_{1}^{(+,Det)}");
histograms[3] = statStudyHistograms->ef1_2Eff_vsSub;  hLegends[3] = new TString("#delta(f_{1}^{(-,Det)})/f_{1}^{(-,Det)}");
plot(4, canvasNameBase+"ef1_vsSub",canvasConfiguration,graphConfigurations,
     "sub-sample", 0.0, 1005.0,
     "#delta(f_{1})/f_{1}", 0.0, 0.003,
     histograms,hLegends,0.6, 0.45, 0.85, 0.9, 0.06);

  histograms[0] = statStudyHistograms->ef1_1Ratio_vsSub;     hLegends[0] = new TString("#delta(f_{1}^{(+,Det)}/f_{1}^{(+)})/f_{1}^{(+,Det)})/f_{1}^{(+)}");
  histograms[1] = statStudyHistograms->ef1_2Ratio_vsSub;     hLegends[1] = new TString("#delta(f_{1}^{(-,Det)}/f_{1}^{(-)})/f_{1}^{(-,Det)})/f_{1}^{(-)}");
  plot(2, canvasNameBase+"ef1Ratio_vsSub",canvasConfiguration,graphConfigurations,
       "sub-sample", 0.0, 1005.0,
       "#delta(f_{1}^{(Det)})/f_{1})/f_{1}^{(Det)})/f_{1}", 0.0, 0.003,
       histograms,hLegends,0.50, 0.45, 0.85, 0.9, 0.06);

histograms[0] = statStudyHistograms->er2_11_vsSub;     hLegends[0] = new TString("#delta(R_{2}^{(++)})/R_{2}^{(++)}");
histograms[1] = statStudyHistograms->er2_12_vsSub;     hLegends[1] = new TString("#delta(R_{2}^{(+-)})/R_{2}^{(+-)}");
histograms[2] = statStudyHistograms->er2_22_vsSub;     hLegends[2] = new TString("#delta(R_{2}^{(--)})/R_{2}^{(--)}");
histograms[3] = statStudyHistograms->er2_11Eff_vsSub;  hLegends[3] = new TString("#delta(R_{2}^{(++,Det)})/R_{2}^{(++,Det)}");
histograms[4] = statStudyHistograms->er2_12Eff_vsSub;  hLegends[4] = new TString("#delta(R_{2}^{(+-,Det)})/R_{2}^{(+-,Det)}");
histograms[5] = statStudyHistograms->er2_22Eff_vsSub;  hLegends[5] = new TString("#delta(R_{2}^{(--,Det)})/R_{2}^{(--,Det)})");
plot(6, canvasNameBase+"er2_vsSub",canvasConfiguration,graphConfigurations,
     "sub-sample", 0.0, 1005.0,
     "#delta(R_{2})/R_{2}", 0.0, 0.3,
     histograms,hLegends,0.60, 0.40, 0.85, 0.92, 0.05);

histograms[0] = statStudyHistograms->er2_11Ratio_vsSub;  hLegends[0] = new TString("#delta(R_{2}^{(++,Det)}/R_{2}^{(++)})/R_{2}^{(++,Det)}/R_{2}^{(++)}");
histograms[1] = statStudyHistograms->er2_12Ratio_vsSub;  hLegends[1] = new TString("#delta(R_{2}^{(+-,Det)}/R_{2}^{(+-)})/R_{2}^{(+-,Det)}/R_{2}^{(+-)}");
histograms[2] = statStudyHistograms->er2_22Ratio_vsSub;  hLegends[2] = new TString("#delta(R_{2}^{(--,Det)}/R_{2}^{(--)})/R_{2}^{(--,Det)}/R_{2}^{(--)}");
plot(3, canvasNameBase+"er2Ratio_vsSub",canvasConfiguration,graphConfigurations,
     "sub-sample", 0.0, 1005.0,
     "#delta(R_{2}^{(Det)}/R_{2})/R_{2}^{(Det)}/R_{2}", 0.0, 0.12,
     histograms,hLegends,0.40, 0.65, 0.85, 0.9, 0.05);



  histograms[0] = statStudyHistograms->enudyn_12_vsSub;     hLegends[0] = new TString("#delta(#nu_{Dyn}^{(+-)})/#nu_{Dyn}^{(+-)}");
  histograms[1] = statStudyHistograms->enudyn_12Eff_vsSub;  hLegends[1] = new TString("#delta(#nu_{Dyn}^{(+-,Det)})/#nu_{Dyn}^{(+-,Det)}");
  plot(2, canvasNameBase+"enudyn_12_vsSub",canvasConfiguration,graphConfigurations,
       "sub-sample", 0.0, 1005.0,
       "#delta(#nu_{Dyn}^{(+-)})/#nu_{Dyn}^{(+-)}", 0.0, 0.35,
       histograms,hLegends,0.50, 0.65, 0.85, 0.9, 0.05);

  histograms[0] = statStudyHistograms->enudyn_12Ratio_vsSub;     hLegends[0] = new TString("#delta(#nu_{Dyn}^{(+-,Det)}/#nu_{Dyn}^{(+-)})/#nu_{Dyn}^{(+-,Det)}/#nu_{Dyn}^{(+-)}");
  plot(1, canvasNameBase+"enudyn_12Ratio_vsSub",canvasConfiguration,graphConfigurations,
       "sub-sample", 0.0, 1005.0,
       "#delta(#nu_{Dyn}^{(+-,Det)}/#nu_{Dyn}^{(+-)})/#nu_{Dyn}^{(+-,Det)}/#nu_{Dyn}^{(+-)}", 0.0, 0.25,
       histograms,hLegends,0.50, 0.65, 0.85, 0.9, 0.05);

}

void StatStudyPlotter::makeComparisonPlots(const TString & canvasNameBase,
                                           CanvasConfiguration * canvasConfiguration,
                                           int nGraphs,
                                           StatStudyHistograms ** statStudyHistograms,
                                           TString ** hLegends,
                                           GraphConfiguration  ** graphConfigurations)
{
  TH1 ** histograms = new TH1* [10];

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);

  for (int iGraph=0;iGraph<nGraphs;iGraph++)
    {
    histograms[iGraph] = statStudyHistograms[iGraph]->r2_11Ratio;
    }
  plot(nGraphs, canvasNameBase+"R2RatioComparison",canvasConfiguration,graphConfigurations,
       "R_{2}^{(Det)}/R_{2}", 0.8,1.2,
       "P(R_{2}^{(Det)}/R_{2})", 0.0, 0.1,
       histograms,hLegends,0.70, 0.6, 0.85, 0.85, 0.06);

  for (int iGraph=0;iGraph<nGraphs;iGraph++)
    {
    histograms[iGraph] = statStudyHistograms[iGraph]->nudyn_12Ratio;
    }
  plot(nGraphs, canvasNameBase+"NuDynRatioComparison",canvasConfiguration,graphConfigurations,
     "#nu_{dyn}^{(Det)}/#nu_{dyn}", 0.5,1.5,
     "P(#nu_{Dyn}^{(Det)}/#nu_{Dyn})", 0.0, 0.08,
     histograms,hLegends,0.70, 0.6, 0.85, 0.85, 0.06);

//  int keep = graphConfigurations[1]->lineWidth ;
//  graphConfigurations[1]->lineWidth = 6;

  for (int iGraph=0;iGraph<nGraphs;iGraph++)
     {
     histograms[iGraph] = statStudyHistograms[iGraph]->ef1_1Ratio_vsSub;
     }
  plot(nGraphs, canvasNameBase+"ef1Ratio_vsSubComparison",canvasConfiguration,graphConfigurations,
      "sub-sample", 0.0, 1005.0,
       "#delta(f_{1}^{(+,Det)})/f_{1}^{(+)})/f_{1}^{(+,Det)})/f_{1}^{(+)}", 0.0, -2.0,
       histograms,hLegends,0.70, 0.6, 0.85, 0.85, 0.06);

  for (int iGraph=0;iGraph<nGraphs;iGraph++)
      {
      histograms[iGraph] = statStudyHistograms[iGraph]->er2_11_vsSub;
      }
  plot(nGraphs, canvasNameBase+"er2_vsSubComparison",canvasConfiguration,graphConfigurations,
     "sub-sample", 0.0, 1005.0,
     "#delta(R_{2}^{(++)})/R_{2}^{(++)}", 0.0,  -2.0,
     histograms,hLegends,0.70, 0.6, 0.85, 0.85, 0.06);

  for (int iGraph=0;iGraph<nGraphs;iGraph++)
       {
       histograms[iGraph] = statStudyHistograms[iGraph]->er2_11Ratio_vsSub;
       }
  plot(nGraphs, canvasNameBase+"er2Ratio_vsSubComparison",canvasConfiguration,graphConfigurations,
     "sub-sample", 0.0, 1005.0,
     "#delta(R_{2}^{(Det)}/R_{2})/R_{2}^{(Det)}/R_{2}", 0.0,  -2.0,
     histograms,hLegends,0.70, 0.6, 0.85, 0.85, 0.06);


  for (int iGraph=0;iGraph<nGraphs;iGraph++)
  {
  histograms[iGraph] = statStudyHistograms[iGraph]->enudyn_12_vsSub;
  }
  plot(nGraphs, canvasNameBase+"enudyn_12_vsSubComparison",canvasConfiguration,graphConfigurations,
       "sub-sample", 0.0, 1005.0,
       "#delta(#nu_{Dyn}^{(+-)})/#nu_{Dyn}^{(+-)}", 2.0,  -2.0,
       histograms,hLegends,0.70, 0.6, 0.85, 0.85, 0.06);

  for (int iGraph=0;iGraph<nGraphs;iGraph++)
   {
   histograms[iGraph] = statStudyHistograms[iGraph]->enudyn_12Ratio_vsSub;
   }
  plot(nGraphs, canvasNameBase+"enudyn_12Ratio_vsSubComparison",canvasConfiguration,graphConfigurations,
       "sub-sample", 0.0, 1005.0,
       "#delta(#nu_{Dyn}^{(+-,Det)}/#nu_{Dyn}^{(+-)})/#nu_{Dyn}^{(+-,Det)}/#nu_{Dyn}^{(+-)}", 0.0,  -2.0,
       histograms,hLegends,0.70, 0.6, 0.85, 0.85, 0.06);

}
