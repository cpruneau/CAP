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

#ifndef _TH2_ENERGY_H_
  #define _TH2_ENERGY_H_

#include "VectorField.hpp"
using CAP::VectorField;

class Energy : public VectorField
{
  public:
    Energy();
    virtual ~Energy() {}

    int    getEnergyType() const;
    double getEnergy() const;
    double getEnergy(double aX, double aY, double aZ) const;
    void   setEnergy(double aEnergy);
    void   setEnergy(VectorField* aEnergy);

protected:

  ClassDef(Energy,0)

};

#endif

/*! @file Energy.h
 * @brief Definition of Energy class. Gives the local energy density @f$ \varepsilon @f$.
 */
/*! @class Energy
 * @brief Class gives the local energy density @f$ \varepsilon @f$.
 *
 * If setEnergy() is called with a VectorField object it will return energy density at @f$ \varepsilon(x,y,z) @f$ 
 * in other case it will return a constant value of energy density.
 *
 * @fn Energy::Energy()
 * @brief Default constructor.
 *
 * @fn Energy::~Energy()
 * @brief Destructor.
 *
 * @fn int Energy::getEnergyType() const
 * @brief get the type of energy density stored.
 * @retval 0 constant, @f$ \varepsilon = const @f$
 * @retval 1 variable, @f$ \varepsilon = \varepsilon(x,y,z) @f$
 * @retval -1 unknown, @f$ \varepsilon = 0 @f$
 *
 * @fn double Energy::getEnergy() const
 * @brief Returns constant energy density value
 *
 * @fn double Energy::getEnergy(double aX, double aY, double aZ) const
 * @brief Returns energy density as a function of <b>x</b>,<b>y</b> and <b>z</b>
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn void Energy::setEnergy(double aEnergy)
 * @brief sets the energy density type to constant.
 * @param [in] aEnergy constant energy density value
 *
 * @fn void Energy::setEnergy(VectorField* aEnergy)
 * @brief sets the energy density type to variable.
 * @param [in] aEnergy pointer to VectorField object with the energy density data
 */
