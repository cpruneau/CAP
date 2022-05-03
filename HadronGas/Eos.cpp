// Copyright 2021 Claude Pruneau
// Copyright 2016 Chun Shen
// This code is adapted from Chun's code
// for the purpose of computing Eosic resonance
// correlations...

//#include <gsl/gsl_sf_bessel.h>
//#include <gsl/gsl_sf_expint.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <TMath.h>
#include "Math/SpecFunc.h"
using namespace std;
#include "Eos.hpp"

ClassImp(Eos);

double Eos::hbarC     = 0.19733;
int    Eos::trunOrder = 10;

Eos::Eos(const TString & name, double _gSpin, double _mass)
:
modelName(""),
gSpin(_gSpin),
mu(0.0),
mass(_mass),
temperature(0.0),
numberDensity(0.0),
energyDensity(0.0),
entropyDensity(0.0),
pressure(0.0),
dPoverTdmu(0.0),
deoverTdmu(0.0),
dndmu(0.0),
dsdmu(0.0),
deltaN_bulk_term1(0.0),
deltaN_bulk_term2(0.0),
deltaN_qmu_term1(0.0),
deltaN_qmu_term2(0.0)
{}

Eos::Eos(const Eos & source)
:
modelName(source.modelName),
gSpin(source.gSpin),
mu(source.muQ),
mass(source.mass),
temperature(source.temperature),
numberDensity(source.numberDensity),
energyDensity(source.energyDensity),
entropyDensity(source.entropyDensity),
pressure(source.pressure),
dPoverTdmu(source.dPoverTdmu),
deoverTdmu(source.deoverTdmu),
dndmu(source.dndmu),
dsdmu(source.dsdmu),
deltaN_bulk_term1(source.deltaN_bulk_term1),
deltaN_bulk_term2(source.deltaN_bulk_term2),
deltaN_qmu_term1(source.deltaN_qmu_term1),
deltaN_qmu_term2(source.deltaN_qmu_term2)
{}

Eos::~Eos()
{}

Eos & Eos::operator=(const Eos & source)
{
  if (this!=&source)
    {
    modelName = source.modelName;
    gSpin = source.gSpin;
    mu    = source.muQ;
    mass  = source.mass;
    temperature    = source.temperature;
    numberDensity  = source.numberDensity;
    energyDensity  = source.energyDensity;
    entropyDensity = source.entropyDensity;
    pressure    = source.pressure;
    dPoverTdmu  = source.dPoverTdmu;
    deoverTdmu  = source.deoverTdmu;
    dndmu       = source.dndmu;
    dsdmu       = source.dsdmu;
    deltaN_bulk_term1 = source.deltaN_bulk_term1;
    deltaN_bulk_term2 = source.deltaN_bulk_term2;
    deltaN_qmu_term1  = source.deltaN_qmu_term1;
    deltaN_qmu_term2  = source.deltaN_qmu_term2;
    }
  return *this;
}

void Eos::calculateProperties(double t, double chemicalPotential)
{
  temperature = t;
  mu = chemicalPotential;
  calculateNumberDensity(temperature,mu);
  calculateEnergyDensity(temperature);
  calculateEntropyDensity(temperature);
  calculatePressure(temperature);
  calculate_dsdmu(temperature);
  calculate_deoverTdmu(temperature);
  calculate_dPoverTdmu(temperature);
  calculate_dndmu(temperature);
}

// Base class does nothing!
void Eos::calculateNumberDensity(double temperature, double mu) {}
void Eos::calculateEnergyDensity(double temperature) {}
void Eos::calculateEntropyDensity(double temperature) {}
void Eos::calculatePressure(double temperature) {}
void Eos::calculate_dsdmu(double temperature) {}
void Eos::calculate_deoverTdmu(double temperature) {}
void Eos::calculate_dPoverTdmu(double temperature) {}
void Eos::calculate_dndmu(double temperature) {}

ostream & Eos::printProperties(ostream & os)
{
  os <<  fixed << setw(10) << setprecision(8) << gSpin;
  os <<  fixed << setw(10) << setprecision(8) << mass;
  os <<  fixed << setw(10) << setprecision(8) << mu;
  os <<  fixed << setw(10) << setprecision(8) << temperature;
  os <<  scientific << setw(20) << setprecision(5) <<  numberDensity;
  os <<  scientific << setw(20) << setprecision(5) <<  energyDensity;
  os <<  scientific << setw(20) << setprecision(5) <<  entropyDensity;
  os <<  scientific << setw(20) << setprecision(5) <<  pressure;
  os <<  scientific << setw(20) << setprecision(5) <<  dPoverTdmu;
  os <<  scientific << setw(20) << setprecision(5) <<  deoverTdmu;
  os <<  scientific << setw(20) << setprecision(5) <<  dndmu;
  os <<  scientific << setw(20) << setprecision(5) <<  dsdmu;
  os <<  scientific << setw(20) << setprecision(5) <<  deltaN_bulk_term1;
  os <<  scientific << setw(20) << setprecision(5) <<  deltaN_bulk_term2;
  os <<  scientific << setw(20) << setprecision(5) <<  deltaN_qmu_term1;
  os <<  scientific << setw(20) << setprecision(5) <<  deltaN_qmu_term2;
  return os;
}

