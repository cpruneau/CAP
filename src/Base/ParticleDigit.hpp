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
#ifndef CAP__ParticleDigit
#define CAP__ParticleDigit
#include "TString.h"
#include "Factory.hpp"

using namespace std;

//!
//!Class ParticleDigit
//!
class ParticleDigit
{
public:
  
  ParticleDigit();
  ParticleDigit(unsigned int _iY,
                unsigned int _iEta,
                unsigned int _iPhi,
                unsigned int _iPt,
                float       _pt,
                float       _phi,
                float       _eta,
                float       _y,
                float       _e);
  virtual ~ParticleDigit();
  ParticleDigit(const ParticleDigit& other);
  ParticleDigit & operator=(const ParticleDigit & other);
  void printProperties(ostream & output);
  void reset();

  unsigned int iY;
  unsigned int iEta;
  unsigned int iPhi;
  unsigned int iPt;
  float pt;
  float phi;
  float eta;
  float y;
  float e;

  static int factorySize;
  static Factory<ParticleDigit> * factory;
  static Factory<ParticleDigit> * getFactory();


  ClassDef(ParticleDigit,0)
};


#endif /* CAP__ParticleDigit */
