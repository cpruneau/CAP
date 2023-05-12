#ifndef CAP_MathBasicInterpolation
#define CAP_MathBasicInterpolation
#include <iostream>
#include <vector>
#include <cstdlib>
//#include <algorithm>
//#include <limits>
#include <cmath>
using namespace std;

namespace CAP
{
namespace Math
{

void   interpolation1DLinear(double* Tb_x, double* Tb_y, double* xval, double* yval, int nTblength) throw (MathException);
//double interpolation2D_bilinear(Table2D* tb_ptr, double varX, double varY, int Iwarning)
double interpolationCubicDirect(const vector<double> &x, const vector<double> &y, double xx) throw (MathException);
double interpolationCubicMono(const vector<double> &x, const vector<double> &y, double xx) throw (MathException);
double interpolationLinearDirect(const vector<double> &x, const vector<double> &y, double xx) throw (MathException);
double interpolationLinearMono(const vector<double> &x, const vector<double> &y, double xx) throw (MathException);
double interpolationNearestDirect(const vector<double> &x, const vector<double> &y, double xx) throw (MathException);
double interpolationNearestMono(const vector<double> &x, const vector<double> &y, double xx) throw (MathException);

} // namespace Math
} // namespace CAP

#endif



