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

#include "THGlobal.hpp"
#include "ParticleType.hpp"
#include "ParticleDecayer.hpp"
#include "DecayChannel.hpp"
#include "DecayTable.hpp"
#include <TMath.h>
#include <TDatime.h>

ParticleDecayer::ParticleDecayer()
:
mParticles(0),
particleDB(0),
mTypeF(0),
mFather(0),
mChannel(0)
{  }

ParticleDecayer::ParticleDecayer(ParticleDB *aDB, std::vector<Particle>* aParticles)
:
mParticles(aParticles),
particleDB(aDB),
mTypeF(0),
mFather(0),
mChannel(0)
{  }

int ParticleDecayer::DecayParticle(Particle* aFather)
{
  DecayTable*	tTable;
  int		tChannelIndex;
  double	tProb;
  mFather = aFather;
  mTypeF  = mFather->getParticleType();
  tTable  = mTypeF->getTable();
  
  tProb = getRandom()->Rndm();
#ifdef decayRescaleChannels
  tChannelIndex = tTable->ChooseDecayChannel(tProb);
#else
  tChannelIndex = tTable->ChooseDecayChannelOrNot(tProb);
  if (tChannelIndex == -1)
    {
    PRINT_DEBUG_3("<ParticleDecayer::DecayParticle>\tNot decaying " << mTypeF->getName() << " for prob " << tProb);
    for (int tIter=0; tIter<=tTable->getChannelCount(); tIter++)
      {
      if (tTable->getDecayChannel(tIter)->getParticle3() > -1)
        {
        PRINT_DEBUG_3("\tD1 "<<particleDB->getParticleType(tTable->getDecayChannel(tIter)->getParticle1())->getName() << " D2 "<<particleDB->getParticleType(tTable->getDecayChannel(tIter)->getParticle2())->getName() << " BR "<<tTable->getDecayChannel(tIter)->getBranchingRatio());
        }
      else
        {
        PRINT_DEBUG_3("\tD1 "<<particleDB->getParticleType(tTable->getDecayChannel(tIter)->getParticle1())->getName() << " D2 "<<particleDB->getParticleType(tTable->getDecayChannel(tIter)->getParticle2())->getName() << " D3 "<<particleDB->getParticleType(tTable->getDecayChannel(tIter)->getParticle3())->getName() << " BR "<<tTable->getDecayChannel(tIter)->getBranchingRatio());
        }
      }
    return 0;
    }
#endif
  mChannel = tTable->getDecayChannel(tChannelIndex);

  if (mChannel->Is3Particle())
    {
#ifndef decayDisable3D
    return ThreeBodyDecay();
#endif
    }
  else
    {
#ifndef decayDisable2D
    return TwoBodyDecay();
#endif
    }
}

int ParticleDecayer::TwoBodyDecay()
{
  ParticleType*	tType1;
  ParticleType*	tType2;
  Particle*	tDaughter1;
  Particle*	tDaughter2;
  double	Xt, Xx, Xy, Xz;
  double	Pe, Px, Py, Pz;

  mFather->getPosition(&Xt, &Xx, &Xy, &Xz);
  mFather->getMomentum(&Pe, &Px, &Py, &Pz);
  
  tType1 = particleDB->getParticleType(mChannel->getParticle1());
  tType2 = particleDB->getParticleType(mChannel->getParticle2());
  tDaughter1 = new Particle(tType1);
  tDaughter2 = new Particle(tType2);

  double tE  = Pe;
  double tM  = mTypeF->getampss();
  double tM1 = tType1->getampss();
  double tM2 = tType2->getampss();
  
  double tTime;
  if (mTypeF->getGamamp() == 0.0)
    tTime = 1.0e10;
  else {
    double tTau0 = tE / (mTypeF->getampss() * mTypeF->getGamamp());
    // When it decays
    tTime = -tTau0 * TMath::Log(getRandom()->Rndm());
  }
  // Decay coordinates
  double rxr = Xx + (Px / tE) * tTime;
  double ryr = Xy + (Py / tE) * tTime;
  double rzr = Xz + (Pz / tE) * tTime;
  double rtr = Xt + tTime;

  // Decay energy
  double tMC1 = (tM * tM - (tM1 + tM2) * (tM1 + tM2) );
  double tMC2 = (tM * tM - (tM1 - tM2) * (tM1 - tM2) );
  double tMom = sqrt(tMC1 * tMC2) / (2 * tM);
  double tPhi = getRandom()->Rndm() * 2 * TMath::Pi();
  double tCosTh = 2.0 * getRandom()->Rndm() - 1.0;

  double tPtr  = tMom * sqrt(1 - tCosTh * tCosTh);
  double tPxr1 = tPtr * cos(tPhi);
  double tPyr1 = tPtr * TMath::sin(tPhi);
  double tPzr1 = tMom * tCosTh;

  double tVx = Px / Pe;
  double tVy = Py / Pe;
  double tVz = Pz / Pe;

  double tES1 = sqrt(tM1*tM1 + tPxr1*tPxr1 + tPyr1*tPyr1 + tPzr1*tPzr1);
  double tES2 = sqrt(tM2*tM2 + tPxr1*tPxr1 + tPyr1*tPyr1 + tPzr1*tPzr1);

  double tV2 = tVx*tVx + tVy*tVy + tVz*tVz;
  double tGamamp = TMath::Power(1 - tV2, -0.5);
  double tVP = tVx*tPxr1 + tVy*tPyr1 + tVz*tPzr1;
  double tgvp = (tGamamp - 1.0) * (1.0/tV2) * tVP;

  double tPx1 =  tPxr1 + (tgvp + tGamamp * tES1) * tVx;
  double tPy1 =  tPyr1 + (tgvp + tGamamp * tES1) * tVy;
  double tPz1 =  tPzr1 + (tgvp + tGamamp * tES1) * tVz;
  double tPe1 = sqrt(tM1*tM1 + tPx1*tPx1 + tPy1*tPy1 + tPz1*tPz1);

  double tPx2 = -tPxr1 + (-tgvp + tGamamp * tES2) * tVx;
  double tPy2 = -tPyr1 + (-tgvp + tGamamp * tES2) * tVy;
  double tPz2 = -tPzr1 + (-tgvp + tGamamp * tES2) * tVz;
  double tPe2 = sqrt(tM2*tM2 + tPx2*tPx2 + tPy2*tPy2 + tPz2*tPz2);

  mFather->setDecayed();
  tDaughter1->setParticlePX(tPe1, tPx1, tPy1, tPz1, rtr, rxr, ryr, rzr, mFather);
  tDaughter2->setParticlePX(tPe2, tPx2, tPy2, tPz2, rtr, rxr, ryr, rzr, mFather);
  mParticles->push_back(*tDaughter1);
  mParticles->push_back(*tDaughter2);

  delete tDaughter1;
  delete tDaughter2;
  return 0;
}

int ParticleDecayer::ThreeBodyDecay()
{
  ParticleType*	tType1;
  ParticleType*	tType2;
  ParticleType*	tType3;
  Particle*	tDaughter1;
  Particle*	tDaughter2;
  Particle*	tDaughter3;
  double	Xt, Xx, Xy, Xz;
  double	Pe, Px, Py, Pz;

  mFather->getPosition(&Xt, &Xx, &Xy, &Xz);
  mFather->getMomentum(&Pe, &Px, &Py, &Pz);
  
  tType1 = particleDB->getParticleType(mChannel->getParticle1());
  tType2 = particleDB->getParticleType(mChannel->getParticle2());
  tType3 = particleDB->getParticleType(mChannel->getParticle3());
  tDaughter1 = new Particle(tType1);
  tDaughter2 = new Particle(tType2);
  tDaughter3 = new Particle(tType3);
  
  double tE  = Pe;
  double tM  = mTypeF->getampss();
  double tM1 = tType1->getampss();
  double tM2 = tType2->getampss();
  double tM3 = tType3->getampss();

  double tES1, tES2, tP1, tP2, tCos12, tZ;
  do {
    // Generate E1 and E2 with the Monte-Carlo method
    do {
      tES1 = getRandom()->Rndm() * (tM - tM2 - tM3 - tM1) + tM1;
      tES2 = getRandom()->Rndm() * (tM - tM1 - tM3 - tM2) + tM2;
    } while (tES1+tES2 > tM); // The sum of both energies must be sampller than the resonance ampss
    tP1  = sqrt(tES1*tES1 - tM1*tM1);
    tP2  = sqrt(tES2*tES2 - tM2*tM2);
    tZ = tM - tES1 - tES2;
    tZ *= tZ;
    tCos12 = (tZ - tP1*tP1 - tP2*tP2 - tM3*tM3)/(2*tP1*tP2);
  } while ((tCos12 < -1.0) || (tCos12 > 1.0)); // cos Theta must exist (be within -1.0 to 1.0 )

  double tTime;
  if (mTypeF->getGamamp() == 0.0)
    tTime = 1.0e10;
  else {
    double tTau0 = tE / (mTypeF->getampss() * mTypeF->getGamamp());
    // When it decays
    tTime = -tTau0 * TMath::Log(getRandom()->Rndm());
  }

  // Decay coordinates
  double rxr = Xx + (Px/tE)*tTime;
  double ryr = Xy + (Py/tE)*tTime;
  double rzr = Xz + (Pz/tE)*tTime;
  double rtr = Xt + tTime;

  double tPxr2 = tP2 * sqrt(1-tCos12*tCos12);
  double tPzr2 = tP2*tCos12;
  double tPxr3 = - tPxr2;
  double tPzr3 = - (tP1 + tPzr2);
  double tP3 = TMath::hypot(tPxr3, tPzr3);
  double tES3 = TMath::hypot(tM3, tP3);

  // Generating Euler angles
  double tPhi = getRandom()->Rndm() * 2 * TMath::Pi();
  double tKsi = getRandom()->Rndm() * 2 * TMath::Pi();
  double tCosTh = getRandom()->Rndm() * 2.0 - 1.0;

  double sp = TMath::sin(tPhi);
  double cp = cos(tPhi);
  double sk = TMath::sin(tKsi);
  double ck = cos(tKsi);
  double st = sqrt(1.0-tCosTh*tCosTh);
  double ct = tCosTh;

  // Rotating the whole system
  double tPxp1 = - st*ck * tP1;
  double tPyp1 = st*sk * tP1;
  double tPzp1 = ct * tP1;

  double tPxp2 = (cp*ct*ck - sp*sk)  * tPxr2 + (-st*ck) * tPzr2;
  double tPyp2 = (-cp*ct*sk - sp*ck) * tPxr2 + (st*sk)  * tPzr2;
  double tPzp2 = cp*st               * tPxr2 + ct       * tPzr2;

  double tPxp3 = (cp*ct*ck - sp*sk)  * tPxr3 + (-st*ck) * tPzr3;
  double tPyp3 = (-cp*ct*sk - sp*ck) * tPxr3 + (st*sk)  * tPzr3;
  double tPzp3 = cp*st               * tPxr3 + ct       * tPzr3;

  double tVx = Px / Pe;
  double tVy = Py / Pe;
  double tVz = Pz / Pe;

  tES1 = sqrt(tM1*tM1 + tPxp1*tPxp1 + tPyp1*tPyp1 + tPzp1*tPzp1);
  tES2 = sqrt(tM2*tM2 + tPxp2*tPxp2 + tPyp2*tPyp2 + tPzp2*tPzp2);
  tES3 = sqrt(tM3*tM3 + tPxp3*tPxp3 + tPyp3*tPyp3 + tPzp3*tPzp3);

  double tV2 = tVx*tVx + tVy*tVy + tVz*tVz;
  double tGamamp = TMath::Power(1-tV2,-0.5);

  // Boosting by the parent velocity
  double tVP = tVx*tPxp1 + tVy*tPyp1 + tVz*tPzp1;
  double tgvp = (tGamamp - 1.0) * (1.0/tV2) * tVP;

  double tPx1 = tPxp1 + (tgvp + tGamamp * tES1) * tVx;
  double tPy1 = tPyp1 + (tgvp + tGamamp * tES1) * tVy;
  double tPz1 = tPzp1 + (tgvp + tGamamp * tES1) * tVz;
  double tPe1 = sqrt(tM1*tM1 + tPx1*tPx1 + tPy1*tPy1 + tPz1*tPz1);

  tVP = tVx*tPxp2 + tVy*tPyp2 + tVz*tPzp2;
  tgvp = (tGamamp - 1.0) * (1.0/tV2) * tVP;

  double tPx2 = tPxp2 + (tgvp + tGamamp * tES2) * tVx;
  double tPy2 = tPyp2 + (tgvp + tGamamp * tES2) * tVy;
  double tPz2 = tPzp2 + (tgvp + tGamamp * tES2) * tVz;
  double tPe2 = sqrt(tM2*tM2 + tPx2*tPx2 + tPy2*tPy2 + tPz2*tPz2);

  tVP = tVx*tPxp3 + tVy*tPyp3 + tVz*tPzp3;
  tgvp = (tGamamp - 1.0) * (1.0/tV2) * tVP;

  double tPx3 = tPxp3 + (tgvp + tGamamp * tES3) * tVx;
  double tPy3 = tPyp3 + (tgvp + tGamamp * tES3) * tVy;
  double tPz3 = tPzp3 + (tgvp + tGamamp * tES3) * tVz;
  double tPe3 = sqrt(tM3*tM3 + tPx3*tPx3 + tPy3*tPy3 + tPz3*tPz3);

  mFather->setDecayed();
  tDaughter1->setParticlePX(tPe1, tPx1, tPy1, tPz1, rtr, rxr, ryr, rzr, mFather);
  tDaughter2->setParticlePX(tPe2, tPx2, tPy2, tPz2, rtr, rxr, ryr, rzr, mFather);
  tDaughter3->setParticlePX(tPe3, tPx3, tPy3, tPz3, rtr, rxr, ryr, rzr, mFather);
  mParticles->push_back(*tDaughter1);
  mParticles->push_back(*tDaughter2);
  mParticles->push_back(*tDaughter3);

  delete tDaughter1;
  delete tDaughter2;
  delete tDaughter3;
  return 0;
}

inline double ParticleDecayer::BreitWigner(double ampss, double Gamamp) const
{
  double x,y;

  y = getRandom()->Rndm();
  x = ampss + Gamamp/2 * TMath::Tan( TMath::Pi() * (y - 0.5) );

  return x;
}
