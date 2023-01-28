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

#include "HistogramGroup.hpp"
#include "Task.hpp"

using CAP::Task;
using CAP::HistogramGroup;
using CAP::String;
using CAP::Configuration;

ClassImp(HistogramGroup);

HistogramGroup::HistogramGroup(Task * _parent,
                               const String & _name,
                               Configuration & _configuration)
:
HistogramCollection(_name),
parent(_parent),
configuration(_configuration)
{
  setClassName("HistogramGroup");
  setInstanceName(_name);
}

//!
//! Overload this class to create histograms.
//!
void HistogramGroup::createHistograms()
{
  if (reportWarning(__FUNCTION__)) cout << "Implement derived class to create histograms." << endl;
}

//!
//! Overload this class to load histograms.
//!
void HistogramGroup::loadHistograms(TFile * inputFile __attribute__((unused)))
{
  
  if (reportWarning(__FUNCTION__)) cout << "Implement derived class to load histograms." << endl;
}


Task * HistogramGroup::getParentTask() const
{
  return parent;
}

const String HistogramGroup::getParentTaskName() const
{
  return parent->getName();
}

const String HistogramGroup::getParentPathName() const
{
  return parent->getFullTaskPath();
}

