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

BalanceFunctionCalculator::BalanceFunctionCalculator(const TString & _name,
                                                     Configuration & _configuration,
                                                     vector<EventFilter*> & _eventFilters,
                                                     vector<ParticleFilter*> & _particleFilters)
:
Task(_name,_configuration,_eventFilters, _particleFilters),
sObservableNames(),
pObservableNames()
{
  appendClassName("BalanceFunctionCalculator");
}

void BalanceFunctionCalculator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  setParameter("UseParticles",           false);
  setParameter("CreateHistograms",       true);
  setParameter("LoadHistograms",         true);
  setParameter("SaveHistograms",         true);
  setParameter("ForceHistogramsRewrite", true);
  setParameter("AppendedString",         TString("BalFct"));
  setParameter("calculateCI",            true);
  setParameter("calculateCD",            true);
  setParameter("calculateBFv1",          true);
  setParameter("calculateDiffs",         true);
  setParameter("calculateBFv2",          true);
  addParameter("FillEta",                true);
  addParameter("FillY",                  false);
  addParameter("FillP2",                 false);
  addParameter("nBins_n2",     100);
  addParameter("Min_n2",       0.0);
  addParameter("Max_n2",       100.0);
  addParameter("nBins_eTot",   100);
  addParameter("Min_eTot",     0.0);
  addParameter("Max_eTot",     100.0);
  addParameter("nBins_pt",     18);
  addParameter("Min_pt",       0.20);
  addParameter("Max_pt",       2.00);
  addParameter("nBins_phi",    72);
  addParameter("Min_phi",      0.0);
  addParameter("Max_phi",      TMath::TwoPi());
  addParameter("nBins_eta",    20);
  addParameter("Min_eta",      -1.0);
  addParameter("Max_eta",      1.0);
  addParameter("nBins_y",      20);
  addParameter("Min_y",        -1.0);
  addParameter("Max_y",        1.0);
  addParameter("nBins_phiEta",      720);
  addParameter("nBins_phiEtaPt",    7200);
  addParameter("nBins_phiY",        720);
  addParameter("nBins_phiYPt",      7200);
  addParameter("nBins_n2",          100);
  addParameter("Min_n2",            0.0);
  addParameter("Max_n2",            1000.0);
  addParameter("nBins_DeltaPlong",  10);
  addParameter("Min_DeltaPlong",    -1.0);
  addParameter("Max_DeltaPlong",    1.0);
  addParameter("nBins_DeltaPside",  10);
  addParameter("Min_DeltaPside",    -1.0);
  addParameter("Max_DeltaPside",    1.0);
  addParameter("range_DeltaPside",  2.0);
  addParameter("nBins_DeltaPout",   10);
  addParameter("Min_DeltaPout",     -1.0);
  addParameter("Max_DeltaPout",     1.0);
  addParameter("range_DeltaPout",   2.0);
  addParameter("nBins_Dphi",        36);
  addParameter("Min_Dphi",          0.0);
  addParameter("Max_Dphi",          TMath::TwoPi());
  addParameter("Width_Dphi",        TMath::TwoPi());
  addParameter("nBins_Dphi_shft",   36);
  addParameter("Min_Dphi_shft",     0.0);
  addParameter("Max_Dphi_shft",     0.0);
  addParameter("nBins_Deta",        39);
  addParameter("Min_Deta",          -2.0);
  addParameter("Max_Deta",          2.0);
  addParameter("Width_Deta",        4.0/39.0);
  addParameter("nBins_Dy",          39);
  addParameter("Min_Dy",            -2.0);
  addParameter("Max_Dy",            2.0);
  addParameter("Width_Dy",          4.0/39.0);
}


TH2* BalanceFunctionCalculator::calculate_CI(const TString & taskName,
                                             const TString & eventClassName,
                                             const TString & particleName1,
                                             const TString & particleName2,
                                             const TString & obsName,
                                             TH2* obs_1_2, TH2* obs_1Bar_2, TH2* obs_1_2Bar, TH2* obs_1Bar_2Bar)
{
  TString name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName, "CI" );
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

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,"CI_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,"CI_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);

  return obs;
}

TH2* BalanceFunctionCalculator::calculate_CD(const TString & taskName,
                                             const TString & eventClassName,
                                             const TString & particleName1,
                                             const TString & particleName2,
                                             const TString & obsName,
                                             TH2* obs_1_2,
                                             TH2* obs_1Bar_2,
                                             TH2* obs_1_2Bar,
                                             TH2* obs_1Bar_2Bar)
{
  TString name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName, "CD" );
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

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,"CD_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,"CD_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);

  return obs;
}

TH2* BalanceFunctionCalculator::calculate_BalFct(const TString & taskName,
                                                 const TString & eventClassName,
                                                 const TString & particleName1,
                                                 const TString & particleName2,
                                                 const TString & obsName,
                                                 const TString & comboName,
                                                 TH1* rho1_2,
                                                 TH2* obs_US,
                                                 TH2* obs_LS)
{
  TString name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_US->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_LS, -1.0);

  double wx = obs->GetXaxis()->GetBinWidth(1);
  //double wy = obs->GetYaxis()->GetBinWidth(1);
  obs->Scale(1.0/wx); // make this a function of delta y
  //double rho1Integral = rho1_2->Integral();
  //obs->Scale(1.0/rho1Integral);
  histograms[0]->append(obs);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);

  return obs;
}

TH2* BalanceFunctionCalculator::calculate_BalFctSum(const TString & taskName,
                                                    const TString & eventClassName,
                                                    const TString & particleName1,
                                                    const TString & particleName2,
                                                    const TString & obsName,
                                                    const TString & comboName,
                                                    TH2* obs_12Bar,
                                                    TH2* obs_1Bar2)
{
  TString name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_12Bar->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_1Bar2, 1.0);

  //double rho1Integral = rho1_2->Integral();
  //obs->Scale(0.5/wx);
  obs->Scale(0.5);
  histograms[0]->append(obs);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);

  return obs;
}

TH2* BalanceFunctionCalculator::calculate_BalFct2(const TString & taskName,
                                                  const TString & eventClassName,
                                                  const TString & particleName1,
                                                  const TString & particleName2,
                                                  const TString & obsName,
                                                  const TString & comboName,
                                                  TH1* rho1_1,
                                                  TH1* rho1_2,
                                                  TH2* obs_US,
                                                  TH2* obs_LS)
{
  TString name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  double rho1_1_Integral = rho1_1->Integral();
  //double rho1_2_Integral = rho1_2->Integral();

//  double low  = rho1_1->GetXaxis()->GetXmin();
//  double high = rho1_1->GetXaxis()->GetXmax();
//  double yieldA = yieldA/(high-low)/TMath::TwoPi();
//  double yieldB = yieldB/(high-low)/TMath::TwoPi();


  obs = (TH2*) obs_US->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_LS, -1.0);
  obs->Scale(rho1_1_Integral);
  histograms[0]->append(obs);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);

  return obs;
}

TH2* BalanceFunctionCalculator::calculate_BalFct3(const TString & taskName,
                                                  const TString & eventClassName,
                                                  const TString & particleName1,
                                                  const TString & particleName2,
                                                  const TString & obsName,
                                                  const TString & comboName,
                                                  TH1* rho1_2 __attribute__((unused)),
                                                  TH2* obs_US,
                                                  TH2* obs_LS)
{
  TString name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_US->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_LS, -1.0);
//  double rho1Integral = rho1_2->Integral("Width");
//  obs->Scale(rho1Integral);
  histograms[0]->append(obs);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);

  return obs;
}



TH2* BalanceFunctionCalculator::calculate_Diff(const TString & taskName,
                                                 const TString & eventClassName,
                                                 const TString & particleName1,
                                                 const TString & particleName2,
                                                 const TString & obsName,
                                                 const TString & comboName,
                                                 TH2* obs_first,
                                                 TH2* obs_second)
{
  TString name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName);
  TH2 * obs;
  TH1 * obs_x;
  TH1 * obs_y;
  obs = (TH2*) obs_first->Clone();
  obs->SetName(name);
  obs->SetTitle(name);
  obs->Add(obs_second, -1.0);
  histograms[0]->append(obs);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_x");
  obs_x = obs->ProjectionX();
  obs_x->SetName(name);
  obs_x->SetTitle(name);
  histograms[0]->append(obs_x);

  name = makeHistoName(taskName,eventClassName,particleName1,particleName2,obsName,comboName+"_y");
  obs_y = obs->ProjectionY();
  obs_y->SetName(name);
  obs_y->SetTitle(name);
  histograms[0]->append(obs_y);

  return obs;
}


//!
//! Create Balance Function Histograms for the input file, event classes, observables and particles...
//!
void BalanceFunctionCalculator::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  TString histogramInputPath  = getValueString("HistogramInputPath");
  TString histogramInputFile  = getValueString("HistogramInputFile");
  TString histogramOutputPath = getValueString("HistogramOutputPath");
  TString histogramOutputFile = getValueString("HistogramOutputFile");
  TString appendedString      = getValueString("AppendedString");
  bool forceHistogramsRewrite = getValueBool("ForceHistogramsRewrite" );
  bool calculateCI            = getValueBool("calculateCI" );
  bool calculateCD            = getValueBool("calculateCD" );
  bool calculateBFv1          = getValueBool("calculateBFv1" );
  bool calculateDiffs         = getValueBool("calculateDiffs" );
  bool calculateBFv2          = getValueBool("calculateBFv2" );
  vector<TString> allFilesToAnalyze;

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " ===========================================================" << endl;
    cout << " Task name.....................: " << taskName << endl;
    cout << " HistogramInputPath............: " << histogramInputPath << endl;
    cout << " HistogramInputFile............: " << histogramInputFile << endl;
    cout << " HistogramOutputPath...........: " << histogramOutputPath << endl;
    cout << " HistogramOutputFile...........: " << histogramOutputFile << endl;
    cout << " ===========================================================" << endl;
    }

  if (histogramInputFile.Contains("none") || histogramInputFile.Contains("null") || histogramInputFile.IsNull() )
    {
    vector<TString> includePatterns = getSelectedValues("IncludedPattern", "none");
    vector<TString> excludePatterns = getSelectedValues("ExcludedPattern", "none");
    for (unsigned int k=0;k<includePatterns.size();k++) cout << " k:" << k << "  Include: " << includePatterns[k] << endl;
    for (unsigned int k=0;k<excludePatterns.size();k++) cout << " k:" << k << "  Exclude: " << excludePatterns[k] << endl;
    cout << " ===========================================================" << endl;
    bool prependPath = true;
    bool verbose     = true;
    int  maximumDepth = 2;
    allFilesToAnalyze = listFilesInDir(histogramInputPath,includePatterns,excludePatterns,prependPath, verbose, maximumDepth);
    }
  else
    {
    allFilesToAnalyze.push_back(histogramInputPath+histogramInputFile);
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
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " ===========================================================" << endl;
    cout << " Task name.....................: " << taskName << endl;
    cout << " HistogramInputPath............: " << histogramInputPath << endl;
    cout << " HistogramInputFile............: " << histogramInputFile << endl;
    cout << " HistogramOutputPath...........: " << histogramOutputPath << endl;
    cout << " HistogramOutputFile...........: " << histogramOutputFile << endl;
    cout << " n files to analyze............: " << nFilesToAnalyze     << endl;
    cout << " appendedString................: " << appendedString      << endl;
    cout << " calculateCI...................: " << calculateCI         << endl;
    cout << " calculateCD...................: " << calculateCD         << endl;
    cout << " calculateBFv1.................: " << calculateBFv1       << endl;
    cout << " calculateDiffs................: " << calculateDiffs      << endl;
    cout << " calculateBFv2.................: " << calculateBFv2       << endl;
    cout << " ===========================================================" << endl;
    }
  postTaskOk();



  for (int iFile =0; iFile<nFilesToAnalyze; iFile++)
    {
    histogramInputFile  = allFilesToAnalyze[iFile];
    histogramOutputFile = removeRootExtension(histogramInputFile);
    histogramOutputFile.ReplaceAll(TString("Derived"),appendedString);
    TFile * inputFile  = nullptr;
    TFile * outputFile = nullptr;
    inputFile = openRootFile("",histogramInputFile,"OLD");
    if (!inputFile) return;
    if (forceHistogramsRewrite)
      outputFile = openRootFile("",histogramOutputFile, "RECREATE");
    else
      outputFile = openRootFile("",histogramOutputFile, "NEW");
    if (!outputFile) return;
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << " CalculateBF...................: "  << calculateBFv2       << endl;
      cout << " From..........................: "  << histogramInputFile << endl;
      cout << " Saved to:.....................: "  << histogramOutputFile << endl;
      }

    // Use hh as helper to load and calculate histograms, etc.
    Histograms * hh = new Histograms(this,taskName, getConfiguration());
    hh->setOwnership(false);
    histograms.push_back(hh); // enables handling in functions.
    loadNEexecutedTask(inputFile);
    loadNEventsAccepted(inputFile);

    unsigned int nSpecies = particleFilters.size()/2;
    vector<TString>  sObservableNames;
    vector<TString>  pObservableNames;
    int observableSelection = 5;
    switch (observableSelection)
      {
        default:
        case 0: // eta based observables, full complement
        sObservableNames.push_back("n1_eta");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("R2_ptpt");
        pObservableNames.push_back("R2_phiPhi");
        pObservableNames.push_back("R2_etaEta");
        pObservableNames.push_back("R2_DetaDphi_shft");
        break;

        case 1: // eta based observables, only DeltaEta vs DeltaPhi
        sObservableNames.push_back("n1_eta");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("rho2_DetaDphi_shft");
        break;

        case 2: // y based observables, full complement
        sObservableNames.push_back("n1_y");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("R2_ptpt");
        pObservableNames.push_back("R2_phiPhi");
        pObservableNames.push_back("R2_yY");
        pObservableNames.push_back("R2_DyDphi_shft");
        break;

        case 3: // y based observables, only DeltaY vs DeltaPhi
        sObservableNames.push_back("n1_y");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("R2_DyDphi_shft");
        break;

        case 4: // eta based observables, only DeltaEta vs DeltaPhi
        sObservableNames.push_back("n1_eta");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("rho2_DetaDphi_shft");
        pObservableNames.push_back("R2_DetaDphi_shft");
        //pObservableNames.push_back("B2AB_DetaDphi_shft");
        //pObservableNames.push_back("B2BA_DetaDphi_shft");
        //        pObservableNames.push_back("n2_phiPhi");
        break;

        case 5: // y based observables, only DeltaEta vs DeltaPhi
        sObservableNames.push_back("n1_y");
        sObservableNames.push_back("n1_phi");
        pObservableNames.push_back("A2_DyDphi_shft");
        pObservableNames.push_back("B2_DyDphi_shft");
        pObservableNames.push_back("C2_DyDphi_shft");
        pObservableNames.push_back("R2_DyDphi_shft");
        pObservableNames.push_back("B2_yY");
        pObservableNames.push_back("B2_phiPhi");
        break;
      }


    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << " nSpecies.........................: "  << nSpecies  << endl;
      cout << " sObservableNames.size()..........: "  << sObservableNames.size()  << endl;
      for (int k=0; k<sObservableNames.size(); k++)
        cout << "   " << k << "    " << sObservableNames[k] << endl;
      cout << " pObservableNames.size()..........: "  << pObservableNames.size()  << endl;
      for (int k=0; k<pObservableNames.size(); k++)
        cout << "   " << k << "    " << pObservableNames[k] << endl;
      }

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
            TH1 * rho1_1    = hh->loadH1(inputFile,makeHistoName(taskName,eventClassName,particleName1,   sObservableNames[0]));
            TH1 * rho1_1Bar = hh->loadH1(inputFile,makeHistoName(taskName,eventClassName,particleName1Bar,sObservableNames[0]));
            TH1 * rho1_2    = hh->loadH1(inputFile,makeHistoName(taskName,eventClassName,particleName2,   sObservableNames[0]));
            TH1 * rho1_2Bar = hh->loadH1(inputFile,makeHistoName(taskName,eventClassName,particleName2Bar,sObservableNames[0]));
            if (!rho1_1 || !rho1_1Bar || !rho1_2 || !rho1_2Bar) return;
            TH2 * obs_1_2       = hh->loadH2(inputFile,makeHistoName(taskName,eventClassName,particleName1,    particleName2,    pObservableNames[iObservable]));
            TH2 * obs_1Bar_2    = hh->loadH2(inputFile,makeHistoName(taskName,eventClassName,particleName1Bar, particleName2,    pObservableNames[iObservable]));
            TH2 * obs_1_2Bar    = hh->loadH2(inputFile,makeHistoName(taskName,eventClassName,particleName1,    particleName2Bar, pObservableNames[iObservable]));
            TH2 * obs_1Bar_2Bar = hh->loadH2(inputFile,makeHistoName(taskName,eventClassName,particleName1Bar, particleName2Bar, pObservableNames[iObservable]));
            if (!obs_1_2 || !obs_1Bar_2 || !obs_1_2Bar || !obs_1Bar_2Bar) return;

            if (calculateCI)
              calculate_CI(taskName,eventClassName,particleName1,particleName2, pObservableNames[iObservable],obs_1_2,obs_1Bar_2,obs_1_2Bar,obs_1Bar_2Bar);

            if (calculateCD)
              calculate_CD(taskName,eventClassName,particleName1,particleName2, pObservableNames[iObservable],obs_1_2,obs_1Bar_2,obs_1_2Bar,obs_1Bar_2Bar);

            if (calculateBFv1)
              {
              TH2* bfa = calculate_BalFct(taskName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "B2_1_2Bar",rho1_2Bar, obs_1_2Bar, obs_1Bar_2Bar);
              TH2* bfb = calculate_BalFct(taskName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "B2_1Bar_2",rho1_2,    obs_1Bar_2, obs_1_2);
              calculate_BalFctSum(taskName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "B2_12Sum",bfa,bfb);
              }
            if (calculateDiffs)
              {
              calculate_Diff(taskName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "Diff_US",   obs_1Bar_2,    obs_1_2Bar);
              calculate_Diff(taskName,eventClassName,particleName1,particleName2, pObservableNames[iObservable], "Diff_LS",   obs_1Bar_2Bar, obs_1_2);
              }
            }
          }
        }
      }
    outputFile->cd();
    hh->saveHistograms(outputFile);
    writeNEventsAccepted(outputFile);
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
