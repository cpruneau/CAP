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
#ifndef CAP__HistogramManager
#define CAP__HistogramManager
#include "HistogramGroup.hpp"

namespace CAP
{

//!
//!HistogramManager manages several sets (vectors) of HistogramGroup objects.
//!Each set is an array (implemented as a vector) of Histogram groups. This enables
//!the management of several groups for each set and the managerment of several sets.
//!Example: In single particle analyses, one can have basic and derived histogram groups. The groups
//!can be design to work with specific collision centrality and particle species. It would then be
//!convenient to have two sets: basic and derived, with the same number of groups, i.e., with a one-to-one mapping
//!of the basic and derived groups, for an arbitrary number of collision centralities and particle species.
//!Evidently, other clustering can also be used. This class is typically used by Analyzer type classes to
//!facilitate the mangement of histogram groups.
//!
class HistogramManager
{
protected:

  using HistogramSet = vector<HistogramGroup*>;

  vector<String> names;
  vector<HistogramSet> sets;

public:

  //!
  //! Default constructor. No resource allocation is performed.
  //!
  HistogramManager();

  //!
  //! CTOR. Allocate resources for nSets=names.size() with the given names. If the given array of name is null, no error is
  //! generated, but no space is allocated.
  //!
  HistogramManager(const vector<String> _names);

  virtual ~HistogramManager() {}

  void addSet(const String & name);

  void addGroupInSet(unsigned int index, HistogramGroup * group);

  //!
  //!Delete all sets and their respective contents
  //!
  void clear();

  //!
  //!Reset histograms of all sets and all groups they contain
  //!
  void reset();

  //!
  //!Save histograms of all sets and all groups they contain
  //!
  void save(TFile & outputFile);

  //!
  //!Save histograms of all sets and all groups they contain
  //!
  void save(ofstream & outputFile);

  
  //!
  //!Load histograms of all sets and all groups they contain
  //!
  void load(TFile & outputFile);

  //!
  //!Scale histograms of all sets and all groups they contain by the given factor
  //!
  void scale(double scalingFactor);

  inline int getNSets()
  {
  return sets.size();
  }

  inline HistogramSet getSet(unsigned int iSet)
  {
  return sets[iSet];
  }

  inline HistogramGroup * getGroup(unsigned int iSet, unsigned int iGroup)
  {
  return sets[iSet][iGroup];
  }

  ClassDef(HistogramManager,0)
};

} // namespace CAP

#endif /* CAP__HistogramManager */
