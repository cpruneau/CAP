/* **********************************************************************
 * Copyright (C) 2019-2022, Claude Pruneau, Victor Gonzalez, Sumit Basu
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2022
 *
 * *********************************************************************/

#include "PythiaEventGeneratorRho.hpp"
#include "ParticleDecayer.hpp"

ClassImp(PythiaEventGeneratorRho);

PythiaEventGeneratorRho::PythiaEventGeneratorRho(const TString & _name,
                                                 Configuration & _configuration,
                                                 vector<EventFilter*>&   _eventFilters,
                                                 vector<ParticleFilter*>&_particleFilters)
:
Task(_name, _configuration, _eventFilters, _particleFilters),
pythia8(nullptr),
nMaxClonesArray(10000),
particles(nullptr)
{
  appendClassName("PythiaEventGeneratorRho");
}


void PythiaEventGeneratorRho::setDefaultConfiguration()
{
  //Task::setDefaultConfiguration();
  setParameter("UseParticles",     true);
  setParameter("UseEventStream0",  true);
  addParameter("RemovePhotons",    true);
  addParameter("SaveHistograms",   false);
  addParameter("LoadHistograms",   false);
  addParameter("ScaleHistograms",  false);
  addParameter("ppOnly",           true);
  addParameter("Beam",             2212);
  addParameter("Target",           2212);
  addParameter("Energy",           2700.0);
  addParameter("nMaxClonesArray",  10000);
  addParameter("DataOutputPath",      "./");
  addParameter("HistogramOutputPath", "./");
  addParameter("HistogramOutputFile", "HistoOutputFile");
  addParameter("SaveHistograms",      false);
  generateKeyValuePairs("Option",     TString("none"),30);
}

//!
//! Initialize generator
//! pythia8->Initialize(2212 /* p */, 2212 /* p */, 14000. /* GeV */);
//!
void PythiaEventGeneratorRho::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  initializeNEventsAccepted();
  pythia8 = new TPythia8();

  int    beam      = getValueInt("Beam");
  int    target    = getValueInt("Target");
  double energy    = getValueDouble("Energy");
  bool   setSeed   = getValueBool("SetSeed");
  long   seedValue = getValueLong("SeedValue");

  cout << endl;
  cout << "===================================================================================" << endl;
  cout << "===================================================================================" << endl;
  cout << "  Pythia:StandaloneMode.........: YES"   << endl;
  cout << "  Pythia:DataOutputUsed.........: NO"    << endl;
  cout << "  Pythia:DataConversionToWac....: YES"   << endl;
  cout << "  Pythia:Beam...................: " << beam << endl;
  cout << "  Pythia:Target.................: " << target << endl;
  cout << "  Pythia:Energy.................: " << energy << endl;
  cout << "  Pythia:SetSeed................: " << setSeed << endl;
  cout << "  Pythia:SeedValue..............: " << seedValue << endl;
  cout << "  Pythia:ReportLevel............: " << getSeverityLevel() << endl;

  if (setSeed)
    {
    TString  seedValueString = "Random:seed = ";
    seedValueString += seedValue;
    pythia8->ReadString("Random:setSeed = on");
    pythia8->ReadString(seedValueString);
    cout << "  Pythia:Random:setSeed = on" <<  endl;
    cout << "  Pythia:" << seedValueString <<  endl;

    }
  for (int k=0; k<30; k++)
    {
    TString key = "Option"; key += k;
    TString  value = getValueString(key);
    if (key.Contains("Option") && !value.Contains("none") )
      {
      cout << "  Pythia::" << key << "......: " << value << endl;
      pythia8->ReadString(value);
      }
    }
  cout << "===================================================================================" << endl;
  cout << "===================================================================================" << endl;

  pythia8->Initialize(beam,target,energy);
  if (reportDebug(__FUNCTION__))
    {
    pythia8->ListAll();
    }

  nMaxClonesArray = getValueInt("nMaxClonesArray");
  particles = new TClonesArray("TParticle", nMaxClonesArray);

  // this for adhoc conversion of particles into rho0 and to decay them
  rhoType     = getParticleTypeCollection()->findPdgCode(  113 );
  piPlusType  = getParticleTypeCollection()->findPdgCode(  211 );
  piMinusType = getParticleTypeCollection()->findPdgCode( -211 );
  rho         = new Particle();
  piPlus      = nullptr;
  piMinus     = nullptr;
  rhoMass     = rhoType->getMass();

  if (reportEnd(__FUNCTION__))
    ;
}

void PythiaEventGeneratorRho::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  Event & event = *getEventStream(0);
  EventProperties & eventProperties = * event.getEventProperties();
  Particle * interaction;
  event.reset();
  particleFactory->reset();
  interaction = particleFactory->getNextObject();
  interaction->reset();
  interaction->setType( ParticleType::getInteractionType());
  interaction->setXYZT(0.0, 0.0, 0.0, 0.0);
  event.add(interaction);
  event.setNucleusA(1.0,1.0);
  event.setNucleusB(1.0,1.0);
  generate(interaction);
  eventProperties.zProjectile        = 1;     // atomic number projectile
  eventProperties.aProjectile        = 1;     // mass number projectile
  eventProperties.nPartProjectile    = 1;     // number of participants  projectile
  eventProperties.zTarget            = 1;     // atomic number target
  eventProperties.aTarget            = 1;     // mass number target
  eventProperties.nPartTarget        = 1;     // number of participants  target
  eventProperties.nParticipantsTotal = 2;     // total number of participants
  eventProperties.nBinaryTotal       = 1;     // total number of binary collisions
  eventProperties.impactParameter    = -99999; // nucleus-nucleus center distance in fm
  eventProperties.fractionalXSection = -99999; // fraction cross section value
  eventProperties.refMultiplicity    = getNParticlesAccepted();
  eventProperties.particlesCounted   = -1;
  eventProperties.particlesAccepted  = getNParticlesAccepted();

  incrementNEventsAccepted(0);
  //if (reportInfo(__FUNCTION__)) eventProperties.printProperties(cout);
  //if (reportInfo(__FUNCTION__)) cout << "Pythia Execute Completed" << endl;
}

void PythiaEventGeneratorRho::generate(Particle * parentInteraction)
{
  int nParticles   = 0;
  double zero  = 0.0;

  bool seekingEvent = true;
  while (seekingEvent)
    {
    pythia8->GenerateEvent();
    if (reportDebug(__FUNCTION__)) pythia8->EventListing();
    pythia8->ImportParticles(particles,"Final");
    nParticles = particles->GetEntriesFast();
    if (nParticles>2) seekingEvent = false;
    }
  if (nParticles>nMaxClonesArray)
    {
    if (reportError(__FUNCTION__)) cout << " ARRAY TOO SMALL np>nMaxClonesArray; nParticles:" << nParticles << " nMax:" << nMaxClonesArray << endl;
    postTaskFatal();
    exit(1);
    }


  TLorentzVector sourcePosition = parentInteraction->getPosition();
  double sourceX = sourcePosition.X();
  double sourceY = sourcePosition.Y();
  double sourceZ = sourcePosition.Z();
  double sourceT = sourcePosition.T();
  ParticleType * type;
  Event & event = * eventStreams[0];
  ParticleFilter & particleFilter = * particleFilters[0];
  // load particles from TClone storage and copy into event.
  //if (reportDebug(__FUNCTION__)) cout << "PythiaEventGeneratorRho::execute() starting copy loop into event.." << endl;

  ParticleDecayer decayer;
  TLorentzVector rhoMomentum;
  TLorentzVector piPlusMomentum;
  TLorentzVector piPlusPosition;
  TLorentzVector piMinusMomentum;
  TLorentzVector piMinusPosition;

  resetNParticlesAcceptedEvent();
  for (int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    TParticle & part = * (TParticle*) particles->At(iParticle);
    int ist = part.GetStatusCode();
    if (ist <= 0) continue;
    int pdg = part.GetPdgCode();
    if (abs(pdg)<=22) continue;

    type = getParticleTypeCollection()->findPdgCode(pdg);
    if (type==nullptr) continue;
    double m = type->getMass();
    if (m<0.100 || m>2.00)
      {
      if (reportInfo(__FUNCTION__))
        cout << " m<0.100 || m>2.00 mass: " << m << " PDG: " << pdg << " Name:" << type->getName() << " Charge:" << type->getCharge() << endl;
      continue;
      }

    rhoMomentum.SetXYZM(part.Px(),part.Py(),part.Pz(),rhoMass);
    decayer.decay2(*rhoType,rhoMomentum,sourcePosition, *piPlusType,piPlusMomentum,piPlusPosition,*piMinusType,piMinusMomentum,piMinusPosition);
    piPlus  = particleFactory->getNextObject();
    piMinus = particleFactory->getNextObject();
    piPlus->set(piPlusType,piPlusMomentum,piPlusPosition, true);
    piMinus->set(piMinusType,piMinusMomentum,piMinusPosition, true);

    if (!particleFilter.accept(*piPlus)) continue;
    incrementNParticlesAccepted();
    event.add(piPlus);
    if (!particleFilter.accept(*piMinus)) continue;
    incrementNParticlesAccepted();
    event.add(piMinus);
    }
}

void PythiaEventGeneratorRho::finalize()
{
  if (reportInfo(__FUNCTION__)) pythia8->PrintStatistics();
  if (reportEnd(__FUNCTION__))
    ;
}
