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
#include "EventPlaneRandomizerTask.hpp"

ClassImp(EventPlaneRandomizerTask);

EventPlaneRandomizerTask::EventPlaneRandomizerTask(const TString & _name,
                                                   Configuration & _configuration,
                                                   vector<EventFilter*> & _eventFilters,
                                                   vector<ParticleFilter*>& _particleFilters)
:
Task(_name,_configuration,_eventFilters,_particleFilters)
{
  appendClassName("EventPlaneRandomizerTask");
}

void EventPlaneRandomizerTask::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  setParameter("UseParticles",    true);
  setParameter("UseEventStream0", true);
}

void EventPlaneRandomizerTask::execute()
{
  incrementTaskExecuted();
  double eventAngle= TMath::TwoPi() * gRandom->Rndm();
  Event * event = eventStreams[0];
  unsigned int nParticles = event->getNParticles();
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    Particle * particle = event->getParticleAt(iParticle);
    particle->getPosition().RotateZ(eventAngle);
    particle->getMomentum().RotateZ(eventAngle);
    }
}

