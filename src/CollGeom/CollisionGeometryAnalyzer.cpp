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
#include "CollisionGeometryAnalyzer.hpp"
ClassImp(CollisionGeometryAnalyzer);

CollisionGeometryAnalyzer::CollisionGeometryAnalyzer(const TString &          _name,
                                                     const Configuration  &   _configuration,
                                                     vector<EventFilter*> &   _eventFilters,
                                                     vector<ParticleFilter*>  _particleFilters,
                                                     MessageLogger::LogLevel  _requiredLevel)
:
Task(_name,_configuration,_eventFilters, _particleFilters,_requiredLevel),
collisionGeometry(nullptr)
{
  appendClassName("CollisionGeometryAnalyzer");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void CollisionGeometryAnalyzer::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  configuration.setName("CollisionGeometryAnalyzer Configuration");
  configuration.addParameter(  "aNucleusZ", 0);
  configuration.addParameter(  "aNucleusA", 0);
  configuration.addParameter(  "aGeneratorType", 0);
  configuration.addParameter(  "aParA", 0.0);
  configuration.addParameter(  "aParB", 0.0);
  configuration.addParameter(  "aParC", 0.0);
  configuration.addParameter(  "aNR",  100);
  configuration.addParameter(  "aMinR", 0.0);
  configuration.addParameter(  "aMaxR", 0.0);
  configuration.addParameter(  "bNucleusZ", 0.0);
  configuration.addParameter(  "bNucleusA", 0.0);
  configuration.addParameter(  "bGeneratorType", 0);
  configuration.addParameter(  "bParA", 0.0);
  configuration.addParameter(  "bParB", 0.0);
  configuration.addParameter(  "bParC", 0.0);
  configuration.addParameter(  "bNR", 100);
  configuration.addParameter(  "bMinR", 0.0);
  configuration.addParameter(  "bMaxR", 0.0);
  configuration.addParameter(  "minB", 0.0);
  configuration.addParameter(  "maxB", 0.0);
  configuration.addParameter(  "nnCrossSection", 40.0);
  configuration.addParameter(  "nBins_b", 100);
  configuration.addParameter(  "min_b", 0.0);
  configuration.addParameter(  "max_b", 0.0);
  configuration.addParameter(  "nBins_nPart", 100);
  configuration.addParameter(  "min_nPart", 0);
  configuration.addParameter(  "max_nPart", 400);
  configuration.addParameter(  "nBins_nBinary", 0);
  configuration.addParameter(  "min_nBinary", 0);
  configuration.addParameter(  "max_nBinary", 0);
  configuration.addParameter(  "nBins_bxSect", 100);

  configuration.addParameter("nBins_b",100);
  configuration.addParameter("min_b", 0.0);
  configuration.addParameter("max_b", 10.0);
  configuration.addParameter("nBins_nPart", 400);
  configuration.addParameter("min_nPart", 0.0);
  configuration.addParameter("max_nPart", 400.0);
  configuration.addParameter("nBins_nBinary", 1000);
  configuration.addParameter("min_nBinary", 0.0);
  configuration.addParameter("max_nBinary", 1000.0);
                             
  configuration.addParameter(  "useRecentering",      true);
  configuration.addParameter(  "useNucleonExclusion", false);
  configuration.addParameter(  "useParticles",        true);
  configuration.addParameter(  "createHistograms",    true);
  configuration.addParameter(  "saveHistograms",      true);
  configuration.addParameter(  "useEventStream0",     true);
  if (reportDebug("CollisionGeometryAnalyzer",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}

void CollisionGeometryAnalyzer::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();

  if (reportInfo(__FUNCTION__))
    {
    cout << "Creating Histograms for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    cout << " iEventFilter:"  << iEventFilter << endl;
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    cout << " evtFilterName:"  << evtFilterName << endl;
    TString histoName     = prefixName;
    histoName += evtFilterName;
    cout << " histoName:"  << histoName << endl;
    CollisionGeometryHistograms * histos = new CollisionGeometryHistograms(histoName, configuration, getReportLevel() );
    histos->createHistograms();
    histograms.push_back( histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void CollisionGeometryAnalyzer::loadHistograms(TFile * inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << "Loading Histograms for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    TString histoName     = prefixName;
    histoName += evtFilterName;
    CollisionGeometryHistograms * histos = new CollisionGeometryHistograms(histoName, configuration, getReportLevel() );
    histos->loadHistograms(inputFile);
    histograms.push_back( histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void CollisionGeometryAnalyzer::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  Event & event = *eventStreams[0];
  for (int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (eventFilters[iEventFilter]->accept(event))
      {
      incrementNEventsAccepted(iEventFilter); // count eventStreams used to fill histograms and for scaling at the end..
      CollisionGeometryHistograms * histos = (CollisionGeometryHistograms *) histograms[iEventFilter];
      histos->fill(event,1.0);
      }
    else
      {
      CollisionGeometryHistograms * histos = (CollisionGeometryHistograms *) histograms[iEventFilter];
      histos->noFill(event,1.0);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}
