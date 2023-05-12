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

#ifndef _TH2_PRESSURE_H_
  #define _TH2_PRESSURE_H_

#include "VectorField.hpp"
using CAP::VectorField;

class Pressure : public VectorField
{
  public:
    Pressure();
    virtual ~Pressure() {}

    int    getPressureType() const;
    double getPressure() const;
    double getPressure(double aX, double aY, double aZ) const;
    void   setPressure(double aPress);
    void   setPressure(VectorField* aPress);

protected:
  ClassDef(Pressure,0)
};

#endif

/*! @file Pressure.h
 * @brief Definition of Pressure class. Gives the local pressure @f$ P @f$.
 */
/*! @class Pressure
 * @brief Class gives the local pressure @f$ P @f$.
 *
 * If setPressure() is called with a VectorField object it will return pressure at @f$ P(x,y,z) @f$ 
 * in other case it will return a constant value of pressure.
 *
 * @fn Pressure::Pressure()
 * @brief Default constructor.
 *
 * @fn Pressure::~Pressure()
 * @brief Destructor.
 *
 * @fn int Pressure::getPressureType() const
 * @brief get the type of pressure stored.
 * @retval 0 constant, @f$ P = const @f$
 * @retval 1 variable, @f$ P = P(x,y,z) @f$
 * @retval -1 unknown, @f$ P = 0 @f$
 *
 * @fn double Pressure::getPressure() const
 * @brief Returns constant pressure value
 *
 * @fn double Pressure::getPressure(double aX, double aY, double aZ) const
 * @brief Returns pressure as a function of <b>x</b>,<b>y</b> and <b>z</b>
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn void Pressure::setPressure(double aPress)
 * @brief sets the pressure type to constant.
 * @param [in] aPress constant pressure value
 *
 * @fn void Pressure::setPressure(VectorField* aPress)
 * @brief sets the pressure type to variable.
 * @param [in] aPress pointer to VectorField object with the pressure data
 */
