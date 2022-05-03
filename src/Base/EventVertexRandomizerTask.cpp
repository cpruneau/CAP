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
#include "EventVertexRandomizerTask.hpp"

ClassImp(EventVertexRandomizerTask);

EventVertexRandomizerTask::EventVertexRandomizerTask(const TString &          _name,
                                                     const Configuration &    _configuration,
                                                     vector<EventFilter*> &   _eventFilters,
                                                     vector<ParticleFilter*>& _particleFilters,
                                                     MessageLogger::LogLevel  _reportLevel)
  :
Task(_name,_configuration,_eventFilters,_particleFilters,_reportLevel),
rConversion(0), tConversion(0),
xAvg(0), yAvg(0), zAvg(0), tAvg(0),
xRms(0), yRms(0), zRms(0), tRms(0)
{
  appendClassName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

//!
//! vertex position and size supplied in micro-meters.
//! vertex time is nanosecond relative to nominal crossing time
//! convert to fm for internal use: 1  micro-meter = 1E9 fm
//!
void EventVertexRandomizerTask::setDefaultConfiguration()
{
  Configuration & configuration = getConfiguration();
  configuration.setName("EventVertexRandomizerTask Configuration");
  configuration.setParameter("useParticles", true);
  configuration.setParameter("useEventStream0", true);
  configuration.addParameter("rConversion", 1.0E9);
  configuration.addParameter("tConversion", 1.0E9);
  configuration.addParameter("xAvg", 0.0);
  configuration.addParameter("yAvg", 0.0);
  configuration.addParameter("zAvg", 0.0);
  configuration.addParameter("tAvg", 0.0);
  configuration.addParameter("xRms", 100.0);
  configuration.addParameter("yRms", 100.0);
  configuration.addParameter("zRms", 5.0);
  configuration.addParameter("tRms", 10.0);
}

void EventVertexRandomizerTask::initialize()
{
  Configuration & configuration = getConfiguration();
  rConversion = configuration.getValueDouble("rConversion");
  tConversion = configuration.getValueDouble("tConversion");
  xAvg = configuration.getValueDouble("xAvg");
  yAvg = configuration.getValueDouble("yAvg");
  zAvg = configuration.getValueDouble("zAvg");
  tAvg = configuration.getValueDouble("tAvg");
  xRms = configuration.getValueDouble("xRms");
  yRms = configuration.getValueDouble("yRms");
  zRms = configuration.getValueDouble("zRms");
  tRms = configuration.getValueDouble("tRms");
}

void EventVertexRandomizerTask::execute()
{
  double eventX = taskRandomGenerator->Gaus(rConversion*xAvg, rConversion*xRms);
  double eventY = taskRandomGenerator->Gaus(rConversion*yAvg, rConversion*yRms);
  double eventZ = taskRandomGenerator->Gaus(rConversion*zAvg, rConversion*zRms);
  double eventT = taskRandomGenerator->Gaus(tConversion*tAvg, tConversion*tRms);
  Event * event = eventStreams[0];
  unsigned int nParticles = event->getNParticles();
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    Particle * particle = event->getParticleAt(iParticle);
    particle->shift(eventX,eventY,eventZ,eventT);
    }
  incrementEventProcessed();
}

