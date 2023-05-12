#ifndef CAP_PhysicsConstants
#define CAP_PhysicsConstants

//!
//! Compendium of physics constants
//!  By convention, all function names  begin with a lower case letter.
//!   Instead of naming function after the letter commonly used in the literature e.g., c for the speed of light,  functions are named
//!   after what they represent to avoid ambiguities and naming crounding. For instance, instead of "c" for the speed of light, we use "lightVelocity"


namespace CAP
{
namespace Physics
{
//!
//! Velocity of light in \f$ m s^{-1} \f$
//!
constexpr double lightVelocity_SI()
{
  return 2.99792458e8;
}

//!
//! \f$ cm s^{-1} \f$
constexpr double lightVelocity_CGS()
{
  return 100.0 * lightVelocity_SI();
}

//!
//! Speed of light uncertainty.
constexpr double lightVelocityUncertainty()
{
  return 0.0;
}

//!
//! Gravitational constant in: \f$ m^{3} kg^{-1} s^{-2} \f$
constexpr double bigG_SI()
{
  //! use 2018 value from NIST  (https://physics.nist.gov/cgi-bin/cuu/Value?bg|search_for=G)
  return 6.67430e-11;
}

//!
//! \f$ cm^{3} g^{-1} s^{-2} \f$
constexpr double bigG_CGS()
{
  return bigG_SI() * 1000.0;
}

//!
//! Gravitational constant uncertainty.
constexpr double bigGUncertainty_SI()
{
  //! use 2018 value from NIST
  return 0.00015e-11;
}

//!
//! \f$ \frac{G}{\hbar C} \f$ in \f$ (GeV/c^{2})^{-2} \f$
constexpr double bigGHbarC_SI()
{
  //! use new value from NIST (2018)
  return 6.70883e-39;
}

//!
//! \f$ \frac{G}{\hbar C} \f$ uncertainty.
constexpr double bigGHbarCUncertainty_SI()
{
  //! use new value from NIST (2018)
  return 0.00015e-39;
}

//!
//! Standard acceleration of gravity in \f$ m s^{-2} \f$
constexpr double gEarth_SI()
{
  return 9.80665;
}

//!
//! Standard acceleration of gravity uncertainty.
constexpr double gEarthUncertainty()
{
  return 0.0;
}

//!
//! Planck's constant in \f$ J s \f$: \f$ h \f$
constexpr double planck_SI()
{
  return 6.62607015e-34;
}

//!
//! \f$ erg s \f$
constexpr double planck_CGS()
{
  return 1.0e7 * planck_SI();
}

//!
//! \f$ GeV s \f$
constexpr double planck()
{
  return 1.0E-9 * planck_SI() / 1.6021773e-19;
}


//!
//! Planck's constant uncertainty.
constexpr double planckUncertainty()
{
  //! Planck constant is exact according to 2019 redefinition
  //! (https://en.wikipedia.org/wiki/2019_redefinition_of_the_SI_base_units)
  return 0.0;
}

//!
//! \f$ \hbar \f$ in \f$ J s \f$: \f$ \hbar = \frac{h}{2\pi} \f$
constexpr double hBar_SI()
{
  return 1.054571817e-34;
}

//!
//! \f$ erg s \f$
constexpr double hBar_CGS()
{
  return 1.0e7 * hBar_SI();
}

//!
//! \f$ GeV s \f$
constexpr double hBar()
{
  return 1.0E-9 * hBar_SI()/ 1.6021773e-19;
}

//!
//! \f$ \hbar \f$ uncertainty.
constexpr double hBarUncertainty()
{
  //! hbar is an exact constant
  return 0.0;
}

//!
//! \f$ hc \f$ in \f$ J m \f$
constexpr double hC_SI()
{
  return planck_SI() * lightVelocity_SI();
}

//!
constexpr double hBarC_SI()
{
  return hBar_SI() * lightVelocity_SI();
}

//!
//! \f$ hc \f$ in \f$ GeV fm \f$
constexpr double hBarC()
{
  return 1.0E15 * hBar() * lightVelocity_SI();
}


//! hBarC Square
constexpr double hBarC2()
{
  return hBarC() * hBarC();
}

//! hBarC Cube
constexpr double hBarC3()
{
  return hBarC2() * hBarC();
}

//! hBarC 4th power
constexpr double hBarC4()
{
  return hBarC2() * hBarC2();
}

//!
//! \f$ erg cm \f$
constexpr double hC_CGS()
{
  return planck_CGS() * lightVelocity_CGS();
}

//!
//! boltzmannConstant's constant in \f$ J K^{-1} \f$: \f$ k \f$
constexpr double boltzmannConstant_SI()
{
  return 1.380649e-23;
}

//!
//! \f$ erg K^{-1} \f$
constexpr double boltzmannConstant_CGS()
{
  return 1.0e7 * boltzmannConstant_SI();
}

//!
//! boltzmannConstant's constant uncertainty.
constexpr double boltzmannConstantUncertainty()
{
  //! constant is exact according to 2019 redefinition
  //! (https://en.wikipedia.org/wiki/2019_redefinition_of_the_SI_base_units)
  return 0.0;
}

//!
//! Stefan-boltzmannConstant constant in \f$ W m^{-2} K^{-4}\f$: \f$ \sigma \f$
constexpr double stefanBoltzmannConstant_SI()
{
  return 5.670373e-8;
}

//!
//! Stefan-boltzmannConstant constant uncertainty.
constexpr double stefanBoltzmannConstantUncertainty()
{
  return 0.0;
}

//!
//! avogadroNumber constant (avogadroNumber's Number) in \f$ mol^{-1} \f$
constexpr double avogadroNumber()
{
  return 6.02214076e+23;
}

//!
//! avogadroNumber constant (avogadroNumber's Number) uncertainty.
constexpr double avogadroNumberUncertainty()
{
  //! constant is exact according to 2019 redefinition
  //! (https://en.wikipedia.org/wiki/2019_redefinition_of_the_SI_base_units)
  return 0.0;
}

//!
//! [Universal gas constant](http://scienceworld.wolfram.com/physics/UniversalGasConstant.html)
//! (\f$ avogadroNumber K \f$) in \f$ J K^{-1} mol^{-1} \f$
//
constexpr double gasConstant_SI()
{
  return boltzmannConstant_SI() * avogadroNumber();
}

//!
//! Universal gas constant uncertainty.
constexpr double gasConstantUncertainty_SI()
{
  return gasConstant_SI() * ((boltzmannConstantUncertainty() / boltzmannConstant_SI()) + (avogadroNumberUncertainty() / avogadroNumber()));
}

//!
//! [Molecular weight of dry air 1976 US Standard Atmosphere](http://atmos.nmsu.edu/jsdap/encyclopediawork.html)
//! in \f$ kg kmol^{-1} \f$ or \f$ gm mol^{-1} \f$
constexpr double molecularWeightAir_SI()
{
  return 28.9644;
}

//!
//! [Dry Air Gas Constant (R / molecularWeightAir)](http://atmos.nmsu.edu/education_and_outreach/encyclopedia/gas_constant.htm)
//! in \f$ J kg^{-1} K^{-1} \f$
constexpr double gasConstantAir()
{
  return (1000.0 * gasConstant_SI()) / molecularWeightAir_SI();
}

//!
//! Euler-Mascheroni Constant.
constexpr double eulerGamma()
{
  return 0.577215664901532860606512090082402431042;
}

//!
//! Elementary charge in \f$ C \f$ .
constexpr double qElectron_SI()
{
  return 1.602176634e-19;
}

//!
//! Elementary charge uncertainty.
constexpr double qElectronUncertainty()
{
  //! constant is exact according to 2019 redefinition
  //! (https://en.wikipedia.org/wiki/2019_redefinition_of_the_SI_base_units)
  return 0.0;
}


//!
//! vacuum electric permittivity in F . m
constexpr double electricPermittivity_SI()
{
  return 8.854187812813E-12;
}

//!
//! vacuum electric permittivity in F . m
constexpr double electricPermittivityUncertainty_SI()
{
  return 1.5E-10;
}

//!
//! vacuum magnetic permeability
constexpr double magneticPermeability_SI()
{
  return 1.2566370621219E-6;
}

//!
//! vacuum electric permittivity in F . m
constexpr double magneticPermeabilityUncertainty_SI()
{
  return 1.5E-10;
}

//!
//! fine structure constant
constexpr double fineStructureConstant()
{
  return 7.297352569311E-3;
}

//!
//! fine structure constant
constexpr double fineStructureConstantUncertainty()
{
  return 1.5E-10;
}



// ========================================================================
// conversions
// ========================================================================



constexpr double inchToCentimeter()
{
  return 2.54;
}

constexpr double inchToMeter()
{
  return 0.0254;
}

constexpr double feetToMeter()
{
  return 0.3048;
}

constexpr double mileToYard()
{
  return 1760;
}


constexpr double mileToMeter()
{
  return 1609.344;
}

constexpr double mileToKilometer()
{
  return 1.609344;
}

constexpr double nauticalMileToKilometer()
{
  return 1.852;
}

constexpr double astronomicalUnitToKilometer()
{
  return 149597870.691;
}

constexpr double parsecToKilometer()
{
  return 30856776000000;
}

constexpr double lightYearToKilometer()
{
  return 9460730472580.8;
}

constexpr double lightMinuteToKilometer()
{
  return 17987547.48;
}


constexpr double lightSecondToKilometer()
{
  return 299792.458;
}

constexpr double parsecToLightYear()
{
  return 3.2615637967311;
}

constexpr double minuteToSecond()
{
  return 60;
}

constexpr double hourToSecond()
{
  return 3600;
}

constexpr double dayToSecond()
{
  return 86400;
}

constexpr double weekToSecond()
{
  return 604800;
}

constexpr double yearToSecond()
{
  return 31556952;
}

constexpr double poundToKilogram()
{
  return 0.45359237;
}


constexpr double angstromToMeter()
{
  return 0.45359237;
}

constexpr double electronChargeToCoulomb()
{
  return 1.6022e-19;
}

constexpr double electronVoltToJoule()
{
  return 1.6021773e-19;
}

}

}
#endif

