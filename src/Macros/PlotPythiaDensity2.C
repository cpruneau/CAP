
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

int PlotPythiaDensity2()
{
  TString includeBasePath = getenv("CAP_SRC");
  loadBase(includeBasePath);
  MessageLogger::LogLevel infoLevel = MessageLogger::Info;
  MessageLogger::LogLevel debugLevel = MessageLogger::Info;
  MessageLogger::LogLevel selectLevel = infoLevel;

  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 1;
  bool useColor = true;
  CanvasConfiguration landscapeLinear(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLinearTight(CanvasConfiguration::LandscapeTight,CanvasConfiguration::Linear);
  CanvasConfiguration landscapeLogY(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations1D = GraphConfiguration::createConfigurationPalette(10,1);
  vector<GraphConfiguration*>  graphConfigurations2D = GraphConfiguration::createConfigurationPalette(10,2);


//  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/2700/PM/Y10WidePt/";
//  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/2700/PM/Y10WidePt/";
  TString inputPath  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/13000/PM/Y10/";
  TString outputPath = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA/PP/13000/PM/Y10/";

  vector<TString> histoInputFileNames;
  vector<TFile*>  histoInputFiles;
  vector<TString> histoNames2D;
  vector<TString> histoNames1Dx;
  vector<TString> histoNames1Dy;
  vector<TString> names;
  vector<TString> titles;
  vector<TH2*>    histos2D;
  vector<TH1*>    histos1Dx;
  vector<TH1*>    histos1Dy;


  names.push_back(TString("PYTHIA_pp_13.0TeV_rho1_y_hp"));
  titles.push_back(TString(" #rho_{1}^{+}"));
  histoNames1Dx.push_back(TString("Part_Gen_All_HP_n1_y"));

  names.push_back(TString("PYTHIA_pp_13.0TeV_rho1_y_hp"));
  titles.push_back(TString(" #rho_{1}^{-}"));
  histoNames1Dx.push_back(TString("Part_Gen_All_HM_n1_y"));


  Configuration taskConfig;

  Plotter * plotter = new Plotter("Plotter",taskConfig);
  plotter->setDefaultOptions(useColor);

  TFile * f =  plotter->openRootFile(inputPath,"PairGenDerivedSum0TO4.root","OLD");
  if (!f)
    {
    cout << " Could not open file named PairGenDerivedSum0TO4.root" <<   endl;
    cout << " ABORT!!!!" << endl;
    return -1;
    }
  TH2 * plus2D = (TH2*) f->Get("PairGen_All_HP_n1_phiY");
  if (!plus2D)
    {
    cout << "PairGen_All_HP_n1_phiY not found - abort" << endl;
    }
  TH2 * minus2D = (TH2*) f->Get("PairGen_All_HM_n1_phiY");
  if (!minus2D)
    {
    cout << "PairGen_All_HM_n1_phiY not found - abort" << endl;
    }

  TH1D * plus1D = plus2D->ProjectionX();
  TH1D * minus1D = minus2D->ProjectionX();

  double binWidth = plus1D->GetXaxis()->GetBinWidth(1);
  double scale = 1.0/binWidth;
  plus1D->Scale(scale);
  minus1D->Scale(scale);
  double integralPlus   = plus1D->Integral("WIDTH");
  double integralMinus  = minus1D->Integral("WIDTH");
  cout << "   plus Integral:" << integralPlus  << endl;
  cout << "  minus Integral:" << integralMinus << endl;
  cout << "   diff Integral:" << integralPlus - integralMinus << endl;
  histos1Dx.push_back(plus1D);
  histos1Dx.push_back(minus1D);

  landscapeLinear.setParameter("windowHeight", 300);

  for (unsigned int iFile=0; iFile<2; iFile++)
    {
    graphConfigurations1D[iFile]->setParameter("yTitleOffset",  0.9);
    graphConfigurations1D[iFile]->setParameter("xTitleSize",   0.1);
    graphConfigurations1D[iFile]->setParameter("xTitleOffset", 0.7);
    graphConfigurations1D[iFile]->setParameter("xLabelSize",   0.08);
    graphConfigurations1D[iFile]->setParameter("xLabelOffset", 0.01);
    graphConfigurations1D[iFile]->setParameter("yTitleSize",   0.11);
    graphConfigurations1D[iFile]->setParameter("yTitleOffset", 0.5);
    graphConfigurations1D[iFile]->setParameter("yLabelSize",   0.085);
    graphConfigurations1D[iFile]->setParameter("yLabelOffset", 0.01);
    graphConfigurations1D[iFile]->setParameter("markerSize",  0.7);
    }




  plotter->plot(histos1Dx,graphConfigurations1D,titles,"PYTHIA_pp_13.0TeV_rho1_y_HP_HM",landscapeLinear,TString("y"), -10.0, 10.0,TString("#rho_{1}(y)"),0.0, 4.499,
                0.70, 0.62, 0.82, 0.9, 0.1);

  TH1 * diff = (TH1*) histos1Dx[0]->Clone();
  diff->Add(histos1Dx[1],-1.0);

  GraphConfiguration config = *new GraphConfiguration(1,0);
  config.setParameter("xTitleSize",   0.1);
  config.setParameter("xTitleOffset", 0.7);
  config.setParameter("xLabelSize",   0.08);
  config.setParameter("xLabelOffset", 0.01);
  config.setParameter("yTitleSize",   0.11);
  config.setParameter("yTitleOffset", 0.5);
  config.setParameter("yLabelSize",   0.085);
  config.setParameter("yLabelOffset", 0.01);

  plotter->plot(diff,"PYTHIA_pp_13.0TeV_rho1_y_Difference",landscapeLinear,config,
                TString("y"), -10.0, 10.0,
                TString("#rho_{1}^{+}(y)-#rho_{1}^{-}(y)"),-0.0990, 0.750,
                "PYTHIA8 pp #sqrt{s}=13.0 TeV", 0.5, 0.77, 0.82, 0.90, 0.08);

  plotter->printAllCanvas(outputPath, printGif, printPdf, printSvg, printC);
  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Configuration.hpp");
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
