/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Victor Gonzalez  Dec 19, 2020
 *
 * *********************************************************************/
#ifndef CAP__BidimGaussFitConfiguration
#define CAP__BidimGaussFitConfiguration
#include "TString.h"
#include <iostream>
#include <ostream>
#include <fstream>
#include <TMath.h>
#include <TROOT.h>
#include <TH2.h>
#include <TF2.h>
#include "Configuration.hpp"

using std::cout;
using std::endl;
using std::ostream;
using std::ofstream;

namespace CAP
{

class BidimGaussFitConfiguration : public Configuration
{
  public:

  enum CollisionSystem
  {
     kPbPb276 = 0,
     kPbPb276MM,
     kPbPb276PP,
     kPbPb276CI,
     kPbPb502,
     kXeXe544,
     kXeXe544MM,
     kXeXe544PP,
     kAMPTFULL,
     kAMPTHALF,
     kAMPTSMALL,
     kAMPTNONE,
     kExtMM,
     kExtPP,
     kExtPM,
     kExtCI,
     kExtCD,
     kExtUS,
     kExtLS
  };

  BidimGaussFitConfiguration();
  BidimGaussFitConfiguration(const BidimGaussFitConfiguration& source);
  virtual ~BidimGaussFitConfiguration();
  const BidimGaussFitconst Configuration & operator=(const BidimGaussFitconst Configuration & source);
  void setExtraConfigParams(TH2 * h);

  void excludedEtaBins(double deltaEtaExclusionRegion,
                       const TH2 *hist,
                       const int   *excludedBins,
                       int   iCent,
                       int   &lowBin, int   &highBin);
  void excludedPhiBins(double deltaEtaExclusionRegion,
                       const TH2 *hist,
                       const int   *excludedBins,
                       int   iCent,
                       int   &lowBin,
                       int   &highBin);
  double excludeCentralRegion(double deltaEtaExclusionRegion,TH2 *hist);

  void reset();
  void setConfiguration(enum CollisionSystem sys, int  iCent);
  virtual void printConfiguration(ostream & out);

  int        maxIterations; //(15000);
  String    defaultFitter; //("Minuit2");

  String    collisionSystemName;
  CollisionSystem collisionSystem;
  int        iCent;
  bool       useFlowInFit;
  bool       useEtaDependentFlowInFit;
  bool       useFullAzimuthalFlow;
  bool       useCentralGaussians;
  bool       useExcludeCentralRegion;
  bool       useEllipticalPatch;
  bool       useScaling;
  bool       useSingleCanvas;
  bool       drawEtaGapDist;

//  double     sigmaLowerLimit, sigmaUpperLimit;
//  double     betaLowerLimit,  betaUpperLimit;
//  double     a0LowerLimit,    a0UpperLimit;
//  double     anLowerLimit,    anUpperLimit;

  String    fitOption;
  int        nFitLoops;
  int        nFitParameters;
  String ** parameterNames;
  bool    *  fixedParameters;
  double  *  initialValues;
  double  *  fixedValues;
  double  *  lowerLimit;
  double  *  upperLimit;


  int        nBinsExcludedEta;
  int        nBinsExcludedPhi;

  double     deltaEtaExclusionRegion;
  double     deltaEtaLimit;
  double     deltaEtaOuterLimit;
  double     deltaPhiLowLimit;
  double     deltaPhiHighLimit;
  double     deltaPhiHighestVal;

  int        bwdDeltaEtaLimitBin;
  int        fwdDeltaEtaLimitBin;
  int        bwdDeltaEtaOuterLimitBin;
  int        fwdDeltaEtaOuterLimitBin;
  int        deltaPhiLowLimitBin;
  int        deltaPhiHighLimitBin;

  double valueInPatchBorder;
  int    lowBinEtaInPatch;
  int    highBinEtaInPatch;
  int    lowBinPhiInPatch;
  int    highBinPhiInPatch;

  double xMinEtaFullFitLegend;
  double yMinEtaFullFitLegend;
  double xMaxEtaFullFitLegend;
  double yMaxEtaFullFitLegend;
  double etaFullFitLegendSize;

  double xMinPhiFullFitLegend;
  double yMinPhiFullFitLegend;
  double xMaxPhiFullFitLegend;
  double yMaxPhiFullFitLegend;
  double phiFullFitLegendSize;

  double xMinEtaPeakFitLegend;
  double yMinEtaPeakFitLegend;
  double xMaxEtaPeakFitLegend;
  double yMaxEtaPeakFitLegend;
  double etaPeakFitLegendSize;

  double xMinPhiPeakFitLegend;
  double yMinPhiPeakFitLegend;
  double xMaxPhiPeakFitLegend;
  double yMaxPhiPeakFitLegend;
  double phiPeakFitLegendSize;

  String xTitle;
  String yTitle;
  String zTitle;

  String outputPath;

// Configuration for different systems..
  // Number of bins to be excluded relative to the central bin
  // in the eta and phi directions..
  static int excludeCentralBinsEtaPbPb276MM[9]; // = {3,3,3,3,3,3,3,-1,-1};
  static int excludeCentralBinsEtaPbPb276PP[9]; // = {3,3,3,3,3,3,3,-1,-1};
  static int excludeCentralBinsEtaPbPb276CI[9]; // = {3,3,3,3,3,3,3,-1,-1};
  static int excludeCentralBinsEtaAMPTFULL [9]; // = {3,3,3,3,3,2,-1,-1,-1};
  static int excludeCentralBinsEtaAMPTHALF [9]; // = {3,3,3,2,-1,-1,-1,-1,-1};
  static int excludeCentralBinsEtaAMPTSMALL[9]; // = {1,1,1,1,0,0,0,0,0};
  static int excludeCentralBinsEtaAMPTNONE [9]; // = {0,0,0,0,0,0,0,0,0};
  static int excludeCentralBinsEtaXeXe544MM[9]; // = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
  static int excludeCentralBinsEtaXeXe544PP[9]; // = {-1,-1,-1,-1,-1,-1,-1,-1,-1};

  static int excludeCentralBinsPhiPbPb276MM[9]; // = {3,3,3,3,3,3,3,-1,-1};
  static int excludeCentralBinsPhiPbPb276PP[9]; // = {3,3,3,3,3,3,3,-1,-1};
  static int excludeCentralBinsPhiPbPb276CI[9]; // = {2,3,3,3,3,3,3,-1,-1};
  static int excludeCentralBinsPhiAMPTFULL [9]; // = {2,2,2,2,2,2,-1,-1,-1};
  static int excludeCentralBinsPhiAMPTHALF [9]; // = {2,2,2,1,-1,-1,-1,-1,-1};
  static int excludeCentralBinsPhiAMPTSMALL[9]; // = {1,1,1,1,0,0,0,0,0};
  static int excludeCentralBinsPhiAMPTNONE [9]; // = {0,0,0,0,0,0,0,0,0};
  static int excludeCentralBinsPhiXeXe544MM[9]; // = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
  static int excludeCentralBinsPhiXeXe544PP[9]; // = {-1,-1,-1,-1,-1,-1,-1,-1,-1};

  static int excludeCentralBinsEtaMM[20]; // = {0};
  static int excludeCentralBinsEtaPP[20]; // = {0};
  static int excludeCentralBinsEtaPM[20]; // = {0};
  static int excludeCentralBinsEtaCI[20]; // = {0};
  static int excludeCentralBinsEtaCD[20]; // = {0};
  static int excludeCentralBinsEtaUS[20]; // = {0};
  static int excludeCentralBinsEtaLS[20]; // = {0};

  static int excludeCentralBinsPhiMM[20]; // = {0};
  static int excludeCentralBinsPhiPP[20]; // = {0};
  static int excludeCentralBinsPhiPM[20]; // = {0};
  static int excludeCentralBinsPhiCI[20]; // = {0};
  static int excludeCentralBinsPhiCD[20]; // = {0};
  static int excludeCentralBinsPhiUS[20]; // = {0};
  static int excludeCentralBinsPhiLS[20]; // = {0};

  ClassDef(BidimGaussFitConfiguration,0)
  };

} // namespace CAP

#endif /* CAP__BidimGaussFitconst Configuration &/
