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
#include "ParticleDecayerTask.hpp"

ClassImp(ParticleDecayerTask);

ParticleDecayerTask::ParticleDecayerTask(const TString &         _name,
                                         const Configuration &   _configuration,
                                         vector<EventFilter*>    _eventFilters,
                                         vector<ParticleFilter*> _particleFilters,
                                         LogLevel                _selectedLevel)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel),
decayer()
{
  appendClassName("ParticleDecayerTask");
  setDefaultConfiguration();
  setConfiguration(_configuration);
 }

void ParticleDecayerTask::setDefaultConfiguration()
{
  configuration.setName("ParticleDecayerTask Configuration");
  configuration.setParameter("useParticles",    true );
  configuration.setParameter("useEventStream0", true );
}

// ====================================================================
// Decay unstable particles in the event
// Decayed particles are retained in the event but their live
// flag is set to false.
// Children particles are added at the tail end of the event
// with a live flag  set to true.
// ====================================================================
void ParticleDecayerTask::execute()
{
  if (reportDebug("ParticleDecayerTask",getName(),"execute()"))
    ;
  incrementEventProcessed();
  Event & event = * eventStreams[0];
  int nParticles = event.getNParticles();
//  if (reportInfo("ParticleDecayerTask",getName(),"execute()"))
//    cout << "     nParticles : " << nParticles << endl;
  if (nParticles< 1)
    {
    if (reportWarning("ParticleDecayerTask",getName(),"execute()"))
      cout << "Skipping empty event." << endl;
    return;
    }

  unsigned int iParticle = 0;
  bool done = false;
  while (!done)
    {
//    if (reportInfo("ParticleDecayerTask",getName(),"execute()"))
//      cout << "iParticle:" << iParticle << endl;
    Particle & parent = * event.getParticleAt(iParticle);
    if (parent.isLive() &&  !parent.isStable() && !parent.isInteraction() )
      {
//      if (reportInfo("ParticleDecayerTask",getName(),"execute()"))
//        cout << "isLive()" <<  endl;
      ParticleType   &    parentType      = parent.getType();
      TLorentzVector &    parentMomentum  = parent.getMomentum();
      TLorentzVector &    parentPosition  = parent.getPosition();
      ParticleDecayMode & decayMode       = parentType.generateDecayMode();
      int nChildren = decayMode.getNChildren();
      switch (nChildren)
        {
          case 1:
          if (reportInfo("ParticleDecayerTask",getName(),"execute()"))
            cout << "case 1  parentType==" << parent.getName() << endl;
          break;
          case 2:
          {
//          if (reportInfo("ParticleDecayerTask",getName(),"execute()"))
//            cout << "               case 2  parentType==" << parent.getName() << endl;
          Particle * child1 = particleFactory->getNextObject();
          Particle * child2 = particleFactory->getNextObject();
          ParticleType   & childType1 = decayMode.getChildType(0); child1->setType(&childType1); child1->setLive(true);
          ParticleType   & childType2 = decayMode.getChildType(1); child2->setType(&childType2); child2->setLive(true);
          TLorentzVector & p1 = child1->getMomentum();
          TLorentzVector & r1 = child1->getPosition();
          TLorentzVector & p2 = child2->getMomentum();
          TLorentzVector & r2 = child2->getPosition();
          decayer.decay2(parentType,
                         parentMomentum,
                         parentPosition,
                         childType1,p1,r1,
                         childType2,p2,r2);
          event.add(child1); child1->setLive(true);
          event.add(child2); child2->setLive(true);
          parent.setDecayed(true);

//          cout << " 2-decay:" << endl;
//          cout << " Parent:" << endl;
//          parent.printProperties(cout);
//          cout << " Child 1:" << endl;
//          child1->printProperties(cout);
//          cout << " Child 2:" << endl;
//          child2->printProperties(cout);


          nParticles += 2;
          }
          break;


          case 3:
          {
//          if (reportInfo("ParticleDecayerTask",getName(),"execute()"))
//            cout << "                           case 3  parentType==" << parent.getName() << endl;
          Particle * child1 = particleFactory->getNextObject();
          Particle * child2 = particleFactory->getNextObject();
          Particle * child3 = particleFactory->getNextObject();
          ParticleType   & childType1 = decayMode.getChildType(0); child1->setType(&childType1); child1->setLive(true);
          ParticleType   & childType2 = decayMode.getChildType(1); child2->setType(&childType2); child2->setLive(true);
          ParticleType   & childType3 = decayMode.getChildType(2); child3->setType(&childType3); child3->setLive(true);
          TLorentzVector & p1 = child1->getMomentum();
          TLorentzVector & r1 = child1->getPosition();
          TLorentzVector & p2 = child2->getMomentum();
          TLorentzVector & r2 = child2->getPosition();
          TLorentzVector & p3 = child3->getMomentum();
          TLorentzVector & r3 = child3->getPosition();
          decayer.decay3(parentType,
                         parentMomentum,
                         parentPosition,
                         childType1,p1,r1,
                         childType2,p2,r2,
                         childType3,p3,r3);
          event.add(child1); child1->setLive(true);
          event.add(child2); child2->setLive(true);
          event.add(child3); child3->setLive(true);
          parent.setDecayed(true);

//          cout << " 3-decay:" << endl;
//          cout << " Parent:" << endl;
//          parent.printProperties(cout);
//          cout << " Child 1:" << endl;
//          child1->printProperties(cout);
//          cout << " Child 2:" << endl;
//          child2->printProperties(cout);
//          cout << " Child 3:" << endl;
//          child3->printProperties(cout);

          nParticles += 3;
          }
          break;
          case 4:
          {
          //cout << "ParticleDecayerTask::execute() skipping 4" << endl;
//          if (reportInfo("ParticleDecayerTask",getName(),"execute()"))
//            cout << "                      case 4  parentType==" << parent.getName() << endl;

          break;
  //        Particle * child1 = particleFactory->getNextObject();
  //        Particle * child2 = particleFactory->getNextObject();
  //        Particle * child3 = particleFactory->getNextObject();
  //        Particle * child4 = particleFactory->getNextObject();
  //        ParticleType   & childType1 = decayMode.getChildType(0); child1->setType(&childType1); child1->setLive(true);
  //        ParticleType   & childType2 = decayMode.getChildType(1); child2->setType(&childType2); child2->setLive(true);
  //        ParticleType   & childType3 = decayMode.getChildType(2); child3->setType(&childType3); child3->setLive(true);
  //        ParticleType   & childType4 = decayMode.getChildType(3); child4->setType(&childType4); child4->setLive(true);
  //        TLorentzVector & p1 = child1->getMomentum();
  //        TLorentzVector & r1 = child1->getPosition();
  //        TLorentzVector & p2 = child2->getMomentum();
  //        TLorentzVector & r2 = child2->getPosition();
  //        TLorentzVector & p3 = child3->getMomentum();
  //        TLorentzVector & r3 = child3->getPosition();
  //        TLorentzVector & p4 = child4->getMomentum();
  //        TLorentzVector & r4 = child4->getPosition();
  //        decayer.decay4(parentType,
  //                       parentMomentum,
  //                       parentPosition,
  //                       childType1,p1,r1,
  //                       childType2,p2,r2,
  //                       childType3,p3,r3,
  //                       childType4,p4,r4);
  //        event.add(child1); child1->setLive(true);
  //        event.add(child2); child2->setLive(true);
  //        event.add(child3); child3->setLive(true);
  //        event.add(child4); child4->setLive(true);
  //        parent.setDecayed(true);
  //        nParticles += 4;
          }
          break;
          case 5:
          break;
        }
     // cout << "ParticleDecayerTask::execute() iParticle" << iParticle << " nParticles:" << nParticles << " event.getNParticles():" << event.getNParticles()<< " Done:" << done << endl;
      }
    iParticle++;
    done = (iParticle >= event.getNParticles());
    }
  nParticles = event.getNParticles();
//  if (reportInfo("ParticleDecayerTask",getName(),"execute()"))
//    cout << "     nParticles : " << nParticles << endl;

 // cout << "ParticleDecayerTask::execute() DONE" << endl;
}





