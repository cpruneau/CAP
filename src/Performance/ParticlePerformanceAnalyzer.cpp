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
#include "ParticlePerformanceAnalyzer.hpp"

ClassImp(ParticlePerformanceAnalyzer);

ParticlePerformanceAnalyzer::ParticlePerformanceAnalyzer(const TString & _name,
                                                         Configuration & _configuration,
                                                         vector<EventFilter*> & _eventFilters,
                                                         vector<ParticleFilter*>& _particleFilters)
:
Task(_name,_configuration,_eventFilters,_particleFilters),
fillEta(true),
fillY(false)
{
  appendClassName("ParticlePerformanceAnalyzer");
}

void ParticlePerformanceAnalyzer::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  addParameter("UseParticles",     true);
  addParameter("UseEventStream0",  true);
  addParameter("UseEventStream1",  true);
  addParameter("CreateHistograms", true);
  addParameter("SaveHistograms",   true);
  addParameter("nBins_pt",100);
  addParameter("Min_pt", 0.0);
  addParameter("Max_pt", 5.0);
  addParameter("nBins_dpt", 100);
  addParameter("Min_dpt",  -1.0);
  addParameter("Max_dpt",   1.0);
  addParameter("nBins_phi", 100);
  addParameter("Min_phi",  -1.0);
  addParameter("Max_phi",   1.0);
  addParameter("nBins_dphi", 100);
  addParameter("Min_dphi",  -1.0);
  addParameter("Max_dphi",   1.0);
  addParameter("nBins_eta", 100);
  addParameter("Min_eta",  -1.0);
  addParameter("Max_eta",   1.0);
  addParameter("nBins_deta", 100);
  addParameter("Min_deta",  -1.0);
  addParameter("Max_deta",   1.0);
  addParameter("nBins_y", 100);
  addParameter("Min_y",  -1.0);
  addParameter("Max_y",  -1.0);
  addParameter("nBins_dy", 100);
  addParameter("Min_dy",  -1.0);
  addParameter("Max_dy",   1.0);
  addParameter("FillEta",  fillEta);
  addParameter("FillY",    fillY);
}

void ParticlePerformanceAnalyzer::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); //prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  
  fillEta = getValueBool("FillEta");
  fillY   = getValueBool("FillY");
  
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Creating Histogram(s) for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString partFilterName = particleFilters[iParticleFilter]->getName();
      TString histoName  = prefixName;
      histoName += "_";
      histoName += evtFilterName;
      histoName += "_";
      histoName += partFilterName;
      ParticlePerformanceHistos * histos = new ParticlePerformanceHistos(this,histoName,configuration);
      histos->createHistograms();
      baseSingleHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void ParticlePerformanceAnalyzer::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  fillEta = getValueBool("FillEta");
  fillY   = getValueBool("FillY");
  
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "Loading Histogram(s) for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
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
      ParticlePerformanceHistos * histos = new ParticlePerformanceHistos(this,histoName,configuration);
      histos->loadHistograms(inputFile);
      baseSingleHistograms.push_back(histos);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePerformanceAnalyzer::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();

  Event & recoEvent = *eventStreams[1];
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  unsigned long nParticles      = recoEvent.getParticleCount();

  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    //cout<< "ParticlePerformanceAnalyzer::execute() -- 4 --" << endl;
    if (!eventFilters[iEventFilter]->accept(recoEvent)) continue;
    //cout<< "ParticlePerformanceAnalyzer::execute() -- 5 --" << endl;
    incrementNEventsAccepted(iEventFilter);
    ParticlePerformanceHistos * histos = (ParticlePerformanceHistos *) baseSingleHistograms[0];
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      //cout<< "ParticlePerformanceAnalyzer::execute() -- 6 --" << endl;
      Particle & recoParticle = * recoEvent.getParticleAt(iParticle);
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        //cout<< "ParticlePerformanceAnalyzer::execute() -- 7 --" << endl;
        if (particleFilters[iParticleFilter]->accept(recoParticle))
          {
          //cout<< "ParticlePerformanceAnalyzer::execute() -- 8 --" << endl;
          histos->fill(recoParticle,1.0);
          } // particle accepted by filter
        } // filter loop
      } //particle loop
    } // event filter loop
}
