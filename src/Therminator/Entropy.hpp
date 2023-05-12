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

#ifndef _TH2_ENTROPY_H_
  #define _TH2_ENTROPY_H_

#include "VectorField.hpp"
using CAP::VectorField;

class Entropy  : public VectorField
{
  public:
    Entropy();
  virtual ~Entropy() {}

    int    getEntropyType() const;
    double getEntropy() const;
    double getEntropy(double aX, double aY, double aZ) const;
    void   setEntropy(double aEntropy);
    void   setEntropy(VectorField* aEntropy);

protected:

  ClassDef(Entropy,0)
};

#endif

/*! @file Entropy.h
 * @brief Definition of Entropy class. Gives the local entropy density @f$ s @f$.
 */
/*! @class Entropy
 * @brief Class gives the local entropy density @f$ s @f$.
 *
 * If setEntropy() is called with a VectorField object it will return entropy density at @f$ s(x,y,z) @f$ 
 * in other case it will return a constant value of entropy density.
 *
 * @fn Entropy::Entropy()
 * @brief Default constructor.
 *
 * @fn Entropy::~Entropy()
 * @brief Destructor.
 *
 * @fn int Entropy::getEntropyType() const
 * @brief get the type of entropy density stored.
 * @retval 0 constant, @f$ s = const @f$
 * @retval 1 variable, @f$ s = s(x,y,z) @f$
 * @retval -1 unknown, @f$ s = 0 @f$
 *
 * @fn double Entropy::getEntropy() const
 * @brief Returns constant entropy density value
 *
 * @fn double Entropy::getEntropy(double aX, double aY, double aZ) const
 * @brief Returns entropy density as a function of <b>x</b>,<b>y</b> and <b>z</b>
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn void Entropy::setEntropy(double aEntropy)
 * @brief sets the entropy density type to constant.
 * @param [in] aEntropy constant entropy density value
 *
 * @fn void Entropy::setEntropy(VectorField* aEntropy)
 * @brief sets the entropy density type to variable.
 * @param [in] aEntropy pointer to VectorField object with the entropy density data
 */
