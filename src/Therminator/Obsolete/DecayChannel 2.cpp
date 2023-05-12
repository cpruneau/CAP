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

#include "DecayChannel.hpp"

DecayChannel::DecayChannel()
: mParticleType1(0), mParticleType2(0), mParticleType3(-1), mBranchRatio(0.0)
{
}

DecayChannel::DecayChannel(const DecayChannel& aChannel) 
{
  mBranchRatio   = aChannel.getBranchingRatio();
  mParticleType1 = aChannel.getParticle1();
  mParticleType2 = aChannel.getParticle2();
  mParticleType3 = aChannel.getParticle3();
}

DecayChannel::DecayChannel(double aBranchRatio, int aPartType1, int aPartType2, int aPartType3)
: mParticleType1(aPartType1), mParticleType2(aPartType2), mParticleType3(aPartType3), mBranchRatio(aBranchRatio)
{
}

DecayChannel::~DecayChannel()
{
}

int DecayChannel::getParticle1() const
{
  return mParticleType1;
}

int DecayChannel::getParticle2() const
{
  return mParticleType2;
}

int DecayChannel::getParticle3() const
{
  return mParticleType3;
}

double DecayChannel::getBranchingRatio() const
{
  return mBranchRatio;
}

int DecayChannel::Is3Particle() const
{
  return (mParticleType3 != -1);
}

void DecayChannel::setParticle1(int aPartType1)
{
  mParticleType1 = aPartType1;
}

void DecayChannel::setParticle2(int aPartType2)
{
  mParticleType2 = aPartType2;
}

void DecayChannel::setParticle3(int aPartType3)
{
  mParticleType3 = aPartType3;
}

void DecayChannel::setBranchingRatio(double aRatio)
{
  mBranchRatio = aRatio;
}
