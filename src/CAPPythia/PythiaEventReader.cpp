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
ClassImp(PythiaEventReader);

PythiaEventReader::PythiaEventReader(const TString &         _name,
                                     const Configuration &   _configuration,
                                     vector<EventFilter*>&   _eventFilters,
                                     vector<ParticleFilter*>&_particleFilters,
                                     LogLevel                _selectedLevel)
:
RootTreeReader(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
}


void PythiaEventReader::setDefaultConfiguration()
{
  RootTreeReader::setDefaultConfiguration();
  Configuration & config = getConfiguration();
  config.addParameter("ppOnly", true);
  config.addParameter("beam",   2212);
  config.addParameter("target", 2212);
  config.addParameter("energy", 2726.0);
}

//!
//! Read a PYTHIA  event from file
//! Copy the event into Event for convenience...
//!
void PythiaEventReader::execute()
{
  if (reportStart("PythiaEventReader",getName(),"execute()"))
    ;
  
  //cout << "eventStreams.size() : " << eventStreams.size() << endl;
  
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
    if (reportDebug("PythiaEventReader",getName(),"execute()")) cout << " nb:" << nb << " nParticles:" <<  nParticles << endl;
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
    
    int ist = particles_fStatusCode[iParticle];
    if (ist <= 0) continue;
    int pdg = particles_fPdgCode[iParticle];
    type = masterCollection->findPdgCode(pdg);
    if (type==nullptr) continue;
    mass = type->getMass();
    if (mass<0.002) continue;  // no photons, electrons...
    charge = type->getCharge();
    double px = particles_fPx[iParticle];
    double py = particles_fPy[iParticle];
    double pz = particles_fPz[iParticle];
    double e  = particles_fE[iParticle];
    double sourceX = 0.0;
    double sourceY = 0.0;
    double sourceZ = 0.0;
    double sourceT = 0.0;
    particle = particleFactory->getNextObject();
    particle->set(type,px,py,pz,e,sourceX,sourceY,sourceZ,sourceT,true);
    incrementParticlesCounted(); // photons are NOT included in this tally
    if (!particleFilters[0]->accept(*particle)) continue;
    event.add(particle);
    incrementParticlesAccepted();
    }
  if (reportDebug("PythiaEventReader",getName(),"execute()"))
    {
    cout << endl;
    cout << "PythiaEventGenerator::execute() No of accepted particles : "<< getNParticlesAccepted() << endl;
    cout << "PythiaEventGenerator::execute() No of counted particles : " << getNParticlesCounted()  << endl;
    }
}

void PythiaEventReader::initInputTreeMapping()
{
  TTree * tree = rootInputTreeChain();
  tree->SetMakeClass(1);
  tree->SetBranchAddress("particles", &nParticles, &b_particles_);
  tree->SetBranchAddress("particles.fUniqueID", particles_fUniqueID, &b_particles_fUniqueID);
  tree->SetBranchAddress("particles.fBits", particles_fBits, &b_particles_fBits);
  tree->SetBranchAddress("particles.fLineColor", particles_fLineColor, &b_particles_fLineColor);
  tree->SetBranchAddress("particles.fLineStyle", particles_fLineStyle, &b_particles_fLineStyle);
  tree->SetBranchAddress("particles.fLineWidth", particles_fLineWidth, &b_particles_fLineWidth);
  tree->SetBranchAddress("particles.fPdgCode", particles_fPdgCode, &b_particles_fPdgCode);
  tree->SetBranchAddress("particles.fStatusCode", particles_fStatusCode, &b_particles_fStatusCode);
  tree->SetBranchAddress("particles.fMother[2]", particles_fMother, &b_particles_fMother);
  tree->SetBranchAddress("particles.fDaughter[2]", particles_fDaughter, &b_particles_fDaughter);
  tree->SetBranchAddress("particles.fWeight", particles_fWeight, &b_particles_fWeight);
  tree->SetBranchAddress("particles.fCalcMass", particles_fCalcMass, &b_particles_fCalcMass);
  tree->SetBranchAddress("particles.fPx", particles_fPx, &b_particles_fPx);
  tree->SetBranchAddress("particles.fPy", particles_fPy, &b_particles_fPy);
  tree->SetBranchAddress("particles.fPz", particles_fPz, &b_particles_fPz);
  tree->SetBranchAddress("particles.fE", particles_fE, &b_particles_fE);
  tree->SetBranchAddress("particles.fVx", particles_fVx, &b_particles_fVx);
  tree->SetBranchAddress("particles.fVy", particles_fVy, &b_particles_fVy);
  tree->SetBranchAddress("particles.fVz", particles_fVz, &b_particles_fVz);
  tree->SetBranchAddress("particles.fVt", particles_fVt, &b_particles_fVt);
  tree->SetBranchAddress("particles.fPolarTheta", particles_fPolarTheta, &b_particles_fPolarTheta);
  tree->SetBranchAddress("particles.fPolarPhi", particles_fPolarPhi, &b_particles_fPolarPhi);
}


