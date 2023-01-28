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
#ifndef CAP__StatStudyModel
#define CAP__StatStudyModel
#include "TString.h"

namespace CAP
{

class StatStudyModel
{
public:
  
  StatStudyModel(double nPlusAvg = 100,
                 double nMinusAvg = 100,
                 double nPlusPlusAvg = 20,
                 double nPlusMinusAvg = 60,
                 double nMinusMinusAvg = 20,
                 double plusEfficiency  = 0.87,
                 double minusEfficiency = 0.85,
                 long   nEventsReq=100000);
  virtual ~StatStudyModel();
  void generate(double & nPlus, double & nMinus, double & nPlusEff, double & nMinusEff);
  int binomialLoss(int nIn, double efficiency);

  
  // Data Members

  double nPlusAvg;
  double nMinusAvg;
  double nPlusPlusAvg;
  double nPlusMinusAvg;
  double nMinusMinusAvg;
  double plusEfficiency;
  double minusEfficiency;
  long   nEvents;

  ClassDef(StatStudyModel,0)
};

} // namespace CAP

#endif /* CAP__StatStudyModel */
