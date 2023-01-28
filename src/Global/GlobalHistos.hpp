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
#include "HistogramGroup.hpp"
#include "ParticleFilter.hpp"
#include "Configuration.hpp"

namespace CAP
{

//!
//!
//! HistogramGroup of "global" observables.
//!
//! Observables are computed in the acceptance defined by
//! particle filters for each of the selected event filters. The particle filters can be set to select narrow
//! or broad ranges of kinematic variables as well as species
//! of particles.
//!
//! n:  number of selected particles in an  event
//! e:  total energy of the selected particles in an event
//! q:  total (net) electric charge of the selected particles in an event
//! b:  total (net) baryon charge of the selected particles in an event
//!
//! The fill method of this class must be called at most once per event
//! otherwise weird multiple counting will happen..
//!
class GlobalHistos : public HistogramGroup
{
public:

  //!
  //!Generic/standard CTOR
  //!
  GlobalHistos(Task * _parent,
               const String & _name,
               Configuration & _configuration,
               vector<ParticleFilter*> _particleFilters);

  //!
  //!DTOR
  //!
  //!Ownership of the histograms is given to root. Arrays of histogram pointers are cleared but the deletion of the histograms
  //!themselves is left to root.
  //!
  virtual ~GlobalHistos() {}

  //!
  //!Creating histograms corresponding to the selected event and particle filters from the given file.
  //!
  void createHistograms();

  //!
  //!Load histograms corresponding to the selected event and particle filters from the given file.
  //!
  void loadHistograms(TFile * inputFile);

  //!
  //!Fill eventswise observables for the selected event filters and particle filters
  //!
  //! @param n  number of particles accepted
  //! @param pTsum total transverse momentum (pT) of  accepted particles
  //! @param e  total energyof  accepted particles
  //! @param q  net charge of  accepted particles
  //! @param b net baryon number of  accepted particles
  //!
  //! By default, only histograms of single particle quantities are filled. These include the number of particles (multiplicity), the pt sum,
  //! the total energy, the net charge, the net strangeness, and the net baryon number. Scatter plot histograms of observable correlations
  //! are also filled including for instance ptSum vs multiplicity, total E vs multiplicity, net charge vs multiplicity, and so on. An eventwise  pt average
  //! histogram is also filled (event by event), for all event and particle filters, when the selected mulitplicity in non zero.
  //!
  //! If the bool variable fillCorrelationHistos is set to true, cross-species histograms are also filled including for instance the multiplicity selected by
  //! particle filter k1 and the multiplicity selected by filter k2.
  //!
  void fill(vector<double> & n, vector<double> & ptSum, vector<double> & e, vector<double> & q, vector<double> & s, vector<double> & b, double weight);

protected:

  vector<ParticleFilter*> particleFilters;
  vector<TH1*>  h_n;
  vector<TH1*>  h_e;
  vector<TH1*>  h_q;
  vector<TH1*>  h_s;
  vector<TH1*>  h_b;
  vector<TH1*>  h_ptSum;
  vector<TH1*>  h_ptAvg;

  vector<TProfile*>  p_eVsN;
  vector<TProfile*>  p_qVsN;
  vector<TProfile*>  p_sVsN;
  vector<TProfile*>  p_bVsN;
  vector<TProfile*>  p_ptSumVsN;
  vector<TProfile*>  p_ptAvgVsN;
  

  vector<TH2*>  h_nVsN;
  vector<TH2*>  h_eVsN;
  vector<TH2*>  h_qVsN;
  vector<TH2*>  h_bVsN;
  vector<TH2*>  h_ptSumVsN;
  vector<TH2*>  h_ptAvgVsN;

  vector<TH2*>  h_eVsE;
  vector<TH2*>  h_qVsE;
  vector<TH2*>  h_bVsE;
  vector<TH2*>  h_qVsQ;
  vector<TH2*>  h_bVsQ;
  vector<TH2*>  h_bVsB;

  bool fill2D;
  bool fillCorrelationHistos;
  
  ClassDef(GlobalHistos,0)
};

} // namespace CAP

#endif /* CAP__GlobalHistos  */



