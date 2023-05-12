#ifndef CAP_MathBasicIntegration
#define CAP_MathBasicIntegration
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include "MathConstants.hpp"
#include "MathBasicFunctions.hpp"

using namespace std;

//!
//! Compendium of basic and essential mathematical  functions
//!  By convention, all function names  begin with a lower case letter.
//!
namespace CAP
{
namespace Math
{

double integrationSimpsonRule(double* , int, double);
double integrationSimpsonRule(const vector<double> &, double);


} // namespace Math

} // namespace CAP

#endif
