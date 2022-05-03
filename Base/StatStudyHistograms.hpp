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
#ifndef CAP__StatStudyHistograms
#define CAP__StatStudyHistograms
#include "Histograms.hpp"
#include "TString.h"

class StatStudyHistograms : public Histograms
{
public:

  StatStudyHistograms(int _nSubSamples,
                      const TString &       _name,
                      const Configuration & _configuration,
                      LogLevel              _debugLevel);
  virtual ~StatStudyHistograms();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void saveHistograms(TFile * outputFile);
  virtual void computeDerivedHistograms();
  virtual void fill(double syb, double nPlus, double nMinus, double nPlusEff,  double nMinusEff);
  virtual void scaleHistograms(double scalingFactor1,double scalingFactor2);

  // r = f1/f2
  void calculateRatio(double f1,double ef1,double f2,double ef2,double & r,double & er);
  void calculateR2(double f1,double ef1,double f2,double ef2,double & r2,double & er2);
  void calculateR2(double f1_1,double ef1_1,double f1_2,double ef1_2,double f2,double ef2,double & r2,double & er2);
  void calculateNudyn(double r2_11,double er2_11,double r2_12,double er2_12,double r2_22,double er2_22,double & nudyn,double & enudyn);
  void calculateR2(TH1* h1, TH1* h2, TH1* hr);
  void calculateR2(TH1* h1_1, TH1* h1_2, TH1* h2_12, TH1* hr);
  void calculateNuDyn(TH1* r2_11, TH1* r2_12, TH1* r2_22, TH1* nudyn);
  void calculateRatio(TH1 * h1, TH1 * h2, TH1 * ratio, TH1 * ratioOptB=0);
  void calculateMeanAndStd(TH1 * h, int nBins, double &mean, double &typicalError, double &eMean, double &std);
  void fillMeanErrStd(TH1 * h, TH1 * eh,int n);
  void fillHist(TH1 * source, TH1 * target, int n);

  // ==================
  // Data Members
  // ==================
  int  nSubSamples;
  TH1* f1_1;
  TH1* f1_2;
  TH1* f2_11;
  TH1* f2_12;
  TH1* f2_22;
  TH1* r2_11;
  TH1* r2_12;
  TH1* r2_22;
  TH1* nudyn_12;
  TH1* f1_1Eff;
  TH1* f1_2Eff;
  TH1* f2_11Eff;
  TH1* f2_12Eff;
  TH1* f2_22Eff;
  TH1* r2_11Eff;
  TH1* r2_12Eff;
  TH1* r2_22Eff;
  TH1* nudyn_12Eff;
  TH1* f1_1Ratio;
  TH1* f1_2Ratio;
  TH1* f2_11Ratio;
  TH1* f2_12Ratio;
  TH1* f2_22Ratio;
  TH1* r2_11Ratio;
  TH1* r2_12Ratio;
  TH1* r2_22Ratio;
  TH1* nudyn_12Ratio;
  TH1* f1_1_vsSub;
  TH1* f1_2_vsSub;
  TH1* f2_11_vsSub;
  TH1* f2_12_vsSub;
  TH1* f2_22_vsSub;
  TH1* r2_11_vsSub;
  TH1* r2_12_vsSub;
  TH1* r2_22_vsSub;
  TH1* nudyn_12_vsSub;
  TH1* f1_1Eff_vsSub;
  TH1* f1_2Eff_vsSub;
  TH1* f2_11Eff_vsSub;
  TH1* f2_12Eff_vsSub;
  TH1* f2_22Eff_vsSub;
  TH1* r2_11Eff_vsSub;
  TH1* r2_12Eff_vsSub;
  TH1* r2_22Eff_vsSub;
  TH1* nudyn_12Eff_vsSub;

  TH1* f1_1Ratio_vsSub;
  TH1* f1_2Ratio_vsSub;
  TH1* f2_11Ratio_vsSub;
  TH1* f2_12Ratio_vsSub;
  TH1* f2_22Ratio_vsSub;
  TH1* r2_11Ratio_vsSub;
  TH1* r2_12Ratio_vsSub;
  TH1* r2_22Ratio_vsSub;
  TH1* nudyn_12Ratio_vsSub;

  TH1* f1_1RatioOptB_vsSub;
   TH1* f1_2RatioOptB_vsSub;
   TH1* f2_11RatioOptB_vsSub;
   TH1* f2_12RatioOptB_vsSub;
   TH1* f2_22RatioOptB_vsSub;
   TH1* r2_11RatioOptB_vsSub;
   TH1* r2_12RatioOptB_vsSub;
   TH1* r2_22RatioOptB_vsSub;
   TH1* nudyn_12RatioOptB_vsSub;


  TH1* ef1_1_vsSub;
  TH1* ef1_2_vsSub;
  TH1* ef2_11_vsSub;
  TH1* ef2_12_vsSub;
  TH1* ef2_22_vsSub;
  TH1* er2_11_vsSub;
  TH1* er2_12_vsSub;
  TH1* er2_22_vsSub;
  TH1* enudyn_12_vsSub;
  TH1* ef1_1Eff_vsSub;
  TH1* ef1_2Eff_vsSub;
  TH1* ef2_11Eff_vsSub;
  TH1* ef2_12Eff_vsSub;
  TH1* ef2_22Eff_vsSub;
  TH1* er2_11Eff_vsSub;
  TH1* er2_12Eff_vsSub;
  TH1* er2_22Eff_vsSub;
  TH1* enudyn_12Eff_vsSub;

  TH1* ef1_1Ratio_vsSub;
  TH1* ef1_2Ratio_vsSub;
  TH1* ef2_11Ratio_vsSub;
  TH1* ef2_12Ratio_vsSub;
  TH1* ef2_22Ratio_vsSub;
  TH1* er2_11Ratio_vsSub;
  TH1* er2_12Ratio_vsSub;
  TH1* er2_22Ratio_vsSub;
  TH1* enudyn_12Ratio_vsSub;

 ClassDef(StatStudyHistograms,0)
};

#endif //CAP__StatStudyHistograms
