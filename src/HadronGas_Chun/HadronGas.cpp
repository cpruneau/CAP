// Copyright 2016 Chun Shen

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <TMath.h>
#include "HadronGas.hpp"
#include "Math/SpecFunc.h"

ClassImp(HadronGas);

using namespace std;

int HadronGas::trunOrder = 10;
double HadronGas::twoPi  = 2.0*3.1415927;
double HadronGas::twoPiCube = twoPi*twoPi*twoPi;

HadronGas::HadronGas(ParticleTypeCollection * _particles,
                     ParticleTypeCollection * _stableParticles,
                     LogLevel debugLevel)
:
particleTypes(_particles),
stableParticleTypes(_stableParticles),
temperature(0),
mu(0),
muB(0),
muS(0),
muQ(0),
thermalNumberDensity(0),
thermalEnergyDensity(0),
thermalEntropyDensity(0),
thermalPressure(0),
chemicalPotentials(),
particleDensities(),
particleEnergies(),
particleEntropies(),
particlePressures(),
particleDsDmu(),
particleDnDmu(),
particleDeOverTDmu(),
particleDpOverTmu(),
stableParticleDensities(),
stableParticlePairDensities(),
stableParticlePairCorrelatedDensities(),
stableParticlePairUncorrelatedDensities(),
decayProbabilities(),
decayPairProbabilities(),
name("HadronGas")
{
  calculateParticleDecayProbability();
}

HadronGas::~HadronGas()
{
}

ostream & HadronGas::printProperties(ostream & os)
{
  os << "===================================================================================================================" << std::endl;
  os << "===================================================================================================================" << std::endl;
  os << " Hadron Gas Global Properties" << endl;
  os << "===================================================================================================================" << std::endl;
  os << "===================================================================================================================" << std::endl;
  os << "           temperature: " << scientific << setw(15)<< setprecision(5) << temperature << endl;
  os << "                    mu: " << scientific << setw(15)<< setprecision(5) << mu << endl;
  os << "                   muB: " << scientific << setw(15)<< setprecision(5) << muB << endl;
  os << "                   muS: " << scientific << setw(15)<< setprecision(5) << muS << endl;
  os << "                   muQ: " << scientific << setw(15)<< setprecision(5) << muQ << endl;
  os << "  thermalNumberDensity: " << scientific << setw(15)<< setprecision(5) << thermalNumberDensity << endl;
  os << "  thermalEnergyDensity: " << scientific << setw(15)<< setprecision(5) << thermalEnergyDensity << endl;
  os << " thermalEntropyDensity: " << scientific << setw(15)<< setprecision(5) << thermalEntropyDensity << endl;
  os << "       thermalPressure: " << scientific << setw(15)<< setprecision(5) << thermalPressure << endl;
  os << "===================================================================================================================" << std::endl;
  os << " Thermal Hadron Properties" << endl;
  os << setw(5)  << "k";
  os << setw(25) << "Name";
  os << setw(15) << "Mu";
  os << setw(15) << "Density";
  os << setw(15) << "Energy";
  os << setw(15) << "Entropy";
  os << setw(15) << "Pressure" << endl;
  os << "===================================================================================================================" << std::endl;
  int nPart = particleTypes->size();
  for (int iPart=0; iPart<nPart; iPart++)
    {
    cout
    << setw(5)    << iPart
    << setw(25)   << particleTypes->getParticleType(iPart)->getName()
    << scientific << setw(15)<< setprecision(5) << chemicalPotentials[iPart]
    << scientific << setw(15)<< setprecision(5) << particleDensities[iPart]
    << scientific << setw(15)<< setprecision(5) << particleEnergies[iPart]
    << scientific << setw(15)<< setprecision(5) << particleEntropies[iPart]
    << scientific << setw(15)<< setprecision(5) << particlePressures[iPart]
    << endl;
    }
  os << "==================================================================================" << std::endl;
  os << "Stable Hadron Properties" << endl;
  os << setw(5)  << "k";
  os << setw(25) << "Name";
  os << setw(25) << "Density"  << endl;
  os << "==================================================================================" << std::endl;
  nPart = stableParticleTypes->size();
  for (int iPart=0; iPart<nPart; iPart++)
    {
    cout
    <<  setw(5)  << iPart
    <<  setw(25) << stableParticleTypes->getParticleType(iPart)->getName()
    <<  scientific << setw(25)<< setprecision(5) << stableParticleDensities[iPart]
    << endl;
    }

  return os;
}

void HadronGas::calculateParticleDecayProbability()
{
  if (reportStart("HadronGas", getName(), "calculateParticleDecayProbability()" ) )
    ;
  int nPart       = particleTypes->size();
  int nStablePart = stableParticleTypes->size();
  if (reportInfo("HadronGas", getName(), "calculateParticleDecayProbability()" ) )
    {
    cout << "==================================================================================" << std::endl;
    cout << "<I> HadronGas::calculateParticleDecayProbability()" << endl;
    cout << "        nPart: " << nPart << endl;
    cout << "  nStablePart: " << nStablePart  << endl;
    cout << "==================================================================================" << std::endl;
    }
  decayProbabilities.clear();
  decayPairProbabilities.clear();
  // Setup arrays
  for (int iPart = 0; iPart < nPart; iPart++)
    {
    vector<double> temp(nStablePart,0.0);
    vector< vector<double> > temp2; // (nStablePart,temp);
    for (int k=0;k<nStablePart;k++) temp2.push_back(temp);

    ParticleType & particleType = * (particleTypes->getParticleType(iPart));
    if (particleType.isStable() && !particleType.isPhoton())
      {
      int indexStable = stableParticleTypes->findIndexForPdgCode(particleType.getPdgCode());
      if (indexStable<0 || indexStable>=nStablePart)
        {
        if (reportError("HadronGas", getName(), "calculateParticleDecayProbability()" ) )
          {
          cout << "==================================================================================" << std::endl;
          cout << "iPart: " << iPart << " indexStable:" << indexStable << " --- LOGIC ERROR - ABORT" << endl;
          cout << "Terminate task."  << endl;
          cout << "==================================================================================" << std::endl;
          exit(1);
          }
        }
      temp[indexStable] = 1.0;
      }
    decayProbabilities.push_back(temp);
    decayPairProbabilities.push_back(temp2);
    }
  // Calculate decayProbabilities
  for (int iPart = 0; iPart < nPart; iPart++)
    {
    vector<double> temp(nStablePart,0.0);
    vector< vector<double> > temp2; // (nStablePart,temp);
    for (int k=0;k<nStablePart;k++) temp2.push_back(temp);

    vector<int> indexOfDecayParts(5,0);
    ParticleType & particleType = * (particleTypes->getParticleType(iPart));
    for (int jChannel = 0; jChannel < particleType.getNDecayModes(); jChannel++)
      {
      ParticleDecayMode & decayMode = particleType.getDecayMode(jChannel);
      double branchingRatio = decayMode.getBranchingRatio();
      int    nChildren      = decayMode.getNChildren();
      for (int kParticle = 0; kParticle < nChildren; kParticle++)
        {
        int pdgCode = decayMode.getChildPdgCode(kParticle);
        int index   = particleTypes->findIndexForPdgCode(pdgCode);
        if (index<0 || index > iPart)
          {
          if (reportError("HadronGas", getName(), "calculateParticleDecayProbability()" ) )
            {
            cout << "==================================================================================" << std::endl;
            cout << "lPart1=" << index <<  " -- Could not find PDG code: " << pdgCode << endl;
            cout << "Terminate task."  << endl;
            cout << "==================================================================================" << std::endl;
            exit(1);
            }
          }
        indexOfDecayParts[kParticle] = index;
        }
      // now we have a local list of the pdg ids iPart decays into (w/o photons)
      // must look at singles and then at pairs
      // if there are no particleTypes left, just skip this decay channel..
      double prob1, prob2;
      for (int kParticle1 = 0; kParticle1 < nChildren; kParticle1++)
        {
        int lPart1 = indexOfDecayParts[kParticle1];
        for (int mStable1 = 0; mStable1 < nStablePart; mStable1++)
          {
          prob1 = decayProbabilities[lPart1][mStable1];
          temp[mStable1] += branchingRatio*prob1;

          //cout << "kParticle1:" << kParticle1 << "mStable1" << mStable1 << endl;

          for (int kParticle2 = 0; kParticle2 < nChildren; kParticle2++)
            {
            if (kParticle1 != kParticle2)
              {
              int lPart2 = indexOfDecayParts[kParticle2];
              //cout << "lPart2:" << lPart2 << endl;

              //cout <<  " kParticle1:" << kParticle1 << "  lPart1: " << lPart1  << " kParticle2:" << kParticle2 << "  lPart2: " << lPart2 << endl;
              for (int mStable2 = 0; mStable2 < nStablePart; mStable2++)
                {
                //cout << "mStable2:" << mStable2 << endl;

                prob2 = decayProbabilities[lPart2][mStable2];
                temp2[mStable1][mStable2] += branchingRatio*prob1*prob2;
                }
              }
            }

          for (int mStable3 = 0; mStable3 < nStablePart; mStable3++)
            {
            //cout << "mStable3:" << mStable3 << endl;

            prob1 = decayProbabilities[mStable1][mStable3];
            //if (branchingRatio * prob1>0.0) cout << " NON ZERO HAPPENS lPart1: " << lPart1 << " mStable3:" << mStable3<< endl;
            temp2[mStable1][mStable3] += branchingRatio *prob1;
            }
          }
        }
      }
   // cout << "Store results." << endl;
    for (int iStable=0;iStable<nStablePart;iStable++)
      {
      decayProbabilities[iPart][iStable] = temp[iStable];
      //cout << "iPart:" << iPart << " iStable:" << iStable <<  " decayProbabilities[iPart][iStable]:" << decayProbabilities[iPart][iStable] << endl;
      for (int jStable=0;jStable<nStablePart;jStable++)
        {
        decayPairProbabilities[iPart][iStable][jStable] = temp2[iStable][jStable];
        //cout << "iPart:" << iPart << " iStable:" << iStable << " jStable:" << jStable <<  " decayPairProbabilities[iPart][iStable][jStable] :" << decayPairProbabilities[iPart][iStable][jStable]  << endl;
        }
      }
//    cout << "Next Particle." << endl;

    }
  if (reportEnd("HadronGas", getName(), "calculateParticleDecayProbability()" ) )
    ;
}

void HadronGas::calculateParticleDecays()
{
  if (reportStart("HadronGas", getName(), "calculateParticleDecays()" ) )
    ;
  int nPart       = particleTypes->size();
  int nStablePart = stableParticleTypes->size();

  if (stableParticleDensities.size()>0)
    {
    stableParticleDensities.clear();
    stableParticlePairDensities.clear();
    stableParticlePairCorrelatedDensities.clear();
    stableParticlePairUncorrelatedDensities.clear();
    }
  vector<double> temp(nStablePart,0.0);
  vector< vector<double> > temp2(nStablePart,temp);
  stableParticleDensities                 = temp;
  stableParticlePairDensities             = temp2;
  stableParticlePairCorrelatedDensities   = temp2;
  stableParticlePairUncorrelatedDensities = temp2;

  double stableDensity;
  for (int jStable = 0; jStable < nStablePart;jStable++)
    {
    stableDensity = 0.0;
    for (int kPart = 0; kPart < nPart;kPart++)
      {
      stableDensity += decayProbabilities[kPart][jStable]*particleDensities[kPart];
      }
    stableParticleDensities[jStable] = stableDensity;
    }
  for (int jStable1 = 0; jStable1 < nStablePart; jStable1++)
    {
    for (int jStable2 = 0; jStable2 < nStablePart; jStable2++)
      {
      stableDensity = 0.0;
      for (int kPart = 0; kPart < nPart;kPart++)
        {
        stableDensity +=  decayPairProbabilities[kPart][jStable1][jStable2]*particleDensities[kPart];
        }
      stableParticlePairCorrelatedDensities[jStable1][jStable2] = stableDensity;

      stableDensity = 0.0;
      for (int kPart1 = 0; kPart1 < nPart;kPart1++)
        {
        double v1 = decayProbabilities[kPart1][jStable1]*particleDensities[kPart1];
        for (int kPart2 = 0; kPart2 < nPart;kPart2++)
          {
          if (kPart1 == kPart2) continue;
          double v2 = decayProbabilities[kPart2][jStable2]*particleDensities[kPart2];
          stableDensity += v1*v2;
          }
        stableParticlePairUncorrelatedDensities[jStable1][jStable2] = stableDensity;
        }

      stableParticlePairDensities[jStable1][jStable2] = stableParticleDensities[jStable1]*stableParticleDensities[jStable2];
      }
    }
  if (reportEnd("HadronGas", getName(), "calculateParticleDecays()" ) )
    ;
}

// ========================================================================================
// Reset all containers to empty.
// ========================================================================================
void HadronGas::clear()
{
  chemicalPotentials.clear();
  particleDensities.clear();
  particleEnergies.clear();
  particlePressures.clear();
  particleEntropies.clear();
  particleDsDmu.clear();
  particleDpOverTmu.clear();
  particleDeOverTDmu.clear();
  particleDnDmu.clear();
  deltaN_bulk_term1s.clear();
  deltaN_bulk_term2s.clear();
  deltaN_qmu_term1s.clear();
  deltaN_qmu_term2s.clear();
  stableParticleDensities.clear();
  stableParticlePairDensities.clear();
  stableParticlePairCorrelatedDensities.clear();
  stableParticlePairUncorrelatedDensities.clear();
  decayProbabilities.clear();
  decayPairProbabilities.clear();
}


// ========================================================================================
// Compute all properties of this gas at the given temperature
// and chemical potentials
// ========================================================================================
void HadronGas::calculateAllProperties(double _temp, double _muB, double _muS)
{
  if (reportStart("HadronGas", getName(), "calculateAllProperties(double _temp, double _muB, double _muS)" ) )
    ;
  temperature = _temp;
  muB         = _muB;
  muS         = _muS;

  chemicalPotentials.clear();
  particleDensities.clear();
  deltaN_bulk_term1s.clear();
  deltaN_bulk_term2s.clear();
  deltaN_qmu_term1s.clear();
  deltaN_qmu_term2s.clear();
  particleEnergies.clear();
  particlePressures.clear();
  particleEntropies.clear();
  particleDpOverTmu.clear();
  particleDeOverTDmu.clear();
  particleDnDmu.clear();
  particleDsDmu.clear();


  double mu;
  double numberDensity;
  double energyDensity;
  double entropyDensity;
  double pressure;

  double deltaN_bulk_term1;
  double deltaN_bulk_term2;
  double deltaN_qmu_term1;
  double deltaN_qmu_term2;

  double dsdmu;
  double dndmu;
  double deoverTdmu;
  double dPoverTdmu;
  double gSpin;
  double arg;
  double lambda;

  double beta = 1.0/temperature;
  double t2   = temperature*temperature;
  double sign;
  double mass;
  double mass2;
  double mass3;
  double mass4;
  double mbeta;
  double mbeta2;
  double mbeta3;
  double mbeta4;
  double hbarC  = 0.19733;
  double hbarC2 = hbarC*hbarC;
  double hbarC3 = hbarC2*hbarC;
  double twiceMPi2 = 2*M_PI*M_PI;
  double theta;
  double fugacity;
  double besselKn1;
  double besselKn2;
  double doubleJ;

  thermalNumberDensity         = 0.0;
  thermalEnergyDensity         = 0.0;
  thermalEntropyDensity        = 0.0;
  thermalPressure              = 0.0;
  thermalNetBaryonDensity      = 0.0;
  thermalNetStrangenessDensity = 0.0;
  thermalNetChargeDensity      = 0.0;

  unsigned int  nPart = particleTypes->size();
  for (unsigned int iPart=0; iPart < nPart; iPart++)
    {
    ParticleType & particleType = *(particleTypes->getParticleType(iPart));
    mass = particleType.getMass();

    if (mass>0.0)
      {
      gSpin     = particleType.getSpinFactor();
      sign      = particleType.getStatistics();
      mass2     = mass*mass;
      mass3     = mass2*mass;
      mass4     = mass3*mass;
      mbeta     = mass*beta;
      mbeta2    = mbeta*mbeta;
      mbeta3    = mbeta2*mbeta;
      mbeta4    = mbeta3*mbeta;
      mu        = muB*particleType.getBaryon() + muS*particleType.getStrange();

      numberDensity     = 0.0;
      deltaN_bulk_term1 = 0.0;
      deltaN_bulk_term2 = 0.0;
      deltaN_qmu_term1  = 0.0;
      energyDensity     = 0.0;
      pressure          = 0.0;
      dPoverTdmu        = 0.0;
      deoverTdmu        = 0.0;
      dndmu             = 0.0;
      for (int j = 1; j < trunOrder; j++)
        {
        doubleJ    = double(j);
        arg        = doubleJ*mbeta;  // argument inside bessel functions
        lambda     = exp(beta*mu);  // fugacity factor
        theta      = pow(-sign,  j-1);
        fugacity   = pow(lambda, j);
        besselKn1  = TMath::BesselK1(arg);
        besselKn2  = TMath::BesselK(2,arg);

        numberDensity     += theta*fugacity*besselKn2/doubleJ;
        deltaN_bulk_term1 += theta*fugacity*(mass*beta*besselKn1 + 3.0/doubleJ*besselKn2);
        deltaN_bulk_term2 += theta*fugacity*besselKn1;
        deltaN_qmu_term1  += theta*fugacity*besselKn2/doubleJ; // baryon diffusion contribution
        double I_1_1 = exp(-arg)/arg*(2./(arg*arg) + 2.0/arg - 0.5);
        double I_1_2 = 3./8.0*ROOT::Math::expint_n(2, arg);
        double I_1_n = I_1_1 + I_1_2;
        double double_factorial    = 1.;  // record (2k-5)!!
        double factorial           = 2.;         // record k! start with 2!
        double factor_2_to_k_power = 4.;      // record 2^k start with 2^2
        for (int k = 3; k < trunOrder; k++)
          {
          double_factorial *= (2*k - 5);
          factorial        *= k;
          factor_2_to_k_power *= 2;
          double rrr = ROOT::Math::expint_n(2*k-2, arg);
          double I_1_k = (3.*double_factorial/factor_2_to_k_power/factorial*rrr);
          I_1_n += I_1_k;
          }
        I_1_n = -mbeta3*I_1_n;
        deltaN_qmu_term2 += doubleJ*theta*fugacity*I_1_n;
        energyDensity += theta*fugacity*(3.*besselKn2/(arg*arg) + besselKn1/arg);
        pressure      += theta*fugacity*besselKn2/doubleJ/doubleJ;
        dPoverTdmu    += theta*fugacity*besselKn2/doubleJ;
        deoverTdmu    += theta*fugacity*doubleJ*(3*besselKn2/(arg*arg)+ besselKn1/arg);
        dndmu         += theta*fugacity*besselKn2;
        }
      numberDensity     = numberDensity*gSpin*mass2*temperature/twiceMPi2/hbarC3;
      deltaN_bulk_term1 = mass2*temperature*deltaN_bulk_term1;
      deltaN_bulk_term2 = 0.33333*mass3*deltaN_bulk_term2;
      deltaN_qmu_term1  = mass2*deltaN_qmu_term1/(beta*beta);
      deltaN_qmu_term2  = deltaN_qmu_term2/(3.0*beta*beta*beta);
      energyDensity     = energyDensity*gSpin*mass4/twiceMPi2/hbarC3;
      pressure          = pressure*gSpin*mass2*t2/twiceMPi2/hbarC3;
      entropyDensity    = (energyDensity + pressure - mu*numberDensity)*beta;    // unit : 1/fm^3
      dPoverTdmu        = dPoverTdmu*gSpin*mass2/twiceMPi2/hbarC3;
      deoverTdmu        = deoverTdmu*gSpin*mass4/twiceMPi2/hbarC3;
      dndmu             = dndmu*gSpin*mass/twiceMPi2;
      dsdmu             = dPoverTdmu - numberDensity - mu*dndmu + deoverTdmu;

      chemicalPotentials.push_back(mu);
      particleDensities.push_back(numberDensity);
      deltaN_bulk_term1s.push_back(deltaN_bulk_term1);
      deltaN_bulk_term2s.push_back(deltaN_bulk_term2);
      deltaN_qmu_term1s.push_back(deltaN_qmu_term1);
      deltaN_qmu_term2s.push_back(deltaN_qmu_term2);
      particleEnergies.push_back(energyDensity);
      particlePressures.push_back(pressure);
      particleEntropies.push_back(entropyDensity);
      particleDpOverTmu.push_back(dPoverTdmu);
      particleDeOverTDmu.push_back(deoverTdmu);
      particleDnDmu.push_back(dndmu);
      particleDsDmu.push_back(dsdmu);

      thermalNumberDensity  += numberDensity;
//      cout << setw(5)  <<  "iPart:" << setw(5)<< iPart;
//      cout << setw(25) <<  "NumberDensity:";
//      cout << scientific << setw(15)<< setprecision(5) << numberDensity;
//      cout << setw(25) <<  "ThermalNumberDensity:";
//      cout << scientific << setw(15)<< setprecision(5) << thermalNumberDensity << endl;

      thermalEnergyDensity  += energyDensity;
      thermalEntropyDensity += entropyDensity;
      thermalPressure       += pressure;
      if (particleType.isBaryon())   thermalNetBaryonDensity      += numberDensity * particleType.getBaryon();
      if (particleType.isCharged())  thermalNetChargeDensity      += numberDensity * particleType.getCharge();
      if (particleType.isStrange())  thermalNetStrangenessDensity += numberDensity * particleType.getStrange();
      }
    else
      {
      double zero = 0.0;
      chemicalPotentials.push_back(zero);
      particleDensities.push_back(zero);
      deltaN_bulk_term1s.push_back(zero);
      deltaN_bulk_term2s.push_back(zero);
      deltaN_qmu_term1s.push_back(zero);
      deltaN_qmu_term2s.push_back(zero);
      particleEnergies.push_back(zero);
      particlePressures.push_back(zero);
      particleEntropies.push_back(zero);
      particleDpOverTmu.push_back(zero);
      particleDeOverTDmu.push_back(zero);
      particleDnDmu.push_back(zero);
      particleDsDmu.push_back(zero);

      }
    }
  calculateParticleDecays();
  setupDecayGenerator();
  if (reportEnd("HadronGas", getName(), "calculateAllProperties(double _temp, double _muB, double _muS)" ) )
    ;
}

void HadronGas::setupDecayGenerator()
{
  hadronRndmSelector = new SelectionGenerator(particleDensities);
}

ParticleType * HadronGas::generateRandomHadron()
{
  int index = hadronRndmSelector->generate();
  return particleTypes->getParticleType(index);
}

int HadronGas::generateRandomHadronByIndex()
{
  return hadronRndmSelector->generate();
}

// ======================================================================
// computes dn/dp at given temperature, mass, mu, g, sign, and p
// ======================================================================
double HadronGas::computeThermalDensityVsP(double temperature, double mass, double mu, double g, double sign, double p)
{
  if (reportStart("HadronGas", getName(), "computeThermalDensityVsP(..)" ) )
    ;
  double e = sqrt(p*p+mass*mass);
  double prefactor = g/twoPiCube;
  double arg = (e-mu)/temperature;
  double density = prefactor*(p/e)/(exp(arg)+sign);
  if (reportEnd("HadronGas", getName(), "computeThermalDensityVsP(..)" ) )
    ;
  return density;
}
