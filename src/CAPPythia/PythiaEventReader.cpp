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
#include "PythiaEventReader.hpp"
using CAP::PythiaEventReader;

ClassImp(PythiaEventReader);


PythiaEventReader::PythiaEventReader(const String & _name,
                                     Configuration &   _configuration,
                                     vector<EventFilter*>&   _eventFilters,
                                     vector<ParticleFilter*>&_particleFilters)
:
RootTreeReader(_name, _configuration, _eventFilters, _particleFilters)
{
}


void PythiaEventReader::setDefaultConfiguration()
{
  RootTreeReader::setDefaultConfiguration();
}

//!
//! Read a PYTHIA  event from file
//! Copy the event into Event for convenience..
//!
void PythiaEventReader::execute()
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
    if (reportDebug(__FUNCTION__)) cout << " nb:" << nb << " nParticles:" <<  nParticles << endl;
    if (nParticles>2) seekingEvent = false;
    }
  
  int thePid;
  double charge, baryon, mass, p_x, p_y, p_z, p_e;
  ParticleType * type;
  Particle * particle;
  int particleAccepted = 0;
  int particleCounted = 0;
  
  for (int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    //  if (reportDebug("PythiaEventReader",getName(),"execute()")) cout << "iParticle: " << iParticle << endl;
    
    int ist = tracks_fStatusCode[iParticle];
    if (ist <= 0) continue;
    int pdg = tracks_fPdgCode[iParticle];
    type = particleTypeCollection->findPdgCode(pdg);
    if (type==nullptr) continue;
    mass = type->getMass();
    if (mass<0.002) continue;  // no photons, electrons..
    charge = type->getCharge();
    double px = tracks_fPx[iParticle];
    double py = tracks_fPy[iParticle];
    double pz = tracks_fPz[iParticle];
    double e  = tracks_fE[iParticle];
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

void PythiaEventReader::initInputTreeMapping()
{
  TTree * tree = rootInputTreeChain();
  tree->SetMakeClass(1);

//  tree->SetBranchAddress("particles", &nParticles, &b_particles_);
//  tree->SetBranchAddress("particles.fUniqueID", particles_fUniqueID, &b_particles_fUniqueID);
//  tree->SetBranchAddress("particles.fBits", particles_fBits, &b_particles_fBits);
//  tree->SetBranchAddress("particles.fLineColor", particles_fLineColor, &b_particles_fLineColor);
//  tree->SetBranchAddress("particles.fLineStyle", particles_fLineStyle, &b_particles_fLineStyle);
//  tree->SetBranchAddress("particles.fLineWidth", particles_fLineWidth, &b_particles_fLineWidth);
//  tree->SetBranchAddress("particles.fPdgCode", particles_fPdgCode, &b_particles_fPdgCode);
//  tree->SetBranchAddress("particles.fStatusCode", particles_fStatusCode, &b_particles_fStatusCode);
//  tree->SetBranchAddress("particles.fMother[2]", particles_fMother, &b_particles_fMother);
//  tree->SetBranchAddress("particles.fDaughter[2]", particles_fDaughter, &b_particles_fDaughter);
//  tree->SetBranchAddress("particles.fWeight", particles_fWeight, &b_particles_fWeight);
//  tree->SetBranchAddress("particles.fCalcMass", particles_fCalcMass, &b_particles_fCalcMass);
//  tree->SetBranchAddress("particles.fPx", particles_fPx, &b_particles_fPx);
//  tree->SetBranchAddress("particles.fPy", particles_fPy, &b_particles_fPy);
//  tree->SetBranchAddress("particles.fPz", particles_fPz, &b_particles_fPz);
//  tree->SetBranchAddress("particles.fE", particles_fE, &b_particles_fE);
//  tree->SetBranchAddress("particles.fVx", particles_fVx, &b_particles_fVx);
//  tree->SetBranchAddress("particles.fVy", particles_fVy, &b_particles_fVy);
//  tree->SetBranchAddress("particles.fVz", particles_fVz, &b_particles_fVz);
//  tree->SetBranchAddress("particles.fVt", particles_fVt, &b_particles_fVt);
//  tree->SetBranchAddress("particles.fPolarTheta", particles_fPolarTheta, &b_particles_fPolarTheta);
//  tree->SetBranchAddress("particles.fPolarPhi", particles_fPolarPhi, &b_particles_fPolarPhi);

  tree->SetBranchAddress("tracks", &nParticles, &b_tracks_);
  tree->SetBranchAddress("tracks.fUniqueID", tracks_fUniqueID, &b_tracks_fUniqueID);
  tree->SetBranchAddress("tracks.fBits", tracks_fBits, &b_tracks_fBits);
  tree->SetBranchAddress("tracks.fLineColor", tracks_fLineColor, &b_tracks_fLineColor);
  tree->SetBranchAddress("tracks.fLineStyle", tracks_fLineStyle, &b_tracks_fLineStyle);
  tree->SetBranchAddress("tracks.fLineWidth", tracks_fLineWidth, &b_tracks_fLineWidth);
  tree->SetBranchAddress("tracks.fPdgCode", tracks_fPdgCode, &b_tracks_fPdgCode);
  tree->SetBranchAddress("tracks.fStatusCode", tracks_fStatusCode, &b_tracks_fStatusCode);
  tree->SetBranchAddress("tracks.fMother[2]", tracks_fMother, &b_tracks_fMother);
  tree->SetBranchAddress("tracks.fDaughter[2]", tracks_fDaughter, &b_tracks_fDaughter);
  tree->SetBranchAddress("tracks.fWeight", tracks_fWeight, &b_tracks_fWeight);
  tree->SetBranchAddress("tracks.fCalcMass", tracks_fCalcMass, &b_tracks_fCalcMass);
  tree->SetBranchAddress("tracks.fPx", tracks_fPx, &b_tracks_fPx);
  tree->SetBranchAddress("tracks.fPy", tracks_fPy, &b_tracks_fPy);
  tree->SetBranchAddress("tracks.fPz", tracks_fPz, &b_tracks_fPz);
  tree->SetBranchAddress("tracks.fE", tracks_fE, &b_tracks_fE);
  tree->SetBranchAddress("tracks.fVx", tracks_fVx, &b_tracks_fVx);
  tree->SetBranchAddress("tracks.fVy", tracks_fVy, &b_tracks_fVy);
  tree->SetBranchAddress("tracks.fVz", tracks_fVz, &b_tracks_fVz);
  tree->SetBranchAddress("tracks.fVt", tracks_fVt, &b_tracks_fVt);
  tree->SetBranchAddress("tracks.fPolarTheta", tracks_fPolarTheta, &b_tracks_fPolarTheta);
  tree->SetBranchAddress("tracks.fPolarPhi", tracks_fPolarPhi, &b_tracks_fPolarPhi);
  tree->SetBranchAddress("HP_Inc1", &HP_Inc1, &b_HP_Inc1);
  tree->SetBranchAddress("HP_Inc2", &HP_Inc2, &b_HP_Inc2);
  tree->SetBranchAddress("HP_Inc3", &HP_Inc3, &b_HP_Inc3);
  tree->SetBranchAddress("HP_Inc4", &HP_Inc4, &b_HP_Inc4);
  tree->SetBranchAddress("HP_Out1", &HP_Out1, &b_HP_Out1);
  tree->SetBranchAddress("HP_Out2", &HP_Out2, &b_HP_Out2);
  tree->SetBranchAddress("HP_Out3", &HP_Out3, &b_HP_Out3);
  tree->SetBranchAddress("HP_Out4", &HP_Out4, &b_HP_Out4);
  tree->SetBranchAddress("HP_Inter1", &HP_Inter1, &b_HP_Inter1);
  tree->SetBranchAddress("HP_Inter2", &HP_Inter2, &b_HP_Inter2);
  tree->SetBranchAddress("HP_Inter3", &HP_Inter3, &b_HP_Inter3);
  tree->SetBranchAddress("HP_Inter4", &HP_Inter4, &b_HP_Inter4);
  tree->SetBranchAddress("HP_Kick1", &HP_Kick1, &b_HP_Kick1);
  tree->SetBranchAddress("HP_Kick2", &HP_Kick2, &b_HP_Kick2);
  tree->SetBranchAddress("HP_Kick3", &HP_Kick3, &b_HP_Kick3);
  tree->SetBranchAddress("HP_Kick4", &HP_Kick4, &b_HP_Kick4);


}


