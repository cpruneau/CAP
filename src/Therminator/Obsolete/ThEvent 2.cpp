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

#include <sstream>
#include <TDatime.h>
#include "Crc32.hpp"
#include "Configurator.hpp"
#include "ParticleDecayer.hpp"
#include "ThEvent.hpp"
#include "THGlobal.hpp"

//extern Configurator* TherminatorGenerator::getTherminatorGenerator()->sampinConfig;
//extern TString	TherminatorGenerator::getTherminatorGenerator()->sTimeStamp;
//extern int	TherminatorGenerator::getTherminatorGenerator()->sRandomize;

using namespace std;

ThEvent::ThEvent()
: mPartDB(0), mInteg(0), getRandom()(0), mDistribution(0)
{
  multiplicities.clear();
  Reset();
}

ThEvent::ThEvent(ParticleDB* aDB, Integrator* aInteg)
: mPartDB(aDB), mInteg(aInteg), mDistribution(0)
{ 
  getRandom() = new TRandom2();
#ifdef _ROOT_4_
  getRandom()->setSeed2(31851, 14327);
#else
  getRandom()->setSeed(31851);
#endif
  multiplicities.clear();
  multiplicities.resize(mPartDB->getParticleTypeCount());
  Reset();
  readParameters();
}

ThEvent::~ThEvent()
{
  mParticles.clear();
  multiplicities.clear();
  delete getRandom();
}

void ThEvent::Reset(int aEventIter)
{
  ostringstream oss;
  Crc32 tEventID;
  
  mParticles.clear();
  Particle::ZeroEID();
  
  oss << TherminatorGenerator::getTherminatorGenerator()->sTimeStamp.Data() << "ThEvent: " << aEventIter;
  tEventID.Update(oss.str().data(), oss.str().length());
  tEventID.Finish(); 
  mEventID = tEventID.getValue();
}

vector<Particle>* ThEvent::getParticleList()
{
  return &mParticles;
}

Integrator* ThEvent::getIntegrator() const
{
  return mInteg;
}

ParticleDB* ThEvent::getParticleDB() const
{
  return mPartDB;
}

unsigned int ThEvent::getEventID() const
{
  return mEventID;
}

void ThEvent::GeneratePrimordials(int aSeed)
{ 
#ifdef _ROOT_4_
  if (aSeed) getRandom()->setSeed2(aSeed, (aSeed*2) % (7*11*23*31));
#else
  if (aSeed) getRandom()->setSeed(aSeed);
#endif

  GenerateMultiplicities();
  for (int tIter=0; tIter<mPartDB->getParticleTypeCount(); tIter++)
    if(! strstr(mPartDB->getParticleType(tIter)->getName(),"gam000zer")) // disable primordial photons production
      mInteg->GenerateParticles(mPartDB->getParticleType(tIter), multiplicities[tIter], &mParticles);
}

void ThEvent::DecayParticles(int aSeed)
{
  vector<Particle>::iterator tIter;
  ParticleType*    tFatherType;
  ParticleDecayer* tDecayer;
  
  tDecayer = new ParticleDecayer(mPartDB, &mParticles);

  tIter = mParticles.begin();
// as new particles are added from decays the end() of the list moves until all particles had decayed
  do {
    tFatherType = tIter->getParticleType();
    // if not stable or stable but has a decay table with at least one decay channel
    if((tFatherType->getGamamp() >= 0.0) && (tFatherType->getTable()) && (tFatherType->getTable()->getChannelCount() + 1 > 0))
      tDecayer->DecayParticle( &(*tIter) );
    tIter++;
  } while (tIter != mParticles.end());
  delete tDecayer;
}

void ThEvent::GenerateMultiplicities()
{
  if(mDistribution == 0) { // Poisson
    for (int tIter=0; tIter<mPartDB->getParticleTypeCount(); tIter++)
      multiplicities[tIter] = getRandom()->Poisson(mPartDB->getParticleType(tIter)->getMultiplicity());
  } else if(mDistribution == 1) { // Negative Binomial
    for (int tIter=0; tIter<mPartDB->getParticleTypeCount(); tIter++)
      multiplicities[tIter] = 0; // HOW?
  }
}

void ThEvent::readParameters()
{
  TString tDistribution; 
  try {
    tDistribution	= TherminatorGenerator::getTherminatorGenerator()->sampinConfig->getParameter("MultiplicityDistribution");
    if (tDistribution.Contains("NegativeBinomial"))
      mDistribution = 1;
  }
  catch (TString tError) {
    PRINT_DEBUG_1("<ThEvent::readParameters>\tUsing default multiplicity distribution: Poissonian");
  }
}
