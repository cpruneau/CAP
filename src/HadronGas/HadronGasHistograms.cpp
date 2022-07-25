//
//  HadronGasVsTempHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "HadronGas.hpp"
#include "HadronGasHistograms.hpp"

ClassImp(HadronGasHistograms);

HadronGasHistograms::HadronGasHistograms(const TString &       _name,
                                         const Configuration & _config,
                                         LogLevel              _debugLevel)
:
Histograms(_name,_config,_debugLevel),
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
  appendClassName("HadronGasHistograms");
  setInstanceName(_name);
}

void HadronGasHistograms::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  TString bn = getHistoBaseName();
  const   Configuration & config   = getConfiguration();
  int     nMass                    = config.getValueInt("nMass");
  double  minMass                  = config.getValueDouble("minMass");
  double  maxMass                  = config.getValueDouble("maxMass");
  int     nThermalSpecies          = config.getValueInt("nThermalSpecies");
  int     nStableSpecies           = config.getValueInt("nStableSpecies");
//  bool    plotStableSpeciesVsT     = config.getValueBool("doPlotVsStableSpecies");
//  bool    plotThermalSpeciesVsT    = config.getValueBool("doPlotVsAllSpecies");
  bool    plotPtDistHistos         = config.getValueBool("plotPtDistHistos");
//  int     nChemicalTemp            = config.getValueInt("nChemicalTemp");
//  double  minChemicalTemp          = config.getValueDouble("minChemicalTemp");
//  double  maxChemicalTemp          = config.getValueDouble("maxChemicalTemp");
//  double  stepTemp                 = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);
//  int     nMuB                     = config.getValueInt("nMuB");
//  double  minMuB                   = config.getValueDouble("minMuB");
//  double  maxMuB                   = config.getValueDouble("maxMuB");
//  double  stepMuB                  = (maxMuB - minMuB)/double(nMuB);
//  int     nMuS                     = configuration.getValueInt("nMuS");
//  double  minMuS                   = configuration.getValueDouble("minMuS");
//  double  maxMuS                   = configuration.getValueDouble("maxMuS");
//  double  stepMuS                  = (maxMuS - minMuS)/double(nMuS);
  int     nP                       = config.getValueInt("nP");
  double  minP                     = config.getValueDouble("minP");
  double  maxP                     = config.getValueDouble("maxP");

  double dSpecies       = nThermalSpecies;
  double dStableSpecies = nStableSpecies;

  vector<TString> allSpeciesLabels;
  vector<TString> stableSpeciesLabels;
  for (int k=0;k<nThermalSpecies; k++)
    {
    TString key = "Species";
    key += k;
    allSpeciesLabels.push_back(config.getValueString(key));
    }
  for (int k=0;k<nStableSpecies; k++)
    {
    TString key = "StableSpecies";
    key += k;
    stableSpeciesLabels.push_back(config.getValueString(key));
    }

  h_numberDensity    = createHistogram(makeName(bn,"numberDensity"),  nThermalSpecies,0.0,dSpecies,"Index","n");
  h_energyDensity    = createHistogram(makeName(bn,"energyDensity"),  nThermalSpecies,0.0,dSpecies,"Index","e");
  h_entropyDensity   = createHistogram(makeName(bn,"entropyDensity"), nThermalSpecies,0.0,dSpecies,"Index","s");
  h_pressure         = createHistogram(makeName(bn,"pressure"),       nThermalSpecies,0.0,dSpecies,"Index","p");

  h_numberDensityVsMass   = createProfile(makeName(bn,"numberDensityVsMass"),  nMass,minMass,maxMass,"Mass (GeV/c^{2})","n");
  h_energyDensityVsMass   = createProfile(makeName(bn,"energyDensityVsMass"),  nMass,minMass,maxMass,"Mass (GeV/c^{2})","e");
  h_entropyDensityVsMass  = createProfile(makeName(bn,"entropyDensityVsMass"), nMass,minMass,maxMass,"Mass (GeV/c^{2})","s");
  h_pressureVsMass        = createProfile(makeName(bn,"pressureVsMass"),       nMass,minMass,maxMass,"Mass (GeV/c^{2})","p");

  h_rho1ThVsIndex    = createHistogram(makeName(bn,"rho1ThVsIndex"),    nStableSpecies,0.0,dStableSpecies,"Index","n");
  h_rho1VsIndex      = createHistogram(makeName(bn,"rho1VsIndex"),      nStableSpecies,0.0,dStableSpecies,"Index","n");
  h_rho1RatioVsIndex = createHistogram(makeName(bn,"rho1RatioVsIndex"), nStableSpecies,0.0,dStableSpecies,"Index","n");

  h_rho1ThVsMass     = createHistogram(makeName(bn,"rho1ThVsMass"),     nMass,minMass,maxMass,"Mass (GeV/c^{2})","n");
  h_rho1VsMass       = createHistogram(makeName(bn,"rho1VsMass"),       nMass,minMass,maxMass,"Mass (GeV/c^{2})","n");
  h_rho1RatioVsMass  = createHistogram(makeName(bn,"rho1RatioVsMass"),  nMass,minMass,maxMass,"Mass (GeV/c^{2})","n");

  h_rho1rho1        = createHistogram(makeName(bn,"rho1rho1"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","#rho_{1}#rho_{1}");
  h_rho1thrho1th    = createHistogram(makeName(bn,"rho1thrho1th"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","#rho_{1}^{Th}#rho_{1}^{Th}");
  h_rho2Corr        = createHistogram(makeName(bn,"rho2Corr"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","Correlated Pairs");
  h_rho2Uncorr      = createHistogram(makeName(bn,"rho2Uncorr"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","Uncorrelated Pairs");
  h_rho2            = createHistogram(makeName(bn,"rho2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","#rho_{2}");
  h_C2              = createHistogram(makeName(bn,"C2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","C_{2}");
  h_R2              = createHistogram(makeName(bn,"R2"),
                                      nStableSpecies,  0.0, dStableSpecies,
                                      nStableSpecies,  0.0, dStableSpecies,
                                      "Species","Species","R_{2}");
  //h_BF              = createHistogram(makeName(bn,"BF"),18,  0.0, 18.0, "Pairs","BF");

  // set labels
  for (int iSpecies=0; iSpecies<nThermalSpecies; iSpecies++)
    {
    int bin = iSpecies+1;
    TString label = allSpeciesLabels[iSpecies];
    h_numberDensity  ->GetXaxis()->SetBinLabel(bin,label);
    h_energyDensity  ->GetXaxis()->SetBinLabel(bin,label);
    h_entropyDensity ->GetXaxis()->SetBinLabel(bin,label);
    h_pressure       ->GetXaxis()->SetBinLabel(bin,label);
    }
  for (int iSpecies=0; iSpecies<nStableSpecies; iSpecies++)
    {
    int bin = iSpecies+1;
    TString label = stableSpeciesLabels[iSpecies];
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
    TString histoName;
    for (int iSpecies=0; iSpecies<nThermalSpecies; iSpecies++)
      {
      h_rho1ThVsP.push_back(createHistogram(makeName(bn,allSpeciesLabels[iSpecies],"rho1ThVsP"),nP,minP,maxP,"p (GeV/c)","#rho_{1}^{Th} (c/GeV)"));
      }
    }
  if (reportEnd("HadronGasHistograms",getName(),"createHistograms()"))
    ;
}

// Fix me...
void HadronGasHistograms::loadHistograms(TFile * inputFile __attribute__((unused)) )
{

}


void HadronGasHistograms::fill(HadronGas & hadronGas)
{
  if (reportStart(__FUNCTION__))
    ;
  const   Configuration & config   = getConfiguration();
//  int     nMass                    = config.getValueInt("nMass");
//  double  minMass                  = config.getValueDouble("minMass");
//  double  maxMass                  = config.getValueDouble("maxMass");
  int     nThermalSpecies          = config.getValueInt("nThermalSpecies");
  int     nStableSpecies           = config.getValueInt("nStableSpecies");
//  bool    plotStableSpeciesVsT     = config.getValueBool("doPlotVsStableSpecies");
//  bool    plotThermalSpeciesVsT    = config.getValueBool("doPlotVsAllSpecies");
  bool    plotPtDistHistos         = config.getValueBool("plotPtDistHistos");
//  int     nChemicalTemp            = config.getValueInt("nChemicalTemp");
//  double  minChemicalTemp          = config.getValueDouble("minChemicalTemp");
//  double  maxChemicalTemp          = config.getValueDouble("maxChemicalTemp");
//  double  stepTemp                 = (maxChemicalTemp - minChemicalTemp)/double(nChemicalTemp);
//  int     nMuB                     = config.getValueInt("nMuB");
//  double  minMuB                   = config.getValueDouble("minMuB");
//  double  maxMuB                   = config.getValueDouble("maxMuB");
//  double  stepMuB                  = (maxMuB - minMuB)/double(nMuB);
//  int     nMuS                     = configuration.getValueInt("nMuS");
//  double  minMuS                   = configuration.getValueDouble("minMuS");
//  double  maxMuS                   = configuration.getValueDouble("maxMuS");
//  double  stepMuS                  = (maxMuS - minMuS)/double(nMuS);
  int     nP                       = config.getValueInt("nP");
  double  minP                     = config.getValueDouble("minP");
  double  maxP                     = config.getValueDouble("maxP");
  double  stepP                    = (maxP - minP)/double(nP);
  double  zero = 0.0;
  
  //
  // thermal (undecayed) particles
  //
  vector<HadronGasParticle*> &  hadrons = hadronGas.getAllHadrons();
  for (int iSpecies1=0; iSpecies1<nThermalSpecies; iSpecies1++)
    {
    HadronGasParticle & hadron = *(hadrons[iSpecies1]); //  getHadronGasParticleAt();
    double mass  = hadron.mass;
    double gSpin = hadron.gSpin;
    double numberDensity  = hadron.numberDensity;
    double energyDensity  = hadron.energyDensity;
    double entropyDensity = hadron.entropyDensity;
    double pressure       = hadron.pressure;
    h_numberDensity  ->SetBinContent(iSpecies1,numberDensity);  h_numberDensity  ->SetBinError(iSpecies1,zero);
    h_energyDensity  ->SetBinContent(iSpecies1,energyDensity);  h_energyDensity  ->SetBinError(iSpecies1,zero);
    h_entropyDensity ->SetBinContent(iSpecies1,entropyDensity); h_entropyDensity ->SetBinError(iSpecies1,zero);
    h_pressure       ->SetBinContent(iSpecies1,pressure);       h_pressure       ->SetBinError(iSpecies1,zero);

    h_numberDensityVsMass   ->Fill(mass,numberDensity/gSpin);
    h_energyDensityVsMass   ->Fill(mass,energyDensity/gSpin);
    h_entropyDensityVsMass  ->Fill(mass,entropyDensity/gSpin);
    h_pressureVsMass        ->Fill(mass,pressure/gSpin);
    }

//  vector<double> rho1_stable;
//  vector< vector<double> > rho2_stable;
//  vector< vector<double> > rho2cor_stable;
//  vector< vector<double> > rho1rho1_stable;
//  vector< vector<double> > c2_stable;
  //
  // Decayed  particles
  //

  ParticleTypeCollection & stableParticles = hadronGas.getStableParticleTypes();

  for (int iStableSpecies1=0; iStableSpecies1<nStableSpecies; iStableSpecies1++)
    {
    // Fix me
    double rho1th_1     = 1.0; //hadronGas.particleDensities[iStableSpecies1];
    double rho1Stable_1 = hadronGas.rho1_stable[iStableSpecies1];
    double mass = stableParticles[iStableSpecies1]->getMass();
    h_rho1ThVsIndex  ->SetBinContent(iStableSpecies1+1, rho1th_1);      h_rho1ThVsIndex ->SetBinError(iStableSpecies1,zero);
    h_rho1VsIndex    ->SetBinContent(iStableSpecies1+1, rho1Stable_1);  h_rho1VsIndex   ->SetBinError(iStableSpecies1,zero);
    h_rho1ThVsMass   ->Fill(mass,rho1th_1);
    h_rho1VsMass     ->Fill(mass,rho1Stable_1);
    for (int iStableSpecies2=0; iStableSpecies2<nStableSpecies; iStableSpecies2++)
      {
      // Fix me
      //double rho1th_2     = 1.0; //hadronGas.rho1_stable[iStableSpecies2];
      double rho1Stable_2 = hadronGas.rho1_stable[iStableSpecies2];
      double rho2_12      = hadronGas.rho2_stable[iStableSpecies1][iStableSpecies2];
      double rho2_12c     = hadronGas.rho2cor_stable[iStableSpecies1][iStableSpecies2];
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
  if (plotPtDistHistos)
    {
    for (unsigned int iSpecies=0; iSpecies<hadrons.size(); iSpecies++)
      {
      HadronGasParticle & hadron = *(hadrons[iSpecies]);
      //double mass  = hadron.mass;
      //double gSpin = hadron.gSpin;
      double p = minP;
      for (int iP=0; iP<nP; iP++)
        {
        p += stepP;
        double density = hadron.computeThermalDensityAtP(p);
        h_rho1ThVsP[iSpecies]->SetBinContent(iP, density);
        h_rho1ThVsP[iSpecies]->SetBinError(iP,zero);
        }
      }
    }

  if (reportEnd("HadronGasHistograms",getName(),"fillRho1VsP(..)"))
    ;
}

