// Author: Victor Gonzalez  Dec 19, 2020

/*************************************************************************
 * Copyright (C) 2019, Victor Gonzalez, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/
#ifndef CAP__BidimGaussFitFunction
#define CAP__BidimGaussFitFunction
#include "Plotter.hpp"
#include "BidimGaussFitConfiguration.hpp"
#include "BidimGaussFitResult.hpp"
#include "TString.h"
#include "TF2.h"
#include "TMatrixDSym.h"
using std::cout;
using std::endl;
using std::ofstream;


double  DeltaPhiWidth2DFitFunction(double  *x, double  *par);
double  CentralPeak2DFitFunction(double  *x, double  *par);
double  Global2DFitFunction(double  *x, double  *par);

class BidimGaussFitter : public Plotter
{
  public:
  BidimGaussFitter(const TString & _name,
                   Configuration &     _configuration,
                   Severity                  _requiredLevel);
  BidimGaussFitter(const BidimGaussFitter& source);
  virtual ~BidimGaussFitter();
  const BidimGaussFitter & operator=(const BidimGaussFitter & source);

  void initialize();
  void flowFit(TH2 *data,  BidimGaussFitConfiguration & fitConfig);
  void fullFit(TH2 *data,  BidimGaussFitConfiguration & fitConfig);

  void setParameters(TF2* f, int first, int last, double value=0.0);
  void setParameterLimits(TF2* f, const BidimGaussFitConfiguration & fitConfig, int first, int last, double scale);
  void releaseAllParameters(TF2* f);
  void releaseParameters(TF2* f, int first, int last);
  void fixUserParameters(TF1 *f, const BidimGaussFitConfiguration & fitConfig);
  void fixPeakParameters(TF2* f, double a,  double sigmaEta, double sigmaPhi, double gammaEta, double gammaPhi);
  void fixFlowParameters(TF2* f,
                         double a0, double a1, double a2, double a3, double a4, double a5, double a6,
                         double a2eta, double a3eta, double a2eta2, double a3eta2);
  void fixFlowParametersToZero(TF2* f);
  void setFunctionParameterNames(TF2* f, const BidimGaussFitConfiguration & fitConfig);
  void setFunctionParameterNamesFlowOnly(TF2* f, const BidimGaussFitConfiguration & fitConfig, double initialValue=1.0E-2);
  void initializePeakFitParameters(TF2 * f, const BidimGaussFitConfiguration & fitConfig, double scale);
  void initializeFlowFitParameters(TF2 * f, const BidimGaussFitConfiguration & fitConfig, double scale);
  void initializeAllFitParameters(TF2 * f, const BidimGaussFitConfiguration & fitConfig, double scale);

  void setOuterRangeByBin(TH2* h, const BidimGaussFitConfiguration & fitConfig);
  bool isFlowPresent(BidimGaussFitResult & fitResult);

  //TH2 *injectErrors(TH2 *h2, Float_t sigma);

  void projectHistoAndFit(TH2 *dataHist,
                          TH2 *fitHist,
                          double xLow,
                          double xHigh,
                          double yLow,
                          double yHigh,
                          const BidimGaussFitConfiguration & fitConfig,
                          const TString  & label);
  
  void projectFlowComponents(TH2 *dataHist,
                             TF2 *f,
                             double xLow,
                             double xHigh,
                             double yLow,
                             double yHigh,
                             const BidimGaussFitConfiguration & fitConfig,
                             const TString  & label);



  TH2* subtractEtaSides(TH2* dataHist,
                        TF2 *flowFitFct,
                        const BidimGaussFitConfiguration & fitConfig);
  TH1* cloneAndReset(const TH1* h, const TString  & hName);
  TH1* cloneAndSetWithFct(const TH1* h,  TF1* f, const TString  & hName);
  TH2* cloneAndReset(const TH2* h, const TString  & hName);
  TH2* cloneAndSetWithFct(const TH2* h,  TF2* f, const TString  & hName);
  void divideByHistoErrors(const TH2* numerator, const TH2* denominator, TH2* target);

  void calculateResidualHistos(const TString  & baseName,
                               const TString  & baseTitle,
                               TH2* data,
                               TH2* fit,
                               TH2*& residuals,
                               TH2*& residualsNorm,
                               TH2*& residualsRatio);

  void calculateChi2(TH2*h,
                     int xFirst, int xLast,
                     int yFirst, int yLast,
                     int xFirstEx, int xLastEx,
                     int yFirstEx, int yLastEx,
                     double & chi2,
                     double & ndf);

  TString histBaseTitle;
  TString histBaseName;
  BidimGaussFitResult  flowFitResult;
  BidimGaussFitResult  fullFitResult;
  BidimGaussFitResult  peakFitResult;

  TH2 * dataMinusFlow;
  TH2 * flowFitOnlyHist;
  TH1 * datMinusFlowEtaProj;
  TH1 * datMinusFlowPhiProj;

  TH2 ** flowFitHist2D;
  TH2 * flowFitHist;
  TH2 * flowFitResiduals;
  TH2 * flowFitResidualsNorm;
  TH2 * flowFitResidualsRatio;

  TH2 * fullFitHist;
  TH2 * fullFitResiduals;
  TH2 * fullFitResidualsNorm;
  TH2 * fullFitResidualsRatio;

  TH2 * peakHist;
  TH2 * peakFitHist;
  TH2 * peakFitResiduals;
  TH2 * peakFitResidualsNorm;
  TH2 * peakFitResidualsRatio;

  TF2 * fullFitFct;
  TF2 * flowFitFct;
  TF2 * peakFitFct;


  TString ** flowLegends;
  TString ** flowLegends2;
  TString ** fullLegends;
  TString ** peakLegends;
  TH1 ** flowEtaProj;
  TH1 ** flowPhiProj;
  TH1 ** flowEtaProj2;
  TH1 ** flowPhiProj2;
  TH1 ** fullEtaProj;
  TH1 ** fullPhiProj;
  TH1 ** peakEtaProj;
  TH1 ** peakPhiProj;

  int   nEtaProjections;
  int   nPhiProjections;
  TString etaProjectName;
  TString phiProjectName;
  TH1 * etaProjections[10];
  TH1 * phiProjections[10];

 
  CanvasConfiguration &  canvasConfig1DLinear;
  CanvasConfiguration &  canvasConfig2DLinear;
  GraphConfiguration  ** graphConfigs1D;
  GraphConfiguration  ** graphConfigs2D;
  TH1     ** histograms;
  TString ** legends;
  TString ** options;
  TString * lineOption;
  TString * pointOption;



  ClassDef(BidimGaussFitter,0)
  };

#endif /* CAP__BidimGaussFitFunction */
