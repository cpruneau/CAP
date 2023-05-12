#ifndef WAC_HadronType
#define WAC_HadronType
#include "ParticleType.h"

class HadronType
{
private:
  static double hbarC;     // GeV*fm
  static double pionMass;  // Monte-Carlo number according PDG
  static int    trunOrder; // truncated order in the summation

  ParticleType * type;
  double mu;
  double temperature;
  double density;        // HadronType thermal yield at given T and mu
  double stableDensity;  // HadronType yield after decays
  double energyDensity;
  double entropyDensity;
  double pressure;       // thermodynamic quantities at given T and mu
  double dPoverTdmu;
  double deoverTdmu;
  double dndmu;
  double dsdmu;
  // array to record HadronType decay probability
  // for the HadronType decays into stable ones
  double* decayProbability;
  double** decayProbabilityPairs;

public:
  HadronType();
  HadronType(const HadronType & source);
  virtual ~HadronType();
  HadronType & operator=(const HadronType & source);

  void setParticleType(ParticleType * _type) { type = _type; }
  void setMu(double _mu)                     { mu   = _mum;  }
  void setTemperature(double _temperature)   { temperature = _temperature;}

  void createDecayProbabilityArray(int nStable);
  void calculateChemicalPotential(double muB, double muS);
  void calculateParticleDensity(double temperature, double muB, double muS);
  void calculateEnergyDensity(double temperature);
  void calculatePressure(double temperature);
  void calculateEntropyDensity(double temperature);
  void computeDensityAt(double p);
  void computeDensityAt(double p,double temperature,double mu);
  void calculate_dsdmu(double temperature);
  void calculate_deoverTdmu(double temperature);
  void calculate_dPoverTdmu(double temperature);
  void calculate_dndmu(double temperature);

  void setStableParticleYield(double yield_in) {stableYield = yield_in;}
  double getStableParticleYield() {return(stableYield);}
  void getBulkViscosityCoefficients(double Tdec, double* bulkvisCoefficients);

  ParticleType * getParticleType(e) { return type; }
  double getMu() const { return mu;}
  double getTemperature() const { return temperature; }
  double getDecayProbability(int i) const {return(decayProbability[i]);}
  double getDecayProbability(int i,int j)  const {return(decayProbabilityPairs[i][j]);}

  void setDecayProbability(int i, double val)
  {
  decayProbability[i] = val;
  }
  void setDecayProbability(int i, int j, double val)
  {
  decayProbabilityPairs[i][j] = val;
  }

  ostream & printProperties(ostream & os);

  ClassDef(HadronType,0)
};

#endif  // SRC_Hadron_H_

