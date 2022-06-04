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
#include "ParticlePairDerivedHistogramCalculator.hpp"

ClassImp(ParticlePairAnalyzer);

ParticlePairAnalyzer::ParticlePairAnalyzer(const TString &          _name,
                                           const Configuration &    _configuration,
                                           vector<EventFilter*> &   _eventFilters,
                                           vector<ParticleFilter*> &_particleFilters,
                                           LogLevel                 _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel),
fillEta(true),
fillY(false),
fillP2(false)
{
  appendClassName("ParticlePairAnalyzer");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
  for (unsigned int k=0; k<particleFilters.size(); k++)
    {
    vector<ParticleDigit*> list;
    filteredParticles.push_back(list);
    }
}

void ParticlePairAnalyzer::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  configuration.setName("ParticlePairAnalyzer Configuration");
  configuration.setParameter("useParticles",      true);
  configuration.setParameter("createHistograms",  true);
  configuration.setParameter("saveHistograms",    true);
  configuration.setParameter("histoAnalyzerName", TString("Pair"));
  configuration.setParameter("histoBaseName",     TString("Pair"));

  configuration.addParameter("fillEta",  fillEta);
  configuration.addParameter("fillY",    fillY);
  configuration.addParameter("fillP2",   fillP2);
  
  configuration.addParameter("nBins_n1",  100);
  configuration.addParameter("min_n1",    0.0);
  configuration.addParameter("max_n1",  100.0);

  configuration.addParameter("nBins_eTot",  100);
  configuration.addParameter("min_eTot",    0.0);
  configuration.addParameter("max_eTot",  100.0);

  configuration.addParameter("nBins_pt",   18);
  configuration.addParameter("min_pt",   0.20);
  configuration.addParameter("max_pt",   2.00);
  
  configuration.addParameter("nBins_phi",  72);
  configuration.addParameter("min_phi",   0.0);
  configuration.addParameter("max_phi", TMath::TwoPi());
  
  configuration.addParameter("nBins_eta",   20);
  configuration.addParameter("min_eta",   -1.0);
  configuration.addParameter("max_eta",    1.0);
  
  configuration.addParameter("nBins_y",     20);
  configuration.addParameter("min_y",     -1.0);
  configuration.addParameter("max_y",      1.0);

  configuration.addParameter("nBins_phiEta",    720);
  configuration.addParameter("nBins_phiEtaPt",  7200);
  configuration.addParameter("nBins_phiY",      720);
  configuration.addParameter("nBins_phiYPt",    7200);
  
  configuration.addParameter("nBins_n2",          100);
  configuration.addParameter("min_n2",            0.0);
  configuration.addParameter("max_n2",         1000.0);
  configuration.addParameter("nBins_DeltaPlong",   10);
  configuration.addParameter("min_DeltaPlong",   -1.0);
  configuration.addParameter("max_DeltaPlong",    1.0);
  configuration.addParameter("nBins_DeltaPside",   10);
  configuration.addParameter("min_DeltaPside",   -1.0);
  configuration.addParameter("max_DeltaPside",    1.0);
  configuration.addParameter("range_DeltaPside",  2.0);
  configuration.addParameter("nBins_DeltaPout",    10);
  configuration.addParameter("min_DeltaPout",    -1.0);
  configuration.addParameter("max_DeltaPout",     1.0);
  configuration.addParameter("range_DeltaPout",   2.0);
  configuration.addParameter("nBins_Dphi",         36);
  configuration.addParameter("min_Dphi",          0.0);
  configuration.addParameter("max_Dphi",TMath::TwoPi());
  configuration.addParameter("width_Dphi",TMath::TwoPi());
  configuration.addParameter("nBins_Dphi_shft",    36);
  configuration.addParameter("min_Dphi_shft",     0.0);
  configuration.addParameter("max_Dphi_shft",     0.0);
  configuration.addParameter("nBins_Deta",         39);
  configuration.addParameter("min_Deta",         -2.0);
  configuration.addParameter("max_Deta",          2.0);
  configuration.addParameter("width_Deta",   4.0/39.0);
  configuration.addParameter("nBins_Dy",           39);
  configuration.addParameter("min_Dy",           -2.0);
  configuration.addParameter("max_Dy",            2.0);
  configuration.addParameter("width_Dy",     4.0/39.0);
  // if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePairAnalyzer::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  LogLevel debugLevel = getReportLevel();
  TString bn  = getName();
  fillEta = configuration.getValueBool("fillEta");
  fillY   = configuration.getValueBool("fillY");
  fillP2  = configuration.getValueBool("fillP2");
  
  Histograms * histos;
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "       Creating Histograms: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle filter (Singles):" << pfn << endl;
      histos = new ParticleHistos(makeHistoName(bn,efn,pfn),configuration,debugLevel);
      histos->createHistograms();
      baseSingleHistograms.push_back(histos);
      }

    // pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
        histos = new ParticlePairHistos(makeHistoName(bn,efn,pfn1,pfn2),configuration,debugLevel);
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
  if (!ptrFileExist(inputFile)) return;
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  Configuration & configuration = getConfiguration();
  LogLevel debugLevel    = getReportLevel();
  TString bn  = getName();
  Histograms * histos;
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "       Creating Histograms: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    for (int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle filter (Singles):" << pfn << endl;
      histos = new ParticlePairHistos(makeHistoName(bn,efn,pfn),configuration,debugLevel);
      histos->loadHistograms(inputFile);
      baseSingleHistograms.push_back(histos);
      }

    // pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
        histos = new ParticlePairHistos(makeHistoName(bn,efn,pfn1,pfn2),configuration,debugLevel);
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
//  
//  if (reportStart(__FUNCTION__))
//    ;
  incrementTaskExecuted();
  Event & event = *eventStreams[0];

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

  //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 2b --" << endl;

  // The event is accepted at least one event filter
  // but it may not have particle pairs. If so skip out.
  // Doing the checks in this order guarantees the accepted
  // event count is correct for normalization purposes.
  unsigned int nParticles = event.getNParticles();
  if (nParticles<2) return;

  //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 3 --" << endl;

  Factory<ParticleDigit> * factory = ParticleDigit::getFactory();
  factory->reset();

  //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 4 --" << endl;

  // produce sublists with ParticleDigits so we do not have to digitize too many
  // times..
  // The histo instance fetched here is used for digitization only. So
  // we use instance [0];
  ParticlePairHistos * histos = (ParticlePairHistos *) basePairHistograms[0];
  
//    cout << "Particle Pair Analyzer Test :" << endl;
//    cout <<  histos->getPtBinFor(0.5) << endl;
//    exit(1);
  
  
  unsigned int nParticleFilters = particleFilters.size();
  for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ ) filteredParticles[iParticleFilter].clear();

  //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 5 --" << endl;

  for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
    {
    Particle & particle = * event.getParticleAt(iParticle);
    //    TLorentzVector & what = particle.getMomentum();
    //    double rap = what.Rapidity();
    //    if (rap<-1.0)
    //      {
    //      cout << " rap: " << rap << endl;
    //      }

    //    int pdg = particle.getType().getPdgCode();
    //    if (pdg==3122|| pdg==-3122)
    //      {
    //      cout << "pdg:" << pdg << endl;
    //      cout << "Found L" << endl;
    //      exit(0);
    //      }
    float pt, e;
    int iPt, iPhi, iEta, iY;
    ParticleDigit * pd;
    bool digitized = false;
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      if (particleFilters[iParticleFilter]->accept(particle))
        {

        if (!digitized)
          {
          TLorentzVector & momentum = particle.getMomentum();
          pt        = momentum.Pt();
          e         = momentum.E();
          iPt       = histos->getPtBinFor(pt);
          iPhi      = histos->getPhiBinFor(momentum.Phi());
          iEta = fillEta ? histos->getEtaBinFor(momentum.Eta())    : -1;
          iY   = fillY   ? histos->getYBinFor(momentum.Rapidity()) : -1;
          
         // cout << " pt: " << pt << " e:" << e <<  " iPt: " << iPt << " iPhi:" << iPhi << " iEta: " << iEta << " iY: " << iY << endl;
          
          pd        = factory->getNextObject();
          pd->iY    = iY;
          pd->iEta  = iEta;
          pd->iPt   = iPt;
          pd->iPhi  = iPhi;
          pd->pt    = pt;
          pd->e     = e;
          digitized = true; // so no need to digitize this particle again..
          }
        if (digitized && iPt>=0 && iPhi>=0 && (iEta>=0 || iY>=0) )
          {
          filteredParticles[iParticleFilter].push_back(pd);
          }
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
      incrementNParticlesAccepted(jEventFilter,iParticleFilter1);
      //if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()")) cout << " -- 10 --" << endl;
      index = baseSingle + iParticleFilter1;
      ParticleHistos * histos = (ParticleHistos *) baseSingleHistograms[index];
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
//  if (reportEnd(__FUNCTION__))
//    ;
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
        cout << "           nEventsAcceptedTotal[iEventFilter]: " <<  nAccepted<< endl;
        cout << "                    no scaling performed: " <<  endl;
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

Task * ParticlePairAnalyzer::getDerivedCalculator()
{
  if (reportDebug(__FUNCTION__))
    ;
  TString nameD = getName();
  if (reportDebug(__FUNCTION__)) cout << "Name of this task is:" << nameD  << endl;
  Configuration derivedCalcConfiguration;
  // copy the parameters of this task to the new task -- so all the histograms will automatically match
  derivedCalcConfiguration.setParameters(configuration);
  derivedCalcConfiguration.setParameter("createHistograms",       true);
  derivedCalcConfiguration.setParameter("loadHistograms",         true);
  derivedCalcConfiguration.setParameter("saveHistograms",         true);
  Task * calculator = new ParticlePairDerivedHistogramCalculator(nameD,derivedCalcConfiguration,eventFilters,particleFilters,getReportLevel());
  return calculator;
}
