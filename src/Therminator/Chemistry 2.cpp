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

//#include <TMath.h>
#include "Chemistry.hpp"
using TMath::Power;

ClassImp(Chemistry);


Chemistry::Chemistry()
:
chemistryType(-1),
muBConst(0.0),
muIConst(0.0),
muSConst(0.0),
muCConst(0.0),
gammaQConst(1.0),
gammaSConst(1.0),
gammaCConst(1.0),
lambdaIConst(1.0),
lambdaQConst(1.0),
lambdaSConst(1.0),
lambdaCConst(1.0),
muBVar(0),
muIVar(0),
muSVar(0),
muCVar(0),
gammaQVar(0),
gammaSVar(0),
gammaCVar(0),
lambdaIVar(0),
lambdaQVar(0),
lambdaSVar(0),
lambdaCVar(0)
{
}

Chemistry::~Chemistry()
{
  if (muBVar) delete muBVar;
  if (muIVar) delete muIVar;
  if (muSVar) delete muSVar;
  if (muCVar) delete muCVar;
  if (lambdaQVar) delete lambdaQVar;
  if (lambdaIVar) delete lambdaIVar;
  if (lambdaSVar) delete lambdaSVar;
  if (lambdaCVar) delete lambdaCVar;
  if (gammaQVar) delete gammaQVar;
  if (gammaSVar) delete gammaSVar;
  if (gammaCVar) delete gammaCVar;
}

double Chemistry::getChemicalPotential(ParticleType & aPartType) const
{
  return (aPartType.getBaryonNumber()  * muBConst +
          aPartType.getIsospin3()      * muIConst +
          aPartType.getNetStrangeness() * muSConst +
          aPartType.getNetCharm()   * muCConst );
}

double Chemistry::getChemicalPotential(ParticleType & aPartType, double aX, double aY, double aZ) const
{
  return (aPartType.getBaryonNumber()  * muBVar->interpolate(aX, aY, aZ) +
          aPartType.getIsospin3()      * muIVar->interpolate(aX, aY, aZ) +
          aPartType.getNetStrangeness() * muSVar->interpolate(aX, aY, aZ) +
          aPartType.getNetCharm()   * muCVar->interpolate(aX, aY, aZ) );
}

double Chemistry::getFugacity(ParticleType & aPartType) const
{
  return lambdaIConst
  * Power(gammaQConst,  aPartType.getNumberQ() + aPartType.getNumberAQ())
  * Power(lambdaQConst, aPartType.getNumberQ() - aPartType.getNumberAQ())
  * Power(gammaSConst,  aPartType.getNumberS() + aPartType.getNumberAS())
  * Power(lambdaSConst, aPartType.getNumberS() - aPartType.getNumberAS())
  * Power(gammaCConst,  aPartType.getNumberC() + aPartType.getNumberAC())
  * Power(lambdaCConst, aPartType.getNumberC() - aPartType.getNumberAC());
}

double Chemistry::getFugacity(ParticleType & aPartType, double aX, double aY, double aZ) const
{
  return lambdaIVar->interpolate(aX, aY, aZ)
  * Power(gammaQVar ->interpolate(aX, aY, aZ), aPartType.getNumberQ() + aPartType.getNumberAQ())
  * Power(lambdaQVar->interpolate(aX, aY, aZ), aPartType.getNumberQ() - aPartType.getNumberAQ())
  * Power(gammaSVar ->interpolate(aX, aY, aZ), aPartType.getNumberS() + aPartType.getNumberAS())
  * Power(lambdaSVar->interpolate(aX, aY, aZ), aPartType.getNumberS() - aPartType.getNumberAS())
  * Power(gammaCVar ->interpolate(aX, aY, aZ), aPartType.getNumberC() + aPartType.getNumberAC())
  * Power(lambdaCVar->interpolate(aX, aY, aZ), aPartType.getNumberC() - aPartType.getNumberAC());
}

int    Chemistry::getChemistryType() const	{ return chemistryType; }
double Chemistry::getMuB() const		{ return muBConst; }
double Chemistry::getMuI() const		{ return muIConst; }
double Chemistry::getMuS() const		{ return muSConst; }
double Chemistry::getMuC() const		{ return muCConst; }
double Chemistry::getLambdaQ() const		{ return lambdaQConst; }
double Chemistry::getLambdaI() const		{ return lambdaIConst; }
double Chemistry::getLambdaS() const		{ return lambdaSConst; }
double Chemistry::getLambdaC() const		{ return lambdaCConst; }
double Chemistry::getGammaQ() const		{ return gammaQConst; }
double Chemistry::getGammaS() const		{ return gammaSConst; }
double Chemistry::getGammaC() const		{ return gammaCConst; }

void Chemistry::setChemistry(double aMuB, double aMuI, double aMuS, double aMuC)
{
  chemistryType = 0;
  muBConst = aMuB;
  muIConst = aMuI;
  muSConst = aMuS;
  muCConst = aMuC;
}
void Chemistry::setChemistry(VectorField* aMuB, VectorField* aMuI, VectorField* aMuS, VectorField* aMuC)
{
  chemistryType = 1;
  muBVar = aMuB; muIVar = aMuI; muSVar = aMuS;; muCVar = aMuC;
}
void Chemistry::setChemistry(double aLambdaQ, double aLambdaI, double aLambdaS, double aLambdaC, double aGamampQ, double aGamampS,  double aGamampC)
{
  chemistryType = 2;
  lambdaQConst = aLambdaQ;
  lambdaIConst = aLambdaI;
  lambdaSConst = aLambdaS;
  lambdaCConst = aLambdaC;
  gammaQConst  = aGamampQ;
  gammaSConst  = aGamampS;
  gammaCConst  = aGamampC;
}
void Chemistry::setChemistry(VectorField* aLambdaQ, VectorField* aLambdaI, VectorField* aLambdaS, VectorField* aLambdaC, VectorField* aGamampQ, VectorField* aGamampS, VectorField* aGamampC)
{
  chemistryType = 3;
  lambdaQVar = aLambdaQ;
  lambdaIVar = aLambdaI;
  lambdaSVar = aLambdaS;
  lambdaCVar = aLambdaC;
  gammaQVar  = aGamampQ;
  gammaSVar  = aGamampS;
  gammaCVar  = aGamampC;
}
