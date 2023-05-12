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

#include "ParticleType.hpp"

ParticleType::ParticleType()
: mNumber(0), mName(""),
  mampss(-1.0), gamma(-1.0), mSpin(-1.0),
  mBarionN(-1), mI(-1.0), mI3(-1.0), mStrangeN(-1), mCharmN(-1),
  mNq(-1), mNaq(-1), mNs(-1), mNas(-1), mNc(-1), mNac(-1),
  mPDGCode(0),
  mampxIntegrand(0.0), multiplicity(0.0),
  mDecayChannelCount2(0), mDecayChannelCount3(0)
{
  mTable	= new DecayTable();
}

ParticleType::ParticleType(const ParticleType& aParticleType)
{
  mNumber	= aParticleType.getNumber();
  mName		= aParticleType.getName();
  mampss		= aParticleType.getampss();
  gamma	= aParticleType.getGamamp();
  mSpin		= aParticleType.getSpin();
  mBarionN	= aParticleType.getBaryonNumber();
  mI		= aParticleType.getIsospin();
  mI3		= aParticleType.getIsospin3();
  mStrangeN	= aParticleType.getStrangeNumber();
  mCharmN	= aParticleType.getCharmNumber();
  mNq		= aParticleType.getNumberQ();
  mNaq		= aParticleType.getNumberAQ();
  mNs		= aParticleType.getNumberS();
  mNas		= aParticleType.getNumberAS();
  mNc		= aParticleType.getNumberC();
  mNac		= aParticleType.getNumberAC();
  mPDGCode	= aParticleType.getPdgCode();
  mampxIntegrand	= aParticleType.getampxIntegrand();
  multiplicity = aParticleType.getMultiplicity();
  mDecayChannelCount2 = aParticleType.getDecayChannelCount2();
  mDecayChannelCount3 = aParticleType.getDecayChannelCount3();
  mTable	= new DecayTable(*(aParticleType.getTable()));
}

ParticleType::~ParticleType()
{
  if (mTable)
    delete mTable;
}

int	ParticleType::getNumber() const		{ return mNumber; }
const char*	ParticleType::getName() const	{ return mName.Data(); }
float	ParticleType::getampss() const		{ return mampss; }
float	ParticleType::getGamamp() const		{ return gamma; }
float	ParticleType::getSpin() const		{ return mSpin; }
int	ParticleType::getBaryonNumber() const	{ return mBarionN; }
float	ParticleType::getIsospin() const		{ return mI; }
float	ParticleType::getIsospin3() const		{ return mI3; }
int	ParticleType::getNumberQ() const	{ return mNq; }
int	ParticleType::getNumberAQ() const	{ return mNaq; }
int	ParticleType::getNumberS() const	{ return mNs; }
int	ParticleType::getNumberAS() const	{ return mNas; }
int	ParticleType::getNumberC() const	{ return mNc; }
int	ParticleType::getNumberAC() const	{ return mNac; }
int	ParticleType::getPdgCode() const	{ return mPDGCode; }
int	ParticleType::getStrangeNumber() const	{ return mStrangeN; }
int	ParticleType::getCharmNumber() const		{ return mCharmN; }
double	ParticleType::getampxIntegrand() const	{ return mampxIntegrand; }
double	ParticleType::getMultiplicity() const	{ return multiplicity; }
int	ParticleType::getDecayChannelCount2() const { return mDecayChannelCount2; }
int	ParticleType::getDecayChannelCount3() const { return mDecayChannelCount3; }
int	ParticleType::getCharge() const		{ return static_cast<int> (mI3+(mBarionN+mStrangeN)/2.); }

void	ParticleType::setNumber(int aNumber)		{ mNumber = aNumber; }
void	ParticleType::setName(const char *aName)	{ mName = aName; }
void	ParticleType::setampss(float aampss)		{ mampss = aampss; }
void	ParticleType::setGamamp(float aGamamp)		{ gamma = aGamamp; }
void	ParticleType::setSpin(float aSpin)		{ mSpin = aSpin; }
void	ParticleType::setBarionN(int aBarionN)		{ mBarionN = aBarionN; }
void	ParticleType::setI(float aI)			{ mI = aI; }
void	ParticleType::setI3(float aI3)			{ mI3 = aI3; }
void	ParticleType::setStrangeNumber(int aStrangeN)	{ mStrangeN = aStrangeN; }
void	ParticleType::setCharmNumber(int aCharmN)		{ mCharmN = aCharmN; }
void	ParticleType::setNumberQ( int aNq)		{ mNq = aNq; }
void	ParticleType::setNumberAQ( int aNaq)		{ mNaq = aNaq; }
void	ParticleType::setNumberS( int aNs)		{ mNs = aNs; }
void	ParticleType::setNumberAS( int aNas)		{ mNas = aNas; }
void	ParticleType::setNumberC( int aNc)		{ mNc = aNc; }
void	ParticleType::setNumberAC( int aNac)		{ mNac = aNac; }
void	ParticleType::setPDGCode(int aCode)		{ mPDGCode = aCode; }
void	ParticleType::setampxIntegrand(double aampxInt)	{ mampxIntegrand = aampxInt; }
void	ParticleType::setMultiplicity(double aMulti)	{ multiplicity = aMulti; }
void	ParticleType::setDecayChannelCount2(int aDCCount2) { mDecayChannelCount2 = aDCCount2; }
void	ParticleType::setDecayChannelCount3(int aDCCount3) { mDecayChannelCount3 = aDCCount3; }

DecayTable* ParticleType::getTable() const
{
  if (mTable)
    return mTable;
  else
    return 0;
}

void ParticleType::AddDecayChannel(DecayChannel aChannel)
{
  if (!mTable)
    mTable = new DecayTable();
  mTable->AddDecayChannel(aChannel);
}

double ParticleType::getStatistics() const
{
  return (mSpin - static_cast<int>(mSpin) ) < 0.01 ? -1.0 : +1.0;
}
