/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Chun Shen
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 * This code is adapted from Chun Shen's code
 * for the purpose of computing hadronic resonance
 * correlations..
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/
#ifndef CAP__ParticleDecayMode
#define CAP__ParticleDecayMode
#include <iostream>
#include <iomanip>
#include <vector>
#include "TString.h"
#include "ParticleType.hpp"
#include "ParticleDb.hpp"

using namespace std;

namespace CAP
{

class ParticleType;
class ParticleDb;

class ParticleDecayMode
{
private:

  double branchingRatio;
  vector<int> childrenPdgCodes;
  vector<ParticleType*> childrenTypes;

public:
  ParticleDecayMode();
  ParticleDecayMode(const ParticleDecayMode & source);
  virtual ~ParticleDecayMode();
  ParticleDecayMode & operator=(const ParticleDecayMode & source);
  void resolveTypes(ParticleDb & collection);

  void   setBranchingRatio(double _branchingRatio);
  double getBranchingRatio() const;
  int    getNChildren() const;
  void   addChild(int pdgCode);
  void   addChild(ParticleType* type);
  inline const vector<int> & getChildrenPdgCodes() const;
  inline vector<int> & getChildrenPdgCodes();
  const vector<ParticleType*> & getChildrenTypes() const;
  vector<ParticleType*> & getChildrenTypes();
  ParticleType & getChildType(unsigned int index) const;
  ParticleType & getChildType(unsigned int index);
  int getChildPdgCode(unsigned int index) const ;
  int getChildPdgCode(unsigned int index);
  ostream & printProperties(ostream & os);

  ClassDef(ParticleDecayMode,0)
};

} // namespace CAP

#endif  // CAP__ParticleDecayMode

