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

namespace CAP
{

double  DeltaPhiWidth2DFitFunction(double  *x, double  *par);
double  CentralPeak2DFitFunction(double  *x, double  *par);
double  Global2DFitFunction(double  *x, double  *par);

class BidimGaussFitter : public Plotter
{
  public:
  BidimGaussFitter(const String & _name,
                   const Configuration &     _configuration,
                   Severity                  _requiredLevel);
  BidimGaussFitter(const BidimGaussFitter& source);
  virtual ~BidimGaussFitter();
  const BidimGaussFitter & operator=(const BidimGaussFitter & source);

  void initialize();
  void flowFit(TH2 *data,  BidimGaussFitconst Configuration & fitConfig);
  void fullFit(TH2 *data,  BidimGaussFitconst Configuration & fitConfig);

  void addParameters(TF2* f, int first, int last, double value=0.0);
  void addParameterLimits(TF2* f, const BidimGaussFitconst Configuration & fitConfig, int first, int last, double scale);
  void releaseAllParameters(TF2* f);
  void releaseParameters(TF2* f, int first, int last);
  void fixUserParameters(TF1 *f, const BidimGaussFitconst Configuration & fitConfig);
  void fixPeakParameters(TF2* f, double a,  double sigmaEta, double sigmaPhi, double gammaEta, double gammaPhi);
  void fixFlowParameters(TF2* f,
                         double a0, double a1, double a2, double a3, double a4, double a5, double a6,
                         double a2eta, double a3eta, double a2eta2, double a3eta2);
  void fixFlowParametersToZero(TF2* f);
  void setFunctionParameterNames(TF2* f, const BidimGaussFitconst Configuration & fitConfig);
  void setFunctionParameterNamesFlowOnly(TF2* f, const BidimGaussFitconst Configuration & fitConfig, double initialValue=1.0E-2);
  void initializePeakFitParameters(TF2 * f, const BidimGaussFitconst Configuration & fitConfig, double scale);
  void initializeFlowFitParameters(TF2 * f, const BidimGaussFitconst Configuration & fitConfig, double scale);
  void initializeAllFitParameters(TF2 * f, const BidimGaussFitconst Configuration & fitConfig, double scale);

  void setOuterRangeByBin(TH2* h, const BidimGaussFitconst Configuration & fitConfig);
  bool isFlowPresent(BidimGaussFitResult & fitResult);

  //TH2 *injectErrors(TH2 *h2, Float_t sigma);

  void projectHistoAndFit(TH2 *dataHist,
                          TH2 *fitHist,
                          double xLow,
                          double xHigh,
                          double yLow,
                          double yHigh,
                          const BidimGaussFitconst Configuration & fitConfig,
                          const String  & label);
  
  void projectFlowComponents(TH2 *dataHist,
                             TF2 *f,
                             double xLow,
                             double xHigh,
                             double yLow,
                             double yHigh,
                             const BidimGaussFitconst Configuration & fitConfig,
                             const String  & label);



  TH2* subtractEtaSides(TH2* dataHist,
                        TF2 *flowFitFct,
                        const BidimGaussFitconst Configuration & fitConfig);
  TH1* cloneAndReset(const TH1* h, const String  & hName);
  TH1* cloneAndSetWithFct(const TH1* h,  TF1* f, const String  & hName);
  TH2* cloneAndReset(const TH2* h, const String  & hName);
  TH2* cloneAndSetWithFct(const TH2* h,  TF2* f, const String  & hName);
  void divideByHistoErrors(const TH2* numerator, const TH2* denominator, TH2* target);

  void calculateResidualHistos(const String  & baseName,
                               const String  & baseTitle,
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

  String histBaseTitle;
  String histBaseName;
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


  String ** flowLegends;
  String ** flowLegends2;
  String ** fullLegends;
  String ** peakLegends;
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
  String etaProjectName;
  String phiProjectName;
  TH1 * etaProjections[10];
  TH1 * phiProjections[10];

 
  Configuration &  canvasConfig1DLinear;
  Configuration &  canvasConfig2DLinear;
  GraphConfiguration  ** graphConfigs1D;
  GraphConfiguration  ** graphConfigs2D;
  TH1     ** histograms;
  String ** legends;
  String ** options;
  String * lineOption;
  String * pointOption;



  ClassDef(BidimGaussFitter,0)
  };

} // namespace CAP

#endif /* CAP__BidimGaussFitFunction */
