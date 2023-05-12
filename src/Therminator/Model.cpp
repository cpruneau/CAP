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

//#include <sys/stat.h>
#include "Model.hpp"
#include "PhysicsConstants.hpp"

using namespace std;
ClassImp(Model);


Model::Model(const Configuration & _requestedConfiguration)
:
IdentifiedObject(),
ConfigurationManager(_requestedConfiguration),
modelType(0),
subModelType(0),
temperature(0),
muB(0),
muI(0),
muS(0),
muC(0),
momentumRapidityRange(0),
spatialRapidityRange(0),
rhoMax(1.0),
tauI(0),
tauC(0),
transverseVelocity(0),
transverseVelocitySq(0),
gammaTransverse(0.0),
amp(0),
delay(0),
onlyBackFlow(0),
hyperCubeSpatialVolume(0.0),
hyperCubeMomentumVolume(0.0),
hyperCubeVolume(0.0),
position(),
momentum(),
thermodynamics(nullptr),
hypersurface(nullptr)
{   }

double Model::getModelType() const
{
  return modelType;
}

double Model::getSubModelType() const
{
  return subModelType;
}


double Model::getTemperature() const
{
  return temperature;
}

double Model::getMuB() const
{
  return muB;
}

double Model::getMuI() const
{
  return muI;
}

double Model::getMuS() const
{
  return muS;
}

double Model::getMuC() const
{
  return muC;
}



double Model::getMomentumRapidityRange() const
{
  return momentumRapidityRange;
}

double Model::getSpatialRapidityRange() const
{
  return spatialRapidityRange;
}

double Model::getRhoMax() const
{
  return rhoMax;
}
double Model::getTauI() const
{
  return tauI;
}

double Model::getTauC() const
{
  return tauC;
}

double Model::getTransverseVelocity() const
{
  return transverseVelocity;
}

double Model::getAmplitude() const
{
  return amp;
}

double Model::getDelay() const
{
  return delay;
}

void Model::calculateHyperCubeSpatialVolume()
{
  if (hypersurface)
    hyperCubeSpatialVolume = hypersurface->getHyperCubeSpatialVolume();
  else
    hyperCubeSpatialVolume = CAP::Math::twoPi() * spatialRapidityRange;
}

void Model::calculateHyperCubeMomentumVolume()
{
  hyperCubeMomentumVolume = CAP::Math::twoPi() * rhoMax * momentumRapidityRange;
}

void Model::calculateHyperCubeVolume()
{
  calculateHyperCubeSpatialVolume();
  calculateHyperCubeMomentumVolume();
  hyperCubeVolume = hyperCubeSpatialVolume * hyperCubeMomentumVolume;

//  cout << " hyperCubeSpatialVolume: " << hyperCubeSpatialVolume << endl;
//  cout << "hyperCubeMomentumVolume: " << hyperCubeMomentumVolume << endl;
//  cout << "        hyperCubeVolume: " << hyperCubeVolume << endl;
//  cout << "       lightVelocity_SI: " << CAP::Physics::lightVelocity_SI() << endl;
//  cout << "               hBarC_SI: " << CAP::Physics::hBarC_SI() << endl;
//  cout << "    CAP::Physics::hBarC: " << CAP::Physics::hBarC() << endl;
//  cout << "   CAP::Physics::hBarC2: " << CAP::Physics::hBarC2() << endl;
//  cout << "   CAP::Physics::hBarC3: " << CAP::Physics::hBarC3() << endl;
//  cout << "   spatialRapidityRange: " << spatialRapidityRange << endl;
//  cout << "                 rhoMax: " << rhoMax << endl;
//  cout << "  momentumRapidityRange: " << momentumRapidityRange << endl;
//  exit(1);
}

double Model::getHyperCubeSpatialVolume()  const
{
  return hyperCubeSpatialVolume;
}

double Model::getHyperCubeMomentumVolume() const
{
  return hyperCubeMomentumVolume;
}

double Model::getHyperCubeVolume() const
{
  return hyperCubeVolume;
}

Thermodynamics * Model::getThermodynamics() const
{
  return thermodynamics;
}

Hypersurface   * Model::getHypersurface() const
{
  return hypersurface;
}

void Model::setParticlePX(Particle& _particle)
{
  _particle.setXYZT(position);
  _particle.setPxPyPzE(momentum);
}

void   Model::setDefaultConfiguration()
{
  ConfigurationManager::setDefaultConfiguration();
  addParameter("ModelType",               0);
  addParameter("SubModelType",            0);
  addParameter("Temperature",             160.0);
  addParameter("MuB",                     0.0);
  addParameter("MuI",                     0.0);
  addParameter("MuS",                     0.0);
  addParameter("MuC",                     0.0);
  addParameter("MomentumRapidityRange",   momentumRapidityRange);
  addParameter("SpatialRapidityRange",    spatialRapidityRange);
  addParameter("RhoMax",                  rhoMax);
  addParameter("TauI",                    tauI);
  addParameter("TauC",                    tauC);
  addParameter("TransverseVelocity",      transverseVelocity);
  addParameter("Amplitude",               amp);
  addParameter("Delay",                   delay);
  addParameter("OnlyBackFlow",            onlyBackFlow);
}

void   Model::configure()
{
  ConfigurationManager::configure();
  modelType             = getValueInt("ModelType");
  subModelType          = getValueInt("SubModelType");
  temperature           = getValueDouble("Temperature") * 0.001;
  muB                   = getValueDouble("MuB") * 0.001;
  muI                   = getValueDouble("MuI") * 0.001;
  muS                   = getValueDouble("MuS") * 0.001;
  muC                   = getValueDouble("MuC") * 0.001;
  momentumRapidityRange = getValueDouble("MomentumRapidityRange");
  spatialRapidityRange  = getValueDouble("SpatialRapidityRange");
  rhoMax                = getValueDouble("RhoMax")/ CAP::Physics::hBarC(); // converts to fm
  tauI                  = getValueDouble("TauI")  / CAP::Physics::hBarC();
  tauC                  = getValueDouble("TauC")  / CAP::Physics::hBarC();
  transverseVelocity    = getValueDouble("TransverseVelocity");
  amp                   = getValueDouble("Amplitude");
  delay                 = getValueDouble("Delay");
  onlyBackFlow          = getValueDouble("OnlyBackFlow");

  transverseVelocitySq  = transverseVelocity*transverseVelocity;
  gammaTransverse       = 1.0/sqrt(1.0 - transverseVelocitySq);
  if (reportDebug(__FUNCTION__)) printConfiguration(cout);
}

void   Model::printConfiguration(ostream & output)
{
  output << endl;
  output << "==================================================================";
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
  ConfigurationManager::printConfiguration(output);
  output << "==================================================================";
}

void   Model::initialize()
{
  configure();
  calculateHyperCubeVolume();
  thermodynamics = new Thermodynamics();
  thermodynamics->setTemperature(temperature);
  thermodynamics->setChemistry(muB,muI,muS,muC);
  hypersurface   = nullptr;
}


double Model::getIntegrand(ParticleType& aPartType __attribute__((unused)))
{
  return -1.0;
}

double Model::getDSigmaP(double mT  __attribute__((unused)),
                         double pT  __attribute__((unused)),
                         double phiP  __attribute__((unused)),
                         double rapidityP  __attribute__((unused)) )
{
  return -1.0;
}

double Model::getPdotU(double mT  __attribute__((unused)),
                       double pT  __attribute__((unused)),
                       double phiP  __attribute__((unused)),
                       double rapidityP  __attribute__((unused)) )
{
  return -1.0;
}
