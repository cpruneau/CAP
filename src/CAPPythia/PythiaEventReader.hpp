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
#ifndef CAP__PythiaEventReader
#define CAP__PythiaEventReader
#include "RootTreeReader.hpp"

//!
//! Class defining a ROOT Tree reader for PYTHIA events produced by Pythia8.
//! The Init() method maps the tree branch onto variables used by this reader
//! Particles may be copied into a CAP event stream for analysis.
//!
class PythiaEventReader : public RootTreeReader
{
public:

  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task
  //! @param _particleFilters Array of particle filters to be used by this task
  //! @param _reportLevel Message log level to be used by this task.
  //!
  PythiaEventReader(const TString &         _name,
                    const Configuration &   _configuration,
                    vector<EventFilter*>&   _eventFilters,
                    vector<ParticleFilter*>&_particleFilters,
                    LogLevel                _selectedLevel);
  
  //!
  //! DTOR
  //!
  virtual ~PythiaEventReader() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  virtual void setDefaultConfiguration();

  //!
  //! Read in one PYTHIA event from file.
  //!
  virtual void execute();
  
  //!
  //! Initialize the mapping of Root Tree Branches onto local variables..
  //!
  void Init(TTree *tree);

protected:

  // Declaration of leaf types
  Int_t           particles_;
  UInt_t          particles_fUniqueID[kMaxparticles];   //[particles_]
  UInt_t          particles_fBits[kMaxparticles];   //[particles_]
  Short_t         particles_fLineColor[kMaxparticles];   //[particles_]
  Short_t         particles_fLineStyle[kMaxparticles];   //[particles_]
  Short_t         particles_fLineWidth[kMaxparticles];   //[particles_]
  Int_t           particles_fPdgCode[kMaxparticles];   //[particles_]
  Int_t           particles_fStatusCode[kMaxparticles];   //[particles_]
  Int_t           particles_fMother[kMaxparticles][2];   //[particles_]
  Int_t           particles_fDaughter[kMaxparticles][2];   //[particles_]
  Float_t         particles_fWeight[kMaxparticles];   //[particles_]
  Double_t        particles_fCalcMass[kMaxparticles];   //[particles_]
  Double_t        particles_fPx[kMaxparticles];   //[particles_]
  Double_t        particles_fPy[kMaxparticles];   //[particles_]
  Double_t        particles_fPz[kMaxparticles];   //[particles_]
  Double_t        particles_fE[kMaxparticles];   //[particles_]
  Double_t        particles_fVx[kMaxparticles];   //[particles_]
  Double_t        particles_fVy[kMaxparticles];   //[particles_]
  Double_t        particles_fVz[kMaxparticles];   //[particles_]
  Double_t        particles_fVt[kMaxparticles];   //[particles_]
  Double_t        particles_fPolarTheta[kMaxparticles];   //[particles_]
  Double_t        particles_fPolarPhi[kMaxparticles];   //[particles_]

  // List of branches
  TBranch        *b_particles_;   //!
  TBranch        *b_particles_fUniqueID;   //!
  TBranch        *b_particles_fBits;   //!
  TBranch        *b_particles_fLineColor;   //!
  TBranch        *b_particles_fLineStyle;   //!
  TBranch        *b_particles_fLineWidth;   //!
  TBranch        *b_particles_fPdgCode;   //!
  TBranch        *b_particles_fStatusCode;   //!
  TBranch        *b_particles_fMother;   //!
  TBranch        *b_particles_fDaughter;   //!
  TBranch        *b_particles_fWeight;   //!
  TBranch        *b_particles_fCalcMass;   //!
  TBranch        *b_particles_fPx;   //!
  TBranch        *b_particles_fPy;   //!
  TBranch        *b_particles_fPz;   //!
  TBranch        *b_particles_fE;   //!
  TBranch        *b_particles_fVx;   //!
  TBranch        *b_particles_fVy;   //!
  TBranch        *b_particles_fVz;   //!
  TBranch        *b_particles_fVt;   //!
  TBranch        *b_particles_fPolarTheta;   //!
  TBranch        *b_particles_fPolarPhi;   //!

  ClassDef(PythiaEventReader,0)
};

#endif /* CAP__PythiaEventReader */
