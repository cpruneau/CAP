void wtf()
{
  TFile  * fg = new TFile("/Volumes/ClaudeDisc4/OutputFiles/RhoDecayTest2/RhoDecay/RhoDecay_Part_Gen_Derived.root");
  TFile  * fr = new TFile("/Volumes/ClaudeDisc4/OutputFiles/RhoDecayTest2/RhoDecay/RhoDecay_Part_Reco_Derived.root");

  TH2F * r2Gen  = (TH2F*) fg->Get("Pair_Gen_All_HP_HM_R2_DetaDphi_shft");
  TH2F * r2Reco = (TH2F*) fg->Get("Pair_Reco_All_HP_HM_R2_DetaDphi_shft");

  TCanvas * c1 = new TCanvas();
  r2Gen->GetXaxis()->SetRangeUser(-1.5,1.5);
  r2Gen->Draw("SURF3");
}
