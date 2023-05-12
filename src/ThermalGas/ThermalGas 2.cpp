/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 * Copyright 2016 Chun Shen
 // Copyright 2022 Claude Pruneau
 *
 * *********************************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <TMath.h>
#include "ThermalGas.hpp"
#include "ThermalGasHistograms.hpp"
#include "Math/SpecFunc.h"
#include "SelectionGenerator.hpp"
using CAP::ThermalGas;
using CAP::ParticleType;
using CAP::ParticleDb;

ClassImp(ThermalGas);

using namespace std;

ThermalGas::ThermalGas(const String & _name,
                     const Configuration &    _configuration,
                     ParticleDb * _particles,
                     ParticleDb * _stableParticles)
:
Task(_name,_configuration),
particleTypes(_particles),
stableParticleTypes(_stableParticles),
hadrons(),
temperature(0.0),
mu(0.0),
muB(0.0),
muS(0.0),
muQ(0.0),
thermalNumberDensity(0.0),
thermalNetBaryonDensity(0.0),
thermalNetStrangenessDensity(0.0),
thermalNetChargeDensity(0.0),
thermalEnergyDensity(0.0),
thermalEntropyDensity(0.0),
pressure(0.0),
hadronRndmSelector(nullptr)
{
  appendClassName("ThermalGas");
}

void ThermalGas::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  addParameter("UseParticles",          false);
  addParameter("EventsUseStream0",       false);
  addParameter("HistogramsCreate",      false);
  addParameter("HistogramsExport",        false);
  addParameter("Temperature",           150.0);
  addParameter("MuB",                   0.0);
  addParameter("MuS",                   0.0);
  addParameter("Volume",                1.0);
}

void ThermalGas::initialize()
{
  if (reportStart(__FUNCTION__) )
;
  reset();
  temperature = getValueDouble("Temperature");
  muB         = getValueDouble("MuB");
  muS         = getValueDouble("MuS");

  if (reportDebug(__FUNCTION__) )
    {
    cout << endl;
    cout << " ======================initialize=============="  << endl;
    cout << " Temperature =" <<  temperature << endl;
    cout << " ======================initialize=============="  << endl;
    }

  for (unsigned int iParticle = 0; iParticle<particleTypes->size(); iParticle++)
    {
    ParticleThermalProperties * p = new ParticleThermalProperties();
    p->setParticleType(particleTypes->getParticleType(iParticle));
    hadrons.push_back(p);
    }

  if (reportEnd(__FUNCTION__) )
;
}

void ThermalGas::calculateParticleDecayProbability()
{
  if (reportStart(__FUNCTION__) )
;
  int nPart       = hadrons.size();
  int nStablePart = stableParticleTypes->size();
  if (reportInfo(__FUNCTION__) )
    {
    cout << std::endl;
    cout << "==================================================================================" << std::endl;
    cout << "        nPart: " << nPart << endl;
    cout << "  nStablePart: " << nStablePart  << endl;
    cout << "==================================================================================" << std::endl;
    }

  // Setup decay probability arrays
  for (int iPart = 0; iPart < nPart; iPart++)
    {
    ParticleThermalProperties & hadron = *(hadrons[iPart]);
    hadron.createDecayProbabilityArray(nStablePart);
    ParticleType & particleType = hadron.getType();
    if (particleType.isStable() && !particleType.isPhoton())
      {
      int indexStable = stableParticleTypes->findIndexForPdgCode(particleType.getPdgCode());
      if (indexStable<0 || indexStable>=nStablePart)
        {
        if (reportError(__FUNCTION__) )
          {
          cout << "==================================================================================" << std::endl;
          cout << "iPart: " << iPart << " indexStable:" << indexStable << " --- LOGIC ERROR - ABORT" << endl;
          cout << "Terminate task."  << endl;
          cout << "==================================================================================" << std::endl;
          exit(1);
          }
        }
      // this hadron is stable and thus has a unit
      // probability of decaying into itself.
      hadron.setDecayProbability(indexStable,1.0);
      }
    }

  vector<double> temp(nStablePart,0.0);
  vector< vector<double> > temp2(nStablePart,temp);

  // Calculate decayProbabilities
  for (int iPart = 0; iPart < nPart; iPart++)
    {
    ParticleThermalProperties & hadron = *(hadrons[iPart]);
    int nDecayModes = hadron.getNDecayModes();
    if (nDecayModes==0) continue;
    vector<ParticleThermalProperties*> ptrToDecayHadron(5,nullptr);
    for (int jChannel = 0; jChannel<nDecayModes; jChannel++)
      {
      ParticleDecayMode & decayMode = hadron.getDecayMode(jChannel);
      double branchingRatio = decayMode.getBranchingRatio();
      int    nChildren      = decayMode.getNChildren();
      for (int kHadron = 0; kHadron<nChildren; kHadron++)
        {
        int pdgCode = decayMode.getChildPdgCode(kHadron);
        ParticleThermalProperties* decayHadron =nullptr; // = hadrons.findPdgCode(pdgCode);
        for (unsigned int k=0;k<hadrons.size(); k++)
          {
          if (hadrons[k]->getType().getPdgCode() == pdgCode)
            {
            decayHadron = hadrons[k];
            }
          }
        if (decayHadron==nullptr)
          {
          if (reportError(__FUNCTION__) )
            {
            cout << "==================================================================================" << std::endl;
            cout << "Could not find PDG code: " << pdgCode << endl;
            cout << "Terminate task."  << endl;
            cout << "==================================================================================" << std::endl;
            exit(1);
            }
          }
        ptrToDecayHadron[kHadron] = decayHadron;
        }
      // now we have a local list of the pdg ids iPart decays into (w/o photons)
      // must look at singles and then at pairs
      // if there are no particleTypes left, just skip this decay channel..
      double prob1, prob2;

      for (int kHadron1 = 0; kHadron1 < nChildren; kHadron1++)
        {
        ParticleThermalProperties & hadron1 =  * (ptrToDecayHadron[kHadron1]);
        for (int mStable1 = 0; mStable1 < nStablePart; mStable1++)
          {
          prob1 = hadron1.getDecayProbability(mStable1);
          temp[mStable1] += branchingRatio*prob1;
          for (int kHadron2=0; kHadron2<nChildren; kHadron2++)
            {
            if (kHadron1 != kHadron2)
              {
              ParticleThermalProperties & hadron2 =  * (ptrToDecayHadron[kHadron2]);
              for (int mStable2 = 0; mStable2 < nStablePart; mStable2++)
                {
                prob2 = hadron2.getDecayProbability(mStable2);
                temp2[mStable1][mStable2] += branchingRatio*prob1*prob2;
                }
              }
            }

          for (int mStable3 = 0; mStable3 < nStablePart; mStable3++)
            {
            temp2[mStable1][mStable3] += branchingRatio * hadron1.getDecayProbability(mStable1,mStable3);
            }
          }
        }
      }

    // Store results
    for (int iStable=0;iStable<nStablePart;iStable++)
      {
      hadron.setDecayProbability(iStable,temp[iStable]);
      for (int jStable=0;jStable<nStablePart;jStable++)
        {
        hadron.setDecayProbability(iStable,jStable,temp2[iStable][jStable]);
        }
      }
    }
  if (reportEnd(__FUNCTION__) )
;
}

//!
//!
void ThermalGas::calculateStableDensities()
{
  if (reportStart(__FUNCTION__) )
;
  int nHadrons = hadrons.size();
  int nStablePart = stableParticleTypes->size();

  //
  // single particle density: rho1_stable
  //
  for (int jStable = 0; jStable < nStablePart;jStable++)
    {
    double rho1 = 0.0;
    for (int kHadron=0; kHadron<nHadrons; kHadron++)
      {
      ParticleThermalProperties & hadron = *(hadrons[kHadron]);
      rho1 += hadron.getDecayProbability(jStable) * hadron.getNumberDensity();
      }
    rho1_stable[jStable] = rho1;
    }

  //
  // product of single particle densities: rho1rho1_stable
  //
  for (int jStable1 = 0; jStable1 < nStablePart; jStable1++)
    {
    double rho1_1 = rho1_stable[jStable1];
    for (int jStable2 = 0; jStable2 < nStablePart; jStable2++)
      {
      double rho1_2 = rho1_stable[jStable2];
      rho1rho1_stable[jStable1][jStable2] = rho1_1*rho1_2;
      }
    }

  //
  // pair densities: rho2_stable
  // correlated pair density
  //
  for (int jStable1 = 0; jStable1 < nStablePart; jStable1++)
    {
    for (int jStable2 = 0; jStable2 < nStablePart; jStable2++)
      {
      double rho2  = 0.0;
      double rho2c = 0.0;
      for (int kHadron1=0; kHadron1<nHadrons; kHadron1++)
        {
        ParticleThermalProperties & hadron1 = *(hadrons[kHadron1]);
        double prob1     = hadron1.getDecayProbability(jStable1);
        double prob12    = hadron1.getDecayProbability(jStable1,jStable2);
        double density1  = hadron1.getNumberDensity();
        double rho1_1    = density1*prob1;
        rho2c +=  density1*prob12;
        for (int kHadron2=0; kHadron2<nHadrons; kHadron2++)
          {
          ParticleThermalProperties & hadron2 = *(hadrons[kHadron2]);
          double prob2     = hadron2.getDecayProbability(jStable2);
          double density2  = hadron2.getNumberDensity();
          double rho1_2    = density2*prob2;
          rho2 += rho1_1 * rho1_2;
          }
        }
      rho2cor_stable[jStable1][jStable2] = rho2c;
      rho2_stable[jStable1][jStable2]    = rho2;
      }
    }

  //
  // pair cumulant rho2-rho1rho1
  //
  for (int jStable1 = 0; jStable1 < nStablePart; jStable1++)
    {
    for (int jStable2 = 0; jStable2 < nStablePart; jStable2++)
      {
      double rho2     = rho2_stable[jStable1][jStable2];
      double rho1rho1 = rho1rho1_stable[jStable1][jStable2];
      c2_stable[jStable1][jStable2] = rho2 - rho1rho1;
      }
    }
  if (reportEnd(__FUNCTION__) )
;
}


void ThermalGas::reset()
{
  temperature = 0.0;
  muB         = 0.0;
  muS         = 0.0;
  mu          = 0.0;
  thermalNumberDensity         = 0.0;
  thermalEnergyDensity         = 0.0;
  thermalEntropyDensity        = 0.0;
  pressure                     = 0.0;
  thermalNetBaryonDensity      = 0.0;
  thermalNetStrangenessDensity = 0.0;
  thermalNetChargeDensity      = 0.0;
  unsigned int nStableHadrons  = stableParticleTypes->size();
  for (unsigned int iHadron=0; iHadron<hadrons.size();iHadron++) hadrons[iHadron]->reset();
  vector<double> array(nStableHadrons,0.0);
  rho1_stable = array;
  rho2_stable.assign(nStableHadrons,array);
  rho2cor_stable.assign(nStableHadrons,array);
  rho1rho1_stable.assign(nStableHadrons,array);
  c2_stable.assign(nStableHadrons,array);
}

void ThermalGas::clear()
{
  temperature = 0.0;
  muB         = 0.0;
  muS         = 0.0;
  mu          = 0.0;
  thermalNumberDensity         = 0.0;
  thermalEnergyDensity         = 0.0;
  thermalEntropyDensity        = 0.0;
  pressure                     = 0.0;
  thermalNetBaryonDensity      = 0.0;
  thermalNetStrangenessDensity = 0.0;
  thermalNetChargeDensity      = 0.0;
  for (unsigned int iHadron=0; iHadron<hadrons.size();iHadron++) hadrons[iHadron]->clear();
  hadrons.clear();
  rho1_stable.clear();
  rho2_stable.clear();
  rho2cor_stable.clear();
  rho1rho1_stable.clear();
  c2_stable.clear();
//  for (int iStable=0; iStable<nStableHadrons; iStable)
//    {
//    rho2_stable.push_back(array);
//    rho2cor_stable.push_back(array);
//    rho1rho1_stable.push_back(array);
//    c2_stable.push_back(array);
//    }
}

void ThermalGas::execute()
{
  if (reportStart(__FUNCTION__))
;

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << " ============================================================"  << endl;
    cout << " temperature =" <<  temperature << endl;
    cout << " ============================================================"  << endl;
    }
  for (unsigned int iHadron=0; iHadron < hadrons.size(); iHadron++)
    {
    if (hadrons[iHadron]->getMass()<=0.0) continue;
    hadrons[iHadron]->calculateAllProperties(temperature, muB, muS);
    thermalNumberDensity  += hadrons[iHadron]->numberDensity;
    thermalEnergyDensity  += hadrons[iHadron]->energyDensity;
    thermalEntropyDensity += hadrons[iHadron]->entropyDensity;
    pressure              += hadrons[iHadron]->pressure;
    thermalNetBaryonDensity      += hadrons[iHadron]->getBaryonDensity();
    thermalNetChargeDensity      += hadrons[iHadron]->getChargeDensity();
    thermalNetStrangenessDensity += hadrons[iHadron]->getStrangeDensity();
    }
  //return;
  calculateParticleDecayProbability();
  calculateStableDensities();
  //setupDecayGenerator();
  if (reportEnd(__FUNCTION__))
;
}

// this needs to be "fixed"
//void ThermalGas::setupDecayGenerator()
//{
//  //hadronRndmSelector = new SelectionGenerator(particleDensities);
//}

// this needs to be "fixed"
//ParticleType * ThermalGas::generateRandomHadron()
//{
//  int index = 0; // hadronRndmSelector->generate();
//  return particleTypes->getParticleType(index);
//}

// this needs to be "fixed"
//int ThermalGas::generateRandomHadronByIndex()
//{
//  return 0; // hadronRndmSelector->generate();
//}

// ======================================================================
// computes dn/dp at given temperature, mass, mu, g, sign, and p
// ======================================================================
//double ThermalGas::computeThermalDensityVsP(double temperature, double mass, double mu, double g, double sign, double p)
//{
//  if (reportStart(__FUNCTION__))
//;
//  double e = sqrt(p*p+mass*mass);
//  double prefactor = g/twoPiCube;
//  double arg = (e-mu)/temperature;
//  double density = prefactor*(p/e)/(exp(arg)+sign);
//  if (reportEnd(__FUNCTION__) )
//;
//  return density;
//}


ostream & ThermalGas::printProperties(ostream & os)
{
  os << "===================================================================================================================" << std::endl;
  os << "===================================================================================================================" << std::endl;
  os << " Thermal Gas : " << getName()  << endl;
  os << "===================================================================================================================" << std::endl;
  printItem("temperature",                     temperature, os);
  printItem("mu",                              mu, os);
  printItem("muB",                             muB, os);
  printItem("muS",                             muS, os);
  printItem("muQ",                             muQ, os);
  printItem("muC",                             muC, os);
  printItem("Thermal Number Density",          thermalNumberDensity,    os);
  printItem("Thermal Net Baryon Density",      thermalNetBaryonDensity, os);
  printItem("Thermal Net Strangeness Density", thermalNetStrangenessDensity, os);
  printItem("Thermal Net Charge Density",      thermalNetChargeDensity, os);
  printItem("Thermal Energy Density",          thermalEnergyDensity,    os);
  printItem("Thermal Entropy Density",         thermalEntropyDensity,   os);
  printItem("Pressure",                        pressure, os);

  os << "===================================================================================================================" << std::endl;
  os << " Particle Thermal Properties" << endl;
  os << "===================================================================================================================" << std::endl;

  os <<  fixed << setw(20) << "Name";
  os <<  fixed << setw(15) << "Mu";
  os <<  fixed << setw(15) << "N-Density";
  os <<  fixed << setw(15) << "E-Density";
  os <<  fixed << setw(15) << "S-Density";
  os <<  fixed << setw(15) << "Pressure";
  os <<  fixed << setw(15) << "dPoverTdmu";
  os <<  fixed << setw(15) << "deoverTdmu";
  os <<  fixed << setw(15) << "dndmu";
  os <<  fixed << setw(15) << "dsdmu";
  os << endl;

  for (unsigned int iHadron=0; iHadron<hadrons.size(); iHadron++)
    {
    hadrons[iHadron]->printProperties(os);
    }
  return os;
}
