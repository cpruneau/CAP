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
#include "MeasurementPerformanceSimulator.hpp"

ClassImp(MeasurementPerformanceSimulator);

MeasurementPerformanceSimulator::MeasurementPerformanceSimulator(const TString & _name,
                                                                 Configuration & _configuration,
                                                                 vector<EventFilter*> & _eventFilters,
                                                                 vector<ParticleFilter*>& _particleFilters)
:
Task(_name,_configuration,_eventFilters,_particleFilters),
allEventsUseSameFilters(true)
{
  appendClassName("MeasurementPerformanceSimulator");
}

void MeasurementPerformanceSimulator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  setParameter("UseParticles",    true);
  setParameter("UseEventStream0", true);
  setParameter("UseEventStream1", true);
  setParameter("LoadHistograms",  false);
  addParameter("allEventsUseSameFilters", allEventsUseSameFilters);
  addParameter("useSameSetForAll",  true);
  addParameter("resolutionOption",  0);
  addParameter("efficiencyOption",  0);
  
  for (int k=0; k<10; k++)
    {
    TString baseName = "Filter";
    baseName += k;
    addParameter(baseName+"_PtBiasAinv",0.0);
    addParameter(baseName+"_PtBiasA0",0.0);
    addParameter(baseName+"_PtBiasA1",0.0);
    addParameter(baseName+"_PtBiasA2",0.0);
    addParameter(baseName+"_PtRmsAinv",0.0);
    addParameter(baseName+"_PtRmsA0",0.0);
    addParameter(baseName+"_PtRmsA1",0.0);
    addParameter(baseName+"_PtRmsA2",0.0);

    addParameter(baseName+"_EtaBiasAinv",0.0);
    addParameter(baseName+"_EtaBiasA0",0.0);
    addParameter(baseName+"_EtaBiasA1",0.0);
    addParameter(baseName+"_EtaBiasA2",0.0);
    addParameter(baseName+"_EtaRmsAinv",0.0);
    addParameter(baseName+"_EtaRmsA0",0.0);
    addParameter(baseName+"_EtaRmsA1",0.0);
    addParameter(baseName+"_EtaRmsA2",0.0);

    addParameter(baseName+"_PhiBiasAinv",0.0);
    addParameter(baseName+"_PhiBiasA0",0.0);
    addParameter(baseName+"_PhiBiasA1",0.0);
    addParameter(baseName+"_PhiBiasA2",0.0);
    addParameter(baseName+"_PhiRmsAinv",0.0);
    addParameter(baseName+"_PhiRmsA0",0.0);
    addParameter(baseName+"_PhiRmsA1",0.0);
    addParameter(baseName+"_PhiRmsA2",0.0);

    addParameter(baseName+"_EffPeakAmp",0.0);
    addParameter(baseName+"_EffPeakPt",0.0);
    addParameter(baseName+"_EffPeakRms",0.0);
    addParameter(baseName+"_EffA1",0.0);
    addParameter(baseName+"_EffA2",0.0);
    }
}

void MeasurementPerformanceSimulator::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  Configuration & configuration = getConfiguration();
  allEventsUseSameFilters = getValueBool("allEventsUseSameFilters");
  bool useSameSetForAll = getValueBool("useSameSetForAll");
  int  resolutionOption = configuration.getValueInt(getName(),"resolutionOption");
  int  efficiencyOption = configuration.getValueInt(getName(),"efficiencyOption");

  nEventFilters    = eventFilters.size();
  nParticleFilters = particleFilters.size();
  if (!allEventsUseSameFilters && nParticleFilters%nEventFilters)
    {
    if (reportError(__FUNCTION__))
      {
      cout << endl;
      cout << "Attempting to initiate task with"  << endl;
      cout << "        allEventsUseSameFilters: " << allEventsUseSameFilters << endl;
      cout << "                  nEventFilters: " << nEventFilters << endl;
      cout << "               nParticleFilters: " << nParticleFilters << endl;
      cout << " nParticleFilters%nEventFilters: " << nParticleFilters%nEventFilters << endl;
      }
    postTaskError();
    return;
    }
  else
    {
    
    if (reportInfo(__FUNCTION__))
      {
      cout << endl;
      cout << "Initiating task with"  << endl;
      cout << "nEventFilters................: " << nEventFilters << endl;
      cout << "nParticleFilters.............: " << nParticleFilters << endl;
      cout << "allEventsUseSameFilters......: " << allEventsUseSameFilters << endl;
      cout << "useSameSetForAll.............: " << useSameSetForAll << endl;
      cout << "resolutionOption.............: " << resolutionOption << endl;
      cout << "efficiencyOption.............: " << efficiencyOption << endl;
      }
    for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++)
      {
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++)
        {
        TString name = "Filter_";
        name += iEventFilter;
        name += "_";
        name += iParticleFilter;
        ParticlePerformanceSimulator * sim = new ParticlePerformanceSimulator(this,iParticleFilter,name, getConfiguration());
        sim->initialize();
        baseSingleHistograms.push_back(sim);
        }
      }
    }
}


void MeasurementPerformanceSimulator::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); prefixName += "_";

  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = * eventFilters[iEventFilter]->getName();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString partFilterName = * particleFilters[iParticleFilter]->getName();
      TString histoName  = prefixName;
      histoName += evtFilterName;
      histoName += "_";
      histoName += partFilterName;
      ParticlePerformanceSimulator * histos = new ParticlePerformanceSimulator(this,iParticleFilter, histoName,configuration);
      histos->loadHistograms(inputFile);
      baseSingleHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! If multiple event filter are present, i.,e., nEventFilters>1, each filter should have its set of particle efficiency filters
//! so it is possible to accounts with efficiencies that depend, say, on global observables. This class does NOT use
//! 2D arrays of ParticleFilters and ParticlePerformanceSimulators, instead, if nEventFilters>1, it will be assume that the number
//! of such particle filters is nEventFilters times the actual number. If you wish to use the same particle filters/smearers,
//! set the configuration parameter "allEventsUseSameFilters" to true.
//!
void MeasurementPerformanceSimulator::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  Event & genEvent  = *eventStreams[0];
  Event & recoEvent = *eventStreams[1];
  unsigned long nPartGen   = genEvent.getParticleCount();
  unsigned long nPartReco  = recoEvent.getParticleCount();
  //if (reportInfo(__FUNCTION__)) cout << " BEFORE PERFORM SIM: GEN EVENT Particle Count: " << nPartGen << endl;
  //if (reportInfo(__FUNCTION__)) cout << " BEFORE PERFORM SIM: RECO EVENT Particle Count: " << nPartReco << endl;
  if (nPartReco>nPartGen)
    {
    if (reportInfo(__FUNCTION__))cout << " nParticles>nPartGen   ABORT NOW!!!!!" << endl;
    exit(1);
    }

  unsigned int nParticles = genEvent.getNParticles();
  unsigned int firstPartFilter;
  unsigned int lastPartFilter;
  TLorentzVector recoMomentum;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(genEvent)) continue;
    incrementNEventsAccepted(iEventFilter);
    if (nParticles<1) return;

    if (allEventsUseSameFilters)
      {
      firstPartFilter = 0;
      lastPartFilter  = nParticleFilters;
      }
    else
      {
      firstPartFilter = iEventFilter * nParticleFilters;
      lastPartFilter  = firstPartFilter + nParticleFilters;
      }
    for (unsigned int iParticleFilter=firstPartFilter; iParticleFilter<lastPartFilter; iParticleFilter++ )
      {
      ParticleFilter  * particleFilter  = particleFilters[iParticleFilter];
      ParticlePerformanceSimulator * simulator = (ParticlePerformanceSimulator *) baseSingleHistograms[iParticleFilter];
      for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
        {
        Particle * genParticle = genEvent.getParticleAt(iParticle);
        if (particleFilter->accept(*genParticle))
          {
          TLorentzVector & genMomentum = genParticle->getMomentum();
          if (!simulator->accept(genMomentum)) continue;
          ParticleType * type = genParticle->getTypePtr();
          TLorentzVector & genPosition = genParticle->getPosition();
          simulator->smearMomentum(genMomentum,recoMomentum);
          Particle * recoParticle = particleFactory->getNextObject();
          // we dont smear the position for now..
          recoParticle->set(type,recoMomentum,genPosition,true);
          recoParticle->setTruth(genParticle);
          recoEvent.add(recoParticle);
          } // particle accepted by filter
        } //particle loop
      } // particle filter loop
    } // event filter loop

  nPartGen   = genEvent.getParticleCount();
  nPartReco  = recoEvent.getParticleCount();
  //if (reportInfo(__FUNCTION__)) cout << " AFTER PERFORM SIM:   GEN EVENT Particle Count: " << nPartGen << endl;
  //if (reportInfo(__FUNCTION__)) cout << " AFTER PERFORM SIM:  RECO EVENT Particle Count: " << nPartReco << endl;
  if (nPartReco>nPartGen)
    {
    if (reportInfo(__FUNCTION__))cout << " nParticles>nPartGen   ABORT NOW!!!!!" << endl;
    exit(1);
    }

}
