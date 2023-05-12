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

#include "PythiaEventGenerator.hpp"
using CAP::PythiaEventGenerator;

ClassImp(PythiaEventGenerator);

PythiaEventGenerator::PythiaEventGenerator(const String & _name,
                                           const Configuration & _configuration,
                                           vector<EventFilter*>&   _eventFilters,
                                           vector<ParticleFilter*>&_particleFilters)
:
EventTask(_name, _configuration, _eventFilters, _particleFilters),
pythia(nullptr),
outputFile(nullptr),
//outputEvent(nullptr),
//outputTree(nullptr),
//nMaxClonesArray(10000),
standaloneMode(true),
printBanner(true),
printStatistics(true),
printNEvents(0),
beamsPdgA(2212),
beamsPdgB(2212),
beamsFrameType(2),
beamsECMS(2700.0),
beamsEA(1350.0),
beamsEB(1350.0),
setSeed(1),
seedValue(1112131),
useQCDCR(1),
useRopes(0),
useShoving(0),
xmlInputPath("nil"),
nEventsPrinted(0)
{
  appendClassName("PythiaEventGenerator");
}


void PythiaEventGenerator::setDefaultConfiguration()
{
  EventTask::setDefaultConfiguration();
  addParameter("StandaloneMode",   standaloneMode);
  addParameter("Print:Banner",     printBanner);
  addParameter("Print:Statistics", printStatistics);
  addParameter("Print:NEvents",    printNEvents);

  addParameter("Beams:idA",        beamsPdgA);
  addParameter("Beams:idB",        beamsPdgB);
  addParameter("Beams:frameType",  beamsFrameType);
  addParameter("Beams:eCM",        beamsECMS);
  addParameter("Beams:eA",         beamsEA);
  addParameter("Beams:eB",         beamsEB);

  addParameter("SetSeed",          setSeed);
  addParameter("SeedValue",        seedValue);
  addParameter("useQCDCR",         useQCDCR);
  addParameter("useRopes",         useRopes);
  addParameter("useShoving",       useShoving);
  addParameter("XmlInputPath",     xmlInputPath);

  generateKeyValuePairs("Option",  TString("none"),30);
}

void PythiaEventGenerator::configure()
{
  EventTask::configure();
  standaloneMode  = getValueBool(  "StandaloneMode");
  printBanner     = getValueBool(  "Print:Banner");
  printStatistics = getValueBool(  "Print:Statistics");
  printNEvents    = getValueInt(   "Print:NEvents");

  beamsPdgA       = getValueInt(   "Beams:idA");
  beamsPdgB       = getValueInt(   "Beams:idB");
  beamsFrameType  = getValueInt(   "Beams:frameType");
  beamsECMS       = getValueDouble("Beams:eCM");
  beamsEA         = getValueDouble("Beams:eA");
  beamsEB         = getValueDouble("Beams:eB");
  setSeed         = getValueBool(  "SetSeed");
  seedValue       = getValueLong(  "SeedValue");
  useQCDCR        = getValueBool(  "UseQCDCR");
  useRopes        = getValueBool(  "UseRopes");
  useShoving      = getValueBool(  "UseShoving");
  xmlInputPath    = getValueString("XmlInputPath");

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("StandaloneMode",   standaloneMode);
    printItem("Print:Banner",     printBanner);
    printItem("Print:Statistics", printStatistics);
    printItem("Print:NEvents",    printNEvents);
    printItem("Beams:idA",        beamsPdgA);
    printItem("Beams:idB",        beamsPdgB);
    printItem("Beams:frameType",  beamsFrameType);
    printItem("Beams:eCM",        beamsECMS);
    printItem("Beams:eA",         beamsEA);
    printItem("Beams:eB",         beamsEB);
    printItem("SetSeed",          setSeed);
    printItem("SeedValue",        seedValue);
    printItem("useQCDCR",         useQCDCR);
    printItem("useRopes",         useRopes);
    printItem("useShoving",       useShoving);
    printItem("XmlInputPath",     xmlInputPath);
    cout << endl;
    }
}



//!
//! Initialize generator
//! pythia->Initialize(2212 /* p */, 2212 /* p */, 14000. /* GeV */);
//!
void PythiaEventGenerator::initialize()
{
;
  EventTask::initialize();

  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    printItem("StandaloneMode", standaloneMode);
    printItem("Print:Banner",   printBanner);
    printItem("Beams:idA",      beamsPdgA);
    printItem("Beams:idB",      beamsPdgB);
    printItem("Beams:frameType",beamsFrameType);
    printItem("Beams:eCM",      beamsECMS);
    printItem("Beams:eA",       beamsEA);
    printItem("Beams:eB",       beamsEB);
    printItem("SetSeed",        setSeed);
    printItem("SeedValue",      seedValue);
    printItem("useQCDCR",       useQCDCR);
    printItem("useRopes",       useRopes);
    printItem("useShoving",     useShoving);
    printItem("XmlInputPath",   xmlInputPath);
    }

  pythia = new Pythia8::Pythia(xmlInputPath.Data(), printBanner);
  pythia->settings.mode("Beams:idA",  beamsPdgA);
  pythia->settings.mode("Beams:idB",  beamsPdgB);
  pythia->settings.mode("Beams:frameType", beamsFrameType);
  switch (beamsFrameType)
    {
      default:
      case 1:
      pythia->settings.parm("Beams:eCM", beamsECMS);
      break;
      case 2:
      pythia->settings.parm("Beams:eA",      beamsEA);
      pythia->settings.parm("Beams:eB",      beamsEB);
      break;
    }


  if (setSeed)
    {
    String  seedValueString = "Random:seed = ";
    seedValueString += seedValue;
    pythia->readString("Random:setSeed = on");
    pythia->readString(seedValueString.Data());
    cout << "  Pythia:Random:setSeed = on" <<  endl;
    cout << "  Pythia:" << seedValueString <<  endl;

    }
  for (int k=0; k<30; k++)
    {
    String key = "Option"; key += k;
    String  value = getValueString(key);
    if (key.Contains("Option") && !value.Contains("none") )
      {
      cout << "  Pythia::" << key << "......: " << value << endl;
      pythia->readString(value.Data());
      }
    }

  if(useQCDCR)
    {
    pythia->readString("MultiPartonInteractions:pT0Ref = 2.15");
    pythia->readString("BeamRemnants:remnantMode = 1");
    pythia->readString("BeamRemnants:saturation = 5");
    pythia->readString("ColourReconnection:mode = 1");
    pythia->readString("ColourReconnection:allowDoubleJunRem = off");
    pythia->readString("ColourReconnection:m0 = 0.3");
    pythia->readString("ColourReconnection:allowJunctions = on");
    pythia->readString("ColourReconnection:junctionCorrection = 1.2");
    pythia->readString("ColourReconnection:timeDilationMode = 2");
    pythia->readString("ColourReconnection:timeDilationPar = 0.18");
    if(!useRopes)
      pythia->readString("Ropewalk:RopeHadronization = off");
    }
  if(useQCDCR && useRopes)
    {
    pythia->readString("Ropewalk:RopeHadronization = on");
    pythia->readString("Ropewalk:doShoving = on");
    pythia->readString("Ropewalk:doFlavour = on");
    pythia->readString("Ropewalk:tInit = 1.5");
    pythia->readString("Ropewalk:deltat = 0.05");
    pythia->readString("Ropewalk:tShove = 0.1");
    pythia->readString("Ropewalk:gAmplitude = 0.");// # Set shoving strength to 0 explicitly
    pythia->readString("Ropewalk:r0 = 0.5");
    pythia->readString("Ropewalk:m0 = 0.2");
    pythia->readString("Ropewalk:beta = 0.1");
    pythia->readString("PartonVertex:setVertex = on");
    pythia->readString("PartonVertex:protonRadius = 0.7");
    pythia->readString("PartonVertex:emissionWidth = 0.1");
  }
  if(!useQCDCR && useRopes)
    {
    cout<<"You are trying to turn on ropes without the necessary junctions! Flip kQCDCR=kTRUE"<<endl;
    exit(1);
    }

  if(useShoving)
    {
    pythia->readString("Ropewalk:RopeHadronization = on");
    pythia->readString("Ropewalk:doShoving = on");
    pythia->readString("Ropewalk:doFlavour = off");
    pythia->readString("Ropewalk:tInit = 1.5");
    pythia->readString("Ropewalk:rCutOff = 10.0");
    pythia->readString("Ropewalk:limitMom =  on");
    pythia->readString("Ropewalk:pTcut = 2.0");
    pythia->readString("Ropewalk:deltat = 0.1");
    pythia->readString("Ropewalk:deltay = 0.1");
    pythia->readString("Ropewalk:tShove = 1.");
    pythia->readString("Ropewalk:deltat = 0.1");
    pythia->readString("Ropewalk:gAmplitude = 10.0");
    pythia->readString("Ropewalk:gExponent = 1.0");
    pythia->readString("Ropewalk:r0 = 0.41");
    pythia->readString("Ropewalk:m0 = 0.2");
    pythia->readString("PartonVertex:setVertex = on");
    pythia->readString("PartonVertex:protonRadius = 0.7");
    pythia->readString("PartonVertex:emissionWidth = 0.1");
    }
  pythia->init();
  if (reportDebug(__FUNCTION__))
    {
    pythia->settings.listAll();
    pythia->settings.listChanged();
    //pythia->particleData.list(id);
    }
//  if (eventsExport )
//    {
//    String outputFileName = getValueString("EventsExportPath");
//    outputFileName += "/";
//    outputFileName += getValueString("EventsExportFileName");
//    outputFile = TFile::Open(outputFileName,"recreate");
//    //outputEvent = &pythia->pythia()->event;
//    outputTree  = new TTree(getValueString("EventsExportTreeName"),"PythiaEventTree");
//    particles = (TClonesArray*) pythia->GetListOfParticles();
//    outputTree->Branch("particles", &particles);
//    outputTree->Branch("event",&outputEvent);
//    }
  if (reportEnd(__FUNCTION__))
;
}

void PythiaEventGenerator::createEvent()
{
  if (reportStart(__FUNCTION__))
;
  incrementTaskExecuted();
  Event & event = *eventStreams[0];
  EventProperties & eventProperties = * event.getEventProperties();
  Particle * interaction;
  standaloneMode = true;
  event.reset();
  particleFactory->reset();
  interaction = particleFactory->getNextObject();
  interaction->reset();
  interaction->setType( ParticleType::getInteractionType());
  interaction->setXYZT(0.0, 0.0, 0.0, 0.0);
  event.add(interaction);
  event.setNucleusA(1.0,1.0);
  event.setNucleusB(1.0,1.0);
  pythia->next();
//  int ioff = 0;
  int nParticleToCopy   = pythia->event.size();
  if (pythia->event[0].id() == 90)
    {
    nParticleToCopy--;
//     ioff = -1;
    }

//  if (reportDebug(__FUNCTION__)) cout << "nParticleToCopy : " << nParticleToCopy << endl;

  for (int i = 1; i <= nParticleToCopy; i++)
    {

//    int ist = tracks_fStatusCode[iParticle];
//    if (ist <= 0) continue;
    int pdg = pythia->event[i].id();
    if (abs(pdg)<40  || pdg==2101) continue; // skip quarks,  leptons, and photons
    if (!pythia->event[i].isFinal()) continue;
    //if (reportDebug(__FUNCTION__))   cout << "     pdg:" << pdg << endl;
    ParticleType * particleType = ParticleDb::getDefaultParticleDb()->findPdgCode(pdg);
    if (particleType==nullptr)
      {
      if (reportDebug(__FUNCTION__)) cout << "     is an unknown code" << endl;
      continue;
      }
    //if (particleType->isPhoton()) continue;

    Particle & particle = *particleFactory->getNextObject();
    particle.setType(particleType);
    particle.setLive(1);
//    particle.setParents(pythia->event[i].mother1()   + ioff,
//                        pythia->event[i].mother2()   + ioff);
//
////    setParents(Particle * parent1, Particle * parent2);
//
//    particle.setChildren(pythia->event[i].daughter1() + ioff,
//                         pythia->event[i].daughter2() + ioff)
    // momentum-energy units are  [GeV/c]
    // positions are in [mm], time in [mm/c]
    particle.setPxPyPzE(pythia->event[i].px(),pythia->event[i].py(),pythia->event[i].pz(),pythia->event[i].e());
    particle.setXYZT(pythia->event[i].xProd(),pythia->event[i].yProd(),pythia->event[i].zProd(),pythia->event[i].tProd());
    //incrementParticlesAccepted();
    //if (reportDebug(__FUNCTION__)) cout << "   Add Particle to the event" << endl;
    event.add(&particle);
    }
  int multiplicity = event.getParticleCount();
//  if (reportDebug(__FUNCTION__)) cout << "multiplicity " << multiplicity << endl;

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
  eventProperties.refMultiplicity    = multiplicity;
  eventProperties.particlesCounted   = -1;
  eventProperties.particlesAccepted  = multiplicity;
  incrementNEventsAccepted(0);
  //if (reportInfo(__FUNCTION__)) eventProperties.printProperties(cout);
  //if (reportInfo(__FUNCTION__)) cout << "Pythia Execute Completed" << endl;
}

void PythiaEventGenerator::finalize()
{
  if (reportInfo(__FUNCTION__) && printStatistics)
    {
    cout << endl;
    pythia->stat();
    cout << endl;
    }
  delete pythia;
//  if (eventsExport)
//    {
//    outputTree->Print();
//    outputTree->Write();
//    delete outputFile;
//    }
  if (reportEnd(__FUNCTION__))
;
}
