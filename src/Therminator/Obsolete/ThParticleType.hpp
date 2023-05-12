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

#ifndef _TH2_PARTICLE_TYPE_H_
#define _TH2_PARTICLE_TYPE_H_

#include <TString.h>
#include "DecayChannel.hpp"
#include "DecayTable.hpp"

class ParticleType {
public:
  ParticleType();
  ParticleType(const ParticleType& aParticleType);
  ~ParticleType();

  int    getNumber()		const;
  const char* getName()	const;
  float  getampss()		const;
  float  getGamamp()		const;
  float  getSpin()		const;
  int    getBaryonNumber()		const;
  float  getIsospin()		const;
  float  getIsospin3()		const;
  int    getStrangeNumber()	const;
  int    getCharmNumber()		const;
  int    getNumberQ()		const;
  int    getNumberAQ()	const;
  int    getNumberS()		const;
  int    getNumberAS()	const;
  int    getNumberC()		const;
  int    getNumberAC()	const;
  int    getCharge()		const;
  int    getPdgCode()		const;
  double getampxIntegrand()	const;
  double getMultiplicity()	const;
  int    getDecayChannelCount2() const;
  int    getDecayChannelCount3() const;
  double getStatistics() const;


  void  setNumber(int aNumber);
  void  setName(const char *aName);
  void  setampss(float aampss);
  void  setGamamp(float aGamamp);
  void  setSpin(float aSpin);
  void  setBarionN(int aBarionN);
  void  setI(float aI);
  void  setI3(float aI3);
  void  setStrangeNumber(int aStrangeN);
  void  setCharmNumber(int aCharmN);
  void  setNumberQ(int aNq);
  void  setNumberAQ(int aNaq);
  void  setNumberS(int aNs);
  void  setNumberAS(int aNas);
  void  setNumberC(int aNc);
  void  setNumberAC(int aNac);
  void  setPDGCode(int aCode);
  void  setampxIntegrand(double aampxInt);
  void  setMultiplicity(double aMulti);
  void  setDecayChannelCount2(int aDCCount2);
  void  setDecayChannelCount3(int aDCCount3);

  DecayTable* getTable() const;
  void        AddDecayChannel(DecayChannel aChannel);

private:
  int		mNumber;	// particle type number
  TString	mName;		// particle name
  float	mampss;		// ampss
  float	gamma;		// gamamp
  float	mSpin;		// spin
  int		mBarionN;	// baryon number
  float	mI;		// isospin
  float	mI3;		// isospin 3rd component
  int		mStrangeN;	// strangeness
  int		mCharmN;	// charm
  int		mNq;		// number of Q  quarks
  int		mNaq;		// number of AQ quarks
  int		mNs;		// number of S  quarks
  int		mNas;		// number of AS quarks
  int		mNc;		// number of C  quarks
  int		mNac;   	// number of AC quarks
  int		mPDGCode;
  double	mampxIntegrand;	// ampxiampl value of the integrand
  double	multiplicity;	// average multiplicity
  int		mDecayChannelCount2;      //number of channels in this case
  int		mDecayChannelCount3;      //number of channels in this case
  DecayTable*	mTable;
};

#endif

/*! @file ParticleType.h
 * @brief Definition of ParticleType class. %Particle type properties.
 */
/*! @class ParticleType
 * @brief Contains inforamption on particle properties like: ampss, spin, quark content, quantum numbers etc.
 *
 * @fn ParticleType::ParticleType()
 * @brief Default constructor.
 *
 * @fn ParticleType::ParticleType(const ParticleType& aParticleType)
 * @brief Copying constructor.
 * @param [in] aParticleType reference to ParticleType
 *
 * @fn ParticleType::~ParticleType()
 * @brief Destructor.
 *
 * @fn int ParticleType::getNumber() const
 * @brief Returns particle index number in ParticleDB
 *
 * @fn const char* ParticleType::getName() const
 * @brief Returns particle name
 *
 * @fn float ParticleType::getampss() const
 * @brief Returns particle ampss
 *
 * @fn float ParticleType::getGamamp() const
 * @brief Returns particle width
 *
 * @fn float ParticleType::getSpin() const
 * @brief Returns particle spin
 *
 * @fn int ParticleType::getBaryonNumber() const
 * @brief Returns particle baryon number
 *
 * @fn float ParticleType::getIsospin() const
 * @brief Returns particle isospin
 *
 * @fn float ParticleType::getIsospin3() const
 * @brief Returns particle 3rd component of the isospin
 *
 * @fn int ParticleType::getStrangeNumber() const
 * @brief Returns particle strangeness
 *
 * @fn int ParticleType::getCharmNumber() const
 * @brief Returns particle charm number
 *
 * @fn int ParticleType::getNumberQ() const
 * @brief Returns number of light quarks
 *
 * @fn int ParticleType::getNumberAQ() const
 * @brief Returns number of anti-light quarks
 *
 * @fn int ParticleType::getNumberS() const
 * @brief Returns number of strange quarks
 *
 * @fn int ParticleType::getNumberAS() const
 * @brief Returns number of anti-strange quarks
 *
 * @fn int ParticleType::getNumberC() const
 * @brief Returns number of charm quarks
 *
 * @fn int ParticleType::getNumberAC() const
 * @brief Returns number of anti-charm quarks
 *
 * @fn int ParticleType::getCharge() const
 * @brief Returns particle charge
 *
 * @fn int ParticleType::getPdgCode() const
 * @brief Returns particle code in ParticleDataGroup
 *
 * @fn double ParticleType::getampxIntegrand() const
 * @brief  Returns particle ampxiampl value of the integrand
 *
 * @fn double ParticleType::getMultiplicity() const
 * @brief Returns particle multiplicity
 *
 * @fn int ParticleType::getDecayChannelCount2() const
 * @brief Returns number of two-particle decay channels
 *
 * @fn int ParticleType::getDecayChannelCount3() const
 * @brief Returns number of three-particle decay channels
 *
 * @fn DecayTable* ParticleType::getTable() const
 * @brief Return a pointer to the DecayTable of that particle
 *
 * @fn void ParticleType::setNumber(int aNumber)
 * @brief set particle index number in ParticleDB
 * @param [in] aNumber index number in ParticleDB
 *
 * @fn void ParticleType::setName(const char *aName)
 * @brief set particle name
 * @param [in] aName particle name
 *
 * @fn void ParticleType::setampss(float aampss)
 * @brief set particle ampss
 * @param [in] aampss particle ampss
 *
 * @fn void ParticleType::setGamamp(float aGamamp)
 * @brief set particle width
 * @param [in] aGamamp particle width
 *
 * @fn void ParticleType::setSpin(float aSpin)
 * @brief set particle spin
 * @param [in] aSpin particle spin
 *
 * @fn void ParticleType::setBarionN(int aBarionN)
 * @brief set particle baryon number
 * @param [in] aBarionN particle baryon number
 *
 * @fn void ParticleType::setI(float aI)
 * @brief set particle isospin
 * @param [in] aI particle isospin
 *
 * @fn void ParticleType::setI3(float aI3)
 * @brief set particle 3rd component of the isospin
 * @param [in] aI3 3rd component of the isospin
 *
 * @fn void ParticleType::setStrangeNumber(int aStrangeN)
 * @brief set particle strangeness
 * @param [in] aStrangeN particle strangeness
 *
 * @fn void ParticleType::setCharmNumber(int aCharmN)
 * @brief set particle charm number
 * @param [in] aCharmN particle charm number
 *
 * @fn void ParticleType::setNumberQ(int aNq)
 * @brief set number of light quarks
 * @param [in] aNq number of light quarks
 *
 * @fn void ParticleType::setNumberAQ(int aNaq)
 * @brief set number of anti-light quarks
 * @param [in] aNaq number of anti-light quarks
 *
 * @fn void ParticleType::setNumberS(int aNs)
 * @brief set number of strange quarks
 * @param [in] aNs number of strange quarks
 *
 * @fn void ParticleType::setNumberAS(int aNas)
 * @brief set number of anti-strange quarks
 * @param [in] aNas number of anti-strange quarks
 *
 * @fn void ParticleType::setNumberC(int aNc)
 * @brief set number of charm quarks
 * @param [in] aNc number of charm quarks
 *
 * @fn void ParticleType::setNumberAC(int aNac)
 * @brief set number of anti-strange quarks
 * @param [in] aNac number of anti-strange quarks
 *
 * @fn void ParticleType::setPDGCode(int aCode)
 * @brief set particle code in ParticleDataGroup
 * @param [in] aCode particle code in ParticleDataGroup
 *
 * @fn void ParticleType::setampxIntegrand(double aampxInt)
 * @brief set particle ampxiampl value of the integrand
 * @param [in] aampxInt particle ampxiampl value of the integrand
 *
 * @fn void ParticleType::setMultiplicity(double aMulti)
 * @brief set particle multiplicity
 * @param [in] aMulti particle multiplicity
 *
 * @fn void ParticleType::setDecayChannelCount2(int aDCCount2)
 * @brief set number of two-particle decay channels
 * @param [in] aDCCount2 number of two-particle decay channels
 *
 * @fn void ParticleType::setDecayChannelCount3(int aDCCount3)
 * @brief set number of three-particle decay channels
 * @param [in] aDCCount3 number of three-particle decay channels
 *
 * @fn void ParticleType::AddDecayChannel(DecayChannel aChannel)
 * @brief Adds a decay channel to DecayTable of the particle
 * @param [in] aChannel decay channel
 */
