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
#include "ResonanceGenerator.hpp"
#include "ParticleDecayer.hpp"
using CAP::ResonanceGenerator;

ClassImp(ResonanceGenerator);

//!
//! This is a simplistic particle generator simulating events with an exponential distribution in pT and pair correlations
//! based on rho-mesons decays.
//!
ResonanceGenerator::ResonanceGenerator(const String  &        _name,
                                       Configuration  &        _configuration,
                                       vector<EventFilter*>&   _eventFilters,
                                       vector<ParticleFilter*>&_particleFilters)
:
Task(_name,_configuration,_eventFilters,_particleFilters),
standaloneMode(0),
nPartMinimum(1),
nPartMaximum(1),
nPartRange(0),
mass(0.4),
yMinimum(-1.0),
yMaximum( 1.0),
yRange(2.0),
pTslope(0.4)
{
  appendClassName("ResonanceGenerator");

}


void ResonanceGenerator::setDefaultConfiguration()
{
  setParameter("UseParticles",      true);
  setParameter("UseEventStream0",   true);
  addParameter("StandaloneMode",    true);
  addParameter("SaveHistograms",    false);
  addParameter("LoadHistograms",    false);
  addParameter("ScaleHistograms",   false);
  addParameter("nParticlesMinimum", 10);
  addParameter("nParticlesMaximum", 40);
  addParameter("yMinimum",   -1.0);
  addParameter("yMaximum",    1.0);
  addParameter("pTslope",     0.5);
  addParameter("mass",        0.4);
}

void ResonanceGenerator::initialize()
{
  Task::initialize();
  standaloneMode = getValueBool("StandaloneMode");
  nPartMinimum   = getValueInt("nParticlesMinimum");
  nPartMaximum   = getValueInt("nParticlesMaximum");
  nPartRange     = nPartMaximum-nPartMinimum;
  yMinimum       = getValueDouble("yMinimum");
  yMaximum       = getValueDouble("yMaximum");
  yRange         = yMaximum - yMinimum;
  pTslope        = getValueDouble("pTslope");
  mass           = getValueDouble("mass");
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << " Reso:standaloneMode.........: " << standaloneMode << endl;
    cout << " Reso:nPartMinimum...........: " << nPartMinimum << endl;
    cout << " Reso:nPartMaximum...........: " << nPartMaximum << endl;
    cout << " Reso:nPartRange.............: " << nPartRange << endl;
    cout << " Reso:yMinimum...............: " << yMinimum << endl;
    cout << " Reso:yMaximum...............: " << yMaximum << endl;
    cout << " Reso:yRange.................: " << yRange << endl;
    cout << " Reso:pTslope................: " << pTslope << endl;
    cout << " Reso:mass...................: " << mass << endl;
    }
}

void ResonanceGenerator::execute()
{
  if (reportStart(__FUNCTION__))
    ;
  incrementTaskExecuted();
  Event & event = *eventStreams[0];
  EventProperties & ep = * eventStreams[0]->getEventProperties();

  Particle * interaction;
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
    ep.zProjectile       = 1;     // atomic number projectile
    ep.aProjectile       = 1;     // mass number projectile
    ep.nPartProjectile   = 1;     // number of participants  projectile
    ep.zTarget           = 1;     // atomic number target
    ep.aTarget           = 1;     // mass number target
    ep.nPartTarget       = 1;     // number of participants  target
    ep.nParticipantsTotal        = 2;     // total number of participants
    ep.nBinaryTotal      = 1;     // total number of binary collisions
    ep.impactParameter   = -99999; // nucleus-nucleus center distance in fm
    ep.fractionalXSection = -99999; // fraction cross section value
//    ep.refMultiplicity = getNParticlesAccepted(); // nominal multiplicity in the reference range
//    ep.particlesCounted   = getNParticlesCounted();
//    ep.particlesAccepted  = getNParticlesAccepted();
    }
  else
    {
    // In this mode, we generate several PYTHIA collisions per event. Interaction vertices and their locations
    // are assumed to be already loaded in the event stream and we produce as many PYTHIA events as there
    // are nucleon-nucleon interactions in the stream. Objects nucleusA and nucleusB are assumed defined by
    // an earlier task and the structure EventProperties is assumed filled by that earlier task.He we only fill the number of
    // particle generated and accepted (in addition to storing the generated particles in the event)

    // May skip this event if it does not satisfy the event cut.
    if (!eventFilters[0]->accept(event)) return;
    if (event.getParticleCount() < 1)
      {
      return;
      }
    vector<Particle*> interactions = event.getNucleonNucleonInteractions();

    unsigned int n = interactions.size();
    //    if (reportWarning("ResonanceGenerator",getName(),"execute()"))
    //      cout << "Size of interactions:" <<  n << endl;
    for (unsigned int kInter=0; kInter<n; kInter++)
      {
      generate(interactions[kInter]);
      }
//    ep.refMultiplicity = getNParticlesAccepted(); // nominal multiplicity in the reference range
//    ep.particlesCounted  = getNParticlesCounted();
//    ep.particlesAccepted = getNParticlesAccepted();
    }
  if (reportDebug(__FUNCTION__))
    {
    ep.printProperties(cout);
    }
}

void ResonanceGenerator::generate(Particle * parentInteraction)
{
  Event & event = * eventStreams[0];
  //ParticleFilter & particleFilter = * particleFilters[0];

  // generate neutral rho-meson and decay them..
  ParticleType * parentType = particleTypeCollection->findPdgCode(113);  // rho-meson only
  ParticleDecayMode & decayMode = parentType->generateDecayMode(); // pi+ and pi- only
  
  // for now, assume a two body decay exclisively..
  //int nChildren = decayMode.getNChildren();
  LorentzVector parentPosition = parentInteraction->getPosition();
  ParticleDecayer decayer;
  Particle * parent = particleFactory->getNextObject();
  double y, phi, pt, mt, px, py, pz, e;
  int mult = int( nPartMinimum +  double(nPartRange) * gRandom->Rndm());
  for (int iParticle = 0; iParticle < mult; iParticle++)
    {
    y   = yMinimum + yRange * gRandom->Rndm();
    phi = TMath::TwoPi() * gRandom->Rndm();
    pt  = 1.0+gRandom->Exp(pTslope);
    px  = pt*cos(phi);
    py  = pt*sin(phi);
    mt  = sqrt(mass*mass+pt*pt);
    pz  = mt * sinh(y);
    e   = mt * cosh(y);
    LorentzVector parentMomentum;
    parentMomentum.SetPxPyPzE (px,py,pz,e);
    parent->set(parentType,parentMomentum,parentPosition,false);

    Particle * child1 = particleFactory->getNextObject();
    Particle * child2 = particleFactory->getNextObject();
    ParticleType   & childType1 = decayMode.getChildType(0); child1->setType(&childType1); child1->setLive(true);
    ParticleType   & childType2 = decayMode.getChildType(1); child2->setType(&childType2); child2->setLive(true);
    LorentzVector & p1 = child1->getMomentum();
    LorentzVector & r1 = child1->getPosition();
    LorentzVector & p2 = child2->getMomentum();
    LorentzVector & r2 = child2->getPosition();
    decayer.decay2(*parentType,
                   parentMomentum,
                   parentPosition,
                   childType1,p1,r1,
                   childType2,p2,r2);
//    double rap1 = p1.Rapidity();
//    double rap2 = p2.Rapidity();
//    if (rap1<-1.0 || rap2<-1.0)
//      {
//      cout << " rap1: " << rap1 << " rap2: " << rap2 << endl;
//      }
    event.add(parent); parent->setLive(false);

    //particleFilter.accept(*child1);
    // // incrementNParticlesAccepted();
    event.add(child1); //child1->setLive(true);
    
    //particleFilter.accept(*child2);
    // // incrementNParticlesAccepted();
    event.add(child2); //child2->setLive(true);
    }
  
/*
  // generate some independent particles
  ParticleType * singleType = particleTypeCollection->findPdgCode(211);
  double pionMass = singleType->getMass();
  mult = int( 5.0 +  double(nParticles) * g->Rndm());
  LorentzVector singlePosition(0.0,0.0,0.0,0.0);
  LorentzVector singleMomentum;
  for (int iParticle = 0; iParticle < mult; iParticle++)
    {
    y   = yMinimum + yRange * g->Rndm();
    phi = TMath::TwoPi() * g->Rndm();
    pt  = 0.2+g->Exp(pTslope);
    px  = pt*cos(phi);
    py  = pt*sin(phi);
    mt  = sqrt(pionMass*pionMass+pt*pt);
    pz  = mt * sinh(y);
    e   = mt * cosh(y);
    singleMomentum.SetPxPyPzE(px,py,pz,e);
    Particle * single = particleFactory->getNextObject();
    single->set(singleType,singleMomentum,singlePosition,true);
    event.add(single);
    }

  singleType = particleTypeCollection->findPdgCode(-211);
  for (int iParticle = 0; iParticle < mult; iParticle++)
    {
    y   = yMinimum + yRange * g->Rndm();
    phi = TMath::TwoPi() * g->Rndm();
    pt  = 0.2+g->Exp(pTslope);
    px  = pt*cos(phi);
    py  = pt*sin(phi);
    mt  = sqrt(pionMass*pionMass+pt*pt);
    pz  = mt * sinh(y);
    e   = mt * cosh(y);
    singleMomentum.SetPxPyPzE(px,py,pz,e);
    Particle * single = particleFactory->getNextObject();
    single->set(singleType,singleMomentum,singlePosition,true);
    event.add(single);
       }
*/

}


