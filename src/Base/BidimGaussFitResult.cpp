// Author: Victor Gonzalez, 2019

/* ***********************************************************************
  Copyright (C) 2019, Victor Gonzalez, Claude Pruneau.
  All rights reserved.
  Based on the ROOT package and environment

  For the licensing terms see LICENSE.
 \class Task
 \ingroup CAP
 
 Class defining Event BidimGaussFitResult
 *************************************************************************/

#include "BidimGaussFitResult.hpp"
ClassImp(BidimGaussFitResult);


BidimGaussFitResult::BidimGaussFitResult()
:
sigmaEta(0.0),
sigmaEtaErr(0.0),
sigmaPhi(0.0),
sigmaPhiErr(0.0),
A(0.0),
Aerr(0.0),
wEta(0.0),
wEtaErr(0.0),
wPhi(0.0),
wPhiErr(0.0),
gammaEta(0.0),
gammaEtaErr(0.0),
gammaPhi(0.0),
gammaPhiErr(0.0),
a0(0.0),
a0err(0.0),
a1(0.0),
a1err(0.0),
a2(0.0),
a2err(0.0),
a3(0.0),
a3err(0.0),
a4(0.0),
a4err(0.0),
a5(0.0),
a5err(0.0),
a6(0.0),
a6err(0.0),
lina2(0.0),
lina2err(0.0),
lina3(0.0),
lina3err(0.0),
sqa2(0.0),
sqa2err(0.0),
sqa3(0.0),
sqa3err(0.0),
ndf(0.0),
ndfWoMiddle(0.0),
ndfFullWoMiddle(0.0),
chi2(0.0),
chi2WoMiddle(0.0),
chi2FullWoMiddle(0.0),
chi2Ndf(0.0),
chi2NdfWoMiddle(0.0),
chi2NdfFullWoMiddle(0.0),

rmsEta(0.0),
rmsEtaErr(0.0),
rmsPhi(0.0),
rmsPhiErr(0.0),
success(kFALSE),
resultPtr(nullptr)
{
  // no ops
}

BidimGaussFitResult::BidimGaussFitResult(const BidimGaussFitResult& source)
:
sigmaEta(source.sigmaEta),
sigmaEtaErr(source.sigmaEtaErr),
sigmaPhi(source.sigmaPhi),
sigmaPhiErr(source.sigmaPhiErr),
A(source.A),
Aerr(source.Aerr),
wEta(source.wEta),
wEtaErr(source.wEtaErr),
wPhi(source.wPhi),
wPhiErr(source.wPhiErr),
gammaEta(source.gammaEta),
gammaEtaErr(source.gammaEtaErr),
gammaPhi(source.gammaPhi),
gammaPhiErr(source.gammaPhiErr),
a0(source.a0),
a0err(source.a0err),
a1(source.a1),
a1err(source.a1err),
a2(source.a2),
a2err(source.a2err),
a3(source.a3),
a3err(source.a3err),
a4(source.a4),
a4err(source.a4err),
a5(source.a5),
a5err(source.a5err),
a6(source.a6),
a6err(source.a6err),
lina2(source.lina2),
lina2err(source.lina2err),
lina3(source.lina3),
lina3err(source.lina3err),
sqa2(source.sqa2),
sqa2err(source.sqa2err),
sqa3(source.sqa3),
sqa3err(source.sqa3err),
ndf(source.ndf),
ndfWoMiddle(source.ndfWoMiddle),
ndfFullWoMiddle(source.ndfFullWoMiddle),
chi2(source.chi2),
chi2WoMiddle(source.chi2WoMiddle),
chi2FullWoMiddle(source.chi2FullWoMiddle),
chi2Ndf(source.chi2Ndf),
chi2NdfWoMiddle(source.chi2NdfWoMiddle),
chi2NdfFullWoMiddle(source.chi2NdfFullWoMiddle),
rmsEta(source.rmsEta),
rmsEtaErr(source.rmsEtaErr),
rmsPhi(source.rmsPhi),
rmsPhiErr(source.rmsPhiErr),
success(kFALSE),
resultPtr(source.resultPtr)
{
  // no ops
}

BidimGaussFitResult::~BidimGaussFitResult()
{
  // no ops
}


const BidimGaussFitResult & BidimGaussFitResult::operator=(const BidimGaussFitResult & source)
{
  if (this!=&source)
    {
    sigmaEta    = source.sigmaEta;
    sigmaEtaErr = source.sigmaEtaErr;
    sigmaPhi    = source.sigmaPhi;
    sigmaPhiErr = source.sigmaPhiErr;
    A           = source.A;
    Aerr        = source.Aerr;
    wEta        = source.wEta;
    wEtaErr     = source.wEtaErr;
    wPhi        = source.wPhi;
    wPhiErr     = source.wPhiErr;
    gammaEta    = source.gammaEta;
    gammaEtaErr = source.gammaEtaErr;
    gammaPhi    = source.gammaPhi;
    gammaPhiErr = source.gammaPhiErr;
    a0          = source.a0;
    a0err       = source.a0err;
    a1          = source.a1;
    a1err       = source.a1err;
    a2          = source.a2;
    a2err       = source.a2err;
    a3          = source.a3;
    a3err       = source.a3err;
    a4          = source.a4;
    a4err       = source.a4err;
    a5          = source.a5;
    a5err       = source.a5err;
    a6          = source.a6;
    a6err       = source.a6err;
    lina2       = source.lina2;
    lina2err    = source.lina2err;
    lina3       = source.lina3;
    lina3err    = source.lina3err;
    sqa2        = source.sqa2;
    sqa2err     = source.sqa2err;
    sqa3        = source.sqa3;
    sqa3err     = source.sqa3err;
    success     = source.success;
    ndf         = source.ndf;
    ndfWoMiddle      = source.ndfWoMiddle;
    ndfFullWoMiddle  = source.ndfFullWoMiddle;
    chi2             = source.chi2;
    chi2WoMiddle     = source.chi2WoMiddle;
    chi2FullWoMiddle = source.chi2FullWoMiddle;
    chi2Ndf          = source.chi2Ndf;
    chi2NdfWoMiddle  = source.chi2NdfWoMiddle;
    chi2NdfFullWoMiddle = source.chi2NdfFullWoMiddle;

    rmsEta      = source.rmsEta;
    rmsEtaErr   = source.rmsEtaErr;
    rmsPhi      = source.rmsPhi;
    rmsPhiErr   = source.rmsPhiErr;
    resultPtr   = source.resultPtr;
    }
  return *this;
}


void BidimGaussFitResult::reset()
{
  sigmaEta     = 0.0;
  sigmaEtaErr  = 0.0;
  sigmaPhi     = 0.0;
  sigmaPhiErr  = 0.0;
  A            = 0.0;
  Aerr         = 0.0;
  wEta         = 0.0;
  wEtaErr      = 0.0;
  wPhi         = 0.0;
  wPhiErr      = 0.0;
  gammaEta     = 0.0;
  gammaEtaErr  = 0.0;
  gammaPhi     = 0.0;
  gammaPhiErr  = 0.0;
  a0     = 0.0;
  a0err  = 0.0;
  a1     = 0.0;
  a1err  = 0.0;
  a2     = 0.0;
  a2err  = 0.0;
  a3     = 0.0;
  a3err  = 0.0;
  a4     = 0.0;
  a4err  = 0.0;
  a5     = 0.0;
  a5err  = 0.0;
  a6     = 0.0;
  a6err  = 0.0;
  lina2  = 0.0;
  lina2err  = 0.0;
  lina3     = 0.0;
  lina3err  = 0.0;
  sqa2      = 0.0;
  sqa2err   = 0.0;
  sqa3      = 0.0;
  sqa3err   = 0.0;
  chi2      = 0.0;
  chi2WoMiddle = 0.0;
  chi2FullWoMiddle = 0.0;
  ndf       = 0.0;
  chi2Ndf   = 0.0;
  rmsEta    = 0.0;
  rmsEtaErr = 0.0;
  rmsPhi    = 0.0;
  rmsPhiErr = 0.0;
  success   = false;
  resultPtr = nullptr;

}


void BidimGaussFitResult::saveResults(TF2 * f)
{
  int nPar =  f->GetNpar();
  bool extractWidths = false;
  A           = -1;  Aerr        = -1;
  wEta        = -1;  wEtaErr     = -1;
  wPhi        = -1;  wPhiErr     = -1;
  gammaEta    = -1;  gammaEtaErr = -1;
  gammaPhi    = -1;  gammaPhiErr = -1;
  a0          = -1;  a0err       = -1;
  a1          = -1;  a1err       = -1;
  a2          = -1;  a2err       = -1;
  a3          = -1;  a3err       = -1;
  a4          = -1;  a4err       = -1;
  a5          = -1;  a5err       = -1;
  a6          = -1;  a6err       = -1;
  lina2       = -1;  lina2err    = -1;
  lina3       = -1;  lina3err    = -1;
  sqa2        = -1;  sqa2err     = -1;
  sqa3        = -1;  sqa3err     = -1;
  sigmaEta    = -1;  sigmaEtaErr = -1;
  sigmaPhi    = -1;  sigmaPhiErr = -1;
  chi2        = -1;
  ndf         = -1;
  chi2Ndf     = -1;
  switch (nPar)
    {
      default:
      return;
      break;

      case 5:     // Peak fit only
      case 6:     // Peak fit only
      extractWidths = true;
      A           = f->GetParameter(0);  Aerr        = f->GetParError (0);
      wEta        = f->GetParameter(1);  wEtaErr     = f->GetParError (1);
      wPhi        = f->GetParameter(2);  wPhiErr     = f->GetParError (2);
      gammaEta    = f->GetParameter(3);  gammaEtaErr = f->GetParError (3);
      gammaPhi    = f->GetParameter(4);  gammaPhiErr = f->GetParError (4);
      a0          = f->GetParameter(5);  a0err       = f->GetParError (5);
      break;

      case 10: // Flow Fit Only
      case 11: // Flow Fit Only
      a0          = f->GetParameter(0);  a0err      = f->GetParError (0);
      a1          = f->GetParameter(1);  a1err      = f->GetParError (1);
      a2          = f->GetParameter(2);  a2err      = f->GetParError (2);
      a3          = f->GetParameter(3);  a3err      = f->GetParError (3);
      a4          = f->GetParameter(4);  a4err      = f->GetParError (4);
      a5          = f->GetParameter(5);  a5err      = f->GetParError (5);
      a6          = f->GetParameter(6);  a6err      = f->GetParError (6);
      lina2       = f->GetParameter(7);  lina2err   = f->GetParError (7);
      lina3       = f->GetParameter(8);  lina3err   = f->GetParError (8);
      sqa2        = f->GetParameter(9);  sqa2err    = f->GetParError (9);
      sqa3        = f->GetParameter(10);  sqa3err    = f->GetParError (10);
      break;

      case 16:    // One peak + flow fit
      extractWidths = true;
      A           = f->GetParameter(0);  Aerr        = f->GetParError (0);
      wEta        = f->GetParameter(1);  wEtaErr     = f->GetParError (1);
      wPhi        = f->GetParameter(2);  wPhiErr     = f->GetParError (2);
      gammaEta    = f->GetParameter(3);  gammaEtaErr = f->GetParError (3);
      gammaPhi    = f->GetParameter(4);  gammaPhiErr = f->GetParError (4);

      a0          = f->GetParameter(5);  a0err       = f->GetParError (5);
      a1          = f->GetParameter(6);  a0err       = f->GetParError (6);
      a2          = f->GetParameter(7);  a2err       = f->GetParError (7);
      a3          = f->GetParameter(8);  a3err       = f->GetParError (8);
      a4          = f->GetParameter(9);  a4err       = f->GetParError (9);
      a5          = f->GetParameter(10); a5err       = f->GetParError(10);
      a6          = f->GetParameter(11); a6err       = f->GetParError (11);

      lina2       = f->GetParameter(12); lina2err    = f->GetParError (12);
      lina3       = f->GetParameter(13); lina3err    = f->GetParError (13);
      sqa2        = f->GetParameter(14); sqa2err     = f->GetParError (14);
      sqa3        = f->GetParameter(15); sqa3err     = f->GetParError (15);
      break;
    }

  if (extractWidths)
  {
  cout << "===============================================================================================" << endl;
  cout << "BidimGaussFitResult::saveResults(TF2 * f) -- Extracting the matrice.." << endl;
  TMatrixDSym cov = resultPtr->GetCovarianceMatrix();
  cout << "BidimGaussFitResult::saveResults(TF2 * f) -- Extracting the matrice.." << endl;
  cout << "===============================================================================================" << endl;

  sigmaEta    = extractGeneralizedRMS(f,1,3);
  sigmaEtaErr = extractGeneralizedRMSError(f,cov,1,3);
  sigmaPhi    = extractGeneralizedRMS(f,2,4);
  sigmaPhiErr = extractGeneralizedRMSError(f,cov,2,4);
  }
  chi2        = f->GetChisquare();
  ndf         = f->GetNDF();
  chi2Ndf     = chi2/ndf;
}

std::tuple<double,double> BidimGaussFitResult::extractGeneralizedRMSError(TH1 *h)
{
  int NLOOPS_RMS = 10000;
  double norm = double(NLOOPS_RMS);
  int npoints = h->GetNbinsX();
    cout << endl
    << "  Low edge :" << h->GetXaxis()->GetBinLowEdge(1) << endl
    << " High edge :" << h->GetXaxis()->GetBinUpEdge(npoints) << endl;
  double *point  = new double[npoints];
  double *point2 = new double[npoints];
  double *val    = new double[npoints];
  double *vale   = new double[npoints];
  for (int ip = 0; ip < npoints; ++ip)
  {
  point[ip]  = h->GetXaxis()->GetBinCenter(ip+1);
  point2[ip] = point[ip]*point[ip];
  val[ip]    = h->GetBinContent(ip+1);
  vale[ip]   = h->GetBinError(ip+1);
  }
  double rms = 0.0;
  double rms2 = 0.0;
  for (int i = 0; i < NLOOPS_RMS; ++i)
  {
  double sumw = 0.0;
  double sumwx = 0.0;
  double sumwx2 = 0.0;
  for (int ip = 0; ip < npoints; ++ip)
    {
    double w = gRandom->Gaus(val[ip],vale[ip]);
    sumw    += w;
    sumwx   += w*point[ip];
    sumwx2  += w*point2[ip];
    }
  double _rms = (sumw != 0) ? TMath::Max(sumwx2/sumw - sumwx/sumw*sumwx/sumw,0.0) : 0.0;
  rms   += TMath::Sqrt(_rms);
  rms2  += _rms;
  }
  delete [] vale;
  delete [] val;
  delete [] point2;
  delete [] point;
  rms  /= norm;
  rms2 /= norm;
  rms2 = (rms2>0) ? TMath::Sqrt(rms2-rms*rms) : 0.0;
  return std::make_tuple(rms,rms2);
}

std::tuple<double,double,double,double> BidimGaussFitResult::extractGeneralizedRMSandRMSError(TH2 *h,int xlowlimbin,int ylowlimbin)
{
  /* from the histogram we extract the rms on both dimensions */
  /* longitudinally we restrict ourselves to the passed range */
  /* azimuthally we restrict ourselves to the near side */
  TH2* forRMS = (TH2*) h->Clone(TString::Format("%s_forRMS", h->GetName()));
  int firstybin = ylowlimbin;
  int lastybin  = h->GetYaxis()->FindBin(-h->GetYaxis()->GetBinCenter(firstybin));
  int firstxbin = xlowlimbin;
  int lastxbin  = h->GetXaxis()->FindBin(-h->GetXaxis()->GetBinCenter(firstxbin));
  forRMS->GetXaxis()->SetRange(firstxbin,lastxbin);
  forRMS->GetYaxis()->SetRange(firstybin,lastybin);
  auto forRMSx = forRMS->ProjectionX(TString::Format("%s_px",forRMS->GetName()));
  auto forRMSy = forRMS->ProjectionY(TString::Format("%s_py",forRMS->GetName()));
  double rmsx  = forRMSx->GetRMS();
  double rmsmc_x,rmsemc_x;
  std::tie(rmsmc_x,rmsemc_x) = extractGeneralizedRMSError(forRMSx);
  double rmsy = forRMSy->GetRMS();
  double rmsmc_y,rmsemc_y;
  std::tie(rmsmc_y,rmsemc_y) = extractGeneralizedRMSError(forRMSy);
  delete forRMSy;
  delete forRMSx;
  delete forRMS;

    cout << endl
    << " X: RMS: " << rmsx << " MC RMS: " <<  rmsmc_x << " DeltaRMS: " << rmsemc_x << endl
    << " Y: RMS: " << rmsy << " MC RMS: " <<  rmsmc_y << " DeltaRMS: " << rmsemc_y << endl;
  return std::make_tuple(rmsx,rmsemc_x,rmsy,rmsemc_y);
}

double BidimGaussFitResult::extractGeneralizedRMS(TF1* f, int sigmaIndex, int betaIndex)
{
  double sigma = f->GetParameter(sigmaIndex);
  double beta  = f->GetParameter(betaIndex);
  return TMath::Sqrt(sigma*sigma*TMath::Gamma(3.0/beta)/TMath::Gamma(1.0/beta));
}

double BidimGaussFitResult::extractGeneralizedRMSError(TF1* f, TMatrixDSym& cov, int   sigmaIndex, int   betaIndex)
{
  double sigma    = f->GetParameter(sigmaIndex);
  double beta     = f->GetParameter(betaIndex);
  double sigmaDer = TMath::Sqrt(TMath::Gamma(3./beta)/TMath::Gamma(1./beta));
  TF1* tmp        = new TF1("tmp","TMath::Sqrt(TMath::Gamma(3./x)/TMath::Gamma(1./x))",1,2);
  double betaDer  = sigma*tmp->Derivative(beta);
  double rmsError =
  TMath::Power(sigmaDer * f->GetParError(sigmaIndex), 2) +
  TMath::Power(betaDer *  f->GetParError(betaIndex),  2) +
  2.0 * sigmaDer * betaDer * cov(sigmaIndex, betaIndex);
  delete tmp;
  return TMath::Sqrt(rmsError);
}


void BidimGaussFitResult::printResults(ostream & out)
{
  out
   << "            sigmaEta : " << sigmaEta << " +- " << sigmaEtaErr << endl
   << "            sigmaPhi : " << sigmaPhi << " +- " <<   sigmaPhiErr << endl
   << "                   A : " << A << " +- " <<   Aerr << endl
   << "                wEta : " << wEta << " +- " <<   wEtaErr << endl
   << "                wPhi : " << wPhi << " +- " <<  wPhiErr << endl
   << "            gammaEta : " << gammaEta << " +- " <<  gammaEtaErr << endl
   << "            gammaPhi : " << gammaPhi << " +- " <<  gammaPhiErr << endl
   << "                  a0 : " << a0 << " +- " <<  a0err << endl
   << "                  a1 : " << a1 << " +- " <<  a1err << endl
   << "                  a2 : " << a2 << " +- " <<  a2err << endl
   << "                  a3 : " << a3 << " +- " <<  a3err << endl
   << "                  a4 : " << a4 << " +- " <<  a4err << endl
   << "                  a5 : " << a5 << " +- " <<  a5err << endl
   << "                  a6 : " << a6 << " +- " <<  a6err << endl
   << "               lina2 : " << lina2 << " +- " <<  lina2err << endl
   << "               lina3 : " << lina3 << " +- " <<  lina3err << endl
   << "                sqa2 : " << sqa2 << " +- " <<  sqa2err << endl
   << "                sqa3 : " << sqa3 << " +- " <<  sqa3err << endl
   << "                 ndf : " << ndf  << endl
   << "                chi2 : " << chi2 << endl
   << "            chi2/Ndf : " << chi2Ndf << endl
   << "         ndfWoMiddle : " << ndfWoMiddle << endl
   << "        chi2WoMiddle : " << chi2WoMiddle << endl
   << "    chi2/NdfWoMiddle : " << chi2NdfWoMiddle << endl
   << "     ndfFullWoMiddle : " << ndfFullWoMiddle << endl
   << "    chi2FullWoMiddle : " << chi2FullWoMiddle << endl
   << " chi2NdfFullWoMiddle : " << chi2NdfFullWoMiddle << endl
   << "              rmsEta : " << rmsEta << " +- " << rmsEtaErr << endl
   << "              rmsPhi : " << rmsPhi << " +- " << rmsPhiErr << endl
   << "             success : " << success << endl
   << "           resultPtr : " << resultPtr << endl;
}
