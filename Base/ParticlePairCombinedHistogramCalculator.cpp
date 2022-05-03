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
#include "ParticlePairCombinedHistogramCalculator.hpp"

ClassImp(ParticlePairCombinedHistogramCalculator);

ParticlePairCombinedHistogramCalculator::ParticlePairCombinedHistogramCalculator(const TString &         _name,
                                                                                 Configuration &         _configuration,
                                                                                 vector<EventFilter*>    _eventFilters,
                                                                                 vector<ParticleFilter*> _particleFilters,
                                                                                 LogLevel                _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  appendClassName("ParticlePairCombinedHistogramCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
  for (unsigned int k=0; k<particleFilters.size(); k++)
    {
    vector<ParticleDigit*> list;
    filteredParticles.push_back(list);
    }
}

void ParticlePairCombinedHistogramCalculator::setDefaultConfiguration()
{
  if (reportStart("ParticlePairCombinedHistogramCalculator",getName(),"setDefaultConfiguration()"))
    {
    }
  Configuration & configuration = getConfiguration();
  configuration.setName("ParticlePairCombinedHistogramCalculator Configuration");
  configuration.setParameter("useParticles", true);
  configuration.setParameter("createHistograms", true);
  configuration.setParameter("saveHistograms", true);
  configuration.setParameter("nBins_n2",          100);
  configuration.setParameter("min_n2",            0.0);
  configuration.setParameter("max_n2",         1000.0);
  configuration.setParameter("nBins_DeltaPlong",   10);
  configuration.setParameter("min_DeltaPlong",   -1.0);
  configuration.setParameter("max_DeltaPlong",    1.0);
  configuration.setParameter("nBins_DeltaPside",   10);
  configuration.setParameter("min_DeltaPside",   -1.0);
  configuration.setParameter("max_DeltaPside",    1.0);
  configuration.setParameter("range_DeltaPside",  2.0);
  configuration.setParameter("nBins_DeltaPout",    10);
  configuration.setParameter("min_DeltaPout",    -1.0);
  configuration.setParameter("max_DeltaPout",     1.0);
  configuration.setParameter("range_DeltaPout",   2.0);
  configuration.setParameter("nBins_Dphi",         36);
  configuration.setParameter("min_Dphi",          0.0);
  configuration.setParameter("max_Dphi",TMath::TwoPi());
  configuration.setParameter("width_Dphi",TMath::TwoPi());
  configuration.setParameter("nBins_Dphi_shft",    36);
  configuration.setParameter("min_Dphi_shft",     0.0);
  configuration.setParameter("max_Dphi_shft",     0.0);
  configuration.setParameter("nBins_Deta",         39);
  configuration.setParameter("min_Deta",         -2.0);
  configuration.setParameter("max_Deta",          2.0);
  configuration.setParameter("width_Deta",   4.0/39.0);
  configuration.setParameter("nBins_Dy",           39);
  configuration.setParameter("min_Dy",           -2.0);
  configuration.setParameter("max_Dy",            2.0);
  configuration.setParameter("width_Dy",     4.0/39.0);
  if (reportDebug("ParticlePairCombinedHistogramCalculator",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}

void ParticlePairCombinedHistogramCalculator::createHistograms()
{
  if (reportStart("ParticlePairCombinedHistogramCalculator",getName(),"createHistograms()"))
    ;
  combinedHistograms.clear();
  Configuration & configuration = getConfiguration();
  LogLevel debugLevel    = getReportLevel();
  TString bn  = getName();
  bn += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  
  Histograms * histos;
  if (reportInfo("ParticlePairCombinedHistogramCalculator",getName(),"createHistograms()"))
    {
    cout << endl;
    cout << "       Creating Histograms: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    if (reportDebug("ParticlePairCombinedHistogramCalculator",getName(),"createHistograms()"))
      {
      cout << "Event filter:" << efn << endl;
      }
    // singles
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug("ParticlePairCombinedHistogramCalculator",getName(),"createHistograms()"))
        {
        cout << "Particle filter (Singles):" << pfn << endl;
        }
      histos = new ParticleHistos(createHistogramName(bn,efn,pfn,""),configuration,debugLevel);
      histos->createHistograms();
      baseSingleHistograms.push_back(histos);
      }
    
    // pairs
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug("ParticlePairCombinedHistogramCalculator",getName(),"createHistograms()"))
          {
          cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
          }
        //        histos = new ParticlePairCombinedHistos(createPairHistogramName(bn,efn,pfn1,pfn2,"_CI"),ac,debugLevel);
        //        histos->createHistograms();
        //        combinedHistograms.push_back(histos);
        //        histos = new ParticlePairCombinedHistos(createPairHistogramName(bn,efn,pfn1,pfn2,"_CD"),ac,debugLevel);
        //        histos->createHistograms();
        //        combinedHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd("ParticlePairCombinedHistogramCalculator",getName(),"createHistograms()"))
    ;
}

void ParticlePairCombinedHistogramCalculator::loadHistograms(TFile * inputFile)
{
  TString fct = "loadHistograms(TFile * inputFile)";
  if (reportStart(fct))
    ;
  if (!ptrFileExist(fct,inputFile)) return;
  combinedHistograms.clear();
  Configuration & configuration = getConfiguration();
  LogLevel debugLevel = getReportLevel();
  TString bn  = getName();
  bn += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  
  Histograms * histos;
  if (reportInfo("ParticlePairCombinedHistogramCalculator",getName(),fct))
    {
    cout << endl;
    cout << "       Creating Histograms: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    if (reportDebug("ParticlePairCombinedHistogramCalculator",getName(),fct))
      {
      cout << "Event filter:" << efn << endl;
      }
    // singles
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug("ParticlePairCombinedHistogramCalculator",getName(),fct))
        {
        cout << "Particle filter (Singles):" << pfn << endl;
        }
      histos = new ParticleHistos(createHistogramName(bn,efn,pfn,""),configuration,debugLevel);
      //      histos->createHistograms();
      //      baseSingleHistograms.push_back(histos);
      }
    
    // pairs
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug("ParticlePairCombinedHistogramCalculator",getName(),fct))
          {
          cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
          }
        //        histos = new ParticlePairCombinedHistos(createPairHistogramName(bn,efn,pfn1,pfn2,"_CI"),ac,debugLevel);
        //        histos->createHistograms();
        //        combinedHistograms.push_back(histos);
        //        histos = new ParticlePairCombinedHistos(createPairHistogramName(bn,efn,pfn1,pfn2,"_CD"),ac,debugLevel);
        //        histos->createHistograms();
        //        combinedHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd("ParticlePairCombinedHistogramCalculator",getName(),fct))
    ;
}



// ====================================
// calculate Derived Histograms
// ====================================
void ParticlePairCombinedHistogramCalculator::calculateCombinedHistograms(ParticlePairDerivedHistos * derivedHistos)
{
  TString fct = "calculateCombinedHistograms()";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  
  //  if (reportInfo(fct))
  //    {
  //    cout << endl;
  //    cout << "           nEventFilters:" << nEventFilters << endl;
  //    cout << "        nParticleFilters:" << nParticleFilters << endl;
  //    cout << "           nSingleHistos:" << getNbaseSingleHistograms() << endl;
  //    cout << "             nPairHistos:" << getNbasePairHistograms() << endl;
  //    cout << "          nDerivedHistos:" << getNDerivedHistograms() << endl;
  //    }
  //  Configuration & ac = getConfiguration();
  //  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
  //    {
  //    if (reportDebug(fct)) cout << "  iEventFilter:" << iEventFilter << endl;
  //    unsigned int index;
  //    unsigned int baseSingle        = iEventFilter*nParticleFilters;
  //    unsigned int basePair          = iEventFilter*nParticleFilters*nParticleFilters;
  //    unsigned int baseDerivedPair   = basePair;
  //    unsigned int baseCombinedPair  = basePair;
  //
  //    //! Calculate derived spectra of pairs
  //    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
  //      {
  //      TString pfn1 = particleFilters[iParticleFilter1]->getName();
  //      index = baseSingle+iParticleFilter1;
  //      ParticleHistos * histos1 = (ParticleHistos *) baseSingleHistograms[index];
  //
  //      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++)
  //        {
  //        TString pfn2 = particleFilters[iParticleFilter2]->getName();
  //        if (reportDebug(fct))
  //          cout << "  iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << ";  iParticleFilter2:" << iParticleFilter2<< " named " << pfn2  << endl;
  //        index = baseSingle+iParticleFilter2;
  //        ParticleHistos * histos2 = (ParticleHistos *) baseSingleHistograms[index];
  //        index = basePair+iParticleFilter1*nParticleFilters+iParticleFilter2;
  //        ParticlePairHistos * pairHistos = (ParticlePairHistos *) basePairHistograms[index];
  //        ParticlePairDerivedHistos * derivedPairHistos = (ParticlePairDerivedHistos *) derivedHistograms[index];
  //        combinedPairHistos->calculateCombinedHistograms(*histos1,*histos2,*pairHistos,ac.binCorrPP);
  //        }
  //      }
  //    }
  if (reportEnd(fct))
    ;
}


