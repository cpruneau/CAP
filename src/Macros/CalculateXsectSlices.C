//  Created by Claude Pruneau on 6/19/2020.
//  Copyright © 2020 Claude Pruneau. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>

int CalculateXsectSlices()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"Factory.hpp");
  gSystem->Load(includesPath+"FunctionCollection.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load(includesPath+"RandomGenerators.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load("libBase.dylib");


  cout << "<I> CalculateXsectSlices() - Starting" << endl;

  TString inputPathName = "/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles//PYTHIA/7TEV/SpeedTest/";
  TString inputFileName = "PYTHIA_pp_7TeV_inelastic_G1Pre_Sum.root";
  TString histoName = "G1Pre_MB_V0M_n";

  TFile * inputFile = new TFile(inputPathName+inputFileName,"OLD");
  if (!inputFile)
    {
    cout << "<F> CalculateXsectSlices() - File not found: " << inputPathName+inputFileName << endl;
    cout << "<F> CalculateXsectSlices() - ABORT" << endl;
    return 1;
    }
  TH1D * histo = (TH1D*) inputFile->Get(histoName);
  if (!histo)
    {
    cout << "<F> CalculateXsectSlices() - Histo not found: " << histoName << endl;
    cout << "<F> CalculateXsectSlices() - ABORT" << endl;
    return 1;
    }
  histo->Draw();

  double fractions[6];
  fractions[0] = 0.2;
  fractions[1] = 0.4;
  fractions[2] = 0.6;
  fractions[3] = 0.8;
  fractions[4] = 1.00;
  fractions[5] = 2.00;

  double sum = 0.0;
  double count = 0;
  double frac;
  double content;

  int n = histo->GetNbinsX();
  for (int i=1; i<=n; i++)
  {
  content = histo->GetBinContent(i);
  sum += content;
  }

  int iFrac = 0;
  for (int i=1; i<=n; i++)
  {
  double edge = histo->GetBinLowEdge(i);
  double width = histo->GetBinWidth(i);
  content = histo->GetBinContent(i);
  count += content;
  frac = count/sum;
  //cout << content << "    "  << count << "     " << frac << "    " << edge << endl;
  if (frac>fractions[iFrac])
    {
    double excessFrac = (frac - fractions[iFrac]);
    double adjustedEdge = edge - excessFrac*width;
    cout << frac  << "   " << edge <<  "   " << adjustedEdge << endl;
    iFrac++;
    }
  }

  cout << "<I> CalculateXsectSlices() - Done" << endl;
  return 0;
}


