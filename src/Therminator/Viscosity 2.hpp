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

#ifndef _TH2_VISCOSITY_H_
#define _TH2_VISCOSITY_H_
#include "VectorField.hpp"
using CAP::VectorField;


enum eViscPi {i00, i0X, i0Y, i0Z,
	           iXX, iXY, iXZ,
	                iYY, iYZ,
	                     iZZ};

class Viscosity : public VectorField
{
  public:
    Viscosity();
  virtual ~Viscosity() {}

    int    getViscosityType() const;
    double getBulkTau() const;
    double getBulkPi(double aX, double aY, double aZ) const;
    double getShearPPdotPi(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz) const;
    
    void   setBulkTau(double aTau);
    void   setBulkPi(VectorField& aBulk);
    void   setShearPi(eViscPi aIndex, VectorField & aShear);
    
  private:
    VectorField shear[10];

protected:

  ClassDef(Viscosity,0)
};

#endif

/*! @file Viscosity.h
 * @brief Definition of Viscosity class. Contains the bulk @f$ \Pi @f$ and shear viscosity tensor @f$ \pi^{/mu/nu} @f$.
 * @warning Not finished.
 */
/*! @enum eViscPi
 * @brief Shear viscosity tensor @f$ \pi^{\mu\nu} @f$ indexes.
 */
/*! @class Viscosity
 * @brief Contains the bulk @f$ \Pi @f$ and shear viscosity tensor @f$ \pi^{/mu/nu} @f$.
 *
 * @fn Viscosity::Viscosity()
 * @brief Default constructor.
 *
 * @fn Viscosity::~Viscosity()
 * @brief Destructor
 * 
 * @fn int Viscosity::getViscosityType() const
 * @brief get the type of viscosity stored.
 * @retval 1 bulk
 * @retval 2 shear
 * @retval 3 bulk and shear
 * @retval 0 unknown
 * 
 * @fn double Viscosity::getBulkTau() const
 * @brief Returns the bulk relaxation time @f$ \tau_{bulk} @f$.
 * 
 * @fn double Viscosity::getBulkPi(double aX, double aY, double aZ) const
 * @brief Returns the bulk viscosity ... @f$ \frac{\Pi}{\zeta} @f$ at the hypersurface.
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 * 
 * @fn double Viscosity::getShearPPdotPi(double aX, double aY, double aZ, double aPe, double aPx, double aPy, double aPz) const
 * @brief Returns the tensor product @f$ p_\mu p_\nu \frac{\pi^{\mu\nu}}{s T} @f$ at the hypersurface.
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 * @param [in] aPe particle energy
 * @param [in] aPx particle momentum, x-coordinate
 * @param [in] aPy particle momentum, y-coordinate
 * @param [in] aPz particle momentum, z-coordinate
 * 
 * @fn void Viscosity::setBulkTau(double aTau)
 * @brief sets the bulk relaxation time @f$ \tau_{bulk} @f$.
 * @param [in] aTau bulk relaxation time
 * 
 * @fn void Viscosity::setBulkPi(VectorField* aBulk)
 * @brief sets the bulk viscosity ... @f$ \frac{\Pi}{\zeta} @f$.
 * @param [in] aBulk 
 * 
 * @fn void Viscosity::setShearPi(eViscPi aIndex, VectorField* aShear)
 * @brief sets shear viscosity tensor @f$ \frac{\pi^{\mu\nu}}{s T} @f$ element given by the index.
 * @param [in] aIndex shear tensor index
 * @param [in] aShear shear tensor element
 * 
 */
