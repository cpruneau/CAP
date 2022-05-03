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
#ifndef CAP__ParticleFilterAliceV0
#define CAP__ParticleFilterAliceV0
#include "TString.h"
#include "ParticleFilter.hpp"

//////////////////////////////////////////////////////////////////////////////////////////
// Single Particle Filter
//
// chargeSelected:
// case -1:    accepts negative only
// case  0:    accepts neutral only
// case  1:    accepts positive only
// case  999:  accepts all
//////////////////////////////////////////////////////////////////////////////////////////



class ParticleFilterAliceV0 : public ParticleFilter
{
public:

  enum V0Selection   { V0A, V0C, V0M };

  ParticleFilterAliceV0(bool   filteringOnPt,
                        double minPt,
                        double maxPt,
                        V0Selection     v0Selected=V0M,
                        FamilySelection familySelected = Hadron,
                        ChargeSelection chargeSelected = Charged,
                        StatusSelection statusSelected = Live);

  ParticleFilterAliceV0(bool   filteringOnPt,
                        double minPt,
                        double maxPt,
                        V0Selection     v0Selected=V0M,
                        NameSelection   nameSelected   = PionPlus,
                        StatusSelection statusSelected = Live);

  ParticleFilterAliceV0(const ParticleFilterAliceV0 & otherFilter);
  ParticleFilterAliceV0 & operator=( const ParticleFilterAliceV0 & otherFilter);

  virtual ~ParticleFilterAliceV0(){}

  virtual inline bool acceptEta(double eta)
  {
  bool accepting;
  switch (v0Selected)
      {
        case V0A:   accepting = (min_eta_v0A<=eta)&&(eta<max_eta_v0A); break;
        case V0C:   accepting = (min_eta_v0C<=eta)&&(eta<max_eta_v0C); break;
        case V0M:   accepting = ((min_eta_v0A<=eta)&&(eta<max_eta_v0A)) ||  ((min_eta_v0C<=eta)&&(eta<max_eta_v0C));break;
      }
  return accepting; //(min_eta<eta) && (eta<= max_eta);
  }

  virtual TString getName() const ;
  virtual TString getTitle() const ;
  virtual TString getLongName() const ;
  virtual TString getLongTitle() const ;

  virtual void printProperties(  ostream & os);

  //////////////////////////////////////////////////////////////////////////////////////////
  // Data Members
  //////////////////////////////////////////////////////////////////////////////////////////
protected:

  V0Selection v0Selected;
  double min_eta_v0A;
  double max_eta_v0A;
  double min_eta_v0C;
  double max_eta_v0C;

  ClassDef(ParticleFilterAliceV0,0)
};

#endif /* CAP__ParticleFilterAliceV0 */
