//
//  HadronGasVsTempHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "HadronGasVsTempHistograms.hpp"

ClassImp(HadronGasVsTempHistograms);

HadronGasVsTempHistograms::HadronGasVsTempHistograms(Task * _parent,
                                                     const TString & _name,
                                                     Configuration & _config)
:
Histograms(_parent,_name,_config),
numberDensityVsT(nullptr),
energyDensityVsT(nullptr),
entropyDensityVsT(nullptr),
pressureVsT(nullptr),
partialNumberDensityVsT(),
partialEnergyDensityVsT(),
partialEntropyDensityVsT(),
partialPressureVsT(),
nStableDensityVsT()
{
  appendClassName("HadronGasVsTempHistograms");
}

void HadronGasVsTempHistograms::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  TString pn = getParentTask()->getName();
  TString bn = getParentTaskName();
  const   Configuration & config   = getConfiguration();
  int     nThermalSpecies       = config.getValueInt(pn,"nThermalSpecies");
  int     nStableSpecies        = config.getValueInt(pn,"nStableSpecies");
  bool    plotStableSpeciesVsT  = config.getValueBool(pn,"DoPlotVsStableSpecies");
  bool    plotThermalSpeciesVsT = config.getValueBool(pn,"DoPlotVsAllSpecies");
  int     nChemicalTemp         = config.getValueInt(pn,"nChemicalTemp");
  double  minChemicalTemp       = config.getValueDouble(pn,"MinChemicalTemp");
  double  maxChemicalTemp       = config.getValueDouble(pn,"MaxChemicalTemp");
  double  stepTemp;
  if (maxChemicalTemp == minChemicalTemp || nChemicalTemp==1)
    {
    nChemicalTemp = 1;
    stepTemp      = minChemicalTemp/10.0;
    }
  else
    stepTemp = (maxChemicalTemp - minChemicalTemp)/(nChemicalTemp-1);


  int     nMuB                  = config.getValueInt(pn,"nMuB");
  double  minMuB                = config.getValueDouble(pn,"MinMuB");
  double  maxMuB                = config.getValueDouble(pn,"MaxMuB");
  double  stepMuB;
  if (maxMuB == minMuB || nMuB==1)
    {
    nMuB    = 1;
    stepMuB = 0.0;
    }
  else
    stepMuB = (maxMuB - minMuB)/double(nMuB-1);

  int     nMuS                  = config.getValueInt(pn,"nMuS");
  double  minMuS                = config.getValueDouble(pn,"MinMuS");
  double  maxMuS                = config.getValueDouble(pn,"MaxMuS");
  double  stepMuS;
  if (maxMuS == minMuS || nMuS==1)
    {
    nMuS    = 1;
    stepMuS = 0.0;
    }
  else
    stepMuS = (maxMuS - minMuS)/double(nMuS-1);

  int    nT    = nChemicalTemp;
  double lowT  = minChemicalTemp - stepTemp/2.0;
  double highT = maxChemicalTemp + stepTemp/2.0;

  // need to check the units of the pressure...

//  if (minChemicalTemp == maxChemicalTemp) maxChemicalTemp = 1.10*minChemicalTemp;

  numberDensityVsT   = createHistogram(makeName(bn,"numberDensityVsT"),  nT, lowT, highT, "T (GeV)","n (fm^{-3})");
  energyDensityVsT   = createHistogram(makeName(bn,"energyDensityVsT"),  nT, lowT, highT, "T (GeV)","e (GeV.fm^{-3})");
  entropyDensityVsT  = createHistogram(makeName(bn,"entropyDensityVsT"), nT, lowT, highT, "T (GeV)","s (fm^{-3})");
  pressureVsT        = createHistogram(makeName(bn,"pressureVsT"),       nT, lowT, highT, "T (GeV)","p");

  if (plotStableSpeciesVsT && nStableSpecies>0)
    {
    nStableDensityVsT.push_back(createHistogram(makeName(bn,"nStableVsT"),  nT, lowT, highT, "T (GeV)","n (fm^{-3})"));
    }

  if (plotThermalSpeciesVsT && nThermalSpecies>0)
    {
    for (int iHadron=0; iHadron<nThermalSpecies; iHadron++)
      {
      TString bnSpecies = bn;
      bnSpecies += "_";
      bnSpecies += iHadron;
      partialNumberDensityVsT.push_back( createHistogram(makeName(bnSpecies,"nPartialVsT"),  nT, lowT, highT, "T (GeV)","n (fm^{-3})"));
      partialEnergyDensityVsT.push_back( createHistogram(makeName(bnSpecies,"ePartialVsT"),  nT, lowT, highT, "T (GeV)","e (GeV.fm^{-3})"));
      partialEntropyDensityVsT.push_back(createHistogram(makeName(bnSpecies,"sPartialVsT"),  nT, lowT, highT, "T (GeV)","s (fm^{-3})"));
      partialPressureVsT.push_back(      createHistogram(makeName(bnSpecies,"pPartialVsT"),  nT, lowT, highT, "T (GeV)","p (fm^{-2})"));
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


//________________________________________________________________________
void HadronGasVsTempHistograms::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  TString pn = getParentTask()->getName();
  TString bn = getParentTaskName();
  const   Configuration & config   = getConfiguration();
  int     nThermalSpecies          = config.getValueInt(pn,"nThermalSpecies");
  int     nStableSpecies           = config.getValueInt(pn,"nStableSpecies");
  bool    plotStableSpeciesVsT     = config.getValueBool(pn,"DoPlotVsStableSpecies");
  bool    plotThermalSpeciesVsT    = config.getValueBool(pn,"DoPlotVsAllSpecies");
  int     nChemicalTemp            = configuration.getValueInt(pn,"nChemicalTemp");
  double  minChemicalTemp          = configuration.getValueDouble(pn,"MinChemicalTemp");
  double  maxChemicalTemp          = configuration.getValueDouble(pn,"MaxChemicalTemp");
  int     nMuB                     = configuration.getValueInt(pn,"nMuB");
  double  minMuB                   = configuration.getValueDouble(pn,"MinMuB");
  double  maxMuB                   = configuration.getValueDouble(pn,"MaxMuB");
  double  stepMuB                  = (maxMuB - minMuB)/double(nMuB);
  double  minMuS                   = configuration.getValueDouble(pn,"MinMuS");
  double  maxMuS                   = configuration.getValueDouble(pn,"MaxMuS");

  numberDensityVsT   = loadH1(inputFile,makeName(bn,"numberyDensityVsT"));
  energyDensityVsT   = loadH1(inputFile,makeName(bn,"energyDensityVsT"));
  entropyDensityVsT  = loadH1(inputFile,makeName(bn,"entropyDensityVsT"));
  pressureVsT        = loadH1(inputFile,makeName(bn,"pressureVsT"));

  if (!numberDensityVsT || !energyDensityVsT ||  !entropyDensityVsT || !pressureVsT)
    {
    if (reportError(__FUNCTION__)) cout << "Could not load global hadron gas histogram. Abort." << endl;
    return;
    }

  if (plotStableSpeciesVsT && nStableSpecies>0)
    {
    nStableDensityVsT.push_back(loadH1(inputFile,makeName(bn,"nStableVsT")));
    }

  if (plotThermalSpeciesVsT && nThermalSpecies>0)
    {
    for (int iHadron=0; iHadron<nThermalSpecies; iHadron++)
      {
      TString bnSpecies = bn;
      bnSpecies += "_";
      bnSpecies += iHadron;
      partialNumberDensityVsT.push_back( loadH1(inputFile,makeName(bnSpecies,"nPartialVsT")));
      partialEnergyDensityVsT.push_back( loadH1(inputFile,makeName(bnSpecies,"ePartialVsT")));
      partialEntropyDensityVsT.push_back(loadH1(inputFile,makeName(bnSpecies,"sPartialVsT")));
      partialPressureVsT.push_back(      loadH1(inputFile,makeName(bnSpecies,"pPartialVsT")));
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void HadronGasVsTempHistograms::fill(HadronGas & hadronGas)
{
  if (reportStart(__FUNCTION__))
    ;
  TString pn = getParentTask()->getName();
  Configuration & config = getConfiguration();
  int  nThermalSpecies       = config.getValueInt(pn,"nThermalSpecies");
  int  nStableSpecies        = config.getValueInt(pn,"nStableSpecies");
  bool plotStableSpeciesVsT  = config.getValueBool(pn,"DoPlotVsStableSpecies");
  bool plotThermalSpeciesVsT = config.getValueBool(pn,"DoPlotVsAllSpecies");

  double zero = 0;
  double temperature    = hadronGas.getTemperature();
  double numberDensity  = hadronGas.getNumberDensity();
  double energyDensity  = hadronGas.getEnergyDensity();
  double entropyDensity = hadronGas.getEntropyDensity();
  double pressure       = hadronGas.getPressure();

  //
  // fill global HG variable for given temperature
  //
  int iT  = numberDensityVsT->GetXaxis()->FindBin(temperature);
  numberDensityVsT  ->SetBinContent(iT, numberDensity);  numberDensityVsT  ->SetBinError(iT,zero);
  energyDensityVsT  ->SetBinContent(iT, energyDensity);  energyDensityVsT  ->SetBinError(iT,zero);
  entropyDensityVsT ->SetBinContent(iT, entropyDensity); entropyDensityVsT ->SetBinError(iT,zero);
  pressureVsT       ->SetBinContent(iT, pressure);       pressureVsT       ->SetBinError(iT,zero);

  if (plotStableSpeciesVsT)
    {
    vector<double> & rho1_stable = hadronGas.rho1_stable;
    int n = rho1_stable.size();
    if (n!=nStableSpecies)
      {
      if (reportError(__FUNCTION__)) cout << "Incompatible number of stable species (booked and actual)" << endl;
      return;
      }
    else
      {
      //
      // fill stable particle variables for given temperature
      //
      for (int iHadron=0; iHadron<nStableSpecies; iHadron++)
        {
        nStableDensityVsT[iHadron]->SetBinContent(iT, rho1_stable[iHadron] );
        nStableDensityVsT[iHadron]->SetBinError(iT,zero);
        }
      }
    }

  if (plotThermalSpeciesVsT)
    {
    vector<HadronGasParticle*> &  hadrons = hadronGas.getAllHadrons();
    //
    // fill thermal particle variables for given temperature
    //
    for (unsigned int iHadron=0; iHadron<hadrons.size(); iHadron++)
      {
      HadronGasParticle & hadron = *(hadrons[iHadron]); //  getHadronGasParticleAt();
      partialNumberDensityVsT[iHadron] ->SetBinContent(iT, hadron.getNumberDensity());  partialNumberDensityVsT[iHadron] ->SetBinError(iT,zero);
      partialEnergyDensityVsT[iHadron] ->SetBinContent(iT, hadron.getEnergyDensity());  partialEnergyDensityVsT[iHadron] ->SetBinError(iT,zero);
      partialEntropyDensityVsT[iHadron]->SetBinContent(iT, hadron.getEntropyDensity()); partialEntropyDensityVsT[iHadron]->SetBinError(iT,zero);
      partialPressureVsT[iHadron]      ->SetBinContent(iT, hadron.getPressure());       partialPressureVsT[iHadron]      ->SetBinError(iT,zero);
      }
    }
  if (reportStart(__FUNCTION__))
    ;
}

