/********************************************************************************
 *                                                                              *
 *             THERMINATOR 2: THERampl heavy-IoN generATOR 2                     *
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
 * ampke modifications to the code, please, inform the authors.                  *
 * Any publication of results obtained using this code must include the         *
 * reference to arXiv:1102.0273 and the published version of it, when           *
 * available.                                                                   *
 *                                                                              *
 ********************************************************************************/

#include <sstream>
#include <TMath.h>
#include "THGlobal.hpp"
#include "Model_KrakowSFO.hpp"

using namespace TMath;
using namespace std;
ClassImp(Model_KrakowSFO);

Model_KrakowSFO::Model_KrakowSFO(const Configuration & _requestedConfiguration)
:
Model(_requestedConfiguration)
{    }

double Model_KrakowSFO::getIntegrand(ParticleType& aPartType)
{
  double dSigmaP, PdotU;
  double spinFactor, statistics, mass;
  double tau, rho, phiS, rapidityS;
  double mT,  pT,  phiP, rapidityP;
  double dPt, tauTrue;
  double chemPotential; //, temperature;
  double zeta, zetac;
  double integrand=0.0;
  double denom=1.0;

  // Type of statistics Bose-Einstein or Fermi-Dirac
  spinFactor    = aPartType.getSpinFactor();
  statistics    = aPartType.getStatistics();
  mass          = aPartType.getMass();
  chemPotential = thermodynamics->getChemicalPotential(aPartType);
  // Generate spacial components
  rho	       = rhoMax * gRandom->Rndm();
  phiS	     = TMath::TwoPi() * gRandom->Rndm();
  rapidityS	 = spatialRapidityRange * (gRandom->Rndm() - 0.5);
  tau	       = tauC; // that is the KrakowSFO Model tau that equals $\tau_{KrakowSFO}^2 = t^2 - x^2 - y^2 - z^2 = \tau^2 - \rho^2 - z^2$
  // Generate momentum components
  zeta       = gRandom->Rndm();
  zetac      = (zeta>0.9999999) ? 0.00000001 : 1.00-zeta;
  pT         = zeta/zetac;
  dPt        = 1.0/(zetac*zetac);
  phiP	     = TMath::TwoPi() * gRandom->Rndm();
  rapidityP	 = momentumRapidityRange * (gRandom->Rndm() - 0.5);;
  mT	       = sqrt(mass*mass+pT*pT);
  tauTrue    =  sqrt(tau*tau + rho*rho);  // that's the true $\tau^2 = t^2 - z^2$
  // Invariants
  PdotU	     = ( mT * tauTrue * cosh(rapidityS - rapidityP) - pT * rho * cos(phiS - phiP) ) / tau;
  dSigmaP    = tau * rho * PdotU;
  // particle X and P coordinates
  position.SetXYZT(rho*cos(phiS),rho*sin(phiS),tauTrue*sinh(rapidityS),tauTrue*cosh(rapidityS));
  momentum.SetPxPyPzE(pT*cos(phiP),pT*sin(phiP),mT*sinh(rapidityP),mT*cosh(rapidityP));
  denom      = statistics + exp( (PdotU-chemPotential)/temperature );
  integrand  = spinFactor * pT * dPt * dSigmaP /(CAP::Math::twoPiCube()*denom);
  return integrand;
}
