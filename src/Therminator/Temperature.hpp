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

#ifndef _TH2_TEMPERATURE_H_
  #define _TH2_TEMPERATURE_H_

#include "VectorField.hpp"
using CAP::VectorField;

class Temperature : public VectorField
{
public:
  Temperature();
  virtual ~Temperature() {}

  int    getTemperatureType() const;
  double getTemperature() const;
  double getTemperature(double aX, double aY, double aZ) const;
  void   setTemperature(double aTemp);
  void   setTemperature(const VectorField* aTemp);
  void   setTemperature(const VectorField & field);

protected:

  ClassDef(Temperature,0)
};

#endif

/*! @file Temperature.h
 * @brief Definition of Temperature class. Gives the local temperature.
 */
/*! @class Temperature
 * @brief Class gives the local temperature.
 *
 * If setTemperature() is called with a VectorField object it will return the temperature at @f$ T(x,y,z) @f$ 
 * in other case it will return a constant value of temperature.
 *
 * @fn Temperature::Temperature()
 * @brief Default constructor.
 *
 * @fn Temperature::~Temperature()
 * @brief Destructor.
 *
 * @fn int Temperature::getTemperatureType() const
 * @brief get the type of temperature stored.
 * @retval 0 constant, @f$ T = const @f$
 * @retval 1 variable, @f$ T = T(x,y,z) @f$
 * @retval -1 unknown, @f$ T = 0 @f$
 *
 * @fn double Temperature::getTemperature() const
 * @brief Returns constant temperature value
 *
 * @fn double Temperature::getTemperature(double aX, double aY, double aZ) const
 * @brief Returns temperature as a function of <b>x</b>,<b>y</b> and <b>z</b>
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn void Temperature::setTemperature(double aTemp)
 * @brief sets the temperature type to constant type.
 * @param [in] aTemp constant temperature value
 *
 * @fn void Temperature::setTemperature(VectorField* aTemp)
 * @brief sets the temperature type to variable.
 * @param [in] aTemp pointer to VectorField object with the temperature data
 */
