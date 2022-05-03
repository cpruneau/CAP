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
#ifndef CAP__GlobalHistos
#define CAP__GlobalHistos
#include "Histograms.hpp"
#include "ParticleFilter.hpp"
#include "Configuration.hpp"

// ==================================================
//
// Histograms of "global" observables.
//
// Observables are computed in the acceptance defined by
// particle filters. The filters can be set to select narrow
// or broad ranges of kinematic variables as well as species
// of particles.
//
// n:  number of selected particles in an  event
// e:  total energy of the selected particles in an event
// q:  total (net) electric charge of the selected particles in an event
// b:  total (net) baryon charge of the selected particles in an event
//
// The fill method of this class must be called at most once per event
// otherwise weird multiple counting will happen...
//
// ==================================================
class GlobalHistos : public Histograms
{
public:

  GlobalHistos(const TString &         _name,
               const Configuration &   _configuration,
               vector<ParticleFilter*> _particleFilters,
               LogLevel                _debugLevel);
  virtual ~GlobalHistos() {}
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  void fill(vector<double> & n, vector<double> & e, vector<double> & q, vector<double> & s, vector<double> & b, double weight);

protected:

  vector<ParticleFilter*> particleFilters;
  vector<TH1*>  h_n;
  vector<TH1*>  h_e;
  vector<TH1*>  h_q;
  vector<TH1*>  h_s;
  vector<TH1*>  h_b;
  vector<TH2*>  h_nVsN;
  vector<TH2*>  h_eVsN;
  vector<TH2*>  h_qVsN;
  vector<TH2*>  h_bVsN;
  vector<TH2*>  h_eVsE;
  vector<TH2*>  h_qVsE;
  vector<TH2*>  h_bVsE;
  vector<TH2*>  h_qVsQ;
  vector<TH2*>  h_bVsQ;
  vector<TH2*>  h_bVsB;

  bool fillCorrelationHistos;
  
  ClassDef(GlobalHistos,0)
};

#endif /* CAP__GlobalHistos  */



