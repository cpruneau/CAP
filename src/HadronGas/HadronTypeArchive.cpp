// Copyright 2021 Claude Pruneau
// Copyright 2016 Chun Shen
// This code is adapted from Chun's code
// for the purpose of computing hadronic resonance
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
#include "HadronType.hpp"

ClassImp(HadronType);

double HadronType::hbarC     = 0.19733;
int    HadronType::trunOrder = 10;

HadronType::HadronType(int pdgCode_in, string name_in, double mass_in,
                           double width_in, int gSpin_in, int baryon_in,
                           int strange_in, int charm_in, int bottom_in,
                           int gIsospin_in, int charge_in, int NdecayChannel_in)
:
pdgCode(pdgCode_in),
name(name_in),
mass(mass_in),
width(width_in),
gSpin(gSpin_in),
baryon(baryon_in),
strange(strange_in),
charm(charm_in),
bottom(bottom_in),
gIsospin(gIsospin_in),
charge(charge_in),
nDecays(0),
stable(0),
mu(0.0),
sign((baryon_in==0)?-1:1), // determine Bose/Fermi statistic for HadronType
channelIdx(0),
yield(0.0),
stableYield(0),
dPoverTdmu(0.0),
deoverTdmu(0.0),
dndmu(0.0),
dsdmu(0.0),
energyDensity(0.0),
entropyDensity(0.0),
pressure(0.0),
nDecayChannels(NdecayChannel_in),
decaysBranchingRatio(nullptr),
decaysNPart(nullptr),
decaysHadron(nullptr),
decayProbability(nullptr),
decayProbabilityPairs(nullptr),
decayRndmSelector(nullptr)
{
  instantiateArrays();
}

HadronType::~HadronType()
{
  delete [] decaysBranchingRatio;
  delete [] decaysNPart;
  for (int i = 0; i < nDecayChannels; i++) delete [] decaysHadron[i];
  delete [] decaysHadron;
  delete [] decayProbability;
  //decayProbabilityPairs[i] = new double[dimension];
  //for (int j = 0; j < dimension; j++) decayProbabilityPairs[i][j] = 0.0;
  delete [] decayProbabilityPairs;
  delete decayRndmSelector;
}

HadronType::HadronType(const HadronType & source)
:
pdgCode(source.pdgCode),
name(source.name),
mass(source.mass),
width(source.width),
gSpin(source.gSpin),
baryon(source.baryon),
strange(source.strange),
charm(source.charm),
bottom(source.bottom),
gIsospin(source.gIsospin),
charge(source.charge),
nDecays(source.nDecays),
stable(source.stable),
mu(source.mu),
sign(source.sign), // determine Bose/Fermi statistic for HadronType
channelIdx(source.channelIdx),
yield(source.yield),
stableYield(source.stableYield),
dPoverTdmu(source.dPoverTdmu),
deoverTdmu(source.deoverTdmu),
dndmu(source.dndmu),
dsdmu(source.dsdmu),
energyDensity(source.energyDensity),
entropyDensity(source.entropyDensity),
pressure(source.pressure),
nDecayChannels(source.nDecayChannels),
decaysBranchingRatio(nullptr),
decaysNPart(nullptr),
decaysHadron(nullptr),
decayProbability(nullptr),
decayProbabilityPairs(nullptr)
{
  instantiateArrays();
}
HadronType & HadronType::operator=(const HadronType & source)
{
  if (this!=&source)
    {
    pdgCode  =  source.pdgCode;
    name     =  source.name;
    mass     =  source.mass;
    width    =  source.width;
    gSpin    =  source.gSpin;
    baryon   =  source.baryon;
    strange  =  source.strange;
    charm    =  source.charm;
    bottom   =  source.bottom;
    gIsospin =  source.gIsospin;
    charge   =  source.charge;
    nDecays  =  source.nDecays;
    stable   =  source.stable;
    mu       =  source.mu;
    sign        =  source.sign;
    channelIdx  =  source.channelIdx;
    yield       =  source.yield;
    stableYield =  source.stableYield;
    dPoverTdmu  =  source.dPoverTdmu;
    deoverTdmu  =  source.deoverTdmu;
    dndmu       =  source.dndmu;
    dsdmu       =  source.dsdmu;
    energyDensity   =  source.energyDensity;
    entropyDensity  =  source.entropyDensity;
    pressure        =  source.pressure;
    nDecayChannels  =  source.nDecayChannels;
    decaysBranchingRatio  =  nullptr;
    decaysNPart           =  nullptr;
    decaysHadron          =  nullptr;
    decayProbability      =  nullptr;
    decayProbabilityPairs =  nullptr;
    instantiateArrays();
    }
  return *this;
}

void HadronType::instantiateArrays()
{
  decaysBranchingRatio = new double[nDecayChannels];
  decaysNPart          = new int[nDecayChannels];
  decaysHadron         = new int* [nDecayChannels];
}

void HadronType::addResonanceDecays(double branchratio,
                                int Npart,
                                int* decayChannelHadrons)
{
  // add resonance decay channel for HadronType
  if (channelIdx > nDecayChannels-1) {
    cout << "Warning: channelidx exceed number of decay channels! "
    << "Please check" << endl;
    exit(1);
  }
  decaysBranchingRatio[channelIdx] = branchratio;
  decaysNPart[channelIdx]          = Npart;
  decaysHadron[channelIdx]   = new int[Npart];
  for (int i = 0; i < Npart; i++)
  decaysHadron[channelIdx][i] = decayChannelHadrons[i];
  channelIdx++;

  if (nDecayChannels == 1 &&
      decaysNPart[0] == 1 &&
      decaysHadron[0][0] == pdgCode &&
      fabs(branchratio - 1.0) < 1e-15)
    {
    // HadronType is stable
    stable = 1;
    }
  return;
}

// ======================================================================
// Return the corresponding anti-HadronType' Monte-Carlo value
// ======================================================================
int HadronType::getAntiHadronPdgCode()
{
  if (baryon == 0 && charge == 0 && strange == 0)
    return(pdgCode);
  else
    return(-pdgCode);
}

void HadronType::calculateChemicalPotential(double muB, double muS)
{
  mu = muB*baryon + muS*strange;
}



void HadronType::createDecayProbabilityArray(int nStable)
{
  decayProbability            = new double[nStable];
  decayProbabilityPairs       = new double*[nStable];
  for (int i = 0; i < nStable; i++)
  {
  decayProbability[i] = 0.0;
  decayProbabilityPairs[i] = new double[nStable];
  for (int j = 0; j < nStable; j++)
    {
    decayProbabilityPairs[i][j] = 0.0;
    }
  }
}

void HadronType::setupDecayGenerator()
{
  decayRndmSelector = new SelectionGenerator(nDecayChannels,decaysBranchingRatio);
}

void HadronType::generateDecay(int&n,int * pid)
{
  int index = decayRndmSelector->generate();
  n = decaysNPart[k];
  for (int k=0; k<n; k++)
  {
  pid[k] = decaysHadron[index][k];
  }
}

// ======================================================================
// Compute HadronType thermal yield at given T, mu
// ======================================================================
void HadronType::calculateParticleDensity(double temperature,
                                          double muB,
                                          double muS)
{
  int sf_expint_truncate_order = trunOrder;

  double N_eq = 0.0;                   // equilibrium contribution
  double deltaN_bulk_term1 = 0.0;      // contribution from bulk delta f
  double deltaN_bulk_term2 = 0.0;      // contribution from bulk delta f
  double deltaN_qmu_term1  = 0.0;      // contribution from baryon diffusion
  double deltaN_qmu_term2  = 0.0;      // contribution from baryon diffusion

  double beta = 1./temperature;
  calculateChemicalPotential(muB, muS);
  double lambda = exp(beta*mu);  // fugacity factor
  double prefactor = gSpin/(2*M_PI*M_PI)/hbarC/hbarC/hbarC;
  double mbeta = mass*beta;

  //cout << scientific << setw(16) << setprecision(6) <<"muB:" << muB << " muS:" << muS << " mu:" << mu << " lambda:" << lambda << " prefactor:" << prefactor << " mbeta" << mbeta << endl;
  //cout << "trunOrder:" << trunOrder << endl;

  for (int n = 1; n < trunOrder; n++)
  {
  double arg = n*mbeta;  // argument inside bessel functions
  double theta = pow(-sign, n-1);
  double fugacity = pow(lambda, n);
//  double K_2 = gsl_sf_bes sel_Kn(2, arg);
//  double K_1 = gsl_sf_bes sel_K1(arg);

  double K_2 = TMath::BesselK(2, arg);
  double K_1 = TMath::BesselK1(arg);
  //if (arg<12.0)  cout << scientific << setw(16) << setprecision(6) << "ARG:" << arg << "  K_1:" << K_1 << " K_2:" << K_2 << endl;
  // equilibrium contribution
  N_eq += theta/double(n)*fugacity*K_2;
  //if (mass< 0.5)  cout << scientific << setw(16) << setprecision(6) << " theta:" << theta << " fugacity:" << fugacity <<  " n=" << n << " N_eq:" << N_eq << endl;
  // bulk viscous contribution
  deltaN_bulk_term1 += theta*fugacity*(mass*beta*K_1 + 3./n*K_2);
  deltaN_bulk_term2 += theta*fugacity*K_1;

  // baryon diffusion contribution
  deltaN_qmu_term1 += theta/n*fugacity*K_2;
  double I_1_1 = exp(-arg)/arg*(2./(arg*arg) + 2./arg - 1./2.);
  //double I_1_2 = 3./8.*gsl_sf_expint_E2(arg);
  //double rrr = ROOT::Math::expint_n(2, arg);
  //if (arg<4.0) cout << " arg:" << arg << " expint:" << rrr << endl;
  double I_1_2 = 3./8.*ROOT::Math::expint_n(2, arg);
  double I_1_n = I_1_1 + I_1_2;

  double double_factorial = 1.;  // record (2k-5)!!
  double factorial = 2.;         // record k! start with 2!
  double factor_2_to_k_power = 4.;      // record 2^k start with 2^2
  for (int k = 3; k < sf_expint_truncate_order; k++)
    {
    double_factorial *= (2*k - 5);
    factorial *= k;
    factor_2_to_k_power *= 2;
    double rrr = ROOT::Math::expint_n(2*k-2, arg);

    //if (arg<4.0) cout << scientific << setw(16) << setprecision(6) << "k:" << k << " arg:" << arg << " expint:" << rrr << endl;
    double I_1_k = (3.*double_factorial/factor_2_to_k_power/factorial
                    *rrr);
//    double I_1_k = (3.*double_factorial/factor_2_to_k_power/factorial
//                    *gsl_sf_expint_En(2*k-2, arg));
    I_1_n += I_1_k;
    }
  I_1_n = -(mbeta*mbeta*mbeta)*I_1_n;
  deltaN_qmu_term2 += n*theta*fugacity*I_1_n;
  }

  // equilibrium contribution
  double prefactor_Neq = mass*mass*temperature;
  N_eq = prefactor*prefactor_Neq*N_eq;

  // bulk viscous contribution
  deltaN_bulk_term1 = mass*mass/beta*deltaN_bulk_term1;
  deltaN_bulk_term2 = mass*mass*mass/3.*deltaN_bulk_term2;

  // baryon diffusion contribution
  deltaN_qmu_term1 = mass*mass/(beta*beta)*deltaN_qmu_term1;
  deltaN_qmu_term2 = 1./(3.*beta*beta*beta)*deltaN_qmu_term2;
  yield = N_eq;

  //cout << "temperature:" << temperature << " mass:" << mass << " N_eq:" << N_eq << endl;
}

// ======================================================================
// Compute HadronType Energy Density at given T
// ======================================================================
double HadronType::calculateEnergyDensity(double temperature)
{
  double results;
  results = 0.0;
  double prefactor = gSpin/(2*M_PI*M_PI)*pow(mass, 4);
  for (int j = 0; j <trunOrder; j++)
  {
  double arg = (j+1)*mass/temperature;
  double lambda = exp(mu/temperature);
//  results += (pow((-1.0)*sign, j)*pow(lambda, j+1)
//              *(3.*gsl_sf_bes sel_Kn(2, arg)/(arg*arg)
//                + gsl_sf_bes sel_Kn(1, arg)/arg));
  results += (pow((-1.0)*sign, j)*pow(lambda, j+1)
              *(3.*TMath::BesselK(2, arg)/(arg*arg)
                + TMath::BesselK(1, arg)/arg));
  }


  results = results*prefactor;
  energyDensity = results/pow(hbarC, 3);    // unit: GeV/fm^3
  return (energyDensity);
}

// ======================================================================
// Compute HadronType (Partial) Pressure at given T
// ======================================================================
double HadronType::calculatePressure(double temperature)
{
  double results;
  results = 0.0;
  double prefactor = gSpin/(2*M_PI*M_PI)*pow(mass, 2)*pow(temperature, 2);
  for (int j = 0; j < trunOrder; j++)
  {
  double arg = (j+1)*mass/temperature;
  double lambda = exp(mu/temperature);
//  results += (pow((-1.0)*sign, j)/pow(j+1., 2)
//              *pow(lambda, j+1)*gsl_sf_bes sel_Kn(2, arg));
  results += (pow((-1.0)*sign, j)/pow(j+1., 2)
              *pow(lambda, j+1)*TMath::BesselK(2, arg));
  }

  results = results*prefactor;
  pressure = results/pow(hbarC, 3);    // unit : GeV/fm^3
  return(pressure);
}

// ======================================================================
// Compute HadronType (Partial) Entropy Density
// using the first law of thermodynamics
// at given T and mu
// ======================================================================
double HadronType::calculateEntropyDensity(double temperature)
{
  entropyDensity = (energyDensity + pressure - mu*yield)/temperature;    // unit : 1/fm^3
  return(entropyDensity);
}

// ======================================================================
// Compute HadronType dN/dmu at given T [1/GeV]
// ======================================================================
double HadronType::calculate_dndmu(double temperature)
{
  double results;
  results = 0.0;
  double prefactor = gSpin/(2*M_PI*M_PI)*mass;
  for (int j = 0; j < trunOrder; j++) {
    double arg = (j+1)*mass/temperature;
    double lambda = exp(mu/temperature);
    //results += pow((-1.0)*sign, j)*pow(lambda, j+1)*gsl_sf_bes sel_Kn(2, arg);
    results += pow((-1.0)*sign, j)*pow(lambda, j+1)*TMath::BesselK(2, arg);
  }

  results = results*prefactor;
  return(results);
}

// ======================================================================
// Compute HadronType 1/T dP/dmu at given T [1/(GeV fm^3)]
// ======================================================================
double HadronType::calculate_dPoverTdmu(double temperature)
{
  double results;
  results = 0.0;
  double prefactor = gSpin/(2*M_PI*M_PI)*mass*mass;
  for (int j = 0; j < trunOrder; j++)
  {
  double arg = (j+1)*mass/temperature;
  double lambda = exp(mu/temperature);
//  results += (pow((-1.0)*sign, j)*pow(lambda, j+1)/(j+1)
//              *gsl_sf_bes sel_Kn(2, arg));
  results += (pow((-1.0)*sign, j)*pow(lambda, j+1)/(j+1)
              *TMath::BesselK(2, arg));
  }

  results = results*prefactor/pow(hbarC, 3);
  return(results);
}

// ======================================================================
// Compute HadronType d(e/T)/dmu [1/(GeV fm^3)]
// ======================================================================
double HadronType::calculate_deoverTdmu(double temperature)
{
  double results;
  results = 0.0;
  double prefactor = gSpin/(2*M_PI*M_PI)*pow(mass, 4);
  for (int j = 0; j < trunOrder; j++) {
    double arg = (j+1)*mass/temperature;
    double lambda = exp(mu/temperature);
//    results += (pow((-1.0)*sign, j)*pow(lambda, j+1)*(j+1)
//                *(3*gsl_sf_bes sel_Kn(2, arg)/(arg*arg)
//                  + gsl_sf_bes sel_Kn(1, arg)/arg));
    results += (pow((-1.0)*sign, j)*pow(lambda, j+1)*(j+1)
                *(3*TMath::BesselK(2, arg)/(arg*arg)
                  + TMath::BesselK(1, arg)/arg));
  }

  results = results*prefactor/pow(hbarC, 3);
  return(results);
}


// ======================================================================
// Compute HadronType first order derivative ds/dmu [1/(GeV fm^3)]
// ======================================================================
double HadronType::calculate_dsdmu(double temperature)
{
   dPoverTdmu = calculate_dPoverTdmu(temperature);
   deoverTdmu = calculate_deoverTdmu(temperature);
   dndmu = calculate_dndmu(temperature);
   dsdmu = dPoverTdmu - yield - mu*dndmu + deoverTdmu;
  return(dsdmu);
}

// ======================================================================
// Compute HadronType density at the given momentum based
// on Maxewell Boltzmann distribution
// ======================================================================
double HadronType::computeDensityAt(double p,double temperature,double mu)
{
  double arg = (sqrt(mass*mass + p*p) - mu)/temperature;
  double densityAtP = yield*exp(-arg);  // need to fix normalization.
  return densityAtP;
}

ostream & HadronType::printBasicProperties(ostream & os)
{
  //scientific
  os <<  fixed << setw(10) << setprecision(8) << pdgCode;         // Monte-Carlo number according PDG
  os <<  setw(20) << name;   // HadronType name
  os <<  scientific << setw(20) << setprecision(4) <<  mass;        // HadronType mass (GeV)
  os <<  scientific << setw(20) << setprecision(4) <<  width;       // decay width
  os <<  fixed << setw(5) << setprecision(3) <<  gSpin;          // spin degeneracy
  os <<  fixed << setw(5) << setprecision(3) <<  baryon;         // baryon number
  os <<  fixed << setw(5) << setprecision(3) <<  strange;        // strangeness
  os <<  fixed << setw(5) << setprecision(3) <<  charm;          // charmness
  os <<  fixed << setw(5) << setprecision(3) <<  bottom;         // bottomness
  os <<  fixed << setw(5) << setprecision(3) <<  gIsospin;       // isospin degeneracy
  os <<  fixed << setw(5) << setprecision(3) <<  charge;         // charge
  os <<  fixed << setw(15) << setprecision(6) <<  nDecays;         // amount of decays listed for this resonance
  os <<  fixed << setw(5) << setprecision(3) <<  stable;         // defines whether this HadronType is stable
  os <<  scientific << setw(20) << setprecision(5) <<  mu;          // chemical potential
  os <<  fixed << setw(5) << setprecision(3) <<  sign;
  return os;
}


ostream & HadronType::printCalcProperties(ostream & os)
{
  //scientific
  os <<  fixed << setw(10) << setprecision(8) << pdgCode;         // Monte-Carlo number according PDG
  os <<  setw(20) << name;   // HadronType name
  os <<  scientific << setw(20) << setprecision(5) <<  dPoverTdmu;
  os <<  scientific << setw(20) << setprecision(5) <<  deoverTdmu;
  os <<  scientific << setw(20) << setprecision(5) <<  dndmu;
  os <<  scientific << setw(20) << setprecision(5) <<  dsdmu;
  os <<  scientific << setw(20) << setprecision(5) <<  energyDensity;
  os <<  scientific << setw(20) << setprecision(5) <<  entropyDensity;
  os <<  scientific << setw(20) << setprecision(5) <<  pressure;
  return os;
}


// void HadronType::getBulkViscosityCoefficients(
//                 double Tdec, double* bulkvisCoefficients) {
// // get transport coefficient for bulk delta f
//     double Tdec_fm = Tdec/hbarC;  // [1/fm]
//     double Tdec_fm_power[11];     // cache the polynomial power of Tdec_fm
//     Tdec_fm_power[1] = Tdec_fm;
//     for (int ipower = 2; ipower < 11; ipower++)
//         Tdec_fm_power[ipower] = Tdec_fm_power[ipower-1]*Tdec_fm;
//     if (bulk_deltaf_kind == 1) {    // relaxation type
//         // parameterization from JF
//         // A Polynomial fit to each coefficient -- X is the temperature in fm^-1
//         // Both fits are reliable between T=100 -- 180 MeV,
//         // do not trust it beyond
//         bulkvisCoefficients[0] = (642096.624265727
//                                   - 8163329.49562861*Tdec_fm_power[1]
//                                   + 47162768.4292073*Tdec_fm_power[2]
//                                   - 162590040.002683*Tdec_fm_power[3]
//                                   + 369637951.096896*Tdec_fm_power[4]
//                                   - 578181331.809836*Tdec_fm_power[5]
//                                   + 629434830.225675*Tdec_fm_power[6]
//                                   - 470493661.096657*Tdec_fm_power[7]
//                                   + 230936465.421*Tdec_fm_power[8]
//                                   - 67175218.4629078*Tdec_fm_power[9]
//                                   + 8789472.32652964*Tdec_fm_power[10]);
// 
//         bulkvisCoefficients[1] = (1.18171174036192
//                                   - 17.6740645873717*Tdec_fm_power[1]
//                                   + 136.298469057177*Tdec_fm_power[2]
//                                   - 635.999435106846*Tdec_fm_power[3]
//                                   + 1918.77100633321*Tdec_fm_power[4]
//                                   - 3836.32258307711*Tdec_fm_power[5]
//                                   + 5136.35746882372*Tdec_fm_power[6]
//                                   - 4566.22991441914*Tdec_fm_power[7]
//                                   + 2593.45375240886*Tdec_fm_power[8]
//                                   - 853.908199724349*Tdec_fm_power[9]
//                                   + 124.260460450113*Tdec_fm_power[10]);
//     } else if (bulk_deltaf_kind == 2) {
//         // A Polynomial fit to each coefficient -- X is the temperature in fm^-1
//         // Both fits are reliable between T=100 -- 180 MeV
//         // do not trust it beyond
//         bulkvisCoefficients[0] = (21091365.1182649
//                                   - 290482229.281782*Tdec_fm_power[1]
//                                   + 1800423055.01882*Tdec_fm_power[2]
//                                   - 6608608560.99887*Tdec_fm_power[3]
//                                   + 15900800422.7138*Tdec_fm_power[4]
//                                   - 26194517161.8205*Tdec_fm_power[5]
//                                   + 29912485360.2916*Tdec_fm_power[6]
//                                   - 23375101221.2855*Tdec_fm_power[7]
//                                   + 11960898238.0134*Tdec_fm_power[8]
//                                   - 3618358144.18576*Tdec_fm_power[9]
//                                   + 491369134.205902*Tdec_fm_power[10]);
// 
//         bulkvisCoefficients[1] = (4007863.29316896
//                                   - 55199395.3534188*Tdec_fm_power[1]
//                                   + 342115196.396492*Tdec_fm_power[2]
//                                   - 1255681487.77798*Tdec_fm_power[3]
//                                   + 3021026280.08401*Tdec_fm_power[4]
//                                   - 4976331606.85766*Tdec_fm_power[5]
//                                   + 5682163732.74188*Tdec_fm_power[6]
//                                   - 4439937810.57449*Tdec_fm_power[7]
//                                   + 2271692965.05568*Tdec_fm_power[8]
//                                   - 687164038.128814*Tdec_fm_power[9]
//                                   + 93308348.3137008*Tdec_fm_power[10]);
//     } else if (bulk_deltaf_kind == 3) {
//         bulkvisCoefficients[0] = (160421664.93603
//                                   - 2212807124.97991*Tdec_fm_power[1]
//                                   + 13707913981.1425*Tdec_fm_power[2]
//                                   - 50204536518.1767*Tdec_fm_power[3]
//                                   + 120354649094.362*Tdec_fm_power[4]
//                                   - 197298426823.223*Tdec_fm_power[5]
//                                   + 223953760788.288*Tdec_fm_power[6]
//                                   - 173790947240.829*Tdec_fm_power[7]
//                                   + 88231322888.0423*Tdec_fm_power[8]
//                                   - 26461154892.6963*Tdec_fm_power[9]
//                                   + 3559805050.19592*Tdec_fm_power[10]);
//         bulkvisCoefficients[1] = (33369186.2536556
//                                   - 460293490.420478*Tdec_fm_power[1]
//                                   + 2851449676.09981*Tdec_fm_power[2]
//                                   - 10443297927.601*Tdec_fm_power[3]
//                                   + 25035517099.7809*Tdec_fm_power[4]
//                                   - 41040777943.4963*Tdec_fm_power[5]
//                                   + 46585225878.8723*Tdec_fm_power[6]
//                                   - 36150531001.3718*Tdec_fm_power[7]
//                                   + 18353035766.9323*Tdec_fm_power[8]
//                                   - 5504165325.05431*Tdec_fm_power[9]
//                                   + 740468257.784873*Tdec_fm_power[10]);
//     } else if (bulk_deltaf_kind == 4) {
//         bulkvisCoefficients[0] = (1167272041.90731
//                                   - 16378866444.6842*Tdec_fm_power[1]
//                                   + 103037615761.617*Tdec_fm_power[2]
//                                   - 382670727905.111*Tdec_fm_power[3]
//                                   + 929111866739.436*Tdec_fm_power[4]
//                                   - 1540948583116.54*Tdec_fm_power[5]
//                                   + 1767975890298.1*Tdec_fm_power[6]
//                                   - 1385606389545*Tdec_fm_power[7]
//                                   + 709922576963.213*Tdec_fm_power[8]
//                                   - 214726945096.326*Tdec_fm_power[9]
//                                   + 29116298091.9219*Tdec_fm_power[10]);
//         bulkvisCoefficients[1] = (5103633637.7213
//                                   - 71612903872.8163*Tdec_fm_power[1]
//                                   + 450509014334.964*Tdec_fm_power[2]
//                                   - 1673143669281.46*Tdec_fm_power[3]
//                                   + 4062340452589.89*Tdec_fm_power[4]
//                                   - 6737468792456.4*Tdec_fm_power[5]
//                                   + 7730102407679.65*Tdec_fm_power[6]
//                                   - 6058276038129.83*Tdec_fm_power[7]
//                                   + 3103990764357.81*Tdec_fm_power[8]
//                                   - 938850005883.612*Tdec_fm_power[9]
//                                   + 127305171097.249*Tdec_fm_power[10]);
//     }
//     return;
// }
