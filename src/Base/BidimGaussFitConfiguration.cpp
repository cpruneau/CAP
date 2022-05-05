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
#include "BidimGaussFitConfiguration.hpp"
ClassImp(BidimGaussFitConfiguration);


BidimGaussFitConfiguration::BidimGaussFitConfiguration()
:
maxIterations(15000),
defaultFitter("Minuit2"),
useFlowInFit(false),
useEtaDependentFlowInFit(false),
useFullAzimuthalFlow(false),
useCentralGaussians(false),
useExcludeCentralRegion(true),
useEllipticalPatch(false),
useScaling(false),
useSingleCanvas(false),
drawEtaGapDist(false),

//sigmaLowerLimit(0.05),
//sigmaUpperLimit(2.0),
//betaLowerLimit(0.1),
//betaUpperLimit(14.0),
//a0LowerLimit(1.0E-10),
//a0UpperLimit(1.0E2),
//anLowerLimit(-1.0),
//anUpperLimit(1.0),


fitOption("S0QME"),
nFitLoops(5),
nFitParameters(16),
parameterNames(nullptr),
fixedParameters(nullptr),
initialValues(nullptr),
fixedValues(nullptr),
lowerLimit(nullptr),
upperLimit(nullptr),

nBinsExcludedEta(-1),
nBinsExcludedPhi(-1),

deltaEtaExclusionRegion(0.13),
deltaEtaLimit(0.95),
deltaEtaOuterLimit(1.55),
deltaPhiLowLimit(- TMath::Pi()*0.5),
deltaPhiHighLimit(TMath::Pi()*0.5),


valueInPatchBorder(-1.0E6),
lowBinEtaInPatch(1),
highBinEtaInPatch(1),
lowBinPhiInPatch(1),
highBinPhiInPatch(1),

xMinEtaFullFitLegend(0.2),
yMinEtaFullFitLegend(0.2),
xMaxEtaFullFitLegend(0.4),
yMaxEtaFullFitLegend(0.4),
etaFullFitLegendSize(0.05),

xMinPhiFullFitLegend(0.2),
yMinPhiFullFitLegend(0.2),
xMaxPhiFullFitLegend(0.4),
yMaxPhiFullFitLegend(0.4),
phiFullFitLegendSize(0.05),

xMinEtaPeakFitLegend(0.2),
yMinEtaPeakFitLegend(0.2),
xMaxEtaPeakFitLegend(0.4),
yMaxEtaPeakFitLegend(0.4),
etaPeakFitLegendSize(0.05),

xMinPhiPeakFitLegend(0.2),
yMinPhiPeakFitLegend(0.2),
xMaxPhiPeakFitLegend(0.2),
yMaxPhiPeakFitLegend(0.2),
phiPeakFitLegendSize(0.05),

xTitle ("#Delta#eta"),
yTitle ("#Delta#varphi"),
zTitle ("G_{2}"),
outputPath("./")
{
  parameterNames  = new TString*[nFitParameters];
  fixedParameters = new bool[nFitParameters];
  fixedValues     = new double[nFitParameters];
  initialValues   = new double[nFitParameters];
  lowerLimit      = new double[nFitParameters];
  upperLimit      = new double[nFitParameters];
  for (int iPar = 0; iPar < nFitParameters; iPar++)
  {
  fixedParameters[iPar] = false;
  fixedValues[iPar]     = 0.0;
  initialValues[iPar]   = 1.0E-2;
  lowerLimit[iPar]      = -1.0E5;
  upperLimit[iPar]      = 1.0E5;
  }
  parameterNames[0]  = new TString("A");
  parameterNames[1]  = new TString("sigma_eta");
  parameterNames[2]  = new TString("sigma_phi");
  parameterNames[3]  = new TString("gamma_eta");
  parameterNames[4]  = new TString("gamma_phi");
  parameterNames[5]  = new TString("a0");
  parameterNames[6]  = new TString("a1");
  parameterNames[7]  = new TString("a2");
  parameterNames[8]  = new TString("a3");
  parameterNames[9]  = new TString("a4");
  parameterNames[10] = new TString("a5");
  parameterNames[11] = new TString("a6");
  parameterNames[12] = new TString("etalina2");
  parameterNames[13] = new TString("etalina3");
  parameterNames[14] = new TString("etasqa2");
  parameterNames[15] = new TString("etasqa3");
}

BidimGaussFitConfiguration::BidimGaussFitConfiguration(const BidimGaussFitConfiguration& source)
:
maxIterations(source.maxIterations),
defaultFitter(source.defaultFitter),
useFlowInFit(source.useFlowInFit),
useEtaDependentFlowInFit(source.useEtaDependentFlowInFit),
useFullAzimuthalFlow(source.useFullAzimuthalFlow),
useCentralGaussians(source.useCentralGaussians),
useExcludeCentralRegion(source.useExcludeCentralRegion),
useEllipticalPatch(source.useEllipticalPatch),
useScaling(source.useScaling),
useSingleCanvas(source.useSingleCanvas),
drawEtaGapDist(source.drawEtaGapDist),

nBinsExcludedEta(source.nBinsExcludedEta),
nBinsExcludedPhi(source.nBinsExcludedPhi),
deltaEtaExclusionRegion(source.deltaEtaExclusionRegion),
deltaEtaLimit(source.deltaEtaLimit),
deltaEtaOuterLimit(source.deltaEtaOuterLimit),
deltaPhiLowLimit(source.deltaPhiLowLimit),
deltaPhiHighLimit(source.deltaPhiHighLimit),

//sigmaLowerLimit(source.sigmaLowerLimit),
//sigmaUpperLimit(source.sigmaUpperLimit),
//betaLowerLimit(source.betaLowerLimit),
//betaUpperLimit(source.betaUpperLimit),
//a0LowerLimit(source.a0LowerLimit),
//a0UpperLimit(source.a0UpperLimit),
//anLowerLimit(source.anLowerLimit),
//anUpperLimit(source.anUpperLimit),

fitOption(source.fitOption),
nFitLoops(source.nFitLoops),
nFitParameters(source.nFitParameters),
parameterNames(nullptr),
fixedParameters(nullptr),
initialValues(nullptr),
fixedValues(nullptr),
lowerLimit(nullptr),
upperLimit(nullptr),


valueInPatchBorder(source.valueInPatchBorder),
lowBinEtaInPatch(source.lowBinEtaInPatch),
highBinEtaInPatch(source.highBinEtaInPatch),
lowBinPhiInPatch(source.lowBinPhiInPatch),
highBinPhiInPatch(source.highBinPhiInPatch),

xMinEtaFullFitLegend(source.xMinEtaFullFitLegend),
yMinEtaFullFitLegend(source.yMinEtaFullFitLegend),
xMaxEtaFullFitLegend(source.xMaxEtaFullFitLegend),
yMaxEtaFullFitLegend(source.yMaxEtaFullFitLegend),
etaFullFitLegendSize(source.etaFullFitLegendSize),

xMinPhiFullFitLegend(source.xMinPhiFullFitLegend),
yMinPhiFullFitLegend(source.yMinPhiFullFitLegend),
xMaxPhiFullFitLegend(source.xMaxPhiFullFitLegend),
yMaxPhiFullFitLegend(source.yMaxPhiFullFitLegend),
phiFullFitLegendSize(source.phiFullFitLegendSize),

xMinEtaPeakFitLegend(source.xMinEtaPeakFitLegend),
yMinEtaPeakFitLegend(source.yMinEtaPeakFitLegend),
xMaxEtaPeakFitLegend(source.xMaxEtaPeakFitLegend),
yMaxEtaPeakFitLegend(source.yMaxEtaPeakFitLegend),
etaPeakFitLegendSize(source.etaPeakFitLegendSize),

xMinPhiPeakFitLegend(source.xMinPhiPeakFitLegend),
yMinPhiPeakFitLegend(source.yMinPhiPeakFitLegend),
xMaxPhiPeakFitLegend(source.xMaxPhiPeakFitLegend),
yMaxPhiPeakFitLegend(source.yMaxPhiPeakFitLegend),
phiPeakFitLegendSize(source.phiPeakFitLegendSize),

xTitle (source.xTitle),
yTitle (source.yTitle),
zTitle (source.zTitle),
outputPath(source.outputPath)
{
  // deep copy and ownership
  parameterNames  = new TString*[nFitParameters];
  fixedParameters = new bool  [nFitParameters];
  initialValues   = new double[nFitParameters];
  fixedValues     = new double[nFitParameters];
  lowerLimit      = new double[nFitParameters];
  upperLimit      = new double[nFitParameters];
  for (int iPar = 0; iPar < nFitParameters; iPar++)
  {
  parameterNames[iPar]  = new TString(*source.parameterNames[iPar]);
  fixedParameters[iPar] = source.fixedParameters[iPar];
  initialValues[iPar]   = source.initialValues[iPar];
  fixedValues[iPar]     = source.fixedValues[iPar];
  lowerLimit[iPar]      = source.lowerLimit[iPar];
  upperLimit[iPar]      = source.upperLimit[iPar];
  }
}

BidimGaussFitConfiguration::~BidimGaussFitConfiguration()
{
  delete[] parameterNames;
  delete[] fixedParameters;
  delete[] initialValues;
  delete[] fixedValues;
  delete[] lowerLimit;
  delete[] upperLimit;
}

const BidimGaussFitConfiguration & BidimGaussFitConfiguration::operator=(const BidimGaussFitConfiguration & source)
{
  if (this!=&source)
    {
    maxIterations            = source.maxIterations;
    defaultFitter            = source.defaultFitter;
    useFlowInFit             = source.useFlowInFit;
    useEtaDependentFlowInFit = source.useEtaDependentFlowInFit;
    useFullAzimuthalFlow     = source.useFullAzimuthalFlow;
    useCentralGaussians      = source.useCentralGaussians;
    useExcludeCentralRegion  = source.useExcludeCentralRegion;
    useEllipticalPatch       = source.useEllipticalPatch;
    useScaling               = source.useScaling;
    useSingleCanvas          = source.useSingleCanvas;
    useExcludeCentralRegion  = source.useExcludeCentralRegion;
    drawEtaGapDist           = source.drawEtaGapDist;

    nBinsExcludedEta         = source.nBinsExcludedEta;
    nBinsExcludedPhi         = source.nBinsExcludedPhi;
    deltaEtaExclusionRegion  = source.deltaEtaExclusionRegion;
    deltaEtaLimit            = source.deltaEtaLimit;
    deltaEtaOuterLimit       = source.deltaEtaOuterLimit;
    deltaPhiLowLimit     = source.deltaPhiLowLimit;
    deltaPhiHighLimit    = source.deltaPhiHighLimit;

//    sigmaLowerLimit      = source.sigmaLowerLimit;
//    sigmaUpperLimit      = source.sigmaUpperLimit;
//    betaLowerLimit       = source.betaLowerLimit;
//    betaUpperLimit       = source.betaUpperLimit;
//    a0LowerLimit         = source.a0LowerLimit;
//    a0UpperLimit         = source.a0UpperLimit;
//    anLowerLimit         = source.anLowerLimit;
//    anUpperLimit         = source.anUpperLimit;

    fitOption            = source.fitOption;
    nFitLoops            = source.nFitLoops;
    nFitParameters       = source.nFitParameters;
    parameterNames       = new TString*[nFitParameters];
    fixedParameters      = new bool[nFitParameters];
    initialValues        = new double[nFitParameters];
    fixedValues          = new double[nFitParameters];
    lowerLimit           = new double[nFitParameters];
    upperLimit           = new double[nFitParameters];
    for (int iPar = 0; iPar < nFitParameters; iPar++)
      {
      parameterNames[iPar]  = new TString(*source.parameterNames[iPar]);
      fixedParameters[iPar] = source.fixedParameters[iPar];
      initialValues[iPar]   = source.initialValues[iPar];
      fixedValues[iPar]     = source.fixedValues[iPar];
      lowerLimit[iPar]      = source.lowerLimit[iPar];
      upperLimit[iPar]      = source.upperLimit[iPar];
      }

    valueInPatchBorder    = source.valueInPatchBorder;
    lowBinEtaInPatch      = source.lowBinEtaInPatch;
    highBinEtaInPatch     = source.highBinEtaInPatch;
    lowBinPhiInPatch      = source.lowBinPhiInPatch;
    highBinPhiInPatch     = source.highBinPhiInPatch;

    xMinEtaFullFitLegend  = source.xMinEtaFullFitLegend;
    yMinEtaFullFitLegend  = source.yMinEtaFullFitLegend;
    xMaxEtaFullFitLegend  = source.xMaxEtaFullFitLegend;
    yMaxEtaFullFitLegend  = source.yMaxEtaFullFitLegend;
    etaFullFitLegendSize  = source.etaFullFitLegendSize;

    xMinPhiFullFitLegend  = source.xMinPhiFullFitLegend;
    yMinPhiFullFitLegend  = source.yMinPhiFullFitLegend;
    xMaxPhiFullFitLegend  = source.xMaxPhiFullFitLegend;
    yMaxPhiFullFitLegend  = source.yMaxPhiFullFitLegend;
    phiFullFitLegendSize  = source.phiFullFitLegendSize;

    xMinEtaPeakFitLegend  = source.xMinEtaPeakFitLegend;
    yMinEtaPeakFitLegend  = source.yMinEtaPeakFitLegend;
    xMaxEtaPeakFitLegend  = source.xMaxEtaPeakFitLegend;
    yMaxEtaPeakFitLegend  = source.yMaxEtaPeakFitLegend;
    etaPeakFitLegendSize  = source.etaPeakFitLegendSize;

    xMinPhiPeakFitLegend  = source.xMinPhiPeakFitLegend;
    yMinPhiPeakFitLegend  = source.yMinPhiPeakFitLegend;
    xMaxPhiPeakFitLegend  = source.xMaxPhiPeakFitLegend;
    yMaxPhiPeakFitLegend  = source.yMaxPhiPeakFitLegend;
    phiPeakFitLegendSize  = source.phiPeakFitLegendSize;

    xTitle      = source.xTitle;
    yTitle      = source.yTitle;
    zTitle      = source.zTitle;
    outputPath  = source.outputPath;
    }
  return *this;
}

void BidimGaussFitConfiguration::printConfiguration(ostream & out)
{
  out << "           maxIterations: " << maxIterations << endl;
  out << "           defaultFitter: " << defaultFitter << endl;
  out << "            useFlowInFit: " << (useFlowInFit ? "true" : "false") << endl;
  out << "useEtaDependentFlowInFit: " << (useEtaDependentFlowInFit ? "true" : "false")  << endl;
  out << "    useFullAzimuthalFlow: " << (useFullAzimuthalFlow ? "true" : "false") << endl;
  out << "     useCentralGaussians: " << (useCentralGaussians  ? "true" : "false") << endl;
  out << " useExcludeCentralRegion: " << (useExcludeCentralRegion ? "true" : "false") << endl;
//  if (useExcludeCentralRegion)
//    {
//    if (nBinsExcludedEta < 0)
//      out << "\tExcluded delta eta region: " << deltaEtaExclusionRegion << endl;
//    else
//      out << "\tExcluded delta eta bins on each delta eta = 0 side: " << nBinsExcludedEta << endl;
//    if (nBinsExcludedPhi < 0)
//      out << "\tExcluded delta phi region: " << deltaEtaExclusionRegion << endl;
//    else
//      out << "\tExcluded delta phi bins on each delta phi = 0 side: " << nBinsExcludedPhi << endl;
//  }

  out << "        nBinsExcludedEta: " << nBinsExcludedEta << endl;         ;
  out << "        nBinsExcludedPhi: " << nBinsExcludedPhi << endl;
  out << " deltaEtaExclusionRegion: " << deltaEtaExclusionRegion << endl;
  out << "           deltaEtaLimit: " << deltaEtaLimit << endl;
  out << "      deltaEtaOuterLimit: " << deltaEtaOuterLimit << endl;
  out << "        deltaPhiLowLimit: " << deltaPhiLowLimit << endl;
  out << "       deltaPhiHighLimit: " << deltaPhiHighLimit << endl;

  out << "      valueInPatchBorder: " << valueInPatchBorder << endl;
  out << "        lowBinEtaInPatch: " << lowBinEtaInPatch << endl;
  out << "       highBinEtaInPatch: " << highBinEtaInPatch << endl;
  out << "        lowBinPhiInPatch: " << lowBinPhiInPatch << endl;
  out << "       highBinPhiInPatch: " << highBinPhiInPatch << endl;

  out << "                  xTitle: " << xTitle << endl;
  out << "                  yTitle: " << yTitle << endl;
  out << "                  zTitle: " << zTitle << endl;
  out << "              outputPath: " << outputPath << endl;

  for (int  iPar = 0; iPar < nFitParameters; iPar++)
  {
  out << "         parameterNames[" << iPar << "]: " << *parameterNames[iPar] << endl;
  out << "        fixedParameters[" << iPar << "]: " << fixedParameters[iPar] << endl;
  out << "          initialValues[" << iPar << "]: " << initialValues[iPar] << endl;
  out << "            fixedValues[" << iPar << "]: " << fixedValues[iPar] << endl;
  out << "             lowerLimit[" << iPar << "]: " << lowerLimit[iPar] << endl;
  out << "             upperLimit[" << iPar << "]: " << upperLimit[iPar] << endl;
  }
}

void BidimGaussFitConfiguration::setConfiguration(enum CollisionSystem collisionSystem, int  iCent)
{
  collisionSystemName = "None";
  nBinsExcludedEta = -1;
  nBinsExcludedPhi = -1;
  switch (collisionSystem)
    {
      case kPbPb276:
      collisionSystemName = "PbPb276";
      break;
      case kPbPb276MM:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaPbPb276MM[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiPbPb276MM[iCent];
      collisionSystemName = "PbPb276";
      break;
      case kPbPb276PP:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaPbPb276PP[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiPbPb276PP[iCent];
      collisionSystemName = "PbPb276";
      break;
      case kPbPb276CI:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaPbPb276CI[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiPbPb276CI[iCent];
      collisionSystemName = "PbPb276";
      break;
      case kPbPb502:
      collisionSystemName = "PbPb502";
      break;
      case kXeXe544:
      collisionSystemName = "XeXe544";
      break;
      case kXeXe544MM:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaXeXe544MM[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiXeXe544MM[iCent];
      collisionSystemName = "XeXe544";
      break;
      case kXeXe544PP:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaXeXe544PP[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiXeXe544PP[iCent];
      collisionSystemName = "XeXe544";
      break;
      case kAMPTFULL:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaAMPTFULL[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiAMPTFULL[iCent];
      collisionSystemName = "AMPTfull";
      break;
      case kAMPTHALF:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaAMPTHALF[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiAMPTHALF[iCent];
      collisionSystemName = "AMPThalf";
      break;
      case kAMPTSMALL:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaAMPTSMALL[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiAMPTSMALL[iCent];
      collisionSystemName = "AMPTsmall";
      break;
      case kAMPTNONE:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaAMPTNONE[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiAMPTNONE[iCent];
      collisionSystemName = "AMPTnone";
      break;
      case kExtMM:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaMM[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiMM[iCent];
      collisionSystemName = "External";
      break;
      case kExtPP:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaPP[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiPP[iCent];
      collisionSystemName = "External";
      break;
      case kExtPM:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaPM[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiPM[iCent];
      collisionSystemName = "External";
      break;
      case kExtCI:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaCI[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiCI[iCent];
      collisionSystemName = "External";
      break;
      case kExtCD:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaCD[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiCD[iCent];
      collisionSystemName = "External";
      break;
      case kExtUS:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaUS[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiUS[iCent];
      collisionSystemName = "External";
      break;
      case kExtLS:
      nBinsExcludedEta = (iCent < 0) ? -1 : excludeCentralBinsEtaLS[iCent];
      nBinsExcludedPhi = (iCent < 0) ? -1 : excludeCentralBinsPhiLS[iCent];
      collisionSystemName = "External";
      break;
      default:
      break;
    }
}


void BidimGaussFitConfiguration::setExtraConfigParams(TH2 * h)
{
  const TAxis * xAxisHist  = h->GetXaxis();
  bwdDeltaEtaOuterLimitBin = xAxisHist->FindBin(-deltaEtaOuterLimit);
  bwdDeltaEtaLimitBin      = xAxisHist->FindBin(-deltaEtaLimit);
  fwdDeltaEtaOuterLimitBin = xAxisHist->FindBin(deltaEtaOuterLimit);
  fwdDeltaEtaLimitBin      = xAxisHist->FindBin(deltaEtaLimit);
  deltaEtaLimit            = xAxisHist->GetBinUpEdge(fwdDeltaEtaLimitBin);
  deltaEtaOuterLimit       = xAxisHist->GetBinUpEdge(fwdDeltaEtaOuterLimitBin);
  const TAxis * yAxisHist  = h->GetYaxis();
  deltaPhiLowLimitBin      = yAxisHist->FindBin(deltaPhiLowLimit) + 1;
  deltaPhiHighLimitBin     = yAxisHist->FindBin(deltaPhiHighLimit) - 1;
  deltaPhiLowLimit         = yAxisHist->GetBinLowEdge(deltaPhiLowLimitBin);
  deltaPhiHighLimit        = yAxisHist->GetBinUpEdge(deltaPhiHighLimitBin);
  deltaPhiHighestVal       = yAxisHist->GetBinUpEdge(yAxisHist->GetNbins());
  if (useExcludeCentralRegion)
    {
    valueInPatchBorder = excludeCentralRegion(deltaEtaExclusionRegion,h);
    }
}


// ==============================================================================
// Determine the low and high bins (in eta) to be used as an exclusion region in the
// bidim gauss fit.
// If excludedBins[iCent] < 0, use the value of deltaEtaExclusionRegion to determine
// the bins to be excluded. Otherwise, use the number of bins specified in the
// array excludedBins for the given "fractionalXSection" class.
// ==============================================================================
void BidimGaussFitConfiguration::excludedEtaBins(double deltaEtaExclusionRegion,
                                          const TH2 *hist,
                                          const int   *excludedBins,
                                          int   iCent,
                                          int   &lowBin, int   &highBin)
{
  const TAxis * xAxisHist = hist->GetXaxis();
  if (excludedBins[iCent] < 0)
    {
    lowBin  = xAxisHist->FindBin(-deltaEtaExclusionRegion);
    highBin = xAxisHist->FindBin( deltaEtaExclusionRegion);
    }
  else
    {
    lowBin  = xAxisHist->FindBin(0.0) - excludedBins[iCent];
    highBin = xAxisHist->FindBin(0.0) + excludedBins[iCent];
    }
}

// ==============================================================================
// Determine the low and high bins (in phi) to be used as an exclusion region in the
// bidim gauss fit.
// If excludedBins[iCent] < 0, use the value of deltaEtaExclusionRegion to determine
// the bins to be excluded. Otherwise, use the number of bins specified in the
// array excludedBins for the given "fractionalXSection" class.
// ==============================================================================
void BidimGaussFitConfiguration::excludedPhiBins(double deltaEtaExclusionRegion,
                                          const TH2 *hist,
                                          const int   *excludedBins,
                                          int   iCent,
                                          int   &lowBin,
                                          int   &highBin)
{
  const TAxis * yAxisHist = hist->GetYaxis();
  if (excludedBins[iCent] < 0)
    {
    lowBin  = yAxisHist->FindBin(-deltaEtaExclusionRegion);
    highBin = yAxisHist->FindBin(deltaEtaExclusionRegion);
    }
  else {
    lowBin  = yAxisHist->FindBin(0.0) - excludedBins[iCent];
    highBin = yAxisHist->FindBin(0.0) + excludedBins[iCent];
  }
}

double BidimGaussFitConfiguration::excludeCentralRegion(double deltaEtaExclusionRegion,TH2 *hist)
{
  const TAxis * xAxisHist = hist->GetXaxis();
  const TAxis * yAxisHist = hist->GetYaxis();
  bool  usedRegion = kTRUE;

  // work variables - at the end, we save these into in patch variables...
  int lowBinEta;
  int highBinEta;
  int lowBinPhi;
  int highBinPhi;

  switch (collisionSystem)
    {
      case kPbPb276MM:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaPbPb276MM,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiPbPb276MM,iCent,lowBinPhi,highBinPhi);
      break;
      case kPbPb276PP:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaPbPb276PP,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiPbPb276PP,iCent,lowBinPhi,highBinPhi);
      break;
      case kPbPb276CI:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaPbPb276CI,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiPbPb276CI,iCent,lowBinPhi,highBinPhi);
      break;
      case kXeXe544MM:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaXeXe544MM,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiXeXe544MM,iCent,lowBinPhi,highBinPhi);
      break;
      case kXeXe544PP:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaXeXe544PP,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiXeXe544PP,iCent,lowBinPhi,highBinPhi);
      break;
      case kAMPTFULL:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaAMPTFULL,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiAMPTFULL,iCent,lowBinPhi,highBinPhi);
      break;
      case kAMPTHALF:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaAMPTHALF,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiAMPTHALF,iCent,lowBinPhi,highBinPhi);
      break;
      case kAMPTSMALL:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaAMPTSMALL,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiAMPTSMALL,iCent,lowBinPhi,highBinPhi);
      break;
      case kAMPTNONE:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaAMPTNONE,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiAMPTNONE,iCent,lowBinPhi,highBinPhi);
      break;
      case kExtMM:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaMM,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiMM,iCent,lowBinPhi,highBinPhi);
      break;
      case kExtPP:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaPP,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiPP,iCent,lowBinPhi,highBinPhi);
      break;
      case kExtPM:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaPM,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiPM,iCent,lowBinPhi,highBinPhi);
      break;
      case kExtCI:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaCI,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiCI,iCent,lowBinPhi,highBinPhi);
      break;
      case kExtCD:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaCD,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiCD,iCent,lowBinPhi,highBinPhi);
      break;
      case kExtUS:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaUS,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiUS,iCent,lowBinPhi,highBinPhi);
      break;
      case kExtLS:
      excludedEtaBins(deltaEtaExclusionRegion,hist,excludeCentralBinsEtaLS,iCent,lowBinEta,highBinEta);
      excludedPhiBins(deltaEtaExclusionRegion,hist,excludeCentralBinsPhiLS,iCent,lowBinPhi,highBinPhi);
      break;
      default:
      usedRegion = kFALSE;
      break;
    }

  if (!usedRegion)
    {
    lowBinEta  = xAxisHist->FindBin(-deltaEtaExclusionRegion);
    highBinEta = xAxisHist->FindBin(deltaEtaExclusionRegion);
    lowBinPhi  = yAxisHist->FindBin(-deltaEtaExclusionRegion);
    highBinPhi = yAxisHist->FindBin(deltaEtaExclusionRegion);
    }

  valueInPatchBorder = -1e6;
  if (useEllipticalPatch)
    {
    int xzero = xAxisHist->FindBin(0.0);
    int yzero = yAxisHist->FindBin(0.0);
    for (int   binx = lowBinEta; binx <= highBinEta; binx++)
      {
      for (int   biny = lowBinPhi; biny <= highBinPhi; biny++)
        {
        float x = float(abs(binx-xzero));
        float y = float(abs(biny-yzero));
        float a = float(xzero-lowBinEta);
        float b = float(yzero-lowBinPhi);
        bool bwithin = TMath::Sqrt(x*x/a/a+y*y/b/b) <= 1.0;
        if (bwithin)
          {
          hist->SetBinError(binx, biny, 1e6);
          }
        else
          {
          valueInPatchBorder = TMath::Max(valueInPatchBorder,hist->GetBinContent(binx,biny));
          }
        }
      }
    Info("fitDeltaPhi2DOneFunction","NOTE : Skipping bins at ellipse (%d, %d) at (%d) = (0) in delta eta",
         lowBinEta, highBinEta, xAxisHist->FindBin(0.0));
    Info("fitDeltaPhi2DOneFunction","NOTE : Skipping bins at ellipse (%d, %d) at (%d) = (0) in delta phi",
         lowBinPhi, highBinPhi, yAxisHist->FindBin(0.0));
    }
  else
    {
    for (int   binx = lowBinEta; binx <= highBinEta; binx++)
      {
      valueInPatchBorder = TMath::Max(valueInPatchBorder,hist->GetBinContent(lowBinEta,lowBinPhi));
      valueInPatchBorder = TMath::Max(valueInPatchBorder,hist->GetBinContent(highBinEta,lowBinPhi));
      for (int   biny = lowBinPhi; biny <= highBinPhi; biny++)
        {
        if (biny == lowBinPhi)       valueInPatchBorder = TMath::Max(valueInPatchBorder,hist->GetBinContent(binx,biny));
        else if (biny == highBinPhi) valueInPatchBorder = TMath::Max(valueInPatchBorder,hist->GetBinContent(binx,biny));
        else
          {
          if (binx == lowBinEta || binx == highBinEta)
            {
            valueInPatchBorder = TMath::Max(valueInPatchBorder,hist->GetBinContent(binx,biny));
            }
          }
        hist->SetBinError(binx, biny, 1e6);
        }
      }
    Info("fitDeltaPhi2DOneFunction","NOTE : Skipping bins at (%d, %d) at (%d) = (0) in delta eta",
         lowBinEta, highBinEta, xAxisHist->FindBin(0.0));
    Info("fitDeltaPhi2DOneFunction","NOTE : Skipping bins at (%d, %d) at (%d) = (0) in delta phi",
         lowBinPhi, highBinPhi, yAxisHist->FindBin(0.0));
    }

  lowBinEtaInPatch  = lowBinEta;
  highBinEtaInPatch = highBinEta;
  lowBinPhiInPatch  = lowBinPhi;
  highBinPhiInPatch = highBinPhi;
  return valueInPatchBorder;
}

int BidimGaussFitConfiguration::excludeCentralBinsEtaPbPb276MM[9] = {3,3,3,3,3,3,3,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsEtaPbPb276PP[9] = {3,3,3,3,3,3,3,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsEtaPbPb276CI[9] = {3,3,3,3,3,3,3,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsEtaAMPTFULL [9] = {3,3,3,3,3,2,-1,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsEtaAMPTHALF [9] = {3,3,3,2,-1,-1,-1,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsEtaAMPTSMALL[9] = {1,1,1,1,0,0,0,0,0};
int BidimGaussFitConfiguration::excludeCentralBinsEtaAMPTNONE [9] = {0,0,0,0,0,0,0,0,0};
int BidimGaussFitConfiguration::excludeCentralBinsEtaXeXe544MM[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsEtaXeXe544PP[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};

int BidimGaussFitConfiguration::excludeCentralBinsPhiPbPb276MM[9] = {3,3,3,3,3,3,3,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsPhiPbPb276PP[9] = {3,3,3,3,3,3,3,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsPhiPbPb276CI[9] = {2,3,3,3,3,3,3,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsPhiAMPTFULL [9] = {2,2,2,2,2,2,-1,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsPhiAMPTHALF [9] = {2,2,2,1,-1,-1,-1,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsPhiAMPTSMALL[9] = {1,1,1,1,0,0,0,0,0};
int BidimGaussFitConfiguration::excludeCentralBinsPhiAMPTNONE [9] = {0,0,0,0,0,0,0,0,0};
int BidimGaussFitConfiguration::excludeCentralBinsPhiXeXe544MM[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
int BidimGaussFitConfiguration::excludeCentralBinsPhiXeXe544PP[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};

int BidimGaussFitConfiguration::excludeCentralBinsEtaMM[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsEtaPP[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsEtaPM[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsEtaCI[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsEtaCD[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsEtaUS[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsEtaLS[20] = {0};

int BidimGaussFitConfiguration::excludeCentralBinsPhiMM[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsPhiPP[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsPhiPM[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsPhiCI[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsPhiCD[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsPhiUS[20] = {0};
int BidimGaussFitConfiguration::excludeCentralBinsPhiLS[20] = {0};
