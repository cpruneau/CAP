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

#ifndef _TH2_CHEMISTRY_H_
  #define _TH2_CHEMISTRY_H_

#include "VectorField.hpp"
#include "ParticleType.hpp"

using CAP::ParticleType;
using CAP::VectorField;

class Chemistry
{
public:
  Chemistry();
  virtual ~Chemistry();

  int    getChemistryType() const;
  double getChemicalPotential(ParticleType & aPartType) const;
  double getChemicalPotential(ParticleType & aPartType, double aX, double aY, double aZ) const;
  double getFugacity(ParticleType & aPartType) const;
  double getFugacity(ParticleType & aPartType, double aX, double aY, double aZ) const;
  double getMuB() const;
  double getMuI() const;
  double getMuS() const;
  double getMuC() const;
  double getLambdaQ() const;
  double getLambdaI() const;
  double getLambdaS() const;
  double getLambdaC() const;
  double getGammaQ() const;
  double getGammaS() const;
  double getGammaC() const;
  void   setChemistry(double    aMuB,     double    aMuI,     double    aMuS,     double    aMuC);
  void   setChemistry(VectorField* aMuB,     VectorField* aMuI,     VectorField* aMuS,     VectorField* aMuC);
  void   setChemistry(double    aLambdaQ,
                      double    aLambdaI,
                      double    aLambdaS,
                      double    aLambdaC,
                      double    aGamampQ,
                      double    aGamampS,
                      double    aGamampC);
  void   setChemistry(VectorField* aLambdaQ,
                      VectorField* aLambdaI,
                      VectorField* aLambdaS,
                      VectorField* aLambdaC,
                      VectorField* aGamampQ,
                      VectorField* aGamampS,
                      VectorField* aGamampC);

private:
  int		chemistryType;

  double	muBConst;
  double	muIConst;
  double	muSConst;
  double	muCConst;

  double	gammaQConst;
  double	gammaSConst;
  double	gammaCConst;
  double	lambdaIConst;
  double	lambdaQConst;
  double	lambdaSConst;
  double	lambdaCConst;

  VectorField*  muBVar;
  VectorField*  muIVar;
  VectorField*  muSVar;
  VectorField*  muCVar;

  VectorField*	gammaQVar;
  VectorField*	gammaSVar;
  VectorField*	gammaCVar;
  VectorField*	lambdaIVar;
  VectorField*	lambdaQVar;
  VectorField*	lambdaSVar;
  VectorField*	lambdaCVar;

protected:

  ClassDef(Chemistry,0)
};

#endif

/*! @file Chemistry.h
 * @brief Definition of Chemistry class. Gives the local chemical potential or fugacity.
 */
/*! @class Chemistry
 * @brief Class gives the local chemical potential or fugacity for the given ParticleType.
 *
 * Depending on the number and type of arguments given to the setChemistry() function, the class 
 * will return chemical potential or fugacity that is constant or as a function of <b>x</b>,<b>y</b> and <b>z</b>.
 * 
 * ParticleType class holds inforamption on the particle's quark content @f$ N_i @f$ (with index 
 * @f$ i = q, \bar q, s, \bar s, c, \bar c @f$) and quantum numbers @f$ B @f$, @f$ I_3 @f$, @f$ S @f$ and @f$ C @f$. <br />
 * Chemical potential is calculated as follows
 * @f[ \mu = \mu_B B + \mu_{I_3} I_3 + \mu_S S + \mu_C C@f]
 * and the fugacity
 * @f[ \Upsilon = \lambda_{I_3} \,\gamamp_q^{N_q + N_{\bar q}} \,\lambda_q^{N_q - N_{\bar q}} \,\gamamp_s^{N_s + N_{\bar s}} \,\lambda_s^{N_s - N_{\bar s}} \,\gamamp_c^{N_c + N_{\bar c}} \,\lambda_c^{N_c - N_{\bar c}} @f]
 *
 * @fn Chemistry::Chemistry()
 * @brief Default constructor.
 *
 * @fn Chemistry::~Chemistry()
 * @brief Destructor.
 *
 * @fn int Chemistry::getChemistryType() const
 * @brief get the type of chemistry stored.
 * @retval 0 constant chemical potentials @f$ \mu_i @f$
 * @retval 1 chemical potentials @f$ \mu_i(x, y, z) @f$
 * @retval 2 constant fugacities @f$ \lambda_j @f$ and phase space occupancies @f$ \gamamp_k @f$
 * @retval 3 fugacities @f$ \lambda_j(x, y, z) @f$ and phase space occupancies @f$ \gamamp_k(x, y, z) @f$
 * @retval -1 unknown type
 * 
 * where @f$ i = B, I_3, S, C @f$, @f$ j = I_3, q, s, c @f$, @f$ k = q, s, c @f$.
 *
 * @fn double Chemistry::getChemicalPotential(ParticleType* aPartType) const
 * @brief Returns the constant chemical potential @f$ \mu @f$ for a given particle type.
 * @param [in] aPartType type of the particle
 *
 * @fn double Chemistry::getChemicalPotential(ParticleType* aPartType, double aX, double aY, double aZ) const
 * @brief Returns the chemical potential @f$ \mu(x, y, z) @f$ for a given particle type
 * @param [in] aPartType type of the particle
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn double Chemistry::getFugacity(ParticleType* aPartType) const
 * @brief Returns the constant fugacity @f$ \Upsilon @f$ for a given particle type.
 * @param [in] aPartType type of the particle
 *
 * @fn double Chemistry::getFugacity(ParticleType* aPartType, double aX, double aY, double aZ) const
 * @brief Returns the fugacity @f$ \Upsilon(x, y, z) @f$ for a given particle type
 * @param [in] aPartType type of the particle
 * @param [in] aX x-coordinate
 * @param [in] aY y-coordinate
 * @param [in] aZ z-coordinate
 *
 * @fn double Chemistry::getMuB() const
 * @brief returns baryochemical potential
 * @warning only constant type
 *
 * @fn double Chemistry::getMuI() const
 * @brief returns isospin chemical potential
 * @warning only constant type
 *
 * @fn double Chemistry::getMuS() const
 * @brief returns strange chemical potential
 * @warning only constant type
 *
 * @fn double Chemistry::getMuC() const
 * @brief returns charm chemical potential
 * @warning only constant type
 *
 * @fn double Chemistry::getLambdaQ() const
 * @brief returns light quark fugacity factor
 * @warning only constant type
 *
 * @fn double Chemistry::getLambdaI() const
 * @brief returns isospin fugacity factor
 * @warning only constant type
 *
 * @fn double Chemistry::getLambdaS() const
 * @brief returns strange quark fugacity factor
 * @warning only constant type
 *
 * @fn double Chemistry::getLambdaC() const
 * @brief returns charm quark fugacity factor
 * @warning only constant type
 *
 * @fn double Chemistry::getGammaQ() const
 * @brief returns light quark phase space occupancy
 * @warning only constant type
 *
 * @fn double Chemistry::getGammaS() const
 * @brief returns strange quark phase space occupancy
 * @warning only constant type
 *
 * @fn double Chemistry::getGammaC() const
 * @brief returns charm quark phase space occupancy
 * @warning only constant type
 *
 * @fn void Chemistry::setChemistry(double aMuB, double aMuI, double aMuS, double aMuC)
 * @brief sets the chemistry type to constant chemical potentials.
 * @param [in] aMuB baryochemical potential
 * @param [in] aMuI isospin chemical potential
 * @param [in] aMuS strange chemical potential
 * @param [in] aMuC strange chemical potential
 *
 * @fn void Chemistry::setChemistry(VectorField* aMuB, VectorField* aMuI, VectorField* aMuS, VectorField* aMuC)
 * @brief sets the chemistry type to variable chemical potentials.
 * @param [in] aMuB baryochemical potential
 * @param [in] aMuI isospin chemical potential
 * @param [in] aMuS strange chemical potential
 * @param [in] aMuC strange chemical potential
 *
 * @fn void Chemistry::setChemistry(double aLambdaQ, double aLambdaI, double aLambdaS, double aLambdaC, double aGamampQ, double aGamampS, double aGamampC)
 * @brief sets the chemistry type to constant fugacities and phase space occupancies.
 * @param [in] aLambdaQ light quark fugacity factor
 * @param [in] aLambdaI isospin fugacity factor
 * @param [in] aLambdaS strange quark fugacity factor
 * @param [in] aLambdaC charm quark fugacity factor
 * @param [in] aGamampQ light quark phase space occupancy
 * @param [in] aGamampS strange quark phase space occupancy
 * @param [in] aGamampC charm quark phase space occupancy
 * 
 *
 * @fn void Chemistry::setChemistry(VectorField* aLambdaQ, VectorField* aLambdaI, VectorField* aLambdaS, VectorField* aLambdaC, VectorField* aGamampQ, VectorField* aGamampS, VectorField* aGamampC)
 * @brief sets the chemistry type to variable fugacities and phase space occupancies.
 * @param [in] aLambdaQ light quark fugacity factor
 * @param [in] aLambdaI isospin fugacity factor
 * @param [in] aLambdaS strange quark fugacity factor
 * @param [in] aLambdaC charm quark fugacity factor
 * @param [in] aGamampQ light quark phase space occupancy
 * @param [in] aGamampS strange quark phase space occupancy
 * @param [in] aGamampC charm quark phase space occupancy
 */
