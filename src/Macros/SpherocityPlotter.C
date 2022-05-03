// ROOT clases
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TString.h>
#include <TProfile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TObject.h>
#include <TMath.h>
#include <TTimeStamp.h>

// Analysiss
#include "AliAnalysisPIDCascadeEvent.h"
#include "AliAnalysisPIDCascadeTrack.h"
#include "AliAnalysisPIDCascadeParticle.h"
#include <AliTOFPIDResponse.h>

#include "TransverseSpherocity.h"

// Help classes
#include "tools.C"

#include <iostream>
#include <string>

using namespace std;

Double_t GetDphi(Double_t phi);

void SpherPlotter(const Char_t* inFileName,
		  const Char_t* outFileName,
		  const Int_t maxEvents=0000000){
 
  TTree* tree = 0;
  
  if(strstr(inFileName, ".dat")) {
    
    tree = ReadChainFromFile(inFileName, "PIDTree", 0);
    tree->SetBranchStatus("AnalysisV0Track*" , 0);
  } else {
    TFile* inFile = TFile::Open(inFileName);
    if(!inFile)
      return;
  }

  
  
  TClonesArray* trackArray = 0;
  TClonesArray* GentrackArray = 0;
  AliAnalysisPIDCascadeEvent* event = 0;
  tree->SetBranchAddress("AnalysisEvent", &event);
  tree->SetBranchAddress("AnalysisTrack", &trackArray);

  TFile* file = new TFile(outFileName, "RECREATE");
  file->cd();
  /////////////////////////////////////////////////////////////////////////////
  TH1D* hSpherCL1_10 = new TH1D("hSpherCL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpherCL1_10->Sumw2();


  TH1D* hSpherV0M_10 = new TH1D("hSpherV0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpherV0M_10->Sumw2();

  TH1D* hSpherCL1_1 = new TH1D("hSpherCL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpherCL1_1->Sumw2();


  TH1D* hSpherV0M_1 = new TH1D("hSpherV0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpherV0M_1->Sumw2();
/////////////////////////////////////////////////////////////////////////////
    TH1D* hSpher_Jetty_01_CL1_10 = new TH1D("hSpher_Jetty_01_CL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_01_CL1_10->Sumw2();


  TH1D* hSpher_Jetty_01_V0M_10 = new TH1D("hSpher_Jetty_01_V0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_01_V0M_10->Sumw2();

  TH1D* hSpher_Jetty_01_CL1_1 = new TH1D("hSpher_Jetty_01_CL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_01_CL1_1->Sumw2();


  TH1D* hSpher_Jetty_01_V0M_1 = new TH1D("hSpher_Jetty_01_V0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_01_V0M_1->Sumw2();
  //
  TH1D* hSpher_Jetty_05_CL1_10 = new TH1D("hSpher_Jetty_05_CL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_05_CL1_10->Sumw2();


  TH1D* hSpher_Jetty_05_V0M_10 = new TH1D("hSpher_Jetty_05_V0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_05_V0M_10->Sumw2();

  TH1D* hSpher_Jetty_05_CL1_1 = new TH1D("hSpher_Jetty_05_CL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_05_CL1_1->Sumw2();


  TH1D* hSpher_Jetty_05_V0M_1 = new TH1D("hSpher_Jetty_05_V0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_05_V0M_1->Sumw2();
  //
 TH1D* hSpher_Jetty_10_CL1_10 = new TH1D("hSpher_Jetty_10_CL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_10_CL1_10->Sumw2();


  TH1D* hSpher_Jetty_10_V0M_10 = new TH1D("hSpher_Jetty_10_V0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_10_V0M_10->Sumw2();

  TH1D* hSpher_Jetty_10_CL1_1 = new TH1D("hSpher_Jetty_10_CL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_10_CL1_1->Sumw2();


  TH1D* hSpher_Jetty_10_V0M_1 = new TH1D("hSpher_Jetty_10_V0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_10_V0M_1->Sumw2();
  //
TH1D* hSpher_Jetty_20_CL1_10 = new TH1D("hSpher_Jetty_20_CL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_20_CL1_10->Sumw2();


  TH1D* hSpher_Jetty_20_V0M_10 = new TH1D("hSpher_Jetty_20_V0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_20_V0M_10->Sumw2();

  TH1D* hSpher_Jetty_20_CL1_1 = new TH1D("hSpher_Jetty_20_CL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_20_CL1_1->Sumw2();


  TH1D* hSpher_Jetty_20_V0M_1 = new TH1D("hSpher_Jetty_20_V0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Jetty_20_V0M_1->Sumw2();
  //
/////////////////////////////////////////////////////////////////////////////

  TH1D* hSpher_Iso_01_CL1_10 = new TH1D("hSpher_Iso_01_CL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_01_CL1_10->Sumw2();


  TH1D* hSpher_Iso_01_V0M_10 = new TH1D("hSpher_Iso_01_V0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_01_V0M_10->Sumw2();

  TH1D* hSpher_Iso_01_CL1_1 = new TH1D("hSpher_Iso_01_CL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_01_CL1_1->Sumw2();


  TH1D* hSpher_Iso_01_V0M_1 = new TH1D("hSpher_Iso_01_V0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_01_V0M_1->Sumw2();
  //
  TH1D* hSpher_Iso_05_CL1_10 = new TH1D("hSpher_Iso_05_CL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_05_CL1_10->Sumw2();


  TH1D* hSpher_Iso_05_V0M_10 = new TH1D("hSpher_Iso_05_V0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_05_V0M_10->Sumw2();

  TH1D* hSpher_Iso_05_CL1_1 = new TH1D("hSpher_Iso_05_CL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_05_CL1_1->Sumw2();


  TH1D* hSpher_Iso_05_V0M_1 = new TH1D("hSpher_Iso_05_V0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_05_V0M_1->Sumw2();
  //
 TH1D* hSpher_Iso_10_CL1_10 = new TH1D("hSpher_Iso_10_CL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_10_CL1_10->Sumw2();


  TH1D* hSpher_Iso_10_V0M_10 = new TH1D("hSpher_Iso_10_V0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_10_V0M_10->Sumw2();

  TH1D* hSpher_Iso_10_CL1_1 = new TH1D("hSpher_Iso_10_CL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_10_CL1_1->Sumw2();


  TH1D* hSpher_Iso_10_V0M_1 = new TH1D("hSpher_Iso_10_V0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_10_V0M_1->Sumw2();
  //
TH1D* hSpher_Iso_20_CL1_10 = new TH1D("hSpher_Iso_20_CL1_10", "SpherCL1_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_20_CL1_10->Sumw2();


  TH1D* hSpher_Iso_20_V0M_10 = new TH1D("hSpher_Iso_20_V0M_10", "SpherV0M_10; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_20_V0M_10->Sumw2();

  TH1D* hSpher_Iso_20_CL1_1 = new TH1D("hSpher_Iso_20_CL1_1", "SpherCL1_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_20_CL1_1->Sumw2();


  TH1D* hSpher_Iso_20_V0M_1 = new TH1D("hSpher_Iso_20_V0M_1", "SpherV0M_1; Spher; Counts",
			     1000, 0, 1);
  hSpher_Iso_20_V0M_1->Sumw2();
  /////////////////////////////////////////////////////////////////////////////

  TH1D* hdndetaCL1_10 = new TH1D("hdndetaCL1_10", "dndetaCL1_10; dndeta; Counts",
				 100, -0.5, 99.5);
  hdndetaCL1_10->Sumw2();

  TH1D* hdndetaCL1_10_Jetty = new TH1D("hdndetaCL1_10_Jetty", "dndetaCL1_10; dndeta; Counts",
				       100, -0.5, 99.5);
  hdndetaCL1_10_Jetty->Sumw2();

  TH1D* hdndetaCL1_10_Iso = new TH1D("hdndetaCL1_10_Iso", "dndetaCL1_10; dndeta; Counts",
				     100, -0.5, 99.5);
  hdndetaCL1_10_Iso->Sumw2();

  TH1D* hdndetaCL1_01 = new TH1D("hdndetaCL1_01", "dndetaCL1_01; dndeta; Counts",
				 100, -0.5, 99.5);
  hdndetaCL1_01->Sumw2();

  TH1D* hdndetaCL1_01_Jetty = new TH1D("hdndetaCL1_01_Jetty", "dndetaCL1_01; dndeta; Counts",
				       100, -0.5, 99.5);
  hdndetaCL1_01_Jetty->Sumw2();

  TH1D* hdndetaCL1_01_Iso = new TH1D("hdndetaCL1_01_Iso", "dndetaCL1_01; dndeta; Counts",
				     100, -0.5, 99.5);
  hdndetaCL1_01_Iso->Sumw2();


  TH1D* hdndetaV0M_10 = new TH1D("hdndetaV0M_10", "dndetaV0M_10; dndeta; Counts",
				 100, -0.5, 99.5);
  hdndetaV0M_10->Sumw2();

  TH1D* hdndetaV0M_10_Jetty = new TH1D("hdndetaV0M_10_Jetty", "dndetaV0M_10; dndeta; Counts",
				       100, -0.5, 99.5);
  hdndetaV0M_10_Jetty->Sumw2();

  TH1D* hdndetaV0M_10_Iso = new TH1D("hdndetaV0M_10_Iso", "dndetaV0M_10; dndeta; Counts",
				     100, -0.5, 99.5);
  hdndetaV0M_10_Iso->Sumw2();

  TH1D* hdndetaV0M_01 = new TH1D("hdndetaV0M_01", "dndetaV0M_01; dndeta; Counts",
				 100, -0.5, 99.5);
  hdndetaV0M_01->Sumw2();

  TH1D* hdndetaV0M_01_Jetty = new TH1D("hdndetaV0M_01_Jetty", "dndetaV0M_01; dndeta; Counts",
				       100, -0.5, 99.5);
  hdndetaV0M_01_Jetty->Sumw2();

  TH1D* hdndetaV0M_01_Iso = new TH1D("hdndetaV0M_01_Iso", "dndetaV0M_01; dndeta; Counts",
				     100, -0.5, 99.5);
  hdndetaV0M_01_Iso->Sumw2();



  TransverseSpherocity* ts = new TransverseSpherocity();
  ts->SetMinMulti(10);

  AliAnalysisPIDCascadeEvent::SetVertexZCuts(-10, 10);
  AliAnalysisPIDCascadeEvent::SetCheckFlag(325); // Vytautas cuts
  AliAnalysisPIDCascadeEvent::PrintEventSelection();
  const Int_t nV0signals = 64;
  
  /////////////////////////////////////////////////////////////////////////////71770950  73821167
  //Event Loop
  /////////////////////////////////////////////////////////////////////////////66968600
  Int_t nEvents = tree->GetEntries();
  cout << "Number of events: " << nEvents << endl;
 if(maxEvents>0 && maxEvents < nEvents) {   
    nEvents = maxEvents;
    cout << "N events was reduced to: " << maxEvents << endl;
  } 

  for(Int_t n =0; n < nEvents; n++) {

    Int_t multflag = 0;    

    tree->GetEntry(n);
    if((n+1)%100000==0)
      {
	TTimeStamp* time = new TTimeStamp();
	cout << outFileName<<" |  Event: " << n+1 << "/" << nEvents<<",   Date: "<<time->GetDate()<<"  Time: "<<time->GetTime()<< endl;
      }
    
    AliAnalysisPIDCascadeEvent::SetCheckFlag(5); // pile up only
    Int_t vtxStatus = event->AcceptEvent(kFALSE); // kFALSE means do not apply vertex cut
    if(vtxStatus == 0) 
      vtxStatus = -2;
    else {
    
      AliAnalysisPIDCascadeEvent::SetCheckFlag(511); // MB Cuts 511 is all, 325 is MB
      vtxStatus = event->AcceptEvent(kFALSE); // kFALSE means do not apply vertex cut
      if(vtxStatus == 0) 
	vtxStatus = -1;
      else {
	vtxStatus = event->AcceptEvent(kTRUE);
	if(vtxStatus == 0)
	  vtxStatus = 0;
	else
	  vtxStatus = event->AcceptEvent(kFALSE);
      }
      
    }


    if(vtxStatus == -2) // pile up
      continue;
        
    if(vtxStatus < 1) // event was rejected
      continue;
    
    ts->Reset();
    const Int_t nTracks = trackArray->GetEntries();
    for(Int_t i = 0; i < nTracks; i++) {
	
      AliAnalysisPIDCascadeTrack* track = (AliAnalysisPIDCascadeTrack*)trackArray->At(i);	
      // use TPC tracks for spherocity
      if(!AcceptTrackTools(track, 4)) //TrackFlag4 = TPCOnly+TPCRefit
	continue;
      if((track->GetStatus()&AliESDtrack::kITSrefit)!=AliESDtrack::kITSrefit)
	{
	  continue;
	}

      ts->AddTrack(1.0*TMath::Cos(track->GetPhi()),
		   1.0*TMath::Sin(track->GetPhi()));


    }//Track Loop For S0 Calc
    
    Double_t s0tracks = ts->GetTransverseSpherocityTracks();
      if(s0tracks<0)
	continue;
      if(event->GetV0Mmultiplicity()<=10){
	hSpherV0M_10->Fill(s0tracks);
	if(s0tracks<=0.357)
	  hSpher_Jetty_01_V0M_10->Fill(s0tracks);
	if(s0tracks<=0.470)
	  hSpher_Jetty_05_V0M_10->Fill(s0tracks);
	if(s0tracks<=0.529)
	  hSpher_Jetty_10_V0M_10->Fill(s0tracks);
	if(s0tracks<=0.600)
	  hSpher_Jetty_20_V0M_10->Fill(s0tracks);
	//
	if(s0tracks>=0.927)
	  hSpher_Iso_01_V0M_10->Fill(s0tracks);
	if(s0tracks>=0.891)
	  hSpher_Iso_05_V0M_10->Fill(s0tracks);
	if(s0tracks>=0.864)
	  hSpher_Iso_10_V0M_10->Fill(s0tracks);
	if(s0tracks>=0.823)
	  hSpher_Iso_20_V0M_10->Fill(s0tracks);
      }//V0M 0-10%
      if(event->GetV0Mmultiplicity()<=1){
	hSpherV0M_1->Fill(s0tracks);
	if(s0tracks<=0.433)
	  hSpher_Jetty_01_V0M_1->Fill(s0tracks);
	if(s0tracks<=0.535)
	  hSpher_Jetty_05_V0M_1->Fill(s0tracks);
	if(s0tracks<=0.589)
	  hSpher_Jetty_10_V0M_1->Fill(s0tracks);
	if(s0tracks<=0.651)
	  hSpher_Jetty_20_V0M_1->Fill(s0tracks);
	//
	if(s0tracks>=0.936)
	  hSpher_Iso_01_V0M_1->Fill(s0tracks);
	if(s0tracks>=0.905)
	  hSpher_Iso_05_V0M_1->Fill(s0tracks);
	if(s0tracks>=0.882)
	  hSpher_Iso_10_V0M_1->Fill(s0tracks);
	if(s0tracks>=0.846)
	  hSpher_Iso_20_V0M_1->Fill(s0tracks);
      }//V0M 0-1%
      if(event->GetRefMult08()<=10){
	hSpherCL1_10->Fill(s0tracks);
	if(s0tracks<=0.408)
	  hSpher_Jetty_01_CL1_10->Fill(s0tracks);
	if(s0tracks<=0.508)
	  hSpher_Jetty_05_CL1_10->Fill(s0tracks);
	if(s0tracks<=0.561)
	  hSpher_Jetty_10_CL1_10->Fill(s0tracks);
	if(s0tracks<=0.625)
	  hSpher_Jetty_20_CL1_10->Fill(s0tracks);
	//
	if(s0tracks>=0.93)
	  hSpher_Iso_01_CL1_10->Fill(s0tracks);
	if(s0tracks>=0.896)
	  hSpher_Iso_05_CL1_10->Fill(s0tracks);
	if(s0tracks>=0.871)
	  hSpher_Iso_10_CL1_10->Fill(s0tracks);
	if(s0tracks>=0.833)
	  hSpher_Iso_20_CL1_10->Fill(s0tracks);
      }//CL1 0-10%
      if(event->GetRefMult08()<=1){
	hSpherCL1_1->Fill(s0tracks);
	if(s0tracks<=0.487)
	  hSpher_Jetty_01_CL1_1->Fill(s0tracks);
	if(s0tracks<=0.577)
	  hSpher_Jetty_05_CL1_1->Fill(s0tracks);
	if(s0tracks<=0.624)
	  hSpher_Jetty_10_CL1_1->Fill(s0tracks);
	if(s0tracks<=0.680)
	  hSpher_Jetty_20_CL1_1->Fill(s0tracks);
	//
	if(s0tracks>=0.942)
	  hSpher_Iso_01_CL1_1->Fill(s0tracks);
	if(s0tracks>=0.913)
	  hSpher_Iso_05_CL1_1->Fill(s0tracks);
	if(s0tracks>=0.892)
	  hSpher_Iso_10_CL1_1->Fill(s0tracks);
	if(s0tracks>=0.859)
	  hSpher_Iso_20_CL1_1->Fill(s0tracks);
      }//CL1 0-10%


      Int_t nAcc=0;
      for(Int_t i = 0; i < nTracks; i++) {
	  
	AliAnalysisPIDCascadeTrack* track = (AliAnalysisPIDCascadeTrack*)trackArray->At(i);	
	if(!AcceptTrackTools(track, 2)) //TrackFlag4 = TPCOnly+TPCRefit
	  continue;
	nAcc++;
    }//Track Loop For Mult
      if(event->GetV0Mmultiplicity()<=10){
	hdndetaV0M_10->Fill(nAcc);
	if(s0tracks<0.357)
	  hdndetaV0M_10_Jetty->Fill(nAcc);
      	if(s0tracks>0.927)
	  hdndetaV0M_10_Iso->Fill(nAcc);
      
	if(event->GetV0Mmultiplicity()<=1){
	  hdndetaV0M_01->Fill(nAcc);
	  if(s0tracks<0.433)
	    hdndetaV0M_01_Jetty->Fill(nAcc);
	  if(s0tracks>0.936)
	    hdndetaV0M_01_Iso->Fill(nAcc);
	}
      }
/////////////////////////////////////////////////////////////////////////////
    if(event->GetRefMult08()<=10){
      hdndetaCL1_10->Fill(nAcc);
	if(s0tracks<0.408)
	  hdndetaCL1_10_Jetty->Fill(nAcc);
      	if(s0tracks>0.93)
	  hdndetaCL1_10_Iso->Fill(nAcc);
      
	if(event->GetRefMult08()<=1){
	  hdndetaCL1_01->Fill(nAcc);
	  if(s0tracks<0.487)
	    hdndetaCL1_01_Jetty->Fill(nAcc);
	  if(s0tracks>0.942)
	    hdndetaCL1_01_Iso->Fill(nAcc);
	}
      }

  }//Event loop

  file->Write();
  file->Close();

}
