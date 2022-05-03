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
#ifndef CAP__TotalXsection
#define CAP__TotalXsection
#include "TObject.h"
#include "TF1.h"
#include "TMath.h"

double GauronModelF1(double *xx, double *pp);
double GauronGammaModelF1(double *xx, double *pp);

// ***************************************************************************
// TotalXsection
//
// Gauron Model:
// Pierre Gauron, Basarab Nicolescu, "A Possible Two-Component Structure of
// the Non-Perturbative Pomeron", Phys.Lett. B486 (2000) 71-76
//
// ***************************************************************************
class TotalXsection : public  TObject
{
public:

  enum TotalXsectionModel   { GauronModel, XModel, YModel};
  enum TotalXsectionSystem  { PPCollisions, PPbarCollisions, PiPlusPCollisions,
      PiMinusPCollisions, KPlusPCollisions, KMinusPCollisions, GammaPCollisions, GammaGammaCollisions };

  TotalXsection();
  virtual ~TotalXsection()
  {
  // no ops.
  }


  TF1 * getFunctionTotalXsectionVsSqrtS(TotalXsectionSystem selectedSystem,
                                        TotalXsectionModel selectedModel,
                                        double minSqrtS,
                                        double maxSqrtS);

  ClassDef(TotalXsection,0)
  
};

#endif /* TotalXsection_hpp */
