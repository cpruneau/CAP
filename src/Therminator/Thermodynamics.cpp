/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

#include <TMath.h>
#include "Thermodynamics.hpp"
using namespace TMath;

ClassImp(Temperature);

Thermodynamics::Thermodynamics()
:
Chemistry(),
Energy(),
Entropy(),
Pressure(),
SoundVelocity(),
Temperature(),
Viscosity()
{  }


double Thermodynamics::getInvFugacityFactor(ParticleType & aPartType) const
{
  switch (getChemistryType())
    {
    case 0:  return Exp( - getChemicalPotential(aPartType) / getTemperature() );
    case 2:  return 1.0 / getFugacity(aPartType);
    default: return 1.0;
  }
}

double Thermodynamics::getInvFugacityFactor(ParticleType & aPartType, double aX, double aY, double aZ) const
{
  switch (getChemistryType()) {
    case 1:  return Exp( - getChemicalPotential(aPartType, aX, aY, aZ) / getTemperature(aX, aY, aZ) );
    case 3:  return 1.0 / getFugacity(aPartType, aX, aY, aZ);
    default: return 1.0;
  }
}

double Thermodynamics::getViscosityCorrection(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz, double aPdotU) const
{
  double tampss2;
  double tTemp;
  double tCs2;
  double tBulkPart  = 0.0;
  double tShearPart = 0.0;
  
  tampss2 = aPe*aPe - aPx*aPx - aPy*aPy - aPz*aPz;
  
  switch (getTemperatureType()) {
    case 0:
      tTemp = getTemperature();
      break;
    case 1:
      tTemp = getTemperature(aX, aY, aZ);
      break;
  }
  switch (getCsType()) {
    case 0:
      tCs2 = getCs2();
      break;
    case 1:
      tCs2 = getCs2(aX, aY, aZ);
      break;
  }
    
  switch (getViscosityType()) {
    case 0:
      tShearPart  = 1.0 / (2.0*tTemp*tTemp) * getShearPPdotPi(aX,aY,aZ,aPe,aPx,aPy,aPz);
      break;
    case 1:
      tBulkPart = ( tCs2*aPdotU + (tampss2 - aPdotU*aPdotU) / (3.0*aPdotU) ) * getBulkTau() * getBulkPi(aX,aY,aZ) / tTemp;
      break;
    case 2:
      tShearPart  = 1.0 / (2.0*tTemp*tTemp) * getShearPPdotPi(aX,aY,aZ,aPe,aPx,aPy,aPz);
      tBulkPart = ( tCs2*aPdotU + (tampss2 - aPdotU*aPdotU) / (3.0*aPdotU) ) * getBulkTau() * getBulkPi(aX,aY,aZ) / tTemp;
      break;      
  }
  return (tBulkPart + tShearPart);
}
