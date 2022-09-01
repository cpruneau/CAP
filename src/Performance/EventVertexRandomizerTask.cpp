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

EventVertexRandomizerTask::EventVertexRandomizerTask(const TString & _name,
                                                     Configuration & _configuration,
                                                     vector<EventFilter*> & _eventFilters,
                                                     vector<ParticleFilter*>& _particleFilters)
  :
Task(_name,_configuration,_eventFilters,_particleFilters),
rConversion(0), tConversion(0),
xAvg(0), yAvg(0), zAvg(0), tAvg(0),
xRms(0), yRms(0), zRms(0), tRms(0)
{
  appendClassName("EventVertexRandomizerTask");
}

//!
//! vertex position and size supplied in micro-meters.
//! vertex time is nanosecond relative to nominal crossing time
//! convert to fm for internal use: 1  micro-meter = 1E9 fm
//!
void EventVertexRandomizerTask::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  setParameter("UseParticles", true);
  setParameter("UseEventStream0", true);
  addParameter("rConversion", 1.0E9);
  addParameter("tConversion", 1.0E9);
  addParameter("xAvg", 0.0);
  addParameter("yAvg", 0.0);
  addParameter("zAvg", 0.0);
  addParameter("tAvg", 0.0);
  addParameter("xRms", 100.0);
  addParameter("yRms", 100.0);
  addParameter("zRms", 5.0);
  addParameter("tRms", 10.0);
}

void EventVertexRandomizerTask::initialize()
{
  rConversion = configuration.getValueDouble(getName(),"rConversion");
  tConversion = configuration.getValueDouble(getName(),"tConversion");
  xAvg = configuration.getValueDouble(getName(),"xAvg");
  yAvg = configuration.getValueDouble(getName(),"yAvg");
  zAvg = configuration.getValueDouble(getName(),"zAvg");
  tAvg = configuration.getValueDouble(getName(),"tAvg");
  xRms = configuration.getValueDouble(getName(),"xRms");
  yRms = configuration.getValueDouble(getName(),"yRms");
  zRms = configuration.getValueDouble(getName(),"zRms");
  tRms = configuration.getValueDouble(getName(),"tRms");
}

void EventVertexRandomizerTask::execute()
{
  incrementTaskExecuted();
  double eventX = gRandom->Gaus(rConversion*xAvg, rConversion*xRms);
  double eventY = gRandom->Gaus(rConversion*yAvg, rConversion*yRms);
  double eventZ = gRandom->Gaus(rConversion*zAvg, rConversion*zRms);
  double eventT = gRandom->Gaus(tConversion*tAvg, tConversion*tRms);
  Event * event = eventStreams[0];
  unsigned int nParticles = event->getNParticles();
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    Particle * particle = event->getParticleAt(iParticle);
    particle->shift(eventX,eventY,eventZ,eventT);
    }
}

