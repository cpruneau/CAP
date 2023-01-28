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
#ifndef CAP__CollisionGeometry
#define CAP__CollisionGeometry
#include "Nucleus.hpp"
#include "NucleusGenerator.hpp"
#include "CollisionGeometryMoments.hpp"

namespace CAP
{


// ***************************************************************************
// CollisionGeometry
//
// type          : proton or neutron (can be used in PYTHIA)
//                 all length units in fm
// x,y,z         : cartesian coordinates relative to its container (nucleus)
// r, phi, theta : spherical coordinates relative to its container (nucleus)
//                 redundant but memory is cheap
// radius        : radius of the CollisionGeometry used in collisions
// ***************************************************************************
class CollisionGeometry 
{
protected:

  double b;
  Nucleus nucleusA;
  Nucleus nucleusB;
  vector<Particle> nnInteractions;
  CollisionGeometryMoments binaryMoments;
  CollisionGeometryMoments participantMoments;

public:

  CollisionGeometry();
  CollisionGeometry(const CollisionGeometry & collisionGeometry);
  CollisionGeometry & operator=(const CollisionGeometry & otherCollisionGeometry);
  virtual ~CollisionGeometry() { }
  virtual void clear();
  virtual void reset();

  void addNNCollision(Particle* nucleonA, Particle* nucleonB);
  void calculateMoments();
  void setImpactParameter(double _b)  { b = _b;   }
  void setNucleusA(int z, int a);
  void setNucleusB(int z, int a);

  double    getImpactParameter() const { return b; }
  double    getCrossSection() const { return b*b/2.0; }
  Nucleus & getNucleusA() { return nucleusA; }
  Nucleus & getNucleusB() { return nucleusB; }
  Particle & getInteractionAt(unsigned long index)
  {
  unsigned long n = nnInteractions.size();
  if (index < n) return nnInteractions[index];
  else  return nnInteractions[n-1];
  }
  const vector<Particle> & getInteractions() const
  {
  return nnInteractions;
  }
  unsigned int getNParticipants();
  unsigned int getNProtonProtonCollisions()   const;
  unsigned int getNProtonNeutronCollisions()  const;
  unsigned int getNNeutronNeutronCollisions() const;
  unsigned int getNBinaryCollisions() const           { return nnInteractions.size(); }

  CollisionGeometryMoments & getBinaryMoments()       { return binaryMoments;      }
  CollisionGeometryMoments & getParticipantMoments()  { return participantMoments; }

protected:

  static CollisionGeometry * defaultCollisionGeometry;

public:

  static CollisionGeometry * getDefaultCollisionGeometry();

  ClassDef(CollisionGeometry,0)
  
};

}


#endif /* CollisionGeometry_hpp */
