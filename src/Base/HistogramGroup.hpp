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
#include "NameManager.hpp"

namespace CAP
{

class Task;

class HistogramGroup : public HistogramCollection
{
public:
  
  HistogramGroup(Task * _parent,
             const String & _name,
             const Configuration & _configuration);

  ~HistogramGroup() {}

  virtual void createHistograms();
  virtual void importHistograms(TFile & inputFile);
  
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
  const Configuration & getConfiguration()
  {
    return configuration;
  }

  //!
  //! Sets the configuration of this task instance.
  //!
  void setConfiguration(const Configuration & config)
  {
    configuration.addParameters(config);
  }
  
  Task * getParentTask() const;
  const String getParentName() const;
  const String getParentPathName() const;

protected:

  Task * parent;
  Configuration configuration;

  ClassDef(HistogramGroup,0)
};

} // namespace CAP

#endif /* CAP__Histograms */
