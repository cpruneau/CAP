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
ClassImp(Histograms);

Histograms::Histograms(const TString &       _name,
                       const Configuration & _configuration,
                       LogLevel              _debugLevel)
:
HistogramCollection(_name,_debugLevel),
configuration(_configuration)
{
  
  appendClassName("Histograms");
  setInstanceName(_name);
}

//Histograms(TFile * inputFile,
//           const TString & name,
//           const Configuration & configuration,
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



TString Histograms::getHistoBaseName() const
{
//  Configuration & ac = *(Configuration*) getConfiguration();
  TString bn; //ac.histoBaseName;
  bn = getName();
  return bn;
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
