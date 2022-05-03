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
                                 LogLevel                 _selectedLevel=Info)
:
Task(_name, _configuration, _eventFilters, _particleFilters, _selectedLevel)
{
  if (reportDebug()) cout << "AmptEventReader::AmptEventReader(...) No ops" << endl;
}

AmptEventReader::~AmptEventReader()
{
if (reportDebug()) cout << "AmptEventReader::~AmptEventReader(...) No ops" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AmptEventReader::initialize()
{
  if (reportDebug()) cout << "AmptEventReader::initialize() Started" << endl;

  TaskConfiguration * genConfig = getTaskConfiguration();
  if (!genConfig)
    {
    if (reportFatal()) cout << "AmptEventReader::initialize() genConfig is a null pointer" << endl;
    postTaskFatal();
    return;
    }
  TChain *chain = new TChain(genConfig->dataInputTreeName," ");
  if (!chain)
     {
     if (reportFatal()) cout << "AmptEventReader::initialize() chain is a null pointer" << endl;
     postTaskFatal();
     return;
     }

  TString dataInputFileName = genConfig->dataInputPath;
  dataInputFileName += "/";
  dataInputFileName += genConfig->dataInputFileName;

  for(int ifl=genConfig->dataInputFileMinIndex; ifl<genConfig->dataInputFileMaxIndex; ifl++)
    {
    //chain->Add(Form("/local/victor/PROJECTS/EPOSWSU/DATA/epos_pbpb_urqmd_on_%i.root",ifl+1));
    //chain->Add(Form("/Users/sumit/Desktop/macros_epos/root_maker/epos_pbpb_urqmd_on_%i.root",ifl+1));
    cout << "AmptEventReader::initialize() Form:" << Form(dataInputFileName,ifl) << endl;
    chain->Add(Form(dataInputFileName,ifl));
    }
  Init(chain);
  jentry = 0;

  nentries = fChain->GetEntries();
  if (reportInfo()) cout << "AmptEventReader::initialize() nEntries: " << nentries << endl;
  if (nentries < 1)
    {
    if (reportError()) cout << "AmptEventReader::initialize() no data found. Abort." << endl;
    postTaskFatal();
    return;
    }
  nbytes = 0;
  nb = 0;
  if (reportDebug()) cout << "AmptEventReader::initialize() Completed" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reset and Initialize the generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AmptEventReader::reset()
{
  if (reportDebug()) cout << "AmptEventReader::reset() Started" << endl;
  event->reset();
  Particle::getFactory()->reset();
  if (reportDebug()) cout << "AmptEventReader::reset() Completed" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read an ampt event from file
// Copy the event into Event for convenience...
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AmptEventReader::execute()
{
  if (reportDebug()) cout << "AmptEventReader::execute() Started" << endl;
  if (!fChain)
    {
    if (reportFatal()) cout << " AmptEventReader::execute() no TChain available" << endl;
    postTaskFatal();
    return;
    }

  Factory<Particle> * particleFactory = Particle::getFactory();

  bool seekingEvent = true;
  while (seekingEvent)
    {
    // load another event from the root file/TTree
    Long64_t ientry = LoadTree(jentry++);

    // returning a null point is an indication that
    // there are no more events in the file or stack of files.
    if (ientry < 0)
      {
      postTaskEod(); // end of data
      return;
      }
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (mult > arraySize)
         {
         if (reportError()) cout<< "AmptEventReader::execute() n particles is " << mult << " and exceeds capacity " << arraySize << endl;
         postTaskError();
         return;
         }
    event->eventNumber     = eventNo;
    event->impactParameter = impact;
    event->nProjectile     = Nproj;
    event->nTarget         = Ntarg;
    event->nParticleTotal  = Nparttotal;
    event->multiplicity    = mult;
    if (eventFilter->accept(*event)) seekingEvent = false;
    }
  int thePid;
  double charge, p_x, p_y, p_z, p_e, mass;
  Particle aParticle;
  Particle * particle;
  int particleAccepted = 0;

   //------------------- Randomizing the particle phi --------------Starts
  double eventAngle= TMath::TwoPi() * gRandom->Rndm();
  double cosPhi = cos(eventAngle);
  double sinPhi = sin(eventAngle);

   for (int iParticle; iParticle<mult; iParticle++)
    {
    thePid = pid[iParticle];
    if ( thePid == 211 || thePid == 321  || thePid ==2212 )
      charge = 1;
    else if (thePid ==-211 || thePid ==-321 || thePid==-2212)
      charge = -1;
    else
      charge = 0;
    p_x  = cosPhi*px[iParticle] - sinPhi*py[iParticle];
    p_y  = sinPhi*px[iParticle] + cosPhi*py[iParticle];
    p_z  = pz[iParticle];
    mass = m[iParticle];
    p_e  =sqrt(p_x*p_x + p_y*p_y + p_z*p_z + mass*mass);
    aParticle.setPidPxPyPzE(thePid, charge, p_x,p_y,p_z,p_e);
    //if (reportDebug("AmptEventReader","execute()",getName())) aParticle.printProperties(cout);
    if (particleFilter->accept(aParticle))
      {
      particle = particleFactory->getNextObject();
      *particle = aParticle;
      particleAccepted++;
      }
    }
  event->nParticles = particleAccepted;
  if (reportDebug("AmptEventReader","execute()",getName()))
    {
    event->printProperties(cout);
    cout << "AmptEventReader::execute() event completed!" << endl;
    }
}

Int_t AmptEventReader::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t AmptEventReader::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void AmptEventReader::Init(TTree *tree)
{
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("eventNo", &eventNo, &b_eventNo);
  fChain->SetBranchAddress("mult", &mult, &b_mult);
  fChain->SetBranchAddress("Nproj", &Nproj, &b_Nproj);
  fChain->SetBranchAddress("Ntarg", &Ntarg, &b_Ntarg);
  fChain->SetBranchAddress("impact", &impact, &b_impact);
  fChain->SetBranchAddress("Nparttotal", &Nparttotal, &b_Nparttotal);
  fChain->SetBranchAddress("pid", pid, &b_pid);
  fChain->SetBranchAddress("px", px, &b_px);
  fChain->SetBranchAddress("py", py, &b_py);
  fChain->SetBranchAddress("pz", pz, &b_pz);
  fChain->SetBranchAddress("m", m, &b_m);
  fChain->SetBranchAddress("Nx", Nx, &b_Nx);
  fChain->SetBranchAddress("Ny", Ny, &b_Ny);
  Notify();
  nentries = fChain->GetEntriesFast();
  nbytes = 0;
  nb = 0;
}

Bool_t AmptEventReader::Notify()
{
  return kTRUE;
}

void AmptEventReader::Show(Long64_t entry)
{
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t AmptEventReader::Cut(Long64_t entry)
{
  return 1;
}


