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
 * Copyright 2022 Claude Pruneau
 *
 * *********************************************************************/
#ifndef CAP_ThermalGas
#define CAP_ThermalGas
#include "Task.hpp"
#include "ParticleDb.hpp"
#include "ParticleThermalProperties.hpp"
#include "SelectionGenerator.hpp"

using namespace std;

namespace CAP
{

class ThermalGas : public Task
{
protected:

  //!
  //!Collection of particles used in the Hadron Gas calculations
  //!
  ParticleDb * particleTypes;

  //!
  //!Collection of particles considered "stable" used in the Hadron Gas calculations
  //!
  ParticleDb * stableParticleTypes;

  //!
  //!Collection of all hadron type considered in the hadron gas
  //!computed with this instance of ThermalGas
  //!
  vector<ParticleThermalProperties*> hadrons;

  //!
  //!Temperature of the gas
  //!
  double temperature;

  //!
  //!Chemical potential of  the gas
  //!
  double mu;

  //!
  //!Baryon chemical potential of  the gas
  //!
  double muB;

  //!
  //!Strange chemical potential of  the gas
  //!
  double muS;

  //!
  //!Charge chemical potential of  the gas
  //!
  double muQ;

  //!
  //!Charm chemical potential of  the gas
  //!
  double muC;

  //!
  //!Thermal (total) number density of all the particles in this hadron gas
  //!
  double thermalNumberDensity;

  //!
  //!Thermal (total) net baryon density of all the particles in this hadron gas
  //!
  double thermalNetBaryonDensity;

  //!
  //!Thermal (total) net strangeness density of all the particles in this hadron gas
  //!
  double thermalNetStrangenessDensity;

  //!
  //!Thermal (total) net charge density of all the particles in this hadron gas
  //!
  double thermalNetChargeDensity;

  //!
  //!Thermal (total) energy density of all the particles in this hadron gas
  //!
  double thermalEnergyDensity;

  //!
  //!Thermal (total) entropy density of all the particles in this hadron gas
  //!
  double thermalEntropyDensity;

  //!
  //!Thermal (total) pressure associated with all the particles in this hadron gas
  //!
  double pressure;

public:

  vector<double> rho1_stable;
  vector< vector<double> > rho2_stable;
  vector< vector<double> > rho2cor_stable;
  vector< vector<double> > rho1rho1_stable;
  vector< vector<double> > c2_stable;

  SelectionGenerator * hadronRndmSelector;

  //!
  //!Task CTOR
  //!@param _name name given to this ThermalGas task
  //!@param _configuration configuration of the task
  //!@param _particles Pointer to "master" particle type collection. This collection will be used to compute the hadron gas properties.
  //!@param _stableParticles Pointer to "stable" particle collection.
  //!@param _debugLevel debug level to be used by this task
  //!
  ThermalGas(const String & _name,
            const Configuration &    _configuration,
            ParticleDb * _particles,
            ParticleDb * _stableParticles);

  virtual ~ThermalGas() {}

  //!
  //!Set the default configuration of this task
  //!
  virtual void setDefaultConfiguration();

  //!
  //!Initialize this hadron gas calculation
  //!
  virtual void initialize();

  //!
  //!Execute (perform) the hadron gas calculation: i.e, compute the properties of the hadron gas based on the
  //!selected value of temperature, baryon chemical potential, and strange chemical potential.
  //!
  virtual void execute();

  virtual void reset();
  virtual void clear();

  //!
  //!Calculate the decay probabilities of all "thermal" hadrons to the selected stable hadrons.
  //!
  virtual void calculateParticleDecayProbability();

  //!
  //!Calculate the decay probabilities of all "thermal" hadrons to the selected stable hadrons.
  //!
  virtual void calculateStableDensities();

  //!
  //!Get arrary containing all particle types used in this hadron gas
  //!
  ParticleDb & getParticleTypes()
  {
  return *particleTypes;
  }

  //!
  //!Get arrary containing all stable particle types used in this hadron gas
  //!
  ParticleDb & getStableParticleTypes()
  {
  return *stableParticleTypes;
  }

  vector<ParticleThermalProperties*> & getAllHadrons()
  {
  return hadrons;
  }

  ParticleType * getParticleTypeAt(int index);
  ParticleType * getStableParticleTypeAt(int index);
  ParticleThermalProperties * getParticleThermalPropertiesAt(int index);

//  void setupDecayGenerator();
//  ParticleType * generateRandomHadron();
//  int generateRandomHadronByIndex();

  //!
  //!Get  the temperature of this hadron gas
  //!
  double getTemperature() const{ return temperature; }

  //!
  //!Get  the chemical potential of this hadron gas
  //!
  double getMu()  const { return mu; }

  //!
  //!Get  the baryon chemical potential of this hadron gas
  //!
  double getMuB() const { return muB; }

  //!
  //!Get  the strangeness chemical potential of this hadron gas
  //!
  double getMuS() const { return muS; }

  //!
  //!Get  the charge chemical potential of this hadron gas
  //!
  double getMuQ() const { return muQ; }

  //!
  //!Get  the charge chemical potential of this hadron gas
  //!
  double getMuC() const { return muC; }

  //!
  //!Get  the particle number density associated with all the particles in this hadron gas
  //!
  double getNumberDensity()    const{ return thermalEnergyDensity; }

  //!
  //!Get  the energy density associated with all the particles in this hadron gas
  //!
  double getEnergyDensity()    const{ return thermalEnergyDensity; }

  //!
  //!Get  the entropy density associated with all the particles in this hadron gas
  //!
  double getEntropyDensity()   const{ return thermalEntropyDensity; }

  //!
  //!Get  (total) pressure associated with all the particles in this hadron gas
  //!
  double getPressure()         const{ return pressure; }

  //!
  //!Print properties of this hadron gas
  //!
  ostream & printProperties(ostream & os);

  ClassDef(ThermalGas,0)
};

} // namespace CAP

#endif  // CAP_ThermalGas

