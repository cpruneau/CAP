#ifndef CAP_EosHadronGas
#define CAP_EosHadronGas
#include <iostream>
#include <iomanip>
#include <string>
#include "TString.h"
#include "EosHadronGas.hpp"

class EosHadronGas : public Eos
{
public:
  EosHadronGas();
  EosHadronGas(const EosHadronGas & source);
  virtual ~EosHadronGas();
  EosHadronGas & operator=(const EosHadronGas & source);

  virtual double calculateNumberDensity(double temperature, double mu);
  virtual double calculateEnergyDensity(double temperature);
  virtual double calculateEntropyDensity(double temperature);
  virtual double calculatePressure(double temperature);
  virtual double calculate_dsdmu(double temperature);
  virtual double calculate_deoverTdmu(double temperature);
  virtual double calculate_dPoverTdmu(double temperature);
  virtual double calculate_dndmu(double temperature);

  ClassDef(EosHadronGas,0)
};

#endif  // CAP_EosHadronGas

