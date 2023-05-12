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

#include "Viscosity.hpp"

ClassImp(Viscosity);

Viscosity::Viscosity()
:
VectorField()
{  }

int Viscosity::getViscosityType() const
{
  return type;
}

double Viscosity::getBulkTau() const
{
  return constValue;
}

double Viscosity::getBulkPi(double aX, double aY, double aZ) const
{
  return interpolate(aX,aY,aZ);
}

double Viscosity::getShearPPdotPi(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz) const
{
  return (
      aPe*aPe * shear[i00].interpolate(aX,aY,aZ)
    + aPx*aPx * shear[iXX].interpolate(aX,aY,aZ)
    + aPy*aPy * shear[iYY].interpolate(aX,aY,aZ)
    + aPz*aPz * shear[iZZ].interpolate(aX,aY,aZ)
    + 2.0 * ( 
        aPx*aPy * shear[iXY].interpolate(aX,aY,aZ)
      + aPx*aPz * shear[iXZ].interpolate(aX,aY,aZ)
      + aPy*aPz * shear[iYZ].interpolate(aX,aY,aZ)
    ) - 2.0 * aPe * (
        aPx * shear[i0X].interpolate(aX,aY,aZ)
      + aPy * shear[i0Y].interpolate(aX,aY,aZ)
      + aPz * shear[i0Z].interpolate(aX,aY,aZ)
    )
  );
}
   
void Viscosity::setBulkTau(double value)
{
  type = 0 | 0x01;
  constValue = value;
}

void Viscosity::setBulkPi(VectorField & value)
{
  type = type | 0x01; // this makes no sense
  constValue = 1.0E-100;
  setValue(value);
}

void Viscosity::setShearPi(eViscPi aIndex, VectorField & aShear)
{
  type = type | 0x02;
  shear[aIndex] = aShear;
}
