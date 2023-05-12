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

#ifndef _TH2_SOUND_VELOCITY_H_
  #define _TH2_SOUND_VELOCITY_H_

#include "VectorField.hpp"
using CAP::VectorField;

class SoundVelocity : public VectorField
{
  public:
    SoundVelocity();
    virtual ~SoundVelocity() {}

    int    getCsType() const;
    double getCs() const;
    double getCs(double aX, double aY, double aZ) const;
    double getCs2() const;
    double getCs2(double aX, double aY, double aZ) const;
    void   setCs(double aCs);
    void   setCs(VectorField* aCs);

protected:
  ClassDef(SoundVelocity,0)
};

#endif

/*! @file SoundVelocity.h
 * @brief Definition of SoundVelocity class. Gives the local sound velocity @f$ c_s @f$.
 */
/*! @class SoundVelocity
 * @brief Class gives the local sound velocity @f$ c_s @f$.
 *
 * If setCs() is called with a VectorField object it will return sound velocity at @f$ c_s(x,y,z) @f$ 
 * in other case it will return a constant value of sound velocity.
 *
 * @fn SoundVelocity::SoundVelocity()
 * @brief Default constructor.
 *
 * @fn SoundVelocity::~SoundVelocity()
 * @brief Destructor.
 *
 * @fn int SoundVelocity::getCsType() const
 * @brief get the type of sound velocity stored.
 * @retval 0 constant, @f$ c_s = const @f$
 * @retval 1 variable, @f$ c_s = c_s(x,y,z) @f$
 * @retval -1 unknown, @f$ c_s = 0 @f$
 *
 * @fn double SoundVelocity::getCs() const
 * @brief Returns constant sound velocity value
 *
 * @fn double SoundVelocity::getCs(double aX, double aY, double aZ) const
 * @brief Returns sound velocity as a function of <b>x</b>,<b>y</b> and <b>z</b>
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn double SoundVelocity::getCs2() const
 * @brief Returns constant sound velocity squared value
 *
 * @fn double SoundVelocity::getCs2(double aX, double aY, double aZ) const
 * @brief Returns sound velocity squared as a function of <b>x</b>,<b>y</b> and <b>z</b>
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 * 
 * @fn void SoundVelocity::setCs(double aCs)
 * @brief sets the sound velocity type to constant.
 * @param [in] aCs constant sound velocity value
 *
 * @fn void SoundVelocity::setCs(VectorField* aCs)
 * @brief sets the sound velocity type to variable.
 * @param [in] aCs pointer to VectorField object with the sound velocity data
 */
