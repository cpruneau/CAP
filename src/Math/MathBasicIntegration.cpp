#include "MathBasicIntegration.hpp"

namespace CAP
{

namespace Math
{

//!
//! Integration of a function based on values obtained from that function and computed at regular
//! intervals of size 'stepSize'.
//!
double integrationSimpsonRule(double* values, int nValues, double stepSize) throw (MathException)
{
  if (!values)     throw MathException("First argument is a null pointer","integrationSimpsonRule()");
  if (nValues<3)   throw MathException("nValues<3","integrationSimpsonRule()");
  if (stepSize<=0) throw MathException("stepSize<=0","integrationSimpsonRule()");
  double sum = 0;
  for (int j=0; j<nValues-2; j+=2)
    sum += values[j]+4.0*values[j+1]+values[j+2];
  sum *= stepSize/3.0;
  return sum;
}

//!
//! Integration of a function based on values obtained from that function and computed at regular
//! intervals of size 'stepSize'.
//!
double integrationSimpsonRule(const vector<double> & values, double stepSize) throw (MathException)
{
  int nValues = values.size();
  if (nValues<3)   throw MathException("values.size()<3","integrationSimpsonRule()");
  if (stepSize<=0) throw MathException("stepSize<=0","integrationSimpsonRule()");
  double sum = 0;
  for (int j=0; j<nValues-2; j+=2)
    sum += values[j]+4.0*values[j+1]+values[j+2];
  sum *= stepSize/3.0;
  return sum;
}



} // namespace Math

} // namespace CAP

