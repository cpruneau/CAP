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
#include "AACollisionGenerator.hpp"
#include "TDatabasePDG.h"
using CAP::AACollisionGenerator;

ClassImp(AACollisionGenerator);

AACollisionGenerator::AACollisionGenerator(const String * _name,
                                           const Configuration & _configuration,
                                           vector<EventFilter*> & _eventFilters,
                                           vector<ParticleFilter*>& _particleFilters)
:
EventTask(_name, _configuration, eventFilters, particleFilters, _reportLevel),
nnCollisionGenerator(NucleonNucleonCollisionGenerator::getDefaultNNCollisionGenerator() )
{
  nnCollisionGenerator = new NucleonNucleonCollisionGenerator("NN",_configuration,_eventFilters,_particleFilters);
  appendClassName("AACollisionGenerator");
}

void AACollisionGenerator::setDefaultConfiguration()
{
  addParameter("UseParticles",    true);
  addParameter("EventsUseStream0", true);
}

void AACollisionGenerator::initialize()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  nnCollisionGenerator->initialize();
}

void AACollisionGenerator::createEvent()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  unsigned int nParticles = eventStreams[0]->getNParticles()
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
  {
  Particle * particle = event->getParticleAt(iParticle)
  if (particle->isInteraction() && particle->isLive() )
    {
    nnCollisionGenerator->generate(particle);
    particle->setLive(false); // the interaction has been generated
    }
  }
}

void AACollisionGenerator::finalize()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Task::finalize();
  nnCollisionGenerator->printStatistics();
}

void AACollisionGenerator::setNucleonNucleonCollisionGenerator(NucleonNucleonCollisionGenerator  * _nnCollisionGenerator)
{
  if (!_nnCollisionGenerator)
    {
    if (reportFatal()) cout << "No nnCollisionGenerator available (nnCollisionGenerator==nullptr)" <<endl;
    postTaskFatal();
    return;
    }
  nnCollisionGenerator = _nnCollisionGenerator;
}
