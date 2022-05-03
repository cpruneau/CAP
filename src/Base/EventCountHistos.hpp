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
#ifndef CAP__EventCountHistos
#define CAP__EventCountHistos
#include "Histograms.hpp"
#include "Particle.hpp"
#include "Configuration.hpp"
#include "TLorentzVector.h"


class EventCountHistos : public Histograms
{
public:

  EventCountHistos(const TString & _collectionName,
                   Configuration & _configuration,
                   LogLevel        _debugLevel);
  virtual ~EventCountHistos(){}
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void fill(vector<double> eventCounts);

  // //////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  // //////////////////////////////////////////////////////////////////////////
  TH1 * h_eventCounts;

    ClassDef(EventCountHistos,0)

};

#endif /* CAP__EventCountHistos  */



