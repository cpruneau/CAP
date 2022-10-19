//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct  4 18:33:21 2022 by ROOT version 6.26/02
// from TTree TT/TT
// found on file: /Volumes/ClaudeDisc4/Simulations/pp13/Herwig7/File_10193004785.root
//////////////////////////////////////////////////////////

#ifndef TT_h
#define TT_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "TObject.h"

class TT {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxtracks = 94;

   // Declaration of leaf types
   Int_t           nTtracks;
   UInt_t          tracks_fUniqueID[kMaxtracks];   //[tracks_]
   UInt_t          tracks_fBits[kMaxtracks];   //[tracks_]
   Double_t        tracks_fPt[kMaxtracks];   //[tracks_]
   Double_t        tracks_fPhi[kMaxtracks];   //[tracks_]
   Double_t        tracks_fY[kMaxtracks];   //[tracks_]
   Double_t        tracks_fMass[kMaxtracks];   //[tracks_]
   Double_t        tracks_fEta[kMaxtracks];   //[tracks_]
   Int_t           tracks_fPDG[kMaxtracks];   //[tracks_]
   Short_t         tracks_fCharge[kMaxtracks];   //[tracks_]
   Short_t         tracks_fIsPrimary[kMaxtracks];   //[tracks_]
   Int_t           tracks_fStatus[kMaxtracks];   //[tracks_]
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

   TT(TTree *tree=0);
   virtual ~TT();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TT_cxx
TT::TT(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/Volumes/ClaudeDisc4/Simulations/pp13/Herwig7/File_10193004785.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/Volumes/ClaudeDisc4/Simulations/pp13/Herwig7/File_10193004785.root");
      }
      f->GetObject("TT",tree);

   }
   Init(tree);
}

TT::~TT()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TT::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TT::LoadTree(Long64_t entry)
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

void TT::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("tracks", &tracks_, &b_tracks_);
   fChain->SetBranchAddress("tracks.fUniqueID", tracks_fUniqueID, &b_tracks_fUniqueID);
   fChain->SetBranchAddress("tracks.fBits", tracks_fBits, &b_tracks_fBits);
   fChain->SetBranchAddress("tracks.fPt", tracks_fPt, &b_tracks_fPt);
   fChain->SetBranchAddress("tracks.fPhi", tracks_fPhi, &b_tracks_fPhi);
   fChain->SetBranchAddress("tracks.fY", tracks_fY, &b_tracks_fY);
   fChain->SetBranchAddress("tracks.fMass", tracks_fMass, &b_tracks_fMass);
   fChain->SetBranchAddress("tracks.fEta", tracks_fEta, &b_tracks_fEta);
   fChain->SetBranchAddress("tracks.fPDG", tracks_fPDG, &b_tracks_fPDG);
   fChain->SetBranchAddress("tracks.fCharge", tracks_fCharge, &b_tracks_fCharge);
   fChain->SetBranchAddress("tracks.fIsPrimary", tracks_fIsPrimary, &b_tracks_fIsPrimary);
   fChain->SetBranchAddress("tracks.fStatus", tracks_fStatus, &b_tracks_fStatus);
   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_event_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_event_fBits);
   fChain->SetBranchAddress("fV0A", &fV0A, &b_event_fV0A);
   fChain->SetBranchAddress("fV0C", &fV0C, &b_event_fV0C);
   fChain->SetBranchAddress("fCL1", &fCL1, &b_event_fCL1);
   Notify();
}

Bool_t TT::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TT::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TT::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TT_cxx
