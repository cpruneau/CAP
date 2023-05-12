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

#ifndef CAP__FunctionCollection
#define CAP__FunctionCollection
#include <stdio.h>
#include "TClass.h"
#include "TFile.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TMath.h"
#include "TPad.h"
#include "TF1.h"
#include "TF2.h"
#include "TRandom.h"
#include "TString.h"
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLegend.h"
#include "GraphConfiguration.hpp"
#include "CanvasConfiguration.hpp"
#include "CanvasCollection.hpp"
#include "Collection.hpp"
using namespace std;

class FunctionCollection  : public Collection<TF1>
{
public:
  
  /////////////////////////////////////////////////////////////
  // Member functions
  /////////////////////////////////////////////////////////////
  FunctionCollection(const TString& _name="",Severity logLevel=Info);
  FunctionCollection(const FunctionCollection & source);
  virtual ~FunctionCollection();
  FunctionCollection & operator=(const FunctionCollection & source);

  int getNFunction()
  {
  return getCollectionSize();
  }

  void setDefaultOptions(bool color=0);
  void setFunctionProperties(TF1 * f, const GraphConfiguration & graphConfiguration);
  void plotAllFunctions(const TString & outputPath, bool doPrint=false);

  ClassDef(FunctionCollection,0)
}; // FunctionCollection

#endif /* FunctionCollection_hpp */
