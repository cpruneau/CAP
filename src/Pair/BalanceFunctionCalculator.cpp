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
  configuration.setParameter("useParticles",           false);
  configuration.setParameter("createHistograms",       true);
  configuration.setParameter("loadHistograms",         true);
  configuration.setParameter("saveHistograms",         true);
  configuration.setParameter("forceHistogramsRewrite", true);
  configuration.setParameter("appendedString",         TString("BalFct"));
  configuration.setParameter("forceHistogramsRewrite", true);
  configuration.setParameter("calculateCI",            true);
  configuration.setParameter("calculateCD",            true);
  configuration.setParameter("calculateBFv1",          true);
  configuration.setParameter("calculateDiffs",         true);
  configuration.setParameter("calculateBFv2",          true);
  configuration.addParameter("fillEta",                true);
  configuration.addParameter("fillY",                  false);
  configuration.addParameter("fillP2",                 false);
  
  configuration.addParameter("nBins_n2",  100);
  configuration.addParameter("min_n2",    0.0);
  configuration.addParameter("max_n2",  100.0);
  
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
  // if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}


void BalanceFunctionCalculator::calculate_CI(const TString & histoBaseName,
                                             const TString & eventClassName,
                                             const TString & particleName1,
                                             const TString & particleName2,
                                             const TString & obsName,
                                             TH2* obs_1_2, TH2* obs_1Bar_2, TH2* obs_1_2Bar, TH2* obs_1Bar_2Bar)
{
  TString name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName, "CI" );
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_1Bar_2->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_1_2Bar);
  obs->Add(obs_1_2);
  obs->Add(obs_1Bar_2Bar);
  obs->Scale(0.25);
  histograms[0]->append(obs);

  name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,"CI_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,"CI_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);
}

void BalanceFunctionCalculator::calculate_CD(const TString & histoBaseName,
                                             const TString & eventClassName,
                                             const TString & particleName1,
                                             const TString & particleName2,
                                             const TString & obsName,
                                             TH2* obs_1_2,
                                             TH2* obs_1Bar_2,
                                             TH2* obs_1_2Bar,
                                             TH2* obs_1Bar_2Bar)
{
  TString name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName, "CD" );
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_1Bar_2->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_1_2Bar);
  obs->Add(obs_1_2, -1.0);
  obs->Add(obs_1Bar_2Bar, -1.0);
  obs->Scale(0.5);
  histograms[0]->append(obs);

  name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,"CD_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,"CD_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);
}

void BalanceFunctionCalculator::calculate_BalFct(const TString & histoBaseName,
                                                 const TString & eventClassName,
                                                 const TString & particleName1,
                                                 const TString & particleName2,
                                                 const TString & obsName,
                                                 const TString & comboName,
                                                 TH1* rho1_2 __attribute__((unused)),
                                                 TH2* obs_US,
                                                 TH2* obs_LS)
{
  TString name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_US->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_LS, -1.0);
  double rho1Integral = rho1_2->Integral();
  obs->Scale(1.0/rho1Integral);
  histograms[0]->append(obs);

  name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);

//  histograms[0]->calculateIntegral(rho1_2Bar,xMin,xMax,triggerInt,triggerIntError,0);
//  hh->calculateIntegral(bf_12B,xMin,xMax,yMin,yMax,integralBF,eIntegralBF,0);
//  cout << "                 rho1_2Bar integral:" << triggerInt <<  " +- " << triggerIntError << endl;
//  cout << "                  bf_12B integral:" << integralBF <<  " +- " << eIntegralBF <<  endl;
//  cout << "bf_12B integral/ rho1_2Bar integral:" << integralBF / triggerInt << endl;
//  cout << "bf_12B integral* rho1_2Bar integral:" << integralBF * triggerInt << endl;
//  integralsBF12B.push_back( integralBF );
//  eIntegralsBF12B.push_back( eIntegralBF );


}

void BalanceFunctionCalculator::calculate_Diff(const TString & histoBaseName,
                                                 const TString & eventClassName,
                                                 const TString & particleName1,
                                                 const TString & particleName2,
                                                 const TString & obsName,
                                                 const TString & comboName,
                                                 TH2* obs_first,
                                                 TH2* obs_second)
{
  TString name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_first->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_second, -1.0);
  histograms[0]->append(obs);

  name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(histoBaseName,eventClassName,particleName1,particleName2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);
}


//!
//! Create Balance Function Histograms for the input file, event classes, observables and particles...
//!
void BalanceFunctionCalculator::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  //incrementTaskExecuted();
  TString histoInputPath      = configuration.getValueString("histoInputPath");
  TString histoInputFileName  = configuration.getValueString("histoInputFileName");
  TString histoOutputPath     = configuration.getValueString("histoOutputPath");
  TString histoOutputFileName = configuration.getValueString("histoOutputFileName");
  TString histoModelDataName  = configuration.getValueString("histoModelDataName");
  TString histoAnalyzerName   = configuration.getValueString("histoAnalyzerName");
  TString appendedString      = configuration.getValueString("appendedString");
  TString histoBaseName       = histoAnalyzerName;
  bool forceHistogramsRewrite = configuration.getValueBool("forceHistogramsRewrite" );
  bool calculateCI            = configuration.getValueBool("calculateCI" );
  bool calculateCD            = configuration.getValueBool("calculateCD" );
  bool calculateBFv1          = configuration.getValueBool("calculateBFv1" );
  bool calculateDiffs         = configuration.getValueBool("calculateDiffs" );
  bool calculateBFv2          = configuration.getValueBool("calculateBFv2" );
  vector<TString> allFilesToAnalyze;



  if (!histoInputFileName.IsNull())
    {
    allFilesToAnalyze.push_back(histoInputPath+histoInputFileName);
    }
  else
    {
//    histoInputFileName  = histoModelDataName;
//    histoInputFileName  += "_";
//    histoInputFileName  += histoAnalyzerName;
    vector<TString> includePatterns = configuration.getSelectedValues("IncludedPattern", "none");
    vector<TString> excludePatterns = configuration.getSelectedValues("ExcludedPattern", "none");
    if (reportDebug(__FUNCTION__))
      {
      for (unsigned int k=0;k<includePatterns.size();k++) cout << " k:" << k << "  Include: " << includePatterns[k] << endl;
      for (unsigned int k=0;k<excludePatterns.size();k++) cout << " k:" << k << "  Exclude: " << excludePatterns[k] << endl;
      }
      allFilesToAnalyze = listFilesInDir(histoInputPath,includePatterns,excludePatterns);
    }

  int nFilesToAnalyze = allFilesToAnalyze.size();
  if (nFilesToAnalyze<1)
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;
      cout << "========================================================================"  << endl;
      cout << "========================================================================"  << endl;
      cout << "  Attempting to execute BalFct analysis with no selected files." << endl;
      cout << "                         Check your code!!!!!!! " <<  endl;
      cout << "========================================================================"  << endl;
      cout << "========================================================================"  << endl;
      }
    return;
    }
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << " ===========================================================" << endl;
    cout << " ===========================================================" << endl;
    cout << "         histoInputPath: " << histoInputPath << endl;
    cout << "     histoInputFileName: " << histoInputFileName<< endl;
    cout << "     n files to analyze: " << nFilesToAnalyze << endl;
    cout << "        histoOutputPath: " << histoOutputPath << endl;
    cout << "    histoOutputFileName: " << histoOutputFileName << endl;
    cout << "          histoBaseName: " << histoBaseName << endl;
    cout << "         appendedString: " << appendedString << endl;
    cout << "              pairLabel: " << histoAnalyzerName << endl;
    cout << "            calculateCI: " << calculateCI << endl;
    cout << "            calculateCD: " << calculateCD << endl;
    cout << "          calculateBFv1: " << calculateBFv1 << endl;
    cout << "         calculateDiffs: " << calculateDiffs << endl;
    cout << "          calculateBFv2: " << calculateBFv2 << endl;
    cout << " ===========================================================" << endl;
    cout << " ===========================================================" << endl;
    }
  postTaskOk();



  for (int iFile =0; iFile<nFilesToAnalyze; iFile++)
    {
    histoInputFileName = allFilesToAnalyze[iFile];
    histoOutputFileName = removeRootExtension(histoInputFileName);
    histoOutputFileName += appendedString;
    if (reportInfo(__FUNCTION__)) cout << " File:" << iFile << " named: " << histoInputFileName << endl;
    TFile * inputFile  = nullptr;
    TFile * outputFile = nullptr;
    inputFile = openRootFile("",histoInputFileName,"OLD");
    if (!inputFile) return;
    if (forceHistogramsRewrite)
      outputFile = openRootFile("",histoOutputFileName, "RECREATE");
    else
      outputFile = openRootFile("",histoOutputFileName, "NEW");
    if (!outputFile) return;


    // Use hh as helper to load and calculate histograms, etc.
    Histograms * hh = new Histograms(getName(), getConfiguration(), getLogLevel());
    hh->setOwnership(false);
    histograms.push_back(hh); // enables handling in functions.

    loadNEexecutedTask(inputFile);
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
            TString eventClassName   = eventFilters[iEventClass]->getName();
            TString particleName1    = particleFilters[iPart1]->getName();
            TString particleName1Bar = particleFilters[iPart1+nSpecies]->getName();
            TString particleName2    = particleFilters[iPart2]->getName();
            TString particleName2Bar   = particleFilters[iPart2+nSpecies]->getName();
            TH1 * rho1_1    = hh->loadH1(inputFile,makeHistoName(histoBaseName,eventClassName,particleName1,   sObservableNames[0]));
            TH1 * rho1_1Bar = hh->loadH1(inputFile,makeHistoName(histoBaseName,eventClassName,particleName1Bar,sObservableNames[0]));
            TH1 * rho1_2    = hh->loadH1(inputFile,makeHistoName(histoBaseName,eventClassName,particleName2,   sObservableNames[0]));
            TH1 * rho1_2Bar = hh->loadH1(inputFile,makeHistoName(histoBaseName,eventClassName,particleName2Bar,sObservableNames[0]));
            if (!rho1_1 || !rho1_1Bar || !rho1_2 || !rho1_2Bar) return;
            TH2 * obs_1_2       = hh->loadH2(inputFile,makeHistoName(histoBaseName,eventClassName,particleName1,    particleName2,    pObservableNames[iObservable]));
            TH2 * obs_1Bar_2    = hh->loadH2(inputFile,makeHistoName(histoBaseName,eventClassName,particleName1Bar, particleName2,    pObservableNames[iObservable]));
            TH2 * obs_1_2Bar    = hh->loadH2(inputFile,makeHistoName(histoBaseName,eventClassName,particleName1,    particleName2Bar, pObservableNames[iObservable]));
            TH2 * obs_1Bar_2Bar = hh->loadH2(inputFile,makeHistoName(histoBaseName,eventClassName,particleName1Bar, particleName2Bar, pObservableNames[iObservable]));
            if (!obs_1_2 || !obs_1Bar_2 || !obs_1_2Bar || !obs_1Bar_2Bar) return;

            if (calculateCI)
              calculate_CI(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable],obs_1_2,obs_1Bar_2,obs_1_2Bar,obs_1Bar_2Bar);

            if (calculateCD)
              calculate_CD(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable],obs_1_2,obs_1Bar_2,obs_1_2Bar,obs_1Bar_2Bar);

            if (calculateBFv1)
              {
              calculate_BalFct(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "B2_1_2Bar",rho1_2Bar, obs_1_2Bar, obs_1Bar_2Bar);
              calculate_BalFct(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "B2_1Bar_2",rho1_2,    obs_1Bar_2, obs_1_2);
              }

            if (calculateDiffs)
              {
              calculate_Diff(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "Diff_US",   obs_1Bar_2,    obs_1_2Bar);
              calculate_Diff(histoBaseName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "Diff_LS",   obs_1Bar_2Bar, obs_1_2);
              }
            }
          }
        }
      }
    outputFile->cd();
    hh->saveHistograms(outputFile);
    writeNEexecutedTask(outputFile);
    outputFile->Close();
    inputFile->Close();
    hh->clear();
    delete hh;
    histograms.clear();
    }
  if (reportEnd(__FUNCTION__))
    ;
}


//vector<double> integralsBF12B;
//vector<double> integralsBF1B2;
//vector<double> eIntegralsBF12B; // estimated errors
//vector<double> eIntegralsBF1B2; // estimated errors



//double xMin = -1.0;
//double xMax =  1.0;
//double yMin = -1.0;
//double yMax =  1.0;
//double triggerInt;
//double triggerIntError;
//int xFirst;
//int xLast;
//double integralBF;
//double eIntegralBF;
