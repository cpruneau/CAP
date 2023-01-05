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
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>

void loadBase(const TString & includeBasePath);


// ============================================================================================================
// 4 parameters function
// ============================================================================================================
// 0  : a        : peak amplitude
// 1  : omegaEta : longitudinal peak width (eta)
// 2  : gammaEta : generalized gaussian exponent -- eta
// 3  : a0       : constant offset
// ============================================================================================================
double GeneralizedGaussian(double *x, double *par)
{
  double eta = x[0];
  double etaSq = eta*eta;
  double a        = par[0];
  double omegaEta = par[1];
  double gammaEta = par[2];
  double a0       = par[3];

  // generalized gaussian peak
  double norm   = gammaEta/2.0/omegaEta/TMath::Gamma(1.0/gammaEta);
  double arg1   = TMath::Abs(eta/omegaEta);
  double peak   = norm * TMath::Exp(-(TMath::Power(arg1,gammaEta) ));
  double result = a*peak;
  result += a0;
  return result;
}

void calculateRmsAndError(double  omega, double  eOmega, double gamma, double eGamma, double & rms, double & erms)
{
  rms =  sqrt(omega*omega*TMath::Gamma(3.0/gamma)/TMath::Gamma(1.0/gamma));
  double term1 = 0.0;
  double term2 = 0.0;
  if (omega>0)
    {
    term1 = eOmega/omega;
    }
  if (gamma>0)
    {
    TF1* tmp = new TF1("tmp","TMath::Sqrt(TMath::Gamma(3./x)/TMath::Gamma(1./x))",1,2);
    double deriv = tmp->Derivative(gamma);
    delete tmp;
    term2 = eGamma*deriv/gamma;
    }
  erms = rms*sqrt(term1*term1 + term2*term2);
}

TFile * openRootFile(const TString & inputPaths, const TString & fileName, const TString & ioOption="OLD")
{
  TString inputFileName = inputPaths;
  // make sure that if an inputPaths is given, it ends with '/'
  int slash = inputFileName.First('/');
  int len = inputFileName.Length();
  //  cout << slash << endl;
  //  cout << len << endl;
  if (len>0 && (len-1)!=slash)
    inputFileName += "/";

  inputFileName += fileName;
  if (!inputFileName.Contains(".root")) inputFileName += ".root";

  cout << "Opening file: " << inputFileName << " with option: " << ioOption << endl;
  TFile * inputFile = new TFile(inputFileName,ioOption);
  if (!inputFile)
    {
    cout << "File named " << inputFileName << " not found." << endl;
    return nullptr;
    }
  else
    {
    if (!inputFile->IsOpen())
      {
      cout << "File named " << inputFileName << " not found." << endl;
      return nullptr;
      }
    else
      {
      cout << "File opened successfully." << endl;
      }
    }
  return inputFile;
}


//Double_t ExtractGeneralizedRMS(TF1* func, Int_t sigmaIndex, Int_t betaIndex)
//{
//  Double_t sigma = func->GetParameter(sigmaIndex);
//  Double_t beta = func->GetParameter(betaIndex);
//  return TMath::Sqrt(sigma*sigma*TMath::Gamma(3./beta)/TMath::Gamma(1./beta));
//}

//Double_t ExtractGeneralizedRMSError(TF1* func, TMatrixDSym& cov, Int_t sigmaIndex, Int_t betaIndex)
//{
//  Double_t sigma = func->GetParameter(sigmaIndex);
//  Double_t beta = func->GetParameter(betaIndex);
//  Double_t sigmaDer = TMath::Sqrt(TMath::Gamma(3./beta)/TMath::Gamma(1./beta));
//  TF1* tmp = new TF1("tmp","TMath::Sqrt(TMath::Gamma(3./x)/TMath::Gamma(1./x))",1,2);
//  Double_t betaDer = sigma*tmp->Derivative(beta);
//  Double_t rmsError =
//  TMath::Power(sigmaDer * func->GetParError(sigmaIndex), 2) +
//  TMath::Power(betaDer * func->GetParError(betaIndex), 2) +
//  2. * sigmaDer * betaDer * cov(sigmaIndex, betaIndex);

class BalFctPlotter : Plotter
{
public:


  BalFctPlotter(const TString & _name,
                Configuration & _configuration)
  :
  Plotter(_name,_configuration)
  {
  appendClassName("BalFctPlotter");
  setInstanceName(_name);
  //  setDefaultConfiguration();
  //  setConfiguration(_configuration);
  }
  virtual ~BalFctPlotter(){};

  void setDefaultConfiguration();
  void configure();
  void openInputFiles();
  void calculateRmsWidth(TH2 * h, double lowEdge, double highEdge, double & mean, double & meanError, double & rmsWidth, double & rmsWidthError);
  void calculateWidths(TH2 * h, double lowEdge, double highEdge, vector<double> & rmsWidths, vector<double> & rmsWidthErrors);

  TGraph * makeGraph(vector<double> vx, vector<double> vex, vector<double> vy,  vector<double> vey);
  TGraph * sumGraphs(TGraph * g1, TGraph * g2);
  TH1 * makeHistoWithNames(const TString & histoName, vector<TString> & entryNames, vector<double> & vy,  vector<double> & vey);
  vector<GraphConfiguration*> createGraphConfigurationPalette(int n, int dim);
  void tweak1DGraphConfiguration(GraphConfiguration* gc, double size= 0.07);
  void tweak2DGraphConfiguration(GraphConfiguration* gc, double size= 0.07);
  TString makeName(const TString & s1, const TString & s2);
  TString makeName(const TString & s1, const TString & s2, const TString & s3);
  TString makeName(const TString & s1, const TString & s2, const TString & s3, const TString & s4);
  TString makeName(const TString & s1, const TString & s2, const TString & s3, const TString & s4, const TString & s5);
  TString makeName(const TString & s1, const TString & s2, const TString & s3, const TString & s4, const TString & s5, const TString & s6);
  void createBalFctNames();
  void loadHistograms();
  void rebin2D(TH2 * h2);
  void createSumGraphs(vector<TGraph*> & source, vector<TGraph*> & target);
  void setSpeciesArrays();
  void setAcceptanceArrays();
  void setBFArrays();
  void setLegendConfigurations();

  void execute();
  TGraph * calculateIntegral(TH2* h2, double etaLow, double etaHigh, double etaStep, int size=500);
  void calculateBalFctIntegrals();
  void create1DPlots();
  void create2DPlots();
  void createIntegralPlots();
  void calculateBalFctWidths();
  void createWidthPlots();

  // ===================
  // Data members
  // ===================

  vector<TString>  balFct_InputPaths;
  vector<TString>  balFct_InputFileNames;

  vector<TFile*>   balFct_InputFiles;
  vector<TString>  BalFct_HistoTitleBases;
  vector<TString>  BalFct_HistoNameBases;
  vector<TString>  BalFct_CanvasNameBases;
  vector<int>      balFct_Types;

  vector<TString>  balFct_DeltaYDeltaPhi_Histo_InputNames;
  vector<TString>  balFct_DeltaY_Histo_InputNames;
  vector<TString>  balFct_DeltaPhi_Histo_InputNames;
  vector<TH2*>     balFct_DeltaYDeltaPhi_Histos;
  vector<TH1*>     balFct_DeltaY_Histos;
  vector<TH1*>     balFct_DeltaPhi_Histos;
  vector<TGraph*>  balFct_Integrals;
  vector<TGraph*>  balFct_IntegralSums;
  vector<TH1*>     balFct_IntegralVsPair;

  vector<TString>  balFct_DeltaYDeltaPhi_Titles;
  vector<TString>  balFct_DeltaY_Titles;
  vector<TString>  balFct_DeltaPhi_Titles;
  vector<TString>  balFct_Integral_DeltaY_Titles;
  vector<TString>  balFct_IntegralSum_DeltaY_Titles;
  vector<TString>  balFct_IntegralSumVsName_Titles;


  vector<TString> speciesNames;
  vector<TString> speciesTitles;
  vector<TString> pairNames;
  vector<TString> pairTitles;
  vector<TString> pairCumulTitles;
  vector<TString> correlatorSourceNames;

  vector<double>   balFct_Minima;
  vector<double>   balFct_Maxima;

  TString outputPathBase;
  TString speciesPath;
  TString acceptancePath;
  TString originCorrelatorPath;
  TString outputPath;

  TString DeltaY_Title;
  TString DeltaPhi_Title;
  TString DeltaY_Name;
  TString DeltaPhi_Name;
  TString balFct_DeltaYDeltaPhi_Name;
  TString balFct_DeltaY_Name;
  TString balFct_DeltaPhi_Name;
  TString balFct_Integral_DeltaY_Name;
  TString balFct_IntegralSum_DeltaY_Name;

  TString balFct_Title;
  TString balFct_Integral_Title;
  TString balFct_IntegralSum_Title;

  TString balFct_Name;
  TString balFct_Integral_Name;
  TString balFct_IntegralSum_Name;

  vector<TString>  balFct_DeltaYDeltaPhi_CanvasNames;
  vector<TString>  balFct_DeltaY_CanvasNames;
  vector<TString>  balFct_DeltaPhi_CanvasNames;
  vector<TString>  balFct_Integral_DeltaY_CanvasNames;
  vector<TString>  balFct_IntegralSum_DeltaY_CanvasNames;
  vector<TString>  balFct_IntegralVsPair_CanvasNames;
  vector<TString>  balFct_DeltaY_VsPair_CanvasNames;

//  CanvasConfiguration landscapeLinear;
//  CanvasConfiguration landscapeLogX;
//  CanvasConfiguration landscapeLogY;
  vector<GraphConfiguration*> graphConfigurations1D;
  vector<GraphConfiguration*> graphConfigurations2D;
  vector<LegendConfiguration*> balFct_LegendConfig1D_DeltaY;
  vector<LegendConfiguration*> balFct_LegendConfig1D_DeltaY_VsPair;
  vector<LegendConfiguration*> balFct_LegendConfig1D_DeltaPhi;
  vector<LegendConfiguration*> balFct_LegendConfig1D_Integral;
  vector<LegendConfiguration*> balFct_LegendConfig1D_IntegralSum;
  vector<LegendConfiguration*> balFct_LegendConfig1D_Width;
  vector<LegendConfiguration*> balFct_LegendConfig1D_IntegralVsPair;


  vector<LegendConfiguration*> balFct_LegendConfig2Ds;
};

void BalFctPlotter::setDefaultConfiguration()
{
  Plotter::setDefaultConfiguration();
  bool YES = true;
  bool NO  = false;
  TString none = "none";
  int nFileMax = 9;

  addParameter("Rebin2D",               YES);
  addParameter("DoPlot1D",              NO);
  addParameter("DoPlot2D",              NO);
  addParameter("DoPlot1DHighRes",       NO);
  addParameter("DoCalculateIntegrals",  NO);
  addParameter("DoPlotIntegrals",       NO);
  addParameter("DoCalculateWidths",     NO);
  addParameter("DoPlotWidths",          NO);
  addParameter("OutputPathBase",        TString("./"));
  addParameter("OutFileNameBase",       TString("BalFct"));
  addParameter("RapidityType",           0);
  addParameter("MinRapidity",           -20.00);
  addParameter("MaxRapidity",            20.00);
  addParameter("MinRapidityHighRes",    -4.00);
  addParameter("MaxRapidityHighRes",     4.00);
  addParameter("MinRapidityInt",         0.00);
  addParameter("MaxRapidityInt",         20.00);
  addParameter("SpeciesOption",          0);   // PM, piKp, Baryons, Strange, ...
  addParameter("AcceptanceOption",       0);   // Y10, ...
  addParameter("BalFctTypeOption",       0);   // B+-, B-+, Bs
  addParameter("BalFctSourceOption",     0);   // from B or from A type correlators...
  addParameter("nFileMax",               nFileMax);
  TString parameterName;
  for (int iFile=0; iFile<nFileMax; iFile++)
    {
    parameterName = "BalFct_InputPath_";
    parameterName += iFile;
    addParameter(parameterName,none);
    parameterName = "BalFct_InputFile_";
    parameterName += iFile;
    addParameter(parameterName,none);
    parameterName = "BalFct_HistoTitleBase_";
    parameterName += iFile;
    addParameter(parameterName,none);
    parameterName = "BalFct_HistoNameBase_";
    parameterName += iFile;
    addParameter(parameterName,none);
    parameterName = "BalFct_CanvasNameBase_";
    parameterName += iFile;
    addParameter(parameterName,none);
    parameterName = "BalFct_Type_";
    parameterName += iFile;
    addParameter(parameterName,0);

    }
}

void BalFctPlotter::configure()
{
  setDefaultConfiguration();
  setConfiguration(requestedConfiguration);
  configuration.printConfiguration(cout);

  MessageLogger::LogLevel selectedLevel = MessageLogger::Debug;
  TString reportLevel                   = getValueBool("LogLevel");
  if (reportLevel.EqualTo("Debug")) selectedLevel = MessageLogger::Debug;
  if (reportLevel.EqualTo("Info"))  selectedLevel = MessageLogger::Info;

  graphConfigurations1D = createGraphConfigurationPalette(30,1);
  graphConfigurations2D = createGraphConfigurationPalette(30,2);

}




void BalFctPlotter::execute()
{
  cout << endl;
  cout << endl;
  cout << endl;
  setDefaultOptions(1);
  openInputFiles();
  setSpeciesArrays();
  setAcceptanceArrays();
  setBFArrays();
  createBalFctNames();
  setLegendConfigurations();
  if (reportInfo(__FUNCTION__))
    {
    cout << "balFct_InputPaths.size()......................... : " << balFct_InputPaths.size() << endl;
    cout << "balFct_InputFileNames.size()..................... : " << balFct_InputFileNames.size() << endl;
    cout << "balFct_InputFiles.size()......................... : " << balFct_InputFiles.size() << endl;
    cout << "balFct_DeltaYDeltaPhi_Histo_InputNames.size().... : " << balFct_DeltaYDeltaPhi_Histo_InputNames.size() << endl;
    cout << "balFct_DeltaY_Histo_InputNames.size()............ : " << balFct_DeltaY_Histo_InputNames.size() << endl;
    cout << "balFct_DeltaPhi_Histo_InputNames.size().......... : " << balFct_DeltaPhi_Histo_InputNames.size() << endl;
    }
  if (!isTaskOk()) return;
  loadHistograms();
  if (!isTaskOk()) return;
  if (getValueBool("DoPlot2D")) create2DPlots();
  if (getValueBool("DoPlot1D")) create1DPlots();


  if (getValueBool("DoCalculateIntegrals"))
    {
    calculateBalFctIntegrals();
    if (getValueBool("DoPlotIntegrals")) createIntegralPlots();
    }

  if (getValueBool("DoCalculateWidths"))
    {
    calculateBalFctWidths();
    if (getValueBool("DoPlotWidths")) createWidthPlots();
    }
  TString outputPathBase = getValueString("OutputPathBase");
  outputPath = outputPathBase;
  outputPath += speciesPath;
  outputPath += acceptancePath;
  outputPath += originCorrelatorPath;
  bool doPrint = getValueBool("DoPrint");
  if (doPrint) printAllCanvas(outputPath);
}

void BalFctPlotter::openInputFiles()
{
   int nFileMax = getValueInt("nFileMax");
  if (reportInfo(__FUNCTION__))
    {
    cout << "nFileMax................................... : " << nFileMax << endl;
    }
  TString parName;
  TString inputPath;
  TString fileName;
  TString parStringValue;
  int     parIntValue;
  for (int iFile=0; iFile<nFileMax; iFile++)
    {
    parName = "BalFct_InputPath_";
    parName += iFile;
    inputPath = getValueString(parName);
    if (!inputPath.EqualTo("none"))
      {
      parName = "BalFct_InputFile_";
      parName += iFile;
      fileName = getValueString(parName);
      TFile * f = openRootFile(inputPath,fileName,"OLD");
      if (!f)
        {
        postTaskError();
        return;
        }
      balFct_InputPaths.push_back(inputPath);
      balFct_InputFileNames.push_back(fileName);
      balFct_InputFiles.push_back(f);

      parName = "BalFct_HistoNameBase_";
      parName += iFile;
      parStringValue = getValueString(parName);
      BalFct_HistoNameBases.push_back(parStringValue);

      parName = "BalFct_HistoTitleBase_";
      parName += iFile;
      parStringValue = getValueString(parName);
      BalFct_HistoTitleBases.push_back(parStringValue);

      parName = "BalFct_CanvasNameBase_";
      parName += iFile;
      parStringValue = getValueString(parName);
      BalFct_CanvasNameBases.push_back(parStringValue);

      parName = "BalFct_Type_";
      parName += iFile;
      parIntValue = getValueInt(parName);
      balFct_Types.push_back(parIntValue);

      if (reportInfo(__FUNCTION__))
        {
        cout << endl;
        cout << " iFile............... : " << iFile << endl;
        cout << " Input path.......... : " << balFct_InputPaths[iFile] << endl;
        cout << " File name........... : " << balFct_InputFileNames[iFile] << endl;
        cout << " Title base.......... : " << BalFct_HistoTitleBases[iFile] << endl;
        cout << " Histo Name Base..... : " << BalFct_HistoNameBases[iFile] << endl;
        cout << " Canvas Name Base.... : " << BalFct_CanvasNameBases[iFile] << endl;
        cout << " Type................ : " << balFct_Types[iFile] << endl;
        }
      }
    }
}

void BalFctPlotter::calculateRmsWidth(TH2 * h, double lowEdge, double highEdge, double & mean, double & meanError, double & rmsWidth, double & rmsWidthError)
{
  int nBins   = h->GetXaxis()->GetNbins();
  int lowBin  = h->GetXaxis()->FindBin(lowEdge);
  int highBin = h->GetXaxis()->FindBin(highEdge);
  if (lowBin<1) lowBin = 1;
  if (highBin>nBins)  highBin = nBins;
  double sum    = 0.0;
  double xSum   = 0.0;
  double x2Sum  = 0.0;
  double eXSum  = 0.0;
  double eX2Sum = 0.0;
  double m1  = 0.0;
  double m2  = 0.0;
  mean       = 0.0;
  meanError  = 0.0;
  rmsWidth   = 0.0;
  rmsWidthError  = 0.0;
  double x, v, ev, arg;

  int ny = h->GetNbinsY();

  // two passes: pass 1 to get the mean, pass 2 to get the rms error,
  for (int j=1; j<=ny; j++)
    {
    for (int k=lowBin; k<highBin; k++ )
      {
      x  = h->GetXaxis()->GetBinCenter(k);
      v  = h->GetBinContent(k,j);
      sum   += v;
      xSum  += x*v;
      x2Sum += x*x*v;
      }
    }

  if (sum>0)
    {
    m1 = xSum/sum;
    m2 = x2Sum/sum - m1*m1;
    if (m2>0.0) rmsWidth = sqrt(m2);

    for (int j=1; j<=ny; j++)
      {
      for (int k=lowBin; k<highBin; k++ )
        {
        x  = h->GetXaxis()->GetBinCenter(k);
        v  = h->GetBinContent(k);
        ev = h->GetBinError(k,j);
        xSum  += x*x*ev*ev;
        arg = x*x - 2*m1*x;
        x2Sum += arg*arg * ev*ev;
        }
      }
    meanError = sqrt(xSum)/sum;
    rmsWidthError = sqrt(x2Sum)/sum;
    }
}

void BalFctPlotter::calculateWidths(TH2 * h, double lowEdge, double highEdge, vector<double> & rmsWidths, vector<double> & rmsWidthErrors)
{
  double mean          = 0;
  double meanError     = 0;
  double rmsWidth      = 0;
  double rmsWidthError = 0;
  calculateRmsWidth(h,lowEdge,highEdge,mean,meanError,rmsWidth,rmsWidthError);
  rmsWidths.push_back(rmsWidth);
  rmsWidthErrors.push_back(rmsWidthError);
}

TGraph * BalFctPlotter::calculateIntegral(TH2* h2,
                                          double xMin,
                                          double xMax,
                                          double xStep,
                                          int size)
{
  if (reportStart(__FUNCTION__))
    ;
  vector<double> vx;
  vector<double> vex;
  vector<double> vy;
  vector<double> vey;
  const TAxis * xAxis  = h2->GetXaxis();
  //const TAxis * yAxis  = h2->GetYaxis();
  int xLow;
  int xHigh;
  int yLow  = 1;
  int yHigh = h2->GetNbinsY();
  double sum  = 0;
  double esum = 0;
  for (double x=xMin; x<xMax; x+=xStep)
    {
    xLow  = xAxis->FindBin(-x);
    xHigh = xAxis->FindBin( x);
    //double factor = 1-eta/xHigh;
    sum = h2->IntegralAndError(xLow,xHigh,yLow,yHigh,esum,"WIDTH");
    vx. push_back(x);
    vex.push_back(0.001);
    vy. push_back(sum);
    vey.push_back(esum);
    }
  //TGraph * g = new TGraphErrors(nPoints, x,y,ex,ey);
  TGraph * g = makeGraph(vx,vex,vy,vey);
  if (reportEnd(__FUNCTION__))
    ;
  return g;
}

TGraph *  BalFctPlotter::sumGraphs(TGraph * g1, TGraph * g2)
{
  if (reportStart(__FUNCTION__))
    ;
  int  n1 = g1->GetN();
  int  n2 = g2->GetN();
  if (n1 != n2)
    {
    if (reportError(__FUNCTION__)) cout << "Args do not have same number of points" << endl;
    return nullptr;
    }
  double * x  = new double[n1];
  double * ex = new double[n1];
  double * y  = new double[n1];
  double * ey = new double[n1];
  for (int index=0; index<n1; index++)
    {
    double x1  = g1->GetPointX(index);
    double ex1 = g1->GetErrorX(index);
    double y1  = g1->GetPointY(index);
    double ey1 = g1->GetErrorY(index);
    double x2  = g2->GetPointX(index);
    double ex2 = g2->GetErrorX(index);
    double y2  = g2->GetPointY(index);
    double ey2 = g2->GetErrorY(index);
    if (x1!=x2)
      {
      if (reportError(__FUNCTION__)) cout << "x1 ne x2" << endl;
      delete[] x;
      delete[] ex;
      delete[] y;
      delete[] ey;
      return nullptr;
      }
    x[index]  = x1;
    ex[index] = ex1;
    y[index]  = y1 + y2;
    ey[index] = sqrt(ey1*ey1 + ey2*ey2);
    }
  TGraph * g = new TGraphErrors(n1, x,y,ex,ey);
  if (reportEnd(__FUNCTION__))
    ;
  return g;
}

TGraph * BalFctPlotter::makeGraph(vector<double> vx, vector<double> vex, vector<double> vy,  vector<double> vey)
{
  int n = vx.size();
  if ( vex.size()!=n || vy.size()!=n || vey.size()!=n )
    {
    if (reportError(__FUNCTION__)) cout  << "Arguments provided have incompatible sizes)" << endl;
    exit(1);
    }
  double * x  = new double[n];
  double * ex = new double[n];
  double * y  = new double[n];
  double * ey = new double[n];
  for (int k=0;k<n;k++)
    {
    x[k]  = vx[k];
    ex[k] = vex[k];
    y[k]  = vy[k];
    ey[k] = vey[k];
    }
  return new TGraphErrors(n,x,y,ex,ey);
}

TH1 * BalFctPlotter::makeHistoWithNames(const TString & histoName, vector<TString> & entryNames, vector<double> & vy,  vector<double> & vey)
{
  int n  = vy.size();
  int n1 = vey.size();
  int nn = entryNames.size();
  if (reportDebug(__FUNCTION__))
    {
    cout << "Creating histo named " << histoName << " with number of bins (n): " << n << endl;
    }
  if ( n<1 || n!=n1 || n!=nn )
    {
    if (reportError(__FUNCTION__))
      {
      cout << "Arguments provided have incompatible sizes: vy.size()=" << n << ";   vey.size()=" << n1 << ";  entryNames.size()=" << nn << endl;
      }
    exit(1);
    }
  TString hn = histoName;
  hn += "_Test";
  TH1 * h = new TH1D(hn,hn,n,0.0, double(n));
  if (!h)
    {
    if (reportError(__FUNCTION__))
      {
      cout << "Histogram not created. Abort." << n1 << endl;
      }
    exit(1);
    }

  for (int k=0;k<n;k++)
    {
    h->SetBinContent(k+1,vy[k]);
    h->SetBinError(k+1,vey[k]);
    h->GetXaxis()->SetBinLabel(k+1,entryNames[k]);
    }
  return h;
}

vector<GraphConfiguration*> BalFctPlotter::createGraphConfigurationPalette(int n, int dim)
{
  vector<GraphConfiguration*>  gc = GraphConfiguration::createConfigurationPalette(n,dim);
  gStyle->SetPalette(kBlueRedYellow);
  float size = 0.07;

  for (int k=0;k<n;k++)
    {
    gc[k]->setParameter("xTitleSize",   size);
    gc[k]->setParameter("xTitleOffset", 0.8);
    gc[k]->setParameter("xLabelSize",   size);
    gc[k]->setParameter("yTitleSize",   size);
    gc[k]->setParameter("yTitleOffset", 1.1);
    gc[k]->setParameter("yLabelSize",   size);
    if (dim==2)
      {
      gc[k]->setParameter("zTitleSize",   size);
      gc[k]->setParameter("zTitleOffset", 0.8);
      gc[k]->setParameter("zLabelSize",   size);
      }
    gc[k]->setParameter("lineColor",    1+k); //*int(60.0/float(n)));
    gc[k]->setParameter("lineStyle",    1);
    gc[k]->setParameter("markerColor",  1+k); //*int(60.0/float(n)));
    gc[k]->setParameter("markerStyle",  kFullSquare);
    gc[k]->setParameter("markerSize",   0.9);
    }
  return gc;
}

void BalFctPlotter::tweak1DGraphConfiguration(GraphConfiguration* gc, double size)
{
  gc->setParameter("markerColor",  kBlack);
  gc->setParameter("markerStyle",  kFullSquare);
  gc->setParameter("markerSize",   1.2);
  gc->setParameter("xLabelSize",   size);
  gc->setParameter("xLabelSize",   0.06);
  gc->setParameter("yTitleSize",   size);
  gc->setParameter("yTitleOffset", 0.7);
  gc->setParameter("yLabelSize",   0.07);
}


void  BalFctPlotter::tweak2DGraphConfiguration(GraphConfiguration* gc, double size= 0.07)
{
  gc->setParameter("xTitleOffset",  1.2);
  gc->setParameter("xTitleSize",   size);
  gc->setParameter("xLabelOffset", 0.01);
  gc->setParameter("xLabelSize",   size);

  gc->setParameter("yTitleOffset", 1.2);
  gc->setParameter("yTitleSize",   size);
  gc->setParameter("yLabelOffset", 0.01);
  gc->setParameter("yLabelSize",   size);

  gc->setParameter("zTitleOffset", 1.2);
  gc->setParameter("zTitleSize",   size);
  gc->setParameter("zLabelOffset", 0.01);
  gc->setParameter("zLabelSize",   size);
}


TString BalFctPlotter::makeName(const TString & s1, const TString & s2)
{
  TString s = s1;
  s += "_";
  s += s2;
  return s;
}

TString BalFctPlotter::makeName(const TString & s1, const TString & s2, const TString & s3)
{
  TString s = s1;
  s += "_";
  s += s2;
  s += "_";
  s += s3;
  return s;
}

TString BalFctPlotter::makeName(const TString & s1, const TString & s2, const TString & s3, const TString & s4)
{
  TString s = s1;
  s += "_";
  s += s2;
  s += "_";
  s += s3;
  s += "_";
  s += s4;
  return s;
}

TString BalFctPlotter::makeName(const TString & s1, const TString & s2, const TString & s3, const TString & s4, const TString & s5)
{
  TString s = s1;
  s += "_";
  s += s2;
  s += "_";
  s += s3;
  s += "_";
  s += s4;
  s += "_";
  s += s5;
  return s;
}

TString BalFctPlotter::makeName(const TString & s1, const TString & s2, const TString & s3, const TString & s4, const TString & s5, const TString & s6)
{
  TString s = s1;
  s += "_";
  s += s2;
  s += "_";
  s += s3;
  s += "_";
  s += s4;
  s += "_";
  s += s5;
  s += "_";
  s += s6;
  return s;
}


void BalFctPlotter::createBalFctNames()
{
  int rapidityType = getValueInt("RapidityType");
  int balFctType   = getValueInt("BalFctTypeOption");
  DeltaPhi_Name  = "DeltaPhi";
  DeltaPhi_Title = "#Delta #varphi";
  switch (rapidityType)
    {
      default:
      case 0: DeltaY_Title   = "#Delta y";    DeltaY_Name    = "DeltaY";    break;
      case 1: DeltaY_Title   = "#Delta #eta"; DeltaY_Name    = "DeltaEta";  break;
    }

  switch (balFctType)
    {
      case 0:
      balFct_Title              = "B^{#bar{#alpha}#beta}";
      balFct_Integral_Title     = "I^{#bar{#alpha}#beta}";
      balFct_IntegralSum_Title  = "#sum I^{#bar{#alpha}#beta}";
      balFct_Name               = "B1Bar2";
      balFct_Integral_Name      = "I1Bar2";
      balFct_IntegralSum_Name   = "I1Bar2Sum";
      break;

      case 1:
      balFct_Title              = "B^{#alpha#bar#beta}";
      balFct_Integral_Title     = "I^{#alpha#bar#beta}";
      balFct_IntegralSum_Title  = "#sum I^{#alpha#bar#beta}";
      balFct_Name               = "B12Bar";
      balFct_Integral_Name      = "I12Bar";
      balFct_IntegralSum_Name   = "I12BarSum";
      break;

      case 2:
      balFct_Title              = "B^{s}";
      balFct_Integral_Title     = "I^{s}";
      balFct_IntegralSum_Title  = "#sum I^{s}";
      balFct_Name               = "B12s";
      balFct_Integral_Name      = "I12s";
      balFct_IntegralSum_Name   = "I12sSum";
      break;

    }

  TString vsPairName = "VsPairName";
  TString baseHistoName;
  TString baseCanvasName;
  TString baseHistoTitle;
  TString pairName;
  TString balFct_DeltaYDeltaPhi_InputName;
  TString balFct_DeltaY_InputName;
  TString balFct_DeltaPhi_InputName;
  TString title;
  TString underscore = "_";
  for (int iFile=0; iFile<balFct_InputFiles.size(); iFile++)
    {
    baseHistoName  = BalFct_HistoNameBases[iFile];
    baseHistoTitle = BalFct_HistoTitleBases[iFile];
    baseCanvasName = BalFct_CanvasNameBases[iFile];
    for (int iPair=0; iPair<pairNames.size(); iPair++)
      {
      pairName  = pairNames[iPair];

      balFct_DeltaYDeltaPhi_InputName     = baseHistoName+pairName+correlatorSourceNames[2];
      balFct_DeltaY_InputName             = baseHistoName+pairName+correlatorSourceNames[2]+"_x";
      balFct_DeltaPhi_InputName           = baseHistoName+pairName+correlatorSourceNames[2]+"_y";
      balFct_DeltaYDeltaPhi_Histo_InputNames.push_back( balFct_DeltaYDeltaPhi_InputName);
      balFct_DeltaY_Histo_InputNames.push_back(         balFct_DeltaY_InputName);
      balFct_DeltaPhi_Histo_InputNames.push_back(       balFct_DeltaPhi_InputName);

      title = baseHistoTitle;
      title += " ";
      title += pairName;
      balFct_DeltaYDeltaPhi_Titles.push_back(    title  );
      balFct_DeltaY_Titles.push_back(            title  );
      balFct_DeltaPhi_Titles.push_back(          title  );
      balFct_Integral_DeltaY_Titles.push_back(   title  );
      balFct_IntegralSum_DeltaY_Titles.push_back(title  );

      }
    for (int iTrigger=0; iTrigger<speciesNames.size(); iTrigger++)
      {
      balFct_DeltaYDeltaPhi_CanvasNames.push_back(    makeName(baseCanvasName,balFct_Name, speciesNames[iTrigger],  DeltaY_Name,DeltaPhi_Name));
      balFct_DeltaY_CanvasNames.push_back(            makeName(baseCanvasName,balFct_Name, speciesNames[iTrigger],  DeltaY_Name  ));

      balFct_DeltaPhi_CanvasNames.push_back(          makeName(baseCanvasName,balFct_Name, speciesNames[iTrigger],  DeltaPhi_Name));
      balFct_Integral_DeltaY_CanvasNames.push_back(   makeName(baseCanvasName,balFct_Integral_Name, speciesNames[iTrigger],  DeltaY_Name  ));
      balFct_IntegralSum_DeltaY_CanvasNames.push_back(makeName(baseCanvasName,balFct_IntegralSum_Name, speciesNames[iTrigger],  DeltaY_Name  ));
      balFct_IntegralVsPair_CanvasNames.push_back(    makeName(baseCanvasName,balFct_Integral_Name, speciesNames[iTrigger],  vsPairName  ));
      balFct_IntegralSumVsName_Titles.push_back(      baseHistoTitle+TString(" ")+ speciesTitles[iTrigger] );

      }
    }
  for (int iPair=0; iPair<pairNames.size(); iPair++)
    {
    balFct_DeltaY_VsPair_CanvasNames.push_back(  makeName(baseCanvasName,balFct_Name, pairNames[iPair],  DeltaY_Name  ));
    }

}


void BalFctPlotter::loadHistograms()
{
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "balFct_InputFiles.size().........................: " <<  balFct_InputFiles.size()<< endl;
    cout << "balFct_DeltaYDeltaPhi_Histo_InputNames.size()....: " <<  balFct_DeltaYDeltaPhi_Histo_InputNames.size() << endl;
    }
  bool rebin = getValueBool("Rebin");
  TString name;
  int nFiles  = balFct_InputFiles.size();
  int nPairs  = pairNames.size();
  for (int iFile=0; iFile<nFiles; iFile++)
    {
    if (reportDebug(__FUNCTION__)) cout << " iFile .............: " <<  iFile << endl;
    TFile * f = balFct_InputFiles[iFile];
    if (!f)
      {
      if (reportError(__FUNCTION__)) cout << "File ptr for iFile=" << iFile << " is null" << endl;
      postTaskError();
      return;
      }
    for (int iPair=0; iPair<nPairs; iPair++)
      {
      int iHist = iFile*nPairs + iPair;
      name = balFct_DeltaYDeltaPhi_Histo_InputNames[iHist];
      TH2 * h2  = (TH2*) f->Get(name);
      if (!h2)
        {
        if (reportError(__FUNCTION__)) cout << "Could not load 2D histogram named : " << name << endl;
        postTaskError();
        return;
        }
      if (rebin) rebin2D(h2);
      double dx = h2->GetXaxis()->GetBinWidth(1);
      double dy = h2->GetYaxis()->GetBinWidth(1);
      h2->Scale(1.0/dy);
      name = balFct_DeltaY_Histo_InputNames[iHist];
      TH1 * h1x = (TH1*) f->Get(name);
      if (!h1x)
        {
        if (reportError(__FUNCTION__)) cout << "Could not load 1D histogram named : " << name << endl;
        postTaskError();
        return;
        }
      name = balFct_DeltaPhi_Histo_InputNames[iHist];
      TH1 * h1y = (TH1*) f->Get(name);
      if (!h1y)
        {
        if (reportError(__FUNCTION__)) cout << "Could not load 1D histogram named : " << name << endl;
        postTaskError();
        return;
        }
      if (reportInfo(__FUNCTION__)) cout << "Histogram " << h2->GetName() << " loaded - add to the lists" << endl;
      balFct_DeltaYDeltaPhi_Histos.push_back(h2);
      balFct_DeltaY_Histos.push_back(h1x);
      balFct_DeltaPhi_Histos.push_back(h1y);
      }
    }
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << " balFct_DeltaYDeltaPhi_Histos.size() .............: " <<  balFct_DeltaYDeltaPhi_Histos.size() << endl;
    cout << " balFct_DeltaY_Histos.size() .....................: " <<  balFct_DeltaY_Histos.size() << endl;
    cout << " balFct_DeltaPhi_Histos.size() ...................: " <<  balFct_DeltaPhi_Histos.size() << endl;
    }
}

void BalFctPlotter::rebin2D(TH2 * h2)
{
  h2->Rebin2D(4,2);
  h2->Scale(0.25);
}

void BalFctPlotter::createSumGraphs(vector<TGraph*> & source, vector<TGraph*> & target)
{
  TGraph * g;
  TGraph * gs;
  for (int iGraph=0; iGraph<source.size(); iGraph++)
    {
    g = source[iGraph];
    if (iGraph==0)
      {
      gs = (TGraph *)g->Clone();
      }
    else
      {
      gs = sumGraphs(gs, g);
      }
    target.push_back(gs);
    }
}

void BalFctPlotter::calculateBalFctIntegrals()
{
  double etaLow  = getValueDouble("MinRapidityInt");
  double etaHigh = getValueDouble("MaxRapidityInt");
  double etaStep = 0.1;
  int    size    = 500;
  if (reportInfo(__FUNCTION__))
    {
    cout << "etaLow................................. : " << etaLow << endl;
    cout << "etaHigh................................ : " << etaHigh << endl;
    cout << "etaStep................................ : " << etaStep << endl;
    cout << "size................................... : " << size<< endl;
    cout << "balFct_DeltaYDeltaPhi_Histos.size().... : " << balFct_DeltaYDeltaPhi_Histos.size() << endl;
    }
  TGraph * g        = nullptr;
  TGraph * gs       = nullptr;
  TH1    * gVsName  = nullptr;
  TGraphErrors * ge = nullptr;
  int nFiles   = balFct_InputFiles.size();
  int nSpecies = speciesNames.size();
  int iPair;
  int iHisto;
  TString histoName;
  vector<TString> pairNamesSubset;
  vector<double>  pairIntegral;
  vector<double>  pairIntegralError;
  for (int iFile=0; iFile<nFiles; iFile++)
    {
    for (int iTrigger=0; iTrigger<nSpecies; iTrigger++)
      {
      pairNamesSubset.clear();
      pairIntegral.clear();
      pairIntegralError.clear();
      histoName = "File";
      histoName += iFile;
      histoName += "IntegralVsPairName";
      histoName += iTrigger;
      for (int iAssoc=0; iAssoc<nSpecies; iAssoc++)
        {
        iHisto = iFile*nSpecies*nSpecies + iTrigger*nSpecies + iAssoc;
        iPair  = iTrigger*nSpecies + iAssoc;
        if (reportDebug(__FUNCTION__))
          {
          cout << " iFile=" << iFile << " iTrigger=" << iTrigger << " iAssoc=" << iAssoc << " iPair=" << iPair << " iHisto=" << iHisto  << endl;
          }
        pairNamesSubset.push_back(pairTitles[iPair]);
        TH2* h2 = balFct_DeltaYDeltaPhi_Histos[iHisto];
        if (!h2)
          {
          if (reportError(__FUNCTION__)) cout << "h2 is a null pointer at iFile=" << iFile << "  iTrigger=" << iTrigger << "   iAssoc=" << iAssoc << endl;
          exit(1);
          }
        g = calculateIntegral(h2,etaLow,etaHigh,etaStep);
        if (iAssoc==0)
          {
          gs = (TGraph *)g->Clone();
          }
        else
          {
          gs = sumGraphs(gs, g);
          }
        balFct_Integrals.push_back(g);
        balFct_IntegralSums.push_back(gs);

        ge = (TGraphErrors*) g;
        int nPoints = ge->GetN();
        double y  = ge->GetPointY(nPoints-1);
        double ey = ge->GetErrorY(nPoints-1);
        pairIntegral.push_back(y);
        pairIntegralError.push_back(ey);
        }
      gVsName = makeHistoWithNames(histoName,pairNamesSubset,  pairIntegral, pairIntegralError);
      balFct_IntegralVsPair.push_back(gVsName);
      }
    }
  if (reportInfo(__FUNCTION__))
    {
    cout << "balFct_IntegralVsPair.size().... : " << balFct_IntegralVsPair.size() << endl;
    }
}

void BalFctPlotter::setSpeciesArrays()
{
  int option = getValueInt("SpeciesOption");
  if (reportInfo(__FUNCTION__)) cout << "SpeciesOption:" << option << endl;
  speciesNames.clear();
  speciesTitles.clear();
  speciesPath = "";

  switch (option)
    {
      case 0:
      speciesNames.push_back(TString("HP"));
      speciesTitles.push_back(TString("h^{+}"));
      speciesPath = "PM/";
      break;

      case 1:
      speciesNames.push_back(TString("PiP"));
      speciesNames.push_back(TString("KP"));
      speciesNames.push_back(TString("PP"));

      speciesTitles.push_back(TString("#pi"));
      speciesTitles.push_back(TString("K"));
      speciesTitles.push_back(TString("P"));
      speciesPath = "PiKP/";
      break;

      case 2:
      speciesNames.push_back(TString("P"));
      speciesNames.push_back(TString("N"));
      speciesNames.push_back(TString("Lambda0"));
      speciesNames.push_back(TString("SigmaP"));
      speciesNames.push_back(TString("Sigma0"));
      speciesNames.push_back(TString("SigmaM"));
      speciesNames.push_back(TString("Xi0"));
      speciesNames.push_back(TString("XiM"));
      speciesNames.push_back(TString("OmegaM"));

      speciesTitles.push_back(TString("p"));
      speciesTitles.push_back(TString("n"));
      speciesTitles.push_back(TString("#Lambda^{0}"));
      speciesTitles.push_back(TString("#Sigma^{+}"));
      speciesTitles.push_back(TString("#Sigma^{0}"));
      speciesTitles.push_back(TString("#Sigma^{-}"));
      speciesTitles.push_back(TString("#Xi^{0}"));
      speciesTitles.push_back(TString("#Xi^{-}"));
      speciesTitles.push_back(TString("#Omega^{-}"));
      speciesPath = "Baryons/";

      break;

      case 3:
      speciesNames.push_back(TString("KP"));
      speciesNames.push_back(TString("Lambda0"));
      speciesNames.push_back(TString("SigmaP"));
      speciesNames.push_back(TString("Sigma0"));
      speciesNames.push_back(TString("SigmaM"));
      speciesNames.push_back(TString("Xi0"));
      speciesNames.push_back(TString("XiM"));
      speciesNames.push_back(TString("OmegaM"));

      speciesTitles.push_back(TString("K^{+}"));
      speciesTitles.push_back(TString("#Lambda^{0}"));
      speciesTitles.push_back(TString("#Sigma^{+}"));
      speciesTitles.push_back(TString("#Sigma^{0}"));
      speciesTitles.push_back(TString("#Sigma^{-}"));
      speciesTitles.push_back(TString("#Xi^{0}"));
      speciesTitles.push_back(TString("#Xi^{-}"));
      speciesTitles.push_back(TString("#Omega^{-}"));
      speciesPath = "Strangeness/";

      break;

      default:
      cout << "Option not available" << endl;
      break;
    }

  int nSpecies = speciesNames.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << " nSpecies................... :" << nSpecies    << endl;
    cout << " speciesPath................ :" << speciesPath << endl;
    for (int iSpecies1=0; iSpecies1<nSpecies; iSpecies1++)
      {
      cout << " iSpecies:" << iSpecies1 << "    " << speciesNames[iSpecies1] << "   " << speciesTitles[iSpecies1] << endl;
      }
    }

  if (nSpecies<1)
    {
    postTaskError();
    return;
    }
  // setup pairs...
  TString pairName;
  TString pairTitle;
  TString pairFirstTitle;
  TString pairCumulTitle;
  TString pairTitlePlus;
  for (int iSpecies2=0; iSpecies2<nSpecies; iSpecies2++)
    {
    if (reportDebug(__FUNCTION__)) cout << " iSpecies2.................. : " << iSpecies2 << endl;
    pairTitlePlus = "";
    for (int iSpecies1=0; iSpecies1<nSpecies; iSpecies1++)
      {
      if (reportDebug(__FUNCTION__)) cout << " iSpecies1.................. : " << iSpecies1 << endl;
      pairTitle     = speciesTitles[iSpecies1] + speciesTitles[iSpecies2];
      pairName      = (speciesNames[iSpecies1]+"_")+speciesNames[iSpecies2];
      if (iSpecies1==0)
        {
        pairFirstTitle  = pairTitle;
        pairCumulTitle  = pairTitle;
        }
      else if (iSpecies1==1)
        {
        pairCumulTitle = pairFirstTitle;
        pairCumulTitle += "+";
        pairCumulTitle += pairTitle;
        }
      else  if (iSpecies1>1)
        {
        pairCumulTitle = "...";    //          pairTitlePlus += "+ ... +";
        pairCumulTitle += "+";
        pairCumulTitle += pairTitle;
        }
      pairNames.push_back(pairName);
      pairTitles.push_back(pairTitle);
      pairCumulTitles.push_back(pairCumulTitle);
      }
    }
  if (reportInfo(__FUNCTION__))
    {
    int nPairs = pairNames.size();
    cout << " nPairs................... :" << nPairs << endl;
    for (int k=0; k<nPairs; k++)
      {
      cout << " k:" << k << "    " << pairNames[k] << "   " << pairTitles[k] << "   " << pairCumulTitles[k] << endl;
      }
    }
}

void BalFctPlotter::setAcceptanceArrays()
{
  int option = getValueInt("AcceptanceOption");
  if (reportInfo(__FUNCTION__)) cout << "AcceptanceOption:" << option << endl;
  switch (option)
    {
      default:
      case 0:
      acceptancePath += "Y10/";
      break;

      case 1:
      acceptancePath += "Y6/";
      break;

      case 2:
      acceptancePath += "Y4/";
      break;

      case 3:
      acceptancePath += "Y2/";
      break;

      case 4:
      acceptancePath += "Ys/";
      break;
    }
}

// BalFctTypeOption == 0 B2 based BalcFct
// BalFctTypeOption == 1 A2 based BalcFct
//
void BalFctPlotter::setBFArrays()
{
  int option = getValueInt("BalFctSourceOption");
  if (reportInfo(__FUNCTION__)) cout << "BalFctTypeOption:" << option << endl;

  switch (option)
    {
      case 0:
      {
      correlatorSourceNames.push_back(TString("_B2_DyDphi_shft_B2_1Bar_2"));
      correlatorSourceNames.push_back(TString("_B2_DyDphi_shft_B2_1_2Bar"));
      correlatorSourceNames.push_back(TString("_B2_DyDphi_shft_B2_12Sum"));
      originCorrelatorPath = "B2Derived/";
      // B vs Y,Phi
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.4);
      // B vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.4);
      // B vs phi
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.24);
      // I vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.24);
      // Isum vs Y
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.24);
      // B width
      balFct_Minima.push_back( 0.00);
      balFct_Maxima.push_back( 2.00);
      }
      break;

      case 1:
      {
      cout << "Processing BalFctTypeOption == 1" << endl;
      correlatorSourceNames.push_back(TString("_A2_DyDphi_shft_B2_1Bar_2"));
      correlatorSourceNames.push_back(TString("_A2_DyDphi_shft_B2_1_2Bar"));
      correlatorSourceNames.push_back(TString("_A2_DyDphi_shft_B2_12Sum"));
      originCorrelatorPath = "A2Derived/";

      // B vs Y,Phi
      balFct_Minima.push_back(-0.05);
      balFct_Maxima.push_back( 0.4);
      // B vs Y
      balFct_Minima.push_back(-0.02);
      balFct_Maxima.push_back( 0.4);
      // B vs phi
      balFct_Minima.push_back(-0.02);
      balFct_Maxima.push_back( 0.20);
      // I vs Y
      balFct_Minima.push_back(-0.02);
      balFct_Maxima.push_back( 1.09);
      // Isum vs Y
      balFct_Minima.push_back(-0.02);
      balFct_Maxima.push_back( 1.09);
      // B width
      balFct_Minima.push_back( 0.00);
      balFct_Maxima.push_back( 1.19);
      break;
      }

      default:
      cout << "Option not available" << endl;
      break;
    }
  if (reportInfo(__FUNCTION__))
    {
    cout << " originCorrelatorPath............ : " << originCorrelatorPath  << endl;
    cout << " correlatorSourceNames.size().... : " << correlatorSourceNames.size() << endl;
    cout << " balFct_Minima.size()............ : " << balFct_Minima.size() << endl;
    cout << " balFct_Maxima.size()............ : " << balFct_Maxima.size() << endl;
    }
}

void BalFctPlotter::setLegendConfigurations()
{
  balFct_LegendConfig2Ds.clear();

  balFct_LegendConfig1D_DeltaY.clear();
  balFct_LegendConfig1D_DeltaY_VsPair.clear();
  balFct_LegendConfig1D_DeltaPhi.clear();
  balFct_LegendConfig1D_Integral.clear();
  balFct_LegendConfig1D_IntegralSum.clear();
  balFct_LegendConfig1D_IntegralVsPair.clear();
  balFct_LegendConfig1D_Width.clear();
  int nSpecies = speciesNames.size();
  int iPair;
  LegendConfiguration * lc;

  for (int iSpecies2=0; iSpecies2<nSpecies; iSpecies2++)
    {
    // B vs Delta y
    lc = new LegendConfiguration(0.65, 0.85, 0.5, 0.9, 0.07);
    lc->setParameter("useLegend",true);
    lc->setParameter("useLabels",true);
    lc->setParameter("useTitles",false);
    lc->setParameter("textIndex",42);
    lc->setParameter("nColumns",2);
    balFct_LegendConfig1D_DeltaY.push_back(lc);
    // B vs Delta phi
    lc = new LegendConfiguration(0.5, 0.85, 0.5, 0.94, 0.07);
    lc->setParameter("useLegend",true);
    lc->setParameter("useLabels",true);
    lc->setParameter("useTitles",false);
    lc->setParameter("textIndex",42);
    lc->setParameter("nColumns",3);
    balFct_LegendConfig1D_DeltaPhi.push_back(lc);
    // I vs Delta y
    lc = new LegendConfiguration(0.20, 0.95, 0.81, 0.98, 0.05);
    lc->setParameter("useLegend",true);
    lc->setParameter("useLabels",true);
    lc->setParameter("useTitles",false);
    lc->setParameter("textIndex",42);
    lc->setParameter("nColumns",3);
    balFct_LegendConfig1D_Integral.push_back(lc);
    // Isum vs Delta y
    lc = new LegendConfiguration(0.20, 0.95, 0.81, 0.98, 0.045);
    lc->setParameter("useLegend",true);
    lc->setParameter("useLabels",true);
    lc->setParameter("useTitles",false);
    lc->setParameter("textIndex",42);
    lc->setParameter("nColumns",3);
    balFct_LegendConfig1D_IntegralSum.push_back(lc);

    // I vs Pair
    lc = new LegendConfiguration(0.60, 0.75, 0.65, 0.9, 0.045);
    lc->setParameter("useLegend",true);
    lc->setParameter("useLabels",true);
    lc->setParameter("useTitles",false);
    lc->setParameter("textIndex",42);
    lc->setParameter("nColumns",1);
    balFct_LegendConfig1D_IntegralVsPair.push_back(lc);

    // width
    lc = new LegendConfiguration(0.20, 0.95, 0.81, 0.98, 0.045);
    lc->setParameter("useLegend",false);
    lc->setParameter("useLabels",true);
    lc->setParameter("useTitles",false);
    lc->setParameter("textIndex",42);
    lc->setParameter("nColumns",1);
    balFct_LegendConfig1D_Width.push_back(lc);

    for (int iSpecies1=0; iSpecies1<nSpecies; iSpecies1++)
      {
      // B vs Delta y
      lc = new LegendConfiguration(0.65, 0.75, 0.5, 0.9, 0.05);
      lc->setParameter("useLegend",true);
      lc->setParameter("useLabels",true);
      lc->setParameter("useTitles",false);
      lc->setParameter("textIndex",42);
      lc->setParameter("nColumns",1);
      for (int iFile=0; iFile<balFct_InputFiles.size(); iFile++)
        {
        int iPair = iSpecies2*nSpecies + iSpecies1;
        TString title = BalFct_HistoTitleBases[iFile];
        title += pairTitles[iPair];
        lc->addLegend(title);

        }
      balFct_LegendConfig1D_DeltaY_VsPair.push_back(lc);

      iPair = iSpecies2*nSpecies + iSpecies1;
      balFct_LegendConfig1D_DeltaY[iSpecies2]->addLegend(pairTitles[iPair]);
      balFct_LegendConfig1D_DeltaPhi[iSpecies2]->addLegend(pairTitles[iPair]);
      balFct_LegendConfig1D_Integral[iSpecies2]->addLegend(pairTitles[iPair]);
      balFct_LegendConfig1D_IntegralSum[iSpecies2]->addLegend(pairCumulTitles[iPair]);
      balFct_LegendConfig1D_Width[iSpecies2]->addLegend(pairTitles[iPair]);
      }

    int nFiles = balFct_InputFiles.size();
    for (int iFile=0; iFile<nFiles; iFile++)
      {
      if (reportDebug(__FUNCTION__)) cout << "iSpecies2=" << iSpecies2 <<  " iFile=" << iFile << " title=" << balFct_IntegralSumVsName_Titles[iFile*nSpecies + iSpecies2] << endl;
       balFct_LegendConfig1D_IntegralVsPair[iSpecies2]->addLegend(balFct_IntegralSumVsName_Titles[iFile*nSpecies + iSpecies2]);
      }
    }



}

void BalFctPlotter::create2DPlots()
{
  double minRapidity;
  double maxRapidity;
  bool   doPlot1DHighRes = getValueBool("DoPlot1DHighRes");
  if (doPlot1DHighRes)
    {
    minRapidity = getValueDouble("MinRapidityHighRes");
    maxRapidity = getValueDouble("MaxRapidityHighRes");
    }
  else
    {
    minRapidity = getValueDouble("MinRapidity");
    maxRapidity = getValueDouble("MaxRapidity");
    }
  for (unsigned int iPlot=0; iPlot<balFct_DeltaYDeltaPhi_Histos.size(); iPlot++)
    {
    //    plotter->plot(balFct_DeltaYDeltaPhi_Histo_InputNames[iPlot],landscapeLinear,*graphConfiguration2D[iPlot],balFct_LegendConfig2Ds[iPlot],
    //                  balFct_DeltaYDeltaPhi_Histos[iPlot],
    //                  DeltaY_Title,minRapidity,maxRapidity,
    //                  DeltaPhi_Title,1.0,-1.0,
    //                  balFct_Titles,balFct_Minima[0], balFct_Maxima[0]);
    cout << " commented out" << endl;
    }
}

void BalFctPlotter::create1DPlots()
{
  double minRapidity;
  double maxRapidity;
  bool   doPlot1DHighRes = getValueBool("DoPlot1DHighRes");
  if (doPlot1DHighRes)
    {
    minRapidity = getValueDouble("MinRapidityHighRes");
    maxRapidity = getValueDouble("MaxRapidityHighRes");
    }
  else
    {
    minRapidity = getValueDouble("MinRapidity");
    maxRapidity = getValueDouble("MaxRapidity");
    }

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " graphConfigurations1D.size()........ : " << graphConfigurations1D.size() << endl;
    cout << " graphConfigurations2D.size()........ : " << graphConfigurations2D.size() << endl;
    cout << " balFct_DeltaY_Histos.size()......... : " << balFct_DeltaY_Histos.size() << endl;
    cout << " balFct_DeltaPhi_Histos.size()....... : " << balFct_DeltaPhi_Histos.size() << endl;
    cout << " balFct_Minima.size()................ : " << balFct_Minima.size() << endl;
    cout << " balFct_Maxima.size()................ : " << balFct_Maxima.size() << endl;
    cout << " DeltaY_Title........................ : " << DeltaY_Title   << endl;
    cout << " DeltaPhi_Title...................... : " << DeltaPhi_Title << endl;
    cout << " balFct_Title........................ : " << balFct_Title   << endl;
    cout << " balFct_DeltaY_CanvasNames.size().... : " << balFct_DeltaY_CanvasNames.size()   << endl;
    cout << " balFct_DeltaPhi_CanvasNames.size().. : " << balFct_DeltaPhi_CanvasNames.size() << endl;
    }

  if (balFct_DeltaY_Histos.size()<1 || balFct_DeltaPhi_Histos.size()<1)
    {
    if (reportError(__FUNCTION__)) cout << "No 1D histogram loaded!!!" << endl;
    postTaskError();
    return;
    }

  int nFiles   = balFct_InputFiles.size();
  int nSpecies = speciesNames.size();
  int nPairs   = pairNames.size();
  CanvasConfiguration landscapeLinear(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);

  for (int iFile=0; iFile<nFiles; iFile++)
    {
    if (reportInfo(__FUNCTION__)) cout << "Plotting histos for iFile = " << iFile << endl;

    for (int iTrigger=0; iTrigger<nSpecies; iTrigger++)
      {
      if (reportInfo(__FUNCTION__)) cout << "Plotting histos for iTrigger = " << iTrigger << endl;

      vector<TH1*> deltaY_Histos;
      vector<TH1*> deltaPhi_Histos;
      for (int iAssoc=0; iAssoc<nSpecies; iAssoc++)
        {
        deltaY_Histos.push_back(    balFct_DeltaY_Histos  [iFile*nSpecies*nSpecies + iTrigger*nSpecies + iAssoc]   );
        deltaPhi_Histos.push_back(  balFct_DeltaPhi_Histos[iFile*nSpecies*nSpecies + iTrigger*nSpecies + iAssoc]   );
        }
      if (reportInfo(__FUNCTION__))
        {
        cout << "deltaY_Histos.size() ..................: " << deltaY_Histos.size() << endl;
        cout << "graphConfigurations1D.size()...........: " << graphConfigurations1D.size() << endl;
        cout << "balFct_LegendConfig1D_DeltaY.size()....: " << balFct_LegendConfig1D_DeltaY.size() << endl;
        cout << "balFct_DeltaY_CanvasNames[iTrigger]....: " << balFct_DeltaY_CanvasNames[iFile*nFiles + iTrigger] << endl;
        cout << "DeltaY_Title...........................: " << DeltaY_Title << endl;
        cout << "minRapidity............................: " << minRapidity << endl;
        cout << "maxRapidity............................: " << maxRapidity << endl;
        cout << "balFct_Title...........................: " << balFct_Title << endl;
        cout << "balFct_Minima[1].......................: " << balFct_Minima[1] << endl;
        cout << "balFct_Maxima[1].......................: " << balFct_Maxima[1] << endl;
        cout << "Call plotting function for Y " <<  endl;
        }


      plot(balFct_DeltaY_CanvasNames[iFile*nSpecies + iTrigger],landscapeLinear,
           graphConfigurations1D,
           *balFct_LegendConfig1D_DeltaY[iTrigger],
           deltaY_Histos,
           DeltaY_Title, minRapidity, maxRapidity,
           balFct_Title, balFct_Minima[1], balFct_Maxima[1]);

      plot(balFct_DeltaPhi_CanvasNames[iFile*nSpecies + iTrigger],landscapeLinear,
           graphConfigurations1D,
           *balFct_LegendConfig1D_DeltaPhi[iTrigger],
           deltaPhi_Histos,
           DeltaPhi_Title, 1.0, -1.0,
           balFct_Title, balFct_Minima[2], balFct_Maxima[2]);
      if (reportInfo(__FUNCTION__)) cout << "Done plotting histos for iTrigger = " << iTrigger << endl;
      }
    }

  for (int iSpecies2=0; iSpecies2<nSpecies; iSpecies2++)
    {
    for (int iSpecies1=0; iSpecies1<nSpecies; iSpecies1++)
      {
      int iPair = iSpecies2*nSpecies + iSpecies1;
      if (reportInfo(__FUNCTION__)) cout << "Plotting histos for iPair = " << iPair << endl;
      vector<TH1*> deltaY_Histos;
      for (int iFile=0; iFile<nFiles; iFile++)
        {
        if (reportInfo(__FUNCTION__)) cout << "Plotting histos for iFile = " << iFile << endl;
        deltaY_Histos.push_back(    balFct_DeltaY_Histos  [iFile*nSpecies*nSpecies + iPair]   );
        }
      plot(balFct_DeltaY_VsPair_CanvasNames[iPair],landscapeLinear,
           graphConfigurations1D,
           *balFct_LegendConfig1D_DeltaY_VsPair[iPair],
           deltaY_Histos,
           DeltaY_Title, -4.0, 4.0,
           balFct_Title, balFct_Minima[1], balFct_Maxima[1]);
      }
    }

  if (reportInfo(__FUNCTION__)) cout << "Done w/ create1DPlots()" << endl;

}

void BalFctPlotter::createIntegralPlots()
{
  double minRapidityInt = getValueDouble("MinRapidityInt");
  double maxRapidityInt = getValueDouble("MaxRapidityInt");

  if (balFct_Integrals.size()<1 || balFct_IntegralSums.size()<1)
    {
    if (reportError(__FUNCTION__)) cout << "No integral graphs loaded!!!" << endl;
    postTaskError();
    return;
    }

  TLine * line = new TLine(0.1,1.0, maxRapidityInt,1.0);
  int nFiles   = balFct_InputFiles.size();
  int nSpecies = speciesNames.size();
  vector<TGraph*> deltaY_Integrals;
  vector<TGraph*> deltaY_IntegralSums;
  vector<TH1*>    deltaY_IntegralVsName;

  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogX(CanvasConfiguration::LandscapeXtop,CanvasConfiguration::LogX);

  for (int iFile=0; iFile<nFiles; iFile++)
    {
    for (int iTrigger=0; iTrigger<nSpecies; iTrigger++)
      {
      deltaY_Integrals.clear();
      deltaY_IntegralSums.clear();

      for (int iAssoc=0; iAssoc<nSpecies; iAssoc++)
        {
        deltaY_Integrals.push_back(     balFct_Integrals[iFile*nSpecies*nSpecies + iTrigger*nSpecies + iAssoc]   );
        deltaY_IntegralSums.push_back(  balFct_IntegralSums[iFile*nSpecies*nSpecies +  iTrigger*nSpecies + iAssoc] );
        }


      plot(balFct_Integral_DeltaY_CanvasNames[iFile*nSpecies + iTrigger],landscapeLogX,
           graphConfigurations1D,
           *balFct_LegendConfig1D_Integral[iTrigger],
           deltaY_Integrals,
           DeltaY_Title, minRapidityInt, maxRapidityInt,
           balFct_Integral_Title, balFct_Minima[3], balFct_Maxima[3]);
      line->Draw();
      plot(balFct_IntegralSum_DeltaY_CanvasNames[iFile*nSpecies + iTrigger],landscapeLogX,
           graphConfigurations1D,
           *balFct_LegendConfig1D_IntegralSum[iTrigger],
           deltaY_IntegralSums,
           DeltaY_Title, minRapidityInt, maxRapidityInt,
           balFct_IntegralSum_Title, balFct_Minima[4], balFct_Maxima[4]);
      line->Draw();
      }
    }

  for (int iTrigger=0; iTrigger<nSpecies; iTrigger++)
    {
    deltaY_IntegralVsName.clear();
    for (int iFile=0; iFile<nFiles; iFile++)
      {
      deltaY_IntegralVsName.push_back( balFct_IntegralVsPair[iFile*nSpecies +  iTrigger] );
      }

   //

    plot(balFct_IntegralVsPair_CanvasNames[iTrigger],landscapeLinear,
         graphConfigurations1D,
         *balFct_LegendConfig1D_IntegralVsPair[iTrigger],
         deltaY_IntegralVsName,
         "", 1.0, -1.0,
         balFct_Integral_Title, -0.02, 1.0);
    }

}

void BalFctPlotter::calculateBalFctWidths()
{

}

void BalFctPlotter::createWidthPlots()
{

}

//
//      if (plotWidths)
//        {
//        vector<double>  rmsWidths;
//        vector<double>  rmsWidthErrors;
//        vector<TH1*>    widthHistos;
//
//        //        balFct12s_DeltaY_Histos;
//        TCanvas * c = new TCanvas();
//        vector<double> rapRange;
//        //rapRange.push_back(2.0);
//        rapRange.push_back(3.0);
//        rapRange.push_back(4.0);
//        rapRange.push_back(5.0);
//        //rapRange.push_back(10.0);
//        TString balFct12s_Width_name = makeName(prefix,"B12s_Width");
//
//        for (int j=0;j<rapRange.size(); j++)
//          {
//          rmsWidths.clear();
//          rmsWidthErrors.clear();
//
//          for (int k=0; k<balFct12s_DeltaY_Histos.size(); k++)
//            {
//            TString name = "f";
//            name += k;
//            name += "_";
//            name += j;
//            double xLowBin  =  -rapRange[j];
//            double xHighBin =  rapRange[j];
//            TF1 * f1 = new TF1(name,GeneralizedGaussian,xLowBin,xHighBin,4);
//            //TF1 * f1 = new TF1(name,"gaus",xLowBin,xHighBin);
//            // set initial parameters (not really needed for gaus)
//            TH1 * h = balFct12s_DeltaY_Histos[k];
//            //f1->SetParameters(h->GetMaximum(), 0.0, 1.0 );
//            f1->SetParName(0,  "A");             f1->SetParameter(0,h->GetMaximum());
//            f1->SetParName(1,  "omegaEta");      f1->SetParameter(1,1.1);
//            f1->SetParName(2,  "gammaEta");      f1->SetParameter(2,2.1);
//            f1->SetParName(3,  "a0");            f1->SetParameter(3,0.00);
//            h->Fit(name,"","",xLowBin,xHighBin);
//
//            TF1 * fitResult = h->GetFunction(name);
//            double a = fitResult->GetParameter(0);
//            double omegaEta = fitResult->GetParameter(1);
//            double gammaEta = fitResult->GetParameter(2);
//            double a0 = fitResult->GetParameter(3);
//            double ea = fitResult->GetParError(0);
//            double eomegaEta = fitResult->GetParError(1);
//            double egammaEta = fitResult->GetParError(2);
//            double ea0 = fitResult->GetParError(3);
//            double chi2 = fitResult->GetChisquare();
//            double ndf  = fitResult->GetNDF();
//            double chi2Ndf  = chi2/ndf;
//
//            double rms  = 0.0;
//            double erms = 0.0;
//            if (a>0.0001 && omegaEta>0.0 && gammaEta>0.0)
//              calculateRmsAndError(omegaEta,eomegaEta,gammaEta,egammaEta,rms,erms);
//
//            cout << " widths ---------------------------------------------" << endl;
//            cout << " a............: " << a << " +- " << ea << endl;
//            cout << " omegaEta.....: " << omegaEta << " +- " << eomegaEta << endl;
//            cout << " gammaEta.....: " << gammaEta << " +- " << egammaEta << endl;
//            cout << " a0...........: " << a0 << " +- " << ea0 << endl;
//            cout << " rms..........: " << rms << " +- " << erms << endl;
//            cout << " chi2.........: " << chi2 << endl;
//            cout << " ndf..........: " << ndf << endl;
//            cout << " chi2/Ndf.....: " << chi2Ndf << endl;
//
//
//            //calculateWidths(h2,MinRapidity, MaxRapidity, rmsWidths, rmsWidthErrors);
//            if (rms>0.0 && rms<2.0 && a>0 )
//              {
//              rmsWidths.push_back(rms); rmsWidthErrors.push_back(erms);
//              }
//            else
//              {
//              rmsWidths.push_back(-1.0); rmsWidthErrors.push_back(0.02);
//              }
//            }
//
//          for (int k=0;k<rmsWidths.size();k++)
//            {
//            cout << "   k : " << k << "  " << rmsWidths[k] << " +- " << rmsWidthErrors[k] << endl;
//            }
//          TString histoName = balFct12s_Width_name;
//          histoName += j;
//          vector<TString> pairTitles = balFct_LegendConfigs[5]->getTexts();
//          TH1 * balFct12s_Width_Histo = makeHistoWithNames(histoName, pairTitles, rmsWidths, rmsWidthErrors);
//          widthHistos.push_back(balFct12s_Width_Histo);
//          }
//
//        plotter->plot(balFct12s_Width_name,landscapeLinear, graphConfigurations1D, *balFct_LegendConfigs[5],
//                      widthHistos,
//                      "",4.0,-4.0,
//                      "RMS",0.0,2.0);
//        //        plotter->plot(balFct12s_Width_name,landscapeLinear, *widthGraphConfiguration, *balFct_LegendConfigs[5],
//        //                      balFct12s_Width_Histo,
//        //                      "",4.0,-4.0,
//        //                      "RMS",0.0,2.0);
//
//
//        }



//      if (plotWidths)
//        {
//        vector<double>  rmsWidths;
//        vector<double>  rmsWidthErrors;
//
////        balFct12s_DeltaY_Histos;
//        TCanvas * c = new TCanvas();
//
//        for (int k=0; k<balFct12s_DeltaY_Histos.size(); k++)
//          {
//          TString name = "f";
//          name += k;
//          TF1 * f1 = new TF1(name,"gaus",-6.0,6.0);
//          // set initial parameters (not really needed for gaus)
//          TH1 * h = balFct12s_DeltaY_Histos[k];
//          f1->SetParameters(h->GetMaximum(), 0.0, 1.0 );
//          h->Fit(name);
//
//          TF1 * fitResult = h->GetFunction(name);
//          double p1 = fitResult->GetParameter(0);
//          double p2 = fitResult->GetParameter(1);
//          double p3 = fitResult->GetParameter(2);
//          double e1 = fitResult->GetParError(0);
//          double e2 = fitResult->GetParError(1);
//          double e3 = fitResult->GetParError(2);
//          cout << " widths ---------------------------------------------" << endl;
//          cout << " p1: " << p1 << " +- " << e1 << endl;
//          cout << " p2: " << p2 << " +- " << e2 << endl;
//          cout << " p3: " << p3 << " +- " << e3 << endl;
//          //calculateWidths(h2,MinRapidity, MaxRapidity, rmsWidths, rmsWidthErrors);
//          rmsWidths.push_back(p3); rmsWidthErrors.push_back(e3);
//          }
//
//
//       for (int k=0;k<rmsWidths.size();k++)
//          {
//          cout << "   k : " << k << "  " << rmsWidths[k] << " +- " << rmsWidthErrors[k] << endl;
//          }
//
//        TString balFct12s_Width_name = makeName(prefix,"B12s");
//        vector<TString> pairTitles = balFct_LegendConfigs[5]->getTexts();
//        TH1 * balFct12s_Width_Histo = makeHistoWithNames(balFct12s_Width_name, pairTitles, rmsWidths, rmsWidthErrors);
//
////
////        for (int k=0; k<pairTitles.size(); k++)
////          {
////          balFct12s_Width_Histo->SetTitle???(k, pairTitles[k]);
////          }
//        plotter->plot(balFct12s_Width_name,landscapeLinear, *widthGraphConfiguration, *balFct_LegendConfigs[5],
//                      balFct12s_Width_Histo,
//                      "",4.0,-4.0,
//                      "RMS",0.0,2.0);
//
//        }




int PlotBFBaryons_version3()
{
  const bool YES = true;
  const bool NO  = false;

  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);

  Configuration configuration;
  //configuration.readFromFile(configFile);
  configuration.setParameter("BalFctPlotter:LogLevel",TString("INFO"));

  configuration.setParameter("BalFctPlotter:Rebin2D",               YES);
  configuration.setParameter("BalFctPlotter:DoPlot1D",              YES);
  configuration.setParameter("BalFctPlotter:DoPlot2D",              NO);
  configuration.setParameter("BalFctPlotter:DoPlot1DHighRes",       NO);
  configuration.setParameter("BalFctPlotter:DoCalculateIntegrals",  YES);
  configuration.setParameter("BalFctPlotter:DoPlotIntegrals",       YES);
  configuration.setParameter("BalFctPlotter:DoCalculateWidths",     NO);
  configuration.setParameter("BalFctPlotter:DoPlotWidths",          NO);
  configuration.setParameter("BalFctPlotter:DoPrint",               YES);
  configuration.setParameter("BalFctPlotter:DoPrintPng",            NO);
  configuration.setParameter("BalFctPlotter:DoPrintPdf",            YES);
  configuration.setParameter("BalFctPlotter:OutputPathBase",        TString("/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/CompareS/"));
  configuration.setParameter("BalFctPlotter:OutFileNameBase",       TString("PYTHIA_pp_CompareS_A2Based"));
  configuration.setParameter("BalFctPlotter:MinRapidity",           -20.00);
  configuration.setParameter("BalFctPlotter:MaxRapidity",            20.00);
  configuration.setParameter("BalFctPlotter:MinRapidityHighRes",    -4.00);
  configuration.setParameter("BalFctPlotter:MaxRapidityHighRes",     4.00);
  configuration.setParameter("BalFctPlotter:MinRapidityInt",         0.00);
  configuration.setParameter("BalFctPlotter:MaxRapidityInt",         20.00);
  configuration.setParameter("BalFctPlotter:AcceptanceOption",       0);   // Y10, ...
  configuration.setParameter("BalFctPlotter:BalFctTypeOption",       2);   // B+-, B-+, Bs
  configuration.setParameter("BalFctPlotter:BalFctSourceOption",     1);   // from B or from A type correlators...

  int choice = 1;

  switch (choice)
    {
      case 0:
      configuration.setParameter("BalFctPlotter:SpeciesOption",          2);   // Baryons

      configuration.setParameter("BalFctPlotter:BalFct_InputPath_0",      TString("/Volumes/ClaudeDisc5/OutputFiles/PYTHIA/PP/900/Baryons/Y10/"));
      configuration.setParameter("BalFctPlotter:BalFct_InputFile_0",      TString("PairGenBalFctSum0TO19.root"));
      configuration.setParameter("BalFctPlotter:BalFct_HistoTitleBase_0", TString("#sqrt{s}=0.90 TeV; "));
      configuration.setParameter("BalFctPlotter:BalFct_HistoNameBase_0",  TString("PairGen_All_"));
      configuration.setParameter("BalFctPlotter:BalFct_CanvasNameBase_0", TString("900_Baryons_Y10_"));
      configuration.setParameter("BalFctPlotter:BalFct_Type_0",          2);

      configuration.setParameter("BalFctPlotter:BalFct_InputPath_1",     TString("/Volumes/ClaudeDisc5/OutputFiles/PYTHIA/PP/13000/Baryons/Y10/"));
      configuration.setParameter("BalFctPlotter:BalFct_InputFile_1",     TString("PairGenBalFctSum0TO19.root"));
      configuration.setParameter("BalFctPlotter:BalFct_HistoTitleBase_1",TString("#sqrt{s}=13.0 TeV; "));
      configuration.setParameter("BalFctPlotter:BalFct_HistoNameBase_1", TString("PairGen_All_"));
      configuration.setParameter("BalFctPlotter:BalFct_CanvasNameBase_1", TString("13000_Baryons_Y10_"));
      configuration.setParameter("BalFctPlotter:BalFct_Type_1",          2);

      configuration.setParameter("BalFctPlotter:BalFct_InputPath_2",     TString("/Volumes/ClaudeDisc5/OutputFiles/PYTHIA/PP/30000/Baryons/Y10/"));
      configuration.setParameter("BalFctPlotter:BalFct_InputFile_2",     TString("PairGenBalFctSum0TO19.root"));
      configuration.setParameter("BalFctPlotter:BalFct_HistoTitleBase_2",TString("#sqrt{s}=30.0 TeV; "));
      configuration.setParameter("BalFctPlotter:BalFct_HistoNameBase_2", TString("PairGen_All_"));
      configuration.setParameter("BalFctPlotter:BalFct_CanvasNameBase_2", TString("30000_Baryons_Y10_"));
      configuration.setParameter("BalFctPlotter:BalFct_Type_2",          2);
      break;

      case 1:
      configuration.setParameter("BalFctPlotter:SpeciesOption",          1);   // pi K p

      configuration.setParameter("BalFctPlotter:BalFct_InputPath_0",      TString("/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/13000/PiKp/y10"));
      configuration.setParameter("BalFctPlotter:BalFct_InputFile_0",      TString("PairGenBalFctSum0TO9.root"));
      configuration.setParameter("BalFctPlotter:BalFct_HistoTitleBase_0", TString("Monash - "));
      configuration.setParameter("BalFctPlotter:BalFct_HistoNameBase_0",  TString("PairGen_All_"));
      configuration.setParameter("BalFctPlotter:BalFct_CanvasNameBase_0", TString("13000_PiKp_Y10_"));
      configuration.setParameter("BalFctPlotter:BalFct_Type_0",          2);

      configuration.setParameter("BalFctPlotter:BalFct_InputPath_1",      TString("/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/13000/PiKp_Ropes"));
      configuration.setParameter("BalFctPlotter:BalFct_InputFile_1",      TString("PairGenBalFctSum0TO9.root"));
      configuration.setParameter("BalFctPlotter:BalFct_HistoTitleBase_1", TString("Ropes - "));
      configuration.setParameter("BalFctPlotter:BalFct_HistoNameBase_1",  TString("PairGen_All_"));
      configuration.setParameter("BalFctPlotter:BalFct_CanvasNameBase_1", TString("13000_ropes_PiKp_Y10_"));
      configuration.setParameter("BalFctPlotter:BalFct_Type_1",          2);

      configuration.setParameter("BalFctPlotter:BalFct_InputPath_2",      TString("/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/13000/PiKp_Shoving"));
      configuration.setParameter("BalFctPlotter:BalFct_InputFile_2",      TString("PairGenBalFctSum0TO7.root"));
      configuration.setParameter("BalFctPlotter:BalFct_HistoTitleBase_2", TString("Shoving - "));
      configuration.setParameter("BalFctPlotter:BalFct_HistoNameBase_2",  TString("PairGen_All_"));
      configuration.setParameter("BalFctPlotter:BalFct_CanvasNameBase_2", TString("13000_shoving_PiKp_Y10_"));
      configuration.setParameter("BalFctPlotter:BalFct_Type_2",          2);

      break;

    }





  BalFctPlotter * bf = new BalFctPlotter("BalFctPlotter",configuration);
  bf->configure();
  bf->execute();

  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load(includePath+"TaskIterator.hpp");
  gSystem->Load(includePath+"Collection.hpp");
  gSystem->Load(includePath+"CanvasCollection.hpp");
  gSystem->Load(includePath+"GraphConfiguration.hpp");
  gSystem->Load(includePath+"CanvasConfiguration.hpp");
  gSystem->Load(includePath+"HistogramCollection.hpp");
  gSystem->Load(includePath+"Histograms.hpp");
  gSystem->Load(includePath+"Particle.hpp");
  gSystem->Load(includePath+"ParticleType.hpp");
  gSystem->Load(includePath+"ParticleTypeCollection.hpp");
  gSystem->Load(includePath+"ParticleDecayMode.hpp");
  gSystem->Load(includePath+"Plotter.hpp");
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
}

