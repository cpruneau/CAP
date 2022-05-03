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
#ifndef CAP__Histograms
#define CAP__Histograms
#include "HistogramCollection.hpp"
#include "Configuration.hpp"

class Histograms : public HistogramCollection
{
public:
  
  Histograms(const TString & name,
             const Configuration & configuration,
             LogLevel  debugLevel);
//  Histograms(TFile * inputFile,
//             const TString & name,
//             const Configuration & configuration,
//             LogLevel  debugLevel);
  ~Histograms() {}

  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void createDerivedHistograms();
  virtual void createDerivedHistograms(const Histograms & histograms);

  
  //!
  //! Returns the configuration of this histogram set
  //!
  const Configuration & getConfiguration() const
  {
    return configuration;
  }
  
  //!
  //! Returns the configuration of this histogram set
  //!
  Configuration & getConfiguration()
  {
    return configuration;
  }

  //!
  //! Sets the configuration of this task instance.
  //!
  void setConfiguration(const Configuration & config)
  {
    configuration = config;
  }
  
  virtual TString getHistoBaseName() const;

  TString makeName(const TString & baseName,const  TString & filterName1);
  TString makeName(const TString & baseName,const  TString & filterName1,const  TString & observableName1);
  TString makeName(const TString & baseName,const  TString & filterName1,const  TString & observableName1,const  TString & filterName2,const  TString & observableName2);
  TString makeName(const TString & baseName, int i1, const TString suffix="");
  TString makeName(const TString & baseName, int i1, int i2, const TString suffix="");
  TString makeName(const TString & baseName, int i1, int i2, int i3, const TString suffix="");
  TString makeName(const TString & baseName, int i1, int i2, int i3, int i4, const TString suffix="");
 
  
protected:
  
  Configuration configuration;

  ClassDef(Histograms,0)
};

#endif /* CAP__Histograms */
