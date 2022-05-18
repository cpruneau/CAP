// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class HadronGasGeneratorTask
 \ingroup WAC

 Class defining HadronGasGeneratorTask
 */
#include "TParameter.h"
#include "HadronGasGeneratorTask.hpp"

ClassImp(HadronGasGeneratorTask);

HadronGasGeneratorTask::HadronGasGeneratorTask(const TString  &        _name,
                                               Configuration  &        _configuration,
                                               vector<EventFilter*>&   _eventFilters,
                                               vector<ParticleFilter*>&_particleFilters,
                                               LogLevel                _selectedLevel)
:
Task(_name,_configuration,_eventFilters,_particleFilters, _selectedLevel),
particleTypes(nullptr),
stableParticleTypes(nullptr),
hadronGases(),
momentumGenerators(),
standaloneMode(true),
minTotalMult(0),
maxTotalMult(1),
rangeTotalMult(1),
minY(-1),
maxY(1),
rangeY(2)
{
  appendClassName("HadronGasGeneratorTask");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void HadronGasGeneratorTask::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration config = getConfiguration();
  config.addParameter("useParticles", true);
  config.addParameter("useEventStream0",true);
  config.addParameter("standaloneMode",  true);
  config.addParameter("nKineticTemp",       1);
  config.addParameter("minKineticTemp",   0.0);
  config.addParameter("maxKineticTemp", 160.0);
  config.addParameter("nChemicalTemp",      1);
  config.addParameter("minChemicalTemp",  150.0);
  config.addParameter("maxChemicalTemp",  240.0);
  config.addParameter("chemicalTemp",     200.0);
  config.addParameter("muB",                0.0);
  config.addParameter("muS",                0.0);
  config.addParameter("volume",             1.0);
  config.addParameter("minTotalMult",         1);
  config.addParameter("maxTotalMult",       100);
  config.addParameter("minY",              -2.0);
  config.addParameter("maxY",               2.0);
  config.addParameter("GeneratorType",        0);
  
  config.addParameter("nP",      200);
  config.addParameter("minP",    0.0);
  config.addParameter("maxP",    5.0);

  config.addParameter("nT",       50);
  config.addParameter("minT",  100.0);
  config.addParameter("maxT",  200.0);
  config.addParameter("nMass",    50);
  config.addParameter("minMass", 0.0);
  config.addParameter("maxMass", 3.0);
  config.addParameter("nMu",                 20);
  config.addParameter("minMu",              0.0);
  config.addParameter("maxMu",              1.0);
  config.addParameter("nMuB",                20);
  config.addParameter("minMuB",             0.0);
  config.addParameter("maxMuB",            20.0);
  config.addParameter("nMuS",                20);
  config.addParameter("minMuS",             0.0);
  config.addParameter("maxMuS",            20.0);
  config.addParameter("nMuQ",                20);
  config.addParameter("minMuQ",             0.0);
  config.addParameter("maxMuQ",            20.0);
                      
}

void HadronGasGeneratorTask::initialize()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  const Configuration & config = getConfiguration();
  standaloneMode = config.getValueBool("standaloneMode");
  minTotalMult   = config.getValueInt("minTotalMult");
  maxTotalMult   = config.getValueInt("minTotalMult");
  rangeTotalMult = maxTotalMult - minTotalMult;
  minY = config.getValueInt("minY");
  maxY = config.getValueInt("maxY");
  rangeY = maxY - minY;
  double temperature = config.getValueDouble("chemicalTemp");
  double muB         = config.getValueDouble("muB");
  double muS         = config.getValueDouble("muS");
  
  particleTypes       = ParticleTypeCollection::getMasterParticleCollection();
  stableParticleTypes = particleTypes->extractCollection(1);
  if (reportDebug("HadronGasGeneratorTask",getName(),"initialize()"))
    {
    stableParticleTypes->printProperties(std::cout);
    }
 
  TString label = "T";
  label += int(1000*temperature);
  HadronGas * gas = new HadronGas(particleTypes,stableParticleTypes,getLogLevel());
  gas->setName(label);
  gas->calculateAllProperties(temperature,muB,muS);
  if (reportDebug("HadronGasGeneratorTask",getName(),"initialize()"))
    {
    gas->printProperties(std::cout);
    }
  hadronGases.push_back(gas);

  if (reportDebug("HadronGasGeneratorTask",getName(),"initialize()"))
    {
    cout << "Setting up momentum generation parameters." << endl;
    }
  int generatorType = config.getValueInt("generatorType");
  vector<double> parameters;
  parameters.clear();
  switch (generatorType)
    {
      case 0:
      case 1:
      case 2:
      parameters.push_back(temperature);
      break;

      case 3:
      case 4:
      parameters.push_back(0.001);
      parameters.push_back(10.00);
      break;

      case 5:
      case 6:
      parameters.push_back(minY);
      parameters.push_back(rangeY);
      parameters.push_back(temperature);
      break;

      case 7:
      parameters.push_back(minY);
      parameters.push_back(rangeY);
      parameters.push_back(temperature);
      parameters.push_back(1000);
      parameters.push_back(0.0);
      parameters.push_back(10.0);
      break;
    }
  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << "           Momentum generator type selected: " << generatorType << endl;
    cout << "   Number of momentum generation parameters: " << parameters.size() << endl;
    for (unsigned int k=0; k<parameters.size(); k++)
      {
      cout << "                            parameter index: " << k << "  value: " <<  parameters[k] << endl;
      }
    }

  unsigned int nSpecies = particleTypes->size();
  if (reportDebug(__FUNCTION__)) cout << " Setting the momentum generators for : " << nSpecies <<  " particle species."  << endl;
  for (unsigned int k=0; k<nSpecies; k++)
    {
    ParticleType * particleType = particleTypes->getParticleType(k);

    MomentumGenerator * gen = new MomentumGenerator(generatorType, particleType, parameters);
    momentumGenerators.push_back(gen);
    }

  if (reportDebug(__FUNCTION__))
    {
    cout << endl;
    cout << " Filling abundance histograms."  << endl;
    cout << "             hadronGases.size:"  << hadronGases.size() << endl;
    }

  vector<double>  & d = hadronGases[0]->particleDensities;
  int nSpeciesConsidered = d.size();
  TString histoName;
  histoName = getName();
  histoName += "_relativeAbundances";
  relativeAbundances = new TH1D(histoName,histoName,nSpeciesConsidered,0.0,double(nSpeciesConsidered));
  histoName = getName();
  histoName += "_relativeAbundancesGas";
  relativeAbundancesGas = new TH1D(histoName,histoName,nSpeciesConsidered,0.0,double(nSpeciesConsidered));

  for (unsigned int k=1; k<=d.size();k++)
  {
  relativeAbundancesGas->SetBinContent(k, d[k-1]);
  relativeAbundancesGas->SetBinError(k, 0.0);
  }
  if (reportEnd(__FUNCTION__))
    ;
}

void HadronGasGeneratorTask::execute()
{
  
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  Event & event = *eventStreams[0];
  Particle * interaction;
  //// resetParticleCounters();

  if (standaloneMode)
    {
    // In this mode, we generate one HG  event per call. One interaction vertex is
    // inserted in the event stream and generate is called to carry out the particle generation.
    particleFactory->reset();
    event.reset();
    interaction = particleFactory->getNextObject();
    interaction->reset();
    interaction->setType( ParticleType::getInteractionType());
    interaction->setXYZT(0.0, 0.0, 0.0, 0.0);
    //interaction->setRThetaPhiT(3.0, TMath::PiOver2(), TMath::TwoPi()*taskRandomGenerator->Rndm(),0.0);
    event.add(interaction);
    event.setNucleusA(1.0,1.0);
    event.setNucleusB(1.0,1.0);
    generate(interaction);
    EventProperties & ep = * eventStreams[0]->getEventProperties();
    ep.zProjectile       = 1;     // atomic number projectile
    ep.aProjectile       = 1;     // mass number projectile
    ep.nPartProjectile   = 1;     // number of participants  projectile
    ep.zTarget           = 1;     // atomic number target
    ep.aTarget           = 1;     // mass number target
    ep.nPartTarget       = 1;     // number of participants  target
    ep.nParticipantsTotal= 2;     // total number of participants
    ep.nBinaryTotal      = 1;     // total number of binary collisions
    ep.impactParameter   = -99999; // nucleus-nucleus center distance in fm
    ep.fractionalXSection= -99999; // fraction cross section value
//    ep.referenceMultiplicity = eventStreams[0]->getNParticles();// nominal multiplicity in the reference range
//    ep.particlesCounted  = getNParticlesCounted();
//    ep.particlesAccepted = getNParticlesAccepted();
    }
  else
    {
    // In this mode, we generate several HG cells per event. Interaction vertices and their locations
    // are assumed to be already loaded in the event stream and we produce as many HG cells as there
    // are nucleon-nucleon interactions in the stream. Objects nucleusA and nucleusB are assumed defined by
    // an earlier task and the structure EventProperties is assumed filled by that earlier task. We only fill the number of
    // particle generated and accepted (in addition to storing the generated particles in the event)

    // May skip this event if it does not satisfy the event cut.
    if (!eventFilters[0]->accept(event)) return;
    if (event.getParticleCount() < 1)
      {
      return;
      }
    vector<Particle*> interactions = event.getNucleonNucleonInteractions();

    unsigned int n = interactions.size();
//    if (reportWarning("PythiaEventGenerator",getName(),"execute()"))
//      cout << "Size of interactions:" <<  n << endl;
    for (unsigned int kInter=0; kInter<n; kInter++)
      {
      generate(interactions[kInter]);
      }
//    EventProperties & ep = * event.getEventProperties();
//    ep.referenceMultiplicity = getNParticlesAccepted(); // nominal multiplicity in the reference range
//    ep.particlesCounted      = getNParticlesCounted();
//    ep.particlesAccepted     = getNParticlesAccepted();
    }
}

void HadronGasGeneratorTask::generate(Particle * parent)
{
  
  if (reportStart(__FUNCTION__))
    ;
  Event & event = * eventStreams[0];
  int multiplicity = minTotalMult + double(rangeTotalMult)*gRandom->Rndm();
  //ParticleFilter & particleFilter = * particleFilters[0];
  Particle * particle;
  TLorentzVector parentPosition = parent->getPosition();
  for (int iPart=0; iPart< multiplicity; iPart++)
    {
    // implement basic no correlation scenario.
    //ParticleType * type = hadronGases[0]->generateRandomHadron();
    int     index  = hadronGases[0]->generateRandomHadronByIndex();
    double  rindex = index + 0.01;
    relativeAbundances->Fill(rindex);
    if (index<0)
      {
      if (reportFatal())
        {
        cout << "hadron index=" << index << endl;
        }
      exit(1);
      }
    ParticleType * type = particleTypes->getParticleType(index);
    if (type==nullptr)
      {
      if (reportError(__FUNCTION__)) cout << "HadronGas::generateRandomHadron() return null pointer. Post task error." << endl;
      postTaskError();
      exit(1);
      return;
      }
    TLorentzVector momentum;
    momentumGenerators[index]->generate(momentum);
    particle = particleFactory->getNextObject();
    particle->setParent(parent);
    particle->set(type, momentum, parentPosition,true);
    //particle->printProperties(cout);
    //incrementParticlesCounted(); // photons are NOT included in this tally
    //if (!particleFilter.accept(*particle)) continue; // no filter for now...
    event.add(particle);
    // // incrementParticlesAccepted();
    }
}
