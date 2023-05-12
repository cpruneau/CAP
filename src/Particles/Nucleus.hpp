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
#ifndef CAP__Nucleus
#define CAP__Nucleus
#include "Particle.hpp"

namespace CAP
{

// ***************************************************************************
// Nucleus
//
// nProtons      : number of protons
// nNeutrons     : number of neutron
// ***************************************************************************
class Nucleus  : public  Particle
{
public:

  Nucleus();
  virtual  ~Nucleus()
  {
  // no ops.
  }
  Nucleus(const Nucleus & otherNucleus);
  Nucleus & operator=(const Nucleus & otherNucleus);

  virtual void defineAs(unsigned int z, unsigned int a);
  virtual void clear();
  virtual void reset();
  virtual unsigned int  countWounded();
  vector<Particle*> getWoundedNucleons();

  Particle * getNucleonAt(unsigned int index)
  {
  if (index>=children.size())
    return nullptr;
  else
   return children[index];
  }

  unsigned int getNProtons() const
  {
  return nProtons;
  }

  unsigned int getNNeutrons() const
  {
    return nNeutrons;
  }

  unsigned int getNNucleons() const
  {
    return nProtons+nNeutrons;
  }

protected:

  unsigned int nProtons;
  unsigned int nNeutrons;

  ClassDef(Nucleus,0)
  
};

} // namespace CAP

#endif /* Nucleus_hpp */
