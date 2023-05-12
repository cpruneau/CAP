#ifndef CAP_ParticleThermalProperties
#define CAP_ParticleThermalProperties
#include "ParticleType.hpp"

namespace CAP
{


//!
//!Thermodynamic properties (basedon hadron gas model) of the hadron species associated with this instance i.e., particle type pointed at by the variable "type".
//!The pointer to ParticleType must be set explicitly after instantiation of this class with the setType() method.
//!Calculation of the thermodynamic properties is carried out by the calculateAllProperties(double _temp, double _muB, double _muS) method for the given
//!temperature, baryon chemical potential, and strangeness chemical potential.
//!
//!Input parameters defining the thermal properties of this hadron are the following hadron gas properties (supplied in the call to calculateAllProperties)
//!-T: temperature
//!-muB: baryon chemical potential
//!-muS: stranges chemical potential
//!The baryon and strangeness chemical potentials are used to compute the particle's chemical potential according to
//!- mu = muB * baryon  + muS * strangeness
//!in which baryon represents the baryon number (+1, 0, -1) and strangeness (-3, -2, -1, 0, 1, 2, 3) of this species.
//!
//!Quantities computed for each hadron species include
//!-numberDensity thermal number density of this species (at T, mu)
//!-stableNumberDensity density of this particle after decay
//!-energyDensity energy density of this particle species at T, mu.
//!-entropyDensity entropy density of this particle species at T, mu.
//!-pressure partial pressure of this particle species at T, mu.
//!-dPoverTdmu (1/T)dP/dmu of this HG particle
//!-deoverTdmu (1/T)dE/dmu of this HG particle
//!-dndmu dN/dmu of this HG particle
//!-dsdmu ds/dmu of this HG particle
//!-deltaN_bulk_term1;
//!-deltaN_bulk_term2;
//!-deltaN_qmu_term1s;
//!-deltaN_qmu_term2s;
//!
//!This class also holds arrays to keep track of the decay probability  of this species into "stable" hadrons (stored in decayProbability)
//! and stable pairs of hadrons (stored in decayProbabilityPairs).
//!
//!The class definition includes a number of non-essential variables to enable (a) faster calculations and (b) easier code readability.
//!Such convenience variables include the following (double)
//!-beta == 1/temperature
//!-mass == mass of the particle copied from ParticleType to avoid frequent dereference.
//!-massBeta == mass*beta
//!-muBeta == mu*beta
//!-spinFactor == spin degeneracy factor of this particle species
//!-sign == statistics of this species.
//!
//!
class ParticleThermalProperties
{
private:
  static double hbarC;     //! h bar * c  GeV*fm
  static double hbarC2;    //! (h bar * c  GeV*fm)^2
  static double hbarC3;    //! (h bar * c  GeV*fm)^3
  static double hbarC4;    //! (h bar * c  GeV*fm)^4
  static int    trunOrder; //! truncated order in the summation of Bessel functions

  static double pi;        //! value of pi (from TMath::Pi()) copied here for speed and convenience
  static double piSquare;  //!  pi*pi
  static double piCube;    //!  pi*pi*pi

public:
  //!
  //!Particle type (\see class ParticleType)
  //!
  ParticleType * type;

  //!
  //!thermal number densityof this HG particle
  //!
  double numberDensity;        // ParticleThermalProperties thermal yield at given T and mu

  //!
  //!thermal energy densityof this HG particle
  //!
  double energyDensity;

  //!
  //!thermal entropy densityof this HG particle
  //!
  double entropyDensity;

  //!
  //!thermal partial pressure of this HG particle
  //!
  double pressure;       // thermodynamic quantities at given T and mu

  //!
  //!(1/T)dP/dmu of this HG particle
  //!
  double dPoverTdmu;

  //!
  //!(1/T)dE/dmu of this HG particle
  //!
  double deoverTdmu;

  //!
  //!dN/dmu of this HG particle
  //!
  double dndmu;

  //!
  //!ds/dmu of this HG particle
  //!
  double dsdmu;

  double prefactor;
  double deltaN_bulk_term1;
  double deltaN_bulk_term2;
  double deltaN_qmu_term1;
  double deltaN_qmu_term2;

  //!
  //!Array contains the decay probability of this thermal hadron species into
  //!the hadrons considered  stable for the purpose of the calculation.
  //!
  vector<double> decayProbability;

  //!
  //!Array contains the decay probability of this thermal hadron species into
  //!specific pair (i,j) of hadrons considered  stable for the purpose of the calculation.
  //!
  vector<vector<double> > decayProbabilityPairs;

public:

  //!
  //! Default CTOR
  //!
  ParticleThermalProperties();
  ParticleThermalProperties(ParticleType * _type);

  //!
  //! Copy CTOR: Performs a shallow copy. ParticleType type is copied as a pointer to the original (and unique) instance of ParticleType for this species.
  //!
  ParticleThermalProperties(const ParticleThermalProperties & source);

  virtual ~ParticleThermalProperties() {}

  //!
  //! Assignment operator
  //!
  ParticleThermalProperties & operator=(const ParticleThermalProperties & source);

  //!
  //! Reset all the ThermalGas properties of this particle but not its identity
  //!
  void reset();

  //!
  //! Reset all the ThermalGas properties of this particle including its identity
  //!
  void clear();

  //!
  //! Computes all thermodynamic properties of this particle at the given temperature, baryon chemical potential
  //! and strange chemical potential. This code copied verbatim from Chun Shen. Formula are from PRC XXXX.
  //! @param _temp temperature of the hadron gas
  //! @param _muB baryon chemical potential of the hadron gas
  //! @param _muS strange chemical potential of the hadron gas
  //!
  void calculateAllProperties(double _temp, double _muB, double _muS);

  //!
  //!stable  number densityof this HG particle (after all decays)
  //!
  double getNumberDensity() const
  {
  return numberDensity;
  }

  //!
  //!thermal energy densityof this HG particle
  //!
  double getEnergyDensity() const
  {
  return energyDensity;
  }

  //!
  //!thermal entropy densityof this HG particle
  //!
  double getEntropyDensity() const
  {
  return entropyDensity;
  }

  //!
  //!thermal partial pressure of this HG particle
  //!
  double getPressure() const
  {
  return pressure;
  }

  //!
  //! get the baryon number of this particle.
  //!
  double getBaryonDensity() const
  {
  return numberDensity * type->getBaryonNumber();
  }

  //!
  //! get the charge of this particle.
  //!
  double getChargeDensity() const
  {
  return numberDensity * type->getCharge();
  }

  //!
  //! get the strangeness of this particle.
  //!
  double getStrangeDensity() const
  {
  return numberDensity * type->getNetStrangeness();
  }

  //!
  //! set the ParticleType of this particle.
  //!
  void setParticleType(ParticleType * _type);

//  void setStableParticleYield(double yield_in) {stableYield = yield_in;}
//  double getStableParticleYield() {return(stableYield);}

  double getBulkViscosityCoefficients(double Tdec __attribute__((unused)), double* bulkvisCoefficients __attribute__((unused)))
  {
  return 0.0; // for now... :-)
  }


  //!
  //! Get the ParticleType of this particle.
  //!
  ParticleType & getType() const     { return *type;   }

  ParticleType * getTypePtr() const     { return type;   }

  double getMass() const
  {
  return type->getMass();
  }

  double getSpinFactor() const
  {
  return type->getSpinFactor();
  }

  //!
  //! Return the number of distinct decay modes of this hadron type
  //!
  int    getNDecayModes() const
  {
  return type->getNDecayModes();
  }

  //!
  //! Get the ith  decay mode of  this particle
  //! @param index : decay mode object encapsulating the branching fraction and the list of particle types this particle decays into.
  //!
  ParticleDecayMode & getDecayMode(int index)
  {
    return type->getDecayMode(index);
  }

  void createDecayProbabilityArray(int nStable);


  void setDecayProbability(int iStable, double val)
  {
  decayProbability[iStable] = val;
  }
  void setDecayProbability(int iStable1, int iStable2, double val)
  {
  decayProbabilityPairs[iStable1][iStable2] = val;
  }

  double getDecayProbability(int iStable) const
  {
  return decayProbability[iStable];
  }

  double getDecayProbability(int iStable1,int iStable2)  const
  {
  return decayProbabilityPairs[iStable1][iStable2];
  }

  void printItem(const String name, const   String value, int size) const;
  void printItem(const String name, bool    value, int size) const;
  void printItem(const String name, int     value, int size) const;
  void printItem(const String name, double  value, int size, int precision) const;

  ostream & printProperties(ostream & os);

  ClassDef(ParticleThermalProperties,0)
};

} // namespace CAP

#endif  // SRC_Hadron_H_

