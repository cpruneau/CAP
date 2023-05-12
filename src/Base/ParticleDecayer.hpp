/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/
#ifndef CAP__ParticleDecayer
#define CAP__ParticleDecayer
#include <iostream>
#include <iomanip>
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "Particle.hpp"

using namespace std;

class ParticleDecayer
{
public:
  ParticleDecayer();
  virtual ~ParticleDecayer() {}
  void setRandomGenerator(TRandom * _random) { random = _random; }
  TRandom * getRandomGenerator() { return random; }

  void decay2(ParticleType   & parentType,
              TLorentzVector & parentMomentum,
              TLorentzVector & parentPosition,
              ParticleType   & childType1,
              TLorentzVector & p1,
              TLorentzVector & r1,
              ParticleType   & childType2,
              TLorentzVector & p2,
              TLorentzVector & r2);

  void decay3(ParticleType   & parentType,
              TLorentzVector & parentMomentum,
              TLorentzVector & parentPosition,
              ParticleType   & childType1,
              TLorentzVector & p1,
              TLorentzVector & r1,
              ParticleType   & childType2,
              TLorentzVector & p2,
              TLorentzVector & r2,
              ParticleType   & childType3,
              TLorentzVector & p3,
              TLorentzVector & r3);

  void decay4(ParticleType   & parentType,
              TLorentzVector & parentMomentum,
              TLorentzVector & parentPosition,
              ParticleType   & childType1,
              TLorentzVector & p1,
              TLorentzVector & r1,
              ParticleType   & childType2,
              TLorentzVector & p2,
              TLorentzVector & r2,
              ParticleType   & childType3,
              TLorentzVector & p3,
              TLorentzVector & r3,
              ParticleType   & childType4,
              TLorentzVector & p4,
              TLorentzVector & r4
              );

  void  decay2(Particle & parent,
               Particle & child1,
               Particle & child2);
  void  decay3(Particle & parent,
               Particle & child1,
               Particle & child2,
               Particle & child3);
  void  decay4(Particle & parent,
               Particle & child1,
               Particle & child2,
               Particle & child3,
               Particle & child4);

protected:

  static double   twoPi;

  TRandom * random;



  ClassDef(ParticleDecayer,0)
};

#endif  // ParticleDecayer

