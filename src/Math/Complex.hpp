#ifndef CAP_Complex
#define CAP_Complex
#include <cmath>
#include <iostream>
#include "TString.h"
#include "MathConstants.hpp"

//using CAP::Math
namespace CAP
{
namespace Math
{
//!
//! Class to encapsultate complex numbers
//!
template <typename T>
class Complex
{
protected:

  T re;  //! real part
  T im;  //! imaginary part

public:

  Complex(): re (0), im(0)
  {    }

  Complex(T _real, T _imaginary=0, bool _polar=false)
  :
  re(_real),
  im(_imaginary)
  {
  if (_polar)
    {
    if (re<0) re=-re;
    re = re*std::cos( (T) im);
    im = re*std::sin( (T) im);
    }
  }

  template <typename N>
  Complex(const Complex<N> c)
  :
  re(c.Re()),
  im(c.Im())
  {  }


  virtual ~Complex() { }

  // constants
  static Complex I() {return Complex(0,1);}

  static Complex One() {return Complex(1,0);}

  // getters and setters
  T Re() const {return re;}

  T Im() const {return im;}

  T modulus() const {return std::sqrt(re*re +im*im);}

  T modulus2() const {return re*re +im*im;}

  T theta() const {return (im||re )?std::atan2(im,re ):0;}

  Complex operator()(T x, T y, bool polar=false)
  {
  if (polar)
    { re  = x*std::cos(y); im = x*std::sin(y); }
  else
    { re  = x; im = y; }
  return *this;
  }

  template <typename N>
  Complex operator = (const Complex<N> & c) const
  {
  re = c.Re();
  im = c.Im();
  return *this;
  }

  // Simple operators complex - complex
  template <typename N>
  Complex operator *(const Complex<N> & c) const
  {
  return Complex(re *c.Re() -im*c.Im(),re *c.Im()+im*c.Re() );
  }

  template <typename N>
  Complex operator +(const Complex<N> & c) const
  {return Complex(re +c.Re() , im+c.Im());}

  template <typename N>
  Complex operator /(const Complex<N> & c) const
  {return Complex(re *c.Re() +im*c.Im(),-re *c.Im()+im*c.Re() )/c.modulus2();}

  template <typename N>
  Complex operator -(const Complex<N> & c) const
  {return Complex(re -c.Re() , im-c.Im());}

  template <typename N>
  Complex operator *=(const Complex<N> & c)
  {return ((*this) = (*this) * c);}

  template <typename N>
  Complex operator +=(const Complex<N> & c)
  {return ((*this) = (*this) + c);}

  template <typename N>
  Complex operator /=(const Complex<N> & c)
  {return ((*this) = (*this) / c);}

  template <typename N>
  Complex operator -=(const Complex<N> & c)
  {return ((*this) = (*this) - c);}

  Complex operator -()
  {return Complex(-re ,-im);}

  Complex operator +()
  {return *this;}

  // Simple operators complex - double
  Complex operator *(T c) const
  {return Complex(re *c,im*c);}

  Complex operator +(T c) const
  {return Complex(re +c, im);}

  Complex operator /(T c) const
  {return Complex(re /c,im/c);}

  Complex operator -(T c) const
  {return Complex(re -c, im);}

  // Simple operators double - complex
  friend Complex operator *(T d, const Complex & c)
  {return Complex(d*c.Re() ,d*c.Im());}

  friend Complex operator +(T d, const Complex & c)
  {return Complex(d+c.Re() , c.Im());}

  friend Complex operator /(T d, const Complex & c)
  {return Complex(d*c.Re() ,-d*c.Im())/c.modulus2();}

  friend Complex operator -(T d, const Complex & c)
  {return Complex(d-c.Re() , -c.Im());}

  ClassDef(Complex,1)  //Complex Class
};

template <typename T>
Complex<T> sqrt(const Complex<T> &c)
{
  return Complex<T>(std::sqrt(c.modulus()),0.5*c.theta(),true);
}

template <typename T>
Complex<T>  exp(const Complex<T>  &c)
{return Complex<T> (std::exp(c.Re() ),c.Im(),true);}

template <typename T>
Complex<T>  log(const Complex<T>  &c)
{return Complex<T>(0.5*std::log(c.modulus2()),c.theta());}

template <typename T>
Complex<T>  log2(const Complex<T>  &c)
{return log(c)/std::log(2);}

template <typename T>
Complex<T>  log10(const Complex<T>  &c)
{return log(c)/std::log(10);}

template <typename T>
Complex<T>  sin(const Complex<T> &c)
{return Complex<T> (std::sin(c.Re() )*std::cosh(c.Im()),
                    std::cos(c.Re() )*std::sinh(c.Im()));}

template <typename T>
Complex<T>  cos(const Complex<T>  &c)
{return Complex<T> (std::cos(c.Re() )*std::cosh(c.Im()),
                    -std::sin(c.Re() )*std::sinh(c.Im()));}

template <typename T>
Complex<T>  tan(const Complex<T>  &c)
{Complex<T>  cc=cos(c); return sin(c)*conjugate(cc)/cc.modulus2();}

template <typename T>
Complex<T>  asin(const Complex<T>  &c)
{return -Complex<T>::I()*log(Complex<T>::I()*c+sign(1.,c.Im())*sqrt(1.-c*c));}

template <typename T>
Complex<T>  acos(const Complex<T>  &c)
{return -Complex<T>::I()*log(c+sign(1.,c.Im())*sqrt(c*c-1.));}

template <typename T>
Complex<T>  atan(const Complex<T>  &c)
{return -0.5*Complex<T>::I()*log((1.+Complex<T>::I()*c)/(1.-Complex<T>::I()*c));}

template <typename T>
Complex<T>  sinh(const Complex<T>  &c)
{return Complex<T> (std::sinh(c.Re() )*std::cos(c.Im()),
                    std::cosh(c.Re() )*std::sin(c.Im()));}

template <typename T>
Complex<T>  cosh(const Complex<T>  &c)
{return Complex<T> (std::cosh(c.Re() )*std::cos(c.Im()),
                    std::sinh(c.Re() )*std::sin(c.Im()));}

template <typename T>
Complex<T>  tanh(const Complex<T>  &c)
{Complex<T>  cc=cosh(c); return sinh(c)*conjugate(cc)/cc.modulus2();}

template <typename T>
Complex<T>  asinh(const Complex<T>  &c)
{return log(c+sign(1.,c.Im())*sqrt(c*c+1.));}

template <typename T>
Complex<T>  acosh(const Complex<T>  &c)
{return log(c+sign(1.,c.Im())*sqrt(c*c-1.));}

template <typename T>
Complex<T>  atanh(const Complex<T>  &c)
{return 0.5*log((1.+c)/(1.-c));}

template <typename T>
T absolute(const Complex<T>  &c)
{return c.modulus();}

template <typename T, typename N>
Complex<T>  power(const Complex<T> & x, const Complex<N> & y)
{
  T lmodulus=std::log(x.modulus());
  T theta=x.theta();
  return Complex<T> (std::exp(lmodulus*y.Re()-theta*y.Im()),
                     lmodulus*y.Im()+theta*y.Re(),true);}

template <typename T>
Complex<T>  power(const Complex<T>& x, double y)
{return Complex<T> (pow(x.modulus(),y),x.theta()*y,true);}

template <typename T>
Complex<T>  power(double x, const Complex<T> & y)
{T lmodulus=std::log(std::fabs(x));
  T theta=(x>0)?0:pi();
  return Complex<T> (std::exp(lmodulus*y.Re()-theta*y.Im()),
                     lmodulus*y.Im()+theta*y.Re(),true);}

template <typename T>
Complex<T>  power(const Complex<T> & x, int y)
{return Complex<T> (pow(x.modulus(),y),x.theta()*y,true);}


template <typename T, typename N>
Complex<T>  minimum(const Complex<T>  &a, const Complex<N>  &b)
{return a.modulus()<=b.modulus()?Complex<T>(a):Complex<T>(b);}

template <typename T, typename N>
Complex<T> maximum(const Complex<T> &a, const Complex<N> &b)
{return a.modulus()>=b.modulus()?Complex<T>(a):Complex<T>(b);}

template <typename T>
Complex<T> normalize(const Complex<T> &c)
{return Complex<T>(1.,c.theta(),true);}

template <typename T>
Complex<T> conjugate(const Complex<T> &c)
{return Complex<T>(c.Re(),-c.Im());}

template <typename T,typename P,typename Q>
Complex<T> range(const Complex<T> &lb, const Complex<P> &ub, const Complex<Q> &c)
{return maximum(lb,minimum(c,ub));}

template <typename T>
std::ostream& operator<<(std::ostream& out, const CAP::Math::Complex<T>& c)
{
  out << "(" << c.Re()  << "," << c.Im() << "i)";
  return out;
}

template <typename T>
std::istream& operator>>(std::istream& in, CAP::Math::Complex<T>& c)
{
  T x;
  T y;
  in >> x >> y;
  c(x,y);
  return in;
}


typedef  Complex<float>  ComplexF;
typedef  Complex<double> ComplexD;
typedef  Complex<long double> ComplexLD;

}
}

#endif

