// Adapted to root envinromemt 2021 C. Pruneau
//
// Copyright 2016 Chun Shen
#ifndef WAC_HadronGas
#define WAC_HadronGas
#include <iostream>
#include "TString.h"
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include "MessageLogger.hpp"
#include "ParticleTypeCollection.hpp"

using namespace std;

namespace CAP
{

class HadronGas : public MessageLogger
{
public:
  ParticleTypeCollection * particleTypes;
  ParticleTypeCollection * stableParticleTypes;

  static double twoPi;
  static double twoPiCube;
  static int trunOrder; //  = 10;

  double temperature;
  double mu;
  double muB;
  double muS;
  double muQ;
  double thermalNumberDensity;
  double thermalNetBaryonDensity;
  double thermalNetStrangenessDensity;
  double thermalNetChargeDensity;
  double thermalEnergyDensity;
  double thermalEntropyDensity;
  double thermalPressure;

  vector<double> stableParticleDensities;
  vector< vector<double> > stableParticlePairDensities;
  vector< vector<double> > stableParticlePairCorrelatedDensities;
  vector< vector<double> > stableParticlePairUncorrelatedDensities;
  SelectionGenerator * hadronRndmSelector;
  String name;
  
public:
  HadronGas(ParticleTypeCollection * _particles,
            ParticleTypeCollection * _stableParticles);
  virtual ~HadronGas();
  virtual void clear();
  virtual void calculateParticleDecayProbability();
  virtual void calculateParticleDecays();
  virtual void calculateAllProperties(double temperature, double muB, double muS);
  virtual double computeThermalDensityVsP(double temperature, double mass, double mu, double g, double sign, double p);

  ParticleTypeCollection & getParticleTypes()
  {
  return *particleTypes;
  }

  ParticleTypeCollection & getStableParticleTypes()
  {
  return *stableParticleTypes;
  }

  Configuration & getName() const
  {
  return name;
  }

  void  setName(String & _name)
  {
  name = _name;
  }

  ParticleType * getParticleType(int index);
  ParticleType * getStableParticleType(int index);
  void setupDecayGenerator();
  ParticleType * generateRandomHadron();
  int generateRandomHadronByIndex();

  double getTemperature() const{ return temperature; }
  double getMu()  const { return mu; }
  double getMuB() const { return muB; }
  double getMuS() const { return muS; }
  double getMuQ() const { return muQ; }
  double getNumberDensity()    const{ return thermalEnergyDensity; }
  double getEnergyDensity()    const{ return thermalEnergyDensity; }
  double getEntropyDensity()   const{ return thermalEntropyDensity; }
  double getPressure()         const{ return thermalPressure; }
  ostream & printProperties(ostream & os);
  ClassDef(HadronGas,0)
};

} // namespace CAP

#endif  // WAC_HadronGas

