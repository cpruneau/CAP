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
#include "TString.h"
#include "TRandom.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <string>
#include <stdio.h>
#include "BalanceFunctionCalculator.hpp"

BalanceFunctionCalculator::BalanceFunctionCalculator(const TString &          _name,
                                                     const Configuration &    _configuration,
                                                     vector<EventFilter*> &   _eventFilters,
                                                     vector<ParticleFilter*>& _particleFilters,
                                                     vector<TString> &        _sObservableNames,
                                                     vector<TString> &        _pObservableNames,
                                                     MessageLogger::LogLevel  _selectedLevel)
:
Task(_name,_configuration,_eventFilters, _particleFilters, _selectedLevel),
sObservableNames(_sObservableNames),
pObservableNames(_pObservableNames)
{
  appendClassName("BalanceFunctionCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}



void BalanceFunctionCalculator::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  configuration.setName("BalanceFunctionCalculator Configuration");
  configuration.setParameter( "useParticles",           false);
  configuration.setParameter( "createHistograms",       true);
  configuration.setParameter( "loadHistograms",         true);
  configuration.setParameter( "saveHistograms",         true);
  configuration.setParameter( "forceHistogramsRewrite", true);

  configuration.addParameter("fillEta",  true);
  configuration.addParameter("fillY",    false);
  configuration.addParameter("fillP2",   false);
  
  configuration.addParameter("nBins_n1",  100);
  configuration.addParameter("min_n1",    0.0);
  configuration.addParameter("max_n1",  100.0);
  
  configuration.addParameter("nBins_eTot",  100);
  configuration.addParameter("min_eTot",    0.0);
  configuration.addParameter("max_eTot",  100.0);
  
  configuration.addParameter("nBins_pt",   18);
  configuration.addParameter("min_pt",   0.20);
  configuration.addParameter("max_pt",   2.00);
  
  configuration.addParameter("nBins_phi",  72);
  configuration.addParameter("min_phi",   0.0);
  configuration.addParameter("max_phi", TMath::TwoPi());
  
  configuration.addParameter("nBins_eta",   20);
  configuration.addParameter("min_eta",   -1.0);
  configuration.addParameter("max_eta",    1.0);
  
  configuration.addParameter("nBins_y",     20);
  configuration.addParameter("min_y",     -1.0);
  configuration.addParameter("max_y",      1.0);
  
  configuration.addParameter("nBins_phiEta",    720);
  configuration.addParameter("nBins_phiEtaPt",  7200);
  configuration.addParameter("nBins_phiY",      720);
  configuration.addParameter("nBins_phiYPt",    7200);
  
  configuration.addParameter("nBins_n2",          100);
  configuration.addParameter("min_n2",            0.0);
  configuration.addParameter("max_n2",         1000.0);
  configuration.addParameter("nBins_DeltaPlong",   10);
  configuration.addParameter("min_DeltaPlong",   -1.0);
  configuration.addParameter("max_DeltaPlong",    1.0);
  configuration.addParameter("nBins_DeltaPside",   10);
  configuration.addParameter("min_DeltaPside",   -1.0);
  configuration.addParameter("max_DeltaPside",    1.0);
  configuration.addParameter("range_DeltaPside",  2.0);
  configuration.addParameter("nBins_DeltaPout",    10);
  configuration.addParameter("min_DeltaPout",    -1.0);
  configuration.addParameter("max_DeltaPout",     1.0);
  configuration.addParameter("range_DeltaPout",   2.0);
  configuration.addParameter("nBins_Dphi",         36);
  configuration.addParameter("min_Dphi",          0.0);
  configuration.addParameter("max_Dphi",TMath::TwoPi());
  configuration.addParameter("width_Dphi",TMath::TwoPi());
  configuration.addParameter("nBins_Dphi_shft",    36);
  configuration.addParameter("min_Dphi_shft",     0.0);
  configuration.addParameter("max_Dphi_shft",     0.0);
  configuration.addParameter("nBins_Deta",         39);
  configuration.addParameter("min_Deta",         -2.0);
  configuration.addParameter("max_Deta",          2.0);
  configuration.addParameter("width_Deta",   4.0/39.0);
  configuration.addParameter("nBins_Dy",           39);
  configuration.addParameter("min_Dy",           -2.0);
  configuration.addParameter("max_Dy",            2.0);
  configuration.addParameter("width_Dy",     4.0/39.0);
  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}


//!
//! Create Balance Function Histograms for the input file, event classes, observables and particles...
//!
void BalanceFunctionCalculator::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  TString histoInputPath      = configuration.getValueString( "histoInputPath"      );
  TString histoInputFileName  = configuration.getValueString( "histoInputFileName"  );
  TString histoOutputPath     = configuration.getValueString( "histoOutputPath"     );
  TString histoOutputFileName = configuration.getValueString( "histoOutputFileName" );
  TString histoBaseName       = configuration.getValueString( "histoBaseName"       );
  bool forceHistogramsRewrite = configuration.getValueBool("forceHistogramsRewrite" );
  histoOutputFileName += "_BF.root";
  
  bool calculateCI = false;
  bool calculateCD = false;
  bool calculateBFv1 = false;
  bool calculateDiffs = false;
  bool calculateBFv2 = true;

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "         histoInputPath: " << histoInputPath << endl;
    cout << "     histoInputFileName: " << histoInputFileName<< endl;
    cout << "        histoOutputPath: " << histoOutputPath << endl;
    cout << "    histoOutputFileName: " << histoOutputFileName << endl;
    cout << "          histoBaseName: " << histoBaseName << endl;
    cout << " forceHistogramsRewrite: " << forceHistogramsRewrite << endl;
    }
  
  TFile * inputFile = nullptr;
  TFile * outputFile = nullptr;
  inputFile = openRootFile(histoInputPath,histoInputFileName,"OLD");
  if (!inputFile) return;
  

  if (forceHistogramsRewrite)
    outputFile = openRootFile(histoOutputPath, histoOutputFileName, "RECREATE");
  else
    outputFile = openRootFile(histoOutputPath, histoOutputFileName, "NEW");
  if (!outputFile) return;
  
  // Use hh as helper to load and calculate histograms, etc.
  Histograms * hh = new Histograms(getName(), getConfiguration(), getLogLevel());
  hh->setOwnership(false);
  
  TH1 * rho1_1;
  TH1 * rho1_1B;
  TH1 * rho1_2;
  TH1 * rho1_2B;
  TH2 * rho1_1_phiY;
  TH2 * rho1_1B_phiY;
  TH2 * rho1_2_phiY;
  TH2 * rho1_2B_phiY;
  TH2 * r2_12;
  TH2 * r2_1B2;
  TH2 * r2_12B;
  TH2 * r2_1B2B;
  
  TH2 * b2_12;
  TH2 * b2_1B2;
  TH2 * b2_12B;
  TH2 * b2_1B2B;
  TH2 * bf_12B;
  TH2 * bf_1B2;
  
  TH2 * r2CI;
  TH2 * r2CD;
  TH2 * b12B;
  TH2 * b1B2;
  TH1 * h_x;
  TH1 * h_y;
  TH2 * r2Diff;
  TH2 * b2Diff;
  
  double rho1Avg, erho1Avg;
  double rho1BarAvg, erho1BarAvg;
  float nx, ny, sum, width;
  double integralBF12B, integralBF1B2;
  double eIntegralBF12B, eIntegralBF1B2;
  TString name;
  
  vector<double> integralsBF12B;
  vector<double> integralsBF1B2;
  vector<double> eIntegralsBF12B; // estimated errors
  vector<double> eIntegralsBF1B2; // estimated errors
  long nEventProcessed = readParameter(inputFile, "EventProcessed");
  long nEventAccepted  = readParameter(inputFile, "EventAccepted");
  unsigned int nSpecies = particleFilters.size()/2;
  for (unsigned int iObservable = 0; iObservable<pObservableNames.size();iObservable++)
    {
    for (unsigned int iPart1=0; iPart1<nSpecies; iPart1++)
      {
      for (unsigned int iPart2=0; iPart2<nSpecies; iPart2++)
        {
        for (unsigned int iEventClass = 0; iEventClass<eventFilters.size();iEventClass++)
          {
          // load histogram and compute derived files.
          TString eventClassName  = eventFilters[iEventClass]->getName();
          TString particleName1   = particleFilters[iPart1]->getName();
          TString particleName1b  = particleFilters[iPart1+nSpecies]->getName();
          TString particleName2   = particleFilters[iPart2]->getName();
          TString particleName2b  = particleFilters[iPart2+nSpecies]->getName();
          rho1_1  = hh->loadH1(inputFile,createHistogramName(histoBaseName,eventClassName,particleName1, sObservableNames[0]));
          rho1_1B = hh->loadH1(inputFile,createHistogramName(histoBaseName,eventClassName,particleName1b,sObservableNames[0]));
          rho1_2  = hh->loadH1(inputFile,createHistogramName(histoBaseName,eventClassName,particleName2, sObservableNames[0]));
          rho1_2B = hh->loadH1(inputFile,createHistogramName(histoBaseName,eventClassName,particleName2b,sObservableNames[0]));
          if (!rho1_1 || !rho1_1B) return;
          hh->calculateAverage(rho1_1, rho1Avg,   erho1Avg);
          hh->calculateAverage(rho1_1B,rho1BarAvg,erho1BarAvg);
          if (!rho1_2 || !rho1_2B) return;
          r2_12     = hh->loadH2(inputFile,createPairHistogramName(histoBaseName,eventClassName,particleName1, particleName2,  pObservableNames[iObservable]));
          r2_1B2    = hh->loadH2(inputFile,createPairHistogramName(histoBaseName,eventClassName,particleName1b,particleName2,  pObservableNames[iObservable]));
          r2_12B    = hh->loadH2(inputFile,createPairHistogramName(histoBaseName,eventClassName,particleName1, particleName2b, pObservableNames[iObservable]));
          r2_1B2B   = hh->loadH2(inputFile,createPairHistogramName(histoBaseName,eventClassName,particleName1b,particleName2b, pObservableNames[iObservable]));
          if (!r2_12 || !r2_1B2 || !r2_12B || !r2_1B2B) return;
          
       
          
          if (calculateCI)
            {
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_CI" );
            r2CI = (TH2*) r2_1B2->Clone();
            r2CI->SetName(name);
            r2CI->SetTitle(name);
            r2CI->Add(r2_12B);
            r2CI->Add(r2_12);
            r2CI->Add(r2_1B2B);
            r2CI->Scale(0.25);
            hh->append(r2CI);
            
            nx = r2CI->GetNbinsX();
            ny = r2CI->GetNbinsY();
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable],"_CI_x");
            h_x = r2CI->ProjectionX();
            h_x->SetName(name);
            h_x->SetTitle(name);
            //h_x->Scale(1.0/ny);
            hh->append(h_x);
            
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable],"_CI_y");
            h_y = r2CI->ProjectionY();
            h_y->SetName(name);
            h_y->SetTitle(name);
            //h_y->Scale(1.0/nx);
            hh->append(h_y);
            }
          
          if (calculateCD)
            {
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_CD");
            r2CD = (TH2*) r2_1B2->Clone();
            r2CD->SetName(name);
            r2CD->SetTitle(name);
            r2CD->Add(r2_12B);
            r2CD->Add(r2_12, -1.0);
            r2CD->Add(r2_1B2B, -1.0);
            r2CD->Scale(0.25);
            hh->append(r2CD);
            
            nx = r2CD->GetNbinsX();
            ny = r2CD->GetNbinsY();
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_CD_x");
            h_x = r2CD->ProjectionX();
            h_x->SetName(name);
            h_x->SetTitle(name);
            //h_x->Scale(1.0/ny);
            hh->append(h_x);
            
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_CD_y");
            h_y = r2CD->ProjectionY();
            h_y->SetName(name);
            h_y->SetTitle(name);
            //h_y->Scale(1.0/nx);
            hh->append(h_y);
            }
          
          if (calculateBFv1)
            {
            // =====
            // B12B
            // =====
            name  = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_B12B");
            b12B = (TH2*) r2_12B->Clone();
            b12B->SetName(name);
            b12B->SetTitle(name);
            b12B->Scale(rho1Avg); // correct
            b12B->Add(r2_1B2B, -rho1BarAvg);
            
            hh->calculateIntegral(b12B,integralBF12B,eIntegralBF12B);
            integralsBF12B.push_back( integralBF12B );
            eIntegralsBF12B.push_back( eIntegralBF12B );
            hh->append(b12B);
            
            nx = b12B->GetNbinsX();
            ny = b12B->GetNbinsY();
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_B12B_x");
            h_x = b12B->ProjectionX();
            h_x->SetName(name);
            h_x->SetTitle(name);
            //h_x->Scale(TMath::TwoPi()/double(ny));
            hh->append(h_x);
            
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_B12B_y");
            h_y = b12B->ProjectionY();
            h_y->SetName(name);
            h_y->SetTitle(name);
            //h_y->Scale(4.0/double(nx));
            hh->append(h_y);
            
            // =====
            // B1B2
            // =====
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_B1B2");
            b1B2 = (TH2*) r2_1B2->Clone();
            b1B2->SetName(name);
            b1B2->SetTitle(name);
            b1B2->Scale(rho1BarAvg);
            b1B2->Add(r2_1B2B, -rho1Avg);
            //b1B2->Add(r2_1B2B, -rho1BarAvg);
            
            
            hh->calculateIntegral(b1B2,integralBF1B2,eIntegralBF1B2);
            integralsBF1B2.push_back( integralBF1B2 );
            eIntegralsBF1B2.push_back( eIntegralBF1B2 );
            hh->append(b1B2);
            
            nx = b1B2->GetNbinsX();
            ny = b1B2->GetNbinsY();
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_B1B2_x");
            h_x = b1B2->ProjectionX();
            h_x->SetName(name);
            h_x->SetTitle(name);
            //h_x->Scale(TMath::TwoPi()/ny);
            //calculateIntegral(h_x);
            hh->append(h_x);
            
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_B1B2_y");
            h_y = b1B2->ProjectionY();
            h_y->SetName(name);
            h_y->SetTitle(name);
            //h_y->Scale(4.0/nx);
            //calculateIntegral(h_y);
            hh->append(h_y);
            }
          
          if (calculateDiffs)
            {
            //b1B2->Add(r2_1B2B, -rho1Avg);
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_R2US_Diff");
            r2Diff = (TH2*) r2_1B2->Clone();
            r2Diff->SetName(name);
            r2Diff->SetTitle(name);
            r2Diff->Add(r2_12B, -1.0);
            hh->append(r2Diff);
            
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_R2LS_Diff");
            r2Diff = (TH2*) r2_12->Clone();
            r2Diff->SetName(name);
            r2Diff->SetTitle(name);
            r2Diff->Add(r2_1B2B, -1.0);
            hh->append(r2Diff);
            
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_B2Diff");
            b2Diff = (TH2*) b1B2->Clone();
            b2Diff->SetName(name);
            b2Diff->SetTitle(name);
            b2Diff->Add(b12B, -1.0);
            hh->append(b2Diff);
            }
          
          if (calculateBFv2)
            {
            // =====
            // bf_12B
            // =====
            
            double trigger2B = rho1_2B->Integral();
            if (trigger2B <= 0.0 ) trigger2B = 1.0E-10;
            
            name  = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_BFct_12B");
            bf_12B = (TH2*) r2_12B->Clone();
            bf_12B->SetName(name);
            bf_12B->SetTitle(name);
            bf_12B->Add(r2_1B2B, -1.0);
            //bf_12B->Scale(1.0/trigger2B);
            hh->calculateIntegral(bf_12B,integralBF12B,eIntegralBF12B);
            integralsBF12B.push_back( integralBF12B );
            eIntegralsBF12B.push_back( eIntegralBF12B );
            hh->append(bf_12B);
            
            nx = bf_12B->GetNbinsX();
            ny = bf_12B->GetNbinsY();
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_BFct_12B_x");
            h_x = bf_12B->ProjectionX();
            h_x->SetName(name);
            h_x->SetTitle(name);
            //h_x->Scale(TMath::TwoPi()/double(ny));
            hh->append(h_x);
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_BFct_12B_y");
            h_y = bf_12B->ProjectionY();
            h_y->SetName(name);
            h_y->SetTitle(name);
            //h_y->Scale(8.0/double(nx));
            hh->append(h_y);
            
            // bf_1B2
            
            double trigger2 = rho1_2->Integral();
            if (trigger2 <= 0.0 ) trigger2 = 1.0E-10;
            name  = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_BFct_1B2");
            bf_1B2 = (TH2*) r2_1B2->Clone();
            bf_1B2->SetName(name);
            bf_1B2->SetTitle(name);
            bf_1B2->Add(r2_12, -1.0);
            //bf_1B2->Scale(1.0/trigger2);
         
            hh->calculateIntegral(bf_1B2,integralBF12B,eIntegralBF12B);
            integralsBF12B.push_back( integralBF12B );
            eIntegralsBF12B.push_back( eIntegralBF12B );
            hh->append(bf_1B2);
            
            nx = bf_1B2->GetNbinsX();
            ny = bf_1B2->GetNbinsY();
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_BFct_1B2_x");
            h_x = bf_1B2->ProjectionX();
            h_x->SetName(name);
            h_x->SetTitle(name);
            //h_x->Scale(TMath::TwoPi()/double(ny));
            hh->append(h_x);
            
            name = createPairHistogramName(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "_BFct_1B2_y");
            h_y = bf_1B2->ProjectionY();
            h_y->SetName(name);
            h_y->SetTitle(name);
            //h_y->Scale(8.0/double(nx));
            hh->append(h_y);
            }
          }
        }
      }
    }
  
  outputFile->cd();
  hh->saveHistograms(outputFile);
  writeParameter(outputFile, "EventProcessed",nEventProcessed);
  writeParameter(outputFile, "EventAccepted", nEventAccepted);
  outputFile->Close();
  if ( reportEnd())
    ;
}
