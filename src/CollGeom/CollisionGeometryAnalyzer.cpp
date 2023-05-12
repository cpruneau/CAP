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
using CAP::CollisionGeometryAnalyzer;

ClassImp(CollisionGeometryAnalyzer);

CollisionGeometryAnalyzer::CollisionGeometryAnalyzer(const String & _name,
                                                     const Configuration &   _configuration,
                                                     vector<EventFilter*> &   _eventFilters,
                                                     vector<ParticleFilter*>  _particleFilters)
:
EventTask(_name,_configuration,_eventFilters, _particleFilters),
collisionGeometry(nullptr)
{
  appendClassName("CollisionGeometryAnalyzer");
}

void CollisionGeometryAnalyzer::setDefaultConfiguration()
{
  addParameter("aNucleusZ", 0);
  addParameter("aNucleusA", 0);
  addParameter("aGeneratorType", 0);
  addParameter("aParA", 0.0);
  addParameter("aParB", 0.0);
  addParameter("aParC", 0.0);
  addParameter("aNR",  100);
  addParameter("aMinR", 0.0);
  addParameter("aMaxR", 0.0);
  addParameter("bNucleusZ", 0.0);
  addParameter("bNucleusA", 0.0);
  addParameter("bGeneratorType", 0);
  addParameter("bParA", 0.0);
  addParameter("bParB", 0.0);
  addParameter("bParC", 0.0);
  addParameter("bNR", 100);
  addParameter("bMinR", 0.0);
  addParameter("bMaxR", 0.0);
  addParameter("MinB", 0.0);
  addParameter("MaxB", 0.0);
  addParameter("nnCrossSection", 40.0);
  addParameter("nBins_b", 100);
  addParameter("Min_b", 0.0);
  addParameter("Max_b", 0.0);
  addParameter("nBins_nPart", 100);
  addParameter("Min_nPart", 0);
  addParameter("Max_nPart", 400);
  addParameter("nBins_nBinary", 0);
  addParameter("Min_nBinary", 0);
  addParameter("Max_nBinary", 0);
  addParameter("nBins_bxSect", 100);
  addParameter("nBins_b",100);
  addParameter("Min_b", 0.0);
  addParameter("Max_b", 10.0);
  addParameter("nBins_nPart", 400);
  addParameter("Min_nPart", 0.0);
  addParameter("Max_nPart", 400.0);
  addParameter("nBins_nBinary", 1000);
  addParameter("Min_nBinary", 0.0);
  addParameter("Max_nBinary", 1000.0);
  addParameter("useRecentering",      true);
  addParameter("useNucleonExclusion", false);
  addParameter("UseParticles",        true);
  addParameter("HistogramsCreate",    true);
  addParameter("HistogramsExport",      true);
  addParameter("EventsUseStream0",     true);
}

void CollisionGeometryAnalyzer::createHistograms()
{
  
  if (reportStart(__FUNCTION__))
    ;
  String prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();

  if (reportInfo(__FUNCTION__))
    {
    cout << "Creating HistogramGroup for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    }
  histogramManager.addSet("CollisionGeometry");
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    cout << " iEventFilter:"  << iEventFilter << endl;
    String evtFilterName = eventFilters[iEventFilter]->getName();
    cout << " evtFilterName:"  << evtFilterName << endl;
    String histoName     = prefixName;
    histoName += evtFilterName;
    cout << " histoName:"  << histoName << endl;
    CollisionGeometryHistograms * histos = new CollisionGeometryHistograms(this,histoName, configuration);
    histos->createHistograms();
    histogramManager.addGroupInSet(0,histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void CollisionGeometryAnalyzer::importHistograms(TFile & inputFile)
{
  
  if (reportStart(__FUNCTION__))
    ;
  String prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  if (reportInfo(__FUNCTION__))
    {
    cout << "Loading HistogramGroup for.."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    }
  histogramManager.addSet("CollisionGeometry");
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    String evtFilterName = eventFilters[iEventFilter]->getName();
    String histoName     = prefixName;
    histoName += evtFilterName;
    CollisionGeometryHistograms * histos = new CollisionGeometryHistograms(this,histoName, configuration);
    histos->importHistograms(inputFile);
    histogramManager.addGroupInSet(0,histos);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void CollisionGeometryAnalyzer::analyzeEvent()
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
      CollisionGeometryHistograms * histos = (CollisionGeometryHistograms *)  histogramManager.getGroup(0,iEventFilter);
      histos->fill(event,1.0);
      }
    else
      {
      CollisionGeometryHistograms * histos = (CollisionGeometryHistograms *) histogramManager.getGroup(0,iEventFilter);
      histos->noFill(event,1.0);
      }
    }
  if (reportEnd(__FUNCTION__))
    ;
}
