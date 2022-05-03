//  Created by Claude Pruneau on 6/19/2020.
//  Copyright © 2020 Claude Pruneau. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>

int RunPartitionCalculator()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"PartitionCalculator.hpp");
  gSystem->Load("libBase.dylib");

  cout << "<I> RunPartitionCalculator()" << endl;
  TString inputPathName  = getenv("WAC_REPOSITORY_PATH");
  TString outputPathName = getenv("WAC_OUTPUT_PATH");
  inputPathName  += "/PYTHIA/7TeV/MinBias/";
  outputPathName += "/PYTHIA/7TeV/MinBias/";
  gSystem->mkdir(outputPathName,1);
  TString inputFileName  = "PYTHIA_pp_7TeV_inelastic_Global_Sum.root";
  TString inputHistoName = "Global_MB_V0M_n";
  TString name = inputPathName+inputFileName;
  cout << "<I> Input path/file:" << name << endl;
  cout << "<I> Input histogram:" << inputHistoName << endl;
  TFile * inputFile = new TFile(name,"OLD");
  if (!inputFile)
    {
    cout << "<F> RunPartitionCalculator() - File not found: " << name << endl;
    cout << "<F> RunPartitionCalculator() - ABORT" << endl;
    return 1;
    }
  TH1D * histo = (TH1D*) inputFile->Get(inputHistoName);
  if (!histo)
    {
    cout << "<F> RunPartitionCalculator()() - Histo not found: " << inputHistoName << endl;
    cout << "<F> RunPartitionCalculator()() - ABORT" << endl;
    return 1;
    }
  histo->Draw();

  vector<double> fractions; // must include both min and max values, i.e., n+1 values, where n is number of bins
  vector<double> boundaries; // partition boundaries sought for...

  fractions.push_back( 0.0);
  fractions.push_back( 0.1);
  fractions.push_back( 0.2);
  fractions.push_back( 0.3);
  fractions.push_back( 0.4);
  fractions.push_back( 0.5);
  fractions.push_back( 0.6);
  fractions.push_back( 0.7);
  fractions.push_back( 0.8);
  fractions.push_back( 0.9);
  fractions.push_back( 1.0);
  PartitionCalculator calculator;
  calculator.computePartitions(histo, fractions, boundaries);
  cout << "<I> RunPartitionCalculator() -- Completed" << endl;
  return 0;
}


