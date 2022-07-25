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
#include "ParticlePair3DDerivedHistogramCalculator.hpp"

ClassImp(ParticlePair3DDerivedHistogramCalculator);

ParticlePair3DDerivedHistogramCalculator::ParticlePair3DDerivedHistogramCalculator(const TString &          _name,
                                                                               const Configuration    & _configuration,
                                                                               vector<EventFilter*>   & _eventFilters,
                                                                               vector<ParticleFilter*>& _particleFilters,
                                                                               LogLevel _selectedLevel)
:
DerivedHistogramCalculator(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  appendClassName("ParticlePair3DDerivedHistogramCalculator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void ParticlePair3DDerivedHistogramCalculator::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  configuration.setName("ParticlePair3DDerivedHistogramCalculator Configuration");
  configuration.setParameter("useParticles",     true);
  configuration.setParameter("createHistograms", true);
  configuration.setParameter("loadHistograms",   true);
  configuration.setParameter("saveHistograms",   true);
  configuration.setParameter("binCorrPP", 1.0);
    
  configuration.addParameter("fillEta",  true);
  configuration.addParameter("fillY",    false);
  configuration.addParameter("fillP2",   false);
  
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
}

void ParticlePair3DDerivedHistogramCalculator::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  derivedSingleHistograms.clear();
  derivedPairHistograms.clear();
  const Configuration & configuration = getConfiguration();
  LogLevel debugLevel = getReportLevel();
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
    // singles
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle with filter: " << pfn1 << endl;
      histos = new ParticleDerivedHistos(makeHistoName(bn,efn,pfn1),configuration,debugLevel);
      histos->createHistograms();
      derivedSingleHistograms.push_back(histos);
      }
    // pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(__FUNCTION__)) cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
        histos = new ParticlePair3DDerivedHistos(makeHistoName(bn,efn,pfn1,pfn2),configuration,debugLevel);
        histos->createHistograms();
        derivedPairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePair3DDerivedHistogramCalculator::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(inputFile)) return;
  
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  const Configuration & configuration = getConfiguration();
  LogLevel debugLevel = getReportLevel();
  TString bn  = getName();
   if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "       Loading Histograms: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    // singles
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle with filter: " << pfn1 << endl;
      ParticleHistos * histos = new ParticleHistos(makeHistoName(bn,efn,pfn1),configuration,debugLevel);
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
        ParticlePair3DHistos * histos = new ParticlePair3DHistos(makeHistoName(bn,efn,pfn1,pfn2),configuration,debugLevel);
        histos->loadHistograms(inputFile);
        basePairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Calculate Derived Histograms
//!
// ====================================
void ParticlePair3DDerivedHistogramCalculator::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  //incrementTaskExecuted();
  //const Configuration & config  = getConfiguration();
   if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "Computing derived histograms for: " << endl;
    cout << "                   nEventFilters: " << nEventFilters << endl;
    cout << "                nParticleFilters: " << nParticleFilters << endl;
    cout << "                   nSingleHistos: " << getNBaseSingleHistograms() << endl;
    cout << "                     nPairHistos: " << getNBasePairHistograms() << endl;
    cout << "                  nDerivedHistos: " << getNDerivedPairHistograms() << endl;
    }
  ParticleHistos        * bSingleHistos1;
  ParticleHistos        * bSingleHistos2;
  ParticlePair3DHistos    * bPairHistos;
  ParticleDerivedHistos * dSingleHistos1;
  ParticleDerivedHistos * dSingleHistos2;
  ParticlePair3DDerivedHistos * dPairHistos;
  
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
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      index = baseSingle+iParticleFilter1;
      //if (reportDebug(__FUNCTION__))   cout << " (1) iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << " with index:" << index << endl;
      bSingleHistos1 = (ParticleHistos *) baseSingleHistograms[index];
      //if (reportDebug(__FUNCTION__))   cout << " (1a) iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << " with index:" << index << endl;
      dSingleHistos1 = (ParticleDerivedHistos *) derivedSingleHistograms[index];
      //if (reportDebug(__FUNCTION__))   cout << " (2) iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << " with index:" << index << endl;
      dSingleHistos1->calculateDerivedHistograms(bSingleHistos1);
      //if (reportDebug(__FUNCTION__))   cout << " (3) iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << " with index:" << index << endl;
      }
    
    //! Calculate derived spectra of pairs
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      index = baseSingle+iParticleFilter1;
      bSingleHistos1 = (ParticleHistos *) baseSingleHistograms[index];
      dSingleHistos1 = (ParticleDerivedHistos *) derivedSingleHistograms[index];
      
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++)
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
//        if (reportDebug(__FUNCTION__))
//          cout << "  iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << ";  iParticleFilter2:" << iParticleFilter2<< " named " << pfn2  << endl;
        index = baseSingle+iParticleFilter2;
        bSingleHistos2 = (ParticleHistos *) baseSingleHistograms[index];
        dSingleHistos2 = (ParticleDerivedHistos *) derivedSingleHistograms[index];
        
        index = basePair+iParticleFilter1*nParticleFilters+iParticleFilter2;
        bPairHistos = (ParticlePair3DHistos *) basePairHistograms[index];
        dPairHistos = (ParticlePair3DDerivedHistos *) derivedPairHistograms[index];
        dPairHistos->calculatePairDerivedHistograms(*bSingleHistos1,*bSingleHistos2,*dSingleHistos1,*dSingleHistos2,*bPairHistos,
                                                    configuration.getValueDouble("binCorrPP") );
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}
