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
using CAP::ParticlePair3DDerivedHistogramCalculator;

ClassImp(ParticlePair3DDerivedHistogramCalculator);

ParticlePair3DDerivedHistogramCalculator::ParticlePair3DDerivedHistogramCalculator(const String & _name,
                                                                                   const Configuration & _configuration,
                                                                                   vector<EventFilter*> & _eventFilters,
                                                                                   vector<ParticleFilter*> &_particleFilters)
:
DerivedHistogramCalculator(_name, _configuration, _eventFilters, _particleFilters)
{
  appendClassName("ParticlePair3DDerivedHistogramCalculator");
}

void ParticlePair3DDerivedHistogramCalculator::setDefaultConfiguration()
{
  Task::setDefaultConfiguration();
  addParameter("UseParticles",     true);
  addParameter("HistogramsCreate", true);
  addParameter("HistogramsImport",   true);
  addParameter("HistogramsExport",   true);
  addParameter("binCorrPP", 1.0);
    
  addParameter(fillEta",  true);
  addParameter(fillY",    false);
  addParameter(fillP2",   false);
  
  addParameter(nBins_n1",  100);
  addParameter(min_n1",    0.0);
  addParameter(max_n1",  100.0);
  
  addParameter(nBins_eTot",  100);
  addParameter(min_eTot",    0.0);
  addParameter(max_eTot",  100.0);
  
  addParameter(nBins_pt",   18);
  addParameter(min_pt",   0.20);
  addParameter(max_pt",   2.00);
  
  addParameter(nBins_phi",  72);
  addParameter(min_phi",   0.0);
  addParameter(max_phi", TMath::TwoPi());
  
  addParameter(nBins_eta",   20);
  addParameter(min_eta",   -1.0);
  addParameter(max_eta",    1.0);
  
  addParameter(nBins_y",     20);
  addParameter(min_y",     -1.0);
  addParameter(max_y",      1.0);
  
  addParameter(nBins_phiEta",    720);
  addParameter(nBins_phiEtaPt",  7200);
  addParameter(nBins_phiY",      720);
  addParameter(nBins_phiYPt",    7200);
  
  addParameter(nBins_n2",          100);
  addParameter(min_n2",            0.0);
  addParameter(max_n2",         1000.0);
  
  addParameter(nBins_DeltaPlong",   10);
  addParameter(min_DeltaPlong",   -1.0);
  addParameter(max_DeltaPlong",    1.0);
  
  addParameter("nBins_DeltaPside",   10);
  addParameter("min_DeltaPside",   -1.0);
  addParameter("max_DeltaPside",    1.0);
  addParameter("range_DeltaPside",  2.0);
  addParameter("nBins_DeltaPout",    10);
  addParameter("min_DeltaPout",    -1.0);
  addParameter("max_DeltaPout",     1.0);
  addParameter("range_DeltaPout",   2.0);
  
  addParameter("nBins_Dphi",         36);
  addParameter("min_Dphi",          0.0);
  addParameter("max_Dphi",TMath::TwoPi());
  addParameter("width_Dphi",TMath::TwoPi());
  
  addParameter("nBins_Dphi_shft",    36);
  addParameter("min_Dphi_shft",     0.0);
  addParameter("max_Dphi_shft",     0.0);
  
  addParameter("nBins_Deta",         39);
  addParameter("min_Deta",         -2.0);
  addParameter("max_Deta",          2.0);
  addParameter("width_Deta",   4.0/39.0);
  
  addParameter("nBins_Dy",           39);
  addParameter("min_Dy",           -2.0);
  addParameter("max_Dy",            2.0);
  addParameter("width_Dy",     4.0/39.0);
}

void ParticlePair3DDerivedHistogramCalculator::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  derivedSingleHistograms.clear();
  derivedPairHistograms.clear();
  const Configuration & configuration = getConfiguration();
  Severity debugLevel = getSeverityLevel();
  String bn  = getName();
  HistogramGroup * histos;
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "       Creating HistogramGroup: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
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
      histos = new ParticleSingleDerivedHistos(createName(bn,efn,pfn1),configuration);
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
        histos = new ParticlePair3DDerivedHistos(createName(bn,efn,pfn1,pfn2),configuration);
        histos->createHistograms();
        derivedPairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void ParticlePair3DDerivedHistogramCalculator::importHistograms(TFile & inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  if (!ptrFileExist(inputFile)) return;
  
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  const Configuration & configuration = getConfiguration();
  Severity debugLevel = getSeverityLevel();
  String bn  = getName();
   if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "       Loading HistogramGroup: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  baseSingleHistograms.clear();
  basePairHistograms.clear();
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String efn = eventFilters[iEventFilter]->getName();
    if (reportDebug(__FUNCTION__)) cout << "Event filter:" << efn << endl;
    // singles
    for (int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      String pfn1 = particleFilters[iParticleFilter1]->getName();
      if (reportDebug(__FUNCTION__)) cout << "Particle with filter: " << pfn1 << endl;
      ParticleSingleHistos * histos = new ParticleSingleHistos(createName(bn,efn,pfn1),configuration);
      histos->importHistograms(inputFile);
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
        ParticlePair3DHistos * histos = new ParticlePair3DHistos(createName(bn,efn,pfn1,pfn2),configuration);
        histos->importHistograms(inputFile);
        basePairHistograms.push_back(histos);
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}

//!
//! Calculate Derived HistogramGroup
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
  ParticleSingleHistos        * bSingleHistos1;
  ParticleSingleHistos        * bSingleHistos2;
  ParticlePair3DHistos    * bPairHistos;
  ParticleSingleDerivedHistos * dSingleHistos1;
  ParticleSingleDerivedHistos * dSingleHistos2;
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
      String pfn1 = particleFilters[iParticleFilter1]->getName();
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
      index = baseSingle+iParticleFilter1;
      bSingleHistos1 = (ParticleSingleHistos *) baseSingleHistograms[index];
      dSingleHistos1 = (ParticleSingleDerivedHistos *) derivedSingleHistograms[index];
      
      for (int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++)
        {
        String pfn2 = particleFilters[iParticleFilter2]->getName();
//        if (reportDebug(__FUNCTION__))
//          cout << "  iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << ";  iParticleFilter2:" << iParticleFilter2<< " named " << pfn2  << endl;
        index = baseSingle+iParticleFilter2;
        bSingleHistos2 = (ParticleSingleHistos *) baseSingleHistograms[index];
        dSingleHistos2 = (ParticleSingleDerivedHistos *) derivedSingleHistograms[index];
        
        index = basePair+iParticleFilter1*nParticleFilters+iParticleFilter2;
        bPairHistos = (ParticlePair3DHistos *) basePairHistograms[index];
        dPairHistos = (ParticlePair3DDerivedHistos *) derivedPairHistograms[index];
        dPairHistos->calculatePairDerivedHistograms(*bSingleHistos1,*bSingleHistos2,*dSingleHistos1,*dSingleHistos2,*bPairHistos,
                                                    configuration.getValueDouble(getName(),"binCorrPP") );
        }
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}
