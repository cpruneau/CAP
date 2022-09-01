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

#include "Histograms.hpp"
#include "Task.hpp"

ClassImp(Histograms);

Histograms::Histograms(Task * _parent,
                       const TString & _name,
                       Configuration & _configuration)
:
HistogramCollection(_name),
parent(_parent),
configuration(_configuration)
{
  setClassName("Histograms");
  setInstanceName(_name);
}

//Histograms(TFile * inputFile,
//           const TString & _name,
//           Configuration & configuration,
//           LogLevel  debugLevel);

//!
//! Overload this class to create histograms.
//!
void Histograms::createHistograms()
{
  
  if (reportWarning(__FUNCTION__)) cout << "Implement derived class to create histograms." << endl;
}

//!
//! Overload this class to load histograms.
//!
void Histograms::loadHistograms(TFile * inputFile __attribute__((unused)))
{
  
  if (reportWarning(__FUNCTION__)) cout << "Implement derived class to load histograms." << endl;
}


TString Histograms::makeName(const TString & s0, const  TString & s1)
{
  TString name = s0;
  name += "_";
  name += s1;
  return name;
}

TString Histograms::makeName(const TString & s0, const  TString & s1,const  TString & s2)
{
  TString name = s0;
  name += "_";
  name += s1;
  name += "_";
  name += s2;
  return name;
}

TString Histograms::makeName(const TString & s0, const  TString & s1, const  TString & s2, const  TString & s3)
{
  TString name = s0;
  name += "_";
  name += s1;
  name += "_";
  name += s2;
  name += "_";
  name += s3;
  return name;
}

TString Histograms::makeName(const TString & s0, const  TString & s1, const  TString & s2,const  TString & s3,const  TString & s4)
{
  TString name = s0;
  name += "_";
  name += s1;
  name += "_";
  name += s2;
  name += "_";
  name += s3;
  name += "_";
  name += s4;
  return name;
}

TString Histograms::makeName(const TString & s0, const TString & s1, int i1, const TString suffix)
{
  TString name = s0;
  name += "_";
  name += s1;
  name += "_";
  name += i1;
  name += "_";
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & s0, const TString & s1, int i1, int i2, const TString suffix)
{
  TString name = s0;
  name += "_";
  name += s1;
  name += "_";
  name += i1;
  name += i2;
  name += "_";
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & s0, const TString & s1, int i1, int i2, int i3, const TString suffix)
{
  TString name = s0;
  name += "_";
  name += s1;
  name += "_";
  name += i1;
  name += i2;
  name += i3;
  name += "_";
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & s0, const TString & s1, int i1, int i2, int i3, int i4, const TString suffix)
{
  TString name = s0;
  name += "_";
  name += s1;
  name += "_";
  name += i1;
  name += i2;
  name += i3;
  name += i4;
  name += "_";
  name += suffix;
  return name;
}


TString Histograms::makeName(const TString & s0, int i1, const TString suffix)
{
  TString name = s0;
  name += "_";
  name += i1;
  name += "_";
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & s0, int i1, int i2, const TString suffix)
{
  TString name = s0;
  name += i1;
  name += i2;
  name += "_";
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & s0, int i1, int i2, int i3, const TString suffix)
{
  TString name = s0;
  name += "_";
  name += i1;
  name += i2;
  name += i3;
  name += "_";
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & s0, int i1, int i2, int i3, int i4, const TString suffix)
{
  TString name = s0;
  name += "_";
  name += i1;
  name += i2;
  name += i3;
  name += i4;
  name += "_";
  name += suffix;
  return name;
}


Task * Histograms::getParentTask() const
{
  return parent;
}

const TString Histograms::getParentTaskName() const
{
  return parent->getName();
}

const TString Histograms::getParentPathName() const
{
  return parent->getFullTaskPath();
}

