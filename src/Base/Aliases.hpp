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
#ifndef CAP__Aliases
#define CAP__Aliases
#include <vector>
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"

namespace CAP
{

using String        = TString;
using Histogram1D   = TH1;
using Histogram2D   = TH2;
using LorentzVector = TLorentzVector;

using VectorString  = std::vector<TString>;

}

#endif /* CAP__NameManager */
