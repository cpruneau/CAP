void wtf()
{
  TFile  * fg = new TFile("/Volumes/ClaudeDisc4/OutputFiles/RhoDecayTest2/RhoDecay/RhoDecay_Pair_Gen_Derived.root");
  TFile  * fr = new TFile("/Volumes/ClaudeDisc4/OutputFiles/RhoDecayTest2/RhoDecay/RhoDecay_Pair_Reco_Derived.root");

  TH2F * r2Gen  = (TH2F*) fg->Get("Pair_Gen_All_HP_HM_R2_DetaDphi_shft");
  TH2F * r2Reco = (TH2F*) fr->Get("Pair_Reco_All_HP_HM_R2_DetaDphi_shft");

  TCanvas * c1 = new TCanvas();
  if (r2Gen)
    {
    r2Gen->GetXaxis()->SetRangeUser(-1.5,1.5);
    r2Gen->Draw("SURF3");
    }
  else
    {
    cout << "no can do" << endl;
    }

  TCanvas * c2 = new TCanvas();
  if (r2Reco)
    {
    r2Reco->GetXaxis()->SetRangeUser(-1.5,1.5);
    r2Reco->Draw("SURF3");
    }
  else
    {
    cout << "no can do" << endl;
    }
  TCanvas * c3 = new TCanvas();
  TH2F * diff = new TH2F(*r2Gen);
  diff->Add(r2Reco,-1.0);
  diff->Draw("SURF3");

  TCanvas * c4 = new TCanvas();
  TH2F * ratio = new TH2F(*r2Gen);
  ratio->Divide(r2Gen,r2Reco,1.0,1.0);
  ratio->Draw("SURF3");


  //
}
