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
#ifndef CAP__PTHistos
#define CAP__PTHistos
#include "HistogramGroup.hpp"
#include "Configuration.hpp"
#include "TMath.h"
#include <cstdlib>
#include <cmath>
#include <chrono>

namespace CAP
{


class PTHistos : public HistogramGroup
{
public:

  PTHistos(Task *          _parent,
           const String & _name,
           const Configuration & _configuration,
           Severity        _debugLevel,
           int             _ord);
  virtual ~PTHistos();

  virtual void createHistograms();
  virtual void importHistograms(TFile & inputFile);
  virtual void fillDerivedHistos(bool *** acceptances, double * mults, double * cents, int * numParticles, double ** pT);
  virtual void exportHistograms(TFile & outputFile, bool saveAll=false);
  virtual void createHistogramRec(String * baseName, String * baseTitle, int depth, int partIndex);
  virtual void loadHistogramRec(String * baseName, int depth, int partIndex, TFile * inputFile);
  virtual void fillEventHistos(double mult, double cent, double weight);
  virtual void fillNormalizedPTValues( int depth, int partIndex, double product, double * SValues, double  mult, double  cent);
  virtual void fillNormalizedPTValues( int depth, int partIndex, double product, TH1 *** values, int* reorder, int*  nBin);
  virtual void calculateCumulants(TProfile ** Shistos, TH1 **CHistos, int nBins, double min, double max);
  virtual void calcRecSum(TH1 **CHistos, int iBin, double& absESq, double curRelESq, int* iHisto, int* Subset, int len,  int * set, int lenSet, double productC, double* used, int& curInd, int productS, double& sum);
  virtual void calculateInclusivePtAverage(bool *** acceptances, int * numParticles, double ** pT);
  virtual void calculateInclusiveYieldsAverage(bool *** acceptances, int * numParticles);
  virtual void calculateEventMoments(bool *** acceptances, int * numParticles, double ** pT);

  ////////////////////////////////////////////////////////////////////////////
  //Helper Functions
  ////////////////////////////////////////////////////////////////////////////

  virtual int convert(int * num, int len);
  virtual int* convert(int num, int & len);
  virtual void convertToBinary(int num, char*str, int len );
  virtual int* getSubset(char* subset, int * set, int len, int& lenSub);
  virtual int* getComplementarySubset(char* subset, int * set, int len, int& lenSub);
  virtual int getSubsetNumber(int * subset, int lenSub, int * mainset, int lenSet);



  ////////////////////////////////////////////////////////////////////////////
  // Data Members - HistogramGroup
  ////////////////////////////////////////////////////////////////////////////
  // S is the pT deviation moments
  // s are the normalized moments
  // s* are the moments normalized by average pT's
  // C is the cumulants
  // c are the normalized cumulants
  // c* are the cumulants normalizd by average pT's

  //maximum order of correlation functions (2,3,4 ..) 
  //order is also equal to number of particle filters in PTCorrelator
  int maxOrder;

  int histoIndex;

  //size of   TProfile ** h_c array
  int size; 

  //keep track of the orders of the correlation functions in the histograms
  int * orders;

  // Min bias all included NOT IN ORDER (They are in "recursive order")
  // in the order S, s, s*, C, c, 
  TH1 * h_eventStreams;
  //first index is function index, second index is histo index
  TProfile *** hS;
  TH1 *** hC;



  // vs Mult measured in fiducial NOT IN ORDER (They are in "recursive order")
  // in the order S, s, s*, C, c, 
  TH1 * h_eventStreams_vsMult;
  //first index is function index, second index is histo index
  TProfile *** hS_vsMult;
  TH1 *** hC_vsMult;


  // vs Centrality NOT IN ORDER (They are in "recursive order")
  // in the order S, s, s*, C, c, c*
  TH1 * h_eventStreams_vsCent;
  //first index is function index, second index is histo index
  TProfile *** hS_vsCent;
  TH1 *** hC_vsCent;


  //number of pairs, triples .. NOT IN ORDER (They are in "recursive order")
  TProfile ** h_counts;
  TProfile ** h_counts_vsMult;
  TProfile ** h_counts_vsCent;

  //number of functions we are counting 
  // here it is 6 (S, s, s*, C, c, c*)
  int numFunc;

  //keep track of which index in "recursive order" corresponds to which index in normal order
  int * reorder;

  int totEvents;

  //store the inclusive avgpT of each of the particle filters
  double * avgpT; 
  
  //store the inclusive average yields of each combination of particle filters (1, 2 .. 4, 11, 12, .. 44 ..)
  //calculated in normal order, but then changed into recursive order.
  double * avgCounts;

  //store the yields of each combination of particle filters (1, 2 .. 4, 11, 12, .. 44 ..) per event
  //first index is event number
  //calculated in normal order, but then changed into recursive order.
  int ** counts;

  //store the moments of each combination of particle filters (1, 2 .. 4, 11, 12, .. 44 ..) per event
  //first index is event number
  //calculated in normal order, but then changed into recursive order.
  double ** SValues;

  TString** names;

TString* *titles;

TString** names2;

TString* *titles2;

  ClassDef(PTHistos,0)
};

} // namespace CAP

#endif /* CAP__PTHistos  */
