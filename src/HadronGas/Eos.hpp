#ifndef CAP_Eos
#define CAP_Eos
#include <iostream>
#include <iomanip>
#include <string>
#include "TString.h"

using namespace std;

class Eos
{
private:
  static double hbarC;     // GeV*fm
  static int    trunOrder; // truncated order in the summation

  TString modelName;
  double gSpin; // degeneracy
  double mu;    // chemical potentials
  double mass;
  double temperature;
  double numberDensity;
  double energyDensity;
  double entropyDensity;
  double pressure;
  double dPoverTdmu;
  double deoverTdmu;
  double dndmu;
  double dsdmu;
  double deltaN_bulk_term1;
  double deltaN_bulk_term2;
  double deltaN_qmu_term1;
  double deltaN_qmu_term2;

public:
  Eos(const TString & _name, double gSpin, double _mass);
  Eos(const Eos & source);
  virtual ~Eos();
  Eos & operator=(const Eos & source);

  virtual double calculateProperties(double temperature, double mu);
  virtual double calculateNumberDensity(double temperature, double mu);
  virtual double calculateEnergyDensity(double temperature);
  virtual double calculateEntropyDensity(double temperature);
  virtual double calculatePressure(double temperature);
  virtual double calculate_dsdmu(double temperature);
  virtual double calculate_deoverTdmu(double temperature);
  virtual double calculate_dPoverTdmu(double temperature);
  virtual double calculate_dndmu(double temperature);
  ostream & printProperties(ostream & os);

  ClassDef(Eos,0)
};

#endif  // CAP_Eos

