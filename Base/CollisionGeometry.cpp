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
#include <iostream>
#include "CollisionGeometry.hpp"

using namespace std;

ClassImp(CollisionGeometry);

CollisionGeometry::CollisionGeometry()
:
b(0.0),
nucleusA(),
nucleusB(),
nnInteractions(),
binaryMoments(),
participantMoments()
{
 // no ops
}

CollisionGeometry::CollisionGeometry(const CollisionGeometry & collisionGeometry)
:
b(collisionGeometry.b),
nucleusA(collisionGeometry.nucleusA),
nucleusB(collisionGeometry.nucleusB),
nnInteractions(collisionGeometry.nnInteractions),
binaryMoments(collisionGeometry.binaryMoments),
participantMoments(collisionGeometry.participantMoments)
{

}

CollisionGeometry & CollisionGeometry::operator=(const CollisionGeometry & collisionGeometry)
{
  if (&collisionGeometry != this)
    {
    b             = collisionGeometry.b;
    nucleusA      = collisionGeometry.nucleusA;
    nucleusB      = collisionGeometry.nucleusB;
    nnInteractions     = collisionGeometry.nnInteractions;
    binaryMoments      = collisionGeometry.binaryMoments;
    participantMoments = collisionGeometry.participantMoments;
    }
  return *this;
}

void CollisionGeometry::clear()
{
  b = -99999;
  nucleusA.clear();
  nucleusB.clear();
  nnInteractions.clear();
  binaryMoments.reset();
  participantMoments.reset();
}

void CollisionGeometry::reset()
{
  b = -99999;
  nucleusA.reset();
  nucleusB.reset();
  nnInteractions.clear();
  binaryMoments.reset();
  participantMoments.reset();
}

void CollisionGeometry::addNNCollision(Particle* nucleonA,
                                       Particle* nucleonB)
{

  Particle interaction;
  interaction.setParents(nucleonA,nucleonB);

  //cout << " A:  x= " << nucleonA->getPosition().X() << " B:  x= " << nucleonB->getPosition().X() << " I:  x= " << interaction.getPosition().X() << endl;

  nnInteractions.push_back(interaction);

  if (!nucleonA->isWounded())
    {
    TLorentzVector & positionA = nucleonA->getPosition();
    participantMoments.fill(positionA.X() ,positionA.Y());
    nucleonA->setWounded(true);
    }
  if (!nucleonB->isWounded())
    {
    TLorentzVector & positionB = nucleonB->getPosition();
    participantMoments.fill(positionB.X() ,positionB.Y());
    nucleonB->setWounded(true);
    }
  TLorentzVector & positionInt = interaction.getPosition();
  binaryMoments.fill(positionInt.X(),positionInt.Y());
}

void CollisionGeometry::calculateMoments()
{
  binaryMoments.calculate();
  participantMoments.calculate();
}


unsigned int CollisionGeometry::getNParticipants()
{
  return  nucleusA.countWounded() + nucleusB.countWounded();
}

unsigned int CollisionGeometry::getNProtonProtonCollisions()   const
{
  unsigned int n = 0;
  for (unsigned int k=0; k<nnInteractions.size(); k++)
    {
    if (nnInteractions[k].isProtonProton()) n++;
    }
  return n;
}

unsigned int CollisionGeometry::getNProtonNeutronCollisions()  const
{
  unsigned int n = 0;
  for (unsigned int k=0; k<nnInteractions.size(); k++)
    {
    if (nnInteractions[k].isProtonNeutron()) n++;
    }
  return n;
}

unsigned int CollisionGeometry::getNNeutronNeutronCollisions() const
{
  unsigned int n = 0;
  for (unsigned int k=0; k<nnInteractions.size(); k++)
    {
    if (nnInteractions[k].isNeutronNeutron()) n++;
    }
  return n;
}

CollisionGeometry * CollisionGeometry::defaultCollisionGeometry = nullptr;

CollisionGeometry * CollisionGeometry::getDefaultCollisionGeometry()
{
  if (!defaultCollisionGeometry)  defaultCollisionGeometry = new CollisionGeometry();
  return defaultCollisionGeometry;
}


void CollisionGeometry::setNucleusA(int z, int a)
{
  nucleusA.defineAs(z,a);
}

void CollisionGeometry::setNucleusB(int z, int a)
{
  nucleusB.defineAs(z,a);
}

