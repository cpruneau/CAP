
#include "TString.h"
#include "TRandom.h"

int PlotPythiaSpherocity()
{
  TString includesPath = getenv("CAP_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load("libBase.dylib");

  bool color = true;
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);

  CanvasConfiguration * landscapeLinear     = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  GraphConfiguration * gc = new GraphConfiguration(1,1);

  TString inputPathName  = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA_/13000/";
  TString outputPathName = "/Volumes/ClaudeDisc4/OutputFiles/PYTHIA_/13000/";
  TString inputFileName  =  inputPathName;
  TString outputFileName;

  inputFileName  += "PYTHIA_S.root";
  TFile * file = new TFile(inputFileName,"OLD");

  TH1 * S_V0MB_TpcAll_S0 = (TH1 *) file->Get("S_V0MB_TpcAll_S0");
  TH1 * S_V0MB_V0M_S0    = (TH1 *) file->Get("S_V0MB_V0M_S0");
  TH1 * S_V0MB_All_S0    = (TH1 *) file->Get("S_V0MB_All_S0");
  TH1 * S_V0MB_CH_S0     = (TH1 *) file->Get("S_V0MB_CH_S0");
  
  TCanvas * c1 = new TCanvas();
  S_V0MB_All_S0->SetLineColor(2);
  S_V0MB_All_S0->SetLineWidth(2);
  S_V0MB_All_S0->SetMarkerStyle(20);
  S_V0MB_All_S0->SetMarkerColor(2);
  S_V0MB_All_S0->Draw();
  
  S_V0MB_CH_S0->SetLineColor(4);
  S_V0MB_CH_S0->SetLineWidth(4);
  S_V0MB_CH_S0->SetMarkerStyle(21);
  S_V0MB_CH_S0->SetMarkerColor(4);
  S_V0MB_CH_S0->Draw("SAME");

  S_V0MB_TpcAll_S0->SetLineColor(6);
  S_V0MB_TpcAll_S0->SetLineWidth(6);
  S_V0MB_TpcAll_S0->SetMarkerStyle(22);
  S_V0MB_TpcAll_S0->SetMarkerColor(6);
  S_V0MB_TpcAll_S0->Draw("SAME");

  outputFileName = "S0_All_CH_TPConly_MinBias.pdf";
  c1->Print(outputPathName+outputFileName);

  TH2 * S_V0MB_TpcAllAll_S0VsS0    = (TH2 *) file->Get("S_V0MB_TpcAllAll_S0VsS0");
  S_V0MB_TpcAllAll_S0VsS0->Draw("ZCOL");
  outputFileName = "S_V0MB_TpcAllAll_S0VsS0.pdf";
  c1->Print(outputPathName+outputFileName);

  TH2 * S_V0MB_AllCH_S0VsS0    = (TH2 *) file->Get("S_V0MB_AllCH_S0VsS0");
  S_V0MB_AllCH_S0VsS0->Draw("ZCOL");
  outputFileName = "S_V0MB_AllCH_S0VsS0.pdf";
  c1->Print(outputPathName+outputFileName);

  return 0;
}

