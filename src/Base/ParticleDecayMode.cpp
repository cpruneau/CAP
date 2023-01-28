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
#include "ParticleDecayMode.hpp"
#include "SelectionGenerator.hpp"
using CAP::ParticleDecayMode;

ClassImp(ParticleDecayMode);

ParticleDecayMode::ParticleDecayMode()
:
branchingRatio(1.0),
childrenCodes(),
childrenTypes()
{  }

ParticleDecayMode::ParticleDecayMode(const ParticleDecayMode & source)
:
branchingRatio(source.branchingRatio),
childrenCodes(source.childrenCodes),
childrenTypes(source.childrenTypes)
{  }

ParticleDecayMode::~ParticleDecayMode()
{
  childrenCodes.clear();
  childrenTypes.clear();
}

ParticleDecayMode & ParticleDecayMode::operator=(const ParticleDecayMode & source)
{
  if (this!=&source)
    {
    branchingRatio = source.branchingRatio;
    childrenCodes  = source.childrenCodes;
    childrenTypes = source.childrenTypes;
    }
  return *this;
}

void ParticleDecayMode::resolveTypes(ParticleTypeCollection * collection)
{
  int n = childrenCodes.size();
  if (n<1) return;
  for (int k=0; k<n; k++)
  {
  ParticleType * type = collection->findPdgCode( childrenCodes[k] );
  if (type) childrenTypes.push_back(type);
  }
}

ostream & ParticleDecayMode::printProperties(ostream & os)
{
  os << " Br:" << branchingRatio << "  ";
  for (unsigned int k=0; k<childrenCodes.size(); k++)
  {
  os << childrenCodes[k]  << "  ";
  }
  os << endl;
  return os;
}

