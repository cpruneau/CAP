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
#ifndef CAP__BalanceFunctionCalculator
#define CAP__BalanceFunctionCalculator
#include "Task.hpp"
using namespace std;

class BalanceFunctionCalculator : public Task
{
public:

  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle filters to be used by this task
  //! @param _sObservableNames Array containing the names of the single particle  observables involved in the balance function calculation
  //! @param _pObservableNames Array containing the names of the pair  observables involved in the balance function calculation
  //! @param _reportLevel Message log level to be used by this task.
  //!
  BalanceFunctionCalculator(const TString &          _name,
                            const Configuration &    _configuration,
                            vector<EventFilter*> &   _eventFilters,
                            vector<ParticleFilter*>& _particleFilters,
                            vector<TString> &        _sObservableNames,
                            vector<TString> &        _pObservableNames,
                            MessageLogger::LogLevel  _reportLevel);
  //!
  //! DTOR
  //!
  virtual ~BalanceFunctionCalculator() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();
  
  
  //!
  //! Execute this task based on the configuration and class variable specified at construction
  //!
  virtual void execute();

  //!
  //!Calculate the "charge independent" combination of the given observable defined as
  //!\verbatim
  //! obs_CI = 0.25*(obs_1_2 + obs_1Bar_2 + obs_1_2Bar + obs_1Bar_2Bar)
  //!\endverbatim
  //!in which 1 and 2 represent the first and second particles respectively, and Bar means anti-particle.
  //!
  //!@param histoBaseName Base name of the histograms (should usually correspond to the pair label
  //!@param eventClassName Name of the event filter used to select the events used to fill the given histograms
  //!@param particleName1 Name of the first particle filter used to select particles used to fill the given histograms
  //!@param particleName2 Name of the second particle filter used to select particles used to fill the given histograms
  //!@param obsName Name of the observable being computed
  //!@param obs_1_2  Pointer to histogram of the observable of interest for particle 1 and 2
  //!@param obs_1Bar_2 Pointer to histogram of the observable of interest for particle 1Bar and 2
  //!@param obs_1_2Bar Pointer to histogram of the observable of interest for particle 1 and 2Bar
  //!@param obs_1Bar_2Bar Pointer to histogram of the observable of interest for particle 1Bar and 2Bar
  //!
  virtual TH2* calculate_CI(const TString & histoBaseName,
                            const TString & eventClassName,
                            const TString & particleName1,
                            const TString & particleName2,
                            const TString & obsName,
                            TH2* obs_1_2, TH2* obs_1Bar_2, TH2* obs_1_2Bar, TH2* obs_1Bar_2Bar);

  //!
  //!Calculate the "charge dependent" combination of the given observable defined as
  //!\verbatim
  //! obs_CI = 0.5*(obs_1_2 - obs_1Bar_2 + obs_1_2Bar - obs_1Bar_2Bar)
  //!\endverbatim
  //!in which 1 and 2 represent the first and second particles respectively, and Bar means anti-particle.
  //!
  //!@param histoBaseName Base name of the histograms (should usually correspond to the pair label
  //!@param eventClassName Name of the event filter used to select the events used to fill the given histograms
  //!@param particleName1 Name of the first particle filter used to select particles used to fill the given histograms
  //!@param particleName2 Name of the second particle filter used to select particles used to fill the given histograms
  //!@param obsName Name of the observable being computed
  //!@param obs_1_2  Pointer to histogram of the observable of interest for particle 1 and 2
  //!@param obs_1Bar_2 Pointer to histogram of the observable of interest for particle 1Bar and 2
  //!@param obs_1_2Bar Pointer to histogram of the observable of interest for particle 1 and 2Bar
  //!@param obs_1Bar_2Bar Pointer to histogram of the observable of interest for particle 1Bar and 2Bar
  //!
  virtual TH2* calculate_CD(const TString & histoBaseName,
                            const TString & eventClassName,
                            const TString & particleName1,
                            const TString & particleName2,
                            const TString & obsName,
                            TH2* obs_1_2,
                            TH2* obs_1Bar_2,
                            TH2* obs_1_2Bar,
                            TH2* obs_1Bar_2Bar);

  //!
  //!Calculate the "balance function" combination of the given observable defined as
  //!\verbatim
  //! obs_BalFct = (obs_US - obs_LS )/int(rho1_2)f
  //!\endverbatim
  //!in which US and LS represent unlike-signs (unlike baryon numbers, etc) and like-sign (same baryon number sign)
  //!
  //!@param histoBaseName Base name of the histograms (should usually correspond to the pair label
  //!@param eventClassName Name of the event filter used to select the events used to fill the given histograms
  //!@param particleName1 Name of the first particle filter used to select particles used to fill the given histograms
  //!@param particleName2 Name of the second particle filter used to select particles used to fill the given histograms
  //!@param obsName Name of the observable being computed
  //!@param comboName Name of given to the combo
  //!@param obs_US  Pointer to histogram of the observable of interest for a US pair
  //!@param obs_LS Pointer to histogram of the observable of interest for LS pair
  //!
  virtual TH2* calculate_BalFct(const TString & histoBaseName,
                                const TString & eventClassName,
                                const TString & particleName1,
                                const TString & particleName2,
                                const TString & obsName,
                                const TString & comboName,
                                TH1* rho1_2,
                                TH2* obs_US,
                                TH2* obs_LS);

  virtual TH2* calculate_BalFct2(const TString & histoBaseName,
                                 const TString & eventClassName,
                                 const TString & particleName1,
                                 const TString & particleName2,
                                 const TString & obsName,
                                 const TString & comboName,
                                 TH1* rho1_1,
                                 TH1* rho1_2,
                                 TH2* obs_US,
                                 TH2* obs_LS);

  virtual TH2* calculate_BalFct3(const TString & histoBaseName,
                                 const TString & eventClassName,
                                 const TString & particleName1,
                                 const TString & particleName2,
                                 const TString & obsName,
                                 const TString & comboName,
                                 TH1* rho1_2,
                                 TH2* obs_US,
                                 TH2* obs_LS);

  virtual TH2* calculate_BalFctSum(const TString & histoBaseName,
                                    const TString & eventClassName,
                                    const TString & particleName1,
                                    const TString & particleName2,
                                    const TString & obsName,
                                    const TString & comboName,
                                    TH2* obs_12Bar,
                                    TH2* obs_1Bar2);

  //!
  //!Calculate the "difference" combination of the given observable defined as
  //!\verbatim
  //! obs_Diff =  (obs_first - obs_second)
  //!\endverbatim
  //!in which obs_first and obs_second represent two distinc pair combinations of the given observable. This function is useful to
  //!compute differences of pair histograms such as obs_1Bar_2 and obs_1_2Bar which is expected to be null in the context
  //!of particle production at very large sqrt(s). For instance, n2(proton,kaon-) could be very similar to n2(proton-bar,kaon+).
  //!
  //!@param histoBaseName Base name of the histograms (should usually correspond to the pair label
  //!@param eventClassName Name of the event filter used to select the events used to fill the given histograms
  //!@param particleName1 Name of the first particle filter used to select particles used to fill the given histograms
  //!@param particleName2 Name of the second particle filter used to select particles used to fill the given histograms
  //!@param obsName Name of the observable being computed
  //!@param comboName Name of given to the combo
  //!@param obs_first  Pointer to histogram of the observable of the first pair
  //!@param obs_second Pointer to histogram of the observable of the second pair
  //!
  virtual TH2* calculate_Diff(const TString & histoBaseName,
                              const TString & eventClassName,
                              const TString & particleName1,
                              const TString & particleName2,
                              const TString & obsName,
                              const TString & comboName,
                              TH2* obs_first,
                              TH2* obs_second);


protected:
  
  //!
  //! Array containing the names of the single particle  observables involved in the balance function calculation
  //!
  vector<TString> & sObservableNames;

  //!
  //! Array containing the names of the pair observables involved in the balance function calculation
  //!
  vector<TString> & pObservableNames;

  ClassDef(BalanceFunctionCalculator,0)
};

#endif /* CAP__BalanceFunctionCalculator */
