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
#include "HistogramManager.hpp"

//using CAP::HistogramManager;

ClassImp(CAP::HistogramManager);

CAP::HistogramManager::HistogramManager()
:
names(),
sets()
{}

//!
//! CTOR. Allocate resources for nSets=names.size() with the given names. If the given array of name is null, no error is
//! generated, but no space is allocated.
//!
CAP::HistogramManager::HistogramManager(const vector<String> _names)
:
names(_names),
sets(_names.size())
{}


void CAP::HistogramManager::addSet(const String & name)
{
  if (name.Length()!=0)
    {
    names.push_back(name);
    sets.push_back( HistogramSet() );
    }
}

void CAP::HistogramManager::addGroupInSet(unsigned int index, HistogramGroup * group)
{
  unsigned int nSets = sets.size();
  if (index>=nSets)
    {
    // do nothing
    }
  sets[index].push_back(group);
}

//!
//!Delete all sets and their respective contents
//!
void CAP::HistogramManager::clear()
{
  for (unsigned int iSet=0; iSet<sets.size(); iSet++)
    {
    for (unsigned int iGroup=0; iGroup<sets[iSet].size(); iGroup++)
      {
      delete sets[iSet][iGroup];
      }
    }
  sets.clear();
  names.clear();
}

//!
//!Reset histograms of all sets and all groups they contain
//!
void CAP::HistogramManager::reset()
{
  for (unsigned int iSet=0; iSet<sets.size(); iSet++)
    {
    for (unsigned int iGroup=0; iGroup<sets[iSet].size(); iGroup++)
      {
      sets[iSet][iGroup]->reset();
      }
    }
}

//!
//!Save histograms of all sets and all groups they contain
//!
void CAP::HistogramManager::save(TFile & outputFile)
{
  for (unsigned int iSet=0; iSet<sets.size(); iSet++)
    {
    for (unsigned int iGroup=0; iGroup<sets[iSet].size(); iGroup++)
      {
      sets[iSet][iGroup]->exportHistograms(outputFile);
      }
    }
}

void CAP::HistogramManager::save(ofstream & outputFile)
{
  for (unsigned int iSet=0; iSet<sets.size(); iSet++)
    {
    for (unsigned int iGroup=0; iGroup<sets[iSet].size(); iGroup++)
      {
      sets[iSet][iGroup]->exportHistograms(outputFile);
      }
    }
}





//!
//!Load histograms of all sets and all groups they contain
//!
void CAP::HistogramManager::load(TFile & outputFile)
{
  for (unsigned int iSet=0; iSet<sets.size(); iSet++)
    {
    for (unsigned int iGroup=0; iGroup<sets[iSet].size(); iGroup++)
      {
      sets[iSet][iGroup]->importHistograms(outputFile);
      }
    }
}

//!
//!Scale histograms of all sets and all groups they contain by the given factor
//!
void CAP::HistogramManager::scale(double scalingFactor)
{
  for (unsigned int iSet=0; iSet<sets.size(); iSet++)
    {
    for (unsigned int iGroup=0; iGroup<sets[iSet].size(); iGroup++)
      {
      sets[iSet][iGroup]->scale(scalingFactor);
      }
    }
}
