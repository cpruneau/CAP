//
//  ThermalGasVsTempHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "ThermalGas.hpp"
#include "ThermalGasHistograms.hpp"
using CAP::ThermalGasHistograms;

ClassImp(ThermalGasHistograms);

ThermalGasHistograms::ThermalGasHistograms(Task * _parent,
                                         const String & _name,
                                         const Configuration & _config)
:
HistogramGroup(_parent,_name,_config),
h_numberDensity(nullptr),
h_energyDensity(nullptr),
h_entropyDensity(nullptr),
h_pressure(nullptr),
h_numberDensityVsMass(nullptr),
h_energyDensityVsMass(nullptr),
h_entropyDensityVsMass(nullptr),
h_pressureVsMass(nullptr),
h_rho1ThVsIndex(nullptr),
h_rho1VsIndex(nullptr),
h_rho1RatioVsIndex(nullptr),
h_rho1ThVsMass(nullptr),
h_rho1VsMass(nullptr),
h_rho1RatioVsMass(nullptr),
h_rho1thrho1th(nullptr),
h_rho1rho1(nullptr),
h_rho2Corr(nullptr),
h_rho2Uncorr(nullptr),
h_rho2(nullptr),
h_C2(nullptr),
h_R2(nullptr),
h_rho1ThVsP()
{
  appendClassName("ThermalGasHistograms");
}

void ThermalGasHistograms::createHistograms()
{
  if (reportStart(__FUNCTION__))
;
  String pn = getParentTask()->getName();
  String bn = getParentName();
  const   Configuration & config   = getConfiguration();
  int     nMass                    = config.getValueInt(pn,"nMass");
  double  minMass                  = config.getValueDouble(pn,"MinMass");
  double  maxMass                  = config.getValueDouble(pn,"MaxMass");
  int     nThermalSpecies          = config.getValueInt(pn,"nThermalSpecies");
  int     nStableSpecies           = config.getValueInt(pn,"nStableSpecies");
//  bool    plotStableSpeciesVsT     = config.getValueBool(pn,"DoPlotVsStableSpecies");
//  bool    plotThermalSpeciesVsT    = config.getValueBool(pn,"DoPlotVsAllSpecies");
  bool    plotPtDistHistos         = config.getValueBool(pn,"PlotPtDistHistos");
//  int     nChemicalTemp            = config.getValueInt(pn,"nChemicalTemp");
//  double  minChemicalTemp          = getValueDouble("MinChemicalTemp");
//  double  maxChemicalTemp          = getValueDouble("MaxChemicalTemp");
//  double  stepTemp                 = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);
//  int     nMuB                     = config.getValueInt(getName(),"nMuB");
//  double  minMuB                   = getValueDouble("MinMuB");
//  double  maxMuB                   = getValueDouble("MaxMuB");
//  double  stepMuB                  = (maxMuB - minMuB)/double(nMuB);
//  int     nMuS                     = configuration.getValueInt(getName(),"nMuS");
//  double  minMuS                   = configuration.getValueDouble(getName(),"MinMuS");
//  double  maxMuS                   = configuration.getValueDouble(getName(),"MaxMuS");
//  double  stepMuS                  = (maxMuS - minMuS)/double(nMuS);
  int     nP                       = config.getValueInt(pn,"nP");
  double  minP                     = config.getValueDouble(pn,"MinP");
  double  maxP                     = config.getValueDouble(pn,"MaxP");

  double dSpecies       = nThermalSpecies;
  double dStableSpecies = nStableSpecies;

  VectorString  allSpeciesLabels;
  VectorString  stableSpeciesLabels;
  for (int k=0;k<nThermalSpecies; k++)
    {
    String key = "Species";
    key += k;
    allSpeciesLabels.push_back(config.getValueString(pn,key));
    }
  for (int k=0;k<nStableSpecies; k++)
    {
    String key = "StableSpecies";
    key += k;
    stableSpeciesLabels.push_back(config.getValueString(pn,key));
    }

  h_numberDensity    = createHistogram(createName(bn,"numberDensity"),  nThermalSpecies,0.0,dSpecies,"Index","n");
  h_energyDensity    = createHistogram(createName(bn,"energyDensity"),  nThermalSpecies,0.0,dSpecies,"Index","e");
  h_entropyDensity   = createHistogram(createName(bn,"entropyDensity"), nThermalSpecies,0.0,dSpecies,"Index","s");
  h_pressure         = createHistogram(createName(bn,"pressure"),       nThermalSpecies,0.0,dSpecies,"Index","p");

  h_numberDensityVsMass   = createProfile(createName(bn,"numberDensityVsMass"),  nMass,minMass,maxMass,"Mass (GeV/c^{2})","n");
  h_energyDensityVsMass   = createProfile(createName(bn,"energyDensityVsMass"),  nMass,minMass,maxMass,"Mass (GeV/c^{2})","e");
  h_entropyDensityVsMass  = createProfile(createName(bn,"entropyDensityVsMass"), nMass,minMass,maxMass,"Mass (GeV/c^{2})","s");
  h_pressureVsMass        = createProfile(createName(bn,"pressureVsMass"),       nMass,minMass,maxMass,"Mass (GeV/c^{2})","p");

  h_rho1ThVsIndex    = createHistogram(createName(bn,"rho1ThVsIndex"),    nStableSpecies,0.0,dStableSpecies,"Index","n");
  h_rho1VsIndex      = createHistogram(createName(bn,"rho1VsIndex"),      nStableSpecies,0.0,dStableSpecies,"Index","n");
  h_rho1RatioVsIndex = createHistogram(createName(bn,"rho1RatioVsIndex"), nStableSpecies,0.0,dStableSpecies,"Index","n");

  h_rho1ThVsMass     = createHistogram(createName(bn,"rho1ThVsMass"),     nMass,minMass,maxMass,"Mass (GeV/c^{2})","n");
  h_rho1VsMass       = createHistogram(createName(bn,"rho1VsMass"),       nMass,minMass,maxMass,"Mass (GeV/c^{2})","n");
  h_rho1RatioVsMass  = createHistogram(createName(bn,"rho1RatioVsMass"),  nMass,minMass,maxMass,"Mass (GeV/c^{2})","n");

  h_rho1rho1        = createHistogram(createName(bn,"rho1rho1"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","#rho_{1}#rho_{1}");
  h_rho1thrho1th    = createHistogram(createName(bn,"rho1thrho1th"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","#rho_{1}^{Th}#rho_{1}^{Th}");
  h_rho2Corr        = createHistogram(createName(bn,"rho2Corr"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","Correlated Pairs");
  h_rho2Uncorr      = createHistogram(createName(bn,"rho2Uncorr"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","Uncorrelated Pairs");
  h_rho2            = createHistogram(createName(bn,"rho2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","#rho_{2}");
  h_C2              = createHistogram(createName(bn,"C2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","C_{2}");
  h_R2              = createHistogram(createName(bn,"R2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","R_{2}");
  //h_BF              = createHistogram(createName(bn,"BF"),18,  0.0, 18.0, "Pairs","BF");

  // set labels
  for (int iSpecies=0; iSpecies<nThermalSpecies; iSpecies++)
    {
    int bin = iSpecies+1;
    String label = allSpeciesLabels[iSpecies];
    h_numberDensity  ->GetXaxis()->SetBinLabel(bin,label);
    h_energyDensity  ->GetXaxis()->SetBinLabel(bin,label);
    h_entropyDensity ->GetXaxis()->SetBinLabel(bin,label);
    h_pressure       ->GetXaxis()->SetBinLabel(bin,label);
    }
  for (int iSpecies=0; iSpecies<nStableSpecies; iSpecies++)
    {
    int bin = iSpecies+1;
    String label = stableSpeciesLabels[iSpecies];
    h_rho1ThVsIndex    ->GetXaxis()->SetBinLabel(bin,label);
    h_rho1VsIndex      ->GetXaxis()->SetBinLabel(bin,label);
    h_rho1RatioVsIndex ->GetXaxis()->SetBinLabel(bin,label);
    h_rho1rho1         ->GetXaxis()->SetBinLabel(bin,label);
    h_rho1rho1         ->GetYaxis()->SetBinLabel(bin,label);
    h_rho1thrho1th     ->GetXaxis()->SetBinLabel(bin,label);
    h_rho1thrho1th     ->GetYaxis()->SetBinLabel(bin,label);
    h_rho2Corr         ->GetXaxis()->SetBinLabel(bin,label);
    h_rho2Corr         ->GetYaxis()->SetBinLabel(bin,label);
    h_rho2Uncorr       ->GetXaxis()->SetBinLabel(bin,label);
    h_rho2Uncorr       ->GetYaxis()->SetBinLabel(bin,label);
    h_rho2             ->GetXaxis()->SetBinLabel(bin,label);
    h_rho2             ->GetYaxis()->SetBinLabel(bin,label);
    h_C2               ->GetXaxis()->SetBinLabel(bin,label);
    h_C2               ->GetYaxis()->SetBinLabel(bin,label);
    h_R2               ->GetXaxis()->SetBinLabel(bin,label);
    h_R2               ->GetYaxis()->SetBinLabel(bin,label);
    }
  if (plotPtDistHistos)
    {
    String histoName;
    for (int iSpecies=0; iSpecies<nThermalSpecies; iSpecies++)
      {
      h_rho1ThVsP.push_back(createHistogram(createName(bn,allSpeciesLabels[iSpecies],"rho1ThVsP"),nP,minP,maxP,"p (GeV/c)","#rho_{1}^{Th} (c/GeV)"));
      }
    }
  if (reportEnd(__FUNCTION__))
;
}

// Fix me...
void ThermalGasHistograms::importHistograms(TFile & inputFile __attribute__((unused)) )
{

}


void ThermalGasHistograms::fill(ThermalGas & ThermalGas)
{
  if (reportStart(__FUNCTION__))
;
  String pn = getParentTask()->getName();
  const  Configuration & config   = getConfiguration();
//  int     nMass                    = config.getValueInt(pn,"nMass");
//  double  minMass                  = config.getValueDouble(pn,"MinMass");
//  double  maxMass                  = config.getValueDouble(pn,"MaxMass");
  int     nThermalSpecies          = config.getValueInt(pn,"nThermalSpecies");
  int     nStableSpecies           = config.getValueInt(pn,"nStableSpecies");
//  bool    plotStableSpeciesVsT     = config.getValueBool(pn,"DoPlotVsStableSpecies");
//  bool    plotThermalSpeciesVsT    = config.getValueBool(pn,"DoPlotVsAllSpecies");
  bool    plotPtDistHistos         = config.getValueBool(pn,"PlotPtDistHistos");
//  int     nChemicalTemp            = config.getValueInt(pn,"nChemicalTemp");
//  double  minChemicalTemp          = config.getValueDouble(pn,"MinChemicalTemp");
//  double  maxChemicalTemp          = config.getValueDouble(pn,"MaxChemicalTemp");
//  double  stepTemp                 = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);
//  int     nMuB                     = config.getValueInt(pn,"nMuB");
//  double  minMuB                   = config.getValueDouble(pn,"MinMuB");
//  double  maxMuB                   = config.getValueDouble(pn,"MaxMuB");
//  double  stepMuB                  = (maxMuB - minMuB)/double(nMuB);
//  int     nMuS                     = configuration.getValueInt(pn,"nMuS");
//  double  minMuS                   = configuration.getValueDouble(pn,"MinMuS");
//  double  maxMuS                   = configuration.getValueDouble(pn,"MaxMuS");
//  double  stepMuS                  = (maxMuS - minMuS)/double(nMuS);
  int     nP                       = config.getValueInt(pn,"nP");
  double  minP                     = config.getValueDouble(pn,"MinP");
  double  maxP                     = config.getValueDouble(pn,"MaxP");
  //double  stepP                    = (maxP - minP)/double(nP);
  double  zero = 0.0;
  
  //
  // thermal (undecayed) particles
  //
  vector<ParticleThermalProperties*> &  hadrons = ThermalGas.getAllHadrons();
  for (int iSpecies1=0; iSpecies1<nThermalSpecies; iSpecies1++)
    {
    ParticleThermalProperties & hadron = *(hadrons[iSpecies1]); //  getParticleThermalPropertiesAt();
    double mass  = hadron.getMass();
    double spinFactor = hadron.getSpinFactor();
    double numberDensity  = hadron.numberDensity;
    double energyDensity  = hadron.energyDensity;
    double entropyDensity = hadron.entropyDensity;
    double pressure       = hadron.pressure;
    h_numberDensity  ->SetBinContent(iSpecies1,numberDensity);  h_numberDensity  ->SetBinError(iSpecies1,zero);
    h_energyDensity  ->SetBinContent(iSpecies1,energyDensity);  h_energyDensity  ->SetBinError(iSpecies1,zero);
    h_entropyDensity ->SetBinContent(iSpecies1,entropyDensity); h_entropyDensity ->SetBinError(iSpecies1,zero);
    h_pressure       ->SetBinContent(iSpecies1,pressure);       h_pressure       ->SetBinError(iSpecies1,zero);

    h_numberDensityVsMass   ->Fill(mass,numberDensity/spinFactor);
    h_energyDensityVsMass   ->Fill(mass,energyDensity/spinFactor);
    h_entropyDensityVsMass  ->Fill(mass,entropyDensity/spinFactor);
    h_pressureVsMass        ->Fill(mass,pressure/spinFactor);
    }

//  vector<double> rho1_stable;
//  vector< vector<double> > rho2_stable;
//  vector< vector<double> > rho2cor_stable;
//  vector< vector<double> > rho1rho1_stable;
//  vector< vector<double> > c2_stable;
  //
  // Decayed  particles
  //

  ParticleDb & stableParticles = ThermalGas.getStableParticleTypes();

  for (int iStableSpecies1=0; iStableSpecies1<nStableSpecies; iStableSpecies1++)
    {
    // Fix me
    double rho1th_1     = 1.0; //ThermalGas.particleDensities[iStableSpecies1];
    double rho1Stable_1 = ThermalGas.rho1_stable[iStableSpecies1];
    double mass = stableParticles[iStableSpecies1]->getMass();
    h_rho1ThVsIndex  ->SetBinContent(iStableSpecies1+1, rho1th_1);      h_rho1ThVsIndex ->SetBinError(iStableSpecies1,zero);
    h_rho1VsIndex    ->SetBinContent(iStableSpecies1+1, rho1Stable_1);  h_rho1VsIndex   ->SetBinError(iStableSpecies1,zero);
    h_rho1ThVsMass   ->Fill(mass,rho1th_1);
    h_rho1VsMass     ->Fill(mass,rho1Stable_1);
    for (int iStableSpecies2=0; iStableSpecies2<nStableSpecies; iStableSpecies2++)
      {
      // Fix me
      //double rho1th_2     = 1.0; //ThermalGas.rho1_stable[iStableSpecies2];
      double rho1Stable_2 = ThermalGas.rho1_stable[iStableSpecies2];
      double rho2_12      = ThermalGas.rho2_stable[iStableSpecies1][iStableSpecies2];
      double rho2_12c     = ThermalGas.rho2cor_stable[iStableSpecies1][iStableSpecies2];
      h_rho1rho1->SetBinContent  (iStableSpecies1+1, iStableSpecies2+1, rho1Stable_1*rho1Stable_2);
      h_rho1rho1->SetBinError    (iStableSpecies1+1, iStableSpecies2+1, zero);
      h_rho2    ->SetBinContent  (iStableSpecies1+1, iStableSpecies2+1, rho2_12);
      h_rho2    ->SetBinError    (iStableSpecies1+1, iStableSpecies2+1, zero);
      h_rho2Corr->SetBinContent  (iStableSpecies1+1, iStableSpecies2+1, rho2_12c);
      h_rho2Corr->SetBinError    (iStableSpecies1+1, iStableSpecies2+1, zero);
      }
    }

  //
  // Momentum/Energy spectra
  //
//  if (plotPtDistHistos)
//    {
//    for (unsigned int iSpecies=0; iSpecies<hadrons.size(); iSpecies++)
//      {
//      ParticleThermalProperties & hadron = *(hadrons[iSpecies]);
//      //double mass  = hadron.mass;
//      //double spinFactor = hadron.spinFactor;
//      double p = minP;
//      for (int iP=0; iP<nP; iP++)
//        {
//        p += stepP;
//        double density = hadron.computeThermalDensityAtP(p);
//        h_rho1ThVsP[iSpecies]->SetBinContent(iP, density);
//        h_rho1ThVsP[iSpecies]->SetBinError(iP,zero);
//        }
//      }
//    }

  if (reportEnd(__FUNCTION__))
;
}

