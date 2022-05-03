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

#include "DetectorFastSimulatorTask.hpp"

ClassImp(DetectorFastSimulatorTask);

DetectorFastSimulatorTask::DetectorFastSimulatorTask(const TString &          _name,
                                                     Configuration &          _configuration,
                                                     vector<EventFilter*>     _eventFilters,
                                                     vector<ParticleFilter*>  _particleFilters,
                                                     vector<ParticleSmearer*> _particleSmearers,
                                                     LogLevel                 _selectedLevel)
:
Task(_name,_configuration,_eventFilters, _particleFilters, _particleSmearers, _selectedLevel)
{
  appendClassName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void DetectorFastSimulatorTask::setDefaultConfiguration()
{
  Configuration & configuration = getConfiguration();
  configuration.setName("DetectorFastSimulatorTask Configuration");
  configuration.setParameter("useParticles",    true);
  configuration.setParameter("useEventStream0", true);
  configuration.setParameter("useEventStream1", true);
  if (reportDebug("DetectorFastSimulatorTask",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}


void DetectorFastSimulatorTask::execute()
{
  incrementEventProcessed();
  ParticleFilter & particleFilterPrimary   = * particleFilters[0];
  ParticleFilter & particleFilterSecondary = * particleFilters[1];
  Event & event    = *eventStreams[0]; // source -- unsmeared
  Event & eventDet = *eventStreams[1]; // target -- smeared.

  unsigned int nParticles = event.getNParticles();
  for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
    {
    Particle & particle = * event.getParticleAt(iParticle);
    if (particle.isLive())
      {
      if (particle.isPrimary())
        {
        if (particleFilterPrimary.accept(particle))
          {
          Particle & particleDet = *particleFactory->getNextObject();
          particleDet = particle;
          particleDet.setTruth(&particle);
          particleSmearers[0]->smear(particle.getMomentum(),particleDet.getMomentum());
          eventDet.add(&particleDet);
          }
        else if (particleFilterSecondary.accept(particle))
          {
          Particle & particleDet = *particleFactory->getNextObject();
          particleDet = particle;
          particleDet.setTruth(&particle);
          particleSmearers[1]->smear(particle.getMomentum(),particleDet.getMomentum());
          eventDet.add(&particleDet);
          }
        }
      }
    }
}

