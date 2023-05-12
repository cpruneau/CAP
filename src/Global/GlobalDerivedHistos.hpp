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
#ifndef CAP__GlobalDerivedHistos
#define CAP__GlobalDerivedHistos
#include "HistogramGroup.hpp"
#include "ParticleFilter.hpp"
#include "GlobalHistos.hpp"

namespace CAP
{


class GlobalDerivedHistos : public HistogramGroup
{
public:

  GlobalDerivedHistos(Task * _parent,
                      const String & _name,
                      const Configuration & _configuration,
                      vector<ParticleFilter*> _particleFilters);
  virtual ~GlobalDerivedHistos() {} 
  virtual void createHistograms();
  virtual void importHistograms(TFile & inputFile);
  virtual void calculateDerivedHistograms(GlobalHistos* baseHistos);

protected:

  vector<ParticleFilter*> particleFilters;

  vector<TH1*>  h_ptAvgInc;    //! Inclusive average pT vs multiplicity computed from ratio of ptSum profile average by average number of particles at given mul
  vector<TH1*>  h_ptAvgIncVsN; //! Inclusive average pT vs multiplicity computed from ratio of ptSum profile average by average number of particles at given mul

  ClassDef(GlobalDerivedHistos,0)

};

}

#endif /* CAP__GlobalDerivedHistos  */



