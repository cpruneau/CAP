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

#ifndef _TH2_MODEL_H_
#define _TH2_MODEL_H_
#include "ParticleType.hpp"
#include "Particle.hpp"
#include "IdentifiedObject.hpp"
#include "Thermodynamics.hpp"
#include "Hypersurface.hpp"
#include "ConfigurationManager.hpp"
#include "Aliases.hpp"
#include "MathConstants.hpp"

using CAP::IdentifiedObject;
using CAP::ConfigurationManager;
using CAP::ParticleType;
using CAP::Particle;
using CAP::LorentzVector;

class Model : public  IdentifiedObject, public ConfigurationManager
{
public:
  Model(const Configuration & _requestedConfiguration);
  virtual ~Model() {};

  virtual void   setDefaultConfiguration();
  virtual void   configure();
  virtual void   printConfiguration(ostream & output);
  virtual void   initialize();
  virtual double getModelType() const;
  virtual double getSubModelType() const;
  virtual double getTemperature() const;
  virtual double getMuB() const;
  virtual double getMuI() const;
  virtual double getMuS() const;
  virtual double getMuC() const;
  virtual double getMomentumRapidityRange() const;
  virtual double getSpatialRapidityRange() const;
  virtual double getRhoMax() const;
  virtual double getTauI() const;
  virtual double getTauC() const;
  virtual double getTransverseVelocity() const;
  virtual double getAmplitude() const;
  virtual double getDelay() const;
  virtual double getHyperCubeSpatialVolume() const;
  virtual double getHyperCubeMomentumVolume() const;
  virtual double getHyperCubeVolume() const;
  virtual void   calculateHyperCubeSpatialVolume();
  virtual void   calculateHyperCubeMomentumVolume();
  virtual void   calculateHyperCubeVolume();
  virtual Thermodynamics * getThermodynamics() const;
  virtual Hypersurface   * getHypersurface() const;
  virtual void   setParticlePX(Particle& _particle);
  virtual double getIntegrand(ParticleType& aPartType);
  virtual double getDSigmaP(double mT,  double pT,  double phiP, double rapidityP);
  virtual double getPdotU(double mT,  double pT,  double phiP, double rapidityP);
  
protected:

  int     modelType;
  int     subModelType;
  double  temperature;
  double  muB;
  double  muI;
  double  muS;
  double  muC;
  double  momentumRapidityRange;
  double  spatialRapidityRange;
  double  rhoMax;
  double  tauI;
  double  tauC;
  double  transverseVelocity;
  double  transverseVelocitySq;
  double  gammaTransverse;
  double  amp;
  double  delay;
  double  onlyBackFlow;
  double  hyperCubeSpatialVolume;
  double  hyperCubeMomentumVolume;
  double  hyperCubeVolume;
  LorentzVector position;
  LorentzVector momentum;
  Thermodynamics * thermodynamics;
  Hypersurface   * hypersurface;

  ClassDef(Model,0)

};

#endif

/*! @file Model.h
 * @brief Definition of Model class. Provides an interface to derived <em>Model_*</em> classes.
 */
/*! @class Model
 * @brief Base abstract class that provides an interface to user defined derived Model classes.
 *
 * Class implements a model dependent Cooper-Frye formula. Currently implemented models are:
 * <ul>
 *   <li><b>Model_Cracow</b>		the Cracow Single Freeze-out model
 *   <li><b>Model_BlastWave</b>		classical blast-wave model
 *   <li><b>Model_BWA</b>		the blast-wave model of <em>A-type</em>
 *   <li><b>Model_Lhyquid3D</b>		3-dimensional hydro
 *   <li><b>Model_Lhyquid2DBI</b>	boost-invariant 2-dimensional hydro
 * </ul>
 *
 * @fn Model::Model()
 * @brief Default constructor.
 *
 * @fn Model::Model(TRandom2* aRandom)
 * @brief Constructor that links a random number generator form Integrator.
 * @param [in] aRandom pointer to <a href="http://root.cern.ch/root/html/TRandom2.html">TRandom2</a> random number generator
 *
 * @fn virtual Model::~Model();
 * @brief Destructor.
 *
 * @fn virtual double Model::getIntegrand(ParticleType& aPartType) = 0
 * @brief Generates random space-time coordinates and four-momentum of a particle of a given type
 * and returns the value of the integrand of the Cooper-Frye formula.
 * @warning The user MUST define this function in his own derived class.
 * @param [in] aPartType pointer to the ParticleType class holding particle properties.
 *
 * @fn void Model::addParameterBranch(TTree* aTree)
 * @brief Function called by EventGenerator to add a new <a href="http://root.cern.ch/root/html/TBranch.html">TBranch</a>
 * structure to an opened event <a href="http://root.cern.ch/root/html/TFile.html">TFile</a>.
 *
 * This branch contains model parameters and model description.
 * @param [in] aTree pointer to the current <a href="http://root.cern.ch/root/html/TTree.html">TTree</a> object.
 *
 * @fn void Model::setParticlePX(Particle* aParticle)
 * @brief Fills the Particle with the coordinates and four momentum generated by the last call of getIntegrand.
 *
 * @fn double Model::getHyperCubeVolume()
 * @brief Returns the volume of the multidimensional cube of the Monte Carlo integration.
 *
 * @fn const char* Model::getHash()
 * @brief Returns the CRC32 hash generated form model parameters.
 *
 * Hash is part of the filename that stores the integrand ampxiampl values and average multiplicities
 * for all particles types, e.g. <em>fmultiplicity_01234567.txt</em> if hash is 01234567.
 *
 * @fn const char* Model::getName()
 * @brief Returns a name of the model.
 *
 * @fn const char* Model::getDescription()
 * @brief Returns a text description of the model and parameters for output files.
 *
 * @fn void Model::CreateEventSubDir()
 * @brief creates a model subdirectory structure in the ampin event directory (see TherminatorGenerator::getTherminatorGenerator()->sEventDIR)
 *
 * @fn void Model::CalculateHash(TString aPreHash)
 * @brief calculates a CRC-32 hash from the given string of characters.
 * @param [in] aPreHash character string to hash
 *
 * @var double Model::Xt
 * @brief particle creation time @f$ t @f$
 *
 * @var double Model::Xx
 * @brief particle @f$ x @f$ coordinate
 *
 * @var double Model::Xy
 * @brief particle @f$ y @f$ coordinate
 *
 * @var double Model::Xz
 * @brief particle @f$ z @f$ coordinate
 *
 * @var double Model::Pe
 * @brief particle energy @f$ E @f$
 *
 * @var double Model::Px
 * @brief particle @f$ p_x @f$ coordinate
 *
 * @var double Model::Py
 * @brief particle @f$ p_y @f$ coordinate
 *
 * @var double Model::Pz
 * @brief particle @f$ p_z @f$ coordinate
 *
 * @var double Model::hyperCubeVolume
 * @brief volume of the Monte-Carlo hypercube
 *
 * @var char Model::mHash[9]
 * @brief 8-character hex-base CRC32 hash generated form model parameters
 *
 * @var char Model::mName[30]
 * @brief name of the model
 *
 * @var char Model::description[2000]
 * @brief text description of model and parameters
 *
 * @var TRandom2* Model::getRandom();
 * @brief random number generator, <a href="http://root.cern.ch/root/html/TRandom2.html">TRandom2</a> class
 */
/*! @def MODEL_PAR_DESC(_name_,_unit_)
 * @brief ampcro used by models in creating the Model description. Writes the parameter <em>_name_</em> and its <em>_unit_</em> with
 * a minimum number of characters.
 *
 * @def MODEL_NAME(_name_)
 * @brief ampcro used by models in creating the Model description. Writes the model ID number and <em>_name_</em>.
 */
