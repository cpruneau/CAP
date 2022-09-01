//
//  runUrqmd.c
//  CodeV3
//
//  Created by Claude Pruneau on 8/22/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//

#include <stdio.h>

//
//  cm_figure9_6.c
//  MyMC
//
//  Created by Claude Pruneau on 4/10/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "TRandom.h"
#include "ParticleFilter.hpp"
#include "ParticlePairFilter.hpp"
#include "AnalysisConfiguration.hpp"
#include "CanvasConfiguration.hpp"
#include "GraphConfiguration.hpp"
#include "GeneratorConfiguration.hpp"
#include "MasterAnalysis.hpp"

#include "UrQMDEvent.hpp"

ClassImp(UrQMDParticle)
ClassImp(UrQMDEvent)


////////////////////////////////////////////////////
// Set Default Style for Plots
////////////////////////////////////////////////////
void setDefaultOptions(int colorPalette=1)
{
  switch (colorPalette)
  {
    default:
    case 0:
      break;
    case 1:
      gStyle->SetPalette(1,0);
      break;
    case 2:
      gStyle->SetPalette(2,0);
      break;
    case 3:
      gStyle->SetPalette(3,0);  //deep sea
      break;
    case 4:
      gStyle->SetPalette(4,0);  //atlantic
      break;
    case 5:
      gStyle->SetPalette(5,0);  //cmyk
      break;
    case 6:
      gStyle->SetPalette(6,0);  //island
      break;
    case 7:
      gStyle->SetPalette(7,0);
      break;
  }
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);

}

/* the mapping of the parameter impact and the centrality */
const Int_t ncent = 14;
const char *cent[ncent] = {
    "0001",
    "0102",
    "0203",
    "0304",
    "0405",
    "0005",
    "0510",
    "1020",
    "2030",
    "3040",
    "4050",
    "5060",
    "6070",
    "7080"
};
const double lowb[ncent] = {
    0.000000001,
    1.57,
    2.22,
    2.71,
    3.13,
    0.000000001,
    3.50,
    4.94,
    6.98,
    8.55,
    9.88,
    11.04,
    12.09,
    13.05,
};
const double highb[ncent] = {
    1.57,
    2.22,
    2.71,
    3.13,
    3.50,
    3.50,
    4.94,
    6.98,
    8.55,
    9.88,
    11.04,
    12.09,
    13.05,
    13.97
};



/////////////////////////////////////////////////////////////////////////////////////////
// Run an analysis with the Cluster Model
/////////////////////////////////////////////////////////////////////////////////////////
void runUrqmd(const char *source, const char *outdir, const char *productiondir, Int_t centix, long nEventsRequested=5000000000)
{
  cout << "-INFO- runClusterModelAnalysis() Setup configuration" << endl;

  // ---------------------------
  // debugLevel Levels
  // ---------------------------
  bool debugLoop      = true;
  bool debugGenerator = true;
  bool debugAnalyzer  = false;

  // ---------------------------
  // Graphics parameters
  // ---------------------------
  setDefaultOptions(1);

  // ---------------------------
  // Histogram parameters
  // ---------------------------

  /* Cummulate errors by default */
  TH1::SetDefaultSumw2(kTRUE);

  AnalysisConfiguration * ac = new AnalysisConfiguration();
  ac->outputPath               = TString::Format("%s/%s/",outdir,productiondir).Data(); // "../OUTPUT/";
  ac->baseName                 = TString::Format("Urqmd_%s",cent[centix]).Data();
  ac->configFileName           = TString::Format("Config_%s.txt",cent[centix]).Data();
  ac->rootOutputFileName       = TString::Format("Urqmd_Histograms_%s.root",cent[centix]).Data();

    cout<<"================================================================"<<endl;
    cout<<""<<endl;
    cout<<"         running for = "<<ac->baseName<<endl;
    cout<<""<<endl;
    cout<<"================================================================"<<endl;
  ac->nBins_nCell = 100;
  ac->min_nCell   = 0.0;
  ac->max_nCell   = 100.0;
  ac->nBins_nPartPerCell = 100;
  ac->min_nPartPerCell   = 0.0;
  ac->max_nPartPerCell   = 100.0;

  ac->nBins_pt    = 18;
  ac->min_pt      = 0.2;
  ac->max_pt      = 2.0;
  ac->nBins_eta   = 32;
  ac->min_eta     = -1.6;
  ac->max_eta     = 1.6;
  ac->nBins_y     = 40;
  ac->min_y       = -4.0;
  ac->max_y       = 4.0;
  ac->nBins_phi   = 72;
  ac->min_phi     = 0.0;
  ac->max_phi     = 2.0*3.1415927;

  ac->nBins_DeltaPlong = 10;
  ac->min_DeltaPlong   = -1.0;
  ac->max_DeltaPlong   =  1.0;
  ac->nBins_DeltaPside = 10;
  ac->min_DeltaPside   = -1.0;
  ac->max_DeltaPside   =  1.0;
  ac->nBins_DeltaPout  = 10;
  ac->min_DeltaPout    = -1.0;
  ac->max_DeltaPout    =  1.0;

  ac->fillPairs        = true;
  ac->fill3D           = false;
  ac->fill6D           = false;
  ac->fillQ3D          = false;
  ac->fillY            = false;

  ac->scaleHistograms      = false;
  ac->externalScaling      = 1.0;
  ac->saveHistosToRootFile = true;
  ac->forceRewrite         = true;
  ac->saveHistosToTextFile = false;
  ac->calculateDerived     = false;
  ac->plotGraphs           = false;

  // use the following options to select which graphs are plotted on screen.
  ac->plotSingles         = true;     // selects the single part histo to be printed
  ac->plotPairs           = true;    // selects the part pair histo to be printed
  ac->plotDerivedPairs    = true;    // selects the derived part pair histo  to be printed
  ac->plotCombined        = true;
  ac->plotEventHistos     = true;    // global event stuff

  ac->printGraphs          = false;
  ac->plotAll              = false;
  ac->printAll             = false;

  ac->particleFilter1      = new ParticleFilter( 1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta,ac->min_y,ac->max_y); // +ve only
  ac->particleFilter2      = new ParticleFilter(-1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta,ac->min_y,ac->max_y); // -ve only
  ac->particlePairFilter12 = new ParticlePairFilter(0.0, 100.0,
                                                    0.0, 2.0*3.1415927,
                                                    0.0, 6.0);

  ac->canvasConfiguration1D  = new CanvasConfiguration(1);
  ac->graphConfiguration1D   = new GraphConfiguration(1,1);
  ac->canvasConfiguration2D  = new CanvasConfiguration(0);
  ac->graphConfiguration2D   = new GraphConfiguration(2);

  // ==================================================================================
  // Urqmd Model Selection Parameters
  // ==================================================================================
 GeneratorConfiguration * mc = new GeneratorConfiguration(5,"UrqmdModel");
  // ---------------------------
  // Cell Parameters
  // ---------------------------
  mc->impactParameterMinimum     = lowb[centix];
  mc->impactParameterMaximum     = highb[centix];
  mc->ptMinimum = ac->min_pt;
  mc->ptMaximum = ac->max_pt;
  mc->yMinimum = ac->min_y;
  mc->yMaximum =  ac->max_y;
  mc->etaMinimum = ac->min_eta;
  mc->etaMaximum =  ac->max_eta;
  mc->eventsSource = source;

  MasterAnalysis * analysis = new MasterAnalysis(mc,ac, debugLoop, debugGenerator,debugAnalyzer);
  analysis->run(nEventsRequested);

  if (analysis->eventsAnalyzed != 0) {
    gSystem->Exec(TString::Format("echo \"%s%s, %ld\" >> %s/fileslist_%s.txt",
        ac->outputPath.Data(),
        ac->rootOutputFileName.Data(),
        analysis->eventsAnalyzed,
        productiondir,
        cent[centix]).Data());
  }
}
