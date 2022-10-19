#include <iostream>
#include <fstream>
#include <vector>
#include <TStyle.h>
#include <TROOT.h>

void loadBase(const TString & includeBasePath);

void calculateOmegaFactor(TH2 * h_DyDphi, vector<double> & omegaFactor)
{
  int nYBins   = h_DyDphi->GetNbinsX();
  int nPhiBins = h_DyDphi->GetNbinsY();
  int nY = (nYBins+1)/2;
  omegaFactor.assign(nYBins,1.0);
  double sum;
  int yIndex = 0;
  for (int iY=1; iY<=nY; iY++)
    {
    sum = 0.0;
    for (int iPhi=1; iPhi<=nPhiBins; iPhi++)
      {
      sum += h_DyDphi->GetBinContent(iY,iPhi);
      }
    if (sum>0.0)
      {
      yIndex = iY;
      break;
      }

    }
  cout << "   nYBins : " << nYBins << endl;
  cout << "       nY : " << nY << endl;
  cout << "   yIndex : " << yIndex << endl;
  double y0 = fabs(h_DyDphi->GetXaxis()->GetBinLowEdge(yIndex));
  cout << "       y0 : " << y0 << endl;
  for (int iY=yIndex; iY<=nYBins+1-yIndex; iY++)
    {
    double dy = h_DyDphi->GetXaxis()->GetBinCenter(iY);
    omegaFactor[-1+iY] = y0-fabs(dy);
    cout << " iY : " << iY << " omega[iY] : " << omegaFactor[-1+iY] << endl;
    }
}


void calculateAverageYbar(TH2 * h_DyDphi, TH2 * h_DyDphi_Avg, vector<double> & omegaFactor)
{
  int nYBins   = h_DyDphi->GetNbinsX();
  int nPhiBins = h_DyDphi->GetNbinsY();
  for (int iY=1; iY<=nYBins; iY++)
    {
    double scale = 1.0/omegaFactor[iY-1];
    for (int iPhi=1; iPhi<=nPhiBins; iPhi++)
      {
      double v  =  h_DyDphi->GetBinContent(iY,iPhi);
      double ev =  h_DyDphi->GetBinError(iY,iPhi);
      h_DyDphi_Avg->SetBinContent(iY,iPhi,v*scale);
      h_DyDphi_Avg->SetBinError(iY,iPhi,ev*scale);
      }
    }
}



int test()
{
  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  MessageLogger::LogLevel infoLevel = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Info;
  MessageLogger::LogLevel selectLevel = infoLevel;

  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 0;
  bool useColor = true;
  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  vector<GraphConfiguration*>  graphConfigurations2D = GraphConfiguration::createConfigurationPalette(10,2);
  Configuration config;

  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/1000/PM/Y1/";
  TString inputFile  = "PairGenBalFctSum0TO9.root";
  Plotter * plotter = new Plotter("Plotter",config);
  plotter->setDefaultOptions(useColor);

  TString histoName = "PairGen_All_HP_HP_A2_DyDphi_shft_B2_1_2Bar";
  TFile * f =  plotter->openRootFile(inputPath,inputFile,"OLD");
  if (!f)
    {
    cout << " Could not open file named " << inputFile << endl;
    cout << "                      from " << inputPath << endl;
    cout << " ABORT!!!!" << endl;
    return -1;
    }

  TH2 * h2 = (TH2*) f->Get(histoName);
  if (!h2)
  {
  cout << "Could not load histogram: " << histoName << endl;
  return -1;
  }

  plotter->plot(h2,"original",landscapeLinear,*(graphConfigurations2D[0]), "y", 1.0, -1.0,  "#phi", 1.0, -1.0,  "yield", 1.0, -1.0);
  //h2->Draw("SURF3");

  TH2 *h2A = (TH2*) h2->Clone();
  h2A->Reset();

  vector<double> omegaFactor;
  calculateOmegaFactor(h2,omegaFactor);
  calculateAverageYbar(h2,h2A, omegaFactor);
  plotter->plot(h2A,"clone",landscapeLinear,*(graphConfigurations2D[0]), "y", 1.0, -1.0,  "#phi", 1.0, -1.0,  "yield", 1.0, -1.0);

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
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
}
