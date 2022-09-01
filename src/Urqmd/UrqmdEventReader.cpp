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
#include "UrqmdEventReader.hpp"
ClassImp(UrqmdEventReader);

UrqmdEventReader::UrqmdEventReader(const TString & _name,
                                   Configuration & _configuration,
                                   vector<EventFilter*> & _eventFilters,
                                   vector<ParticleFilter*> & _particleFilters)
:
RootTreeReader(_name, _configuration, _eventFilters, _particleFilters)
{
  appendClassName("UrqmdEventReader");

}

void UrqmdEventReader::setDefaultConfiguration()
{
  RootTreeReader::setDefaultConfiguration();
}

void UrqmdEventReader::execute()
{
  //  if (reportDebug("UrqmdEventReader",getName(),"execute()"))
  //    ;
  incrementTaskExecuted();
  EventFilter & eventFilter = * eventFilters[0];
  ParticleFilter & particleFilter = * particleFilters[0];
  incrementTaskExecuted();
  Event & event = * eventStreams[0];
  event.reset();
  particleFactory->reset();
  // resetParticleCounters();
  Particle * parentInteraction;
  parentInteraction = particleFactory->getNextObject();
  parentInteraction->reset();
  parentInteraction->setType( ParticleType::getInteractionType());
  parentInteraction->setXYZT(0.0, 0.0, 0.0, 0.0);
  event.add(parentInteraction);
  Long64_t ientry = LoadTree(entryIndex);
  if (ientry < 0)
    {
    postTaskEod();  return;
    }
  nb = rootInputTreeChain()->GetEntry(entryIndex++);
  nBytes += nb;
  int nParticles = fTracks_;
  

  //event.setEventNumber(events);
  EventProperties & eventProperties = * event.getEventProperties();
  eventProperties.zProjectile           = 0;
  eventProperties.aProjectile           = 0;
  eventProperties.nPartProjectile       = 0;
  eventProperties.zTarget               = 0;
  eventProperties.aTarget               = 0;
  eventProperties.nPartTarget           = 0;
  eventProperties.nParticipantsTotal    = 0;
  eventProperties.nBinaryTotal          = 0;
  eventProperties.impactParameter       = 0; //impact;
  eventProperties.fractionalXSection    = -99999;
//  eventProperties.refMultiplicity = getNParticlesAccepted();
//  eventProperties.particlesCounted      = getNParticlesCounted();
//  eventProperties.particlesAccepted     = getNParticlesAccepted();
  incrementNEventsAccepted(0);
}

void UrqmdEventReader::initInputTreeMapping()
{
  //TTree * tree = rootInputTreeChain();
//  tree->SetMakeClass(1);
//  tree->SetBranchAddress("fUniqueID", &fUniqueID, &b_UrQMDEventBranch_fUniqueID);
//  tree->SetBranchAddress("fBits", &fBits, &b_UrQMDEventBranch_fBits);
//  tree->SetBranchAddress("fEvtNum", &fEvtNum, &b_UrQMDEventBranch_fEvtNum);
//  tree->SetBranchAddress("fb", &fb, &b_UrQMDEventBranch_fb);
//  tree->SetBranchAddress("fTracks", &fTracks_, &b_UrQMDEventBranch_fTracks_);
//  tree->SetBranchAddress("fTracks.fUniqueID", fTracks_fUniqueID, &b_fTracks_fUniqueID);
//  tree->SetBranchAddress("fTracks.fBits", fTracks_fBits, &b_fTracks_fBits);
//  tree->SetBranchAddress("fTracks.ft", fTracks_ft, &b_fTracks_ft);
//  tree->SetBranchAddress("fTracks.fx", fTracks_fx, &b_fTracks_fx);
//  tree->SetBranchAddress("fTracks.fy", fTracks_fy, &b_fTracks_fy);
//  tree->SetBranchAddress("fTracks.fz", fTracks_fz, &b_fTracks_fz);
//  tree->SetBranchAddress("fTracks.fE", fTracks_fE, &b_fTracks_fE);
//  tree->SetBranchAddress("fTracks.fPx", fTracks_fPx, &b_fTracks_fPx);
//  tree->SetBranchAddress("fTracks.fPy", fTracks_fPy, &b_fTracks_fPy);
//  tree->SetBranchAddress("fTracks.fPz", fTracks_fPz, &b_fTracks_fPz);
//  tree->SetBranchAddress("fTracks.fMass", fTracks_fMass, &b_fTracks_fMass);
//  tree->SetBranchAddress("fTracks.fChIso3Ityp", fTracks_fChIso3Ityp, &b_fTracks_fChIso3Ityp);
//
}
