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
nucleusGeneratorA(new NucleusGenerator()),
nucleusGeneratorB(new NucleusGenerator()),
minB(0), minBSq(0.0), maxB(10.0), maxBSq(100.0),
nnCrossSection(0.0),
maxNNDistanceSq(0.)
{
  appendClassName(_name);
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

CollisionGeometryGenerator::~CollisionGeometryGenerator()
{
  clear();
  delete nucleusGeneratorA;
  delete nucleusGeneratorB;
}

void CollisionGeometryGenerator::setDefaultConfiguration()
{
  if (reportStart("CollisionGeometryGenerator",getName(),"setDefaultConfiguration()"))
    {
    }
  Configuration & configuration = getConfiguration();
  configuration.setName("CollisionGeometryGenerator Configuration");
  configuration.addParameter(  "aNucleusZ", 0);
  configuration.addParameter(  "aNucleusA", 0);
  configuration.addParameter(  "aGeneratorType", NucleusGenerator::WoodsSaxon);
  configuration.addParameter(  "aParA", 0.0);
  configuration.addParameter(  "aParB", 0.0);
  configuration.addParameter(  "aParC", 0.0);
  configuration.addParameter(  "aNR",  100);
  configuration.addParameter(  "aMinR", 0.0);
  configuration.addParameter(  "aMaxR", 0.0);
  configuration.addParameter(  "bNucleusZ", 0.0);
  configuration.addParameter(  "bNucleusA", 0.0);
  configuration.addParameter(  "bGeneratorType", NucleusGenerator::WoodsSaxon);
  configuration.addParameter(  "bParA", 0.0);
  configuration.addParameter(  "bParB", 0.0);
  configuration.addParameter(  "bParC", 0.0);
  configuration.addParameter(  "bNR", 100);
  configuration.addParameter(  "bMinR", 0.0);
  configuration.addParameter(  "bMaxR", 0.0);
  configuration.addParameter(  "minB", 0.0);
  configuration.addParameter(  "maxB", 0.0);
  configuration.addParameter(  "nnCrossSection", 40.0);
  configuration.addParameter(  "nBins_b", 100);
  configuration.addParameter(  "min_b", 0.0);
  configuration.addParameter(  "max_b", 0.0);
  configuration.addParameter(  "nBins_bxSect", 100);

  configuration.addParameter("nBins_b",100);
  configuration.addParameter("min_b", 0.0);
  configuration.addParameter("max_b", 10.0);
  configuration.addParameter("nBins_nPart", 400);
  configuration.addParameter("min_nPart", 0.0);
  configuration.addParameter("max_nPart", 400.0);
  configuration.addParameter("nBins_nBinary", 1000);
  configuration.addParameter("min_nBinary", 0.0);
  configuration.addParameter("max_nBinary", 1000.0);
                             
  configuration.addParameter(  "useRecentering",      true);
  configuration.addParameter(  "useNucleonExclusion", false);
  configuration.addParameter(  "useParticles",        true);
  configuration.addParameter(  "createHistograms",    true);
  configuration.addParameter(  "saveHistograms",      true);
  configuration.addParameter(  "useEventStream0",     true);
  configuration.addParameter(  "createDerivedHistograms", true);
  if (reportDebug("CollisionGeometryGenerator",getName(),"setDefaultConfiguration()"))
    {
    configuration.printConfiguration(cout);
    }
}


void CollisionGeometryGenerator::initialize()
{
  if (reportStart("CollisionGeometryGenerator",getName(),"initialize()"))
    ;
  Task::initialize();
  Configuration & configuration = getConfiguration();
  Event & event = * eventStreams[0];
  event.setNucleusA(configuration.getValueInt( "aNucleusZ"), configuration.getValueInt( "aNucleusA") );
  event.setNucleusB(configuration.getValueInt( "bNucleusZ"), configuration.getValueInt( "bNucleusA") );
  nucleusGeneratorA->initialize("NGA",
                                configuration.getValueInt( "aGeneratorType"),
                                configuration.getValueDouble( "aParA"),
                                configuration.getValueDouble( "aParB"),
                                configuration.getValueDouble( "aParC"),
                                configuration.getValueInt( "aNR"),
                                configuration.getValueDouble( "aMinR"),
                                configuration.getValueDouble( "aMaxR"),
                                configuration.getValueBool( "useRecentering"),
                                configuration.getValueBool( "useNucleonExclusion"),
                                configuration.getValueDouble( "exclusionRadius"));
  nucleusGeneratorB->initialize("NGB",
                                configuration.getValueInt( "bGeneratorType"),
                                configuration.getValueDouble( "bParA"),
                                configuration.getValueDouble( "bParB"),
                                configuration.getValueDouble( "bParC"),
                                configuration.getValueInt( "bNR"),
                                configuration.getValueDouble( "bMinR"),
                                configuration.getValueDouble( "bMaxR"),
                                configuration.getValueBool( "useRecentering"),
                                configuration.getValueBool( "useNucleonExclusion"),
                                configuration.getValueDouble( "exclusionRadius"));
  minB   = configuration.getValueDouble( "minB"); minBSq = minB*minB;
  maxB   = configuration.getValueDouble( "maxB"); maxBSq = maxB*maxB;
  nnCrossSection  = configuration.getValueDouble( "nnCrossSection");
  maxNNDistanceSq = nnCrossSection/3.1415927;
  if (reportInfo("CollisionGeometryGenerator",getName(),"initialize()"))
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
  if (reportEnd("CollisionGeometryGenerator",getName(),"initialize()"))
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
  if (reportStart("CollisionGeometryGenerator",getName(),"execute()"))
    ;
  incrementEventProcessed();
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
          event.getParticipantMoments().fill(positionA.X() ,positionA.Y());
          nucleonA->setWounded(true);
          }
        if (!nucleonB->isWounded())
          {
          TLorentzVector & positionB = nucleonA->getPosition();
          event.getParticipantMoments().fill(positionB.X() ,positionB.Y());
          nucleonB->setWounded(true);
          }
        TLorentzVector & positionInt = interaction->getPosition();
        event.getBinaryMoments().fill(positionInt.X(),positionInt.Y());
        }
      }
    }
  event.getBinaryMoments().calculate();
  event.getBinaryMoments().calculate();
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
  ep.nParticipantsTotal        = nWoundedA+nWoundedB;        // total number of participants
  ep.nBinaryTotal      = event.getNBinaryCollisions();  // total number of binary collisions
  ep.impactParameter   = b;    // nucleus-nucleus center distance in fm
  ep.fractionalXSection        = -99999; // fraction cross section value
  ep.referenceMultiplicity     = -99999; // number of binary collisions
  ep.particlesCounted  = -99999;
  ep.particlesAccepted = -99999;
  if (reportDebug("CollisionGeometryGenerator",getName(),"execute()"))
    {
    event.printProperties(cout);
    }
  if (reportEnd("CollisionGeometryGenerator",getName(),"execute()"))
    ;
}

