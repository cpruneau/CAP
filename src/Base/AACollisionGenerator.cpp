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

ClassImp(AACollisionGenerator);

AACollisionGenerator::AACollisionGenerator(const TString &          _name,
                                           const Configuration  &   _configuration,
                                           vector<EventFilter*> &   _eventFilters,
                                           vector<ParticleFilter*>& _particleFilters,
                                           MessageLogger::LogLevel  _reportLevel)
:
Task(_name, _configuration, eventFilters, particleFilters, _reportLevel),
nnCollisionGenerator(NucleonNucleonCollisionGenerator::getDefaultNNCollisionGenerator() )
{
  nnCollisionGenerator = new NucleonNucleonCollisionGenerator("NN",_configuration,_eventFilters,_particleFilters,_reportLevel);
  appendClassName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void AACollisionGenerator::setDefaultConfiguration()
{
  Configuration * configuration = getConfiguration();
  configuration.setName("AACollisionGenerator Configuration");
  configuration.setParameter("useParticles",    true);
  configuration.setParameter("useEventStream0", true);
}

void AACollisionGenerator::initialize()
{
  Task::initialize();
  nnCollisionGenerator->initialize();
}

void AACollisionGenerator::execute()
{
  unsigned int nParticles = eventStreams[0]->getNParticles()
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
  {
  Particle * particle = event->getParticleAt(iParticle)
  if (particle->isInteraction() && particle->isLive() )
    {
    nnCollisionGenerator->generate(particle);
    particle->setLive(false); // the interaction has been generated
    }
  incrementEventProcessed();
  incrementEventAccepted();
  }
}

void AACollisionGenerator::finalize()
{
  Task::finalize();
  nnCollisionGenerator->printStatistics();
}

void AACollisionGenerator::setNucleonNucleonCollisionGenerator(NucleonNucleonCollisionGenerator  * _nnCollisionGenerator)
{
  if (!_nnCollisionGenerator)
    {
    if (reportFatal("AACollisionGenerator",getName(),"initialize()"))
      {
      cout << "No nnCollisionGenerator available (nnCollisionGenerator==nullptr)" <<endl;
      }
    postTaskFatal();
    return;
    }
  nnCollisionGenerator = _nnCollisionGenerator;
}
