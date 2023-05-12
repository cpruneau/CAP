// Author: Victor Gonzalez  Dec 19, 2020

/*************************************************************************
 * Copyright (C) 2019, Victor Gonzalez, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class Task
 \ingroup CAP
 
 Class defining Event BidimGaussFitter
 */
#include "TStyle.h"
#include "BidimGaussFitter.hpp"
using CAP::BidimGaussFitter;
ClassImp(BidimGaussFitter);


// ============================================================================================================
// 16 parameters function
// ============================================================================================================
// 0  : a        : peak amplitude
// 1  : omegaEta : longitudinal peak width (eta)
// 2  : omegaPhi : azimuthal peak witdh (phi)
// 3  : gammaEta : generalized gaussian exponent -- eta
// 4  : gammaPhi : generalized gaussian exponent -- eta
// 5  : a0       : constant offset
// 6  : a1       : first harmonic coefficient
// 7  : a2       : second harmonic coefficient
// 8  : a3       : third harmonic coefficient
// 9  : a4       : fourth harmonic coefficient
// 10  : a5       : fift harmonic coefficient
// 11 : a6       : sixth harmonic coefficient
// 12 : a2LinEta : secong harmonic coefficient * fabsolute(eta)
// 13 : a3LinEta : third harmonic coefficient * fabsolute(eta)
// 14 : a2SqEta  : second harmonic coefficient * square(fabsolute(eta))
// 15 : a3SqEta  : third harmonic coefficient * square(fabsolute(eta))
// ============================================================================================================
double FlowPlusGen2DGaussianFunction(double *x, double *par)
{
  double eta = x[0];
  double etaSq = eta*eta;
  double phi   = x[1];
  // harmonic components
  double cos1Phi = cos(phi);
  double cos2Phi = cos(2.0*phi);
  double cos3Phi = cos(3.0*phi);
  double cos4Phi = cos(4.0*phi);
  double cos5Phi = cos(5.0*phi);
  double cos6Phi = cos(6.0*phi);
  // generalized gaussian peak
  double norm1  = par[3]/2.0/par[1]/TMath::Gamma(1.0/par[3]);
  double norm2  = par[4]/2.0/par[2]/TMath::Gamma(1.0/par[4]);
  double arg1   = absolute(eta/par[1]);
  double arg2a  = absolute(phi/par[2]);
  double arg2b  = absolute((phi-twoPi())/par[2]);
  double peakX  = norm1 * exp(-(power(arg1,par[3]) ));
  double peakY  = norm2 * ( exp(-(power(arg2a,par[4]))) + exp(-(power(arg2b,par[4]))));
  double result = par[0]*peakX*peakY;
  result += par[5];
  result += par[6]*cos1Phi;
  result += par[7]*cos2Phi;
  result += par[8]*cos3Phi;
  result += par[9]*cos4Phi;
  result += par[10]*cos5Phi;
  result += par[11]*cos6Phi;
  /* the eta dependency of vn */
  result += par[12]*eta*cos2Phi;
  result += par[13]*eta*cos3Phi;
  result += par[14]*etaSq*cos2Phi;
  result += par[15]*etaSq*cos3Phi;
  return result;
}

// ============================================================================================================
// 11 parameters function
// ============================================================================================================
// 0  : a0        : constant offset
// 1  : a1        : first harmonic coefficient
// 2  : a2        : second harmonic coefficient
// 3  : a3        : third harmonic coefficient
// 4  : a4        : fourth harmonic coefficient
// 5  : a5        : fift harmonic coefficient
// 6  : a6        : sixth harmonic coefficient
// 7  : a2LinEta  : secong harmonic coefficient * fabsolute(eta)
// 8  : a3LinEta  : third harmonic coefficient * fabsolute(eta)
// 9  : a2SqEta   : second harmonic coefficient * square(fabsolute(eta))
// 10 : a3SqEta   : third harmonic coefficient * square(fabsolute(eta))
// ============================================================================================================
double Flow2DFunction(double *x, double *par)
{
  double eta   = absolute(x[0]);
  double etaSq = eta*eta;
  double phi   = x[1];
  double cos1Phi = cos(phi);
  double cos2Phi = cos(2.0*phi);
  double cos3Phi = cos(3.0*phi);
  double cos4Phi = cos(4.0*phi);
  double cos5Phi = cos(5.0*phi);
  double cos6Phi = cos(6.0*phi);
  double result;
  /* the flow part */
  result =  par[0];
  result += par[1]*cos1Phi;
  result += par[2]*cos2Phi;
  result += par[3]*cos3Phi;
  result += par[4]*cos4Phi;
  result += par[5]*cos5Phi;
  result += par[6]*cos6Phi;
  /* the eta dependency of vn */
  result += par[7]*eta*cos2Phi;
  result += par[8]*eta*cos3Phi;
  result += par[9]*etaSq*cos2Phi;
  result += par[10]*etaSq*cos3Phi;
  return result;
}

// ============================================================================================================
// 5 parameters function
// ============================================================================================================
// 0  : a        : peak amplitude
// 1  : omegaEta : longitudinal peak width (eta)
// 2  : omegaPhi : azimuthal peak witdh (phi)
// 3  : gammaEta : generalized gaussian exponent -- eta
// 4  : gammaPhi : generalized gaussian exponent -- etadouble Gen2DGaussianFunction(double *x, double *par)
// 5  : a0       : constant offset
double Gen2DGaussianFunction(double *x, double *par)
{
  double eta = x[0];
  double phi = x[1];
  double norm1  = par[3]/2.0/par[1]/TMath::Gamma(1.0/par[3]);
  double norm2  = par[4]/2.0/par[2]/TMath::Gamma(1.0/par[4]);
  double arg1   = absolute(eta/par[1]);
  double arg2a  = absolute(phi/par[2]);
  double arg2b  = absolute((phi-twoPi())/par[2]);
  double peakX   = norm1 * exp(-(power(arg1,par[3]) ));
  double peakY   = norm2 * ( exp(-(power(arg2a,par[4]))) + exp(-(power(arg2b,par[4]))));
  double result = par[0]*peakX*peakY;
  result += par[5];  // allow for a residual offset
  return result;
}

/* 20 parameters function */
double Global2DFitFunction(double *x, double *par)
{
  return FlowPlusGen2DGaussianFunction(x,par) + Gen2DGaussianFunction(x,&par[17]);
}


BidimGaussFitter::BidimGaussFitter(const String & _name,
                                   const Configuration &     _configuration,
                                   Severity                  _requiredLevel)
:
Plotter(_name,_configuration,_requiredLevel),
histBaseTitle(""),
histBaseName(""),
flowFitResult(),
fullFitResult(),
peakFitResult(),
canvasConfig1DLinear(nullptr),
canvasConfig2DLinear(nullptr),
graphConfigs1D(nullptr),
graphConfigs2D(nullptr),
histograms(nullptr),
legends(nullptr),
options(nullptr),
lineOption(nullptr),
pointOption(nullptr)
{
  initialize();
}

BidimGaussFitter::BidimGaussFitter(const BidimGaussFitter& source)
:
Plotter(source),
histBaseTitle(source.histBaseTitle),
histBaseName(source.histBaseName),
flowFitResult(source.flowFitResult),
fullFitResult(source.fullFitResult),
peakFitResult(source.peakFitResult),
canvasConfig1DLinear(nullptr),
canvasConfig2DLinear(nullptr),
graphConfigs1D(nullptr),
graphConfigs2D(nullptr),
histograms(nullptr),
legends(nullptr),
options(nullptr),
lineOption(nullptr),
pointOption(nullptr)
{
  initialize();
}

BidimGaussFitter::~BidimGaussFitter()
{
  delete   canvasConfig1DLinear;
  delete   canvasConfig2DLinear;
  delete[] graphConfigs1D;
  delete[] graphConfigs2D;
  delete[] histograms;
  delete[] legends;
  delete[] options;
  delete   lineOption;
  delete   pointOption;

  delete[] flowLegends;
  delete[] flowLegends2;
  delete[] fullLegends;
  delete[] peakLegends;
  delete[] flowEtaProj;
  delete[] flowPhiProj;
  delete[] flowEtaProj2;
  delete[] flowPhiProj2;
  delete[] fullEtaProj;
  delete[] fullPhiProj;
  delete[] peakEtaProj;
  delete[] peakPhiProj;

}

const BidimGaussFitter & BidimGaussFitter::operator=(const BidimGaussFitter & source)
{
  if (this!=&source)
    {
    histBaseTitle   = source.histBaseTitle;
    histBaseName    = source.histBaseName;
    flowFitResult   = source.flowFitResult;
    fullFitResult   = source.fullFitResult;
    peakFitResult   = source.peakFitResult;
    }
  return *this;
}

void BidimGaussFitter::initialize()
{
  cout << "BidimGaussFitter::initialize() Initialization with " << endl;
  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);

  canvasConfig1DLinear = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  canvasConfig2DLinear = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  graphConfigs1D = new GraphConfiguration*[20];
  graphConfigs2D = new GraphConfiguration*[20];
  for (int i=0;i<20;i++)
  {
  graphConfigs1D[i] = new GraphConfiguration(1,i); graphConfigs1D[i]->plotOption = "HIST P";
  graphConfigs2D[i] = new GraphConfiguration(2,0); graphConfigs2D[i]->plotOption = "SURF3Z";
  }

  histograms  = new TH1*[20];
  legends     = new TString*[20];
  options     = new TString*[20];
  lineOption  = new TString("L");
  pointOption = new TString("P");
  etaProjectName = "_EtaProj_";
  phiProjectName = "_PhiProj_";
  flowLegends  = new TString*[5];
  flowLegends2 = new TString*[12];
  fullLegends  = new TString*[5];
  peakLegends  = new TString*[5];
  flowEtaProj  = new TH1*[5];
  flowPhiProj  = new TH1*[5];
  flowEtaProj2 = new TH1*[16];
  flowPhiProj2 = new TH1*[16];
  fullEtaProj  = new TH1*[5];
  fullPhiProj  = new TH1*[5];
  peakEtaProj  = new TH1*[5];
  peakPhiProj  = new TH1*[5];
  flowFitHist2D  = new TH2*[16];
  flowLegends[0] = new TString("Data");
  flowLegends[1] = new TString("Flow Fit");
  flowLegends[2] = new TString("Residuals");
  flowLegends[3] = new TString("Normalized Residuals");
  flowLegends[4] = new TString("Residuals Ratio");
  flowLegends2[0] = new TString("Data");
  flowLegends2[1] = new TString("Flow-Fit");
  flowLegends2[2] = new TString("a_{0}");
  flowLegends2[3] = new TString("a_{1}");
  flowLegends2[4] = new TString("a_{2}");
  flowLegends2[5] = new TString("a_{3}");
  flowLegends2[6] = new TString("a_{4}");
  flowLegends2[7] = new TString("a_{5}");
  flowLegends2[8] = new TString("a_{6}");
  flowLegends2[9] = new TString("a_{2}^{#eta}");
  flowLegends2[10] = new TString("a_{3}^{#eta}");
  flowLegends2[11] = new TString("a_{2}^{#eta^{2}}");
  flowLegends2[12] = new TString("a_{3}^{#eta^{2}}");

  fullLegends[0] = new TString("Data");
  fullLegends[1] = new TString("Fit");
  fullLegends[2] = new TString("Residuals");
  fullLegends[3] = new TString("Normalized Residuals");
  fullLegends[4] = new TString("Residuals Ratio");
  peakLegends[0] = new TString("Peak");
  peakLegends[1] = new TString("Fit");
  peakLegends[2] = new TString("Residuals");
  peakLegends[3] = new TString("Normalized Residuals");
  peakLegends[4] = new TString("Residuals Ratio");

}



//TH2 *BidimGaussFitter::injectErrors(TH2 *h2, float sigma)
//{
//  TH2 *h = (TH2 *) h2->Clone(Form("%s_err", h2->GetName()));
//  TRandom *rsave = gRandom;
//  TRandom *r2    = new TRandom2();
//  for (int   ix = 0; ix < h2->GetNbinsX(); ix++)
//  {
//  for (int   iy = 0; iy < h2->GetNbinsY(); iy++)
//    {
//    h->SetBinError(ix+1,iy+1,r2->Gaus(sigma)*h2->GetBinContent(ix+1,iy+1));
//    }
//  }
//  gRandom = rsave;
//  delete r2;
//  return h;
//}


// ================================================================================================
// Project the given 2D data and fit histograms onto the x and y axes.
// The projection is limited by the range determined by fitConfig
// ================================================================================================
void BidimGaussFitter::projectHistoAndFit(TH2 *dataHist,
                                          TH2 *fitHist,
                                          double xLow,
                                          double xHigh,
                                          double yLow,
                                          double yHigh,
                                          const BidimGaussFitconst Configuration & fitConfig,
                                          const Configuration & label)
{
  String baseName = dataHist->GetName();
  baseName += label;

//  int xLowBin  = dataHist->GetXaxis()->FindBin( xLow);
//  int xHighBin = dataHist->GetXaxis()->FindBin( xHigh);
//  int yLowBin  = dataHist->GetYaxis()->FindBin( yLow);
//  int yHighBin = dataHist->GetYaxis()->FindBin( yHigh);
//  dataHist->GetXaxis()->SetRange(xLowBin,xHighBin);
//  dataHist->GetYaxis()->SetRange(yLowBin,yHighBin);
//  fitHist ->GetXaxis()->SetRange(xLowBin,xHighBin);
//  fitHist ->GetYaxis()->SetRange(yLowBin,yHighBin);
//
//  double yScalingFactor = 1.0/double(xHighBin-xLowBin+1);
//  double xScalingFactor = 1.0/double(yHighBin-yLowBin+1);
//  vector<TH1*>    histograms;
//  VectorString  legends;
//  legends.push_back(TString("Data"));
//  legends.push_back(TString("Fit"));
//  //legends.push_back(TString("Data/Fit"));
//
//  dataHist->Scale(xScalingFactor);
//  fitHist ->Scale(xScalingFactor);

//  histograms.push_back(dataHist->ProjectionX(baseName+"_dataEtaProj",yLowBin, yHighBin));
//  histograms.push_back(fitHist ->ProjectionX(baseName+"_fitEtaProj", yLowBin, yHighBin));
//
//  graphConfigs1D[0]->plotOption = "HIST";
//  graphConfigs1D[1]->plotOption = "HIST";
//
//  plot(histograms,legends,graphConfigs1D,legends,
//       baseName+"_DataAndFitEtaProj",canvasConfig1DLinear,
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.zTitle,1.2, -1.2,
//       0.25, 0.15, 0.35, 0.35, 0.03);
//
//  histograms[2] = cloneAndReset(histograms[0],baseName+"_DataToFitRatioEtaProj");
//  histograms[2]->Divide(histograms[0],histograms[1]);
//  plot(1,baseName+"_DataToFitRatioEtaProj",canvasConfig1DLinear, graphConfigs1D,
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.zTitle,1.2, -1.2,
//       histograms+2,legends+2, 0.25, 0.15, 0.35, 0.35, 0.03);
//  histograms[3] = dataHist->ProjectionY(baseName+"_dataPhiProj",xLowBin, xHighBin); histograms[3]->Scale(yScalingFactor);
//  histograms[4] = fitHist ->ProjectionY(baseName+"_fitPhiProj", xLowBin, xHighBin); histograms[4]->Scale(yScalingFactor);
//  plot(2,baseName+"_DataAndFitPhiProj",canvasConfig1DLinear, graphConfigs1D,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.2, -1.2,
//       histograms+3,legends, 0.25, 0.15, 0.35, 0.35, 0.03);
//  histograms[5] = cloneAndReset(histograms[3],baseName+"_DataToFitRatioPhiProj");
//  histograms[5]->Divide(histograms[4],histograms[3]);
//  plot(1,baseName+"_DataToFitRatioPhiProj",canvasConfig1DLinear, graphConfigs1D,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.2, -1.2,
//       histograms+5,legends+2, 0.25, 0.15, 0.35, 0.35, 0.03);
}


void BidimGaussFitter::projectFlowComponents(TH2 *dataHist,
                                             TF2 *f,
                                             double xLow,
                                             double xHigh,
                                             double yLow,
                                             double yHigh,
                                             const BidimGaussFitconst Configuration & fitConfig,
                                             const Configuration & label)
{
  int nPar = f->GetNpar();
  int first;
  switch (nPar)
    {
      case  6: return; break;
      case 11: first = 0; break;
      case 16: first = 5; break;
      case 21: first = 5; break;
    }
  // save/cache the flow parameters
  double * flowParsCache = new double[11];
  for (int iPar=0; iPar<11; iPar++) flowParsCache[iPar] = f->GetParameter(first+iPar);
  String baseName = dataHist->GetName();
  baseName += label;
  int xLowBin  = dataHist->GetXaxis()->FindBin( xLow);
  int xHighBin = dataHist->GetXaxis()->FindBin( xHigh);
  int yLowBin  = dataHist->GetYaxis()->FindBin( yLow);
  int yHighBin = dataHist->GetYaxis()->FindBin( yHigh);
  double yScalingFactor = 1.0/double(xHighBin-xLowBin+1);
  double xScalingFactor = 1.0/double(yHighBin-yLowBin+1);
  int nHist = 0;
  int iHist = 0;
  // project data
  flowFitHist2D[iHist] = dataHist;
  flowEtaProj2[iHist]  = dataHist->ProjectionX(baseName+"_DataEtaProj",yLowBin, yHighBin); flowEtaProj2[iHist]->Scale(xScalingFactor);
  flowPhiProj2[iHist]  = dataHist->ProjectionY(baseName+"_DataPhiProj",xLowBin, xHighBin); flowPhiProj2[iHist]->Scale(yScalingFactor);
  iHist++;
  flowFitHist2D[iHist] = cloneAndSetWithFct(dataHist,f,baseName+"_Flow2D");
  flowEtaProj2[iHist]  = flowFitHist2D[iHist]->ProjectionX(baseName+"_FlowFitEtaProj",yLowBin, yHighBin); flowEtaProj2[iHist]->Scale(xScalingFactor);
  flowPhiProj2[iHist]  = flowFitHist2D[iHist]->ProjectionY(baseName+"_FlowFitPhiProj",xLowBin, xHighBin); flowPhiProj2[iHist]->Scale(yScalingFactor);

  iHist++;
  for (int iPar = 0; iPar<7; iPar++)
  {
  addParameters(f, 0, 11, 0.0); f->addParameter(iPar,flowParsCache[iPar]);
  String suffix = "_a";
  suffix += iPar;
  suffix += "_";
  flowFitHist2D[iHist] = cloneAndSetWithFct(dataHist,f,baseName+suffix+"_2D");
  flowEtaProj2[iHist] = flowFitHist2D[iHist]->ProjectionX(baseName+suffix+"_FlowFitaEtaProj",yLowBin, yHighBin); flowEtaProj2[iHist]->Scale(xScalingFactor);
  flowPhiProj2[iHist] = flowFitHist2D[iHist]->ProjectionY(baseName+suffix+"_FlowFitPhiProj", xLowBin, xHighBin); flowPhiProj2[iHist]->Scale(yScalingFactor);
  iHist++;
  }
  nHist = iHist;
  bool plotFlowComp2D       = false;
  bool plotFlowCompEtaProj  = false;
  bool plotFlowCompPhiProj  = true;
  if (plotFlowComp2D)
    {
    for (iHist=0;iHist<nHist;iHist++)
      {
      plot(baseName+flowFitHist2D[iHist]->GetName(), canvasConfig2DLinear, graphConfigs2D[0],
           fitConfig.xTitle, xLow, xHigh,
           fitConfig.yTitle, yLow, yHigh,
           fitConfig.zTitle, 1.0, -1.0,
           flowFitHist2D[iHist]);
      }
    }
  if (plotFlowCompEtaProj)
    {
    plot(nHist,baseName+"_EtaProjs", canvasConfig1DLinear, graphConfigs1D,
         fitConfig.xTitle, xLow, xHigh,
         fitConfig.zTitle, 1.2, -1.3,
         flowEtaProj2, flowLegends2, 0.2, 0.2, 0.4, 0.4, 0.03);
    }
  if (plotFlowCompPhiProj)
    {
    plot(3,baseName+"_PhiProjs12", canvasConfig1DLinear, graphConfigs1D,
         fitConfig.yTitle, yLow, yHigh,
         fitConfig.zTitle, 1.2, -1.2,
         flowPhiProj2, flowLegends2, 0.2, 0.2, 0.4, 0.4, 0.03);
    plot(nHist-3,baseName+"_PhiProjsAllVs", canvasConfig1DLinear, graphConfigs1D,
         fitConfig.yTitle, yLow, yHigh,
         fitConfig.zTitle, 1.2, -1.2,
         flowPhiProj2+3, flowLegends2+3, 0.2, 0.2, 0.4, 0.4, 0.03);
    }
  // restore flow parameters from local cache
  for (int iPar=0; iPar<11; iPar++) flowFitFct->addParameter(first+iPar,flowParsCache[iPar]);
  delete[] flowParsCache;
}

TH2* BidimGaussFitter::subtractEtaSides(TH2* dataHist,
                                        TF2 *flowFitFct,
                                        const BidimGaussFitconst Configuration & fitConfig)
{
  String baseName = dataHist->GetName();
  int xLowBin  = dataHist->GetXaxis()->FindBin(-fitConfig.deltaEtaOuterLimit);
  int xHighBin = dataHist->GetXaxis()->FindBin( fitConfig.deltaEtaOuterLimit);
  int yLowBin  = dataHist->GetYaxis()->FindBin( fitConfig.deltaPhiLowLimit);
  int yHighBin = dataHist->GetYaxis()->FindBin( fitConfig.deltaPhiHighLimit);
  double yScalingFactor = 1.0/double(xHighBin-xLowBin+1);
  double xScalingFactor = 1.0/double(yHighBin-yLowBin+1);
  dataMinusFlow   = cloneAndReset(dataHist,baseName+"_MinusFlow");
  flowFitOnlyHist = cloneAndSetWithFct(dataHist,flowFitFct,baseName+"_FlowFitOnly");
  dataMinusFlow->Add(flowFitOnlyHist, -1);
  datMinusFlowEtaProj = dataMinusFlow->ProjectionX(baseName+"_DataMinusFlowEtaProj", yLowBin, yHighBin); datMinusFlowEtaProj ->Scale(xScalingFactor);
  datMinusFlowPhiProj = dataMinusFlow->ProjectionY(baseName+"_DataMinusFlowPhiProj", xLowBin, xHighBin); datMinusFlowPhiProj ->Scale(yScalingFactor);
  plot(baseName+"_DataMinusFlow",canvasConfig2DLinear, graphConfigs2D[0],
       fitConfig.xTitle, 1.0, -1.0,
       fitConfig.yTitle, 1.0, -1.0,
       fitConfig.zTitle, 1.0, -1.0,
       dataMinusFlow);
  plot(baseName+"_DataMinusFlowEtaProj",canvasConfig1DLinear, graphConfigs1D[0],
       fitConfig.xTitle, 1.0, -1.0,
       fitConfig.zTitle, 1.0, -1.0,
       datMinusFlowEtaProj,"Data - #eta_{Side}", 0.1, 0.2, 0.4, 0.4, 0.05);
  plot(baseName+"_DataMinusFlowPhiProj",canvasConfig1DLinear, graphConfigs1D[0],
       fitConfig.xTitle, 1.0, -1.0,
       fitConfig.zTitle, 1.0, -1.0,
       datMinusFlowEtaProj,"Data - #eta_{Side}", 0.1, 0.2, 0.4, 0.4, 0.05);
  return dataMinusFlow;
}


// =========================================================================================================
// Carry out a 2D fit of the given data at large delta-eta where flow (if present) should dominate
// =========================================================================================================
void BidimGaussFitter::flowFit(TH2 *dataHist, BidimGaussFitconst Configuration & fitConfig)
{
  flowFitResult.reset();
  initializeFlowFitParameters(flowFitFct,fitConfig,1.0);
  for (int iLoop = 0; iLoop < fitConfig.nFitLoops; iLoop++)
  {
  flowFitResult.resultPtr = dataHist->Fit(flowFitFct, fitConfig.fitOption, "");
  if (!flowFitResult.resultPtr)
    {
    if (reportWarning("BidimGaussFitter","2D Gauss Fit","flowFit(const BidimGaussConfig &fitConfig,const TH2 *data)") )
      {
      cout << endl << "Flow fit was UNSUCCESSFUL after iLoop==" << iLoop << endl;
      }
    return;
    }
  if (flowFitResult.resultPtr->IsValid()) break;
  if (iLoop==fitConfig.nFitLoops)
    {
    if (reportWarning("BidimGaussFitter","2D Gauss Fit","flowFit(const BidimGaussConfig &fitConfig,const TH2 *data)") )
      {
      cout << endl << "Flow fit was UNSUCCESSFUL after iLoop==" << iLoop << endl;
      }
    return;
    }
  }
  flowFitResult.success = true;
  flowFitResult.saveResults(flowFitFct);
  if (reportInfo("BidimGaussFitter","2D Gauss Fit","flowFit(..)"))
    {
    flowFitResult.printResults(cout);
    }
}


/* main fit procedure */
/* by default a 0.13 radius area around (0,0) is excluded from the fit, i.e, 3x3 bins area centered in (0,0), see fitfunction.H */
/* to obtain this behavior set the sys parameter to kNoSystem or the cent parameter to -1 */
/* for different behavior see fitfunction.H for the different sys values and the excludeCentralBins.. structures and */
/* the excludedEtaBins, excludedPhiBins and excludeCentralRegion routines, all of them in the beginning of this source file, for */
/* its processing */
void BidimGaussFitter::fullFit(TH2 *data, BidimGaussFitconst Configuration & fitConfig)
{
   if (!data)
    {
    if (reportError("BidimGaussFitter","2D Gauss Fit","fullFit(const TH2 *data, const BidimGaussFitconst Configuration & fitConfig)"))
      {
      cout << "Input histogram 'data' is a null pointer. Abort.." << endl;
      }
    return;
    }

  TVirtualFitter::SetMaxIterations(fitConfig.maxIterations);
  TVirtualFitter::SetDefaultFitter(fitConfig.defaultFitter);


  histBaseTitle = data->GetTitle();
  histBaseName  = data->GetName();
  TH2 * dataHist = (TH2*) data->Clone(histBaseName+"_DataClone");


  fullFitResult.reset();
  // int nPeakPar     = 6;
  int nFlowPar     = 11;
  int nFlowParUsed = 7;
  // =========================================================
  // Fitting the large eta range..
  // =========================================================
  double xLow   = fitConfig.deltaEtaLimit;
  double xHigh  = fitConfig.deltaEtaOuterLimit;
  double yLow   = dataHist->GetYaxis()->GetXmin();
  double yHigh  = dataHist->GetYaxis()->GetXmax();
  int xLowBin  = dataHist->GetXaxis()->FindBin( xLow);
  int xHighBin = dataHist->GetXaxis()->FindBin( xHigh);
  int yLowBin  = dataHist->GetYaxis()->FindBin( yLow);
  int yHighBin = dataHist->GetYaxis()->FindBin( yHigh);
  dataHist->GetXaxis()->SetRange(xLowBin,xHighBin);
  dataHist->GetYaxis()->SetRange(yLowBin,yHighBin);
  flowFitHist = cloneAndReset(dataHist, histBaseName+"_flowFitHist");
  flowFitHist->GetXaxis()->SetRange(xLowBin,xHighBin);
  flowFitHist->GetYaxis()->SetRange(yLowBin,yHighBin);
  flowFitFct = new TF2(histBaseName+"flowFitFct",Flow2DFunction,xLow,xHigh,yLow,yHigh,nFlowPar);
  setFunctionParameterNames(flowFitFct,fitConfig);
  initializeFlowFitParameters(flowFitFct,fitConfig,1.0);
  flowFit(dataHist,fitConfig);
  if (!flowFitResult.success)
    {
    fullFitResult.success = false;
    return;
    }
  flowFitHist->Eval(flowFitFct,"A");
  plot(histBaseName+"_FlowData",canvasConfig2DLinear, graphConfigs2D[0],
       fitConfig.xTitle,xLow,xHigh,
       fitConfig.yTitle,yLow,yHigh,
       fitConfig.zTitle,1.5,-1.5,
       dataHist);
  plot(histBaseName+"_FlowFit",canvasConfig2DLinear, graphConfigs2D[0],
       fitConfig.xTitle,xLow,xHigh,
       fitConfig.yTitle,yLow,yHigh,
       fitConfig.zTitle,1.5,-1.5,
       flowFitHist);
  projectHistoAndFit(dataHist,flowFitHist,
                     fitConfig.deltaEtaLimit,        fitConfig.deltaEtaOuterLimit,
                     dataHist->GetYaxis()->GetXmin(),dataHist->GetYaxis()->GetXmax(),
                     fitConfig,"_FlowProj_");
  projectFlowComponents(dataHist,flowFitFct,
                        fitConfig.deltaEtaLimit,        fitConfig.deltaEtaOuterLimit,
                        dataHist->GetYaxis()->GetXmin(),dataHist->GetYaxis()->GetXmax(),
                        fitConfig,"_FlowComp_");

  // =========================================================
  // is there "flow" in the data?
  // =========================================================
  bool  isFlow = isFlowPresent(flowFitResult);

  xLow   = -fitConfig.deltaEtaLimit;
  xHigh  = fitConfig.deltaEtaLimit;
//  xLow   = -fitConfig.deltaEtaOuterLimit;
//  xHigh  = fitConfig.deltaEtaOuterLimit;
  yLow   = dataHist->GetYaxis()->GetXmin();
  yHigh  = -yLow+0.5; // dataHist->GetYaxis()->GetXmax();
  xLowBin  = dataHist->GetXaxis()->FindBin( xLow);
  xHighBin = dataHist->GetXaxis()->FindBin( xHigh);
  yLowBin  = dataHist->GetYaxis()->FindBin( yLow);
  yHighBin = dataHist->GetYaxis()->FindBin( yHigh);

  int nx = dataHist->GetNbinsX();
  int ny = dataHist->GetNbinsY();
  double error;

  flowFitOnlyHist = cloneAndReset(dataHist, histBaseName+"_flowOnlyHist");
  flowFitOnlyHist->GetXaxis()->SetRange(xLowBin,xHighBin);
  flowFitOnlyHist->GetYaxis()->SetRange(yLowBin,yHighBin);
  flowFitOnlyHist->Eval(flowFitFct,"A");

  for (int iEta=1; iEta<=nx; iEta++)
    {
    for (int iPhi=1; iPhi<=ny; iPhi++)
      {
      error = dataHist->GetBinError(iEta,iPhi);
      flowFitOnlyHist->SetBinError(iEta,iPhi,0,0);
      }
    }

  plot(histBaseName+"_flowOnlyHist2D",canvasConfig2DLinear, graphConfigs2D[0],
       fitConfig.xTitle,xLow,xHigh,
       fitConfig.yTitle,yLow,yHigh,
       fitConfig.zTitle,1.5,-1.5,
       flowFitOnlyHist);

  ///peakHist = cloneAndReset(dataHist, histBaseName+"_peakOnlyHist");
  dataHist->Add(flowFitOnlyHist,-1.0);

  // ================================
  // Prepare the full fit
  // ================================
//  for (int iEta=1; iEta<=nx; iEta++)
//    {
//    for (int iPhi=1; iPhi<=ny; iPhi++)
//      {
//      error = dataHist->GetBinError(iEta,iPhi);
//      dataHist->SetBinError(iEta,iPhi,0.05*error);
//      }
//    }
  //Exclude the (0,0) region from the fit..
  for (int iEta=14; iEta<=18; iEta++)
  {
  for (int iPhi=14; iPhi<=18; iPhi++)
    {
    dataHist->SetBinError(iEta,iPhi,1.0E10);
    }
  }

  dataHist->GetXaxis()->SetRange(xLowBin,xHighBin);
  dataHist->GetYaxis()->SetRange(yLowBin,yHighBin);
  fullFitFct = new TF2(histBaseName+"fullFitFct",FlowPlusGen2DGaussianFunction,xLowBin,xHighBin,yLowBin,yHighBin,16);
  setFunctionParameterNames(fullFitFct,fitConfig);
  // =========================================================
  // Do the full fit
  // =========================================================
  initializePeakFitParameters(fullFitFct,fitConfig,1.0);

  isFlow = false;
  if (isFlow)
    {
    if (reportInfo("BidimGaussFitter","2D Gauss Fit","fullFit(const BidimGaussConfig &fitConfig,const TH2 *data)"))
      {
      cout << endl;
      cout << "=====================================================================" << endl;
      cout << "Some flow is present!" << endl;
      cout << "=====================================================================" << endl;
      }
    for (int iPar=0; iPar<nFlowPar; iPar++)
      {
      double value = flowFitFct->GetParameter(iPar);
      if (true)
        {
        // fixing flow parameters
        fullFitFct->addParameter(5+iPar, value);
        fullFitFct->FixParameter(5+iPar, 0.0);
        }
      else
        {
        fullFitFct->addParameter(5+iPar, value);
        fullFitFct->SetParLimits(5+iPar, 0.8*value, 1.2*value);
        if (iPar>=nFlowParUsed) // if not using eta dependent coefficients.
          {
          fullFitFct->addParameter(5+iPar, 0.0);
          fullFitFct->FixParameter(5+iPar, 0.0);
          }
        }
      }
    }
  else
    {
    if (reportInfo("BidimGaussFitter","2D Gauss Fit","flowFit(const BidimGaussConfig &fitConfig,const TH2 *data)"))
      {
      cout << endl;
      cout << "=====================================================================" << endl;
      cout << "No flow present!" << endl;
      cout << "=====================================================================" << endl;
      }
    //double offset = flowFitFct->GetParameter(0);
    fullFitFct->addParameter(5, 0.0);  // flow is now removed, so is the offset
    //fullFitFct->FixParameter(5, offset);
    fullFitFct->SetParLimits(5, -0.001, 0.001);
    for (int iPar=1; iPar<nFlowPar; iPar++)  // allow for constant background.
      {
      fullFitFct->addParameter(5+iPar, 0.0);
      fullFitFct->FixParameter(5+iPar, 0.0);
      }
    //for (int iPar=0; iPar<10; iPar++) fullFitFct->FixParameter(5+iPar, flowFitFct->GetParameter(iPar));
    }

  //return;

  fullFitResult.reset();
  for (int iLoop = 0; iLoop < fitConfig.nFitLoops; iLoop++)
  {
  fullFitResult.resultPtr = dataHist->Fit(fullFitFct, fitConfig.fitOption, "");
  if (!fullFitResult.resultPtr )
    {
    if (reportWarning("BidimGaussFitter","2D Gauss Fit","fullFit(const BidimGaussConfig &fitConfig,const TH2 *data)") )
      {
      cout << endl;
      cout << "=====================================================================" << endl;
      cout << "Full fit was UNSUCCESSFUL. Stop." << endl;
      cout << "=====================================================================" << endl;
      }
    return;
    }
  if (fullFitResult.resultPtr->IsValid()) break;
  if (iLoop==fitConfig.nFitLoops)
    {
    if (reportWarning("BidimGaussFitter","2D Gauss Fit","flowFit(const BidimGaussConfig &fitConfig,const TH2 *data)") )
      {
      cout << endl << "Full fit was UNSUCCESSFUL after iLoop==" << iLoop << endl;
      }
    return;
    }
  }
  fullFitResult.saveResults(fullFitFct);
  if (reportInfo("BidimGaussFitter","2D Gauss Fit","fullFit(const BidimGaussConfig &fitConfig,const TH2 *data)"))
    {
    cout << endl;
    cout << "=====================================================================" << endl;
    cout << endl << "Full fit was SUCCESSFUL." << endl;
    cout << "=====================================================================" << endl;
    fullFitResult.printResults(cout);
    }

  plot(histBaseName+"_DataForFullFit",canvasConfig2DLinear, graphConfigs2D[0],
       fitConfig.xTitle,xLow,xHigh,
       fitConfig.yTitle,yLow,yHigh,
       fitConfig.zTitle,1.5,-1.5,
       dataHist);
  fullFitHist = cloneAndReset(dataHist, histBaseName+"_fullFitHist");
  fullFitHist->GetXaxis()->SetRange(xLowBin,xHighBin);
  fullFitHist->GetYaxis()->SetRange(yLowBin,yHighBin);
  fullFitHist->Eval(fullFitFct,"A");
  plot(histBaseName+"_FullFit",canvasConfig2DLinear, graphConfigs2D[0],
       fitConfig.xTitle,xLow,xHigh,
       fitConfig.yTitle,yLow,yHigh,
       fitConfig.zTitle,1.5,-1.5,
       fullFitHist);
  projectHistoAndFit(dataHist,fullFitHist,xLow,xHigh, yLow,yHigh,fitConfig,"_FullProj_");

  calculateResidualHistos(histBaseName+"_Full",histBaseTitle,
                          dataHist,fullFitHist,
                          fullFitResiduals,
                          fullFitResidualsNorm,
                          fullFitResidualsRatio);
  plot(histBaseName+"_FullFitRes",canvasConfig2DLinear, graphConfigs2D[0],
       fitConfig.xTitle,xLow,xHigh,
       fitConfig.yTitle,yLow,yHigh,
       fitConfig.zTitle,1.5,-1.5,
       fullFitResiduals);
  plot(histBaseName+"_FullFitResNorm",canvasConfig2DLinear, graphConfigs2D[0],
       fitConfig.xTitle,xLow,xHigh,
       fitConfig.yTitle,yLow,yHigh,
       fitConfig.zTitle,1.5,-1.5,
       fullFitResidualsNorm);
//  plot(histBaseName+"_FullFitResRatio",canvasConfig2DLinear, graphConfigs2D[0],
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.5,-1.5,
//       fullFitResidualsRatio);



//  for (int iPar=0; iPar<nFlowPar;iPar++)
//  {
//  flowFitFct->addParameter(iPar, fullFitFct->GetParameter(iPar+5));
//  }
//  flowFitOnlyHist = cloneAndReset(dataHist, histBaseName+"_flowOnlyHist");
//  flowFitOnlyHist->GetXaxis()->SetRange(xLowBin,xHighBin);
//  flowFitOnlyHist->GetYaxis()->SetRange(yLowBin,yHighBin);
//  flowFitOnlyHist->Eval(flowFitFct,"A");
//  plot(histBaseName+"_flowOnlyHist2D",canvasConfig2DLinear, graphConfigs2D[0],
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.5,-1.5,
//       flowFitOnlyHist);
//
//  peakHist = cloneAndReset(dataHist, histBaseName+"_peakOnlyHist");
//  peakHist->Add(dataHist,flowFitOnlyHist,1.0,-1.0);
//  plot(histBaseName+"_peakOnlyHist2D",canvasConfig2DLinear, graphConfigs2D[0],
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.5,-1.5,
//       peakHist);
//
//  peakFitFct = new TF2(histBaseName+"peakFitFct",Gen2DGaussianFunction,xLowBin,xHighBin,yLowBin,yHighBin,nPeakPar);
//  setFunctionParameterNames(peakFitFct,fitConfig);  // yyyyyy
//  releaseAllParameters(peakFitFct);
//  for (int iPar=0; iPar<5;iPar++)
//  {
//  double initialValue = fullFitFct->GetParameter(iPar);
//  peakFitFct->addParameter(iPar, initialValue);
//  peakFitFct->SetParLimits(iPar, initialValue/2.0, 2.0*initialValue);
//  }
//  peakFitFct->addParameter(5,0.00001);
//  peakFitFct->SetParLimits(5,-0.001,0.001);
//
//  peakFitResult.reset();
//  for (int iLoop = 0; iLoop < fitConfig.nFitLoops; iLoop++)
//  {
//  peakFitResult.resultPtr = peakHist->Fit(peakFitFct, fitConfig.fitOption, "");
//  if (!peakFitResult.resultPtr )
//    {
//    if (reportWarning("BidimGaussFitter","2D Gauss Fit","fullFit(const BidimGaussConfig &fitConfig,const TH2 *data)") )
//      {
//      cout << endl;
//      cout << "=====================================================================" << endl;
//      cout << "Peak fit was UNSUCCESSFUL. Stop." << endl;
//      cout << "=====================================================================" << endl;
//      }
//    return;
//    }
//  }
//  peakFitResult.saveResults(peakFitFct);
//
//  if (reportInfo("BidimGaussFitter","2D Gauss Fit","fullFit(const BidimGaussConfig &fitConfig,const TH2 *data)"))
//    {
//    cout << endl;
//    cout << "=====================================================================" << endl;
//    cout << "Peak fit was SUCCESSFUL." << endl;
//    cout << "=====================================================================" << endl;
//    peakFitResult.printResults(cout);
//    cout << endl;
//    cout << "=====================================================================" << endl;
//    }
//
//  peakFitHist = cloneAndReset(peakHist, histBaseName+"_peakFitHist");
//  peakFitHist->GetXaxis()->SetRange(xLowBin,xHighBin);
//  peakFitHist->GetYaxis()->SetRange(yLowBin,yHighBin);
//  peakFitHist->Eval(peakFitFct,"A");
//  plot(histBaseName+"_PeakFit",canvasConfig2DLinear, graphConfigs2D[0],
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.5,-1.5,
//       peakFitHist);
//  projectHistoAndFit(peakHist,peakFitHist,xLow,xHigh, yLow,yHigh,fitConfig,"_PeakProj_");
//
//  calculateResidualHistos(histBaseName+"_Peak",histBaseTitle,
//                          dataHist,peakFitHist,
//                          peakFitResiduals,
//                          peakFitResidualsNorm,
//                          peakFitResidualsRatio);
//  plot(histBaseName+"_PeakFitRes",canvasConfig2DLinear, graphConfigs2D[0],
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.5,-1.5,
//       peakFitResiduals);
//  plot(histBaseName+"_PeakFitResNorm",canvasConfig2DLinear, graphConfigs2D[0],
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.5,-1.5,
//       peakFitResidualsNorm);
//  plot(histBaseName+"_PeakFitResRatio",canvasConfig2DLinear, graphConfigs2D[0],
//       fitConfig.xTitle,xLow,xHigh,
//       fitConfig.yTitle,yLow,yHigh,
//       fitConfig.zTitle,1.5,-1.5,
//       peakFitResidualsRatio);

//  peakHist->GetYaxis()->SetRange(yLowBin,yHighBin);
//  peakHist->Eval(fullFitFct,"A");

  //projectFlowComponents(dataHist,flowFitFct,fitConfig);

//  releaseParameters(flowFitFct, 0, 10);
//  for (int   i=0; i<10; i++)
//  {
//  flowFitFct->ReleaseParameter(i);
//  if (func->GetParameter(i) > 0)
//    func->SetParLimits(i, func->GetParameter(i) * 0.8, func->GetParameter(i) * 1.2);
//  else
//    func->SetParLimits(i, func->GetParameter(i) * 1.2, func->GetParameter(i) * 0.8);
//  }
//  if (func->GetParameter(3) * 0.8 < fitConfig.betaLowerLimit && func->GetParameter(3) * 1.2 > fitConfig.betaUpperLimit) func->SetParLimits(3,fitConfig.betaLowerLimit,fitConfig.betaUpperLimit);
//  else if (func->GetParameter(3) * 0.8 < fitConfig.betaLowerLimit) func->SetParLimits(3,fitConfig.betaLowerLimit,func->GetParameter(3) * 1.2);
//  else if (func->GetParameter(3) * 1.2 > fitConfig.betaUpperLimit) func->SetParLimits(3,func->GetParameter(3) * 0.8,fitConfig.betaUpperLimit);
//  if (func->GetParameter(4) * 0.8 < fitConfig.betaLowerLimit && func->GetParameter(4) * 1.2 > fitConfig.betaUpperLimit) func->SetParLimits(4,fitConfig.betaLowerLimit,fitConfig.betaUpperLimit);
//  else if (func->GetParameter(4) * 0.8 < fitConfig.betaLowerLimit) func->SetParLimits(4,fitConfig.betaLowerLimit,func->GetParameter(4) * 1.2);
//  else if (func->GetParameter(4) * 1.2 > fitConfig.betaUpperLimit) func->SetParLimits(4,func->GetParameter(4) * 0.8,fitConfig.betaUpperLimit);
//  if (!isFlow)
//    {
//    for (int   i=6; i<11; i++)
//      {
//      /* properly set a fix parameter to zero value */
//      func->FixParameter(i, 0);
//      }
//    }
//  xAxisHist->SetRange(fitConfig.bwdDeltaEtaOuterLimitBin, fitConfig.fwdDeltaEtaOuterLimitBin);
//  fixUserParameters(func);
//  for (int   i = 0; i < fitConfig.nfitloops; i++)
//  {
//  fitResult.resultPtr = dataHist->Fit(func, fitConfig.fitOption, "");
//  }


//  double peakheight = 0.0;

//  if (useCentralGaussians)
//    {
//    //Info("FitFunction","STEP3b: fit the central spike with everything else fixed \n");
//    /* recover the errors for bins around (0,0) */
//    /* get the height of the peak to fit by difference with neighbors */
//    float neighavg = 0.0;
//    int   nbinsavg = 0;
//    if (fitConfig.useExcludeCentralRegion)
//      {
//      for (int   binx = xAxisHist->FindBin(-deltaEtaExclusionRegion); binx <= xAxisHist->FindBin(deltaEtaExclusionRegion); binx++) {
//        for (int   biny = yAxisHist->FindBin(-deltaEtaExclusionRegion); biny <= yAxisHist->FindBin(deltaEtaExclusionRegion); biny++) {
//          if (binx == xAxisHist->FindBin(-deltaEtaExclusionRegion)) {
//            nbinsavg++;
//            neighavg += dataHist->GetBinContent(binx,biny);
//          }
//          else if (binx == xAxisHist->FindBin(deltaEtaExclusionRegion)) {
//            nbinsavg++;
//            neighavg += dataHist->GetBinContent(binx,biny);
//          }
//          else if (biny == yAxisHist->FindBin(-deltaEtaExclusionRegion)) {
//            nbinsavg++;
//            neighavg += dataHist->GetBinContent(binx,biny);
//          }
//          else if (biny == yAxisHist->FindBin(deltaEtaExclusionRegion)) {
//            nbinsavg++;
//            neighavg += dataHist->GetBinContent(binx,biny);
//          }
//          dataHist->SetBinError(binx, biny, h->GetBinError(binx,biny));
//        }
//      }
//      }
//    neighavg /= nbinsavg;
//    peakheight = dataHist->GetBinContent(xAxisHist->FindBin(0.0), yAxisHist->FindBin(0.0)) - neighavg;
//
//    if (deltaEtaExclusionRegion != 0)
//      {
//      if (reportInfo("BidimGaussFitter","2D Gauss Fit","fullFit(..)"))
//        {
//        cout << endl
//        << " NOTE : Recovered bins at (0, 0) within the exclusion region  : " << deltaEtaExclusionRegion << endl;
//        }
//      }
//    fullFitFct = new TF2("fullFitFct", Global2DFitFunction, -fitConfig.deltaEtaOuterLimit,
//                           fitConfig.deltaEtaOuterLimit, fitConfig.deltaPhiLowLimit, fitConfig.deltaPhiHighLimit,20);
////    for (int   i=0;  i<11; i++) fullFitFct->FixParameter(i,flowFitFct->GetParameter(i));
////    for (int   i=11; i<11; i++)
////      {
////      fullFitFct->addParameter(i,0);
////      fullFitFct->FixParameter(i,0);
////      }
////    for (int   i=11; i<15; i++) fullFitFct->FixParameter(i,flowFitFct->GetParameter(i));
////    for (int   i=15; i<peakIndexBase; i++)
////      {
////      fullFitFct->addParameter(i,0);
////      fullFitFct->FixParameter(i,0);
////      }
//
//    //Info("FitFunction","Height of the spike: %f\n", peakheight);
//    if (peakheight < 0) peakheight = dataHist->GetBinContent(xAxisHist->FindBin(0.0), yAxisHist->FindBin(0.0));
//    fullFitFct->addParameter(peakIndexBase,    peakheight*0.2);
//    fullFitFct->SetParLimits(peakIndexBase,0,  peakheight);
//    fullFitFct->addParameter(peakIndexBase+1,  deltaEtaExclusionRegion*0.1);
//    fullFitFct->SetParLimits(peakIndexBase+1,  0.01,deltaEtaExclusionRegion*0.75);
//    fullFitFct->addParameter(peakIndexBase+2,  deltaEtaExclusionRegion*0.1);
//    fullFitFct->SetParLimits(peakIndexBase+2,  0.01,deltaEtaExclusionRegion*0.65);
//    /* let's fix pure gaussian for the time being */
//    fullFitFct->addParameter(peakIndexBase+3, 2.0);
//    fullFitFct->SetParLimits(peakIndexBase+3, 2.0, 2.0);
//    fullFitFct->addParameter(peakIndexBase+4, 2.0);
//    fullFitFct->SetParLimits(peakIndexBase+4, 2.0, 2.0);
//    delete func;
//    func = fullFitFct;
//
//    fixUserParameters(fullFitFct);
//    fullFitResult.fitFunction = fullFitFct;
//    fullFitResult.resultPtr   = dataHist->Fit(fullFitFct,fitConfig.fitOption, "");
//    fullFitResult.result      = fullFitResult.resultPtr;
//
//    if (fullFitResult.result != 0)
//      {
//      if (fullFitResult.resultPtr->IsValid())
//        {
//        if (reportWarning("BidimGaussFitter","2D Gauss Fit","fullFit(..)"))
//          {
//          cout << endl
//          << " Fit valid but fit status equal to : " << fullFitResult.resultPtr->Status() << endl
//          << "             while returned result : " << fullFitResult.result << endl;
//          }
//        fullFitResult.result = 0; // discarded the not fully correct fit result
//        }
//      }
////    fullFitFct->FixParameter(peakIndexBase,  fullFitFct->GetParameter(peakIndexBase));
////    fullFitFct->FixParameter(peakIndexBase+1,fullFitFct->GetParameter(peakIndexBase+1));
////    fullFitFct->FixParameter(peakIndexBase+2,fullFitFct->GetParameter(peakIndexBase+2));
////    fullFitFct->FixParameter(peakIndexBase+3,fullFitFct->GetParameter(peakIndexBase+3));
////    fullFitFct->FixParameter(peakIndexBase+4,fullFitFct->GetParameter(peakIndexBase+4));
//    }

//  if (isFlow && useEtaDependentFlowInFit)
//    {
//    if (reportWarning("BidimGaussFitter","2D Gauss Fit","fullFit(..)"))
//      {
//      cout << " STEP3b: fit everything, with limits, including flow eta dependence." << endl;
//      }
//    for (int   iPar=11; iPar<15; iPar++)
//      {
//      fullFitFct->ReleaseParameter(iPar);
//      fullFitFct->addParameter(iPar,-1.0E-3);
//      fullFitFct->SetParLimits(iPar,-1.0E-2,0.0);
//      }
//    fixUserParameters(fullFitFct);
//    for (int   i = 0; i < fitConfig.nfitloops; i++)
//      {
//      fullFitResult.resultPtr = dataHist->Fit(fullFitFct, fitConfig.fitOption, "");
//      }
//    fullFitResult.result = int  (fullFitResult.resultPtr);
//    if (fullFitResult.result != 0)
//      {
//      if (fullFitResult.resultPtr->IsValid())
//        {
//        if (reportWarning("BidimGaussFitter","2D Gauss Fit","fullFit(..)"))
//          {
//          cout << " Status is weird." << endl;
//          }
//        fullFitResult.result= 0; // discarded the not fully correct fit result
//        }
//      }
//    if (reportInfo("BidimGaussFitter","2D Gauss Fit","fullFit(..)"))
//      {
//      cout << endl
//      << "   Step3b: Fit result : " << fitResult << endl
//      << "                 Chi2 : " << func->GetChisquare() << endl
//      << "                 NDF : " << func->GetNDF() << endl;
//      << Recovered bins at (0, 0) within the exclusion region  : " << deltaEtaExclusionRegion << endl;
//      }
//    }
//  releaseAllParameters(fullFitFct);
//
//  fullFitFct->SetParLimits(1, fitConfig.sigmaLowerLimit, fitConfig.sigmaUpperLimit);
//  fullFitFct->SetParLimits(2, fitConfig.sigmaLowerLimit, fitConfig.sigmaUpperLimit);
//  fullFitFct->SetParLimits(3, fitConfig.betaLowerLimit, fitConfig.betaUpperLimit);
//  fullFitFct->SetParLimits(4, fitConfig.betaLowerLimit, fitConfig.betaUpperLimit);
//  if (useFlowInFit || isFlow)
//    {
//    initializeFlowParameters(fullFitFct);
//    }
//  else
//    {
//    fixFlowParametersToZero(fullFitFct);
//    }
//  for (int   i = 0; i < fitConfig.nfitloops; i++)
//  {
//  fullFitResult.resultPtr = dataHist->Fit(fullFitFct, fitConfig.fitOption, "");
//  }

//  projectEtaHistoAndFit(histInterest, fitMinusBGFct, "#Delta#eta",    "G_{2} - collectivity ((GeV/c)^{2})");
//  projectPhiHistoAndFit(histInterest, fitMinusBGFct, "#Delta#varphi", "G_{2} - collectivity ((GeV/c)^{2})");



//  setOuterRangeByBin(peakHist, fitConfig);
//  setOuterRangeByBin(dipHist,  fitConfig);
//  peakHist->Sumw2();
//  peakHist->SetTitle("After BG subtraction");
//  peakHist->Add(BG, -1);
//  peakHist->Draw("surf1");
//
//  dipHist->Sumw2();
//  dipHist->SetTitle("After fit subtraction");
//  dipHist->Add(func, -1);
//  dipHist->DrawCopy("surf1");
//
//  double yieldIntegral = peakHist->Integral("Width");
//  double integral = 0, integralError = 0;
//  double integralAll = 0, integralErrorAll = 0;
//  double centralExclusionForDip = 0.04; //0.08 for normal binning
//  if (deltaEtaExclusionRegion > centralExclusionForDip)
//    {
//    TH2* dipHistClone = (TH2*) dipHist->Clone("dipHistClone");
//    for (int xBin=dipHistClone->GetXaxis()->FindBin(-centralExclusionForDip); xBin<=dipHistClone->GetXaxis()->FindBin(centralExclusionForDip); xBin++)
//    for (int y=dipHistClone->GetYaxis()->FindBin(-centralExclusionForDip); y<=dipHistClone->GetYaxis()->FindBin(centralExclusionForDip); y++)
//      {
//      dipHistClone->SetBinContent(xBin, y, 0);
//      dipHistClone->SetBinError(xBin, y, 0);
//      }
//
//    integral = dipHistClone->IntegralAndError(dipHistClone->GetXaxis()->FindBin(-deltaEtaExclusionRegion),
//                                              dipHistClone->GetXaxis()->FindBin(deltaEtaExclusionRegion),dipyAxisHist->FindBin(-deltaEtaExclusionRegion),
//                                              dipyAxisHist->FindBin(deltaEtaExclusionRegion),
//                                              integralError,
//                                              "Width");
//    delete dipHistClone;
//    }
//  integralAll = dipHist->IntegralAndError(fitConfig.bwdDeltaEtaOuterLimitBin,fitConfig.fwdDeltaEtaOuterLimitBin,
//                                          fitConfig.deltaPhiLowLimitBin,fitConfig.deltaPhiHighLimitBin,
//                                          integralErrorAll,
//                                          "Width");



  //TAxis * yAxisHist = dataHist->GetYaxis();

  //double chi2 = 0, chi2FullWoMiddle = 0;
  //double ndf = 0,  ndfWoMiddle = 0,  ndfFullWoMiddle = 0;

//  if (reportInfo("BidimGaussFitter","2D Gauss Fit","fullFit(const BidimGaussConfig &fitConfig,const TH2 *data)"))
//    {
//    cout << endl << "Next step: calculateChi2()" << endl;
//    }
//
//  calculateChi2(fullFitResidualsNorm,
//                fitConfig.bwdDeltaEtaOuterLimitBin,  fitConfig.fwdDeltaEtaOuterLimitBin,
//                yAxisHist->FindBin(-0.8),            yAxisHist->FindBin(0.8),
//                -1, -1, -1, -1, chi2, ndf);
//  calculateChi2(fullFitResidualsNorm,
//                fitConfig.bwdDeltaEtaOuterLimitBin,fitConfig.fwdDeltaEtaOuterLimitBin,
//                yAxisHist->FindBin(-0.8),          yAxisHist->FindBin(0.8),
//                fitConfig.lowBinEtaInPatch,        fitConfig.highBinEtaInPatch,
//                fitConfig.lowBinPhiInPatch,        fitConfig.highBinPhiInPatch,
//                chi2WoMiddle, ndfWoMiddle);
//  calculateChi2(fullFitResidualsNorm,
//                fitConfig.bwdDeltaEtaOuterLimitBin, fitConfig.fwdDeltaEtaOuterLimitBin,
//                fitConfig.deltaPhiLowLimitBin,      fitConfig.deltaPhiHighLimitBin,
//                fitConfig.lowBinEtaInPatch,         fitConfig.highBinEtaInPatch,
//                fitConfig.lowBinPhiInPatch,         fitConfig.highBinPhiInPatch,
//                chi2FullWoMiddle,                   ndfFullWoMiddle);
//
//  ndf -= fullFitFct->GetNumberFreeParameters();
//  ndfWoMiddle -= fullFitFct->GetNumberFreeParameters();
//  ndfFullWoMiddle -= fullFitFct->GetNumberFreeParameters();
//
//
//  fullFitResult.chi2    = chi2;
//  fullFitResult.ndf     = ndf;
//  fullFitResult.chi2Ndf = chi2/ndf;
//  fullFitResult.chi2WoMiddle    = chi2WoMiddle;
//  fullFitResult.ndfWoMiddle     = ndfWoMiddle;
//  fullFitResult.chi2NdfWoMiddle = chi2WoMiddle/ndfWoMiddle;
//  fullFitResult.chi2FullWoMiddle = chi2FullWoMiddle;
//  fullFitResult.ndfFullWoMiddle  = ndfFullWoMiddle;
//  fullFitResult.chi2NdfFullWoMiddle = chi2FullWoMiddle/ndfFullWoMiddle;

  if (reportInfo("BidimGaussFitter","2D Gauss Fit","fullFit(const BidimGaussConfig &fitConfig,const TH2 *data)"))
    {
    cout << endl << "DoNE!!!!!!!!!!!!!!!!" << endl;
    }

 

}

bool BidimGaussFitter::isFlowPresent(BidimGaussFitResult & fitResult)
{
  bool result = false;
  double a0 = absolute(fitResult.a0);
  double a1 = absolute(fitResult.a1);
  double a2 = absolute(fitResult.a2);
  double a3 = absolute(fitResult.a3);
  double a4 = absolute(fitResult.a4);
  if (a0>1.0E-6)
    {
    double a1a0 = a1/a0;
    double a2a0 = a2/a0;
    double a3a0 = a3/a0;
    double a4a0 = a4/a0;
    result = !(a1a0<1e-2  && a2a0<1e-2  && a3a0<1e-2 && a4a0<1e-2);
    }
  else
    {
    result = !(a1<1e-2  &&  a2<1e-2 && a3<1e-2 && a4<1e-2);
    }
  return result;
}

void BidimGaussFitter::setOuterRangeByBin(TH2* h, const BidimGaussFitconst Configuration & fitConfig)
{
  h->GetXaxis()->SetRange(fitConfig.bwdDeltaEtaOuterLimitBin,    fitConfig.fwdDeltaEtaOuterLimitBin);
  h->GetYaxis()->SetRange(fitConfig.deltaPhiLowLimitBin, fitConfig.deltaPhiHighLimitBin);
  h->SetStats(false);
}


TH1* BidimGaussFitter::cloneAndReset(const TH1* h, const Configuration & hName)
{
  TH1 * clone = (TH1*) h->Clone(hName);
  clone->Reset();
  return clone;
}

TH1* BidimGaussFitter::cloneAndSetWithFct(const TH1* h,   TF1* f, const Configuration & hName)
{
  TH1 * clone = (TH1*) h->Clone(hName);
  clone->Reset();
  clone->Eval(f,"A");
  return clone;
}

TH2* BidimGaussFitter::cloneAndReset(const TH2* h, const Configuration & hName)
{
  TH2 * clone = (TH2*) h->Clone(hName);
  clone->Reset();
  return clone;
}

TH2* BidimGaussFitter::cloneAndSetWithFct(const TH2* h,   TF2* f, const Configuration & hName)
{
  TH2 * clone = (TH2*) h->Clone(hName);
  clone->Reset();
  clone->Eval(f,"A");
  return clone;
}


void BidimGaussFitter::divideByHistoErrors(const TH2* numerator, const TH2* denominator, TH2* target)
{
  int nx = numerator->GetNbinsX();
  int ny = numerator->GetNbinsY();
  for (int   ix=1; ix<=nx; ix++)
  {
  for (int   iy=1; iy<=ny; iy++)
    {
    double dem = denominator->GetBinError(ix,iy);
    if (dem > 0)
      {
      double num = numerator->GetBinContent(ix,iy);
      target->SetBinContent(ix,iy,num/dem);
      target->SetBinError(ix,iy,0.0);
      }
    else
      {
      target->SetBinContent(ix,iy,-9999.0);
      target->SetBinError(ix,iy,-9999.0);
      }
    }
  }
}


// ========================================================================================
// data : histogram containing data being fitted.
// fit  : histogram containing a pixelized version of the fit to the data.
// residuals: data - fit
// residualsNorm:  residuals/dataErrors
// residualsRatio: residuals/data
// ========================================================================================
void BidimGaussFitter::calculateResidualHistos(const String & baseName,
                                               const String & baseTitle,
                                               TH2* data,
                                               TH2* fit,
                                               TH2*& residuals,
                                               TH2*& residualsNorm,
                                               TH2*& residualsRatio)
{
  residuals = (TH2*) data->Clone(baseName+"_residualHist");
  residuals->Add(fit,-1.0);
  residuals->SetTitle(baseTitle+" Residuals");

  residualsNorm = (TH2*) cloneAndReset(residuals,baseName+"_normalizedResidualHist");
  residualsNorm->SetTitle("(Data - Fit) / Errors");
  divideByHistoErrors(residuals,data,residualsNorm);

  residualsRatio = cloneAndReset(data,"ResidualsRatioHist");
  residualsRatio->Divide(residuals,data);
  residualsRatio->SetTitle("(Data - Fit) / Data");
}

// =======================================================================
// Use the given histogram "h" as a normalized residue histogram
// and compute the chi2 of the fit between the given boundaries
// while excluding the region [xFirstEx,xLastEx]x[yFirstEx,yLastEx]
// The content of a bin  included in the calculation of the chi2 and ndf
// only if the error in that given bin is equal or larger than zero.
// Bins with a negative error are excluded from the calculation.
// =======================================================================
void  BidimGaussFitter::calculateChi2(TH2*h,
                                      int xFirst, int xLast,
                                      int yFirst, int yLast,
                                      int xFirstEx, int xLastEx,
                                      int yFirstEx, int yLastEx,
                                      double & chi2,
                                      double & ndf)
{
  chi2 = 0.0;
  ndf  = 0;
  double v, ev;
  for (int ix=xFirst; ix<=xLast; ix++)
  {
  for (int iy=yFirst; iy<=yLast; iy++)
    {
    if (ix>=xFirstEx && ix<=xLastEx &&
        iy>=yFirstEx && iy<=yLastEx) continue;
    ev = h->GetBinError(ix,iy);
    if (ev>=0.0)
      {
      v = h->GetBinContent(ix,iy);
      chi2 += v*v;
      ndf  += 1;
      }
    }
  }
}

// =========================================================================================================
// Use this function to set the name of the parameters of full fit function
// Parameters are named: a, wEta, wPhi, gammaEta, gammaPhi, a0, a2, a3, a4, a5, a6, lina2, lina3, sqa2, sqa3
// =========================================================================================================
void BidimGaussFitter::setFunctionParameterNames(TF2* f, const BidimGaussFitconst Configuration & fitConfig)
{
  cout << "BidimGaussFitter::setFunctionParameterNames() " << endl;
  int nPar  = f->GetNpar();
  cout << "BidimGaussFitter::setFunctionParameterNames()  nPar:" << nPar << endl;
  int first = 0;
  switch (nPar)
    {
      case  6: first = 0; break;
      case 11: first = 5; break;
      case 16: first = 0; break;
      default: return; break;
    }
  for (int iPar=0; iPar<nPar; iPar++)
  {
  f->SetParName(iPar,  *fitConfig.parameterNames[first+iPar]);
  f->addParameter(iPar,fitConfig.initialValues [first+iPar]);
  }
}


void BidimGaussFitter::releaseAllParameters(TF2* f)
{
  int nPar =  f->GetNpar();
  for (int iPar=0; iPar<nPar; iPar++) f->ReleaseParameter(iPar);
}


void BidimGaussFitter::releaseParameters(TF2* f, int first, int last)
{
  for (int iPar=first; iPar<=last; iPar++) f->ReleaseParameter(iPar);
}

// ====================================================================================
// Use this function to fix the user parameters to values selected by the user.
// ====================================================================================
void BidimGaussFitter::fixUserParameters(TF1 *f, const BidimGaussFitconst Configuration & fitConfig)
{
  int nPar =  f->GetNpar();
  int first;
  switch (nPar)
    {
      case  6: first = 0; break;  // peak fit
      case 11: first = 5; break;  // flow fit
      case 16: first = 0; break;  // peak+flow
      default: return; break;
    }
  for (int iPar = 0; iPar < nPar; iPar++)
  {
  if (fitConfig.fixedParameters[first+iPar])
    {
    f->addParameter(iPar,fitConfig.fixedValues[first+iPar]);
    f->FixParameter(iPar,fitConfig.fixedValues[first+iPar]);
    }
  }
}

// ==============================================================================================
// Use this function to set and fix the peak fit parameters when using the full fit function.
// ==============================================================================================
void BidimGaussFitter::fixPeakParameters(TF2* f, double a, double sigmaEta, double sigmaPhi, double gammaEta, double gammaPhi)
{
  int iPar = 0;
  f->addParameter(iPar,a);        f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,sigmaEta); f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,sigmaPhi); f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,gammaEta); f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,gammaPhi); f->FixParameter(iPar, f->GetParameter(iPar));
}

// ==============================================================================================
// Use this function to set and fix the flow fit parameters when using the full fit function.
// ==============================================================================================
void BidimGaussFitter::fixFlowParameters(TF2* f, double a0, double a1, double a2, double a3, double a4, double a5, double a6,
                                         double a2eta, double a3eta, double a2eta2, double a3eta2)
{
  int iPar = 5;
  f->addParameter(iPar,a0);     f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a1);     f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a2);     f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a3);     f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a4);     f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a5);     f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a6);     f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a2eta);  f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a3eta);  f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a2eta2); f->FixParameter(iPar, f->GetParameter(iPar)); iPar++;
  f->addParameter(iPar,a3eta2); f->FixParameter(iPar, f->GetParameter(iPar));
}

void BidimGaussFitter::initializeFlowFitParameters(TF2 * f, const BidimGaussFitconst Configuration & fitConfig, double scale)
{
  int nPar  = f->GetNpar();
  int first = 0;
  switch (nPar)
    {
      case  6: first = 0; return; break; // this function does not have flow features
      case 11: first = 0; break;         // flow parameters span 0 to 9 in this this function
      case 16: first = 5; break;         // flow parameters span 5 to 14 in this this function
      default: return; break;
    }
  for (int iPar=0; iPar<11; iPar++)
  {
  if (fitConfig.fixedParameters[5+iPar])
    {
    f->addParameter(first+iPar, fitConfig.fixedValues[5+iPar]);
    f->FixParameter(first+iPar, fitConfig.fixedValues[5+iPar]);
    }
  else
    {
    f->addParameter(first+iPar, fitConfig.initialValues[5+iPar]);
    double min = fitConfig.lowerLimit[5+iPar]; // if min >= max, do not set limites
    double max = fitConfig.upperLimit[5+iPar];
    if (min < max ) // limits are to be set
      f->SetParLimits(first+iPar, scale*min, scale*max);
    else
      f->ReleaseParameter(iPar); // no limits
    }
  }
}

void BidimGaussFitter::initializePeakFitParameters(TF2 * f, const BidimGaussFitconst Configuration & fitConfig, double scale)
{
  int nPar  = f->GetNpar();
  if (nPar==11)  return; // Do NOT want to initialize flow parameters here..
  int first = 0;
  int last  = 5;
  for (int iPar=first; iPar<=last; iPar++)
  {
  if (fitConfig.fixedParameters[iPar])
    {
    f->addParameter(iPar, fitConfig.fixedValues[iPar]);
    f->FixParameter(iPar, fitConfig.fixedParameters[iPar]);
    }
  else
    {
    f->addParameter(iPar, fitConfig.initialValues[iPar]);
    double min = fitConfig.lowerLimit[iPar]; // if min >= max, do not set limites
    double max = fitConfig.upperLimit[iPar];
    if (min < max ) // limits are to be set
      f->SetParLimits(iPar, scale*min, scale*max);
    else
      f->ReleaseParameter(iPar); // no limits

    }
  }
}



void BidimGaussFitter::initializeAllFitParameters(TF2 * f, const BidimGaussFitconst Configuration & fitConfig, double scale)
{
  int nPar  = f->GetNpar();
  int first = 0;
  switch (nPar)
    {
      case  6: first = 0; break; // this function does not have flow features
      case 11: first = 5; break; // flow parameters span 0 to 9 in this this function
      case 16: first = 5; break; // flow parameters span 5 to 14 in this this function
                                 // and 5 to 14 in config arrays
      default: return; break;
    }
  for (int iPar=0; iPar<nPar; iPar++)
  {
  if (fitConfig.fixedParameters[first+iPar])
    {
    f->addParameter(iPar, fitConfig.fixedValues[first+iPar]);
    f->FixParameter(iPar, fitConfig.fixedParameters[first+iPar]);
    }
  else
    {
    f->addParameter(iPar, fitConfig.initialValues[first+iPar]);
    double min = fitConfig.lowerLimit[first+iPar]; // if min >= max, do not set limites
    double max = fitConfig.upperLimit[first+iPar];
    if (min < max ) // limits are to be set
      f->SetParLimits(iPar, scale*min, scale*max);
    else
      f->ReleaseParameter(iPar); // no limits
    }
  }
}

void BidimGaussFitter::addParameters(TF2* f, int first, int last, double value)
{
  for (int iPar=first; iPar<=last; iPar++)
  {
  f->addParameter(iPar,value);
  }
}

void BidimGaussFitter::addParameterLimits(TF2* f, const BidimGaussFitconst Configuration & fitConfig, int first, int last, double scale)
{
  double min, max;
  for (int iPar=first; iPar<=last; iPar++)
  {
   min = fitConfig.lowerLimit[first+iPar]; // if min >= max, do not set limites
   max = fitConfig.upperLimit[first+iPar];
  if (min < max ) // limits are to be set
    f->SetParLimits(iPar, scale*min, scale*max);
  }
}

// ==============================================================================================
// Use this function to set and fix all flow fit parameters to zero when using the
// full fit function.
// ==============================================================================================
void BidimGaussFitter::fixFlowParametersToZero(TF2* f)
{
  for (int iPar=5; iPar<=15; iPar++)
  {
  f->addParameter(iPar, 0.0);
  f->FixParameter(iPar, 0.0);
  }
}

