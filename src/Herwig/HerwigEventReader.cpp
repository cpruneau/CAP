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
#include "HerwigEventReader.hpp"
#include "TMath.h"
using CAP::HerwigEventReader;

ClassImp(HerwigEventReader);

HerwigEventReader::HerwigEventReader(const String &          _name,
                                     Configuration &          _configuration,
                                     vector<EventFilter*>   & _eventFilters,
                                     vector<ParticleFilter*>& _particleFilters)
:
RootTreeReader(_name, _configuration, _eventFilters, _particleFilters)
{
  appendClassName("HerwigEventReader");
}

void HerwigEventReader::setDefaultConfiguration()
{
  RootTreeReader::setDefaultConfiguration();
}

void HerwigEventReader::execute()
{
  if (reportStart(__FUNCTION__))
    ;

  //cout << "eventStreams.size() : " << eventStreams.size() << endl;
  incrementTaskExecuted();
  Event & event = * eventStreams[0];
  event.reset();
  particleFactory->reset();
  bool seekingEvent = true;
  //if (reportDebug("PythiaEventReader",getName(),"execute()")) cout << "Start seek loop" << endl;
  while (seekingEvent)
    {
    //if (reportDebug("PythiaEventReader",getName(),"execute()")) cout << "jentry:" << entryIndex << endl;
    // load another event from the root file/TTree
    Long64_t ientry = LoadTree(entryIndex++);
    //if (reportDebug("PythiaEventReader",getName(),"execute()")) cout << "ientry:" << ientry << endl;
    // returning a null point is an indication that
    // there are no more events in the file or stack of files.
    if (ientry < 0)
      {
      postTaskEod(); // end of data
      return;
      }
    nb = rootInputTreeChain()->GetEntry(entryIndex);   nBytes += nb;
    if (reportDebug(__FUNCTION__)) cout << " nb:" << nb << " nParticles:" <<  nTracks << endl;
    if (nTracks>2) seekingEvent = false;
    }

  int thePid;
  double charge, baryon, mass, p_x, p_y, p_z, p_e;
  ParticleType * type;
  Particle * particle;
  int particleAccepted = 0;
  int particleCounted = 0;

  for (int iParticle = 0; iParticle < nTracks; iParticle++)
    {
    //  if (reportDebug("PythiaEventReader",getName(),"execute()")) cout << "iParticle: " << iParticle << endl;


    bool isPrimary = tracks_fIsPrimary[iParticle]>0;
    if (!isPrimary) continue;
//    int ist = tracks_fStatusCode[iParticle];
  //  if (ist <= 0) continue;

    int pdg = tracks_fPDG[iParticle];
    type = particleTypeCollection->findPdgCode(pdg);
    if (type==nullptr) continue;
    mass = type->getMass();
    if (mass<0.002) continue;  // no photons, electrons..
    charge = type->getCharge();
    double pt   = tracks_fPt[iParticle];
    double y    = tracks_fY[iParticle];
    double phi  = tracks_fPhi[iParticle];
    //double m    = tracks_fMass[iParticle];
    //if (m != mass) cout << "  Particle " << iParticle << " pdg Mass:" << mass << " mass in tree:" << m << endl;
    double mt   = sqrt(mass*mass + pt*pt);
    double e    = mt*cosh(y);
    double px   = pt*cos(phi);
    double py   = pt*sin(phi);
    double pz   = mt*sinh(y);
    double sourceX = 0.0;
    double sourceY = 0.0;
    double sourceZ = 0.0;
    double sourceT = 0.0;

    particle = particleFactory->getNextObject();
    particle->set(type,px,py,pz,e,sourceX,sourceY,sourceZ,sourceT,true);
    // incrementParticlesCounted(); // photons are NOT included in this tally
    if (!particleFilters[0]->accept(*particle)) continue;
    event.add(particle);
    // // incrementParticlesAccepted();
    }
}

void HerwigEventReader::initInputTreeMapping()
{
  TTree * tree = rootInputTreeChain();
  tree->SetMakeClass(1);
  tree->SetBranchAddress("tracks", &nTracks, &b_tracks_);
  tree->SetBranchAddress("tracks.fUniqueID", tracks_fUniqueID, &b_tracks_fUniqueID);
  tree->SetBranchAddress("tracks.fBits", tracks_fBits, &b_tracks_fBits);
  tree->SetBranchAddress("tracks.fPt", tracks_fPt, &b_tracks_fPt);
  tree->SetBranchAddress("tracks.fPhi", tracks_fPhi, &b_tracks_fPhi);
  tree->SetBranchAddress("tracks.fY", tracks_fY, &b_tracks_fY);
  tree->SetBranchAddress("tracks.fMass", tracks_fMass, &b_tracks_fMass);
  tree->SetBranchAddress("tracks.fEta", tracks_fEta, &b_tracks_fEta);
  tree->SetBranchAddress("tracks.fPDG", tracks_fPDG, &b_tracks_fPDG);
  tree->SetBranchAddress("tracks.fCharge", tracks_fCharge, &b_tracks_fCharge);
  tree->SetBranchAddress("tracks.fIsPrimary", tracks_fIsPrimary, &b_tracks_fIsPrimary);
  tree->SetBranchAddress("tracks.fStatus", tracks_fStatus, &b_tracks_fStatus);
  tree->SetBranchAddress("fUniqueID", &fUniqueID, &b_event_fUniqueID);
  tree->SetBranchAddress("fBits", &fBits, &b_event_fBits);
  tree->SetBranchAddress("fV0A", &fV0A, &b_event_fV0A);
  tree->SetBranchAddress("fV0C", &fV0C, &b_event_fV0C);
  tree->SetBranchAddress("fCL1", &fCL1, &b_event_fCL1);



}
