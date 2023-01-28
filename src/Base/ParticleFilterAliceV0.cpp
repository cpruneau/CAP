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
#include <TMath.h>
#include "ParticleFilterAliceV0.hpp"
using CAP::ParticleFilterAliceV0;

ClassImp(ParticleFilterAliceV0);

// ==========================================================================================
// CTOR
// Note: To turn off filtering on a specific variable, set the min large than the max.
// ==========================================================================================
ParticleFilterAliceV0::ParticleFilterAliceV0(int option=0)
:
ParticleFilter()
{
  switch (option)
    {
      case 0: // V0M
      setName("V0M");
      setLongName("V0M");
      setTitle("V0M");
      setLongTitle("V0M");
      addCondition(0, 1,    0.0, 0.0);  // live particles only
      addCondition(4, 1101, 0.0, 0.0);   // low mass charged hadron
      addCondition(5, 1,    0.05, 100.0);   // pt Cut
      addCondition(5, 7, -3.7, -1.7, 2.8, 5.1); // pseudorapidity in V0A and V0C

      case 1: // V0A
      setName("V0A");
      setLongName("V0A");
      setTitle("V0A");
      setLongTitle("V0A");
      addCondition(0, 1,  0.0, 0.0);  // live particles only
      addCondition(4, 1101, 0.0, 0.0);   // low mass charged hadron
      addCondition(5, 1, 0.05, 100.0);
      addCondition(5, 7, 2.8, 5.1);

      case 2: // V0C
      setName("V0C");
      setLongName("V0C");
      setTitle("V0C");
      setLongTitle("V0C");
      addCondition(0, 1,  0.0, 0.0);  // live particles only
      addCondition(4, 1101, 0.0, 0.0);   // low mass charged hadron
      addCondition(5, 1, 0.05, 100.0);
      addCondition(5, 7, -3.7, -1.7);
    }
}


ParticleFilterAliceV0 & ParticleFilterAliceV0::operator=( const ParticleFilterAliceV0 & otherFilter)
{
  if (this!=&otherFilter)
    {
    ParticleFilter::operator=(otherFilter);
    }
  return *this;
}

