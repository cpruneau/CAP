//  Created by Claude Pruneau on 6/19/2020.
//  Copyright © 2020 Claude Pruneau. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
#include "CollisionGeometry.hpp"
#include "CollisionGeometryConfiguration.hpp"
#include "CollisionGeometryGenerator.hpp"
#include "CollisionGeometryAnalyzer.hpp"
#include "EventLoop.hpp"

int CalculateBSlices()
{
  cout << "<I> CalculateBSlices() - Starting" << endl;

  TString inputPathName = "/Users/claudeapruneau/Documents/GitHub/run/GeometryStudies/";
  TString inputFileName = "CollisionGeometryPbPbWS-Nominal.root";
  TString histoName = "geom_b";

  TFile * inputFile = new TFile(inputPathName+inputFileName,"OLD");
  if (!inputFile)
    {
    cout << "<F> CalculateBSlices() - File not found: " << inputPathName+inputFileName << endl;
    cout << "<F> CalculateBSlices() - ABORT" << endl;
    return 1;
    }
  TH1D * histo = (TH1D*) inputFile->Get(histoName);
  if (!histo)
    {
    cout << "<F> CalculateBSlices() - Histo not found: " << histoName << endl;
    cout << "<F> CalculateBSlices() - ABORT" << endl;
    return 1;
    }
  histo->Draw();

  double fractions[12];
  fractions[0] = 0.05;
  fractions[1] = 0.1;
  fractions[2] = 0.2;
  fractions[3] = 0.3;
  fractions[4] = 0.4;
  fractions[5] = 0.5;
  fractions[6] = 0.6;
  fractions[7] = 0.7;
  fractions[8] = 0.8;
  fractions[9] = 0.9;
  fractions[10] = 1.00;
  fractions[11] = 2.00;

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

  cout << "<I> CalculateBSlices() - Done" << endl;
  return 0;
}


