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
#ifndef CAP__HerwigEventReader
#define CAP__HerwigEventReader
#include "RootTreeReader.hpp"

namespace CAP
{


//!
//! This class defines tasks capable of reading Herwig data file.
//!
//!
class HerwigEventReader : public RootTreeReader
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
  HerwigEventReader(const String &          _name,
                  Configuration &          _configuration,
                  vector<EventFilter*>   & _eventFilters,
                  vector<ParticleFilter*>& _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~HerwigEventReader() {}
  
  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  void setDefaultConfiguration();
  
  //!
  //! Read one Herwig event and insert it in the CAP event stream.
  //!
  void execute();

  //!
  //! Initialize the mapping between the branches and the storage variables
  //! used in Herwig data files.
  //!
  virtual void initInputTreeMapping();

protected:
  

  static const int arraySize = 50000;
  
  // Declaration of leaf types
  Int_t           nTracks;
  UInt_t          tracks_fUniqueID[arraySize];   //[tracks_]
  UInt_t          tracks_fBits[arraySize];   //[tracks_]
  Double_t        tracks_fPt[arraySize];   //[tracks_]
  Double_t        tracks_fPhi[arraySize];   //[tracks_]
  Double_t        tracks_fY[arraySize];   //[tracks_]
  Double_t        tracks_fMass[arraySize];   //[tracks_]
  Double_t        tracks_fEta[arraySize];   //[tracks_]
  Int_t           tracks_fPDG[arraySize];   //[tracks_]
  Short_t         tracks_fCharge[arraySize];   //[tracks_]
  Short_t         tracks_fIsPrimary[arraySize];   //[tracks_]
  Int_t           tracks_fStatus[arraySize];   //[tracks_]
  //MyEvent         *event;
  UInt_t          fUniqueID;
  UInt_t          fBits;
  Int_t           fV0A;
  Int_t           fV0C;
  Int_t           fCL1;

  // List of branches
  TBranch        *b_tracks_;   //!
  TBranch        *b_tracks_fUniqueID;   //!
  TBranch        *b_tracks_fBits;   //!
  TBranch        *b_tracks_fPt;   //!
  TBranch        *b_tracks_fPhi;   //!
  TBranch        *b_tracks_fY;   //!
  TBranch        *b_tracks_fMass;   //!
  TBranch        *b_tracks_fEta;   //!
  TBranch        *b_tracks_fPDG;   //!
  TBranch        *b_tracks_fCharge;   //!
  TBranch        *b_tracks_fIsPrimary;   //!
  TBranch        *b_tracks_fStatus;   //!
  TBranch        *b_event_fUniqueID;   //!
  TBranch        *b_event_fBits;   //!
  TBranch        *b_event_fV0A;   //!
  TBranch        *b_event_fV0C;   //!
  TBranch        *b_event_fCL1;   //!

  ClassDef(HerwigEventReader,0)
};

} // namespace CAP

#endif /* CAP_HerwigEventReader */
