#ifndef CAP_BesselFunctions
#define CAP_BesselFunctions
#include "MathBasicFunctions.hpp"

using namespace std;


//!
//! Compendium of basic and essential mathematical  functions
//!  By convention, all function names  begin with a lower case letter.

namespace CAP
{
namespace Math
{

//! bessel functions
double besselI(int n,double x) throw (MathException);
double besselK(int n,double x) throw (MathException);
double besselI0(double x);
double besselK0(double x) throw (MathException);
double besselI1(double x);
double besselK1(double x) throw (MathException);
double besselJ0(double x);
double besselJ1(double x);
double besselY0(double x);
double besselY1(double x);
double struveH0(double x);
double struveH1(double x);
double struveL0(double x);
double struveL1(double x);

} // namespace Math
} // namespace CAP

#endif
