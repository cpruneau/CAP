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
                                           Configuration & _configuration,
                                           vector<EventFilter*>&   _eventFilters,
                                           vector<ParticleFilter*>&_particleFilters)
:
Task(_name, _configuration, _eventFilters, _particleFilters),
pythia8(nullptr),
nMaxClonesArray(10000),
particles(nullptr),
outputFile(nullptr),
outputEvent(nullptr),
outputTree(nullptr),
standaloneMode(true)
{
  appendClassName("PythiaEventGenerator");
}


void PythiaEventGenerator::setDefaultConfiguration()
{
  //Task::setDefaultConfiguration();
  setParameter("UseParticles",     true);
  setParameter("UseEventStream0",  true);
  addParameter("RemovePhotons",    true);
  addParameter("StandaloneMode",   true);
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
  addParameter("DataConversionToWac", true);
  addParameter("DataInputUsed",       false);
  addParameter("SaveHistograms",      false);
  addParameter("useQCDCR",            false);
  addParameter("useRopes",            false);
  addParameter("useShoving",          false);
  generateKeyValuePairs("Option",     TString("none"),30);
}

//!
//! Initialize generator
//! pythia8->Initialize(2212 /* p */, 2212 /* p */, 14000. /* GeV */);
//!
void PythiaEventGenerator::initialize()
{
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  initializeNEventsAccepted();
  pythia8 = new TPythia8();

  standaloneMode      = getValueBool("StandaloneMode");
  dataOutputUsed      = getValueBool("DataOutputUsed");
  DataConversionToWac = getValueBool("DataConversionToWac");
  int    beam         = getValueInt("Beam");
  int    target       = getValueInt("Target");
  double energy       = getValueDouble("Energy");
  bool   setSeed      = getValueBool("SetSeed");
  long   seedValue    = getValueLong("SeedValue");
  bool   useQCDCR     = getValueBool("useQCDCR");
  bool   useRopes     = getValueBool("useRopes");
  bool   useShoving   = getValueBool("useShoving");

  cout << endl;
  cout << "===================================================================================" << endl;
  cout << "===================================================================================" << endl;
  cout << "  Pythia:StandaloneMode.........: " << standaloneMode      << endl;
  cout << "  Pythia:DataOutputUsed.........: " << dataOutputUsed      << endl;
  cout << "  Pythia:DataConversionToWac....: " << DataConversionToWac << endl;
  cout << "  Pythia:Beam...................: " << beam << endl;
  cout << "  Pythia:Target.................: " << target << endl;
  cout << "  Pythia:Energy.................: " << energy << endl;
  cout << "  Pythia:SetSeed................: " << setSeed << endl;
  cout << "  Pythia:SeedValue..............: " << seedValue << endl;
  cout << "  Pythia:useQCDCR...............: " << useQCDCR << endl;
  cout << "  Pythia:useRopes...............: " << useRopes << endl;
  cout << "  Pythia:useShoving.............: " << useShoving << endl;
  cout << "  Pythia:ReportLevel............: " << getSeverityLevel() << endl;

  if (setSeed)
    {
    String  seedValueString = "Random:seed = ";
    seedValueString += seedValue;
    pythia8->ReadString("Random:setSeed = on");
    pythia8->ReadString(seedValueString);
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
      pythia8->ReadString(value);
      }
    }

  if(useQCDCR)
    {

    pythia8->ReadString("MultiPartonInteractions:pT0Ref = 2.15");
    pythia8->ReadString("BeamRemnants:remnantMode = 1");
    pythia8->ReadString("BeamRemnants:saturation = 5");
    pythia8->ReadString("ColourReconnection:mode = 1");
    pythia8->ReadString("ColourReconnection:allowDoubleJunRem = off");
    pythia8->ReadString("ColourReconnection:m0 = 0.3");
    pythia8->ReadString("ColourReconnection:allowJunctions = on");
    pythia8->ReadString("ColourReconnection:junctionCorrection = 1.2");
    pythia8->ReadString("ColourReconnection:timeDilationMode = 2");
    pythia8->ReadString("ColourReconnection:timeDilationPar = 0.18");
    if(!useRopes)
      pythia8->ReadString("Ropewalk:RopeHadronization = off");
  }
  if(useQCDCR && useRopes){
    pythia8->ReadString("Ropewalk:RopeHadronization = on");
    pythia8->ReadString("Ropewalk:doShoving = on");
    pythia8->ReadString("Ropewalk:doFlavour = on");

    pythia8->ReadString("Ropewalk:tInit = 1.5");
    pythia8->ReadString("Ropewalk:deltat = 0.05");
    pythia8->ReadString("Ropewalk:tShove = 0.1");
    pythia8->ReadString("Ropewalk:gAmplitude = 0.");// # Set shoving strength to 0 explicitly
    pythia8->ReadString("Ropewalk:r0 = 0.5");
    pythia8->ReadString("Ropewalk:m0 = 0.2");
    pythia8->ReadString("Ropewalk:beta = 0.1");
    pythia8->ReadString("PartonVertex:setVertex = on");
    pythia8->ReadString("PartonVertex:protonRadius = 0.7");
    pythia8->ReadString("PartonVertex:emissionWidth = 0.1");
  }
  if(!useQCDCR && useRopes)
    {
    cout<<"You are trying to turn on ropes without the necessary junctions! Flip kQCDCR=kTRUE"<<endl;
    exit(1);
    }

  if(useShoving)
    {
    pythia8->ReadString("Ropewalk:RopeHadronization = on");
    pythia8->ReadString("Ropewalk:doShoving = on");
    pythia8->ReadString("Ropewalk:doFlavour = off");

    pythia8->ReadString("Ropewalk:tInit = 1.5");
    pythia8->ReadString("Ropewalk:rCutOff = 10.0");
    pythia8->ReadString("Ropewalk:limitMom =  on");
    pythia8->ReadString("Ropewalk:pTcut = 2.0");
    pythia8->ReadString("Ropewalk:deltat = 0.1");
    pythia8->ReadString("Ropewalk:deltay = 0.1");
    pythia8->ReadString("Ropewalk:tShove = 1.");
    pythia8->ReadString("Ropewalk:deltat = 0.1");
    pythia8->ReadString("Ropewalk:gAmplitude = 10.0");
    pythia8->ReadString("Ropewalk:gExponent = 1.0");
    pythia8->ReadString("Ropewalk:r0 = 0.41");
    pythia8->ReadString("Ropewalk:m0 = 0.2");
    pythia8->ReadString("PartonVertex:setVertex = on");
    pythia8->ReadString("PartonVertex:protonRadius = 0.7");
    pythia8->ReadString("PartonVertex:emissionWidth = 0.1");
  }


  cout << "===================================================================================" << endl;
  cout << "===================================================================================" << endl;



  pythia8->Initialize(beam,target,energy);
  if (reportDebug(__FUNCTION__))
    {
    pythia8->ListAll();
    }
  if (dataOutputUsed )
    {
    String outputFileName = getValueString("DataOutputPath");
    outputFileName += "/";
    outputFileName += getValueString("dataOutputFileName");
    outputFile = TFile::Open(outputFileName,"recreate");
    //outputEvent = &pythia8->Pythia8()->event;
    outputTree  = new TTree(getValueString("dataOutputTreeName"),"PythiaEventTree");
    particles = (TClonesArray*) pythia8->GetListOfParticles();
    outputTree->Branch("particles", &particles);
    //outputTree->Branch("event",&outputEvent);
    }
  if (DataConversionToWac)
    {
    nMaxClonesArray = getValueInt("nMaxClonesArray");
    particles = new TClonesArray("TParticle", nMaxClonesArray);
    }
  if (reportEnd(__FUNCTION__))
    ;
}

void PythiaEventGenerator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  Event & event = *getEventStream(0);
  EventProperties & eventProperties = * event.getEventProperties();
  Particle * interaction;
  standaloneMode = true;
  if (standaloneMode)
    {
    // In this mode, we generate one PYTHIA (pp) collision per event. One interaction vertex is
    // inserted in the event stream and PYTHIA is called to carry out the particle generation.
    // factory and event resets done by the task iterator to avoid repetition.
    event.reset();
    particleFactory->reset();
    // resetParticleCounters();
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
    }
//  else
//    {
//    // In this mode, we generate several PYTHIA collisions per event. Interaction vertices and their locations
//    // are assumed to be already loaded in the event stream and we produce as many PYTHIA events as there
//    // are nucleon-nucleon interactions in the stream. Objects nucleusA and nucleusB are assumed defined by
//    // an earlier task and the structure EventProperties is assumed filled by that earlier task.He we only fill the number of
//    // particle generated and accepted (in addition to storing the generated particles in the event)
//
//    // May skip this event if it does not satisfy the event cut.
//    if (!eventFilters[0]->accept(event)) return;
//    if (event.getParticleCount() < 1)
//      {
//      return;
//      }
//    vector<Particle*> interactions = event.getNucleonNucleonInteractions();
//
//    unsigned int n = interactions.size();
////    if (reportWarning("PythiaEventGenerator",getName(),"execute()"))
////      cout << "Size of interactions:" <<  n << endl;
//    for (unsigned int kInter=0; kInter<n; kInter++)
//      {
//      generate(interactions[kInter]);
//      }
////    eventProperties.refMultiplicity = getNParticlesAccepted(); // nominal multiplicity in the reference range
////    eventProperties.particlesCounted      = getNParticlesCounted();
////    eventProperties.particlesAccepted     = getNParticlesAccepted();
//    }
  incrementNEventsAccepted(0);
  //if (reportInfo(__FUNCTION__)) eventProperties.printProperties(cout);
  //if (reportInfo(__FUNCTION__)) cout << "Pythia Execute Completed" << endl;
}

void PythiaEventGenerator::generate(Particle * parentInteraction)
{
  Particle * particle;
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
  if (dataOutputUsed)
    {
    outputTree->Fill();
    }
  if (DataConversionToWac)
    {
    LorentzVector sourcePosition = parentInteraction->getPosition();
    double sourceX = sourcePosition.X();
    double sourceY = sourcePosition.Y();
    double sourceZ = sourcePosition.Z();
    double sourceT = sourcePosition.T();
    ParticleType * type;
    Event & event = * eventStreams[0];
    ParticleFilter & particleFilter = * particleFilters[0];
    // load particles from TClone storage and copy into event.
    //if (reportDebug(__FUNCTION__)) cout << "PythiaEventGenerator::execute() starting copy loop into event.." << endl;

    resetNParticlesAcceptedEvent();
    for (int iParticle = 0; iParticle < nParticles; iParticle++)
      {
      TParticle & part = * (TParticle*) particles->At(iParticle);
      int ist = part.GetStatusCode();
      if (ist <= 0) continue;
      int pdg = part.GetPdgCode();
      if (abs(pdg)<=22) continue;
      //if (pdg==311 || pdg==-311 || pdg==3122|| pdg==-3122)
//      if (pdg==3122|| pdg==-3122)
//        {
//        cout << "pdg:" << pdg << endl;
//        cout << "L are not decayed by PYTHIA" << endl;
//        cout << "L are not decayed by PYTHIA" << endl;
//        type = particleTypeCollection->findPdgCode(pdg);
//        cout << "Name is: " << type->getName() << endl;
//        if (type==nullptr)
//          {
//          cout << "But the type is not found in the list" << endl;
//          exit(0);
//          }
//        }
//      if (pdg==2212 || pdg==-2212 )
//        {
//        if (part.GetMother(1)==3122 || part.GetMother(1)==-3122)
//          {
//          cout << "K SHORT are decaed by PYTHIA" << endl;
//          cout << "K SHORT are decaed by PYTHIA" << endl;
//          exit(0);
//          }
//        }

      type = getParticleTypeCollection()->findPdgCode(pdg);
      if (type==nullptr) continue;
      double m = type->getMass();
      if (m<0.100 || m>2.00)
        {
        if (reportInfo(__FUNCTION__))
          cout << " m<0.100 || m>2.00 mass: " << m << " PDG: " << pdg << " Name:" << type->getName() << " Charge:" << type->getCharge() << endl;
        continue;
        }
      particle = particleFactory->getNextObject();
      particle->set(type,part.Px(),part.Py(),part.Pz(),part.Energy(),sourceX,sourceY,sourceZ,sourceT,true);
      //incrementParticlesCounted(0); // photons are NOT included in this tally
      if (!particleFilter.accept(*particle)) continue;
      incrementNParticlesAccepted();
      event.add(particle);
      // // incrementParticlesAccepted();
      }
//    if (reportDebug("PythiaEventGenerator",getName(),"execute()"))
//      {
//      cout << endl;
//      cout << "PythiaEventGenerator::execute() No of accepted particles : "<< getNParticlesAccepted() << endl;
//      cout << "PythiaEventGenerator::execute() No of counted particles : " << getNParticlesCounted()  << endl;
//      }
    }
}

void PythiaEventGenerator::finalize()
{
  if (reportInfo(__FUNCTION__)) pythia8->PrintStatistics();
  if (dataOutputUsed)
    {
    outputTree->Print();
    outputTree->Write();
    delete outputFile;
    }
  if (reportEnd(__FUNCTION__))
    ;
}




//pythia8->ReadString("Init:showChangedSettings = on");      // list changed settings
//pythia8->ReadString("Init:showChangedParticleData = off"); // list changed particle data
//pythia8->ReadString("Next:numberCount = 100000");            // print message every n events
//pythia8->ReadString("Next:numberShowInfo = 1");            // print event information n times
//pythia8->ReadString("Next:numberShowProcess = 0");         // print process record n times
//pythia8->ReadString("Next:numberShowEvent = 0");
//
//pythia8->ReadString("SoftQCD:all = off");                   // Allow total sigma = elastic/SD/DD/ND
//                                                           // Optionally only study one or a few processes at a time.
//                                                           //  pythia8->ReadString("SoftQCD:elastic = on");               // Elastic
//                                                           //  pythia8->ReadString("SoftQCD:singleDiffractive = on");     // Single diffractive
//                                                           //  pythia8->ReadString("SoftQCD:doubleDiffractive = on");     // Double diffractive
//                                                           //  pythia8->ReadString("SoftQCD:centralDiffractive = on");    // Central diffractive
//                                                           //  pythia8->ReadString("SoftQCD:nonDiffractive = on");        // Nondiffractive (inelastic)
//                                                           //  pythia8->ReadString("SoftQCD:inelastic = on");             // All inelastic
//                                                           // Optionally switch on hand-set cross section and Coulomb term.
//                                                           // Note: these values are illustrative only, not to be taken seriously.
//                                                           //  pythia8->ReadString("SigmaTotal:setOwn = on");              // set your own total/elastic/diffr. sigma
//                                                           //  pythia8->ReadString("SigmaTotal:sigmaTot = 106.0");         // total cross section (excluding Coulomb)
//                                                           //  pythia8->ReadString("SigmaTotal:sigmaXX = 5.0");        // reduce double diffractive cross section
//                                                           //  pythia8->ReadString("SigmaElastic:setOwn = on");        // suboption for Coulomb term in elastic
//                                                           //  pythia8->ReadString("SigmaElastic:tAbsMin = 4e-5");        // divergence requires lower cutoff
//
//pythia8->ReadString("HardQCD:all = on");
//
//
////  Optionally select diffractive model.
////  pythia8->ReadString("SigmaDiffractive:mode = 2");           // the MBR model for diffraction
//
//// Multiparton interactions and impact parameter picture.
//// Note: these values are illustrative only, not to be taken seriously.
////  pythia8->ReadString("MultipartonInteractions:pT0Ref = 2.");
////  pythia8->ReadString("MultipartonInteractions:ecmRef = 1960.");
////  pythia8->ReadString("MultipartonInteractions:ecmPow = 0.16");
////  pythia8->ReadString("MultipartonInteractions:pTmin = 0.2");
////  pythia8->ReadString("MultipartonInteractions:bProfile = 2");
////  pythia8->ReadString("MultipartonInteractions:coreRadius = 0.4");
////  pythia8->ReadString("MultipartonInteractions:coreFraction = 0.5");
////  pythia8->ReadString("ultipartonInteractions:expPow = 1.");
//

//for (int iParticle = 0; iParticle < nParticles; iParticle++)
//  {
//  TParticle & part = * (TParticle*) particles->At(iParticle);
//  int ist = part.GetStatusCode();
//  //if (reportDebug(__FUNCTION__)) cout << "PythiaEventGenerator::execute() ist: " << ist << endl;
//  if (ist <= 0) continue;
//  int pdg = part.GetPdgCode();
//  mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
//  if (mass<0.002) continue;  // no photons, electrons..
//  charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
//  p_x  = cosPhi*part.Px() - sinPhi*part.Py();
//  p_y  = sinPhi*part.Px() + cosPhi*part.Py();
//  p_z  = part.Pz();
//  p_e  = part.Energy();
//  aParticle.setPidPxPyPzE(pdg,p_x,p_y,p_z,p_e);
//  //aParticle.printProperties(cout);
//  //if (reportDebug(__FUNCTION__)) cout << "PythiaEventGenerator::execute() calling filter " << endl;
//  particleCounted++;
//  if (!particleFilter->accept(aParticle)) continue;
//  particle = particleFactory->getNextObject();
//  *particle = aParticle;
//  event->add(particle);
//  particleAccepted++;
//  //    if (true)
//  //      {
//  //      cout << "PythiaEventGenerator::execute() particle: " << iParticle << " / " << particleAccepted << endl;
//  //      particle->printProperties(cout);
//  //      }
//  }
