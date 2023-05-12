#ifndef CAP_VectorLorentz
#define CAP_VectorLorentz
#include "Vector3.hpp"

namespace CAP
{
namespace Math
{

//class Rotation<Q>;

template <typename T>
class VectorLorentz
{
protected:
  T v0;

public:

  VectorLorentz();

  VectorLorentz(const T & v0, const T & v1, const T & v2, const T & v3, int storageType=0);

  VectorLorentz(T * values, int storageType=0);

  VectorLorentz(const std::vector<T> values, int storageType=0);

  VectorLorentz(const VectorLorentz<T> & source);

  int storage() const
  {
  return storageType;
  }


  virtual ~VectorLorentz() {};
  virtual T  operator() (unsigned int index) const throw (MathException);
  virtual T  operator[] (unsigned int index) const throw (MathException);
  virtual T  x()  const throw (MathException);
  virtual T  y()  const throw (MathException);
  virtual T  z()  const throw (MathException);

  void setTXYZ(const T * values)  throw (MathException);

  void setTXYZ(const std::vector<T> & values)  throw (MathException);

  void setTXYZ(const T & x, const T & y, const T & z)  throw (MathException);

  void setFromTXYZ(const T & x, const T & y, const T & z)  throw (MathException);

  void setPhiThetaRT(const T & phi, const T & theta, const T & r, const T & t)  throw (MathException);

  void setPhiRhoZT(const T & phi, const T & rho, const T & z, const T & t)  throw (MathException);

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


  template <typename Q>
  VectorLorentz & operator= (const VectorLorentz<Q> & otherVector)
  {
  if (this!=&otherVector)
    {
    v0 = otherVector.v0;
    v1 = otherVector.v1;
    v2 = otherVector.v2;
    v3 = otherVector.v3;
    storageType = otherVector.storageType;
    }
  return *this;
  }

  template <typename Q>
  bool  operator== (const VectorLorentz<Q> & otherVector) const
  {
  return (t()==otherVector.t())  && x()==otherVector.x()) && (y()==otherVector.y()) && (z()==otherVector.z());
  }

  template <typename Q>
  bool  operator!= (const VectorLorentz<Q> & otherVector) const
  {
  return (t()!=otherVector.t()) || x()!=otherVector.x()) || (y()!=otherVector.y()) || (z()!=otherVector.z());
  }

  template <typename Q>
  VectorLorentz<T> & operator+= (const VectorLorentz<Q> & otherVector)
  {
  T tt = t() + otherVector.t();
  T xx = x() + otherVector.x();
  T yy = y() + otherVector.y();
  T zz = z() + otherVector.z();
  setFromTXYZ(tt,xx,yy,zz);
  return *this;
  }

  template <typename Q>
  VectorLorentz<T> & operator-= (const VectorLorentz<Q> & otherVector)
  {
  T tt = t() - otherVector.t();
  T xx = x() - otherVector.x();
  T yy = y() - otherVector.y();
  T zz = z() - otherVector.z();
  setFromTXYZ(tt,xx,yy,zz);
  return *this;
  }

  //! Unary minus.
  VectorLorentz operator - () const throw (MathException);

  VectorLorentz & operator *= (const T & scaleFactor);

  VectorLorentz & normalize() throw (MathException);


  //! unit VectorLorentz parallel to this vecotr.
  VectorLorentz unit() const  throw (MathException);

  // Scalar product.
  template <typename Q>
  T  scalarProduct(const VectorLorentz<Q> & otherVector) const
  {
  return x()*otherVector.x() + y()*otherVector.y() + z()*otherVector.z();
  }

  //! Create a new vector which is the cross product of this and the given vector.
  template <typename Q>
  VectorLorentz crossProduct(const VectorLorentz<Q> & otherVector) const
  {
  double xx = y() * otherVector.z() - z() * otherVector.y();
  double yy = z() * otherVector.x() - x() * otherVector.z();
  double zz = x() * otherVector.y() - y() * otherVector.x();
  return VectorLorentz<T>(xx,yy,zz,0);
  }

  //! Active rotation of this VectorLorentz by the given angle relative to the x-axis
  void rotateX(const T & angle);

  //! Active rotation of this VectorLorentz by the given angle relative to the y-axis
  void rotateY(const T & angle);

  //! Active rotation of this VectorLorentz by the given angle relative to the z-axis
  void rotateZ(const T & angle);

  //! Active rotation of this VectorLorentz by the given angle relative to the given VectorLorentz
  //void rotate(const T & angle, const VectorLorentz & otherVector3);

  // Active rotation of this VectorLorentz by the given euler angles
  //void rotate(const T & angle1, const T & angle2, const T & angle2);

  VectorLorentz projectOntoXY() const;
  VectorLorentz projectOntoYZ() const;
  VectorLorentz projectOntoXZ() const;

  virtual void print() const throw (MathException);

  ClassDef(VectorLorentz,1)

 // friend class CAP::Math::LorentzVector;
};


//! Addition of two Vector3s.
template <typename T, typename Q>
VectorLorentz<T> operator+ (const VectorLorentz<T> & left, const VectorLorentz<Q> & right)
{
  T xx = left.x() + right.x();
  T yy = left.y() + right.y();
  T zz = left.z() + right.z();
  return VectorLorentz<T>(xx,yy,zz,left.storage());
}

//! Subtraction of two Vector3s.
template <typename T, typename Q>
VectorLorentz<T> operator- (const VectorLorentz<T> & left, const VectorLorentz<Q> & right)
{
  T xx = left.x() - right.x();
  T yy = left.y() - right.y();
  T zz = left.z() - right.z();
  return VectorLorentz<T>(xx,yy,zz,left.storage());
}

//! Scalar product of two Vector3s.
template <typename T, typename Q>
T operator * (const VectorLorentz<T> & left, const VectorLorentz<Q> & right)
  {
  return left.x()*right.x() + left.y()*right.y() + left.z()*right.z();
  }


//! Scaling by a scalar value from the right
template <typename T, typename Q>
VectorLorentz<T> operator* (const VectorLorentz<T> & left, Q  a)
  {
  VectorLorentz<T> v(0,0,0,left.storage());
  v.setFromXYZ(a*left.x(), a*left.y(), a*left.z());
  return v;
  }


//! Scaling by a scalar value from the left
template <typename T, typename Q>
VectorLorentz<T> operator* (const Q & a, const VectorLorentz<T> & right)
  {
  VectorLorentz<T> v(0,0,0,right.storageType);
  v.setFromXYZ(a*right.x(), a*right.y(), a*right.z());
  return v;
  }

// ====

template <typename T>
VectorLorentz<T>::VectorLorentz()
:
v1(0),
v2(0),
v3(0),
storageType(0)
{ }

template <typename T>
VectorLorentz<T>::VectorLorentz(const T & _v1, const T & _v2, const T & _v3, int _storageType)
:
v1(_v1),
v2(_v2),
v3(_v3),
storageType(_storageType)
{  }


template <typename T>
VectorLorentz<T>::VectorLorentz(T * _values, int _storageType)
:
v1(_values[0]),
v2(_values[1]),
v3(_values[2]),
storageType(_storageType)
{ }

template <typename T>
VectorLorentz<T>::VectorLorentz(std::vector<T> _values, int _storageType)
:
v1(_values[0]),
v2(_values[1]),
v3(_values[2]),
storageType(_storageType)
{ }

template <typename T>
VectorLorentz<T>::VectorLorentz(const VectorLorentz<T> & source)
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
T VectorLorentz<T>::operator() (unsigned int index) const throw (MathException)
{
  switch (index)
    {
      case 0: return v1;
      case 1: return v2;
      case 2: return v3;
      default: throw MathException("index>2","VectorLorentz<T>::operator()");
    }
}


template <typename T>
T VectorLorentz<T>::operator[] (unsigned int index) const throw (MathException)
{
  switch (storageType)
    {
      case 0: // cartesian coordinates storage and output
      switch (index)
        {
          case 0: return v1; //!> x
          case 1: return v2; //!> y
          case 2: return v3;              //!> z
          default: throw MathException("index>2","VectorLorentz<T>::operator []");
        }

      case 1: // spherical coordinates storage and cartesian output
      switch (index)
        {
          case 0: return v3*std::sin(v2)*std::cos(v1); //!> x
          case 1: return v3*std::sin(v2)*std::sin(v1); //!> y
          case 2: return v3*std::cos(v2);              //!> z
          default: throw MathException("index>2","VectorLorentz<T>::operator []");
        }

      case 2: // cylindrical coordinates storage and cartesian output
      switch (index)
        {
          case 0: return v2*std::cos(v1); //!> x
          case 1: return v2*std::sin(v1); //!> y
          case 2: return v3;              //!> z
          default: throw MathException("index>2","VectorLorentz<T>::operator []");
        }

      default: throw MathException("Internal error","VectorLorentz<T>::operator[]");

    }
}


template <typename T>
T   VectorLorentz<T>::x()  const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v1; //!> x
      case 1: return v3*std::sin(v2)*std::cos(v1); //!> x
      case 2: return v2*std::cos(v1); //!> x
      default: throw MathException("Internal error","VectorLorentz<T>::x()");
    }
}

template <typename T>
T   VectorLorentz<T>::y()  const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v2; //!> y
      case 1: return v3*std::sin(v2)*std::sin(v1); //!> x
      case 2: return v2*std::sin(v1); //!> y
      default: throw MathException("Internal error","VectorLorentz<T>::y()");
    }
}


template <typename T>
T   VectorLorentz<T>::z()  const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v3;              //!> z
      case 1: return v3*std::cos(v2); //!> x
      case 2: return v3;              //!> z
      default: throw MathException("Internal error","VectorLorentz<T>::z()");
    }
}

template <typename T>
void VectorLorentz<T>::setXYZ(const T * values) throw (MathException)
{
  v1 = values[0];
  v2 = values[1];
  v3 = values[2];
  storageType = 0;
}

template <typename T>
void VectorLorentz<T>::setXYZ(const std::vector<T> & values) throw (MathException)
{
  v1 = values.x();
  v2 = values.y();
  v3 = values.z();
  storageType = 0;
}

template <typename T>
void VectorLorentz<T>::setXYZ(const T & x, const T & y, const T & z) throw (MathException)
{
  v1 = x;
  v2 = y;
  v3 = z;
  storageType = 0;
}

template <typename T>
void VectorLorentz<T>::setFromXYZ(const T & x, const T & y, const T & z) throw (MathException)
{
  switch (storageType)
    {
      default: throw MathException("Internal error","VectorLorentz<T>::setFromXYZ()");
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
void VectorLorentz<T>::setPhiThetaR(const T & phi, const T & theta, const T & r) throw (MathException)
{
  v1 = phi;
  v2 = theta;
  v3 = r;
  storageType = 1;
}


template <typename T>
void VectorLorentz<T>::setPhiRhoZ(const T & phi, const T & rho, const T & z) throw (MathException)
{
  v1 = phi;
  v2 = rho;
  v3 = z;
  storageType = 2;
}

template <typename T>
T  VectorLorentz<T>::phi() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return std::atan2(v2,v1);
      case 1: return v1;
      case 2: return v1;
      default: throw MathException("Internal error","VectorLorentz<T>::phi()");
    }
}

template <typename T>
T  VectorLorentz<T>::cosPhi() const  throw (MathException)
{
  switch (storageType)
    {
      case 0: return v1/std::sqrt(v1*v1+v2*v2);
      case 1: return std::cos(v1);
      case 2: return std::cos(v1);
      default: throw MathException("Internal error","VectorLorentz<T>::cosPhi()");
    }
}

template <typename T>
T  VectorLorentz<T>::sinPhi() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v2/std::sqrt(v1*v1+v2*v2);
      case 1: return std::sin(v1);
      case 2: return std::sin(v1);
      default: throw MathException("Internal error","VectorLorentz<T>::sinPhi()");
    }
}


template <typename T>
T  VectorLorentz<T>::theta() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return std::atan2(std::sqrt(v1*v1+v2*v2),v3);
      case 1: return v2;
      case 2: return std::atan2(v2,v3);
      default: throw MathException("Internal error","VectorLorentz<T>::theta()");
    }
}

template <typename T>
T  VectorLorentz<T>::cosTheta() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v3/std::sqrt(v1*v1+v2*v2+v3*v3);
      case 1: return std::cos(v2);
      case 2: return v3/std::sqrt(v2*v2+v3*v3);
      default: throw MathException("Internal error","VectorLorentz<T>::cosTheta()");
    }
}

template <typename T>
T  VectorLorentz<T>::sinTheta() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v3/std::sqrt(v1*v1+v2*v2+v3*v3);
      case 1: return std::cos(v2);
      case 2: return v3/std::sqrt(v2*v2+v3*v3);
      default: throw MathException("Internal error","VectorLorentz<T>::sinTheta()");
    }
}

template <typename T>
T  VectorLorentz<T>::modulus() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return std::sqrt(v1*v1+v2*v2+v3*v3);
      case 1: return v3;
      case 2: return std::sqrt(v2*v2+v3*v3);
      default: throw MathException("Internal error","VectorLorentz<T>::modulus()");
    }
}

template <typename T>
T  VectorLorentz<T>::modulusSquare() const throw (MathException)
{
  switch (storageType)
    {
      case 0: return v1*v1+v2*v2+v3*v3;
      case 1: return v3*v3;
      case 2: return v2*v2+v3*v3;
      default: throw MathException("Internal error","VectorLorentz<T>::modulusSquare()");
    }
}

template <typename T>
T  VectorLorentz<T>::perp() const  throw (MathException)
{
  switch (storageType)
    {
      case 0: return std::sqrt(v1*v1+v2*v2);
      case 1: return v3*std::sin(v2);
      case 2: return v2;
      default: throw MathException("Internal error","VectorLorentz<T>::perp()");
    }
}

template <typename T>
T  VectorLorentz<T>::perpSquare() const   throw (MathException)
{
  switch (storageType)
    {
      default:
      throw MathException("Internal error","VectorLorentz<T>::perpSquare()");
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
VectorLorentz<T> VectorLorentz<T>::operator- () const throw (MathException)
{
  VectorLorentz v(0,0,0,storageType);
  switch (storageType)
    {
      default: throw MathException("Internal error","VectorLorentz<T>::operator- ()");

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
VectorLorentz<T> & VectorLorentz<T>::operator *= (const T & scaleFactor)
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
VectorLorentz<T>& VectorLorentz<T>::normalize() throw (MathException)
{
  switch (storageType)
    {
      case 0:
      {
      T r = std::sqrt(v1*v1+v2*v2+v3*v3);
      if (r==0) throw MathException("Cannot produced unit vector from null vector","VectorLorentz<T>::unit()");
      v1 /= r;
      v2 /= r;
      v3 /= r;
      break;
      }
      case 1:
      if (v3==0) throw MathException("Cannot produced unit vector from null vector","VectorLorentz<T>::unit()");
      v3 = 1;
      break;

      case 2:
      {
      T r = std::sqrt(v2*v2+v3*v3);
      if (r==0) throw MathException("Cannot produced unit vector from null vector","VectorLorentz<T>::unit()");
      v2 /= r;
      v3 /= r;
      break;
      }
      default:
      throw MathException("Internal error","VectorLorentz<T>::normalize()");
    }
  return *this;
}

//! unit VectorLorentz parallel to this vecotr.
template <typename T>
VectorLorentz<T> VectorLorentz<T>::unit() const throw (MathException)
{
  switch (storageType)
    {
      default:
      throw MathException("Internal error","VectorLorentz<T>::unit()");

      case 0:
      {
      T r = std::sqrt(v1*v1+v2*v2+v3*v3);
      if (r==0) throw MathException("Cannot produced unit vector from null vector","VectorLorentz<T>::unit()");
      VectorLorentz<T> vxyz(v1/r,v2/r,v3/r,storageType);
      return vxyz;
      break;
      }

      case 1:
      {
      if (v3==0) throw MathException("Cannot produced unit vector from null vector","VectorLorentz<T>::unit()");
      VectorLorentz<T> vr(v1,v2,1.0,storageType);
      return vr;
      break;
      }

      case 2:
      {
      T r = std::sqrt(v2*v2+v3*v3);
      if (r==0) throw MathException("Cannot produced unit vector from null vector","VectorLorentz<T>::unit()");
      VectorLorentz<T> vrho(v1,v2/r,v3/r,storageType);
      return vrho;
      }
    }
}



//! Active rotation of this VectorLorentz by the given angle relative to the x-axis
template <typename T>
void VectorLorentz<T>::rotateX(const T & angle)
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

//! Active rotation of this VectorLorentz by the given angle relative to the y-axis
template <typename T>
void VectorLorentz<T>::rotateY(const T & angle)
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

//! Active rotation of this VectorLorentz by the given angle relative to the z-axis
template <typename T>
void VectorLorentz<T>::rotateZ(const T & angle)
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
VectorLorentz<T> VectorLorentz<T>::projectOntoXY() const
{
  return VectorLorentz<T>(x(), y(), 0, 0);
}

template <typename T>
VectorLorentz<T> VectorLorentz<T>::projectOntoYZ() const
{
  return VectorLorentz<T>(0, y(), z(), 0);
}


template <typename T>
VectorLorentz<T> VectorLorentz<T>::projectOntoXZ() const
{
  return VectorLorentz<T>(x(), 0, z(), 0);
}


template <typename T>
void VectorLorentz<T>::print() const  throw (MathException)
{
  switch (storageType)
    {
      case 0:
      std::cout << "(x:" << v1 << ", y:" << v2 << ", z:" << v3 << ") " << std::endl; break;
      case 1:
      std::cout << "(phi:" << v1 << ", theta:" << v2 << ", r:" << v3 << ") " << std::endl; break;
      case 2:
      std::cout << "(phi:" << v1 << ", rho:" << v2 << ", z:" << v3 << ") " << std::endl; break;
      default: throw MathException("Internal error","VectorLorentz<T>::print()");
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const CAP::Math::VectorLorentz<T>& v)
{
  out << "(" << v.x()  << "," << v.y() << "," << v.z() << ")" ;
  return out;
}



typedef  VectorLorentz<float>  Vector3F;
typedef  VectorLorentz<double> Vector3D;
typedef  VectorLorentz<long double> Vector3LD;

} // namespace Math
} // namespace CAP

#endif
