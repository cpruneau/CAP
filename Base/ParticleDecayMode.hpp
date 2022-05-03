/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Chun Shen
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 * This code is adapted from Chun Shen's code
 * for the purpose of computing hadronic resonance
 * correlations...
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
#include "ParticleTypeCollection.hpp"

using namespace std;
class ParticleType;
class ParticleTypeCollection;

class ParticleDecayMode
{
private:
  double branchingRatio;
  vector<int> childrenCodes;
  vector<ParticleType*> childrenTypes;

public:
  ParticleDecayMode();
  ParticleDecayMode(const ParticleDecayMode & source);
  virtual ~ParticleDecayMode();
  ParticleDecayMode & operator=(const ParticleDecayMode & source);

  void   setBranchingRatio(double _branchingRatio) { branchingRatio = _branchingRatio; }
  double getBranchingRatio() const                 { return branchingRatio;            }
  int    getNChildren() const                      { return childrenCodes.size();      }
  void   addChild(int pdgCode)                     { childrenCodes.push_back(pdgCode); }
  void   addChild(ParticleType* type)              { childrenTypes.push_back(type);    }
  vector<int> & getChildrenPdgCodes()              { return childrenCodes;             }
  vector<ParticleType*> & getChildrenTypes()       { return childrenTypes;             }
  ParticleType & getChildType(int index)
  {
  return * childrenTypes[index];
  }
  int getChildPdgCode(int index)
  {
  return childrenCodes[index];
  }
  void resolveTypes(ParticleTypeCollection * collection);
  ostream & printProperties(ostream & os);

  ClassDef(ParticleDecayMode,0)
};

#endif  // CAP__ParticleDecayMode

