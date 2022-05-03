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
#include "TLorentzVector.h"
#include "EventCountHistos.hpp"
ClassImp(EventCountHistos);

EventCountHistos::EventCountHistos(const TString & _name,
                                   Configuration & _configuration,
                                   LogLevel        _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
h_eventCounts(nullptr)
{
  // no ops
}

// for now use the same boundaries for eta and y histogram
void EventCountHistos::createHistograms()
{
  TString bn = getHistoBaseName();
  h_eventCounts = createHistogram(bn+TString("eventCount"),100,  0.0,  100.0,  "FilterIndex","EventCount");
 }

//________________________________________________________________________
void EventCountHistos::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "Attempting to load EventCountHistos from an invalid file pointer" << endl;
    return;
    }
  TString bn = getHistoBaseName();
  TString histoName = bn+TString("eventCount");
  h_eventCounts = loadH1(inputFile,histoName);
  if (!h_eventCounts)
    {
    if (reportError()) cout << "Could not load histogram: " << histoName << endl;
    return;
    }
}

// This fills the event count histogram. It should be called only
// once per event or once per histo file save.
void EventCountHistos::fill(vector<double> eventCounts)
{
  double x = 0.5;
  for (unsigned int k=0; k<eventCounts.size(); k++)
    {
    h_eventCounts->Fill(x,eventCounts[k]);
    x += 1.0;
    }
}

