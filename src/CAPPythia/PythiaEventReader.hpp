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

namespace CAP
{


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
  PythiaEventReader(const String & _name,
                    Configuration & _configuration,
                    vector<EventFilter*>&   _eventFilters,
                    vector<ParticleFilter*>&_particleFilters);
  
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
  //! Initialize the mapping of Root Tree Branches onto local variables.
  //!
  virtual void initInputTreeMapping();
  
protected:

  static const int nMaxTracks = 5000;
  
  // Declaration of leaf types
//  Int_t           nParticles;
//  UInt_t          particles_fUniqueID[kMaxparticles];   //[particles_]
//  UInt_t          particles_fBits[kMaxparticles];   //[particles_]
//  Short_t         particles_fLineColor[kMaxparticles];   //[particles_]
//  Short_t         particles_fLineStyle[kMaxparticles];   //[particles_]
//  Short_t         particles_fLineWidth[kMaxparticles];   //[particles_]
//  Int_t           particles_fPdgCode[kMaxparticles];   //[particles_]
//  Int_t           particles_fStatusCode[kMaxparticles];   //[particles_]
//  Int_t           particles_fMother[kMaxparticles][2];   //[particles_]
//  Int_t           particles_fDaughter[kMaxparticles][2];   //[particles_]
//  Float_t         particles_fWeight[kMaxparticles];   //[particles_]
//  Double_t        particles_fCalcMass[kMaxparticles];   //[particles_]
//  Double_t        particles_fPx[kMaxparticles];   //[particles_]
//  Double_t        particles_fPy[kMaxparticles];   //[particles_]
//  Double_t        particles_fPz[kMaxparticles];   //[particles_]
//  Double_t        particles_fE[kMaxparticles];   //[particles_]
//  Double_t        particles_fVx[kMaxparticles];   //[particles_]
//  Double_t        particles_fVy[kMaxparticles];   //[particles_]
//  Double_t        particles_fVz[kMaxparticles];   //[particles_]
//  Double_t        particles_fVt[kMaxparticles];   //[particles_]
//  Double_t        particles_fPolarTheta[kMaxparticles];   //[particles_]
//  Double_t        particles_fPolarPhi[kMaxparticles];   //[particles_]
//
//  // List of branches
//  TBranch        *b_particles_;   //!
//  TBranch        *b_particles_fUniqueID;   //!
//  TBranch        *b_particles_fBits;   //!
//  TBranch        *b_particles_fLineColor;   //!
//  TBranch        *b_particles_fLineStyle;   //!
//  TBranch        *b_particles_fLineWidth;   //!
//  TBranch        *b_particles_fPdgCode;   //!
//  TBranch        *b_particles_fStatusCode;   //!
//  TBranch        *b_particles_fMother;   //!
//  TBranch        *b_particles_fDaughter;   //!
//  TBranch        *b_particles_fWeight;   //!
//  TBranch        *b_particles_fCalcMass;   //!
//  TBranch        *b_particles_fPx;   //!
//  TBranch        *b_particles_fPy;   //!
//  TBranch        *b_particles_fPz;   //!
//  TBranch        *b_particles_fE;   //!
//  TBranch        *b_particles_fVx;   //!
//  TBranch        *b_particles_fVy;   //!
//  TBranch        *b_particles_fVz;   //!
//  TBranch        *b_particles_fVt;   //!
//  TBranch        *b_particles_fPolarTheta;   //!
//  TBranch        *b_particles_fPolarPhi;   //!

  Int_t           nParticles;
  UInt_t          tracks_fUniqueID[nMaxTracks];   //[tracks_]
  UInt_t          tracks_fBits[nMaxTracks];   //[tracks_]
  Short_t         tracks_fLineColor[nMaxTracks];   //[tracks_]
  Short_t         tracks_fLineStyle[nMaxTracks];   //[tracks_]
  Short_t         tracks_fLineWidth[nMaxTracks];   //[tracks_]
  Int_t           tracks_fPdgCode[nMaxTracks];   //[tracks_]
  Int_t           tracks_fStatusCode[nMaxTracks];   //[tracks_]
  Int_t           tracks_fMother[nMaxTracks][2];   //[tracks_]
  Int_t           tracks_fDaughter[nMaxTracks][2];   //[tracks_]
  Float_t         tracks_fWeight[nMaxTracks];   //[tracks_]
  Double_t        tracks_fCalcMass[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPx[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPy[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPz[nMaxTracks];   //[tracks_]
  Double_t        tracks_fE[nMaxTracks];   //[tracks_]
  Double_t        tracks_fVx[nMaxTracks];   //[tracks_]
  Double_t        tracks_fVy[nMaxTracks];   //[tracks_]
  Double_t        tracks_fVz[nMaxTracks];   //[tracks_]
  Double_t        tracks_fVt[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPolarTheta[nMaxTracks];   //[tracks_]
  Double_t        tracks_fPolarPhi[nMaxTracks];   //[tracks_]
  Int_t           HP_Inc1;
  Int_t           HP_Inc2;
  Int_t           HP_Inc3;
  Int_t           HP_Inc4;
  Int_t           HP_Out1;
  Int_t           HP_Out2;
  Int_t           HP_Out3;
  Int_t           HP_Out4;
  Int_t           HP_Inter1;
  Int_t           HP_Inter2;
  Int_t           HP_Inter3;
  Int_t           HP_Inter4;
  Int_t           HP_Kick1;
  Int_t           HP_Kick2;
  Int_t           HP_Kick3;
  Int_t           HP_Kick4;

  // List of branches
  TBranch        *b_tracks_;   //!
  TBranch        *b_tracks_fUniqueID;   //!
  TBranch        *b_tracks_fBits;   //!
  TBranch        *b_tracks_fLineColor;   //!
  TBranch        *b_tracks_fLineStyle;   //!
  TBranch        *b_tracks_fLineWidth;   //!
  TBranch        *b_tracks_fPdgCode;   //!
  TBranch        *b_tracks_fStatusCode;   //!
  TBranch        *b_tracks_fMother;   //!
  TBranch        *b_tracks_fDaughter;   //!
  TBranch        *b_tracks_fWeight;   //!
  TBranch        *b_tracks_fCalcMass;   //!
  TBranch        *b_tracks_fPx;   //!
  TBranch        *b_tracks_fPy;   //!
  TBranch        *b_tracks_fPz;   //!
  TBranch        *b_tracks_fE;   //!
  TBranch        *b_tracks_fVx;   //!
  TBranch        *b_tracks_fVy;   //!
  TBranch        *b_tracks_fVz;   //!
  TBranch        *b_tracks_fVt;   //!
  TBranch        *b_tracks_fPolarTheta;   //!
  TBranch        *b_tracks_fPolarPhi;   //!
  TBranch        *b_HP_Inc1;   //!
  TBranch        *b_HP_Inc2;   //!
  TBranch        *b_HP_Inc3;   //!
  TBranch        *b_HP_Inc4;   //!
  TBranch        *b_HP_Out1;   //!
  TBranch        *b_HP_Out2;   //!
  TBranch        *b_HP_Out3;   //!
  TBranch        *b_HP_Out4;   //!
  TBranch        *b_HP_Inter1;   //!
  TBranch        *b_HP_Inter2;   //!
  TBranch        *b_HP_Inter3;   //!
  TBranch        *b_HP_Inter4;   //!
  TBranch        *b_HP_Kick1;   //!
  TBranch        *b_HP_Kick2;   //!
  TBranch        *b_HP_Kick3;   //!
  TBranch        *b_HP_Kick4;   //!


  ClassDef(PythiaEventReader,0)
};

}

#endif /* CAP__PythiaEventReader */
