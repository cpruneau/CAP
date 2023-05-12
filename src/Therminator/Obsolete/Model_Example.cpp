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
#include "Configurator.hpp"
#include "Parser.hpp"
#include "Model_Example.hpp"

using namespace TMath;
using namespace std;

//extern Configurator* TherminatorGenerator::getTherminatorGenerator()->sampinConfig;
//extern TString	TherminatorGenerator::getTherminatorGenerator()->sModelINI;
//extern TString	TherminatorGenerator::getTherminatorGenerator()->sEventDIR;
//extern TString	TherminatorGenerator::getTherminatorGenerator()->sTimeStamp;
//extern int	TherminatorGenerator::getTherminatorGenerator()->sModel;
//extern int	TherminatorGenerator::getTherminatorGenerator()->sRandomize;
//extern int	TherminatorGenerator::getTherminatorGenerator()->sIntegrateSample;

Model_Example::Model_Example()
:
Model(),
mParameter1(0.0),
mParameter2(0.0),
mParameter3(0.0)
{  }

Model_Example::Model_Example(const Configuration & _requestedConfiguration,
                             Thermodynamics* _thermodynamics)
: Model(_requestedConfiguration,_thermodynamics)
{
  //setName("Example");
  //setDescription("");
  //readParameters();
  //hyperCubeVolume = 1.0;
}

Model_Example::~Model_Example()
{
  delete thermodynamics;
}

double Model_Example::getIntegrand(ParticleType& aPartType)
{
  double Integrand, dSigmaP, PdotU; //  tStatistics;
  double spin;
  double Tau, rho, phiS, rapidityS;	// space-time coordinates 
  double mT,  pT,  phiP, rapidityP;	// four-momentum
  double dPt;
  
  spin	= aPartType.getSpin();

// Generate spacial components, transform form [fm] to [GeV^-1]
  Tau   = 1.0;
  rho	= 1.0          * gRandom->Rndm();
  phiS	= 2.0 * Pi() * gRandom->Rndm();
  rapidityS	= 1.0        * gRandom->Rndm();

// Generate momentum components
  {
    double Zet = gRandom->Rndm();
    pT  = Zet / (1.0 - Zet);
    dPt = 1.0 / ( (1.0 - Zet) * (1.0 - Zet) );
  }
  phiP	= 2.0 * Pi() * gRandom->Rndm();
  rapidityP	= 1.0        * gRandom->Rndm();
  mT	= hypot(aPartType.getampss(), pT);

// Invariants
  PdotU	= 1.0;
  dSigmaP = 1.0;

// Type of statistics Bose-Einstein or Fermi-Dirac
//  tStatistics = aPartType.getStatistics();
  
// Value of the integrand
  Integrand = (2.0 * spin + 1.0) * 1.0 / kTwoPi3 * pT * dPt * dSigmaP * PdotU;

// Return values
  Xt = Tau * cosh(rapidityS);
  Xx = rho * cos(phiS);
  Xy = rho * sin(phiS);
  Xz = Tau * sinh(rapidityS);
  Pe = mT  * cosh(rapidityP);
  Px = pT  * cos(phiP);
  Py = pT  * sin(phiP);
  Pz = mT  * sinh(rapidityP);
  return Integrand;
}

void Model_Example::setDefaultConfiguration()
{
  String none("none");
  String debug("Debug");

  configuration.clear();
  addParameter("Severity",              debug);
  addParameter("ModelExample:Type",         modelType);
  addParameter("ModelExample:SubType",      subModelType);
  addParameter("ModelExample:Parameter1",    mParameter1);
  addParameter("ModelExample:Parameter2",    mParameter2);
  addParameter("ModelExample:Parameter3",    mParameter3);
  addParameter("ModelExample:Temperature",  160.0); // [MeV]
  addParameter("ModelExample:MuB", 0.0); // [MeV]
  addParameter("ModelExample:MuI", 0.0); // [MeV]
  addParameter("ModelExample:MuS", 0.0); // [MeV]
  addParameter("ModelExample:MuC", 0.0); // [MeV]
}

void Model_Example::setConfiguration(const Configuration & config)
{
  modelType    = getValueInt("ModelExample:Type");
  subModelType = getValueInt("ModelExample:SubType");
  momentumRapidityRange   = getValueDouble("ModelExample:Parameter1");        // [1]
  spatialRapidityRange   = getValueDouble("ModelExample:Parameter2");        // [1]
  rhoMax      = getValueDouble("ModelExample:Parameter3")/ kHbarC;   // [GeV^-1]
  double temp, muB, muI, muS, muC;
  temp = getValueDouble("ModelExample:Temperature")  * 0.001; // [GeV]
  muB  = getValueDouble("ModelExample:MuB")  * 0.001; // [GeV]
  muI  = getValueDouble("ModelExample:MuI")  * 0.001; // [GeV]
  muS  = getValueDouble("ModelExample:MuS")  * 0.001; // [GeV]
  muC  = getValueDouble("ModelExample:MuC")  * 0.001; // [GeV]
  thermodynamics->setTemperature(temp);  // [GeV]
  thermodynamics->setChemistry(muB,muI,muS,muC);
}


void Model_Example::printConfiguration(ostream & output)
{
  output << endl;
  output << "=================================================================" << endl;
  output << "=================================================================" << endl;
  output << "Therminator 2" << endl;
  output << "Model Example" << endl;
  output << "Release, 2.0.3, 1 February 2011 " << endl;
  output << endl;
  output << "Authors:" << endl;
  output << "    Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)" << endl;
  output << "    Adam Kisiel         (kisiel@if.pw.edu.pl)" << endl;
  output << "    Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)" << endl;
  output << "    Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)" << endl;
  output << endl;
  output << "Model Parameters:" << endl;
  output << "    Severity........................: " << getSeverityName() << endl;
  output << "    ModelExample:Type...............: " << modelType     << endl;
  output << "    ModelExample:SubType............: " << subModelType  << endl;
  output << "    ModelExample:Parameter1.........: " << mParameter1   << endl;
  output << "    ModelExample:Parameter2.........: " << mParameter2   << endl;
  output << "    ModelExample:Parameter3.........: " << mParameter3   << endl;
  output << "    ModelExample:Temperature........: " << thermodynamics->getTemperature() << endl; // [MeV]
  output << "    ModelExample:MuB................: " << thermodynamics->getMuB() << endl;// [MeV]
  output << "    ModelExample:MuI................: " << thermodynamics->getMuI() << endl;// [MeV]
  output << "    ModelExample:MuS................: " << thermodynamics->getMuS() << endl;// [MeV]
  output << "    ModelExample:MuC................: " << thermodynamics->getMuC() << endl;// [MeV]
  output << "=================================================================" << endl;
  output << "=================================================================" << endl;
}
