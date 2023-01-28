// Copyright 2021 Claude Pruneau
// Copyright 2016 Chun Shen
// This code is adapted from Chun's code
// for the purpose of computing hadronic resonance
// correlations..

//#include <gsl/gsl_sf_bessel.h>
//#include <gsl/gsl_sf_expint.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <TMath.h>
#include "Math/SpecFunc.h"
#include "HadronGasParticle.hpp"
using CAP::HadronGasParticle;

ClassImp(HadronGasParticle);

double HadronGasParticle::hbarC     = 0.19733;
double HadronGasParticle::hbarC2    = HadronGasParticle::hbarC * HadronGasParticle::hbarC;
double HadronGasParticle::hbarC3    = HadronGasParticle::hbarC2 * HadronGasParticle::hbarC;
double HadronGasParticle::hbarC4    = HadronGasParticle::hbarC3 * HadronGasParticle::hbarC;

double HadronGasParticle::pi        = TMath::Pi();
double HadronGasParticle::piSquare  = HadronGasParticle::pi * HadronGasParticle::pi;
double HadronGasParticle::piCube    = HadronGasParticle::piSquare * HadronGasParticle::pi;

int    HadronGasParticle::trunOrder = 10;

HadronGasParticle::HadronGasParticle()
:
type(nullptr),
muB(0.0),
muS(0.0),
mu(0.0),
temperature(0.0),
beta(0.0),
massBeta(0.0),
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
deltaN_qmu_term2(0.0),

decayProbability(),
decayProbabilityPairs()
{
}


HadronGasParticle::HadronGasParticle(const HadronGasParticle & source)
:
type(source.type),
muB(source.muB),
muS(source.muS),
mu(source.mu),
temperature(source.temperature),
beta(source.beta),
massBeta(source.massBeta),
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
deltaN_qmu_term2(source.deltaN_qmu_term2),
/// ????
decayProbability(source.decayProbability),
decayProbabilityPairs(source.decayProbabilityPairs)
{

}

HadronGasParticle & HadronGasParticle::operator=(const HadronGasParticle & source)
{
  if (this!=&source)
    {
    type                  = source.type;
    muB                   = source.muB;
    muS                   = source.muS;
    mu                    = source.mu;
    temperature           = source.temperature;
    beta                  = source.beta;
    massBeta              = source.massBeta;
    numberDensity         = source.numberDensity;
    energyDensity         = source.energyDensity;
    entropyDensity        = source.entropyDensity;
    pressure              = source.pressure;
    dPoverTdmu            = source.dPoverTdmu;
    deoverTdmu            = source.deoverTdmu;
    dndmu                 = source.dndmu;
    dsdmu                 = source.dsdmu;
    decayProbability      = source.decayProbability;
    decayProbabilityPairs = source.decayProbabilityPairs;
    }
  return *this;
}

void HadronGasParticle::reset()
{
  muB                   = 0.0;
  muS                   = 0.0;
  mu                    = 0.0;
  temperature           = 0.0;
  beta                  = 0.0;
  massBeta              = 0.0;
  numberDensity         = 0.0;
  energyDensity         = 0.0;
  entropyDensity        = 0.0;
  pressure              = 0.0;
  dPoverTdmu            = 0.0;
  deoverTdmu            = 0.0;
  dndmu                 = 0.0;
  dsdmu                 = 0.0;
  decayProbability.clear();
  decayProbabilityPairs.clear();
}

void HadronGasParticle::clear()
{
  type = nullptr;
  reset();
}


void HadronGasParticle::setParticleType(ParticleType * _type)
{
  type   = _type;
  mass   = type->getMass();
  gSpin  = type->getSpinFactor();
  sign   = type->getStatistics();
  mass2  = mass*mass;
  mass3  = mass2*mass;
  mass4  = mass3*mass;
}


void HadronGasParticle::createDecayProbabilityArray(int nStable)
{
  decayProbability.assign(nStable,0.0);
  vector<double> list(nStable,0.0);
  for (int i = 0; i < nStable; i++)
    {
    decayProbabilityPairs.push_back(list);
    }
}


//!
//! Compute HadronGasParticle thermal yield at given T, muB, and muS
//!
//! Energy Density at given T
//! unit: GeV/fm^3
//! Compute HadronGasParticle 1/T dP/dmu at given T [1/(GeV fm^3)]
//! Compute HadronGasParticle d(e/T)/dmu [1/(GeV fm^3)]
//!
void HadronGasParticle::calculateAllProperties(double _temp, double _muB, double _muS)
{
  temperature = _temp;
  muB         = _muB;
  muS         = _muS;
  beta        = 1.0/temperature;
  beta2       = beta*beta;
  beta3       = beta2*beta;
  massBeta    = mass*beta;
  massBeta2   = massBeta*massBeta;
  massBeta3   = massBeta2*massBeta;
  mu = muB*type->getBaryon() + muS*type->getStrange();
  muBeta      = mu*beta;
  lambda      = exp(muBeta);  

//  cout << " temperature:" << temperature << endl;
//  cout << "         muB:" << muB << endl;
//  cout << "         muS:" << muS << endl;
//  cout << "        beta:" << beta << endl;
//  cout << "    massBeta:" << massBeta << endl;
//  cout << "          mu:" << mu << endl;
//  cout << "      muBeta:" << muBeta << endl;
//  cout << "      lambda:" << lambda << endl;

  double besselKn1, besselKn2;
  double arg;
  double theta;
  double fugacity;
  double doubleJ;

  numberDensity     = 0.0;
  deltaN_bulk_term1 = 0.0;
  deltaN_bulk_term2 = 0.0;
  deltaN_qmu_term1  = 0.0;
  deltaN_qmu_term2  = 0.0;
  energyDensity     = 0.0;
  pressure          = 0.0;
  dPoverTdmu        = 0.0;
  deoverTdmu        = 0.0;
  dndmu             = 0.0;
  dsdmu             = 0.0;
  N_eq              = 0.0;

  for (int j = 1; j < trunOrder; j++)
    {
    doubleJ    = double(j);
    arg        = doubleJ*massBeta;  // argument inside bessel functions
    theta      = pow(-sign, j-1);
    fugacity   = pow(lambda, j);
    besselKn1  = TMath::BesselK1(arg);
    besselKn2  = TMath::BesselK(2, arg);
    N_eq       += theta/double(j)*fugacity*besselKn2;

//    if (j<2)
//      {
//      cout << "    massBeta:" << massBeta << endl;
//      cout << "          mu:" << mu << endl;
//      cout << "      muBeta:" << muBeta << endl;
//      cout << "      lambda:" << lambda << endl;
//      cout << "     doubleJ:" << doubleJ << endl;
//      cout << "         arg:" << arg << endl;
//      cout << "       theta:" << theta << endl;
//      cout << "    fugacity:" << fugacity << endl;
//      cout << "   besselKn1:" << scientific << setw(15) << setprecision(5)<< besselKn1 << endl;
//      cout << "   besselKn2:" << scientific << setw(15) << setprecision(5)<< besselKn2 << endl;
//      cout << "        N_eq:" << scientific << setw(15) << setprecision(5)<< N_eq << endl;
//      }

    numberDensity     += theta*fugacity*besselKn2/doubleJ;
    deltaN_bulk_term1 += theta*fugacity*(massBeta*besselKn1 + 3.0*besselKn2/doubleJ);
    deltaN_bulk_term2 += theta*fugacity*besselKn1;
    deltaN_qmu_term1  += theta*fugacity*besselKn2/doubleJ; // baryon diffusion contribution

    double I_1_1 = exp(-arg)/arg*(2./(arg*arg) + 2./arg - 0.5);
    double I_1_2 = 3.0/8.0*ROOT::Math::expint_n(2, arg);
    double I_1_n = I_1_1 + I_1_2;
    double double_factorial    = 1.;  // record (2k-5)!!
    double factorial           = 2.;  // record k! start with 2!
    double factor_2_to_k_power = 4.;  // record 2^k start with 2^2
    for (int k = 3; k < trunOrder; k++)
      {
      double_factorial    *= (2*k - 5);
      factorial           *= k;
      factor_2_to_k_power *= 2;
      double rrr = ROOT::Math::expint_n(2*k-2, arg);
      double I_1_k = (3.*double_factorial/factor_2_to_k_power/factorial*rrr);
      I_1_n += I_1_k;
      }
    I_1_n            = -massBeta3*I_1_n;
    deltaN_qmu_term2 += doubleJ*theta*fugacity*I_1_n;
    energyDensity += theta*fugacity*(3.*besselKn2/(arg*arg) + besselKn1/arg);
    pressure      += theta*fugacity*besselKn2/doubleJ/doubleJ;
    dPoverTdmu    += theta*fugacity*besselKn2/doubleJ;
    deoverTdmu    += theta*fugacity*doubleJ*(3*besselKn2/(arg*arg)+ besselKn1/arg);
    dndmu         += theta*fugacity*besselKn2;
    }
  numberDensity     = numberDensity*gSpin*mass2*temperature/(2*piSquare*hbarC3);
  deltaN_bulk_term1 = mass2*temperature*deltaN_bulk_term1;
  deltaN_bulk_term2 = 0.3333333*mass3*deltaN_bulk_term2;
  deltaN_qmu_term1  = mass2*deltaN_qmu_term1/(beta2);
  deltaN_qmu_term2  = deltaN_qmu_term2/(3.0*beta3);
  energyDensity     = energyDensity*gSpin*mass4/(2*piSquare*hbarC3);
  pressure          = pressure*gSpin*mass2*temperature*temperature/(2*piSquare*hbarC3);
  entropyDensity    = (energyDensity + pressure - mu*numberDensity)*beta;    // unit : 1/fm^3
  dPoverTdmu        = dPoverTdmu*gSpin*mass2/(2*piSquare*hbarC3);
  deoverTdmu        = deoverTdmu*gSpin*mass4/(2*piSquare*hbarC3);
  dndmu             = dndmu*gSpin*mass/(2*piSquare);
  dsdmu             = dPoverTdmu - numberDensity - mu*dndmu + deoverTdmu;
}

//!
//!Compute HadronGasParticle density at the given momentum based
//!on Maxewell Boltzmann distribution
//!
//double HadronGasParticle::computeDensityAt(double p,double temperature,double mu)
//{
//  double arg = (sqrt(mass*mass + p*p) - mu)/temperature;
//  double densityAtP = yield*exp(-arg);  // need to fix normalization.
//  return densityAtP;
//}

ostream & HadronGasParticle::printProperties(ostream & os)
{
  //scientific
  os <<  fixed << setw(20) << setprecision(8)
  << type->getName();
  //os <<  scientific << setw(15) << setprecision(5)<< muB;
  //os <<  scientific << setw(15) << setprecision(5)<< muS;
  os <<  scientific << setw(15) << setprecision(5)<< mu;
  //os <<  scientific << setw(15) << setprecision(5)<< temperature;
  //os <<  scientific << setw(15) << setprecision(5)<< beta;
  os <<  scientific << setw(15) << setprecision(5)<< numberDensity;        // HadronGasParticle thermal yield at given T and mu
  os <<  scientific << setw(15) << setprecision(5)<< energyDensity;
  os <<  scientific << setw(15) << setprecision(5)<< entropyDensity;
  os <<  scientific << setw(15) << setprecision(5)<< pressure;       // thermodynamic quantities at given T and mu
  os <<  scientific << setw(15) << setprecision(5)<< dPoverTdmu;
  os <<  scientific << setw(15) << setprecision(5)<< deoverTdmu;
  os <<  scientific << setw(15) << setprecision(5)<< dndmu;
  os <<  scientific << setw(15) << setprecision(5)<< dsdmu;
  os << endl;
  return os;
}


// void HadronGasParticle::getBulkViscosityCoefficients(
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
