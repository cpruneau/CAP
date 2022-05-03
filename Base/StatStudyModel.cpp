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
#include "StatStudyModel.hpp"
#include "TRandom.h"

ClassImp(StatStudyModel);

StatStudyModel::StatStudyModel(double nPlusAvgReq,
                               double nMinusAvgReq,
                               double nPlusPlusAvgReq,
                               double nPlusMinusAvgReq,
                               double nMinusMinusAvgReq,
                               double plusEfficiencyReq,
                               double minusEfficiencyReq,
                               long   nEventsReq)
:
nPlusAvg(nPlusAvgReq),
nMinusAvg(nMinusAvgReq),
nPlusPlusAvg(nPlusPlusAvgReq),
nPlusMinusAvg(nPlusMinusAvgReq),
nMinusMinusAvg(nMinusMinusAvgReq),
plusEfficiency(plusEfficiencyReq),
minusEfficiency(minusEfficiencyReq),
nEvents(nEventsReq)
{  }

StatStudyModel::~StatStudyModel()
{
}

void StatStudyModel::generate(double & nPlus, double & nMinus, double & nPlusEff, double & nMinusEff)
{
   double nPlusPlus;
   double nPlusMinus;
   double nMinusMinus;

  nPlus       = int( gRandom->Gaus(nPlusAvg,sqrt(nPlusAvg))    +0.5        );
  nMinus      = int( gRandom->Gaus(nMinusAvg,sqrt(nMinusAvg))     +0.5     );
  nPlusPlus   = int( gRandom->Gaus(nPlusPlusAvg,sqrt(nPlusPlusAvg)) +0.5   );
  nPlusMinus  = int( gRandom->Gaus(nPlusMinusAvg,sqrt(nPlusMinusAvg))+0.5  );
  nMinusMinus = int( gRandom->Gaus(nMinusMinusAvg,sqrt(nMinusMinusAvg))+0.5);
  nPlus  += 2.0*nPlusPlus;
  nPlus  += nPlusMinus;
  nMinus += 2.0*nMinusMinus;
  nMinus += nPlusMinus;
  nPlusEff   = binomialLoss(nPlus,plusEfficiency);
  nMinusEff  = binomialLoss(nMinus,minusEfficiency);
}

int StatStudyModel::binomialLoss(int nIn, double efficiency)
{
  int nOut = 0;
  for (int n=0; n<nIn; n++)
    {
    if (gRandom->Rndm()<efficiency) nOut++;
    }
  return nOut;
}
