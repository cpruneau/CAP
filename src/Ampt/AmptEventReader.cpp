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
#include "AmptEventReader.hpp"
ClassImp(AmptEventReader);

AmptEventReader::AmptEventReader(const TString &          _name,
                                 Configuration &          _configuration,
                                 vector<EventFilter*>   & _eventFilters,
                                 vector<ParticleFilter*>& _particleFilters,
                                 LogLevel                 _selectedLevel)
:
RootTreeReader(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  appendClassName("RootTreeReader");
  setDefaultConfiguration();
  setConfiguration(_configuration);
}

void AmptEventReader::setDefaultConfiguration()
{
  RootTreeReader::setDefaultConfiguration();
}

void AmptEventReader::execute()
{
//  if (reportDebug("AmptEventReader",getName(),"execute()"))
//    ;

  EventFilter & eventFilter = * eventFilters[0];
  ParticleFilter & particleFilter = * particleFilters[0];
  incrementEventProcessed();
  Event & event = * eventStreams[0];
  event.reset();
  particleFactory->reset();
  resetParticleCounters();
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
  if (nParticles > arraySize)
    {
    if (reportError("AmptEventReader",getName(),"execute()"))
      cout<< "nParticles: " << nParticles << "  exceeds capacity " << arraySize << endl;
    postTaskFatal();
    exit(1);
    }
  
  double eventPhi;
  double cosPhi;
  double sinPhi;
  if (useRandomizeEventPlane())
    {
    eventPhi = getRandomEventPlaneAngle();
    cosPhi = cos(eventPhi);
    sinPhi = sin(eventPhi);
    }
  
  TLorentzVector & sourcePosition = parentInteraction->getPosition();
  double r_x = sourcePosition.X();
  double r_y = sourcePosition.Y();
  double r_z = sourcePosition.Z();
  double r_t = sourcePosition.T();
  ParticleType * type;
  //if (reportDebug("AmptEventReader",getName(),"execute()")) cout << " Starting loop with nParticles: " << nParticles << endl;
  for (int iParticle=0; iParticle<nParticles; iParticle++)
    {
    int pdgCode = pid[iParticle];
    type = masterCollection->findPdgCode(pdgCode);
    if (type==nullptr)
      {
      if (reportWarning("AmptEventReader",getName(),"execute()")) cout << "Encountered unknown pdgCode: " << pdgCode << " Particle not added to event." << endl;
      continue;
      }
    Particle * particle = particleFactory->getNextObject();
    double p_x, p_y, p_z, p_e, mass;
    if (useRandomizeEventPlane())
      {
      p_x  = cosPhi*px[iParticle] - sinPhi*py[iParticle];
      p_y  = sinPhi*px[iParticle] + cosPhi*py[iParticle];
      }
    else
      {
      p_x  = px[iParticle];
      p_y  = py[iParticle];
      }
    p_z  = pz[iParticle];
    mass = m[iParticle];
    p_e  =sqrt(p_x*p_x + p_y*p_y + p_z*p_z + mass*mass);
    particle->set(type,p_x,p_y,p_z,p_e,r_x,r_y,r_z,r_t,true);
    incrementParticlesCounted(); // photons are NOT included in this tally
    if (!particleFilter.accept(*particle)) continue;
    event.add(particle);
    incrementParticlesAccepted();
    }
  event.setEventNumber(eventNo);
  EventProperties & eventProperties = * event.getEventProperties();
  eventProperties.zProjectile           = 0;
  eventProperties.aProjectile           = 0;
  eventProperties.nPartProjectile       = Nproj;
  eventProperties.zTarget               = 0;
  eventProperties.aTarget               = 0;
  eventProperties.nPartTarget           = Ntarg;
  eventProperties.nParticipantsTotal    = Nproj+Ntarg;
  eventProperties.nBinaryTotal          = 0;
  eventProperties.impactParameter       = impact;
  eventProperties.fractionalXSection    = -99999;
  eventProperties.referenceMultiplicity = getNParticlesAccepted();
  eventProperties.particlesCounted      = getNParticlesCounted();
  eventProperties.particlesAccepted     = getNParticlesAccepted();
  incrementEventAccepted(0);
//  if (reportDebug("AmptEventReader",getName(),"execute()"))
//    {
//    eventProperties.printProperties(cout);
//    cout << "AmptEventReader::execute() event completed!" << endl;
//    }
}

void AmptEventReader::initInputTreeMapping()
{
  TTree * tree = rootInputTreeChain();
  tree->SetMakeClass(1);
  tree->SetBranchAddress("eventNo", &eventNo, &b_eventNo);
  tree->SetBranchAddress("mult",    &nParticles, &b_mult);
  tree->SetBranchAddress("Nproj",   &Nproj, &b_Nproj);
  tree->SetBranchAddress("Ntarg",   &Ntarg, &b_Ntarg);
  tree->SetBranchAddress("impact",  &impact, &b_impact);
  tree->SetBranchAddress("Nparttotal", &nPartTotal, &b_nPartTotal);
  tree->SetBranchAddress("pid", pid, &b_pid);
  tree->SetBranchAddress("px", px, &b_px);
  tree->SetBranchAddress("py", py, &b_py);
  tree->SetBranchAddress("pz", pz, &b_pz);
  tree->SetBranchAddress("m", m, &b_m);
  tree->SetBranchAddress("Nx", Nx, &b_Nx);
  tree->SetBranchAddress("Ny", Ny, &b_Ny);
  }

