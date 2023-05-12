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

#include "Temperature.hpp"
ClassImp(Temperature);

Temperature::Temperature()
:
VectorField()
{  }

double Temperature::getTemperature() const
{
  return constValue;
}

double Temperature::getTemperature(double aX, double aY, double aZ) const
{
  return interpolate(aX, aY, aZ);
}

int Temperature::getTemperatureType() const
{
  return type;
}

void Temperature::setTemperature(double aTemp)
{
  type = 0;
  constValue = aTemp;
}

void Temperature::setTemperature(const VectorField* field)
{
  type = 1;
  setValue(*field);
}


void Temperature::setTemperature(const VectorField & field)
{
  type = 1;
  setValue(field);
}
