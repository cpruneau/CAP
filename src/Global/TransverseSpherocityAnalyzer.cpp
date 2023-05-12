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

#include "TransverseSpherocityAnalyzer.hpp"
using CAP::TransverseSpherocityAnalyzer;

ClassImp(TransverseSpherocityAnalyzer);

TransverseSpherocityAnalyzer::TransverseSpherocityAnalyzer(const String & _name,
                                                           const Configuration & _configuration,
                                                           vector<EventFilter*> & _eventFilters,
                                                           vector<ParticleFilter*> & _particleFilters)
:
EventTask(_name,_configuration,_eventFilters,_particleFilters),
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
  addParameter("HistogramsCreate",    true);
  addParameter("HistogramsExport",    true);
  addParameter("EventsAnalyze",       true);
  addParameter("EventsUseStream0",    true);
  addParameter("EventsUseStream1",    false);
  addParameter("SetEvent",            true);
  addParameter("FillCorrelationHistos",false);
  addParameter("nSteps", 1000);
  addParameter("FillS0", true);
  addParameter("FillS1", false);
  addParameter("FillS1VsS0", false);
  addParameter("nBins_spherocity", 100);
  addParameter("Min_spherocity",   0.0);
  addParameter("Max_spherocity",   1.0);
}

void TransverseSpherocityAnalyzer::configure()
{
  EventTask::configure();
  setEvent   = getValueBool("SetEvent");
  fillS0     = getValueBool("FillS0");
  fillS1     = getValueBool("FillS1");
  fillS1VsS0 = getValueBool("FillS1VsS0");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("EventsAnalyze");
    printItem("EventsUseStream0");
    printItem("EventsUseStream1");
    printItem("EventsUseStream2");
    printItem("EventsUseStream3");
    printItem("HistogramsCreate");
    printItem("HistogramsExport");
    printItem("SetEvent");
    printItem("FillCorrelationHistos");
    printItem("nSteps");
    printItem("FillS0");
    printItem("FillS1");
    printItem("FillS1VsS0");
    printItem("nBins_spherocity");
    printItem("Min_spherocity");
    printItem("Max_spherocity");
    cout << endl;
    }
}



void TransverseSpherocityAnalyzer::initialize()
{
  if (reportStart(__FUNCTION__))
;
  EventTask::initialize();

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
  String prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << " S:Creating HistogramGroup for....: " << prefixName  << endl;
    cout << " S:nEventFilters..............: " << nEventFilters << endl;
    cout << " S:nParticleFilters...........: " << nParticleFilters << endl;
    }
  histogramManager.addSet("Spherocity");
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TransverseSpherocityHistos * histos = new TransverseSpherocityHistos(this,prefixName+eventFilters[iEventFilter]->getName(),configuration,particleFilters);
    histos->createHistograms();
    histogramManager.addGroupInSet(0,histos);
    }
  if (reportEnd(__FUNCTION__))
;
}

void TransverseSpherocityAnalyzer::importHistograms(TFile & inputFile)
{
  
  if (reportStart(__FUNCTION__))
;
  String prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();;
  unsigned int nParticleFilters = particleFilters.size();
  if (reportDebug(__FUNCTION__))
    {
    cout << "Loading HistogramGroup for " << prefixName  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  histogramManager.addSet("Spherocity");
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String evtFilterName = eventFilters[iEventFilter]->getName();
    TransverseSpherocityHistos * histos = new TransverseSpherocityHistos(this, prefixName+eventFilters[iEventFilter]->getName(),configuration,particleFilters);
    histos->importHistograms(inputFile);
    histogramManager.addGroupInSet(0,histos);
    }
  if (reportEnd(__FUNCTION__))
;
}

void TransverseSpherocityAnalyzer::analyzeEvent()
{
//  
//  if (reportStart(__FUNCTION__))
//;
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
        nx = cos(refPhi); // x component of a unitary vector n
        ny = sin(refPhi); // y component of a unitary vector n
        num0 = 0;
        num1 = 0;
        for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
          {
          Particle & particle = * event.getParticleAt(iParticle);
          if (!particleFilters[iParticleFilter]->accept(particle)) continue;
          LorentzVector & momentum = particle.getMomentum();
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
    TransverseSpherocityHistos * histos = (TransverseSpherocityHistos * ) histogramManager.getGroup(0,iEventFilter);
    histos->fill(s0Filtered,s1Filtered,1.0);
  }
}

void TransverseSpherocityAnalyzer::createDerivedHistograms()
{

}

void TransverseSpherocityAnalyzer::importDerivedHistograms(TFile & inputFile __attribute__((unused)))
{

}

void TransverseSpherocityAnalyzer::calculateDerivedHistograms()
{

}
