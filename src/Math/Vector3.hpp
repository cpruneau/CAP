#ifndef CAP_Vector3
#define CAP_Vector3
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include "TString.h"
#include "Exceptions.hpp"
#include "MathConstants.hpp"

//!
//! The Vector3 class encapuslate physical 3D vectors.
//!
//!It is templated to enable different types of storage.
//!Some functions are only meaningfull in 3 or 4 dimensions. If called for lower dimension, they throw an
//!exception of type MathException.
//!
//!
//!storageType for 3D
//!0  : x, y, z (cartesian coordinates)
//!1  : phi, theta, r (spherical coordinates)
//!2  : phi, rho, z (cylindrical coordinates)
//!
namespace CAP
{
namespace Math
{

//class Rotation<Q>;

template <typename T>
class Vector3
{
protected:
  T v1, v2, v3;
  int storageType;

public:

  Vector3();

  Vector3(const T & v1, const T & v2, const T & v3, int storageType=0);

  Vector3(T * values, int storageType=0);

  Vector3(const std::vector<T> values, int storageType=0);

  Vector3(const Vector3<T> & source);

  int storage() const
  {
  return storageType;
  }


  virtual ~Vector3() {};
  virtual T  operator() (unsigned int index) const throw (MathException);
  virtual T  operator[] (unsigned int index) const throw (MathException);
  virtual T  x()  const throw (MathException);
  virtual T  y()  const throw (MathException);
  virtual T  z()  const throw (MathException);

  void setXYZ(const T * values)  throw (MathException);

  void setXYZ(const std::vector<T> & values)  throw (MathException);

  void setXYZ(const T & x, const T & y, const T & z)  throw (MathException);

  void setFromXYZ(const T & x, const T & y, const T & z)  throw (MathException);

  void setPhiThetaR(const T & phi, const T & theta, const T & r)  throw (MathException);

  void setPhiRhoZ(const T & phi, const T & rho, const T & z)  throw (MathException);

  T  phi() const  throw (MathException);
  T  cosPhi() const  throw (MathException);
  T  sinPhi() const  throw (MathException);
  T  theta() const  throw (MathException);
  T  cosTheta() const  throw (MathException);
  T  sinTheta() const  throw (MathException);
  T  modulus() const  throw (MathException);
  T  modulusSquare() const  throw (MathException);
  T  perp() const  throw (MathException);
  T  perpSquare() const  throw (MathException);

  //! The angle w.r.t. another 3-Vector3.
  template <typename Q>
  T  angle(const Vector3<Q> & otherVector) const
  {
  T sp = scalarProduct(otherVector);
  T m1 = modulus();
  T m2 = otherVector.modulus();
  if (m1>0 && m2>0)
    return sp/(m1*m2);
  else
    return 0;
  }

  template <typename Q>
  Vector3 & operator= (const Vector3<Q> & otherVector)
  {
  if (this!=&otherVector)
    {
    v1 = otherVector.v1;
    v2 = otherVector.v2;
    v3 = otherVector.v3;
    storageType = otherVector.storageType;
    }
  return *this;
  }

  template <typename Q>
  bool  operator== (const Vector3<Q> & otherVector) const
  {
  return (x()==otherVector.x()) && (y()==otherVector.y()) && (z()==otherVector.z());
  }

  template <typename Q>
  bool  operator!= (const Vector3<Q> & otherVector) const
  {
  return (x()!=otherVector.x()) || (y()!=otherVector.y()) || (z()!=otherVector.z());
  }

  template <typename Q>
  Vector3<T> & operator+= (const Vector3<Q> & otherVector)
  {
  T xx = x() + otherVector.x();
  T yy = y() + otherVector.y();
  T zz = z() + otherVector.z();
  setFromXYZ(xx,yy,zz);
  return *this;
  }

  template <typename Q>
  Vector3<T> & operator-= (const Vector3<Q> & otherVector)
  {
  T xx = x() - otherVector.x();
  T yy = y() - otherVector.y();
  T zz = z() - otherVector.z();
  setFromXYZ(xx,yy,zz);
  return *this;
  }

  //! Unary minus.
  Vector3 operator - () const throw (MathException);

  Vector3 & operator *= (const T & scaleFactor);

  Vector3 & normalize() throw (MathException);


  //! unit Vector3 parallel to this vecotr.
  Vector3 unit() const  throw (MathException);

  // Scalar product.
  template <typename Q>
  T  scalarProduct(const Vector3<Q> & otherVector) const
  {
  return x()*otherVector.x() + y()*otherVector.y() + z()*otherVector.z();
  }

  //! Create a new vector which is the cross product of this and the given vector.
  template <typename Q>
  Vector3 crossProduct(const Vector3<Q> & otherVector) const
  {
  double xx = y() * otherVector.z() - z() * otherVector.y();
  double yy = z() * otherVector.x() - x() * otherVector.z();
  double zz = x() * otherVector.y() - y() * otherVector.x();
  return Vector3<T>(xx,yy,zz,0);
  }

  //! Active rotation of this Vector3 by the given angle relative to the x-axis
  void rotateX(const T & angle);

  //! Active rotation of this Vector3 by the given angle relative to the y-axis
  void rotateY(const T & angle);

  //! Active rotation of this Vector3 by the given angle relative to the z-axis
  void rotateZ(const T & angle);

  //! Active rotation of this Vector3 by the given angle relative to the given Vector3
  //void rotate(const T & angle, const Vector3 & otherVector3);

  // Active rotation of this Vector3 by the given euler angles
  //void rotate(const T & angle1, const T & angle2, const T & angle2);

  Vector3 projectOntoXY() const;
  Vector3 projectOntoYZ() const;
  Vector3 projectOntoXZ() const;

  virtual void print() const throw (MathException);

  ClassDef(Vector3,1)

 // friend class CAP::Math::LorentzVector;
};


//! Addition of two Vector3s.
template <typename T, typename Q>
Vector3<T> operator+ (const Vector3<T> & left, const Vector3<Q> & right)
{
  T xx = left.x() + right.x();
  T yy = left.y() + right.y();
  T zz = left.z() + right.z();
  return Vector3<T>(xx,yy,zz,left.storage());
}

//! Subtraction of two Vector3s.
template <typename T, typename Q>
Vector3<T> operator- (const Vector3<T> & left, const Vector3<Q> & right)
{
  T xx = left.x() - right.x();
  T yy = left.y() - right.y();
  T zz = left.z() - right.z();
  return Vector3<T>(xx,yy,zz,left.storage());
}

//! Scalar product of two Vector3s.
template <typename T, typename Q>
T operator * (const Vector3<T> & left, const Vector3<Q> & right)
  {
  return left.x()*right.x() + left.y()*right.y() + left.z()*right.z();
  }


//! Scaling by a scalar value from the right
template <typename T, typename Q>
Vector3<T> operator* (const Vector3<T> & left, Q  a)
  {
  Vector3<T> v(0,0,0,left.storage());
  v.setFromXYZ(a*left.x(), a*left.y(), a*left.z());
  return v;
  }


//! Scaling by a scalar value from the left
template <typename T, typename Q>
Vector3<T> operator* (const Q & a, const Vector3<T> & right)
  {
  Vector3<T> v(0,0,0,right.storageType);
  v.setFromXYZ(a*right.x(), a*right.y(), a*right.z());
  return v;
  }

// ====

template <typename T>
Vector3<T>::Vector3()
:
v1(0),
v2(0),
v3(0),
storageType(0)
{ }

template <typename T>
Vector3<T>::Vector3(const T & _v1, const T & _v2, const T & _v3, int _storageType)
:
v1(_v1),
v2(_v2),
v3(_v3),
storageType(_storageType)
{  }


template <typename T>
Vector3<T>::Vector3(T * _values, int _storageType)
:
v1(_values[0]),
v2(_values[1]),
v3(_values[2]),
storageType(_storageType)
{ }

template <typename T>
Vector3<T>::Vector3(std::vector<T> _values, int _storageType)
:
v1(_values[0]),
v2(_values[1]),
v3(_values[2]),
storageType(_storageType)
{ }

template <typename T>
Vector3<T>::Vector3(const Vector3<T> & source)
:
v1(source.v1),
v2(source.v1),
v3(source.v2),
storageType(source.storageType)
{ }

//!
//! Returns the value stored at the give index.
//!
template <typename T>
T Vector3<T>::operator() (unsigned int index) const throw (MathException)
{
  switch (index)
    {
      case 0: return v1;
      case 1: return v2;
      case 2: return v3;
      default: throw MathException("index>2","Vector3<T>::operator()");
    }
}


template <typename T>
T Vector3<T>::operator[] (unsigned int index) const throw (MathException)
{
  switch (storageType)
    {
      case 0: // cartesian coordinates storage and output
      switch (index)
        {
          case 0: return v1; //!> x
          case 1: return v2; //!> y
          case 2: return v3;              //!> z
          default: throw MathException("index>2","Vector3<T>::operator []");
        }

      case 1: // spherical coordinates storage and cartesian output
      switch (index)
        {
          case 0: return v3*std::sin(v2)*std::cos(v1); //!> x
          case 1: return v3*std::sin(v2)*std::sin(v1); //!> y
          case 2: return v3*std::cos(v2);              //!> z
          default: throw MathException("index>2","Vector3<T>::operator []");
        }

      case 2: // cylindrical coordinates storage and cartesian output
      switch (index)
        {
          case 0: return v2*std::cos(v1); //!> x
          case 1: return v2*std::sin(v1); //!> y
          case 2: return v3;              //!> z
          default: throw MathException("index>2","Vector3<T>::operator []");
        }

      default: throw MathException("Internal error","Vector3<T>::operator[]");

    }
}


template <typename T>
T   Vector3<T>::x()  const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v1; //!> x
      case 1: return v3*std::sin(v2)*std::cos(v1); //!> x
      case 2: return v2*std::cos(v1); //!> x
      default: throw MathException("Internal error","Vector3<T>::x()");
    }
}

template <typename T>
T   Vector3<T>::y()  const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v2; //!> y
      case 1: return v3*std::sin(v2)*std::sin(v1); //!> x
      case 2: return v2*std::sin(v1); //!> y
      default: throw MathException("Internal error","Vector3<T>::y()");
    }
}


template <typename T>
T   Vector3<T>::z()  const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v3;              //!> z
      case 1: return v3*std::cos(v2); //!> x
      case 2: return v3;              //!> z
      default: throw MathException("Internal error","Vector3<T>::z()");
    }
}

template <typename T>
void Vector3<T>::setXYZ(const T * values) throw (MathException)
{
  v1 = values[0];
  v2 = values[1];
  v3 = values[2];
  storageType = 0;
}

template <typename T>
void Vector3<T>::setXYZ(const std::vector<T> & values) throw (MathException)
{
  v1 = values.x();
  v2 = values.y();
  v3 = values.z();
  storageType = 0;
}

template <typename T>
void Vector3<T>::setXYZ(const T & x, const T & y, const T & z) throw (MathException)
{
  v1 = x;
  v2 = y;
  v3 = z;
  storageType = 0;
}

template <typename T>
void Vector3<T>::setFromXYZ(const T & x, const T & y, const T & z) throw (MathException)
{
  switch (storageType)
    {
      default: throw MathException("Internal error","Vector3<T>::setFromXYZ()");
      break;

      case 0:
      v1 = x;
      v2 = y;
      v3 = z;
      break;

      case 1:
      {
      T rhoSq = x*x + y*y;
      v1 = std::atan2(y,x);
      v2 = std::atan2(std::sqrt(rhoSq),z);
      v3 = std::sqrt(rhoSq + z*z);
      break;
      }

      case 2:
      v1 = std::atan2(y,x);
      v2 = std::sqrt(x*x + y*y);
      v3 = z;
      break;
    }
}


template <typename T>
void Vector3<T>::setPhiThetaR(const T & phi, const T & theta, const T & r) throw (MathException)
{
  v1 = phi;
  v2 = theta;
  v3 = r;
  storageType = 1;
}


template <typename T>
void Vector3<T>::setPhiRhoZ(const T & phi, const T & rho, const T & z) throw (MathException)
{
  v1 = phi;
  v2 = rho;
  v3 = z;
  storageType = 2;
}

template <typename T>
T  Vector3<T>::phi() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return std::atan2(v2,v1);
      case 1: return v1;
      case 2: return v1;
      default: throw MathException("Internal error","Vector3<T>::phi()");
    }
}

template <typename T>
T  Vector3<T>::cosPhi() const  throw (MathException)
{
  switch (storageType)
    {
      case 0: return v1/std::sqrt(v1*v1+v2*v2);
      case 1: return std::cos(v1);
      case 2: return std::cos(v1);
      default: throw MathException("Internal error","Vector3<T>::cosPhi()");
    }
}

template <typename T>
T  Vector3<T>::sinPhi() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v2/std::sqrt(v1*v1+v2*v2);
      case 1: return std::sin(v1);
      case 2: return std::sin(v1);
      default: throw MathException("Internal error","Vector3<T>::sinPhi()");
    }
}


template <typename T>
T  Vector3<T>::theta() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return std::atan2(std::sqrt(v1*v1+v2*v2),v3);
      case 1: return v2;
      case 2: return std::atan2(v2,v3);
      default: throw MathException("Internal error","Vector3<T>::theta()");
    }
}

template <typename T>
T  Vector3<T>::cosTheta() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v3/std::sqrt(v1*v1+v2*v2+v3*v3);
      case 1: return std::cos(v2);
      case 2: return v3/std::sqrt(v2*v2+v3*v3);
      default: throw MathException("Internal error","Vector3<T>::cosTheta()");
    }
}

template <typename T>
T  Vector3<T>::sinTheta() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v3/std::sqrt(v1*v1+v2*v2+v3*v3);
      case 1: return std::cos(v2);
      case 2: return v3/std::sqrt(v2*v2+v3*v3);
      default: throw MathException("Internal error","Vector3<T>::sinTheta()");
    }
}

template <typename T>
T  Vector3<T>::modulus() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return std::sqrt(v1*v1+v2*v2+v3*v3);
      case 1: return v3;
      case 2: return std::sqrt(v2*v2+v3*v3);
      default: throw MathException("Internal error","Vector3<T>::modulus()");
    }
}

template <typename T>
T  Vector3<T>::modulusSquare() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v1*v1+v2*v2+v3*v3;
      case 1: return v3*v3;
      case 2: return v2*v2+v3*v3;
      default: throw MathException("Internal error","Vector3<T>::modulusSquare()");
    }
}

template <typename T>
T  Vector3<T>::perp() const  throw (MathException)
{
  switch (storageType)
    {
      case 0: return std::sqrt(v1*v1+v2*v2);
      case 1: return v3*std::sin(v2);
      case 2: return v2;
      default: throw MathException("Internal error","Vector3<T>::perp()");
    }
}

template <typename T>
T  Vector3<T>::perpSquare() const   throw (MathException)
{
  switch (storageType)
    {
      default:
      throw MathException("Internal error","Vector3<T>::perpSquare()");
      break;

      case 0:
      return v1*v1+v2*v2;
      break;

      case 1:
      {
        T rho = v3*std::sin(v2);
        return rho*rho;
        break;
      }
      case 2: return v2*v2;
    }
}





//! Unary minus.
template <typename T>
Vector3<T> Vector3<T>::operator- () const throw (MathException)
{
  Vector3 v(0,0,0,storageType);
  switch (storageType)
    {
      default: throw MathException("Internal error","Vector3<T>::operator- ()");

      case 0:
      v.v1 = -v1;
      v.v2 = -v2;
      v.v3 = -v3;
      break;

      case 1:
      {
      T phi = pi()+v1;
      v.v1 = phi>twoPi() ?  phi -= twoPi() : phi;
      v.v2 = pi()-v2;
      v.v3 = v3;
      break;
      }

      case 2:
      {
      T phi = pi()+v1;
      v.v1 = phi>twoPi() ?  phi += twoPi() : phi;
      v.v2 = v2;
      v.v3 = -v3;
      break;
      }
    }
  return v;
}

template <typename T>
Vector3<T> & Vector3<T>::operator *= (const T & scaleFactor)
{
  switch (storageType)
    {
      case 0:
      v1 *= scaleFactor;
      v2 *= scaleFactor;
      v3 *= scaleFactor;
      break;

      case 1:
      v3 *= scaleFactor;
      break;

      case 2:
      v2 *= scaleFactor;
      v3 *= scaleFactor;
      break;
    }
  return *this;
}

//! Normalize this vector to be a unitvector
template <typename T>
Vector3<T>& Vector3<T>::normalize() throw (MathException)
{
  switch (storageType)
    {
      case 0:
      {
      T r = std::sqrt(v1*v1+v2*v2+v3*v3);
      if (r==0) throw MathException("Cannot produced unit vector from null vector","Vector3<T>::unit()");
      v1 /= r;
      v2 /= r;
      v3 /= r;
      break;
      }
      case 1:
      if (v3==0) throw MathException("Cannot produced unit vector from null vector","Vector3<T>::unit()");
      v3 = 1;
      break;

      case 2:
      {
      T r = std::sqrt(v2*v2+v3*v3);
      if (r==0) throw MathException("Cannot produced unit vector from null vector","Vector3<T>::unit()");
      v2 /= r;
      v3 /= r;
      break;
      }
      default:
      throw MathException("Internal error","Vector3<T>::normalize()");
    }
  return *this;
}

//! unit Vector3 parallel to this vecotr.
template <typename T>
Vector3<T> Vector3<T>::unit() const throw (MathException)
{
  switch (storageType)
    {
      default:
      throw MathException("Internal error","Vector3<T>::unit()");

      case 0:
      {
      T r = std::sqrt(v1*v1+v2*v2+v3*v3);
      if (r==0) throw MathException("Cannot produced unit vector from null vector","Vector3<T>::unit()");
      Vector3<T> vxyz(v1/r,v2/r,v3/r,storageType);
      return vxyz;
      break;
      }

      case 1:
      {
      if (v3==0) throw MathException("Cannot produced unit vector from null vector","Vector3<T>::unit()");
      Vector3<T> vr(v1,v2,1.0,storageType);
      return vr;
      break;
      }

      case 2:
      {
      T r = std::sqrt(v2*v2+v3*v3);
      if (r==0) throw MathException("Cannot produced unit vector from null vector","Vector3<T>::unit()");
      Vector3<T> vrho(v1,v2/r,v3/r,storageType);
      return vrho;
      }
    }
}



//! Active rotation of this Vector3 by the given angle relative to the x-axis
template <typename T>
void Vector3<T>::rotateX(const T & angle)
{
  T s  = std::sin(angle);
  T c  = std::cos(angle);
  T xx   = x();
  T yy1  = y();
  T zz   = z();
  T yy   = c*yy1 - s*zz;
  zz     = s*yy1 + c*zz;
  setFromXYZ(xx,yy,zz);
}

//! Active rotation of this Vector3 by the given angle relative to the y-axis
template <typename T>
void Vector3<T>::rotateY(const T & angle)
{
  T s  = std::sin(angle);
  T c  = std::cos(angle);
  T xx   = x();
  T yy   = y();
  T zz1  = z();
  T zz   = c*zz1 - s*xx;
  xx     = s*zz1 + c*xx;
  setFromXYZ(xx,yy,zz);
}

//! Active rotation of this Vector3 by the given angle relative to the z-axis
template <typename T>
void Vector3<T>::rotateZ(const T & angle)
{
  T s  = std::sin(angle);
  T c  = std::cos(angle);
  T xx1  = x();
  T yy   = y();
  T zz   = z();
  T xx   = c*xx1 - s*yy;
  yy     = s*xx1 + c*yy;
  setFromXYZ(xx,yy,zz);
}


template <typename T>
Vector3<T> Vector3<T>::projectOntoXY() const
{
  return Vector3<T>(x(), y(), 0, 0);
}

template <typename T>
Vector3<T> Vector3<T>::projectOntoYZ() const
{
  return Vector3<T>(0, y(), z(), 0);
}


template <typename T>
Vector3<T> Vector3<T>::projectOntoXZ() const
{
  return Vector3<T>(x(), 0, z(), 0);
}


template <typename T>
void Vector3<T>::print() const  throw (MathException)
{
  switch (storageType)
    {
      case 0:
      std::cout << "(x:" << v1 << ", y:" << v2 << ", z:" << v3 << ") " << std::endl; break;
      case 1:
      std::cout << "(phi:" << v1 << ", theta:" << v2 << ", r:" << v3 << ") " << std::endl; break;
      case 2:
      std::cout << "(phi:" << v1 << ", rho:" << v2 << ", z:" << v3 << ") " << std::endl; break;
      default: throw MathException("Internal error","Vector3<T>::print()");
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const CAP::Math::Vector3<T>& v)
{
  out << "(" << v.x()  << "," << v.y() << "," << v.z() << ")" ;
  return out;
}



typedef  Vector3<float>  Vector3F;
typedef  Vector3<double> Vector3D;
typedef  Vector3<long double> Vector3LD;

} // namespace Math
} // namespace CAP

#endif
