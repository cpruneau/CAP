// Author: Victor Gonzalez  Dec 19, 2020

/*************************************************************************
 * Copyright (C) 2019, Victor Gonzalez, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/
#ifndef CAP__BidimGaussFitResult
#define CAP__BidimGaussFitResult
#include <iostream>
#include <ostream>
#include <TMath.h>
#include <TROOT.h>
#include <TH2.h>
#include <TF2.h>
#include <TVirtualFitter.h>
#include <TFitResult.h>
#include <TMatrixDSym.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TPad.h>
#include <TLatex.h>
#include <TRandom2.h>
#include <TRandom3.h>
#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"

using namespace std;
using std::ostream;

namespace CAP
{

class BidimGaussFitResult
{
public:

  BidimGaussFitResult();
  BidimGaussFitResult(const BidimGaussFitResult& source);
  virtual ~BidimGaussFitResult();
  const BidimGaussFitResult & operator=(const BidimGaussFitResult & source);
  virtual void reset();
  virtual void saveResults(TF2 * fitFunction);
  virtual void printResults(ostream & out);

  double  extractGeneralizedRMS(TF1* func, int  sigmaIndex, int  betaIndex);
  double  extractGeneralizedRMSError(TF1* func, TMatrixDSym& cov, int  sigmaIndex, int  betaIndex);
//  std::tuple<double,double> extractHistoRMSError(TH1 *h);
  std::tuple<double,double> extractGeneralizedRMSError(TH1 *h);
  std::tuple<double,double,double,double> extractGeneralizedRMSandRMSError(TH2 *h,int xlowlimbin,int ylowlimbin);

  double  sigmaEta;
  double  sigmaEtaErr;
  double  sigmaPhi;
  double  sigmaPhiErr;
  double  A;
  double  Aerr;
  double  wEta;
  double  wEtaErr;
  double  wPhi;
  double  wPhiErr;
  double  gammaEta;
  double  gammaEtaErr;
  double  gammaPhi;
  double  gammaPhiErr;
  double  a0;
  double  a0err;
  double  a1;
  double  a1err;
  double  a2;
  double  a2err;
  double  a3;
  double  a3err;
  double  a4;
  double  a4err;
  double  a5;
  double  a5err;
  double  a6;
  double  a6err;
  double  lina2,lina2err;
  double  lina3,lina3err;
  double  sqa2,sqa2err;
  double  sqa3,sqa3err;
  double  ndf;
  double  ndfWoMiddle;
  double  ndfFullWoMiddle;
  double  chi2;
  double  chi2WoMiddle;
  double  chi2FullWoMiddle;
  double  chi2Ndf;
  double  chi2NdfWoMiddle;
  double  chi2NdfFullWoMiddle;

  double  rmsEta;
  double  rmsEtaErr;
  double  rmsPhi;
  double  rmsPhiErr;
  bool    success;
  TFitResultPtr resultPtr; /* we are not responsible of deleting this memory */

  ClassDef(BidimGaussFitResult,0)
  };

} // namespace CAP

#endif /* CAP__BidimGaussFitResult */
