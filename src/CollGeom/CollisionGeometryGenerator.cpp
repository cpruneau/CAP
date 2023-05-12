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
#include "CollisionGeometryGenerator.hpp"
using CAP::CollisionGeometryGenerator;

ClassImp(CollisionGeometryGenerator);

//!
//! Constructor.
//! @param _name Name given to this task instance
//! @param _configuration Configuration to be used by this task instance
//! @param _eventFilters  Vector of event filters to be used by this task instance
//! @param _requiredLevel Debug/report level to be used by this task instance
CollisionGeometryGenerator::CollisionGeometryGenerator(const String & _name,
                                                       const Configuration & _configuration,
                                                       vector<EventFilter*> & _eventFilters,
                                                       vector<ParticleFilter*>& _particleFilters)
:
EventTask(_name,_configuration,_eventFilters, _particleFilters),
minB(0), minBSq(0.0), maxB(10.0), maxBSq(100.0),
nnCrossSection(0.0),
maxNNDistanceSq(0.)
{
  appendClassName("CollisionGeometryGenerator");
}

CollisionGeometryGenerator::~CollisionGeometryGenerator()
{
  clear();
}

void CollisionGeometryGenerator::setDefaultConfiguration()
{
  addParameter("aNucleusZ", 0);
  addParameter("aNucleusA", 0);
  addParameter("aGeneratorType",    0);
  addParameter("aNRadiusBins",    100);
  addParameter("aMinimumRadius",  0.0);
  addParameter("aMaximumRadius", 10.0);
  addParameter("aParA", 0.0);
  addParameter("aParB", 0.0);
  addParameter("aParC", 0.0);
  addParameter("bNucleusZ", 0.0);
  addParameter("bNucleusA", 0.0);
  addParameter("bGeneratorType", 0);
  addParameter("bNRadiusBins",    100);
  addParameter("bMinimumRadius",  0.0);
  addParameter("bMaximumRadius", 10.0);
  addParameter("bParA", 0.0);
  addParameter("bParB", 0.0);
  addParameter("bParC", 0.0);
  addParameter("nnCrossSection", 40.0);
  addParameter("nBins_b", 100);
  addParameter("Min_b", 0.0);
  addParameter("Max_b", 0.0);
  addParameter("nBins_bxSect", 100);
  addParameter("useRecentering",      true);
  addParameter("useNucleonExclusion", false);
  addParameter("UseParticles",        true);
  addParameter("HistogramsCreate",    true);
  addParameter("HistogramsExport",      true);
  addParameter("EventsUseStream0",     true);
}


void CollisionGeometryGenerator::initialize()
{
  if (reportStart(__FUNCTION__))
;
  Task::initialize();
  Event & event = * eventStreams[0];
  event.setNucleusA(configuration.getValueInt(getName(),"aNucleusZ"), configuration.getValueInt(getName(),"aNucleusA") );
  event.setNucleusB(configuration.getValueInt(getName(),"bNucleusZ"), configuration.getValueInt(getName(),"bNucleusA") );
  addParameter("aNRadiusBins",    100);
  addParameter("aMinimumRadius",  0.0);
  addParameter("aMaximumRadius", 10.0);
  addParameter("aParA", 0.0);
  addParameter("aParB", 0.0);
  addParameter("aParC", 0.0);

  Configuration configGeneratorA;
  configGeneratorA.addParameter(getName(),"generatorType",  configuration.getValueInt(getName(),"aGeneratorType"));
  configGeneratorA.addParameter(getName(),"nRadiusBins",    configuration.getValueInt(getName(),"aNRadiusBins"));
  configGeneratorA.addParameter(getName(),"MinimumRadius",  configuration.getValueInt(getName(),"aMinimumRadius"));
  configGeneratorA.addParameter(getName(),"MaximumRadius",  configuration.getValueInt(getName(),"aMaximumRadius"));
  configGeneratorA.addParameter(getName(),"parA",           configuration.getValueDouble(getName(),"aParA"));
  configGeneratorA.addParameter(getName(),"parB",           configuration.getValueDouble(getName(),"aParB"));
  configGeneratorA.addParameter(getName(),"parC",           configuration.getValueDouble(getName(),"aParC"));
  configGeneratorA.addParameter(getName(),"useRecentering",     configuration.getValueDouble(getName(),"useRecentering"));
  configGeneratorA.addParameter(getName(),"useNucleonExclusion",configuration.getValueDouble(getName(),"useNucleonExclusion"));
  configGeneratorA.addParameter(getName(),"exclusionRadius",    configuration.getValueDouble(getName(),"exclusionRadius"));
  NucleusGenerator * nucleusGeneratorA = new NucleusGenerator("NucleusGeneratorA", configGeneratorA);
  addSubTask(nucleusGeneratorA);

  Configuration configGeneratorB;
  configGeneratorB.addParameter(getName(),"generatorType",  configuration.getValueInt(getName(),"bGeneratorType"));
  configGeneratorB.addParameter(getName(),"nRadiusBins",    configuration.getValueInt(getName(),"bNRadiusBins"));
  configGeneratorB.addParameter(getName(),"MinimumRadius",  configuration.getValueInt(getName(),"bMinimumRadius"));
  configGeneratorB.addParameter(getName(),"MaximumRadius",  configuration.getValueInt(getName(),"bMaximumRadius"));
  configGeneratorB.addParameter(getName(),"parA",           configuration.getValueDouble(getName(),"bParA"));
  configGeneratorB.addParameter(getName(),"parB",           configuration.getValueDouble(getName(),"bParB"));
  configGeneratorB.addParameter(getName(),"parC",           configuration.getValueDouble(getName(),"bParC"));
  configGeneratorB.addParameter(getName(),"useRecentering",     configuration.getValueDouble(getName(),"useRecentering"));
  configGeneratorB.addParameter(getName(),"useNucleonExclusion",configuration.getValueDouble(getName(),"useNucleonExclusion"));
  configGeneratorB.addParameter(getName(),"exclusionRadius",    configuration.getValueDouble(getName(),"exclusionRadius"));
  NucleusGenerator * nucleusGeneratorB = new NucleusGenerator("NucleusGeneratorB", configGeneratorB);
  addSubTask(nucleusGeneratorB);

  minB   = configuration.getValueDouble( "MinB"); minBSq = minB*minB;
  maxB   = configuration.getValueDouble( "MaxB"); maxBSq = maxB*maxB;
  nnCrossSection  = configuration.getValueDouble( "nnCrossSection");
  maxNNDistanceSq = nnCrossSection/3.1415927;
  if (reportInfo(__FUNCTION__))
    {
    cout << endl;
    cout << "================================================================" << endl;
    cout << "================================================================" << endl;
    cout << "      nnCrossSection:"  << nnCrossSection << endl;
    cout << "     maxNNDistanceSq:" << maxNNDistanceSq << endl;
    cout << "        max distance:" << sqrt(maxNNDistanceSq) << endl;
    cout << "================================================================" << endl;
    cout << "================================================================" << endl;
    }
  if (reportEnd(__FUNCTION__))
;
}

void CollisionGeometryGenerator::clear()
{
  eventStreams[0]->clear();
}

void CollisionGeometryGenerator::reset()
{
  eventStreams[0]->reset();
}

void CollisionGeometryGenerator::createEvent()
{
  
  if (reportStart(__FUNCTION__))
;
  incrementTaskExecuted();
  Event & event = *eventStreams[0];
  //event.reset();
  Nucleus & nucleusA = event.getNucleusA();
  Nucleus & nucleusB = event.getNucleusB();
  double rr = gRandom->Rndm();
  double b  = sqrt(minBSq + rr*(maxBSq-minBSq));
  nucleusGeneratorA->generate(nucleusA, -b/2.0);
  nucleusGeneratorB->generate(nucleusB,  b/2.0);
  Particle* interaction;
  for (unsigned int i1=0; i1<nucleusA.getNNucleons(); i1++)
    {
    Particle * nucleonA = nucleusA.getNucleonAt(i1);
    for (unsigned int i2=0; i2<nucleusB.getNNucleons(); i2++)
      {
      Particle * nucleonB = nucleusB.getNucleonAt(i2);
      double dSq = nucleonA->distanceXYSq(nucleonB);
      if (dSq<maxNNDistanceSq)
        {
        //cout << " A:  x= " << nucleonA->getPosition().X() << " B:  x= " << nucleonB->getPosition().X() << " I:  x= " << interaction.getPosition().X() << endl;
        interaction = event.addInteraction(nucleonA,nucleonB);
        if (!nucleonA->isWounded())
          {
          LorentzVector & positionA = nucleonA->getPosition();
        //  event.getParticipantMoments().fill(positionA.X() ,positionA.Y());
          nucleonA->setWounded(true);
          }
        if (!nucleonB->isWounded())
          {
          LorentzVector & positionB = nucleonA->getPosition();
        //  event.getParticipantMoments().fill(positionB.X() ,positionB.Y());
          nucleonB->setWounded(true);
          }
        LorentzVector & positionInt = interaction->getPosition();
       // event.getBinaryMoments().fill(positionInt.X(),positionInt.Y());
        }
      }
    }
  //event.getBinaryMoments().calculate();
  //event.getBinaryMoments().calculate();
//  if (reportInfo("CollisionGeometryGenerator",getName(),"createEvent()"))
//   {
//   cout << "Nuclei after generation" << endl;
//   cout << "             b:" << b << endl;
//   cout << "A:  n  protons:" << nucleusA.getNProtons() << endl;
//   cout << "A:  n neutrons:" << nucleusA.getNNeutrons() << endl;
//   cout << "A:  n nucleons:" << nucleusA.getNNucleons() << endl;
//   cout << "A:  n  wounded:" << nucleusA.countWounded() << endl;
//   cout << "B:  n  protons:" << nucleusB.getNProtons() << endl;
//   cout << "B:  n neutrons:" << nucleusB.getNNeutrons() << endl;
//   cout << "B:  n nucleons:" << nucleusB.getNNucleons() << endl;
//   cout << "B:  n  wounded:" << nucleusB.countWounded() << endl;
//   }
  double nWoundedA = nucleusA.countWounded();
  double nWoundedB = nucleusB.countWounded();
  EventProperties & ep = *event.getEventProperties();
  ep.impactParameter   = b;
  ep.zProjectile       = nucleusA.getNProtons();     // atomic number projectile
  ep.aProjectile       = nucleusA.getNNucleons();    // mass number projectile
  ep.nPartProjectile   = nWoundedA;                  // number of participants  projectile
  ep.zTarget           = nucleusB.getNProtons();     // atomic number target
  ep.aTarget           = nucleusB.getNNucleons();    // mass number target
  ep.nPartTarget       = nWoundedB;                  // number of participants  target
  ep.nParticipantsTotal = nWoundedA+nWoundedB;        // total number of participants
  ep.nBinaryTotal       = event.getNBinaryCollisions();  // total number of binary collisions
  ep.impactParameter    = b;    // nucleus-nucleus center distance in fm
  ep.fractionalXSection        = -99999; // fraction cross section value
  ep.refMultiplicity     = -99999; // number of binary collisions
  if (reportDebug(__FUNCTION__)) event.printProperties(cout);
}

