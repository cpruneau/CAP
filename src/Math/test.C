//#ifndef CAP_Complex
//#define CAP_Complex
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

namespace CAP
{
namespace Math
{

double pi()
  {
  return 3.1415927;
  }

  double sign(double x, double y)
  {
  if (y>=0) return x;
  else return -x;
  }

  double power(double x, double y)
  {
  return std::exp(y*std::log(x));
  }


///#endif






void test()
{
  using CAP::Math::Complex;
  using CAP::Math::ComplexF;
  using CAP::Math::ComplexD;
  using CAP::Math::ComplexLD;
//  using ComplexF = Complex<float>;
//  using ComplexD = Complex<double>;

  ComplexLD  x(2,2);
  ComplexF   y(1,1);
  ComplexLD  z = y + x;
  ComplexLD  w(y);
  long double a = 55;
  cout << "a             : " <<  a << endl;
  cout << "x             : " <<  x << endl;
  cout << "y             : " <<  y << endl;
  cout << "z             : " <<  z << endl;
  cout << "w(y)          : " <<  w << endl;
  cout << "a+w           : " <<  a+w << endl;
  cout << "w+a           : " <<  w+a << endl;
  cout << "a-w           : " <<  a-w << endl;
  cout << "w-a           : " <<  w-a << endl;
  cout << "w+I()         : " <<  w+ComplexD::I() << endl;
  cout << "a*w           : " <<  a*w << endl;
  cout << "w*a           : " <<  w*a << endl;
  cout << "a/w           : " <<  a/w << endl;
  cout << "w/a           : " <<  w/a << endl;
  cout << "z.modulus()   : " <<  z.modulus() << endl;
  cout << "z.modulus2()  : " <<  z.modulus2() << endl;
  cout << "sqrt(z)       : " <<  sqrt(z) << endl;
  cout << "exp(z)        : " <<  exp(z) << endl;
  cout << "log(z)        : " <<  log(z) << endl;
  cout << "sinh(z)       : " <<  sinh(z) << endl;
  cout << "cosh(z)       : " <<  cosh(z) << endl;
  cout << "tanh(z)       : " <<  tanh(z) << endl;
  cout << "sin(z)        : " <<  sin(z) << endl;
  cout << "cos(z)        : " <<  cos(z) << endl;
  cout << "tan(z)        : " <<  tan(z) << endl;
  cout << "w             : " <<  w << endl;
  cout << "normalize(z)  : " <<  normalize(z)<< endl;
  cout << "conjugate(z)  : " <<  conjugate(z)<< endl;
  cout << "power(z,4)    : " <<  power(z,4)<< endl;
  cout << "power(z,5.)   : " <<  power(z,5.4)<< endl;
  cout << "power(z,y)    : " <<  power(z,y)<< endl;
  cout << "power(z,z)    : " <<  power(z,z)<< endl;
  cout << "minimum(z,y)  : " <<  minimum(z,y)<< endl;
  cout << "range(x,z,y)  : " <<  range(x,z,y)<< endl;
  cout << "cin >> y        " << endl;
  cin >> y;
  cout << " new y        : " << y << endl;
//  cout << " x: " << x << endl;

}
