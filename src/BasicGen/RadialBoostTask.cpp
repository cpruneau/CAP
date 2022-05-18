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
#include "RadialBoostTask.hpp"
#include "CollisionGeometryGradientHistograms.hpp"

ClassImp(RadialBoostTask);

RadialBoostTask::RadialBoostTask(const TString &          _name,
                                 const Configuration &    _configuration,
                                 vector<EventFilter*>&    _eventFilters,
                                 vector<ParticleFilter*>& _particleFilters,
                                 LogLevel                 _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel),
param_b(0),
param_a(0),
betaMaximum(0)
{
  appendClassName("RadialBoostTask");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
 }

void RadialBoostTask::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  configuration.setName("RadialBoostTask Configuration");
  configuration.setParameter("useEventStream0",  true);
  configuration.setParameter("useParticles",     true);
  configuration.setParameter("loadHistograms",   true);
  configuration.setParameter("createHistograms", true);
  configuration.setParameter("saveHistograms",   true);
  
  configuration.addParameter("param_a", 0.9);
  configuration.addParameter("param_b", 1.0);
  configuration.addParameter("betaMaximum", 0.999);
  configuration.addParameter("min_phi", 0.0);
  configuration.addParameter("max_phi", TMath::TwoPi());
  configuration.addParameter("nBins_r", 100);
  configuration.addParameter("min_r",   0.0);
  configuration.addParameter("max_r",   10.0);
  configuration.addParameter("nBins_beta", 100);
  configuration.addParameter("min_beta",   0.0);
  configuration.addParameter("max_beta",   1.0);
  if (reportEnd(__FUNCTION__))
    ;
}

void RadialBoostTask::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  param_a     = configuration.getValueDouble("param_a");
  param_b     = configuration.getValueDouble("param_b"); // exponent of order 1
  betaMaximum = configuration.getValueDouble("betaMaximum");;
  RadialBoostHistos * histos = new RadialBoostHistos(getName(),configuration,getReportLevel());
  histos->createHistograms();
  histograms.push_back(histos);
  if (reportEnd(__FUNCTION__))
    ;
}

void RadialBoostTask::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  RadialBoostHistos * histos = new RadialBoostHistos(getName(),configuration,getReportLevel());
  histos->loadHistograms(inputFile);
  histograms.push_back(histos);
  if (reportEnd(__FUNCTION__))
    ;
}

void RadialBoostTask::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  double beta, betax, betay;
  double rx, ry, r, gx,gy, phi;
  unsigned int nEventFilters    = eventFilters.size();
 // unsigned int nParticleFilters = particleFilters.size();
  Event & event = * eventStreams[0];

  if (reportDebug(__FUNCTION__)) cout << "       nEventFilters: " << nEventFilters << endl;

  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (reportDebug(__FUNCTION__)) cout << "       iEventFilter: " << iEventFilter << endl;

    if (!eventFilters[iEventFilter]->accept(event)) continue;
    if (reportDebug(__FUNCTION__)) cout << "       iEventFilter: " << iEventFilter << " accepted event" << endl;

    incrementNEventsAccepted(iEventFilter);
    CollisionGeometryGradientHistograms * cggh = (CollisionGeometryGradientHistograms *) inputHistograms[iEventFilter];

    if (reportDebug(__FUNCTION__)) cout << "       cggh: " << cggh <<  endl;


    unsigned int nParticles = event.getNParticles();
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      if (reportDebug(__FUNCTION__)) cout << "       iParticle: " << iParticle <<  endl;

      Particle & particle = * event.getParticleAt(iParticle);
      if (particle.isLive() || particle.isInteraction() )
        {
        if (reportDebug(__FUNCTION__)) cout << "       iParticle: " << iParticle <<  "  loop" << endl;

        TLorentzVector & position = particle.getPosition();
        rx  = position.X(); // units are fm
        ry  = position.Y();
        if (reportDebug(__FUNCTION__))
          cout << "       iParticle: " << iParticle <<  "  loop2" << endl
          << "              rx: " << rx << endl
          << "              ry: " << ry << endl;
        cggh->getRadiusAndGradient(rx,ry, r,gx,gy);
        if (reportDebug(__FUNCTION__))
          cout << "       iParticle: " << iParticle <<  "  loop3" << endl;
//        phi = 0.0;
        phi = TMath::ATan2(gy,gx);
        if (phi<0) phi += TMath::TwoPi();
        beta = param_a * TMath::Power(r, param_b);
        if (beta > betaMaximum) beta = betaMaximum;
        //cout << " gx:" << gx << "  gy:" << gy << "  phi:" << phi*180.0/3.1415927 << endl;
        double g = sqrt(gx*gx+gy*gy);
        betax = beta * gx/g;
        betay = beta * gy/g;
        RadialBoostHistos * histos = (RadialBoostHistos *) histograms[0];
        histos->fill(rx,ry,r,phi,beta,1.0);
        particle.boost(betax,betay,0.0);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}
