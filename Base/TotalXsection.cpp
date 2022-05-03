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
#include "TotalXsection.hpp"
#include "TMath.h"

// Compute Total Xsect vs sqrt(s)
// x[0] : sqrt(s)
// Gauron parameterization is "s"
double GauronModelF1(double *xx, double *pp)
{
  double s   = xx[0]*xx[0];
  double Z   = pp[0];
  double X   = pp[1];
  double eps = pp[2];
  double Y1  = pp[3];
  double Y2  = pp[4];
  double alpha = pp[5]-1.0;
  return Z + X*TMath::Power(s,eps) + (Y1 - Y2)*TMath::Power(s,alpha);
}

double GauronGammaModelF1(double *xx, double *pp)
{
  double s     = xx[0]*xx[0];
  double delta = pp[0];
  double Z     = pp[1];
  double X     = pp[2];
  double eps   = pp[3];
  double Y1    = pp[4];
  double alpha = pp[5]-1;
  return delta*Z + delta*X*TMath::Power(s,eps) + Y1*TMath::Power(s,alpha);
}



ClassImp(TotalXsection);

//double TotalXsection::radius = 1.2;

TotalXsection::TotalXsection()
{
}


TF1 * TotalXsection::getFunctionTotalXsectionVsSqrtS(TotalXsectionSystem selectedSystem,
                                                     TotalXsectionModel selectedModel,
                                                     double minSqrtS,
                                                     double maxSqrtS)
{
  TF1 * function;
  TString functionName;

  switch (selectedModel)
    {
      case GauronModel:
      //      double s   = xx[0]*xx[0];
      //      double Z   = pp[0];
      //      double X   = pp[1];
      //      double eps = pp[2];
      //      double Y1  = pp[3];
      //      double Y2  = pp[4];
      //      double alpha = pp[5]-1;
      switch (selectedSystem)
        {
          case PPCollisions:
          functionName = "GauronPPCollisions";
          function = new TF1(functionName,GauronModelF1, minSqrtS, maxSqrtS, 6);
          function->SetParameter(0,20.251); // Z
          function->SetParameter(1,7.572); // X
          function->SetParameter(2,0.132); // eps
          function->SetParameter(3,74.811); // Y1
          function->SetParameter(4,29.918); // Y2
          function->SetParameter(5,0.48); // alpha0
          break;
          case PPbarCollisions:
          functionName = "GauronPPbarCollisions";
          function = new TF1(functionName,GauronModelF1, minSqrtS, maxSqrtS, 6);
          function->SetParameter(0,20.251); // Z
          function->SetParameter(1,7.572); // X
          function->SetParameter(2,0.132); // eps
          function->SetParameter(3,74.811); // Y1
          function->SetParameter(4,-29.918); // Y2
          function->SetParameter(5,0.48); // alpha0
          break;
          case PiPlusPCollisions:
          functionName = "GauronPiPlusPCollisions";
          function = new TF1(functionName,GauronModelF1, minSqrtS, maxSqrtS, 6);
          function->SetParameter(0,5.283); // Z
          function->SetParameter(1,7.572); // X
          function->SetParameter(2,0.132); // eps
          function->SetParameter(3,48.972); // Y1
          function->SetParameter(4,6.028); // Y2
          function->SetParameter(5,0.48); // alpha0
          break;
          case PiMinusPCollisions:
          functionName = "GauronPiMinusPCollisions";
          function = new TF1(functionName,GauronModelF1, minSqrtS, maxSqrtS, 6);
          function->SetParameter(0,5.283); // Z
          function->SetParameter(1,7.572); // X
          function->SetParameter(2,0.132); // eps
          function->SetParameter(3,48.972); // Y1
          function->SetParameter(4,-6.028); // Y2
          function->SetParameter(5,0.48); // alpha0
          break;
          case KPlusPCollisions:
          functionName = "GauronKPlusPCollisions";
          function = new TF1(functionName,GauronModelF1, minSqrtS, maxSqrtS, 6);
          function->SetParameter(0,2.208); // Z
          function->SetParameter(1,7.572); // X
          function->SetParameter(2,0.132); // eps
          function->SetParameter(3,34.483); // Y1
          function->SetParameter(4,11.935); // Y2
          function->SetParameter(5,0.48); // alpha0
          break;
          case KMinusPCollisions:
          functionName = "GauronKMinusPCollisions";
          function = new TF1(functionName,GauronModelF1, minSqrtS, maxSqrtS, 6);
          function->SetParameter(0,2.208); // Z
          function->SetParameter(1,7.572); // X
          function->SetParameter(2,0.132); // eps
          function->SetParameter(3,34.483); // Y1
          function->SetParameter(4,-11.935); // Y2
          function->SetParameter(5,0.48); // alpha0
          break;
          case GammaPCollisions:
          //          double delta = pp[0];
          //          double Z     = pp[1];
          //          double X     = pp[2];
          //          double eps   = pp[3];
          //          double Y1    = pp[4];
          //          double alpha = pp[5]-1;
          functionName = "GauronGammaPCollisions";
          function = new TF1(functionName,GauronGammaModelF1, minSqrtS, maxSqrtS, 6);
          function->SetParameter(0,0.303E-2); // delta
          function->SetParameter(1,20.251);    // Z
          function->SetParameter(2,7.572);    // X
          function->SetParameter(3,0.132);   // eps
          function->SetParameter(4,0.121);   // Y1
          function->SetParameter(5,0.48);   // alpha0
          break;
          case GammaGammaCollisions:
          functionName = "GauronGammaGammaCollisions";
          function = new TF1(functionName,GauronGammaModelF1, minSqrtS, maxSqrtS, 6);
          function->SetParameter(0,0.303E-2*0.303E-2); // delta^2
          function->SetParameter(1,20.251);    // Z
          function->SetParameter(2,7.572);    // X
          function->SetParameter(3,0.132);   // eps
          function->SetParameter(4,0.0);   // Y1
          function->SetParameter(5,0.48);   // alpha0
          break;
        }
      break;

      case XModel: function =  nullptr; break;

      case YModel:  function =  nullptr; break;
    }
  return function;
}
