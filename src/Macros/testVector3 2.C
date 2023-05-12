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
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
void loadBase(const TString & includeBasePath);
void loadMath(const TString & includeBasePath);

int testVector3()
{
  TString includeBasePath = getenv("CAP_SRC");
  cout << " includeBasePath: " << includeBasePath << endl;
  loadBase(includeBasePath);
  loadMath(includeBasePath);
  cout << "------------------------------------------------------------------------------------------------------" << endl;
  cout << "- test -----------------------------------------------------------------------------------------------" << endl;
  cout << "------------------------------------------------------------------------------------------------------" << endl;

  CAP::Math::Vector3<double> v1(1.0,1.,1.,0);
  CAP::Math::Vector3<double> v2(4.0,2.,1.,0);
  CAP::Math::Vector3<double> v3 = v1+v2;
  v3.print();
  cout << " v1.scalarProduct(v2):" << v1.scalarProduct(v2) << endl;
  cout << " v2.scalarProduct(v1):" << v2.scalarProduct(v1) << endl;
  CAP::Math::Vector3<double> v4 = v2.crossProduct(v1);
  v4.print();
  CAP::Math::Vector3<double> v5 = v1.crossProduct(v2);
  v5.print();
  v5.normalize();
  v5.print();
  CAP::Math::Vector3<double> v6 = v4.unit();
  v4.print();
  v6.print();
  CAP::Math::Vector3<double> v7 = v5 + v6;
  v7.print();
  try {
  v7.normalize();
  v7.print();
}
  catch (CAP::MathException e)
  {
  e.print();
  }
  cout << "  v4:" << v4 <<  "  v5:" << v5 << endl;
  return 0;
}

void loadBase(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Base/";
  gSystem->Load(includePath+"Configuration.hpp");
  gSystem->Load(includePath+"Timer.hpp");
  gSystem->Load(includePath+"MessageLogger.hpp");
  gSystem->Load(includePath+"Task.hpp");
  gSystem->Load(includePath+"TaskIterator.hpp");
  gSystem->Load(includePath+"Collection.hpp");
  gSystem->Load(includePath+"HistogramCollection.hpp");
  //gSystem->Load(includePath+"Histograms.hpp");
  gSystem->Load(includePath+"DerivedHistoIterator.hpp");
  gSystem->Load("libBase.dylib");
}

void loadMath(const TString & includeBasePath)
{
  TString includePath = includeBasePath + "/Math/";
  gSystem->Load(includePath+"BesselFunctions.hpp");
  gSystem->Load(includePath+"Complex.hpp");
  gSystem->Load(includePath+"MathBasicFunctions.hpp");
  gSystem->Load(includePath+"MathBasicGeometry.hpp");
  gSystem->Load(includePath+"MathBasicIntegration.hpp");
  gSystem->Load(includePath+"MathBasicInterpolation.hpp");
  gSystem->Load(includePath+"MathBasicSearch.hpp");
  gSystem->Load(includePath+"MathBasicSortingFunctions.hpp");
  gSystem->Load(includePath+"MathBasicStatistics.hpp");
  gSystem->Load(includePath+"MathConstants.hpp");
  gSystem->Load(includePath+"Vector3.hpp");
  gSystem->Load("libMath.dylib");
}
