/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/
#include "RandomGenerators.hpp"

using CAP::RandomGenerator;
using CAP::ScalarIntRandomGenerator;
using CAP::FixedScalarIntRandomGenerator;
using CAP::ExpScalarIntRandomGenerator;
using CAP::ScalarDoubleRandomGenerator;
using CAP::FixedScalarDoubleRandomGenerator;
using CAP::LinearScalarDoubleRandomGenerator;
using CAP::ExpScalarDoubleRandomGenerator;
using CAP::HistoScalarDoubleRandomGenerator;
using CAP::VectorRandomGenerator;


ClassImp(RandomGenerator);

RandomGenerator::RandomGenerator(Random * rg)
: randomGenerator(rg)
{}

ClassImp(ScalarIntRandomGenerator);

ScalarIntRandomGenerator::ScalarIntRandomGenerator(int min,
                                                 int max,
                                                 Random * rg)
:
RandomGenerator(rg),
minimum (min),
maximum (max),
range   (max-min)
{
}

int ScalarIntRandomGenerator::generate()
{
  return minimum + range * randomGenerator->Rndm();
}

FixedScalarIntRandomGenerator::FixedScalarIntRandomGenerator(int fixedValue)
:
ScalarIntRandomGenerator(fixedValue,0.0, 0)
{
}

// Always return the same selected value
int FixedScalarIntRandomGenerator::generate()
{
  // cout << " FixedScalarIntRandomGenerator:: generate() was called." << endl;

  return minimum;
}

ClassImp(ExpScalarIntRandomGenerator);


ExpScalarIntRandomGenerator::ExpScalarIntRandomGenerator(int    minimum,
                                                       double theSlope,
                                                       Random * randomGenerator)
:
ScalarIntRandomGenerator(minimum,minimum,randomGenerator),
slope(theSlope)
{

}

// Always return the same selected value
int ExpScalarIntRandomGenerator::generate()
{
  // cout << " ExpScalarIntRandomGenerator:: generate() was called." << endl;

  // used stored value of maximum as slope
  return minimum + randomGenerator->Exp(slope);
}



ClassImp(ScalarDoubleRandomGenerator);


ScalarDoubleRandomGenerator::ScalarDoubleRandomGenerator(double min, double max, Random * rg) :
RandomGenerator(rg),
minimum (min),
maximum (max),
range   (max-min)
{  }

double ScalarDoubleRandomGenerator::generate()
{
  double value = minimum + range * randomGenerator->Rndm();
  // cout << " ScalarDoubleRandomGenerator:: generate() was called." << endl;
  // cout << " minimum : " << minimum << endl;
  // cout << "   range : " << range << endl;
  // cout << "   value : " << value << endl;
  return value;
}


ClassImp(FixedScalarDoubleRandomGenerator);


FixedScalarDoubleRandomGenerator::FixedScalarDoubleRandomGenerator(double fixedValue)
:
ScalarDoubleRandomGenerator(fixedValue, 0, 0)
{   }

// Always return the same selected value
double FixedScalarDoubleRandomGenerator::generate()
{
  return minimum;
}


ClassImp(LinearScalarDoubleRandomGenerator);


LinearScalarDoubleRandomGenerator::LinearScalarDoubleRandomGenerator(double minimum,double maximum, Random * randomGenerator)
:
ScalarDoubleRandomGenerator(minimum*minimum, maximum*maximum, randomGenerator)
{ }

// Always return the same selected value
double  LinearScalarDoubleRandomGenerator::generate()
{
  // cout << " LinearScalarDoubleRandomGenerator:: generate() was called." << endl;
  double rr = randomGenerator->Rndm();
  return sqrt( rr*maximum + (1-rr)*minimum);
}

ClassImp(ExpScalarDoubleRandomGenerator);


ExpScalarDoubleRandomGenerator::ExpScalarDoubleRandomGenerator(double minimum,
                                                               double slope,
                                                               Random * randomGenerator)
:
ScalarDoubleRandomGenerator(minimum,slope,randomGenerator)
{  }

// Always return the same selected value
double  ExpScalarDoubleRandomGenerator::generate()
{

  // used stored value of maximum as slope
  return minimum + randomGenerator->Exp(maximum);
}

ClassImp(HistoScalarDoubleRandomGenerator);


HistoScalarDoubleRandomGenerator::HistoScalarDoubleRandomGenerator(TH1 * theHistogram)
:
ScalarDoubleRandomGenerator(0, 0, 0),
histogram (theHistogram)
{  }

// Always return the same selected value
double  HistoScalarDoubleRandomGenerator::generate()
{
  return histogram->GetRandom();
}

ClassImp(VectorRandomGenerator);

VectorRandomGenerator::VectorRandomGenerator(Random * rg)
:
RandomGenerator(rg)
{
}

void VectorRandomGenerator::generate(double &theta, double &phi, double & ax, double & ay,double & az)
{
  double cosTh = -1.0 + 2.0 * randomGenerator->Rndm();
  double sinTh = sqrt(1.0 - cosTh*cosTh);
  theta = acos(cosTh);
  phi   = 2.0*3.1415927*randomGenerator->Rndm();
  ax = sinTh * cos(phi);
  ay = sinTh * sin(phi);
  az = cosTh;
}
