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
//  bn += "_";
  bn = getName();
  bn += "_";
  return bn;
}

//!
//! Overload this class to calculated derived histograms based on members of primary histogram of this class.
//!
void Histograms::createDerivedHistograms()
{
  setFunctionName("createDerivedHistograms()");
  if (reportWarning(__FUNCTION__)) cout << "Implement derived class to calculate derived histograms." << endl;
}

//!
//! Overload this class to calculated derived histograms based on members of the give primary histogram class instance.
//!
void Histograms::createDerivedHistograms(const Histograms & histograms __attribute__((unused)))
{
  setFunctionName("createDerivedHistograms(const Histograms & histograms)");
  if (reportWarning(__FUNCTION__)) cout << "Implement derived class to calculate derived histograms from external source..." << endl;
}

TString Histograms::makeName(const TString & baseName,const  TString & filterName1)
{
  TString name = baseName;
  name += "_";
  name += filterName1;
  return name;
}

TString Histograms::makeName(const TString & baseName,const  TString & filterName1,const  TString & observableName1)
{
  TString name = baseName;
  name += "_";
  name += filterName1;
  name += "_";
  name += observableName1;
  return name;
}


TString Histograms::makeName(const TString & baseName,const  TString & filterName1,const  TString & observableName1,const  TString & filterName2,const  TString & observableName2)
{
  TString name = baseName;
  name += "_";
  name += filterName1;
  name += "_";
  name += observableName1;
  name += "_";
  name += filterName2;
  name += "_";
  name += observableName2;
  return name;
}

TString Histograms::makeName(const TString & baseName, int i1, const TString suffix)
{
  TString name = baseName;
  name += i1;
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & baseName, int i1, int i2, const TString suffix)
{
  TString name = baseName;
  name += i1;
  name += i2;
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & baseName, int i1, int i2, int i3, const TString suffix)
{
  TString name = baseName;
  name += i1;
  name += i2;
  name += i3;
  name += suffix;
  return name;
}

TString Histograms::makeName(const TString & baseName, int i1, int i2, int i3, int i4, const TString suffix)
{
  TString name = baseName;
  name += i1;
  name += i2;
  name += i3;
  name += i4;
  name += suffix;
  return name;
}
