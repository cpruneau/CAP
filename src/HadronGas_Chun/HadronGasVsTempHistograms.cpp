//
//  HadronGasVsTempHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "HadronGasVsTempHistograms.hpp"
using CAP::HadronGasVsTempHistograms;

ClassImp(HadronGasVsTempHistograms);

HadronGasVsTempHistograms::HadronGasVsTempHistograms(Task * _parent,
                                                     const String & _name,
                                                     Configuration & _config,
                                                     HadronGas * _hadronGas)
:
HistogramGroup(_parent,_name,_config),
numberDensityVsT(nullptr),
energyDensityVsT(nullptr),
entropyDensityVsT(nullptr),
pressureVsT(nullptr),
nDensityVsT(),
eDensityVsT(),
sDensityVsT()
{
  // no ops
}

void HadronGasVsTempHistograms::createHistograms()
{
  Configuration & config = getConfiguration();
  String bn = getParentTaskName();
  String pn = getParentName();
  int nT      = config.getValueInt(pn,"nChemicalTemp");
  double minT = config.getValueDouble(pn,"MinChemicalTemp");
  double maxT = config.getValueDouble(pn,"MaxChemicalTemp");

  numberDensityVsT   = createHistogram(createName(bn,"numberyDensityVsT"), nT, minT, maxT, "T (GeV)","n (fm^{-3})");
  energyDensityVsT   = createHistogram(createName(bn,"energyDensityVsT"),  nT, minT, maxT, "T (GeV)","e (GeV.fm^{-3})");
  entropyDensityVsT  = createHistogram(createName(bn,"entropyDensityVsT"), nT, minT, maxT, "T (GeV)","s (fm^{-3})");
  pressureVsT        = createHistogram(createName(bn,"pressureVsT"),       nT, minT, maxT, "T (GeV)","p");

  // FIX ME!!!!!!!!!!!!!!! 
  unsigned int nStableSpecies = 1111; // nStableTypes;
  nDensityVsT.clear();
  eDensityVsT.clear();
  sDensityVsT.clear();
  for (unsigned int iSpecies=0; iSpecies<nStableSpecies; iSpecies++)
  {
  String bnSpecies = bn;
  bnSpecies += "_";
  bnSpecies += iSpecies;
  nDensityVsT.push_back(createHistogram(createName(bnSpecies,"nVsT"),  nT, minT, maxT, "T (GeV)","n (fm^{-3})"));
  eDensityVsT.push_back(createHistogram(createName(bnSpecies,"eVsT"),  nT, minT, maxT, "T (GeV)","e (GeV.fm^{-3})"));
  sDensityVsT.push_back(createHistogram(createName(bnSpecies,"sVsT"),  nT, minT, maxT, "T (GeV)","s (fm^{-3})"));
  }
}


//________________________________________________________________________
void HadronGasVsTempHistograms::loadHistograms(TFile * inputFile)
{
  if (!ptrFileExist(__FUNCTION__, inputFile)) return;
  String bn  = getParentTaskName();
  numberDensityVsT   = loadH1(inputFile,createName(bn,"numberyDensityVsT"));
  energyDensityVsT   = loadH1(inputFile,createName(bn,"energyDensityVsT"));
  entropyDensityVsT  = loadH1(inputFile,createName(bn,"entropyDensityVsT"));
  pressureVsT        = loadH1(inputFile,createName(bn,"pressureVsT"));
  if (!numberDensityVsT)
    {
    if (reportError(__FUNCTION__)) cout << "Could not load histogram: " << createName(bn,"numberDensityVsT") << endl;
    return;
    }
}

void HadronGasVsTempHistograms::fill(HadronGas & hadronGas)
{
  //cout << " HadronGasVsTempHistograms::fill(HadronGas & hadronGas) --1--" << endl;
  double zero = 0;
  double temperature = hadronGas.getTemperature();
  int    iT  = energyDensityVsT->GetXaxis()->FindBin(temperature);
  ParticleTypeCollection & particleTypes       = hadronGas.getParticleTypes();
  ParticleTypeCollection & stableParticleTypes = hadronGas.getStableParticleTypes();
  vector<double> & particleDensities           = hadronGas.particleDensities;
  vector<double> & stableParticleDensities     = hadronGas.stableParticleDensities;
  int nSpecies       = particleTypes.size();
  int nStableSpecies = stableParticleTypes.size();
  numberDensityVsT   ->SetBinContent(iT, hadronGas.getNumberDensity() ); energyDensityVsT->SetBinError(iT,zero);
  energyDensityVsT   ->SetBinContent(iT, hadronGas.getEnergyDensity() ); energyDensityVsT->SetBinError(iT,zero);
  entropyDensityVsT  ->SetBinContent(iT, hadronGas.getEntropyDensity()); entropyDensityVsT->SetBinError(iT,zero);
  pressureVsT        ->SetBinContent(iT, hadronGas.getPressure()      ); pressureVsT->SetBinError(iT,zero);

  for (int iSpecies=0; iSpecies<nStableSpecies; iSpecies++)
  {

  //cout << "hadronGas.particleDensities[iSpecies]:" << hadronGas.particleDensities[iSpecies] << endl;
  //cout << "hadronGas.particleEnergies[iSpecies]:" << hadronGas.particleEnergies[iSpecies] << endl;
  //cout << "hadronGas.particleEntropies[iSpecies]:" << hadronGas.particleEntropies[iSpecies] << endl;

    nDensityVsT[iSpecies]->SetBinContent(iT, hadronGas.particleDensities[iSpecies] ); nDensityVsT[iSpecies]->SetBinError(iT,zero);
    eDensityVsT[iSpecies]->SetBinContent(iT, hadronGas.particleEnergies[iSpecies]  ); eDensityVsT[iSpecies]->SetBinError(iT,zero);
    sDensityVsT[iSpecies]->SetBinContent(iT, hadronGas.particleEntropies[iSpecies] ); sDensityVsT[iSpecies]->SetBinError(iT,zero);
  }
  //cout << " HadronGasVsTempHistograms::fill(HadronGas & hadronGas) --7--" << endl;

}

