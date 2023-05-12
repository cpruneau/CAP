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

void   interpolation1DLinear(double* Tb_x, double* Tb_y, double* xval, double* yval, int nTblength);
//double interpolation2D_bilinear(Table2D* tb_ptr, double varX, double varY, int Iwarning)
double interpolationCubicDirect(const vector<double> &x, const vector<double> &y, double xx);
double interpolationCubicMono(const vector<double> &x, const vector<double> &y, double xx);
double interpolationLinearDirect(const vector<double> &x, const vector<double> &y, double xx);
double interpolationLinearMono(const vector<double> &x, const vector<double> &y, double xx);
double interpolationNearestDirect(const vector<double> &x, const vector<double> &y, double xx);
double interpolationNearestMono(const vector<double> &x, const vector<double> &y, double xx);

} // namespace Math
} // namespace CAP

#endif



