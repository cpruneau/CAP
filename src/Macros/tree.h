//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May  5 12:54:14 2022 by ROOT version 6.26/02
// from TChain tree/
//////////////////////////////////////////////////////////

#ifndef tree_h
#define tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           eventNo;
   Int_t           mult;
   Int_t           Nproj;
   Int_t           Ntarg;
   Float_t         impact;
   Int_t           Nparttotal;
   Int_t           pid[31193];   //[mult]
   Float_t         px[31193];   //[mult]
   Float_t         py[31193];   //[mult]
   Float_t         pz[31193];   //[mult]
   Float_t         m[31193];   //[mult]
   Float_t         Nx[414];   //[Nparttotal]
   Float_t         Ny[414];   //[Nparttotal]

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

   tree(TTree *tree=0);
   virtual ~tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tree_cxx
tree::tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("tree",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("tree","");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_01.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_010.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0100.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0101.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0102.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0103.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0104.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0105.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0106.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0107.root/tree");
      chain->Add("/Volumes/ClaudeDisc4/Simulations/ampt/PbPb_SM_1_2760GeV_0108.root/tree");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

tree::~tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tree::LoadTree(Long64_t entry)
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

void tree::Init(TTree *tree)
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
}

Bool_t tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tree_cxx
