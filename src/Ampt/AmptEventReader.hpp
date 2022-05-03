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
#ifndef WAC_AmptEventReader
#define WAC_AmptEventReader
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "Task.hpp"

class AmptEventReader : public Task
{
public:

  AmptEventReader(const TString &          _name,
                  Configuration &          _configuration,
                  vector<EventFilter*>   & _eventFilters,
                  vector<ParticleFilter*>& _particleFilters,
                  LogLevel                 _selectedLevel=Info);
  virtual ~AmptEventReader();
  virtual void initialize();
  virtual void reset();
  void execute();

protected:
  
  Int_t GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void Init(TTree *tree);
  Bool_t Notify();
  void Show(Long64_t entry);
  Int_t Cut(Long64_t entry);

  ////////////////////////////////////////////////////////////////////////////////////////
  // Data members
  ////////////////////////////////////////////////////////////////////////////////////////
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  Long64_t nentries;
  Long64_t nbytes;
  Long64_t nb;

  long jentry;

  static const int arraySize = 30000;

  // Declaration of leaf types
  Int_t           eventNo;
  Int_t           mult;
  Int_t           Nproj;
  Int_t           Ntarg;
  Float_t         impact;
  Int_t           Nparttotal;
  Int_t           pid[arraySize];   //[Mult]
  Float_t         px[arraySize];   //[Mult]
  Float_t         py[arraySize];   //[Mult]
  Float_t         pz[arraySize];   //[Mult]
  Float_t         m[arraySize];   //[Mult]
  Float_t         Nx[arraySize];   //[Mult]
  Float_t         Ny[arraySize];   //[Mult]

  // List of branches
  TBranch        *b_eventNo;   //!
  TBranch        *b_mult;   //!
  TBranch        *b_Nproj;   //!
  TBranch        *b_Ntarg;   //!
  TBranch        *b_impact;   //!
  TBranch        *b_Nparttotal;   //!
  TBranch        *b_pid;   //!
  TBranch        *b_px;   //!
  TBranch        *b_py;   //!
  TBranch        *b_pz;   //!
  TBranch        *b_m;   //!
  TBranch        *b_Nx;   //!
  TBranch        *b_Ny;   //!

  ClassDef(AmptEventReader,0)
};

#endif /* WAC_AmptEventReader */
