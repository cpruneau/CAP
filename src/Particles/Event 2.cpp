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
#include "Event.hpp"
using CAP::Event;
using CAP::Particle;

ClassImp(Event);

Event::Event()
 :
eventIndex(0),
eventNumber(0),
particles(),
eventProperties(new EventProperties() ),
//b(-9999.0),
nucleusA(new Nucleus()),
nucleusB(new Nucleus())
//binaryMoments(new CollisionGeometryMoments()),
//participantMoments(new CollisionGeometryMoments())
{
 // no ops
}

// ====================================================
// DTOR
// ====================================================
Event::~Event()
{
  particles.clear();
  eventProperties->clear();
  delete eventProperties;
  delete nucleusA;
  delete nucleusB;
//  delete binaryMoments;
//  delete participantMoments;
}

// ====================================================
// Call before (re)starting simulation
// ====================================================
void Event::clear()
{
  eventIndex      = 0;
  eventNumber     = 0;
  //b               = -99999;
  particles.clear();
  if (nucleusA) nucleusA->clear();
  if (nucleusB) nucleusB->clear();
//  if (binaryMoments) binaryMoments->reset();
//  if (participantMoments) participantMoments->reset();
  if (eventProperties) eventProperties->reset();
  //Particle::getFactory()->reset();
}

// ====================================================
// Call before generating new event
// ====================================================
void Event::reset()
{
  eventIndex++;
  eventNumber   = 0;
  //b             = -99999;
  particles.clear();
  if (nucleusA) nucleusA->reset();
  if (nucleusB) nucleusB->reset();
//  if (binaryMoments) binaryMoments->reset();
//  if (participantMoments) participantMoments->reset();
  if (eventProperties) eventProperties->reset();
}


// ====================================================
// Get the particle at the given index
// ====================================================
void Event::add(Particle * particle)
{
  particles.push_back(particle);
}


Particle* Event::addInteraction(Particle* particleA,
                                Particle* particleB)
{

  Particle * interaction = Particle::getFactory()->getNextObject();
  interaction->setParents(particleA,particleB);
  interaction->setType(ParticleType::getInteractionType());
  interaction->setLive(true);
  add(interaction);
  return interaction;
}

unsigned int Event::getNParticipants() const
{
  if (nucleusA && nucleusB)
    return  nucleusA->countWounded() + nucleusB->countWounded();
  else
    return 0;
}

unsigned int Event::getNBinaryCollisions() const
{
  unsigned int  nBinary = 0;
  for (unsigned int iPart=0; iPart<particles.size(); iPart++)
    {
    if (particles[iPart]->isNucleonNucleonInteraction()) nBinary++;
    }
  return nBinary;
}

vector<Particle*> Event::getNucleonNucleonInteractions()
{
  vector<Particle*>  interactions;
  for (unsigned int iPart=0; iPart<particles.size(); iPart++)
    {
    if (particles[iPart]->isNucleonNucleonInteraction()) interactions.push_back(particles[iPart]);
    }
  return interactions;
}



//unsigned int CollisionGeometry::getNProtonProtonCollisions()   const
//{
//  unsigned int n = 0;
//  for (unsigned int k=0; k<nnInteractions.size(); k++)
//    {
//    if (nnInteractions[k].isProtonProton()) n++;
//    }
//  return n;
//}
//
//unsigned int CollisionGeometry::getNProtonNeutronCollisions()  const
//{
//  unsigned int n = 0;
//  for (unsigned int k=0; k<nnInteractions.size(); k++)
//    {
//    if (nnInteractions[k].isProtonNeutron()) n++;
//    }
//  return n;
//}
//
//unsigned int CollisionGeometry::getNNeutronNeutronCollisions() const
//{
//  unsigned int n = 0;
//  for (unsigned int k=0; k<nnInteractions.size(); k++)
//    {
//    if (nnInteractions[k].isNeutronNeutron()) n++;
//    }
//  return n;
//}


void Event::setNucleusA(unsigned int z, unsigned int a)
{
  if (!nucleusA) nucleusA = new Nucleus();
  if (nucleusA->getNProtons()==z && nucleusA->getNNucleons()==a) return;
  nucleusA->defineAs(z,a);
}

void Event::setNucleusB(unsigned int z, unsigned  int a)
{
  if (!nucleusB) nucleusB = new Nucleus();
  if (nucleusB->getNProtons()==z && nucleusB->getNNucleons()==a) return;
  nucleusB->defineAs(z,a);
}


 ///////////////////////////////////////////////////////
 // Print properties of this event at the given output
 ///////////////////////////////////////////////////////
void Event::printProperties(ostream & output)
 {
 output << "----------------------------------------------------------------" << endl;
 output << "      stream index : " << streamIndex << endl;
 output << "       event index : " << eventIndex << endl;
 output << "      event number : " << eventNumber << endl;
 output << " number of particle: " << particles.size() << endl;
 eventProperties->printProperties(output);
 for (unsigned long iParticle=0; iParticle<particles.size(); iParticle++)
   {
   particles[iParticle]->printProperties(output);
   }
 }

vector<Event*> Event::eventStreamsStore;

Event * Event::getEventStream(unsigned int index)
{
  //cout << "Event::getEventStream(unsigned int index) w/ index:" << index << endl;
  unsigned int nStreams = eventStreamsStore.size();
  Event * event;
  if (nStreams == 0)
    {
    // none exist, create one
    cout << "Event::getEventStream(unsigned int index) nStreams == 0; Create an event" << endl;
    event = new Event();
    event->setStreamIndex(index);
    eventStreamsStore.push_back(event);
    return event;
    }
  // streams already exist, find the one requested
  //cout << "Event::getEventStream(unsigned int index) nStreams: " << nStreams << " Find it" << endl;

  for (unsigned int k=0; k<nStreams; k++)
    {
    event = eventStreamsStore[k];
    if (event->getStreamIndex() == index) return event;
    }
  //cout << "Event::getEventStream(unsigned int index) stream index " << index << " not found. Create it" << endl;

  // stream does not exist, create it
  event = new Event();
  event->setStreamIndex(index);
  eventStreamsStore.push_back(event);
  return event;
}

unsigned int Event::getNEventStreams()
{
  return eventStreamsStore.size();
}

void Event::resetEventStreams()
{
  unsigned int nStreams = eventStreamsStore.size();
  for (unsigned int iStream=0; iStream<nStreams; iStream++)
    {
    eventStreamsStore[iStream]->reset();
    }
}


void Event::clearEventStreams()
{
  unsigned int nStreams = eventStreamsStore.size();
  for (unsigned int iStream=0; iStream<nStreams; iStream++)
    {
    delete eventStreamsStore[iStream];
    }
  eventStreamsStore.clear();
}

