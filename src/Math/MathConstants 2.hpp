#ifndef CAP_MathConstants
#define CAP_MathConstants
#include <cmath>

//! 
//! Compendium of basic mathematical constants
//!
//!
namespace CAP
{

namespace Math
{

//!
//! Basic  constants
//!
//! \f$ \pi\f$
constexpr double pi()
{
  return 3.14159265358979323846;
}

//!
//! \f$ 2\pi\f$
constexpr double twoPi()
{
  return 2.0 * pi();
}

//!
//! \f$ 4\pi\f$
constexpr double fourPi()
{
  return 4.0 * pi();
}


//!
//! \f$ \frac{\pi}{2} \f$
constexpr double piOver2()
{
  return pi() / 2.0;
}

//!
//! \f$ \frac{\pi}{4} \f$
constexpr double piOver4()
{
  return pi() / 4.0;
}

//!
//! \f$ \frac{1.}{\pi}\f$
constexpr double invPi()
{
  return 1.0 / pi();
}

//!
//! \f$ \pi^2\f$
constexpr double piSquare()
{
  return pi() * pi();
}

//!
//! \f$ \pi^3\f$
constexpr double piCube()
{
  return piSquare() * pi();
}

//!
//! \f$ (2\pi)^2\f$
constexpr double twoPiSquare()
{
  return twoPi()*twoPi();
}

//!
//! \f$ (2\pi)^3\f$
constexpr double twoPiCube()
{
  return twoPiSquare()*twoPi();
}


//!
//! \f$ \sqrt{2\pi}\f$
constexpr double sqrtTwoPi()
{
  return sqrt(twoPi());
}

//!
//! \f$1/ \sqrt{2\pi}\f$
constexpr double oneOverSqrtTwoPi()
{
  return 1.0/sqrt(twoPi());
}


//!
//! Conversion from radian to degree: \f$ \frac{180}{\pi} \f$
constexpr double radToDeg()
{
  return 180.0 / pi();
}

//!
//! Conversion from degree to radian: \f$ \frac{\pi}{180} \f$
constexpr double degToRad()
{
  return pi() / 180.0;
}

//!
//! \f$ \sqrt{2} \f$
constexpr double sqrt2()
{
  return 1.4142135623730950488016887242097;
}

//!
//! Base of natural log: \f$ e \f$
constexpr double e()
{
  return 2.71828182845904523536;
}


//!
//! Natural log of 10 (to convert log to ln)
constexpr double ln2()
{
  return 0.69314718055994528623;
}

//!
//! Natural log of 10 (to convert log to ln)
constexpr double ln10()
{
  return 2.30258509299404568402;
}

//!
//! Base-10 log of e  (to convert ln to log)
constexpr double logE()
{
  return 0.43429448190325182765;
}



} // Math namespace
} // CAP namespace

#endif
