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
#include "Hypersurface.hpp"

ClassImp(Hypersurface);

using namespace std;
using namespace TMath;

Hypersurface::Hypersurface()
:
ConfigurationManager(),
deviceName(""),
collidingSystem(""),
lambda(0.0),
momentumRapidityRange(0.0),
spatialRapidityRange(0.0),
tauI(0.0),
collidingEnergy(0.0),
centralityMin(0.0),
centralityMax(0.0),
impactParameter(0.0),
tempI(0.0),
zeta(0.0),
phiS(0.0),
Theta(0.0),
Tau(0.0),
rho(0.0),
rapidityS(0.0),
Dhs(0.0),
dDdPhi(0.0),
dDdZeta(0.0),
dDdTheta(0.0),
vT(0.0),
gammaT(0.0),
phiF(0.0),
thermodynamics(nullptr),
mDistance(nullptr),
mDistanceDPhi(nullptr),
mDistanceDZeta(nullptr)
{    }

Hypersurface::Hypersurface(const Configuration & _requestedConfiguration,
                           Thermodynamics * _thermodynamics)
:
ConfigurationManager(_requestedConfiguration),
deviceName(""),
collidingSystem(""),
lambda(0.0),
momentumRapidityRange(0.0),
spatialRapidityRange(0.0),
tauI(0.0),
collidingEnergy(0.0),
centralityMin(0.0),
centralityMax(0.0),
impactParameter(0.0),
tempI(0.0),
zeta(0.0),
phiS(0.0),
Theta(0.0),
Tau(0.0),
rho(0.0),
rapidityS(0.0),
Dhs(0.0),
dDdPhi(0.0),
dDdZeta(0.0),
dDdTheta(0.0),
thermodynamics(_thermodynamics),
mDistance(nullptr),
mDistanceDPhi(nullptr),
mDistanceDZeta(nullptr)
{    }

void Hypersurface::setDefaultConfiguration()
{
  ConfigurationManager::setDefaultConfiguration();
  addParameter("DeviceName",        deviceName);
  addParameter("CollidingSystem",   collidingSystem);
  addParameter("Lambda",            lambda);
  addParameter("RapPRange",         momentumRapidityRange);
  addParameter("RapSRange",         spatialRapidityRange);
  addParameter("TauI",              tauI);
  addParameter("CollidingEnergy",   collidingEnergy);
  addParameter("CentralityMin",     centralityMin);
  addParameter("CentralityMax",     centralityMax);
  addParameter("ImpactParameter",   impactParameter);
  addParameter("TempI",             tempI);
}

void Hypersurface::configure()
{
  ConfigurationManager::configure();
  deviceName           = getValueString("DeviceName");
  collidingSystem      = getValueString("CollidingSystem");
  lambda               = getValueDouble("Lambda");
  momentumRapidityRange= getValueDouble("RapPRange");
  spatialRapidityRange = getValueDouble("RapSRange");
  tauI                 = getValueDouble("TauI");
  collidingEnergy      = getValueDouble("CollidingEnergy");
  centralityMin        = getValueDouble("CentralityMin");
  centralityMax        = getValueDouble("CentralityMax");
  impactParameter      = getValueDouble("ImpactParameter");
  tempI                = getValueDouble("TempI");
  if (reportDebug(__FUNCTION__)) printConfiguration(cout);
}

void Hypersurface::printConfiguration(ostream & output)
{
  output << endl;
  output << "=================================================================" << endl;
  output << "=================================================================" << endl;
  output << endl;
  output << "Therminator 2" << endl;
  output << "Model BWA" << endl;
  output << "Release, 2.0.3, 1 February 2011 " << endl;
  output << endl;
  output << "Authors:" << endl;
  output << "    Mikolaj Chojnacki   (Mikolaj.Chojnacki@ifj.edu.pl)" << endl;
  output << "    Adam Kisiel         (kisiel@if.pw.edu.pl)" << endl;
  output << "    Wojciech Broniowski (Wojciech.Broniowski@ifj.edu.pl)" << endl;
  output << "    Wojciech Florkowski (Wojciech.Florkowski@ifj.edu.pl)" << endl;
  output << endl;
  output << "Hypersurface Model Parameters:" << endl;
  output << "    Severity...........: " << getSeverityName() << endl;
  output << "    DeviceName.........: " << deviceName << endl;
  output << "    CollidingSystem....: " << collidingSystem << endl;
  output << "    Lambda.............: " << lambda << endl;
  output << "    RapPRange..........: " << momentumRapidityRange << endl;
  output << "    RapSRange..........: " << spatialRapidityRange << endl;
  output << "    TauI...............: " << tauI << endl;
  output << "    ollidingEnergy.....: " << collidingEnergy << endl;
  output << "    CentralityMin......: " << centralityMin << endl;
  output << "    CentralityMax......: " << centralityMax << endl;
  output << "    ImpactParameter....: " << impactParameter << endl;
  output << "    TempI..............: " << tempI << endl;
  output << "    Temperature........: " << thermodynamics->getTemperature() << endl; // [MeV]
  output << "    MuB................: " << thermodynamics->getMuB() << endl;// [MeV]
  output << "    MuI................: " << thermodynamics->getMuI() << endl;// [MeV]
  output << "    MuS................: " << thermodynamics->getMuS() << endl;// [MeV]
  output << "    MuC................: " << thermodynamics->getMuC() << endl;// [MeV]
  output << "=================================================================" << endl;
  output << "=================================================================" << endl;
}

double Hypersurface::getHyperCubeSpatialVolume() const
{
  double sizeX = mDistance->getXMax() - mDistance->getXMin();
  double sizeY = mDistance->getYMax() - mDistance->getYMin();
  return sizeX * sizeY * momentumRapidityRange;
}
