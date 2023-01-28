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
#ifndef CAP__UrqmdEventReader
#define CAP__UrqmdEventReader
#include "RootTreeReader.hpp"

namespace CAP
{


class UrqmdEventReader : public RootTreeReader
{
public:

  //!
  //! Detailed CTOR
  //!
  //! @param _name Name given to task instance
  //! @param _configuration Configuration used to run this task
  //! @param _eventFilters Array of event filters to be used by this task (currently a placeholder not used)
  //! @param _particleFilters Array of particle filters to be used by this task -- use this filter to limit the range of particles read into CAP arrays.
  //! @param _reportLevel Message log level to be used by this task.
  //!
  UrqmdEventReader(const String & _name,
                   Configuration & _configuration,
                   vector<EventFilter*> & _eventFilters,
                   vector<ParticleFilter*> & _particleFilters);
  
  //!
  //! DTOR
  //!
  virtual ~UrqmdEventReader() {}

  //!
  //! Sets the default  values of the configuration parameters used by this task
  //!
  void setDefaultConfiguration();
  
  //!
  //! Read one AMPT event and insert it in the CAP event stream.
  //!
  void execute();
  
  //!
  //! Initialize the mapping between the branches and the storage variables
  //! used in EPOS data files.
  //!
  virtual void initInputTreeMapping();
  
  

protected:
  
  static constexpr Int_t kMaxfTracks = 50000;

  // Declaration of leaf types
//UrQMDEvent      *UrQMDEventBranch;
  UInt_t          fUniqueID;
  UInt_t          fBits;
  Int_t           fEvtNum;
  Float_t         fb;
  Int_t           fTracks_;
  UInt_t          fTracks_fUniqueID[kMaxfTracks];   //[fTracks_]
  UInt_t          fTracks_fBits[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_ft[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_fx[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_fy[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_fz[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_fE[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_fPx[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_fPy[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_fPz[kMaxfTracks];   //[fTracks_]
  Float_t         fTracks_fMass[kMaxfTracks];   //[fTracks_]
  UInt_t          fTracks_fChIso3Ityp[kMaxfTracks];   //[fTracks_]

  // List of branches
  TBranch        *b_UrQMDEventBranch_fUniqueID;   //!
  TBranch        *b_UrQMDEventBranch_fBits;   //!
  TBranch        *b_UrQMDEventBranch_fEvtNum;   //!
  TBranch        *b_UrQMDEventBranch_fb;   //!
  TBranch        *b_UrQMDEventBranch_fTracks_;   //!
  TBranch        *b_fTracks_fUniqueID;   //!
  TBranch        *b_fTracks_fBits;   //!
  TBranch        *b_fTracks_ft;   //!
  TBranch        *b_fTracks_fx;   //!
  TBranch        *b_fTracks_fy;   //!
  TBranch        *b_fTracks_fz;   //!
  TBranch        *b_fTracks_fE;   //!
  TBranch        *b_fTracks_fPx;   //!
  TBranch        *b_fTracks_fPy;   //!
  TBranch        *b_fTracks_fPz;   //!
  TBranch        *b_fTracks_fMass;   //!
  TBranch        *b_fTracks_fChIso3Ityp;   //!
                                           //!
};

} // namespace CAP

#endif /* CAP__UrqmdEventReader */
