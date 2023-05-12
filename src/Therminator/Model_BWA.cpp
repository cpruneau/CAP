/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERMal heavy-IoN generATOR 2                     *
 *                                                                              *
 * Version:                                                                     *
 *      Release, 2.0.3, 1 February 2011                                         *
 *                                                                              *
 * Authors:                                                                     *
 *      Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)                      *
 *      Adam Kisiel         (kisiel@if.pw.edu.pl)                               *
 *      Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)                    *
 *      Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)                    *
 *                                                                              *
 * Project homepage:                                                            *
 *      http://therminator2.ifj.edu.pl/                                         *
 *                                                                              *
 * For the detailed description of the program and further references           *
 * to the description of the model please refer to                              *
 * http://arxiv.org/abs/1102.0273                                               *
 *                                                                              *
 * This code can be freely used and redistributed. However if you decide to     *
 * make modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/
#include <sstream>
//#include <TMath.h>
#include "Model_BWA.hpp"

using namespace std;

ClassImp(Model_BWA);


Model_BWA::Model_BWA(const Configuration & _requestedConfiguration)
:
Model(_requestedConfiguration)
{    }

double Model_BWA::getIntegrand(ParticleType& aPartType)
{
  double dSigmaP, PdotU;
  double spinFactor, statistics, mass;
  double tau, rho, phiS, rapidityS;
  double mT,  pT,  dPt, phiP, rapidityP;
  double chemPotential; //, temperature;
  double vT;
  double zeta, zetac;
  double integrand=0.0;
  double denom=1.0;
  double mTcoshDiff;
  double cosDphi, gammaT;

  spinFactor    = aPartType.getSpinFactor();
  statistics    = aPartType.getStatistics();
  mass          = aPartType.getMass();
  chemPotential = thermodynamics->getChemicalPotential(aPartType);
  // Generate spacial components
  rho	      = rhoMax * gRandom->Rndm();
  phiS    	= TMath::TwoPi() * gRandom->Rndm();
  rapidityS	= spatialRapidityRange * (gRandom->Rndm() - 0.5);;
  tau       = tauI + amp * rho;
// Generate momentum components
  zeta      = gRandom->Rndm();
  zetac     = (zeta>0.9999999) ? 0.00000001 : 1.00-zeta;
  pT        = zeta/zetac;
  dPt       = 1.0/(zetac*zetac);
  phiP	    = TMath::TwoPi() * gRandom->Rndm();
  rapidityP	= momentumRapidityRange * (gRandom->Rndm() - 0.5);// * momentumRapidityRange;
  mT	      = sqrt(mass*mass+pT*pT);
// Transverse velocity
  //vT        = pT/(mT*cosh(rapidityP));
  switch (subModelType) // there is a bug here///
    {
      default:
      case 2:
      case 3:
      vT =   transverseVelocity; break;
      case 4:
      case 5:
      case 6:
      vT  = (rho / rhoMax) / (transverseVelocity + rho / rhoMax); break;
    }
  // Invariants
  gammaT     = 1.0 / sqrt(1 -vT*vT);
  mTcoshDiff = mT * (cosh(rapidityS - rapidityP));
  cosDphi    = cos(phiS - phiP);
  PdotU      = gammaT * (mTcoshDiff - vT*pT* cosDphi);
  dSigmaP    = tau * rho * (mTcoshDiff - amp*pT * cosDphi);

  // Return values
  double Xt = tau * cosh(rapidityS);
  double Xx = rho * cos(phiS);
  double Xy = rho * sin(phiS);
  double Xz = tau * sinh(rapidityS);
  double Pe = mT  * cosh(rapidityP);
  double Px = pT  * cos(phiP);
  double Py = pT  * sin(phiP);
  double Pz = mT  * sinh(rapidityP);

  switch (subModelType)
    {
      default: break;
      case 3:
      case 5:
      Xt += -delay * log(gRandom->Rndm()); break;
      case 6:
      double Energy = sqrt(mT*mT + Pz*Pz);    //hypot(mT,Pz);
      Xt += -delay * log(gRandom->Rndm());
      Xx += Xt * Px / Energy;
      Xy += Xt * Py / Energy;
      Xz += Xt * Pz / Energy;
      break;
    }
  position.SetXYZT(Xx,Xy,Xz,Xt);
  momentum.SetPxPyPzE(Px,Py,Pz,Pe);
  denom = statistics + exp( (PdotU-chemPotential)/temperature );
  integrand = spinFactor * pT * dPt * dSigmaP /(CAP::Math::twoPiCube()*denom);
  return integrand;
}

