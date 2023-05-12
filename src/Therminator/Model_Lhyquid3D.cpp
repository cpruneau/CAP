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

#include "Model_Lhyquid3D.hpp"
#include "Hypersurface_Lhyquid3D.hpp"

ClassImp(Model_Lhyquid3D);

Model_Lhyquid3D::Model_Lhyquid3D(const Configuration & _requestedConfiguration)
:
Model(_requestedConfiguration)
{   }

double Model_Lhyquid3D::getIntegrand(ParticleType& aPartType)
{
  double dSigmaP, PdotU;
  double spinFactor, statistics, mass;
  double mT,  pT,  phiP, rapidityP;
  double dPt;
  double chemPotential, temperature;
  double zeta, zetac;
  double integrand=0.0;
  double denom=1.0;

// Type of statistics Bose-Einstein or Fermi-Dirac
  spinFactor    = aPartType.getSpinFactor();
  statistics    = aPartType.getStatistics();
  mass          = aPartType.getMass();
  chemPotential = thermodynamics->getChemicalPotential(aPartType);
  temperature   = thermodynamics->getTemperature();
// Generate random position on the hypersurface
  hypersurface->setPositionOnHypersurface(position);
// Generate momentum components
  zeta  = gRandom->Rndm();
  zetac = (zeta>0.9999999) ? 0.00000001 : 1.00-zeta;
  pT    = zeta/zetac;
  dPt    = 1.0/(zetac*zetac);
  phiP	= TMath::TwoPi() * gRandom->Rndm();
  //rapidityP  = momentumRapidityRange * gRandom->Rndm() - 0.5 * momentumRapidityRange;
  rapidityP  = momentumRapidityRange * (gRandom->Rndm() - 0.5);
  mT	= sqrt(mass*mass+pT*pT);
// d Sigamp_\mu p^\mu
  dSigmaP = getDSigmaP(mT, pT, phiP, rapidityP);
  if (onlyBackFlow)
    {
    // only particle emission directed back to the hydro-region
    if(dSigmaP >= 0.0)
      dSigmaP = 0.0;
    else
      dSigmaP = -dSigmaP;
    }
  else
    {
    // disable particle emission back to the hydro-region
    if (dSigmaP < 0.0) dSigmaP = 0.0;
    }
  momentum.SetPxPyPzE(pT*cos(phiP),pT*sin(phiP),mT*sinh(rapidityP),mT*cosh(rapidityP));
  PdotU     = getPdotU(mT, pT, phiP, rapidityP);
  denom     = statistics + exp( (PdotU-chemPotential)/temperature );
  integrand = spinFactor * pT * dPt * dSigmaP /(CAP::Math::twoPiCube()*denom);
  return integrand;
}

void   Model_Lhyquid3D::initialize()
{
  Model::initialize();
  hypersurface = new Hypersurface_Lhyquid3D(*requestedConfiguration,thermodynamics);
}
