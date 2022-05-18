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
ClassImp(CollisionGeometryGenerator);

//!
//! Constructor.
//! @param _name Name given to this task instance
//! @param _configuration Configuration to be used by this task instance
//! @param _eventFilters  Vector of event filters to be used by this task instance
//! @param _requiredLevel Debug/report level to be used by this task instance
CollisionGeometryGenerator::CollisionGeometryGenerator(const TString &          _name,
                                                       const Configuration  &   _configuration,
                                                       vector<EventFilter*> &   _eventFilters,
                                                       vector<ParticleFilter*>  _particleFilters,
                                                       MessageLogger::LogLevel  _reportLevel)
:
Task(_name,_configuration,_eventFilters, _particleFilters, _reportLevel),
minB(0), minBSq(0.0), maxB(10.0), maxBSq(100.0),
nnCrossSection(0.0),
maxNNDistanceSq(0.)
{
  appendClassName("CollisionGeometryGenerator");
  setInstanceName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

CollisionGeometryGenerator::~CollisionGeometryGenerator()
{
  clear();
}

void CollisionGeometryGenerator::setDefaultConfiguration()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Configuration & configuration = getConfiguration();
  configuration.setName("CollisionGeometryGenerator Configuration");
  configuration.addParameter("aNucleusZ", 0);
  configuration.addParameter("aNucleusA", 0);
  configuration.addParameter("aGeneratorType",    0);
  configuration.addParameter("aNRadiusBins",    100);
  configuration.addParameter("aMinimumRadius",  0.0);
  configuration.addParameter("aMaximumRadius", 10.0);
  configuration.addParameter("aParA", 0.0);
  configuration.addParameter("aParB", 0.0);
  configuration.addParameter("aParC", 0.0);

  configuration.addParameter("bNucleusZ", 0.0);
  configuration.addParameter("bNucleusA", 0.0);
  configuration.addParameter("bGeneratorType", 0);
  configuration.addParameter("bNRadiusBins",    100);
  configuration.addParameter("bMinimumRadius",  0.0);
  configuration.addParameter("bMaximumRadius", 10.0);
  configuration.addParameter("bParA", 0.0);
  configuration.addParameter("bParB", 0.0);
  configuration.addParameter("bParC", 0.0);

  configuration.addParameter("nnCrossSection", 40.0);
  configuration.addParameter("nBins_b", 100);
  configuration.addParameter("min_b", 0.0);
  configuration.addParameter("max_b", 0.0);
  configuration.addParameter("nBins_bxSect", 100);

  configuration.addParameter("useRecentering",      true);
  configuration.addParameter("useNucleonExclusion", false);
  configuration.setParameter("useParticles",        true);
  configuration.addParameter("createHistograms",    true);
  configuration.addParameter("saveHistograms",      true);
  configuration.addParameter("useEventStream0",     true);

  if (reportDebug(__FUNCTION__)) configuration.printConfiguration(cout);
}


void CollisionGeometryGenerator::initialize()
{
  
  if (reportStart(__FUNCTION__))
    ;
  Task::initialize();

  Event & event = * eventStreams[0];
  event.setNucleusA(configuration.getValueInt("aNucleusZ"), configuration.getValueInt("aNucleusA") );
  event.setNucleusB(configuration.getValueInt("bNucleusZ"), configuration.getValueInt("bNucleusA") );

  configuration.addParameter("aNRadiusBins",    100);
  configuration.addParameter("aMinimumRadius",  0.0);
  configuration.addParameter("aMaximumRadius", 10.0);
  configuration.addParameter("aParA", 0.0);
  configuration.addParameter("aParB", 0.0);
  configuration.addParameter("aParC", 0.0);


  Configuration configGeneratorA;
  configGeneratorA.addParameter("generatorType",  configuration.getValueInt("aGeneratorType"));
  configGeneratorA.addParameter("nRadiusBins",    configuration.getValueInt("aNRadiusBins"));
  configGeneratorA.addParameter("minimumRadius",  configuration.getValueInt("aMinimumRadius"));
  configGeneratorA.addParameter("maximumRadius",  configuration.getValueInt("aMaximumRadius"));
  configGeneratorA.addParameter("parA",           configuration.getValueDouble("aParA"));
  configGeneratorA.addParameter("parB",           configuration.getValueDouble("aParB"));
  configGeneratorA.addParameter("parC",           configuration.getValueDouble("aParC"));
  configGeneratorA.addParameter("useRecentering",     configuration.getValueDouble("useRecentering"));
  configGeneratorA.addParameter("useNucleonExclusion",configuration.getValueDouble("useNucleonExclusion"));
  configGeneratorA.addParameter("exclusionRadius",    configuration.getValueDouble("exclusionRadius"));
  NucleusGenerator * nucleusGeneratorA = new NucleusGenerator("NucleusGeneratorA", configGeneratorA, getReportLevel());
  addSubTask(nucleusGeneratorA);

  Configuration configGeneratorB;
  configGeneratorB.addParameter("generatorType",  configuration.getValueInt("bGeneratorType"));
  configGeneratorB.addParameter("nRadiusBins",    configuration.getValueInt("bNRadiusBins"));
  configGeneratorB.addParameter("minimumRadius",  configuration.getValueInt("bMinimumRadius"));
  configGeneratorB.addParameter("maximumRadius",  configuration.getValueInt("bMaximumRadius"));
  configGeneratorB.addParameter("parA",           configuration.getValueDouble("bParA"));
  configGeneratorB.addParameter("parB",           configuration.getValueDouble("bParB"));
  configGeneratorB.addParameter("parC",           configuration.getValueDouble("bParC"));
  configGeneratorB.addParameter("useRecentering",     configuration.getValueDouble("useRecentering"));
  configGeneratorB.addParameter("useNucleonExclusion",configuration.getValueDouble("useNucleonExclusion"));
  configGeneratorB.addParameter("exclusionRadius",    configuration.getValueDouble("exclusionRadius"));
  NucleusGenerator * nucleusGeneratorB = new NucleusGenerator("NucleusGeneratorB", configGeneratorB, getReportLevel());
  addSubTask(nucleusGeneratorB);

  minB   = configuration.getValueDouble( "minB"); minBSq = minB*minB;
  maxB   = configuration.getValueDouble( "maxB"); maxBSq = maxB*maxB;
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

void CollisionGeometryGenerator::execute()
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
          TLorentzVector & positionA = nucleonA->getPosition();
        //  event.getParticipantMoments().fill(positionA.X() ,positionA.Y());
          nucleonA->setWounded(true);
          }
        if (!nucleonB->isWounded())
          {
          TLorentzVector & positionB = nucleonA->getPosition();
        //  event.getParticipantMoments().fill(positionB.X() ,positionB.Y());
          nucleonB->setWounded(true);
          }
        TLorentzVector & positionInt = interaction->getPosition();
       // event.getBinaryMoments().fill(positionInt.X(),positionInt.Y());
        }
      }
    }
  //event.getBinaryMoments().calculate();
  //event.getBinaryMoments().calculate();
//  if (reportInfo("CollisionGeometryGenerator",getName(),"execute()"))
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
  ep.referenceMultiplicity     = -99999; // number of binary collisions
  if (reportDebug(__FUNCTION__)) event.printProperties(cout);
}

