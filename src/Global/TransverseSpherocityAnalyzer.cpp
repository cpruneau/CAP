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
#include <TLorentzVector.h>
#include "TransverseSpherocityAnalyzer.hpp"

ClassImp(TransverseSpherocityAnalyzer);

TransverseSpherocityAnalyzer::TransverseSpherocityAnalyzer(const TString & _name,
                                                           Configuration & _configuration,
                                                           vector<EventFilter*> & _eventFilters,
                                                           vector<ParticleFilter*> & _particleFilters)
:
Task(_name,_configuration,_eventFilters,_particleFilters),
setEvent(true),
fillS0(true),
fillS1(false),
nSteps(360),
stepSize(TMath::TwoPi()/360.0)
{
  appendClassName("TransverseSpherocityAnalyzer");
}

//!
//!
void TransverseSpherocityAnalyzer::setDefaultConfiguration()
{
  setParameter("UseParticles",      true);
  setParameter("CreateHistograms",  true);
  setParameter("SaveHistograms",    true);
  setParameter("UseEventStream0",   true);
  setParameter("UseEventStream1",   false);
  addParameter("SetEvent",         true);
  addParameter("FillCorrelationHistos",false);
  addParameter("nSteps", 1000);
  addParameter("FillS0", true);
  addParameter("FillS1", false);
  addParameter("FillS1VsS0", false);
  addParameter("nBins_spherocity", 100);
  addParameter("Min_spherocity",   0.0);
  addParameter("Max_spherocity",   1.0);
}

void TransverseSpherocityAnalyzer::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  setEvent   = getValueBool("SetEvent");
  fillS0     = getValueBool("FillS0");
  fillS1     = getValueBool("FillS1");
  fillS1VsS0 = getValueBool("FillS1VsS0");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " S:setEvent............: " << setEvent   << endl;
    cout << " S:fillS0..............: " << fillS0     << endl;
    cout << " S:fillS1..............: " << fillS1     << endl;
    cout << " S:fillS1VsS0..........: " << fillS1VsS0 << endl;
    cout << " S:#event streams......: " << getNEventStreams() << endl;
    }
}

void TransverseSpherocityAnalyzer::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << " S:Creating Histograms for....: " << prefixName  << endl;
    cout << " S:nEventFilters..............: " << nEventFilters << endl;
    cout << " S:nParticleFilters...........: " << nParticleFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TransverseSpherocityHistos * histos = new TransverseSpherocityHistos(this,prefixName+eventFilters[iEventFilter]->getName(),configuration,particleFilters);
    histos->createHistograms();
    histograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void TransverseSpherocityAnalyzer::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();;
  unsigned int nParticleFilters = particleFilters.size();
  if (reportDebug(__FUNCTION__))
    {
    cout << "Loading Histograms for " << prefixName  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    TransverseSpherocityHistos * histos = new TransverseSpherocityHistos(this, prefixName+eventFilters[iEventFilter]->getName(),configuration,particleFilters);
    histos->loadHistograms(inputFile);
    histograms.push_back(histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void TransverseSpherocityAnalyzer::execute()
{
//  
//  if (reportStart(__FUNCTION__))
//    ;
  incrementTaskExecuted();
  static double factor = TMath::Pi()*TMath::Pi()/4.0;
  incrementTaskExecuted();
  Event & event = * getEventStream(0);
   // count eventStreams used to fill histograms and for scaling at the end..
  // resetParticleCounters();
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  unsigned int nParticles       = event.getNParticles();
  
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(event)) continue;
    incrementNEventsAccepted(iEventFilter);
    if (nParticles<1) continue;
    vector<double> s0Filtered(nParticleFilters,0.0);
    vector<double> s1Filtered(nParticleFilters,0.0);
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      double  s0 = 1.0E10;
      double  s1 = 1.0E10;
      double  num0, num1, nx, ny, px, py, pt;
      double  denom0 = 0;
      double  denom1 = 0;
      double  refPhi  = 0.0;
      for(int k = 0; k < nSteps; k++)
        {
        //double  phiparam = ((TMath::Pi()) * i * stepSize) / 180;
        nx = TMath::Cos(refPhi); // x component of a unitary vector n
        ny = TMath::Sin(refPhi); // y component of a unitary vector n
        num0 = 0;
        num1 = 0;
        for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
          {
          Particle & particle = * event.getParticleAt(iParticle);
          if (!particleFilters[iParticleFilter]->accept(particle)) continue;
          TLorentzVector & momentum = particle.getMomentum();
          pt = momentum.Pt();
          px = momentum.Px();
          py = momentum.Py();
          if (fillS0)
            {
            num0 += TMath::Abs(ny*px - nx*py);
            if(k==0) denom0 += pt;
            }
          if (fillS1)
            {
            double  ax = px/pt;
            double  ay = py/pt;
            num1 += TMath::Abs(ny*ax - nx*ay);
            if(k==0) denom1 += 1;
            }
          }
        if (fillS0)
          {
          double ratio = num0/denom0;
          double r2 = ratio*ratio;
          if (r2 < s0) s0 = r2;
          }
        if (fillS1)
          {
          double ratio = num1/denom1;
          double r2 = ratio*ratio;
          if (r2 < s1) s1 = r2;
          }
        refPhi += stepSize;
        }
        if (fillS0) s0Filtered[iParticleFilter] = s0*factor;
        if (fillS1) s1Filtered[iParticleFilter] = s1*factor;
        }
    if (setEvent && iEventFilter==0)
        {
        EventProperties * ep = event.getEventProperties();
        ep->fillSpherocity(s0Filtered,s1Filtered);
        }
    TransverseSpherocityHistos * histos = (TransverseSpherocityHistos * ) histograms[iEventFilter];
    histos->fill(s0Filtered,s1Filtered,1.0);
  }
}

void TransverseSpherocityAnalyzer::createDerivedHistograms()
{

}

void TransverseSpherocityAnalyzer::loadDerivedHistograms(TFile * inputFile __attribute__((unused)))
{

}

void TransverseSpherocityAnalyzer::calculateDerivedHistograms()
{

}
