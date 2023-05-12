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
#include "Model_BlastWave.hpp"
ClassImp(Model_BlastWave);

Model_BlastWave::Model_BlastWave(const Configuration & _requestedConfiguration)
:
Model(_requestedConfiguration)
{   }

double Model_BlastWave::getIntegrand(ParticleType& aPartType)
{
  double dSigmaP, PdotU;
  double spinFactor, statistics, mass;
  double Tau, rho, phiS, rapidityS;
  double mT,  pT,  phiP, rapidityP;
  double dPt;
  double chemPotential; //, temperature;
  double zeta, zetac;
  double integrand=0.0;
  double denom=1.0;
// Type of statistics Bose-Einstein or Fermi-Dirac
  spinFactor = aPartType.getSpinFactor();
  statistics = aPartType.getStatistics();
  mass       = aPartType.getMass();
  chemPotential = thermodynamics->getChemicalPotential(aPartType);
  //temperature   = thermodynamics->getTemperature();
  // Generate spacial components
  rho	  = rhoMax * gRandom->Rndm();
  phiS	= TMath::TwoPi() * gRandom->Rndm();
  rapidityS  = spatialRapidityRange * (gRandom->Rndm() - 0.5);
  Tau	= tauI;
// Generate momentum components
  zeta  = gRandom->Rndm();
  zetac = (zeta>0.9999999) ? 0.00000001 : 1.00-zeta;
  pT	  = zeta/zetac;
  dPt	  = 1.0/(zetac*zetac);

  phiP	= TMath::TwoPi() * gRandom->Rndm();
  mT    = sqrt(mass*mass+pT*pT);
  rapidityP	= momentumRapidityRange * (gRandom->Rndm() - 0.5);
// Invariants
 // double vT     = transverseVelocity;
 // double gammaT = 1.0 / sqrt(1.000 - vT * vT);
  double mTcoshDiff = mT * (cosh(rapidityS - rapidityP));
  PdotU	  = gammaTransverse * (mTcoshDiff - transverseVelocity * pT * cos(phiS - phiP));
  dSigmaP = Tau * rho * mTcoshDiff;
// particle X and P coordinates
  position.SetXYZT(rho*cos(phiS),rho*sin(phiS),Tau*sinh(rapidityS),Tau*cosh(rapidityS));
  momentum.SetPxPyPzE(pT*cos(phiP),pT*sin(phiP),mT*sinh(rapidityP),mT*cosh(rapidityP));
// integrand
  denom     = statistics + exp( (PdotU-chemPotential)/temperature );
  integrand = spinFactor * pT * dPt * dSigmaP /(CAP::Math::twoPiCube()*denom);

//  if (integrand<0 && calls<10)
//    {
//    calls++;
//    cout << endl;
//    cout << " spatialRapidityRange : " << spatialRapidityRange << endl;
//    cout << "momentumRapidityRange : " << momentumRapidityRange << endl;
//    cout << "        rhoMax : " << rhoMax << endl;
//    cout << "           Tau : " << Tau<< endl;
//    cout << " rhoMax*kHbarC : " << rhoMax*kHbarC << endl;
//    cout << "    Tau*kHbarC : " << Tau*kHbarC    << endl;
//    cout << "           muB : " << muB<< endl;
//    cout << "           muI : " << muI<< endl;
//    cout << "           muS : " << muS<< endl;
//    cout << "           muC : " << muC<< endl;
//    cout << " transverseVel : " << transverseVelocity << endl;
//    cout << "          spin : " << aPartType.getSpin() << endl;
//    cout << "    spinFactor : " << aPartType.getSpinFactor() << endl;
//    cout << "    statistics : " << statistics << endl;
//    cout << "          mass : " << mass << endl;
//    cout << " chemPotential : " << chemPotential << endl;
//    cout << "   temperature : " << temperature << endl;
//    cout << "          zeta : " << zeta << endl;
//    cout << "         zetac : " << zetac << endl;
//    cout << "            pT : " << pT << endl;
//    cout << "           dPt : " << dPt << endl;
//    cout << "            mT : " << mT << endl;
//    cout << "       kTwoPi3 : " << kTwoPi3 << endl;
//    cout << "         denom : " << denom << endl;
//    cout << "     integrand : " << integrand << endl;
//    }

  return integrand;
}

