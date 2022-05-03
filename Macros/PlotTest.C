
#include "TString.h"
#include "TRandom.h"
#include <vector>

int PlotTest()
{
  TString hName = "BE";
  TH1 * hNudynVsEta = new TH1D("h","h",100, 0.0, 1.6);
  TH1 * hNudynVsEtaScaled = new TH1D("hs","hs",100, 0.0, 1.6);

  hNudynVsEta->Fill(0.2, 0.008);
  hNudynVsEta->Fill(0.5, 0.0073);
  hNudynVsEta->Fill(1.0, 0.0063);
  hNudynVsEta->Fill(1.6, 0.006);

  hNudynVsEtaScaled->Fill(0.2, 0.008*0.2);
  hNudynVsEtaScaled->Fill(0.5, 0.0073*0.5);
  hNudynVsEtaScaled->Fill(1.0, 0.0063);
  hNudynVsEtaScaled->Fill(1.6, 0.006*1.6);

  for (int k=1;k<=100;k++) hNudynVsEta->SetBinError(k,0.0);

  for (int k=1;k<=100;k++) hNudynVsEtaScaled->SetBinError(k,0.0);



  TCanvas * c1 = new TCanvas("c1","c1",500,500);
  hNudynVsEta->SetMarkerColor(2);
  hNudynVsEta->SetMarkerSize(1.0);
  hNudynVsEta->SetMarkerStyle(21);
  hNudynVsEta->Draw("P");

  TCanvas * c2 = new TCanvas("c2","c2",500,500);
  hNudynVsEtaScaled->SetMarkerColor(2);
  hNudynVsEtaScaled->SetMarkerSize(1.0);
  hNudynVsEtaScaled->SetMarkerStyle(21);
  hNudynVsEtaScaled->Draw("P");



  return 0;
}


