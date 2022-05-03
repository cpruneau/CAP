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
#ifndef CAP__ParticlePairCombinedHistogramCalculator
#define CAP__ParticlePairCombinedHistogramCalculator
#include "Task.hpp"
#include "ParticleHistos.hpp"
#include "ParticlePairHistos.hpp"
#include "ParticlePairDerivedHistos.hpp"
#include "ParticlePairCombinedHistos.hpp"

class ParticlePairCombinedHistogramCalculator : public Task
{
public:

  ParticlePairCombinedHistogramCalculator(const TString &         _name,
                                          Configuration &         _configuration,
                                          vector<EventFilter*>    _eventFilters,
                                          vector<ParticleFilter*> _particleFilters,
                                          LogLevel                _selectedLevel);
  virtual ~ParticlePairCombinedHistogramCalculator() {}
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void calculateCombinedHistograms(ParticlePairDerivedHistos * derivedHistos);

protected:

   ClassDef(ParticlePairCombinedHistogramCalculator,0)
};


#endif /* CAP__ParticlePairCombinedHistogramCalculator */
