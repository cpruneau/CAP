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

#ifndef CAP__PartitionCalculator
#define CAP__PartitionCalculator
#include <iostream>
#include "TClass.h"
#include "TH1.h"
#include "TFile.h"
#include "TString.h"
using namespace std;

class PartitionCalculator
{
public:

  PartitionCalculator();
  virtual ~PartitionCalculator() {}
  virtual void computePartitions(TH1* h, vector<double>& fractions, vector<double>& boundaries);

  ClassDef(PartitionCalculator,0)
};

#endif /* CAP__PartitionCalculator */
