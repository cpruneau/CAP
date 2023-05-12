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
#ifndef CAP__ParticlePerformanceSimulator
#define CAP__ParticlePerformanceSimulator
#include "HistogramGroup.hpp"

namespace CAP
{

class EfficiencyFunction;
class ResolutionFunction;
//!
//!Smears the momentum of particles  based on user defined configuration parameters provided at construction
//!of instances of this class.
//!
//!This class assumes the particles detected are either charged electrons, muons, pions, kaons, or protons (or either
//!positive or negative charge). The following (arbitrary) identifiers are used internally to select which histograms or
//!functions are used to compute resolution parameters (bias and rms) of particles. Subclass this class to add
//!particle types as needed. 
//!
//!
class ParticlePerformanceSimulator : public HistogramGroup
{
public:
  
  ParticlePerformanceSimulator(Task * _parent,
                               int _filterIndex,
                               const String & _name,
                               const Configuration & _configuration);
  virtual ~ParticlePerformanceSimulator(){}
  
  virtual void initialize();
  virtual void importHistograms(TFile & inputFile);

  virtual void smearMomentum(const LorentzVector &in, LorentzVector & out);
  virtual void smearMomentum(double pt, double eta, double phi,
                             double &smearedPt, double &smearedEta, double &smearedPhi);
  virtual void smearFromHisto(double pt, double eta, double phi,
                              TH1 * biasHisto, TH1 * rmsHisto,
                              double & bias, double & rms);
  virtual void smearFromFunction(double pt, double eta, double phi,
                                 ResolutionFunction* f,
                                 double & bias, double & rms);
  
  bool accept(const LorentzVector& momentum);
  bool accept(double pt, double eta, double phi);
  bool acceptFromHisto(double pt, double eta, double phi);
  bool acceptFromFunction(double pt, double eta, double phi);
  
  
protected:
  
  int  filterIndex;
  bool useSameSetForAll;
  int  resolutionOption;
  int  efficiencyOption;
  
  TH1 *biasPtHistogram;
  TH1 *rmsPtHistogram;
  TH1 *biasEtaHistogram;
  TH1 *rmsEtaHistogram;
  TH1 *biasPhiHistogram;
  TH1 *rmsPhiHistogram;
  TH1 *efficienyHistogram;
  
  
  ResolutionFunction* ptFunction;
  ResolutionFunction* etaFunction;
  ResolutionFunction* phiFunction;
  EfficiencyFunction* efficiencyFunction;
  
  ClassDef(ParticlePerformanceSimulator,0)
};



class EfficiencyFunction
{
public:
  
  EfficiencyFunction(double _peakAmp,
                     double _peakPt,
                     double _peakRms,
                     double _a1,
                     double _a2)
  :
  peakAmp(_peakAmp),
  peakPt(_peakPt),
  peakRms(_peakRms),
  a1(_a1),
  a2(_a2)
  {
  
  }
  
  ~EfficiencyFunction() {}
  
  virtual  double getEfficiency(double pt, double eta __attribute__((unused)), double phi __attribute__((unused)))
  {
  double eff;
  double arg1 = pt-peakPt;
  if (pt<peakPt)
    {
    double arg2 = arg1/peakRms;
    eff = peakAmp*exp(-arg2*arg2/2.0);
    }
  else
    {
    eff = peakAmp + a1*arg1 + a2*arg1*arg1;
    }
  return (eff<1)? eff : 1.0 ;
  }
  
  
protected:
  
  double peakAmp;
  double peakPt;
  double peakRms;
  double a1;
  double a2;
  
  ResolutionFunction *ptFunction;
  ResolutionFunction *etaFunction;
  ResolutionFunction *phiFunction;

};



class ResolutionFunction
{
public:
   
  ResolutionFunction(int    _option,
                     double _aInvBias,
                     double _a0Bias,
                     double _a1Bias,
                     double _a2Bias,
                     double _aInvRms,
                     double _a0Rms,
                     double _a1Rms,
                     double _a2Rms)
  :
  option(_option),
  aInvBias(_aInvBias),
  a0Bias(_a0Bias),
  a1Bias(_a1Bias),
  a2Bias(_a2Bias),
  aInvRms(_aInvRms),
  a0Rms(_a0Rms),
  a1Rms(_a1Rms),
  a2Rms(_a2Rms)
  {
  
  }
  
  ~ResolutionFunction() {}
  
  virtual  double getBias(double pt, double eta, double phi __attribute__((unused)))
  {
  double bias = 0.0;
  switch (option)
    {
      case 0: bias = aInvBias/pt + a0Bias + a1Bias*pt + a2Bias*pt*pt; break;
      case 1: bias = a0Bias + a1Bias*eta + a2Bias*eta*eta; break;
    }
  return bias;
  }
  
  virtual  double getRms(double pt, double eta, double phi __attribute__((unused)))
  {
  double rms = 0.0;
  switch (option)
  {
    case 0: rms = aInvRms/pt + a0Rms + a1Rms*pt + a2Rms*pt*pt; break;
    case 1: rms = a0Rms + a1Rms*eta + a2Rms*eta*eta; break;
  }
  return rms;
  }
  
protected:
  
  bool useSameSetForAll;
  int  resolutionOption;
  int  efficiencyOption;
 
  
  int option;
  double aInvBias;
  double a0Bias;
  double a1Bias;
  double a2Bias;
  
  double aInvRms;
  double a0Rms;
  double a1Rms;
  double a2Rms;
  
};

} // namespace CAP

#endif /* CAP__ParticlePerformanceSimulator */
