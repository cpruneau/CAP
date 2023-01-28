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
#include "ParticlePairAnalyzer.hpp"
#include "ParticleSingleHistos.hpp"
#include "ParticlePairHistos.hpp"
#include "ParticlePairDerivedHistos.hpp"
using CAP::ParticlePairAnalyzer;

ClassImp(ParticlePairAnalyzer);

ParticlePairAnalyzer::ParticlePairAnalyzer(const String & _name,
                                           Configuration & _configuration,
                                           vector<EventFilter*> & _eventFilters,
                                           vector<ParticleFilter*> & _particleFilters)
:
Task(_name, _configuration, _eventFilters, _particleFilters),
fillEta(true),
fillY(false),
fillP2(false)
{
  appendClassName("ParticlePairAnalyzer");
  for (unsigned int k=0; k<particleFilters.size(); k++)
    {
    vector<ParticleDigit*> list;
    filteredParticles.push_back(list);
    }
}

void ParticlePairAnalyzer::setDefaultConfiguration()
{
  //Task::setDefaultConfiguration();
  setParameter("UseParticles",      true);
  setParameter("CreateHistograms",  true);
  setParameter("SaveHistograms",    true);
  setParameter("UseEventStream0",   true);
  setParameter("UseEventStream1",   false);
  addParameter("FillEta",           fillEta);
  addParameter("FillY",             fillY);
  addParameter("FillP2",            fillP2);
  addParameter("nBins_n1",          100);
  addParameter("Min_n1",            0.0);
  addParameter("Max_n1",            100.0);
  addParameter("nBins_eTot",        100);
  addParameter("Min_eTot",          0.0);
  addParameter("Max_eTot",          100.0);
  addParameter("nBins_pt",           18);
  addParameter("Min_pt",           0.20);
  addParameter("Max_pt",           2.00);
  addParameter("nBins_phi",          72);
  addParameter("Min_phi",           0.0);
  addParameter("Max_phi",         TMath::TwoPi());
  addParameter("nBins_eta",           20);
  addParameter("Min_eta",           -1.0);
  addParameter("Max_eta",            1.0);
  addParameter("nBins_y",             20);
  addParameter("Min_y",             -1.0);
  addParameter("Max_y",              1.0);
  addParameter("nBins_phiEta",            720);
  addParameter("nBins_phiEtaPt",          7200);
  addParameter("nBins_phiY",              720);
  addParameter("nBins_phiYPt",            7200);
  addParameter("nBins_n2",                  100);
  addParameter("Min_n2",                    0.0);
  addParameter("Max_n2",                 1000.0);
  addParameter("nBins_Dphi",                 36);
  addParameter("Min_Dphi",                  0.0);
  addParameter("Max_Dphi",TMath::TwoPi());
  addParameter("Width_Dphi",TMath::TwoPi());
  addParameter("nBins_Dphi_shft",    36);
  addParameter("Min_Dphi_shft",     0.0);
  addParameter("Max_Dphi_shft",     0.0);
  addParameter("nBins_Deta",         39);
  addParameter("Min_Deta",         -2.0);
  addParameter("Max_Deta",          2.0);
  addParameter("Width_Deta",   4.0/39.0);
  addParameter("nBins_Dy",           39);
  addParameter("Min_Dy",           -2.0);
  addParameter("Max_Dy",            2.0);
  addParameter("Width_Dy",     4.0/39.0);
  addParameter("nBins_DeltaP",   10);
  addParameter("Min_DeltaP",   -4.0);
  addParameter("Max_DeltaP",    4.0);
}

void ParticlePairAnalyzer::createHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  Severity debugLevel = getSeverityLevel();
  String bn  = getName();
  fillEta = getValueBool("FillEta");
  fillY   = getValueBool("FillY");
  fillP2  = getValueBool("FillP2");
  
  HistogramGroup * histos;
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Creating HistogramGroup...........: " << bn << endl;
    cout << "nEventFilters.................: " << nEventFilters << endl;
    cout << "nParticleFilters..............: " << nParticleFilters << endl;
    cout << "fillEta.......................: " << fillEta  << endl;
    cout << "fillY.........................: " << fillY    << endl;
    cout << "fillP2........................: " << fillP2   << endl;
    cout << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      String pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle filter (Singles):" << pfn << endl;
      histos = new ParticleSingleHistos(this,createName(bn,efn,pfn),configuration);
      histos->createHistograms();
      baseSingleHistograms.push_back(histos);
      }

    // pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      String pfn1 = particleFilters[iParticleFilter1]->getName();
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        String pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
        histos = new ParticlePairHistos(this,createName(bn,efn,pfn1,pfn2),configuration);
        histos->createHistograms();
        basePairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePairAnalyzer::loadHistograms(TFile * inputFile)
{
  if (reportStart(__FUNCTION__))
    ;
  if (!inputFile)
    {
    if (reportError(__FUNCTION__)) cout << "Given inputFile pointer is null" << endl;
    postTaskError();
    return;
    }
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  String bn  = getName();
  HistogramGroup * histos;

  fillEta = getValueBool("FillEta");
  fillY   = getValueBool("FillY");
  fillP2  = getValueBool("FillP2");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Loading HistogramGroup............: " << bn << endl;
    cout << "nEventFilters.................: " << nEventFilters << endl;
    cout << "nParticleFilters..............: " << nParticleFilters << endl;
    cout << "fillEta.......................: " << fillEta  << endl;
    cout << "fillY.........................: " << fillY    << endl;
    cout << "fillP2........................: " << fillP2   << endl;
    cout << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      String pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle filter (Singles):" << pfn << endl;
      histos = new ParticleSingleHistos(this,createName(bn,efn,pfn),configuration);
      histos->loadHistograms(inputFile);
      baseSingleHistograms.push_back(histos);
      }

    // pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      String pfn1 = particleFilters[iParticleFilter1]->getName();
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        String pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
        histos = new ParticlePairHistos(this,createName(bn,efn,pfn1,pfn2),configuration);
        histos->loadHistograms(inputFile);
        basePairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}


void ParticlePairAnalyzer::execute()
{
  incrementTaskExecuted();
  Event & event = *eventStreams[0];

  vector<Particle*> & particles = event.getParticles();
  unsigned int nParticles = particles.size();
  if (false)
    {
    //Is this event accepted by this task's event filters?
    bool analyzeThisEvent = false;
    unsigned int nEventFilters = eventFilters.size();
    vector<unsigned int> eventFilterPassed;
    eventFilterPassed.clear();
    resetNParticlesAcceptedEvent();
    for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
      {
      if (!eventFilters[iEventFilter]->accept(event)) continue;
      incrementNEventsAccepted(iEventFilter);
      eventFilterPassed.push_back(iEventFilter);
      analyzeThisEvent = true;
      }
    //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 2 --" << endl;
    if (!analyzeThisEvent) return;
    if (nParticles<2) return;

    // in this branch we pixelize before filling histograms...
    Factory<ParticleDigit> * factory = ParticleDigit::getFactory();
    factory->reset();

    //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 4 --" << endl;

    // produce sublists with ParticleDigits so we do not have to digitize too many
    // times..
    // The histo instance fetched here is used for digitization only. So
    // we use instance [0];
    ParticlePairHistos * histos = (ParticlePairHistos *) basePairHistograms[0];
    unsigned int nParticleFilters = particleFilters.size();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ ) filteredParticles[iParticleFilter].clear();
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      Particle & particle = * (particles[iParticle]);
      float pt, e, phi;
      int iPt, iPhi, iEta, iY;
      ParticleDigit * pd;
      bool digitized = false;
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        if (particleFilters[iParticleFilter]->accept(particle))
          {
          if (!digitized)
            {
            LorentzVector & momentum = particle.getMomentum();
            pt        = momentum.Pt();
            e         = momentum.E();
            phi       = momentum.Phi();
            if (phi<0.0) phi += TMath::TwoPi();
            iPt       = histos->getPtBinFor(pt);
            if (iPt==0) continue;
            iPhi      = histos->getPhiBinFor(phi);
            if (iPhi==0) continue;
            iEta = fillEta ? histos->getEtaBinFor(momentum.Eta())    : 0;
            iY   = fillY   ? histos->getYBinFor(momentum.Rapidity()) : 0;
            if (iEta==0 && iY==0) continue;
            pd        = factory->getNextObject();
            pd->iY    = iY;
            pd->iEta  = iEta;
            pd->iPt   = iPt;
            pd->iPhi  = iPhi;
            pd->pt    = pt;
            pd->e     = e;
            digitized = true; // so no need to digitize this particle again..
            }
          filteredParticles[iParticleFilter].push_back(pd);
          //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 7 --" << endl;
          } // particle accepted by filter
        } //particle loop
      } // particle filter loop
    //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 8 --" << endl;
    // use the filtered particles to fill the histos for the accepted event filters
    for (unsigned int jEventFilter=0; jEventFilter<eventFilterPassed.size(); jEventFilter++ )
      {
      //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 9 --" << endl;
      unsigned int  iEventFilter = eventFilterPassed[jEventFilter];
      unsigned int  baseSingle   = iEventFilter*nParticleFilters;
      unsigned int  basePair     = iEventFilter*nParticleFilters*nParticleFilters;
      unsigned int  index;
      for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
        {
        incrementNParticlesAccepted(iEventFilter,iParticleFilter1);
        //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 10 --" << endl;
        index = baseSingle + iParticleFilter1;
        ParticleSingleHistos * histos = (ParticleSingleHistos *) baseSingleHistograms[index];
        histos->fill(filteredParticles[iParticleFilter1],1.0);
        for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
          {
          //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 11 --" << endl;
          index = basePair + iParticleFilter1*nParticleFilters + iParticleFilter2;
          ParticlePairHistos * histos = (ParticlePairHistos *) basePairHistograms[index];
          histos->fill(filteredParticles[iParticleFilter1],filteredParticles[iParticleFilter2],iParticleFilter1==iParticleFilter2,1.0);
          //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 13 --" << endl;
          }
        }
      }
    }
  else
    {
    for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
      {
      if (!eventFilters[iEventFilter]->accept(event)) continue;
      incrementNEventsAccepted(iEventFilter);
      unsigned int  baseSingle   = iEventFilter*nParticleFilters;
      unsigned int  basePair     = iEventFilter*nParticleFilters*nParticleFilters;
      unsigned int  index;
      for (unsigned int iParticle1=0; iParticle1<nParticles; iParticle1++)
        {
        Particle & particle1 = *(particles[iParticle1]);
        for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
          {
          if (particleFilters[iParticleFilter1]->accept(particle1))
            {
            incrementNParticlesAccepted(iEventFilter,iParticleFilter1);
            index = baseSingle + iParticleFilter1;
            ParticleSingleHistos * histos = (ParticleSingleHistos *) baseSingleHistograms[index];
            histos->fill(particle1,1.0);
            }
          }
        for (unsigned int iParticle2=0; iParticle2<nParticles; iParticle2++)
          {
          if (iParticle1==iParticle2) continue;
          Particle & particle2 = *(particles[iParticle2]);
          for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
            {
            bool accept1 = particleFilters[iParticleFilter1]->accept(particle1);
            for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
              {
              bool accept2 = particleFilters[iParticleFilter2]->accept(particle2);
              if (accept1 & accept2)
                {
                index = basePair + iParticleFilter1*nParticleFilters + iParticleFilter2;
                ParticlePairHistos * histos = (ParticlePairHistos *) basePairHistograms[index];
                histos->fill(particle1, particle2,1.0);
                }
              }
            }
          }
        }
      }
    }
}


void ParticlePairAnalyzer::scaleHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  double scalingFactor;
  if (reportInfo(__FUNCTION__)) printEventStatistics();
  int index = 0;
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    long nAccepted = getNEventsAcceptedReset();
    if (nAccepted>1)
      {

      scalingFactor = 1.0/double(nAccepted);
      for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        index = iEventFilter*nParticleFilters + iParticleFilter;
        baseSingleHistograms[index]->scale(scalingFactor);
        }
      for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
        {
        for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
          {
          index = iEventFilter*nParticleFilters*nParticleFilters + iParticleFilter1*nParticleFilters + iParticleFilter2;
          basePairHistograms[index]->scale(scalingFactor);
          }
        }
      }
    else
      {
      if (reportWarning(__FUNCTION__))
        {
        cout << endl;
        cout << "                            iEventFilter: " <<  iEventFilter<< endl;
        cout << "      nEventsAcceptedTotal[iEventFilter]: " <<  nAccepted<< endl;
        cout << "                    no scaling performed: " <<  endl;
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePairAnalyzer::createDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  derivedSingleHistograms.clear();
  derivedPairHistograms.clear();
  Configuration & configuration = getConfiguration();
  String bn  = getName();
  HistogramGroup * histos;
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Creating Derived HistogramGroup....: " << bn << endl;
    cout << "nEventFilters..................: " << nEventFilters << endl;
    cout << "nParticleFilters...............: " << nParticleFilters << endl;
    cout << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    // singles
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      String pfn1 = particleFilters[iParticleFilter1]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle with filter: " << pfn1 << endl;
      histos = new ParticleSingleDerivedHistos(this,createName(bn,efn,pfn1),configuration);
      histos->createHistograms();
      derivedSingleHistograms.push_back(histos);
      }
    // pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      String pfn1 = particleFilters[iParticleFilter1]->getName();
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        String pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
        histos = new ParticlePairDerivedHistos(this,createName(bn,efn,pfn1,pfn2),configuration);
        histos->createHistograms();
        derivedPairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePairAnalyzer::loadDerivedHistograms(TFile * inputFile __attribute__((unused)))
{

}

void ParticlePairAnalyzer::calculateDerivedHistograms()
{
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  bool binCorrPP = getValueDouble("binCorrPP");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "Computing derived histograms for.......: " << endl;
    cout << "nEventFilters..........................: " << nEventFilters << endl;
    cout << "nParticleFilters.......................: " << nParticleFilters << endl;
    cout << "nSingleHistos..........................: " << getNBaseSingleHistograms() << endl;
    cout << "nPairHistos............................: " << getNBasePairHistograms() << endl;
    cout << "nDerivedHistos.........................: " << getNDerivedPairHistograms() << endl;
    }
  ParticleSingleHistos        * bSingleHistos1;
  ParticleSingleHistos        * bSingleHistos2;
  ParticlePairHistos    * bPairHistos;
  ParticleSingleDerivedHistos * dSingleHistos1;
  ParticleSingleDerivedHistos * dSingleHistos2;
  ParticlePairDerivedHistos * dPairHistos;

  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (reportDebug(__FUNCTION__)) cout << "  iEventFilter:" << iEventFilter << endl;
    unsigned int index;
    unsigned int baseSingle        = iEventFilter*nParticleFilters;
    unsigned int basePair          = iEventFilter*nParticleFilters*nParticleFilters;
    //unsigned int baseDerivedPair   = basePair;

    //! Calculate derived spectra of singles
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
      {
      String pfn1 = particleFilters[iParticleFilter1]->getName();
      if (reportDebug(__FUNCTION__)) cout << "  Single: iParticleFilter1:" << iParticleFilter1 << " named:" << pfn1 << endl;
      index = baseSingle+iParticleFilter1;
      //if (reportDebug(__FUNCTION__))   cout << " (1) iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << " with index:" << index << endl;
      bSingleHistos1 = (ParticleSingleHistos *) baseSingleHistograms[index];
      //if (reportDebug(__FUNCTION__))   cout << " (1a) iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << " with index:" << index << endl;
      dSingleHistos1 = (ParticleSingleDerivedHistos *) derivedSingleHistograms[index];
      //if (reportDebug(__FUNCTION__))   cout << " (2) iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << " with index:" << index << endl;
      dSingleHistos1->calculateDerivedHistograms(bSingleHistos1);
      //if (reportDebug(__FUNCTION__))   cout << " (3) iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << " with index:" << index << endl;
      }

    //! Calculate derived spectra of pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
      {
      String pfn1 = particleFilters[iParticleFilter1]->getName();
      if (reportDebug(__FUNCTION__)) cout << "  Pair: iParticleFilter1:" << iParticleFilter1 << " named:" << pfn1 << endl;
      index = baseSingle+iParticleFilter1;
      bSingleHistos1 = (ParticleSingleHistos *) baseSingleHistograms[index];
      dSingleHistos1 = (ParticleSingleDerivedHistos *) derivedSingleHistograms[index];

      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++)
        {
        String pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "  Pair: iParticleFilter2:" << iParticleFilter2 << " named:" << pfn2 << endl;
        index = baseSingle+iParticleFilter2;
        bSingleHistos2 = (ParticleSingleHistos *) baseSingleHistograms[index];
        dSingleHistos2 = (ParticleSingleDerivedHistos *) derivedSingleHistograms[index];
        index = basePair+iParticleFilter1*nParticleFilters+iParticleFilter2;
        bPairHistos = (ParticlePairHistos *) basePairHistograms[index];
        dPairHistos = (ParticlePairDerivedHistos *) derivedPairHistograms[index];
        if (reportDebug(__FUNCTION__))
          {
          cout << endl;
          cout << "  bSingleHistos1.........:" << bSingleHistos1->getName() << endl;
          cout << "  bSingleHistos2.........:" << bSingleHistos2->getName() << endl;
          cout << "  dSingleHistos1.........:" << dSingleHistos1->getName() << endl;
          cout << "  dSingleHistos2.........:" << dSingleHistos2->getName() << endl;
          cout << "  bPairHistos............:" << bPairHistos->getName() << endl;
          cout << "  dPairHistos............:" << dPairHistos->getName() << endl;
          }
        dPairHistos->calculatePairDerivedHistograms(*bSingleHistos1,*bSingleHistos2,*dSingleHistos1,*dSingleHistos2,*bPairHistos,binCorrPP);
        if (reportDebug(__FUNCTION__)) cout << "   WTF 8" << endl;
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}
