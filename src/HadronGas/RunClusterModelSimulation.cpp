//
//  cm_figure9_7.c
//  MyMC
//
//  Created by Claude Pruneau on 4/10/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
#include "ClusterModelEvent.hpp"
#include "ClusterModelConfiguration.hpp"
#include "ClusterModelEventGenerator2.hpp"
#include "AnalysisConfiguration.hpp"
#include "ClusterModelEventAnalyzer.hpp"
#include "TwoPartCorrelationAnalyzer.hpp"
#include "EventLoop.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"

/////////////////////////////////////////////////////////////////////////////////////////
// Run an analysis with the Cluster Model
/////////////////////////////////////////////////////////////////////////////////////////
//void runClusterModel(long nEventsRequested=50000)

int main()
{
  cout << "<INFO> Cluster Model Analysis - Starting" << endl;

  long nEventsRequested=100;

  AnalysisConfiguration & ac = new AnalysisConfiguration("ClusterModel1","ClusterModel","1.0");
  ac->loadHistograms  = false;
  ac->createHistograms  = true;
  ac->scaleHistograms  = false;
  ac->calculateDerivedHistograms  = false;
  ac->saveHistograms  = true;
  ac->resetHistograms  = false;
  ac->clearHistograms  = false;
  ac->ForceHistogramsRewrite  = true;
  ac->inputPath = "./";
  ac->configurationFileName = "configuration";
  ac->rootInputFileName = "";
  ac->outputPath = "./";
  ac->rootOuputFileName =  "output";
  ac->histoBaseName =  "CMsim";
  ac->nBins_nCell = 100;
  ac->min_nCell   = 0.0;
  ac->max_nCell   = 100.0;
  ac->nBins_nPartPerCell = 100;
  ac->min_nPartPerCell   = 0.0;
  ac->max_nPartPerCell   = 100.0;
  ac->nBins_nPartTot = 300.0;
  ac->min_nPartTot = 0.0;
  ac->max_nPartTot = 300;

  ac->nBins_pt    = 40;
  ac->min_pt      = 0.0;
  ac->max_pt      = 3.0;
  ac->nBins_eta   = 20;
  ac->min_eta     = -2;
  ac->max_eta     = 2;
  ac->nBins_y     = 20;
  ac->min_y       = -2;
  ac->max_y       = 2;
  ac->nBins_phi   = 36;
  ac->min_phi     = 0.0;
  ac->max_phi     = 2.0*3.1415927;

  ac->nBins_DeltaPlong = 40;
  ac->min_DeltaPlong   = -1.0;
  ac->max_DeltaPlong   =  1.0;
  ac->nBins_DeltaPside = 40;
  ac->min_DeltaPside   = -1.0;
  ac->max_DeltaPside   =  1.0;
  ac->nBins_DeltaPout  = 40;
  ac->min_DeltaPout    = -1.0;
  ac->max_DeltaPout    =  1.0;

  ac->fillPairs        = true;
  ac->fill3D           = false;
  ac->fill6D           = false;
  ac->fillQ3D          = false;
  ac->fillY            = true;

  // ==================================================================================
  // Cluster Model Generator Parameters
  // ==================================================================================
  ClusterModelConfiguration & cmc = new ClusterModelConfiguration("ClusterModel2","ClusterModel","1.0");
  cmc->loadHistograms    = false;
  cmc->createHistograms  = false;
  cmc->scaleHistograms   = false;
  cmc->calculateDerivedHistograms  = false;
  cmc->saveHistograms   = false;
  cmc->resetHistograms  = false;
  cmc->clearHistograms  = false;
  cmc->inputPath = "./";
  cmc->configurationFileName = "configuration";
  cmc->rootInputFileName = "";
  cmc->outputPath = "./";
  cmc->rootOuputFileName =  "";

  // ---------------------------
  // Cell Parameters
  // ---------------------------
  cmc->cellMultiplicity_Random     = 1;//
  cmc->cellMultiplicity_Fixed      = 1;//
  cmc->cellMultiplicity_Minimum    = 1;//2
  cmc->cellMultiplicity_Maximum    = 2;//10
  cmc->cellMultiplicity_Slope      = 2.0;

  cmc->cellBeta_Random             = 1;
  cmc->cellBeta_Fixed              = 0.9;
  cmc->cellBeta_Minimum            = 0.6;
  cmc->cellBeta_Maximum            = 0.95;

  cmc->cellPhi_Random              = 1;
  cmc->cellPhi_Fixed               = 0.5;
  cmc->cellPhi_Minimum             = 0.0;
  cmc->cellPhi_Maximum             = 2.0*3.1415927;

  // ------------------------------------------------------
  // Cluster Parameters
  // ------------------------------------------------------
  cmc->clusterMultiplicity_Random     = 1;
  cmc->clusterMultiplicity_Fixed      = 1;
  cmc->clusterMultiplicity_Minimum    = 1;//2
  cmc->clusterMultiplicity_Maximum    = 2;//10
  cmc->clusterMultiplicity_Slope      = 2.0;

  cmc->clusterMass_Random      = 2;
  cmc->clusterMass_Fixed       = 0.77;
  cmc->clusterMass_Minimum     = 0.0; // above the 2 pion threshold.
  cmc->clusterMass_Maximum     = 1.5;
  cmc->clusterMass_Slope       = 0.13;

  // case 0: // fixed
  // case 1: // uniform rapidity
  // case 2: // exponential
  // case 3: // exponential paired
  cmc->clusterRapidity_Random   =  1;
  cmc->clusterRapidity_Fixed    =  0.0;
  cmc->clusterRapidity_Minimum  = -3;
  cmc->clusterRapidity_Maximum  =  3;
  cmc->clusterRapidity_Slope    =  0.0;

  cmc->clusterMomentum_Random   =  2;
  cmc->clusterMomentum_Fixed    =  0.0000;
  cmc->clusterMomentum_Minimum  =  0.0;
  cmc->clusterMomentum_Maximum  =  2.0;
  cmc->clusterMomentum_Slope    =  0.010;

  // ------------------------------------------------------
  // Cluster Parameters
  // ------------------------------------------------------
  cmc->particleMultiplicity_Random     = 0;
  cmc->particleMultiplicity_Fixed      = 2;
  cmc->particleMultiplicity_Minimum    = 2;
  cmc->particleMultiplicity_Maximum    = 2;
  cmc->particleMultiplicity_Slope      = 0;

  cmc->particleMomenum_Random     = 2;
  cmc->particleMomenum_Fixed      = 0;
  cmc->particleMomenum_Minimum    = 0.0;
  cmc->particleMomenum_Maximum    = 4.0;
  cmc->particleMomenum_Slope      = 0.15;


  EventFilter     * eventFilter      = new EventFilter();
  ParticleFilter  * particleFilter1  = new ParticleFilter( 1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y); // +ve only
  ParticleFilter  * particleFilter2  = new ParticleFilter( 1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y); // +ve only
  ParticleFilter  * particleFilter   = new ParticleFilter(-1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y); // -ve only

  ClusterModelEvent * event = ClusterModelEvent::getClusterModelEvent();
  ClusterModelEventGenerator2 * gen  = new ClusterModelEventGenerator2("CMG2",cmc, event);
  ClusterModelEventAnalyzer   * ana1 = new ClusterModelEventAnalyzer  ("CMA", ac,  event, eventFilter,particleFilter);
  TwoPartCorrelationAnalyzer  * ana2 = new TwoPartCorrelationAnalyzer ("TPA", ac,  event, eventFilter,particleFilter1,particleFilter2);

  EventLoop * eventLoop = new EventLoop();

  gen->setReportLevel(MessageLogger::Info);
  ana1->setReportLevel(MessageLogger::Info);
  ana2->setReportLevel(MessageLogger::Info);
  eventLoop->setReportLevel(MessageLogger::Info);

  eventLoop->addTask(gen);
  eventLoop->addTask(ana1);
  eventLoop->addTask(ana2);
  eventLoop->run(nEventsRequested,10);

  cout << "<INFO> Cluster Model Analysis - Completed" << endl;

}
