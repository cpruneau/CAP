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
class Task;

class Histograms : public HistogramCollection
{
public:
  
  Histograms(Task * _parent,
             const TString & _name,
             Configuration & _configuration);

  ~Histograms() {}

  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  
  //!
  //! Returns the configuration of this histogram set
  //!
  Configuration & getConfiguration() const
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
  void setConfiguration(Configuration & config)
  {
    configuration = config;
  }
  

  TString makeName(const TString & s0, const  TString & s1);
  TString makeName(const TString & s0, const  TString & s1, const TString & s2);
  TString makeName(const TString & s0, const  TString & s1, const TString & s2, const TString & s3);
  TString makeName(const TString & s0, const  TString & s1, const TString & s2, const TString & s3, const TString & s4);

  TString makeName(const TString & s0, int i1, const TString suffix="");
  TString makeName(const TString & s0, int i1, int i2, const TString suffix="");
  TString makeName(const TString & s0, int i1, int i2, int i3, const TString suffix="");
  TString makeName(const TString & s0, int i1, int i2, int i3, int i4, const TString suffix="");

  TString makeName(const TString & s0, const TString & s1, int i1, const TString suffix);
  TString makeName(const TString & s0, const TString & s1, int i1, int i2, const TString suffix);
  TString makeName(const TString & s0, const TString & s1, int i1, int i2, int i3, const TString suffix);
  TString makeName(const TString & s0, const TString & s1, int i1, int i2, int i3, int i4, const TString suffix);

  Task * getParentTask() const;
  const TString getParentTaskName() const;
  const TString getParentPathName() const;

protected:

  Task * parent;
  Configuration & configuration;

  ClassDef(Histograms,0)
};

#endif /* CAP__Histograms */
